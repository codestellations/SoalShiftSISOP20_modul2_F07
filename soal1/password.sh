#!/bin/bash

genpass() #a function to generate random password
{
  pass=$(head /dev/urandom | tr -dc A-Za-z0-9 | head -c 28)
}

i=0

while [ $i -eq 0 ]
do
	if [[ $pass =~ [A-Z] ]]
	then
		if [[ $pass=~[a-z] ]]
		then
			if [[ $pass=~[0-9] ]]
			then i=1
			else genpass
			fi
		else genpass
		fi
	else genpass
fi
done

echo $pass >> "/home/el/password/straykids.txt"
