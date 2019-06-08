CC=g++
svmtest:svmtest.o
	${CC} -o svmtest svmtest.o -O3
svmtest.o:svmtest.cpp ../BOBHash32.h ../MinHash.h ../OOPH.h ../SplitOddEven.h
	${CC} -c svmtest.cpp
clean:
	rm *.o
	rm *.h.gzh
