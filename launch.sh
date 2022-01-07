#!/bin/bash

#seed=("4137502622" "357668081" "658417210" "3103433381" "1938583126" "2581622833" "2813222724" "1902999301" "677638891" "2424358276" "1530873975")
firstCount=0
secondCount=0


check_output()
{
see0=0
see1=0
while read ligne
do
  if [[ $ligne =~ "[0]" ]]
  then
  	((see0=$see0 + 1))
  fi
  if [[ $ligne =~ "[1]" ]]
  then
  	((see1=$see1 + 1))
  fi
  if [ "$see0" = "2" ]
  then
  	firstCount=$(( $firstCount + 1 ))
  	break
  elif [ "$see1" = "2" ]
  then
  	secondCount=$(( $secondCount + 1 ))
  	break
  fi
done <<<$(cat out.txt)
}

for (( i=0; i < ${#seed[*]}; i++ ))
do 
	#echo "./labyrinth-server -s ${seed[i]} ${1} ${2}"
	./labyrinth-server -s ${seed[i]} $1 $2 >> /dev/null 2>> out.txt
	check_output
	rm out.txt
done
echo "${1} algo win ${firstCount} times"
echo "${2} algo win ${secondCount} times"
if [ $firstCount -gt $secondCount ]
then
	echo -e "\e[32m${1} Win\e[0m"
elif [ $firstCount -lt $secondCount ]
then
	echo -e "\e[34m${2} Win\e[0m"
else
	echo -e "\e[35mEquality\e[0m"
fi

firstCount=0
secondCount=0
cp=200

while [ "$cp" != 0 ]
do 
	echo -en "\rTest restants: ${cp} ./labyrinth-server ${1} ${2}"
	./labyrinth-server $1 $2 >> /dev/null 2>> out.txt
	check_output
	rm out.txt
	cp=$(($cp-1))
done

cp=200

while [ "$cp" != 0 ]
do 
	echo -en "\rTest restants: ${cp} ./labyrinth-server ${2} ${1}"
	./labyrinth-server $1 $2 >> /dev/null 2>> out.txt
	check_output
	rm out.txt
	cp=$(($cp-1))
done

echo ""
echo "${1} algo win ${firstCount} times"
echo "${2} algo win ${secondCount} times"
if [ $firstCount -gt $secondCount ]
then
	echo -e "\e[32m${1} Win\e[0m"
elif [ $firstCount -lt $secondCount ]
then
	echo -e "\e[34m${2} Win\e[0m"
else
	echo -e "\e[35mEquality\e[0m"
fi

