# Build System

The project is build using `cmake`. The usage of the build tool [ninja](https://ninja-build.org/) is supported (and encouraged). To build the project run
```
cmake . && make
```
or
```
cmake . -GNinja && ninja
```
respectively.

## Dependencies
Dependencies are all included using cmake's external project infrastructure. All dependencies are fetched and build during the build process, thus initial compilation time is quite massive. This project relies on

* [google-benchmark](third-party/google-benchmark/) (v1.6.1)
* [googletest](third-party/gtest/) (1.8.0)
* [nlohmann-json](third-party/json/) (v3.10.5)
* [OGDF](third-party/ogdf/) (v2020.02 Catalpa)
* [SCIP](third-party/scip/) (v800)
* [SoPlex](third-party/soplex/) (v600)


# Code Overview
Coming soon...


# Tooling

## Unit Testing
The code is tested with the [google-benchmark](https://github.com/google/benchmark) unit testing framework. Unit tests are build by default and can be exectuted with `ctest`. To disable build of tests pass the (standard) cmake flag `BUILD_TESTING=OFF`.

## Code Format

The source code is formatted using `clang-format` with the format specified in [.clang-format](.clang-format) (mozilla format with increased spacing).

To apply this code format run
```
find . -iname *.hpp -o -iname *.cpp -o -iname *.h | xargs clang-format -style=file -i
```

## Static Analysis
Coming soon...

## Custom Tools
Coming soon...

## Citings
Paper for overview and general reductions [[1]](#1).
Paper for DOUBLE and further reductions [[2]](#2).
Paper for VC reductions [[3]](#3).


## References
<a id="1">[1]</a> Mile Lemaic (2008). Markov-Chain-Based Heuristics for the Feedback Vertex Set Problem for Digraphs. https://kups.ub.uni-koeln.de/2547/1/Dissertation.pdf

<a id="2">[2]</a> Orenstein, T., Kohavi, Z. & Pomeranz, I. (1995). An optimal algorithm for cycle breaking in directed graphs, https://doi.org/10.1007/BF00993315

<a id="3">[3]</a> Plachetta, Grinten, SAT-and-Reduce for Vertex Cover: Accelerating Branch-and-Reduce by SAT Solving, https://doi.org/10.1137/1.9781611976472.13