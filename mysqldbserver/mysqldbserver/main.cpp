
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <mysql.h>
#include <iostream>

using namespace std;

#include "simplebasedata.h"

MYSQL mysql;

// ��ѯ
int query();
// �޸�
int update();
// �������
my_ulonglong add();
// �������������
my_ulonglong addByParams();
// ɾ������
my_ulonglong Delete();
// ��ӡ���ݿ��������Ϣ
void printMySqlInfo();

int main(int argc, const char * argv[])
{
	CDataBase tDataBase;
	tDataBase.Initialize("./config/mysqlinfo.xml");
	bool bQuery = tDataBase.Query("select * from ccc");
	CQueryResult & rQueryResult = tDataBase.GetQueryResult();
	while (bQuery && rQueryResult.NextRow())
	{
		CField* pField = rQueryResult.Fetch();
		if (pField != NULL)
		{
			for (int i = 0; i < rQueryResult.GetFieldCount(); ++i)
			{
				cout << pField[i].GetType() <<"\t";
				cout << pField[i].GetString() << endl;
			}
		}
	}
	return 0;
}
//
///*����֮ǰ������mysql_init��ʼ��MYSQL���Ӿ��*/
//mysql_init(&mysql);
//
///*ʹ��mysql_real_connect���ӷ�����,���������ΪMYSQL�����������IP��ַ��
//��¼mysql���û��������룬Ҫ���ӵ����ݿ��*/
//if (!mysql_real_connect(&mysql, "192.168.10.12", "root", "webgame", "test", 3306, NULL, CLIENT_MULTI_RESULTS))
//{
//	printf("connecting to Mysql error:%d from %s\n", mysql_errno(&mysql), mysql_error(&mysql));
//	return -1;
//}
//else
//{
//	printf("Connected Mysql successful!\n");
//}
//
//// ���ñ���
//mysql_query(&mysql, "set names utf8");
//
////printMySqlInfo();
//
//// �������������
////addByParams();
//
//// ��ѯ
////query();
//
//// �޸�
//update();
//
//// ���
////add();
//
//// ɾ��
////Delete();
//
///*�ر�����*/
//mysql_close(&mysql);

// ��ѯ
int query()
{
	int flag, i;
	const char *sql = NULL;
	MYSQL_RES *res = NULL;
	MYSQL_ROW row = NULL;
	MYSQL_FIELD *fields = NULL;
	sql = "select * from name";
	flag = mysql_real_query(&mysql, sql, (unsigned int)strlen(sql));
	if (flag) 
	{
		printf("query error:%d from %s\n", mysql_errno(&mysql), mysql_error(&mysql));
		return -1;
	}

	// ����ѯ�����ȡ���ڴ浱�У�������ݺܶ�������ȽϺ��ڴ�
	res = mysql_store_result(&mysql);
	// res = mysql_use_result(&mysql); // ��Ҫ�õ���ʱ��ÿ�δӷ������ж�ȡһ��

	// �ֶ�����
	unsigned int field_count = mysql_field_count(&mysql);
	printf("field_cout:%d\n", field_count);

	// ��ѯ����
	my_ulonglong rows = mysql_num_rows(res);
	printf("%lld\n", rows);

	// ��ȡ�����ֶ�
	fields = mysql_fetch_fields(res);
	for (int i = 0; i < mysql_num_fields(res); i++)
	{
		printf("%s\t", fields[i].name);
		printf("%s\t", fields[i].table);
		printf("%s\t", fields[i].db);
		printf("%s\t", fields[i].catalog);
		if (fields[i].def == NULL)
		{
			printf("NULL\n");
		}
		else
		{
			printf("%s\n", fields[i].def);
		}
	}

	// ���������
	while ((row = mysql_fetch_row(res)))
	{
		for (i = 0; i < mysql_num_fields(res); i++)
		{
			printf("%s\t", row[i]);

		}

		printf("\n");
	}

	// �ͷŽ����
	mysql_free_result(res);

	return 0;
}

// �޸�
int update()
{
	const char *sql = NULL;
	int flag = -1;
	sql = "update t_user set name='lisi',age=20 where id=1";
	// ִ��SQLָ��
	flag = mysql_real_query(&mysql, sql, (unsigned int)strlen(sql));
	if (flag) 
	{
		printf("update data error:%d from %s\n", mysql_errno(&mysql), mysql_error(&mysql));
		return  -1;
	}

	printf("update success.\n");

	return 0;
}

