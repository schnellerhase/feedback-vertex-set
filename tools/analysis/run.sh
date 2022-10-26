NP=55
TIMEOUT=1800 # 30 minutes

mkdir -p data
for SOLVER in analysis; do

    seq -f "%03g" 1 2 009 | xargs --max-procs=$NP -I [] sh -c "{ /usr/bin/time -f \"%e\" timeout --preserve-status $TIMEOUT ../../build/tools/analysis/$SOLVER ../../tracks/e_[] > /dev/null; }"
done