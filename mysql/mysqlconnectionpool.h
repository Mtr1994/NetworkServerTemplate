#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include "mysqlconnection.h"

class MySQLConnectionPool
{
private:
    MySQLConnectionPool();
    MySQLConnectionPool(const MySQLConnectionPool& obj) = delete;
    MySQLConnectionPool& operator=(const MySQLConnectionPool& obj) = delete;

public:
    ~MySQLConnectionPool();

public:
    static MySQLConnectionPool* getInstance();
    void init();
    std::shared_ptr<MysqlConnection> getConnection();

private:
    void produceConnection();
    void recycleConnection();
    bool addConnection();

    std::string m_ip;
    std::string m_user;
    std::string m_passwd;
    std::string m_dbName;
    unsigned short m_port;
    size_t m_minSize;
    size_t m_maxSize;
    int m_timeout;
    int m_maxIdleTime;
    std::queue<MysqlConnection*> m_connectionQ;
    std::mutex m_mutexQ;
    std::condition_variable m_cond;
};

