mkdir -p build
cd build

cmake .. -GNinja -DCMAKE_BUILD_TYPE=Release -DBUILD_BENCHMARKS=ON 
ninja benchmark_solvers
./benchmark_solvers --benchmark_format=json | tee benchmark_result_current.json

git checkout origin/main
cmake .. -GNinja -DCMAKE_BUILD_TYPE=Release -DBUILD_BENCHMARKS=ON 
ninja benchmark_solvers
./benchmark_solvers --benchmark_format=json | tee benchmark_result_main.json

