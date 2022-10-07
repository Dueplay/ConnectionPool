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
	//�����ӳػ�ȡһ������
	shared_ptr<Connection> getConnection();
private:
	//����#1 ����˽�л�
	ConnectionPool();
	//�������ļ��м�������
	bool loadConfigFile();

	//�����ڶ������߳��У��������̣߳�ר�Ÿ�������������
	void produceConnectionTask();

	//����һ����ʱ�̣߳�ɨ�賬��max����ʱ������ӣ����յ�
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
	condition_variable cv;//���������������ߺ������ߵ�ͨ��
};