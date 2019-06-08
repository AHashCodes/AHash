## Runtime Experiment

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
