#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <map>
#include <string>

class AppConfig 
{
private:
    explicit AppConfig();
    AppConfig(const AppConfig& signal) = delete;
    AppConfig& operator=(const AppConfig& signal) = delete;

public:
    static AppConfig* getInstance();

    bool init();

    std::string getValue(const std::string& section, const std::string& key);

private:
    std::map<std::string, std::map<std::string, std::string>> mMapConfigValue;
};

#endif // APPCONFIG_H