#!/bin/bash

name_dir="build"
name_exe="main"
sucsess_massage="programm build successfully"

rm -rf $name_dir

mkdir $name_dir
cd $name_dir
cmake ..
make

echo $sucsess_massage
#./$name_exe