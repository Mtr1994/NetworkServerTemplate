#!/bin/bash

echo "copy dynamic library into target place ..."

cp $PWD/../sdk/crypto++/lib/lib* $PWD/
cp $PWD/../sdk/hpsocket/lib/lib* $PWD/
cp $PWD/../sdk/mysqlclient/lib/lib* $PWD/
cp $PWD/../sdk/spdlog/lib/lib* $PWD/

echo "done."