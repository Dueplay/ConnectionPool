#include "Connection.h"

Connection::Connection()
{
	_conn = mysql_init(nullptr);
}

Connection::~Connection()
{
	if(_conn != nullptr)
		mysql_close(_conn);
}

bool Connection::connect(string ip, unsigned short port, string user, string password, string dbname)
{
	MYSQL* p = mysql_real_connect(_conn, ip.c_str(), user.c_str(),
		password.c_str(), dbname.c_str(), port, nullptr, 0);
	return p != nullptr;
}

bool Connection::update(string sql)
{
	if(mysql_query(_conn, sql.c_str()))
	{
		LOG("????ʧ??: " + sql);
		return false;
	}
	return true;
}

MYSQL_RES* Connection::query(string sql)
{
	if(mysql_query(_conn, sql.c_str()))
	{
		LOG("??ѯʧ??: " + sql);
		return nullptr;
	}
	return mysql_use_result(_conn);
}
