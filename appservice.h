#ifndef APPSERVICE_H
#define APPSERVICE_H

class AppService
{
private:
    explicit AppService();
    AppService(const AppService& signal) = delete;
    AppService& operator=(const AppService& signal) = delete;

public:
    static AppService* getInstance();

    void start();
};

#endif // APPSERVICE_H
