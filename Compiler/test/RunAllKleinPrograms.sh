#!/bin/bash
for i in `ls ../programs/`;
do 
	echo "****************************************"
	echo "PROGRAM: $i"
	echo "****************************************"
	../kleinv ../programs/$i;
done;
