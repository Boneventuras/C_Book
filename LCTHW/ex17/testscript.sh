#!/bin/bash
echo -e "\nRunning test for $1\n"

check_status(){
if [ "$2" != "0" ]
	then
		echo -e "\n$1 exit status is $2\n"
		exit
fi
}

# create db
./$1 x.txt c 4 7
status=$?
check_status $1 $?
xxd -c 8 x.txt

./$1 x.txt s 0 Tom Tmail 45 23 Rukla
status=$?
check_status $1 $?
xxd -c 8 x.txt

./$1 x.txt s 1 Dick Stick 10 1 Skuodas
status=$?
check_status $1 $?
xxd -c 8 x.txt

./$1 x.txt s 2 Will Robot
status=$?
check_status $1 $?
xxd -c 8 x.txt

./$1 x.txt s 3 Steve Jhon 8
status=$?
check_status $1 $?
xxd -c 8 x.txt

./$1 x.txt l
status=$?
check_status $1 $?

echo -e "\n$1 exit status is $status\n"

xxd -c 8 x.txt

# save entries to db

