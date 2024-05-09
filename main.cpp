#include <iostream>
#include <sys/types.h> 
#include <signal.h>

#include "appservice.h"
#include "log/logger.h"
#include "conf/appconfig.h"
#include "mysql/mysqlconnectionpool.h"

using namespace std;

int main()
{
    // init config file
    bool status = AppConfig::getInstance()->init();
    if (!status) return -1;

    string daemonize = AppConfig::getInstance()->getValue("App", "daemonize");
    if (daemonize.empty()) daemonize = "1";

    // run in daemonize mode (前面不能有任何创建线程的逻辑)
    if (stoi(daemonize))
    {
        pid_t pid;

        signal(SIGTTOU, SIG_IGN);
        signal(SIGTTIN, SIG_IGN);
        signal(SIGTSTP, SIG_IGN);

        if (setpgrp() == -1)
        {
            perror("setpgrp failure");
        }

        signal(SIGHUP, SIG_IGN);

        if ((pid = fork()) < 0)
        {
            perror("fork failure");
            exit(1);
        }
        else if (pid > 0)
        {
            exit(0);
        }

        setsid();

        signal(SIGCLD, SIG_IGN);
        signal(SIGCHLD, SIG_IGN);
        signal(SIGPIPE, SIG_IGN);
    }

    // 初始化日志，不能放 daemonize 逻辑
    Logger logger;
    logger.init();

    // init mysql connection
    MySQLConnectionPool::getInstance()->init();

    LOG_DEBUG("start rtps instance");
    AppService::getInstance()->start();
    LOG_DEBUG("stop rtps instance");

    return 0;
}
