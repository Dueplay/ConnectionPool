#pragma once
#include <string>
#include<queue>
#include <mutex>
#include <memory>
#include <atomic>
#include <thread>
#include <functional>
#include <condition_variable>
#include "Connection.h"
using namespace std;
class ConnectionPool {
public:
	static ConnectionPool* getConnectionPool();
	//从连接池获取一个连接
	shared_ptr<Connection> getConnection();
private:
	//单例#1 构造私有化
	ConnectionPool();
	//从配置文件中加载配置
	bool loadConfigFile();

	//运行在独立的线程中，生产者线程，专门负责生产新连接
	void produceConnectionTask();

	//启动一个定时线程，扫描超过max空闲时间的连接，回收掉
	void scannerConnectionTask();
	string _ip;
	unsigned short _port;
	string _username;
	string _passwd;
	string _dbname;
	int _initSize;
	int _maxSize;
	int _maxIdleTime;
	int _connectionTimeOut;
	queue<Connection*> _connectionQue;
	mutex _queueMutex;
	atomic_int _connectionCnt;
	condition_variable cv;//条件变量，生产者和消费者的通信
};