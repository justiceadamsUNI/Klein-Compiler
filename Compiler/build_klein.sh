#!/bin/bash
kleins (){
	buffer="$buffer -s --kleins"
	echo "Creating kleins.  Please Wait."
	g++ -std=gnu++11 -w -o kleins src/BuildTokens.cpp src/implementation/*.cpp
	echo "kleins has been created."
}
blank (){
	echo "Functionality coming soon!"
}
kleinf (){
	buffer="$buffer -f --kleinf"
	echo "Creating kleinf.  Please Wait."
	g++ -std=gnu++11 -w -o kleinf src/ProgramValidator.cpp src/implementation/*.cpp
	echo "kleinf has been created."
}
kleinp (){
	buffer="$buffer -p --kleinp"
	echo "Creating klienp. Please Wait."
	g++ -std=gnu++11 -w -o kleinp src/PrintASTNodeTree.cpp src/implementation/*.cpp
	echo "kleinp has been created"
}
kleinv (){
	buffer="$buffer -v --kleinv"
	echo "Creating kleinv. Please Wait."
	g++ -std=gnu++11 -w -o kleinv src/SemanticValidator.cpp src/implementation/*.cpp
	echo "kleinv has been created."
}
kleinc (){
	buffer="$buffer -c --kleinc"
	blank
}
i=0
buffer=""
if [ "$#" -eq "0" ]; then
echo "Please enter a command line argument"
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

