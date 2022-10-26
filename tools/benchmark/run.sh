NP=55
# TIMEOUT=1800 # 30 minutes
TIMEOUT=10

mkdir -p data
for SOLVER in naive reduce_deg_1 reduce_deg_1_scc reduce; do
    mkdir -p data/$SOLVER

    seq -f "%03g" 1 2 199 | xargs --max-procs=$NP -I [] sh -c "{ /usr/bin/time -f \"%e\" timeout --preserve-status $TIMEOUT ../../build/$SOLVER ../../build/tracks/e_[] > /dev/null; } 2> data/$SOLVER/[];"
done
