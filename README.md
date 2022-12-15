<h1 align='center'>A Feedback Vertex Set Solver</h1>

This repository provides a feedback vertex set solver, inspried by the [PACE 2022](https://pacechallenge.org/2022/) challenge.

> Given a directed Graph $G=(V,E)$ one aims to find a *minimal* vertex set $FVS(G) \subset V$ such that $G \setminus FVS(G)$ is acyclic.

Note that this is not the most general form of the feedback vertex set problem, as no weights on edges are assumend (and currently not supported). This case fits in the more general setting by the special choice of uniform weights on all edges, i.e. $w(e) = 1 \ \forall e \in E $.

## Build

The project is build using `cmake`. The usage of the build tool [`ninja`](https://ninja-build.org/) is supported (and encouraged). To build the project run
```
cmake . && make
```
or
```
cmake . -GNinja && ninja
```
respectively.

## Dependencies
This project relies on

* [SCIP](https://www.scipopt.org/) (ILP solvers for the FVS and VC (auxiliary) problem)
* [SoPlex](https://soplex.zib.de/) (SCIP backend solver)
* [googletest](https://github.com/google/googletest) (unit-testing framework of choice)
* [google-benchmark](https://github.com/google/benchmark) (benchmarking framework of choice)

## Code/Algorithm Overview
Coming soon...

Paper for overview and general reductions [[1]](#1).
Paper for DOUBLE and further reductions [[2]](#2).
Paper for VC reductions [[3]](#3).


## References
<a id="1">[1]</a> Mile Lemaic (2008). Markov-Chain-Based Heuristics for the Feedback Vertex Set Problem for Digraphs. https://kups.ub.uni-koeln.de/2547/1/Dissertation.pdf

<a id="2">[2]</a> Orenstein, T., Kohavi, Z. & Pomeranz, I. (1995). An optimal algorithm for cycle breaking in directed graphs, https://doi.org/10.1007/BF00993315

<a id="3">[3]</a> Plachetta, Grinten, SAT-and-Reduce for Vertex Cover: Accelerating Branch-and-Reduce by SAT Solving, https://doi.org/10.1137/1.9781611976472.13