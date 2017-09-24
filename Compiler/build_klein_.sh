#!/bin/bash
kleins (){
	buffer="$buffer -s --kleins"
	echo "Creating kleins.  Please Wait."
	g++ -std=gnu++11 -o kleins src/BuildTokens.cpp src/implementation/*.cpp
	echo "kleins has been created."
}
blank (){
	echo "Functionality coming soon!"
}
kleinf (){
	buffer="$buffer -f --kleinf"
	blank
}
kleinp (){
	buffer="$buffer -p --kleinp"
	blank
}
kleinv (){
	buffer="$buffer -v --kleinv"
	blank
}
kleinc (){
	buffer="$buffer -c --kleinc"
	blank
}
i=0
buffer=""
if [ "$#" -eq "0" ]; then
echo "Please enter a command line argumet"
else
for ((i=1; i<= $#; i++))
do
	if [[ $buffer =~ .*${!i}.* ]]
	then
		continue
	else
	case ${!i}
	in
		-s) kleins;;
		--kleins) kleins;;
		-f) kleinf;;
		--kleinf) kleinf;;
		-p) kleinp;;
		--kleinp) kleinp;;
		-v) kleinv;;
		--kleinv) kleinv;;
		-c) kleinc;;
		--kleinc) kleinc;;
		*) echo "${!i}: Not an acceptible command.";;
	esac
	fi
done
fi

