#!/bin/bash
_passback() { while [ 0 -lt $# ]; do printf '%q=%q;' "$1" "${!1}"; shift; done; }

#seed=("4137502622" "357668081" "658417210" "3103433381" "1938583126" "2581622833")
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
	echo "./labyrinth-server -s ${seed[i]} ${1} ${2}"
	./labyrinth-server -s ${seed[i]} $1 $2 >> /dev/null 2>> out.txt
	check_output
	rm out.txt
done
echo "${1} algo win ${firstCount} times"
echo "${2} algo win ${secondCount} times"
if [ $firstCount -gt $secondCount ]
then
	echo "${1} Win"
elif [ $firstCount -lt $secondCount ]
then
	echo "${2} Win"
else
	echo "Equality"
fi

firstCount=0
secondCount=0
cp=100

while [ "$cp" != 0 ]
do 
	echo "./labyrinth-server ${1} ${2}"
	./labyrinth-server $1 $2 >> /dev/null 2>> out.txt
	check_output
	rm out.txt
	cp=$(($cp-1))
done
echo "${1} algo win ${firstCount} times"
echo "${2} algo win ${secondCount} times"
if [ $firstCount -gt $secondCount ]
then
	echo "${1} Win"
elif [ $firstCount -lt $secondCount ]
then
	echo "${2} Win"
else
	echo "Equality"
fi
