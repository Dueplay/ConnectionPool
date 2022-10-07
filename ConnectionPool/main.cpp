#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include "Connection.h"
#include "pch.h"
#include "CommonConnectionPool.h"
//#define _test1_

int main()
{
	/*
	Connection conn;
	char sql[1024] = { 0 };
	sprintf(sql, "insert into tbl(no,name) value(%d,'%s')", 99, "guoxianjun");
	conn.connect("127.0.0.1", 3306, "root", "123456", "test");
	conn.update(sql);
	*/

	
#ifdef _test1_
	//测试单线程使用和不使用线程池的区别
	clock_t begin = clock();
	ConnectionPool* cp = ConnectionPool::getConnectionPool();
	for (int i = 0; i < 10000; i++) {
		Connection conn;
		char sql[1024] = { 0 };
		sprintf(sql, "insert into tbl(no,name) value(%d,'%s')", 99, "guoxianjun");
		conn.connect("127.0.0.1", 3306, "root", "123456", "test");
		conn.update(sql);

		/*shared_ptr<Connection> sp = cp->getConnection();
		char sql[1024] = { 0 };
		sprintf(sql, "insert into tbl(no,name) value(%d,'%s')", 99, "guoxianjun");
		sp->update(sql);*/
	}

	clock_t end = clock();
	cout << (end - begin) << "ms" << endl;
#endif // 0

	//测试多线程使用和不使用线程池的区别
	clock_t begin = clock();
	
	thread t1([]() {
		ConnectionPool* cp = ConnectionPool::getConnectionPool();
		for (int i = 0; i < 2500; i++) {
			Connection conn;
			char sql[1024] = { 0 };
			sprintf(sql, "insert into tbl(no,name) value(%d,'%s')", 99, "guoxianjun");
			conn.connect("127.0.0.1", 3306, "root", "123456", "test");
			conn.update(sql);

			/*shared_ptr<Connection> sp = cp->getConnection();
			char sql[1024] = { 0 };
			sprintf(sql, "insert into tbl(no,name) value(%d,'%s')", 99, "guoxianjun");
			sp->update(sql);*/
		}
	});
	thread t2([]() {
		ConnectionPool* cp = ConnectionPool::getConnectionPool();
		for (int i = 0; i < 2500; i++) {
			Connection conn;
			char sql[1024] = { 0 };
			sprintf(sql, "insert into tbl(no,name) value(%d,'%s')", 99, "guoxianjun");
			conn.connect("127.0.0.1", 3306, "root", "123456", "test");
			conn.update(sql);

			/*shared_ptr<Connection> sp = cp->getConnection();
			char sql[1024] = { 0 };
			sprintf(sql, "insert into tbl(no,name) value(%d,'%s')", 99, "guoxianjun");
			sp->update(sql);*/
		}
		});
	thread t3([]() {
		ConnectionPool* cp = ConnectionPool::getConnectionPool();
		for (int i = 0; i < 2500; i++) {
			Connection conn;
			char sql[1024] = { 0 };
			sprintf(sql, "insert into tbl(no,name) value(%d,'%s')", 99, "guoxianjun");
			conn.connect("127.0.0.1", 3306, "root", "123456", "test");
			conn.update(sql);

			/*shared_ptr<Connection> sp = cp->getConnection();
			char sql[1024] = { 0 };
			sprintf(sql, "insert into tbl(no,name) value(%d,'%s')", 99, "guoxianjun");
			sp->update(sql);*/
		}
		});
	thread t4([]() {
		ConnectionPool* cp = ConnectionPool::getConnectionPool();
		for (int i = 0; i < 2500; i++) {
			Connection conn;
			char sql[1024] = { 0 };
			sprintf(sql, "insert into tbl(no,name) value(%d,'%s')", 99, "guoxianjun");
			conn.connect("127.0.0.1", 3306, "root", "123456", "test");
			conn.update(sql);

			/*shared_ptr<Connection> sp = cp->getConnection();
			char sql[1024] = { 0 };
			sprintf(sql, "insert into tbl(no,name) value(%d,'%s')", 99, "guoxianjun");
			sp->update(sql);*/
		}
		});
	t1.join();
	t2.join();
	t3.join();
	t4.join();

	clock_t end = clock();
	cout << (end - begin) << "ms" << endl;
	return 0;
}