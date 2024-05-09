#include "messageparser.h"
#include "log/logger.h"
#include "mysql/mysqlconnectionpool.h"

#include "hex.h"
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;

#include "filters.h"
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;

#include "aes.h"
using CryptoPP::AES;

#include "modes.h"
using CryptoPP::ECB_Mode;

#include "sha.h"
using CryptoPP::SHA256;
using CryptoPP::HashFilter;

#include <functional>
#include <arpa/inet.h>

// test
#include <iomanip>

MessageParser::MessageParser()
{

}

MessageParser::~MessageParser()
{
    mWorkingStatus = false;
    mCVMessage.notify_one();
}

MessageParser *MessageParser::getInstance()
{
    static MessageParser inst;
    return &inst;
}

void MessageParser::init()
{
    LOG_DEBUG("message parser init");

    // 计算密钥
    SHA256 hash;
    StringSource(mAesKey, true, new HashFilter(hash, new StringSink(mHashKey)));

    auto func = std::bind(&MessageParser::parseMessage, this);
    std::thread th(func);
    th.detach();
    mInitStatus = true;
}

void MessageParser::parse(const std::string &message)
{
    if (!mInitStatus)
    {
        LOG_DEBUG("please init message parser first");
        return;
    }

    std::string encoded;
    StringSource(message, true, new HexEncoder(new StringSink(encoded))); 
    LOG_DEBUG("recv hex data >" + encoded);

    mBufferString.append(message);
    // pack format: {$IDSSEffffxxxxxxxxxx.....CCCC} {$IDSSE marker} {ffff length of total pack} {xxxxxxx... data that encrypted} {CCCC CRC Check}
    while (mBufferString.size() > 8)
    {
        size_t pos = mBufferString.find("$IDSSE");
        if (pos == std::string::npos) 
        {
            mBufferString.clear();
            continue;
        }
        mBufferString.substr(pos);

        uint16_t len = 0;
        memcpy(&len, mBufferString.data() + 6, 2); len = ntohs(len);
        if (mBufferString.size() < len) continue;
        
        std::string pack = mBufferString.substr(8, len - 10);
        mBufferString = mBufferString.substr(len);

        std::lock_guard<std::mutex> lock(mMutexMessage);
        mQueueMessage.push(pack);
    }
    
    if (!mQueueMessage.empty()) mCVMessage.notify_one();
}

void MessageParser::parseMessage()
{
    while (mWorkingStatus)
    {
        std::unique_lock<std::mutex> lock(mMutexMessage);
        mCVMessage.wait(lock, [this]{ return !mQueueMessage.empty() || !mWorkingStatus; });

        if (!mWorkingStatus) break;
        if (mQueueMessage.empty()) continue;

        std::string message = mQueueMessage.front();
        mQueueMessage.pop();
        if (message.size() % 16 != 0) continue;

        std::string recovered;
        ECB_Mode<AES>::Decryption decryption;
        decryption.SetKey((const byte*)mHashKey.data(), sizeof(mHashKey));
        StringSource(message, true, new StreamTransformationFilter(decryption, new StringSink(recovered), CryptoPP::BlockPaddingSchemeDef::ZEROS_PADDING));
        LOG_DEBUG("recv message >" + recovered);

        std::string sql = "";
        // parser success, insert into database
        auto conn = MySQLConnectionPool::getInstance()->getConnection();
        if (nullptr == conn)
        {
            LOG_DEBUG("can not get mysql connection, please check mysql status or connect parameters");
            continue;
        }

        LOG_DEBUG("start exec sql >" + sql);
        bool status = conn->query(sql);
        if (!status)
        {
            LOG_DEBUG("run sql failed, error > " + std::string(conn->lastError()));
            continue;
        }

        LOG_DEBUG("exec sql success, affect rows > " + std::to_string(conn->affectedRows()));
    }
    LOG_DEBUG("message parser thread finished");
}
