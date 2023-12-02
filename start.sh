#!/bin/bash

path="./build"
name="main"
check_file=$path
check_file+="/"
check_file+=$name
file_exist_massage="Project has not built. Start build.sh to build project!"

if test -f $check_file; then
  cd $path
  ./$name

else
    echo $file_exist_massage
fi