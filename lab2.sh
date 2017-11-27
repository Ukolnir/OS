#! /bin/bash

if [ $# -eq 0 ]; then
echo "Script is worked"
fi

for A in $@; 
do  
if [ -f $A ]; then 
ls -l $A
else
echo "It is not file or not exist"
fi
done
