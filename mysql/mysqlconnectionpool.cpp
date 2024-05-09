#include "mysqlconnectionpool.h"
#include "log/logger.h"
#include "conf/appconfig.h"

#include <fstream>
#include <thread>

MySQLConnectionPool::MySQLConnectionPool()
{

}

MySQLConnectionPool::~MySQLConnectionPool()
{
    while (!m_connectionQ.empty())
    {
        MysqlConnection* conn = m_connectionQ.front();
        m_connectionQ.pop();
        delete conn;
    }
}

MySQLConnectionPool* MySQLConnectionPool::getInstance()
{
    static MySQLConnectionPool pool;
    return &pool;
}

void MySQLConnectionPool::init()
{
	// config para
    m_ip = AppConfig::getInstance()->getValue("MySQL", "address");
	m_port = 3306;
    m_user = AppConfig::getInstance()->getValue("MySQL", "user");
    m_passwd = AppConfig::getInstance()->getValue("MySQL", "pwd");
    m_dbName = AppConfig::getInstance()->getValue("MySQL", "db");
    m_minSize = 3;
    m_maxSize = 6;
    m_maxIdleTime = 300000;
	m_timeout = 1000;

    std::thread producer(&MySQLConnectionPool::produceConnection, this);
    std::thread recycler(&MySQLConnectionPool::recycleConnection, this);
    producer.detach();
    recycler.detach();

    LOG_DEBUG("init mysql connection finish, " + std::to_string(m_minSize)  + " connection will be create.");
}

std::shared_ptr<MysqlConnection> MySQLConnectionPool::getConnection()
{
    std::unique_lock<std::mutex> locker(m_mutexQ);
    if (m_connectionQ.empty()) return nullptr;

    std::shared_ptr<MysqlConnection> connptr(m_connectionQ.front(), [this](MysqlConnection* conn) {
        std::lock_guard<std::mutex> locker(m_mutexQ);
        conn->refreshAliveTime();
        m_connectionQ.push(conn);
        });
    m_connectionQ.pop();
    m_cond.notify_all();
    return connptr;
}

void MySQLConnectionPool::produceConnection()
{
    while (true)
    {
        std::unique_lock<std::mutex> locker(m_mutexQ);
        while (m_connectionQ.size() >= m_minSize)
        {
            m_cond.wait(locker);
        }
        bool status = addConnection();
        if (!status)
        {
            locker.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(60));
        }
        m_cond.notify_all();
    }
}

void MySQLConnectionPool::recycleConnection()
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        {
            std::lock_guard<std::mutex> locker(m_mutexQ);
		    while (m_connectionQ.size() >= m_minSize)
		    {
		        MysqlConnection* conn = m_connectionQ.front();
		        if (conn->getAliveTime() >= m_maxIdleTime)
		        {
		            m_connectionQ.pop();
		            delete conn;
		        }
		        else
		        {
		            break;
		        }
		    }
        }
        
        m_cond.notify_all();
    }
}

bool MySQLConnectionPool::addConnection()
{
    MysqlConnection* conn = new MysqlConnection;
    bool status = conn->connect(m_user, m_passwd, m_dbName, m_ip, m_port);
    if (!status) 
    {
        LOG_DEBUG("connect to mysql database failed, please check connect parameter.");
        return status;
    }
    conn->refreshAliveTime();
    LOG_DEBUG("one mysql connection create at this time.");
    m_connectionQ.push(conn);
    return true;
}





