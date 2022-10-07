#pragma once
#include<mysql.h>
#include<string>
#include <ctime>
#include "public.h"
#include "pch.h"

using namespace std;
class Connection {
public:
	// ��ʼ�����ݿ�����
	Connection();
	
	~Connection();
	
	bool connect(string ip, unsigned short port, string user, string password,string dbname);

	// ���²��� insert�� del ete�� update
	bool update(string sql);
	
	// ��ѯ���� sel ect
	MYSQL_RES* query(string sql);

	void refreshAliveTime() { _alivetime = clock(); }
	clock_t getAliveTime() const {
		return clock() - _alivetime;
	}

private:
	MYSQL* _conn; //��mysql server��һ������
	clock_t _alivetime;//��¼������еĴ��ʱ��
};