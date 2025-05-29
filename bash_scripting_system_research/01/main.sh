#!/bin/bash

# task
# Write a bash script. The script is run with one parameter. It is a text parameter.
# The script outputs the value of the parameter.
# If the parameter is a number, the script must output an invalid input message.

if [[ $1 =~ ^-?[0-9]+$ ]]; then
    echo "invalid input message"
else
    echo "$1"
fi