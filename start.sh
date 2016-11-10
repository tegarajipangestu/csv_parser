#!/bin/bash

arr=("$1*.csv")


for filename in $arr; do
 ./csv_parser $filename
done
