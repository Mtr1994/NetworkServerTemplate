#include "mysqlconnection.h"

MysqlConnection::MysqlConnection()
{
    m_conn = mysql_init(nullptr);
    mysql_set_character_set(m_conn, "utf8");
}

MysqlConnection::~MysqlConnection()
{
    if (m_conn != nullptr)
    {
        mysql_close(m_conn);
    }
    freeResult();
}

bool MysqlConnection::connect(std::string user, std::string passwd, std::string dbName, std::string ip, unsigned short port)
{
    MYSQL* ptr = mysql_real_connect(m_conn, ip.c_str(), user.c_str(), passwd.c_str(), dbName.c_str(), port, nullptr, 0);
    return ptr != nullptr;
}

bool MysqlConnection::update(std::string sql)
{
    if (mysql_query(m_conn, sql.c_str()))
    {
        return false;
    }
    return true;
}

bool MysqlConnection::query(std::string sql)
{
    freeResult();
    if (mysql_query(m_conn, sql.c_str()))
    {
        return false;
    }
    m_result = mysql_store_result(m_conn);
    return true;
}

bool MysqlConnection::next()
{
    if (m_result != nullptr)
    {
        m_row = mysql_fetch_row(m_result);
        if (m_row != nullptr)
        {
            return true;
        }
    }
    return false;
}

std::string MysqlConnection::value(int index)
{
    int rowCount = mysql_num_fields(m_result);
    if (index >= rowCount || index < 0)
    {
        return std::string();
    }
    char* val = m_row[index];
    unsigned long length = mysql_fetch_lengths(m_result)[index];
    return std::string(val, length);
}

bool MysqlConnection::transaction()
{
    return 0 == mysql_autocommit(m_conn, false);
}

bool MysqlConnection::commit()
{
    return 0 == mysql_commit(m_conn);
}

bool MysqlConnection::rollback()
{
    return 0 == mysql_rollback(m_conn);
}

void MysqlConnection::refreshAliveTime()
{
    m_alivetime = steady_clock::now();
}

long long MysqlConnection::getAliveTime()
{
    nanoseconds res = steady_clock::now() - m_alivetime;
    milliseconds millsec = duration_cast<milliseconds>(res);
    return millsec.count();
}

const char* MysqlConnection::lastError()
{
	return mysql_error(m_conn);
}

uint64_t MysqlConnection::affectedRows()
{
	return mysql_affected_rows(m_conn);
}

void MysqlConnection::freeResult()
{
    if (m_result)
    {
        mysql_free_result(m_result);
        m_result = nullptr;
    }
}