// ���
my_ulonglong add()
{
	const char *sql = NULL;
	int flag = -1;
	sql = "insert into t_user(name,age,address) values ('zhangsan',40,'beijing')";
	// ִ��
	flag = mysql_real_query(&mysql, sql, strlen(sql));
	if (flag)
	{
		printf("add data error:%d from %s\n", mysql_errno(&mysql), mysql_error(&mysql));
		return -1;
	}

	// ɾ��������
	my_ulonglong affected_rows = mysql_affected_rows(&mysql);

	// ����Ӽ�¼��ID
	my_ulonglong newid = mysql_insert_id(&mysql);

	printf("success add %lld record data, newid:%lld!\n", affected_rows, newid);

	return newid;
}

// ���������
my_ulonglong addByParams()
{
	const char *sql = NULL;
	MYSQL_STMT *stmt = NULL;
	MYSQL_BIND bnd[3];  // ռλ����������
	memset(&bnd, 0, sizeof(bnd));

	// �����ֶ��ύ����
	//mysql_autocommit(&mysql, 0);

	// ͨ������ռλ���ķ�ʽִ��SQL
	sql = "insert into t_user(name,age,address) values (?,?,?)";

	stmt = mysql_stmt_init(&mysql);
	// Ԥ����SQL
	if (mysql_stmt_prepare(stmt, sql, (unsigned int)strlen(sql)))
	{
		fprintf(stderr, "mysql_stmt_prepare faild:%d from %s\n",
			mysql_stmt_errno(stmt), mysql_stmt_error(stmt));
		return -1;
	}

	// ��װռλ������
	const char *name = "hanzhiqiang";
	bnd[0].buffer = (void *)name;
	bnd[0].buffer_type = MYSQL_TYPE_STRING;
	bnd[0].buffer_length = strlen(name);

	int age = 30;
	bnd[1].buffer = (void *)&age;
	bnd[1].buffer_length = sizeof(int);
	bnd[1].buffer_type = MYSQL_TYPE_LONG;

	const char *addres = "heimuer";
	bnd[2].buffer = (void *)addres;
	bnd[2].buffer_length = strlen(addres);
	bnd[2].buffer_type = MYSQL_TYPE_STRING;

	// ��ռλ������ֵ
	if (mysql_stmt_bind_param(stmt, bnd))
	{
		fprintf(stderr, "mysql_stmt_bind_param faild:%d from %s\n", mysql_errno(&mysql), mysql_error(&mysql));
		return -2;
	}

	// ִ��SQL
	if (mysql_stmt_execute(stmt)) 
	{
		fprintf(stderr, "mysql_stmt_execute faild:%d from %s\n", mysql_errno(&mysql), mysql_error(&mysql));
		return -3;
	}

	// �ر�statement
	mysql_stmt_close(stmt);

	// �ύ����
	//mysql_commit(&mysql);

	// ����ع������ύ����ǰ��ִ��
	// mysql_rollback(&mysql);

	// ��ȡ�������ݺ����ݿ���Ӱ��ļ�¼��
	my_ulonglong newid = mysql_stmt_insert_id(stmt);
	printf("�������������¼�¼��id: %lld\n", newid);

	// ��Ӱ�������
	my_ulonglong affectedrows = mysql_stmt_affected_rows(stmt);
	printf("��������������Ӱ���������%lld\n", affectedrows);

	return newid;
}

my_ulonglong Delete()
{
	const char *sql = NULL;
	int flag = -1;
	sql = "delete from t_user where id > 10";
	flag = mysql_real_query(&mysql, sql, strlen(sql));
	if (flag) {
		printf("delete data error:%d from %s\n", mysql_errno(&mysql), mysql_error(&mysql));
		return -1;
	}

	my_ulonglong affected_rows = mysql_affected_rows(&mysql);
	printf("ɾ����������%lld\n", affected_rows);

	printf("success delete %lld record data !\n", affected_rows);
	return affected_rows;
}

void printMySqlInfo()
{
	const char *stat = mysql_stat(&mysql);
	const char *server_info = mysql_get_server_info(&mysql);
	const char *clientInfo = mysql_get_client_info();
	unsigned long version = mysql_get_client_version();
	const char *hostinfo = mysql_get_host_info(&mysql);
	unsigned long serverversion = mysql_get_server_version(&mysql);
	unsigned int protoinfo = mysql_get_proto_info(&mysql);

	printf("stat:%s\n", stat);
	printf("server_info:\n");
	printf("clientInfo:%s\n", clientInfo);
	printf("version:%ld\n", version);
	printf("hostinfo:%s\n", hostinfo);
	printf("serverversion:%ld\n", serverversion);
	printf("protoinfo:%d\n", protoinfo);

	const char *charactername = mysql_character_set_name(&mysql);
	printf("client character set:%s\n", charactername);
	if (!mysql_set_character_set(&mysql, "utf8"))
	{
		printf("New client character set:%s\n",
			mysql_character_set_name(&mysql));
	}
}