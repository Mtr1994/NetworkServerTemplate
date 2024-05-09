#!/bin/bash

value=`ps -ef | grep rtps | grep -v grep`

echo "start searching the rtps process"
if [ ${#value} -lt 30 ]; then
	echo "these are no active rtps process"
    	exit 0
fi

echo -e "\r"
while [ ${#value} -gt 30 ]; do
	items=(${value// / }) 
	echo "the process id is ${items[1]}"
	kill -9 ${items[1]}
	value=`ps -ef | grep rtps | grep -v grep`
done
echo -e "\r"
echo "the rtps process has been killed"
