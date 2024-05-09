#include "appconfig.h"

#include <fstream>
#include <iostream>

using namespace std;

AppConfig::AppConfig()
{

}

AppConfig *AppConfig::getInstance()
{
    static AppConfig inst;
    return &inst;
}

bool AppConfig::init()
{
    ifstream file("conf.ini");
    if (!file.is_open())
    {
        cout << "error, can not read app config file, process exit." << endl;
        return false;
    }

    std::string line;
	std::string currentSection;
	while (std::getline(file, line)) 
    {
		// 移除行首和行尾的空格和制表符
		line.erase(line.find_last_not_of(" \t") + 1);
		line.erase(0, line.find_first_not_of(" \t"));

		//cout << "line:" << line << endl;
		if (line.empty() || line[0] == ';' || line[0] == '#') 
        {
			// 空行或注释行，忽略
			continue;
		}
		else if (line[0] == '[' && line[line.length() - 1] == ']') 
        {
			// 新的节(section)
			currentSection = line.substr(1, line.length() - 2);
		}
		else 
        {
			// 键值对
			std::size_t equalsPos = line.find('=');
			if (equalsPos != std::string::npos) 
            {
				std::string key = line.substr(0, equalsPos);
				std::string value = line.substr(equalsPos + 1);
                mMapConfigValue[currentSection][key] = value;
			}
		}
	}

	file.close();
    return true;
}

std::string AppConfig::getValue(const std::string& section, const std::string& key)
{
    auto iterA = mMapConfigValue.find(section);
    if (iterA == mMapConfigValue.end()) return "";

    auto iterB = iterA->second.find(key);
    if (iterB == iterA->second.end()) return "";
    
    return iterB->second;
}
