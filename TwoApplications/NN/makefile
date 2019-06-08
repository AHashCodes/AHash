CC=g++
all:NNtest #testsim
.PHONY:all

NNtest:NNtest.o
	${CC} -o NNtest NNtest.o -O3
NNtest.o:NNtest.cpp ../BOBHash32.h ../MinHash.h ../OOPH.h ../SplitOddEven.h
	${CC} -c NNtest.cpp
clean:
	rm *.o
	rm *.h.gzh
