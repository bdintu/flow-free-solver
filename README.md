# flow-free-solver
Artificial intelligence assigment

## Reference
[Flow Free solver - matt_zucker](https://mzucker.github.io/2016/08/28/flow-solver.html)

## Install
```sh
apt install build-essential
```

## Clone
```sh
git clone https://github.com/bdintu/flow-free-solver
cd flow-free-solver
```

## Build
```sh
g++ main.cc
```

## Run
```sh
./a.out <path-of-matrix> <algorithms>
```

### Algorithm
- 0 - BFS
- 1 - DFS
- 2 - Heuristic

### Example
```
./a.out example/m1.txt 0
./a.out example/m1.txt 1
./a.out example/m1.txt 2
```
