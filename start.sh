#!/bin/bash

arr=("$1*.csv")

for filename in $arr; do
  echo $filename
  echo $1
 ./csv_parser $filename
done
