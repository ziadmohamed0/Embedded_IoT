#!/bin/bash

echo "---------- Welcome in Payment Program ----------"
echo "for build project enter {1} for run project enter {2}"
printf "Enter you choise: "
read choise

if [ ${choise} -eq 1 ]; then
    bazel build //:main
elif [ ${choise} -eq 2 ]; then
    bazel build //:main
    ./bazel-bin/main
fi