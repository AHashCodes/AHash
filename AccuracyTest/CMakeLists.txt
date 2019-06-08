#project name
PROJECT(Permutation)

include_directories(.)
add_definitions(-std=c++11 -O3)

AUX_SOURCE_DIRECTORY(. DIR_SRCS)

#add executable file
ADD_EXECUTABLE(Permutation ${DIR_SRCS})
