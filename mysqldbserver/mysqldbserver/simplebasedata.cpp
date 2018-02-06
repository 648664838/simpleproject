#include "simplebasedata.h"
#include "tinyxml.h"
#include <iostream>
#include <stdarg.h>
#include "cscenetype.h"
using namespace std;

CDataBase::CDataBase()
{
	// Mysql Library Init
	mysql_library_init(-1, NULL, NULL);
	mMySql = NULL;
}

CDataBase::~CDataBase()
{
	mysql_close(mMySql);
	mysql_library_end();
}

bool CDataBase::Initialize(const char * pPath)
{
	if (pPath == NULL)
	{
		cout << "CDataBase::Initialize : pPath == NULL" << endl;
		return false;
	}
	TiXmlDocument tDoc;
	if (tDoc.LoadFile(pPath) == false)
	{
		cout << "CDataBase::Initialize : tDoc false" << endl;
		return false;
	}

	TiXmlElement * pRoot = tDoc.RootElement();
	if (pRoot == NULL)
	{
		cout << "CDataBase::Initialize : pRoot NULL" << endl;
		return false;
	}

	TiXmlElement * pXmlBaseData = pRoot->FirstChildElement("baseinfo");
	if (pXmlBaseData == NULL)
	{
		cout << "CDataBase::Initialize : pXmlBaseData NULL" << endl;
		return false;
	} 
	if (pXmlBaseData->Attribute("host")     == NULL     ||
		pXmlBaseData->Attribute("port")     == NULL     ||
		pXmlBaseData->Attribute("user")     == NULL     ||
		pXmlBaseData->Attribute("password") == NULL     ||
		pXmlBaseData->Attribute("database") == NULL)
	{
		cout << "CDataBase::Initialize : attr NULL" << endl;
		return false;
	}
	string strHostIp   = pXmlBaseData->Attribute("host");
	int nPort     = atoi(pXmlBaseData->Attribute("port"));
	string strUser     = pXmlBaseData->Attribute("user");
	string strPassWord = pXmlBaseData->Attribute("password");
	string strDataBase = pXmlBaseData->Attribute("database");

	//初始化
	mMySql = mysql_init(NULL);
	if (mMySql == NULL)
	{
		cout << "CDataBase::Initialize : mMySql == NULL" << endl;
		return false;
	}
	/*使用mysql_real_connect连接服务器,其参数依次为MYSQL句柄，服务器IP地址，
	登录mysql的用户名，密码，要连接的数据库等*/
	mMySql = mysql_real_connect(mMySql, strHostIp.c_str(), strUser.c_str(), strPassWord.c_str(), strDataBase.c_str(), nPort, NULL, CLIENT_MULTI_RESULTS);
	if (!mMySql)
	{
		printf("connecting to Mysql error:%d from %s\n", mysql_errno(mMySql), mysql_error(mMySql));
		return -1;
	}
	else 
	{
		printf("Connected Mysql successful!\n");
	}

	return true;
}

bool CDataBase::ExecuteSql(const char* pSql, ...)
{
	if (pSql == NULL)
	{
		cout << "Query : sql == NULL" << endl;
		return false;
	}

	if (mMySql == NULL)
	{
		cout << "Query : mMySql == NULL" << endl;
		return false;
	}

	va_list tArgs;

	char tSqlCommand[MAX_SQL_BUFF_LENGTH] = { 0 };

	va_start(tArgs, pSql);
	::vsprintf(tSqlCommand, pSql, tArgs);
	va_end(tArgs);

	string strSql(tSqlCommand);
//	escape_string(strSql);

	int nFlag = mysql_real_query(mMySql, strSql.c_str(), (unsigned int)strSql.length());
	if (nFlag)
	{
		printf("query error:%d from %s\n", mysql_errno(mMySql), mysql_error(mMySql));
		return false;
	}

	MYSQL_RES * pSqlResult = mysql_store_result(mMySql);
	if (pSqlResult == NULL)
	{
		return true;
	}

	uint64 nRowCount = mysql_affected_rows(mMySql);
	uint32 nFieldCount = mysql_field_count(mMySql);
	mQueryResult.Init(pSqlResult, nRowCount, nFieldCount);
	return true;
}

void CDataBase::escape_string(std::string& str)
{
	if (str.empty())
	{
		return;
	}
	char* buf = new char[str.size() * 2 + 1];
	escape_string(buf, str.c_str(), str.size());
	str = buf;
	delete[] buf;
}

unsigned long CDataBase::escape_string(char *to, const char *from, unsigned long length)
{
	if (!mMySql || !to || !from || !length)
	{
		return 0;
	}
	return(mysql_real_escape_string(mMySql, to, from, length));
}

CQueryResult::CQueryResult()
{
	mCurRow = NULL;
	mFieldCount = 0;
	mRowCount = 0;
	mFieldNames = NULL;
	mResult = NULL;
}

