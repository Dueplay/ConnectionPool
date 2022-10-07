#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include "CommonConnectionPool.h"

ConnectionPool* ConnectionPool::getConnectionPool()
{
    static ConnectionPool pool;
    return &pool;
}

shared_ptr<Connection> ConnectionPool::getConnection()
{
    unique_lock<mutex> lock(_queueMutex);
    while (_connectionQue.empty()) {
        if (cv_status::timeout == cv.wait_for(lock, chrono::milliseconds(_connectionTimeOut))) {
            if (_connectionQue.empty()) {
                LOG("��ȡ�������ӳ�ʱ��...��ȡ����ʧ��!");
                return nullptr;
            }
        }
    }

    shared_ptr<Connection> sp(_connectionQue.front(),
        [&](Connection* pcon) {
            unique_lock<mutex> lock(_queueMutex);
            pcon->refreshAliveTime();
            _connectionQue.push(pcon);
        });
    _connectionQue.pop();
    cv.notify_all();
    return sp;
}

bool ConnectionPool::loadConfigFile()
{
    FILE* pf = fopen("mysql.ini", "r");
    if (pf == nullptr) {
        LOG("mysql.ini file is not exist!");
        return false;
    }
    
    while (!feof(pf)) {
        char line[1024] = { 0 };
        fgets(line, 1024, pf);//read a line
        string str = line;
        
        int idx = str.find("=");
        if(idx==-1) {
            continue;
        }
        int endIdx = str.find("\n",idx);
        string key = str.substr(0, idx);
        string value = str.substr(idx+1, endIdx - idx - 1);
        
        if (key == "ip") {
            _ip = value;
        }
        else if (key == "port") {
            _port = atoi(value.c_str());
        }
        else if (key == "username") {
            _username = value;
        }
        else if (key == "passwd") {
            _passwd = value;
        }
        else if (key == "dbname") {
            _dbname = value;
        }
        else if (key == "initSize") {
            _initSize = atoi(value.c_str());
        }
        else if (key == "maxSize") {
            _maxSize = atoi(value.c_str());
        }
        else if (key == "maxIdleTime") {
            _maxIdleTime = atoi(value.c_str());
        }
        else if (key == "connectionTimeOut") {
            _connectionTimeOut = atoi(value.c_str());
        }
    }
    return true;
}

void ConnectionPool::produceConnectionTask()
{
    for (;;) {
        unique_lock<mutex> lock(_queueMutex);
        //���в��յȴ�
        while (!_connectionQue.empty()) {
            cv.wait(lock);
        }

        if (_connectionCnt < _maxSize) {
            Connection* p = new Connection;
            p->connect(_ip, _port, _username, _passwd, _dbname);
            p->refreshAliveTime();
            _connectionQue.push(p);
            _connectionCnt++;
        }
        //֪ͨ�������߳̿�������������
        cv.notify_all();
    }
}

void ConnectionPool::scannerConnectionTask()
{
    for (;;) {
        //sleepģ�¶�ʱЧ��
        this_thread::sleep_for(chrono::seconds(_maxIdleTime));
        unique_lock<mutex> lock(_queueMutex);
        while (_connectionCnt > _initSize) {
            Connection* p = _connectionQue.front();
            if (p->getAliveTime() > (_maxIdleTime * 1000)) {
                _connectionQue.pop();
                _connectionCnt--;
                delete p;
            }
            else {
                break;
            }
        }
    }
}

ConnectionPool::ConnectionPool() {
    if (!loadConfigFile()) {
        return;
    }
    for (int i = 0; i < _initSize; i++) {
        Connection* p = new Connection;
        p->connect(_ip, _port, _username, _passwd, _dbname);
        p->refreshAliveTime();
        _connectionQue.push(p);
        _connectionCnt++;
    }
    //����һ���̣߳���Ϊ���ӵ�������
    thread produce(bind(&ConnectionPool::produceConnectionTask,this));
    produce.detach();
    //����һ����ʱ�̣߳�ɨ�賬��max����ʱ������ӣ����յ�
    thread scanner(bind(&ConnectionPool::scannerConnectionTask, this));
    scanner.detach();
}