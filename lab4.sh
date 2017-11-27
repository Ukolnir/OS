#! /bin/bash

if [ $# -eq 0 ]; then
echo "Script is worked"
fi

if [ $# -gt 1 ]; then
echo "Incorrect count argument"
fi

if [ -f $1 ]; then
grep -v "^$" $1 >> temp.txt
cat temp.txt > $1
rm temp.txt
else
echo "Invalid argument"
fi
#Проверить файл текстовый, и что он вообше существует. Также подумать о том, 
#что он файл ввода, для вывода нежизнеспособен 
exit
