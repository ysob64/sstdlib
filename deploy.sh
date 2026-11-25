#!/bin/bash

#Deploy mini v1

logs=$(gcc test.c -o run.exec 2>&1)

if [ -z "$logs" ]; then
    ./run.exec
else
    echo "$logs"
fi