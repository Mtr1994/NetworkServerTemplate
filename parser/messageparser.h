#ifndef MESSAGEPARSER_H
#define MESSAGEPARSER_H

#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>

class MessageParser
{
public:
    explicit MessageParser();
    ~MessageParser();
    MessageParser(const MessageParser& signal) = delete;
    MessageParser& operator=(const MessageParser& signal) = delete;

public:
    static MessageParser* getInstance();
    void init();
    void parse(const std::string &message);

private:
    void parseMessage();

private:
    std::queue<std::string> mQueueMessage;
    std::mutex mMutexMessage;
    std::condition_variable mCVMessage;

    bool mInitStatus = false;
    bool mWorkingStatus = true;

    std::string mAesKey = "IRDPtpDqzNWcUQ2sGYXV8r1BxPhxAGRR";
    std::string mHashKey;

    std::string mBufferString;
};

#endif // MESSAGEPARSER_H
