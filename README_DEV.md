# 初始编译步骤

#### 安装 crypto++ 库

sudo apt install libcrypto++-dev

#### 安装 mysql 库

sudo apt-get install libmysqlclient-dev

#### 安装 Hp-Socket 库

下载 hp-socket 源码，进入 Linux 文件夹下的 Script 文件夹，修改 compile.sh 配置，运行

#### 查看 so 库是否存在 debug 信息

readelf -S libxxx.so | grep debug

#### 安装 spdlog 库

sudo apt install libspdlog-dev

#### 注意事项

编译的时候需要加 sudo 选项，否则编译进度显示不正常,也可以先给文件夹授权
sudo chmod 777 -R ./RealTimePositiobService

