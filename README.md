## AHash: A Load-Balanced One Permutation Hash

## Datasets

https://www.csie.ntu.edu.tw/~cjlin/libsvmtools/datasets/binary.html
1）RCV1
2）NEWS20
3）URL
## Accuracy Experiment

---

This is the source code for the runtime experiment

#### Files

---

+ BOBHash32.h : The headerfile for the construction of multiple hash functions

+ MinHash.h : Our implementation of Minhash

+ OOPH.h : Our implementation of OOPH

+ SplitMaxMin.h : Our implementation of Amortization Hash using the strategy of keeping both the Max value and the Min value in each bin.

+ SplitOddEven.h : Our implementation of Amortization Hash using the strategy of keeping both the Odd value and the Even value in each bin.

+ Standard.h : The headerfile for the calculation of accurate similarity

+ test-mse.cpp : The test code used for comparing the mse of above implementations

#### Building from source

---

```bash
mkdir build && cd build
cmake ../
make
```

#### Running the test program

---

```bash
./Permutation data_path
```

**Note**

1. The *data_path* denotes the path of your test data

2. You may need to use the command *ulimit* to set a proper stack size since the huge data is read into memory

## Speed Experiment

------

This is the source code for the runtime experiment

#### Files

------

- BOBHash32.h : The headerfile for the construction of multiple hash functions
- MinHash.h : Our implementation of Minhash
- OOPH.h : Our implementation of OOPH
- SplitMaxMin.h : Our implementation of Amortization Hash using the strategy of keeping both the Max value and the Min value in each bin.
- SplitOddEven.h : Our implementation of Amortization Hash using the strategy of keeping both the Odd value and the Even value in each bin.
- Standard.h : The headerfile for the calculation of accurate similarity
- test-runtime.cpp : The test code used for comparing the running times of above implementations

#### Building from source

------

```bash
mkdir build && cd build
cmake ../
make
```

#### Running the test program

------

```bash
./Permutation data_path
```

**Note**

1. The *data_path* denotes the path of your test data
2. You may need to use the command *ulimit* to set a proper stack size since the huge data is read into memory## Runtime Experiment

---

This is the source code for the runtime experiment

#### Files

---

+ BOBHash32.h : The headerfile for the construction of multiple hash functions

+ MinHash.h : Our implementation of Minhash

+ OOPH.h : Our implementation of OOPH

+ SplitMaxMin.h : Our implementation of Amortization Hash using the strategy of keeping both the Max value and the Min value in each bin.

+ SplitOddEven.h : Our implementation of Amortization Hash using the strategy of keeping both the Odd value and the Even value in each bin.

+ Standard.h : The headerfile for the calculation of accurate similarity

+ test-runtime.cpp : The test code used for comparing the running times of above implementations

#### Building from source

---

```bash
mkdir build && cd build
cmake ../
make
```

#### Running the test program

---

```bash
./Permutation data_path
```

**Note**

1. The *data_path* denotes the path of your test data
2. You may need to use the command *ulimit* to set a proper stack size since the huge data is read into memory


## Leanring adn Searching

------

This is the source code for SVM  Leanring adn Near neighbour Searching.

#### Files

------

- BOBHash32.h : The headerfile for the construction of multiple hash functions
- MinHash.h : Our implementation of Minhash
- OOPH.h : Our implementation of OOPH
- SplitMaxMin.h : Our implementation of Amortization Hash using the strategy of keeping both the Max value and the Min value in each bin.
- SplitOddEven.h : Our implementation of Amortization Hash using the strategy of keeping both the Odd value and the Even value in each bin.

#### Near Neighbour Search

------
input:
input dataresource, b (for b bit), hashnum(for how mant bins will MinHash,OOPH and SplitOddEven have during the test) as command line parameters
output:
the F1-score of the three hashing strategies averaged over 2000 queris

#### SVM

------
input:
input dataresource, b (for b bit), hashnum(for how mant bins will MinHash,OOPH and SplitOddEven have during the test) as command line parameters
output:
output three datasets that are produced by processing the original dataset using MinHash, OOPH and SplitOddEven

