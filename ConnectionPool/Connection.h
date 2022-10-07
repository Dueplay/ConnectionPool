#pragma once
#include<mysql.h>
#include<string>
#include <ctime>
#include "public.h"
#include "pch.h"

using namespace std;
class Connection {
public:
	// 初始化数据库连接
	Connection();
	
	~Connection();
	
	bool connect(string ip, unsigned short port, string user, string password,string dbname);

	// 更新操作 insert、 del ete、 update
	bool update(string sql);
	
	// 查询操作 sel ect
	MYSQL_RES* query(string sql);

	void refreshAliveTime() { _alivetime = clock(); }
	clock_t getAliveTime() const {
		return clock() - _alivetime;
	}

private:
	MYSQL* _conn; //和mysql server的一条连接
	clock_t _alivetime;//记录进入空闲的存活时间
};