void CQueryResult::Init(MYSQL_RES *result, uint64 rowCount, uint32 fieldCount)
{
	Initailize();
	mRowCount = rowCount;
	mFieldCount = fieldCount;
	mResult = result;
	if (mRowCount <= 0 || mFieldCount <= 0 || mResult == NULL)
	{
		return;
	}
	mCurRow = new CField[mFieldCount];
	if (mCurRow == NULL)
	{
		return;
	}

	MYSQL_FIELD * pFields = mysql_fetch_fields(mResult);
	if (pFields == NULL)
	{
		return;
	}

	mFieldNames = new string[mFieldCount];
	for (int i = 0; i < mFieldCount; ++i)
	{
		mFieldNames[i] = pFields[i].name;
		mCurRow[i].SetType(ConvertNativeType(pFields[i].type));
	}
}

void CQueryResult::Initailize()
{
	EndQuery();
	mCurRow = NULL;
	mFieldCount = 0;
	mRowCount = 0;
	mFieldNames = NULL;
	mResult = NULL;
}

CQueryResult::~CQueryResult()
{
	EndQuery();
}
bool CQueryResult::NextRow()
{
	MYSQL_ROW row;

	if (!mResult)
	{
		return false;
	}
	row = mysql_fetch_row(mResult);
	unsigned long *field_len = mysql_fetch_lengths(mResult);
	if (!row || !field_len)
	{
		EndQuery();
		return false;
	}

	memset(mCurRow, 0, sizeof(mCurRow));
	for (int i = 0; i < mFieldCount; i++)
	{
		mCurRow[i].SetValue(row[i], field_len[i]);
	}
	return true;

}

void CQueryResult::EndQuery()
{
	if (mCurRow)
	{
		delete[] mCurRow;
		mCurRow = NULL;
	}

	if (mResult)
	{
		mysql_free_result(mResult);
		mResult = 0;
	}

	if (mFieldNames)
	{
		delete[] mFieldNames;
		mFieldNames = NULL;
	}
}


enum CField::DataTypes CQueryResult::ConvertNativeType(enum_field_types mysqlType) const
{
	switch (mysqlType)
	{
	case FIELD_TYPE_TIMESTAMP:
	case FIELD_TYPE_DATE:
	case FIELD_TYPE_TIME:
	case FIELD_TYPE_DATETIME:
	case FIELD_TYPE_YEAR:
	case FIELD_TYPE_STRING:
	case FIELD_TYPE_VAR_STRING:
	case FIELD_TYPE_BLOB:
	case FIELD_TYPE_SET:
	case FIELD_TYPE_NULL:
		return CField::DB_TYPE_STRING;
	case FIELD_TYPE_TINY:

	case FIELD_TYPE_SHORT:
	case FIELD_TYPE_LONG:
	case FIELD_TYPE_INT24:
	case FIELD_TYPE_LONGLONG:
	case FIELD_TYPE_ENUM:
		return CField::DB_TYPE_INTEGER;
	case FIELD_TYPE_DECIMAL:
	case FIELD_TYPE_FLOAT:
	case FIELD_TYPE_DOUBLE:
		return CField::DB_TYPE_FLOAT;
	default:
		return CField::DB_TYPE_UNKNOWN;
	}
}

CField * CQueryResult::GetCurRowFieldByIndex(int nIndex)
{
	if (nIndex < 0 || nIndex >= mFieldCount)
	{
		return NULL;
	}

	return &mCurRow[nIndex];
}

CField::CField() :mValue(NULL), mType(DB_TYPE_UNKNOWN)
{
}

CField::CField(CField &f)
{
	const char *value;

	value = f.GetString();

	if (value && (mValue = new char[strlen(value) + 1]))
	{
		strcpy(mValue, value);
	}
	else
	{
		mValue = NULL;
	}
	mType = f.GetType();
}

CField::CField(const char *value, enum CField::DataTypes type) :mType(type)
{
	if (value && (mValue = new char[strlen(value) + 1]))
	{
		strcpy(mValue, value);
	}
	else
	{
		mValue = NULL;
	}
}

CField::~CField()
{
	if (mValue)
	{
		delete[] mValue;
	}
}

void CField::SetValue(const char *value)
{
	if (mValue)
	{
		delete[] mValue;
	}

	if (value)
	{
		mValue = new char[strlen(value) + 1];
		strcpy(mValue, value);
	}
	else
	{
		mValue = NULL;
	}
}

void CField::SetValue(const void *value, uint16 value_len)
{
	delete[] mValue;

	if (value && value_len > 0)
	{
		mValue = new char[value_len + 1];
		mValue[value_len] = 0;
		mValueLen = value_len;
		memcpy(mValue, value, value_len);
	}
	else
	{
		mValueLen = 0;
		mValue = NULL;
	}
}
