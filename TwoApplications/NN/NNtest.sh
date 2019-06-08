#!/bin/bash
rm ../NNtest_$1_result.txt
for((b=32;b<=32;b=b*2))
do
	for((k=5;k<=10;k=k*2))
	do
		for((copynum=5;copynum<=5;copynum++))
		do
			echo b=$b k=$k copynum=$copynum >> ../NNtest_$1_result.txt
			./NNtest $1 $b $k $copynum >> ../NNtest_$1_result.txt
			echo -e "\n" >> ../NNtest_$1_result.txt
		done
	done
done
