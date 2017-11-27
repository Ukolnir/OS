#! /bin/bash

for ((i=0; i<20; i++))
do
  let "number = 1000 + $RANDOM % 1100"  
  #echo $number  
  if [[ $(( $number % 100 )) -ne 0 ]] && [[ $(( $number % 4 )) -eq 0 ]] || 
[[ $(( $number % 400 )) -eq 0 ]]
  then
    echo "$number - TRUE"
  else
    echo "$number - FALSE"
  fi 
done