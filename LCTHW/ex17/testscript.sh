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
echo -e "\nCreate DB\n"
./$1 x.txt c 4 7
status=$?
check_status $1 $?
echo
xxd -c 8 x.txt

echo -e "\n\nSave first entrie\n"
./$1 x.txt s 0 Tom Tmail 45 23 Rukla
status=$?
check_status $1 $?
xxd -c 8 x.txt

echo -e "\n\nSave second entrie\n"
./$1 x.txt s 1 Dick Stick 10 1 Skuodas
status=$?
check_status $1 $?
xxd -c 8 x.txt

echo -e "\n\nSave third entrie\n"
./$1 x.txt s 2 Will Roboinson
status=$?
check_status $1 $?
xxd -c 8 x.txt

echo -e "\n\nSave fourth entrie\n"
./$1 x.txt s 3 Steve Jhon 8
status=$?
check_status $1 $?
xxd -c 8 x.txt

echo -e "\n\nList all entries\n"
./$1 x.txt l
status=$?
check_status $1 $?

echo -e "\n$1 exit status is $status\n"

xxd -c 8 x.txt
echo

# save entries to db

