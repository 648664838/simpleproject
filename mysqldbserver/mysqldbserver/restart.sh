#! /bin/sh
./stop.sh
./mysimpledb -D
ps aux | grep mysimpledb