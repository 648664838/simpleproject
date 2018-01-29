#include <mysql.h>
#include <string>
using namespace std;
#include "base.h"
#include <string>
#include <stdio.h>
#include <algorithm>
class CField
{
public:
	enum DataTypes
	{
		DB_TYPE_UNKNOWN = 0x00,
		DB_TYPE_STRING = 0x01,
		DB_TYPE_INTEGER = 0x02,
		DB_TYPE_FLOAT = 0x03,
		DB_TYPE_BOOL = 0x04
	};

	CField();
	CField(CField &f);
	CField(const char *value, enum DataTypes type);

	~CField();

	enum DataTypes GetType() const { return mType; }

	const char *GetString() const { return mValue; }
	std::string GetCppString() const
	{
		return mValue ? mValue : "";                    // std::string s = 0 have undefine result in C++
	}
	float GetFloat() const { return mValue ? static_cast<float>(atof(mValue)) : 0; }
	bool GetBool() const { return mValue ? atoi(mValue) > 0 : false; }
	int32 GetInt32() const { return mValue ? static_cast<int32>(atol(mValue)) : 0; }
	uint8 GetUInt8() const { return mValue ? static_cast<uint8>(atol(mValue)) : 0; }
	uint16 GetUInt16() const { return mValue ? static_cast<uint16>(atol(mValue)) : 0; }
	int16 GetInt16() const { return mValue ? static_cast<int16>(atol(mValue)) : 0; }
	uint32 GetUInt32() const { return mValue ? static_cast<uint32>(atol(mValue)) : 0; }
	uint64 GetUInt64() const
	{
		if (mValue)
		{
			uint64 value;
			sscanf(mValue, I64FMTD, &value);
			return value;
		}
		else
			return 0;
	}
	uint16 GetValueLen() { return mValueLen; }

	void SetType(enum DataTypes type) { mType = type; }

	void SetValue(const char *value);
	void SetValue(const void *value, uint16 value_len);
private:
	char *mValue;
	uint16 mValueLen;
	enum DataTypes mType;
};

class CQueryResult
{
public:
	CQueryResult();
	~CQueryResult();
	bool NextRow();
	CField * Fetch(){ return mCurRow; }
	void Init(MYSQL_RES *result, uint64 rowCount, uint32 fieldCount);
	void Initailize();
	enum CField::DataTypes ConvertNativeType(enum_field_types mysqlType) const;
	void EndQuery();
	int GetFieldCount(){ return mFieldCount; }
private:
	CField *mCurRow;
	uint32 mFieldCount;
	uint64 mRowCount;
	string *mFieldNames;
	MYSQL_RES *mResult;
};

class CDataBase
{
public:
	CDataBase();
	~CDataBase();

	bool Initialize(const char * pPath);
	bool Query(const char * pSql);
	void escape_string(std::string& str);

	CQueryResult& GetQueryResult(){ return mQueryResult; }
protected:
	unsigned long escape_string(char *to, const char *from, unsigned long length);
private:
	MYSQL * mMySql;
	CQueryResult mQueryResult;
};