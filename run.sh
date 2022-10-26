# echo Fetching Tracks...
# rm -rf tracks/
# wget -q --show-progress -O tmp.tar.gz https://heibox.uni-heidelberg.de/f/be4337d9e4234bca8606/?dl=1
# tar -xzf tmp.tar.gz
# rm tmp.tar.gz
# mv exact_public/ tracks/
NP=1

for TRACK_NO in {001..200..2}; do
    # echo Running Track No $TRACK_NO...
    # build/solver_naive/solver_naive tracks/e_$TRACK_NO > /dev/null

    # echo Verifiying Track No $TRACK_NO...
    # build/verifier tracks/e_$TRACK_NO tracks/e_$TRACK_NO.sol || { echo Track $TRACK_NO failed! Exiting! ; exit 1; }

    echo Analyzing Track No $TRACK_NO...
    build/tools/analysis/analysis tracks/e_$TRACK_NO

# echo Computing reduced components of Track No $TRACK_NO...
# seq -f "%03g" 1 2 199 | xargs --max-procs=$NP -I [] sh -c "./build/tools/analysis/analysis tracks/e_[]"

# build/tools/components/components tracks/e_$TRACK_NO
done
