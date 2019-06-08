#!/bin/bash
svmdir="../../data/liblinear-2.30"
datadir="../../data"
hashdir="."
rm ../svm_$1_result.txt
carray=(0.001 0.01 0.1 1 10 100)
for((k=150;k<=150;k+=50))
do
	for((b=8;b<=8;b=b*2))
	do
	  for((cindex=0;cindex<=5;cindex+=1))
	  do
  		echo c=${carray[$cindex]},b=$b,k=$k >> ../svm_$1_result.txt
		$hashdir/svmtest $1 $b $k 
		$svmdir/train -c ${carray[$cindex]} -p 0.0001 -e 0.0001 $datadir/$1.binary $datadir/model/$1_origin.model
		$svmdir/train -c ${carray[$cindex]} -p 0.0001 -e 0.0001 $datadir/$1_minhash.binary $datadir/model/$1_minhash.model
		$svmdir/train -c ${carray[$cindex]} -p 0.0001 -e 0.0001 $datadir/$1_oophhash.binary $datadir/model/$1_oophhash.model
		$svmdir/train -c ${carray[$cindex]} -p 0.0001 -e 0.0001 $datadir/$1_splitoehash.binary $datadir/model/$1_splitoehash.model
		
		echo orgin: >> ../svm_$1_result.txt
		$svmdir/predict  $datadir/$1.binary $datadir/model/$1_origin.model $datadir/output/$1_origin.output >> ../svm_$1_result.txt
		
		echo minhash: >> ../svm_$1_result.txt
		$svmdir/predict  $datadir/$1_minhash.binary $datadir/model/$1_minhash.model $datadir/output/$1_minhash.output >> ../svm_$1_result.txt
		
		echo oophhash: >> ../svm_$1_result.txt
		$svmdir/predict  $datadir/$1_oophhash.binary $datadir/model/$1_oophhash.model $datadir/output/$1_oophhash.output >> ../svm_$1_result.txt
		
		echo splitoehash: >> ../svm_$1_result.txt
		$svmdir/predict  $datadir/$1_splitoehash.binary $datadir/model/$1_splitoehash.model $datadir/output/$1_splitoehash.output >> ../svm_$1_result.txt

		echo -e "\n" >> ../svm_$1_result.txt
	  done
	done
done
