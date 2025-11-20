#!/bin/bash
#SBATCH --job-name=MergeSort
#SBATCH --partition=Centaurus
#SBATCH --time=01:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=8

set -e

cd $SLURM_SUBMIT_DIR

make clean
make

RESULTS_FILE="results.txt"
rm -f $RESULTS_FILE

echo "Merge Sort Benchmark Results" > $RESULTS_FILE
echo "-----------------------------------" >> $RESULTS_FILE

THRESHOLD=1000

for threads in 1 2 4 8; do
  echo "--- Testing with $threads threads ---" | tee -a $RESULTS_FILE
  
  export OMP_NUM_THREADS=$threads

  echo "Running Small Array (n=100,000, threshold=$THRESHOLD)..."
  echo "Small Array ($threads threads):" >> $RESULTS_FILE
  (time ./mergesort 100000 $threads $THRESHOLD > /dev/null) 2>> $RESULTS_FILE
  echo "" >> $RESULTS_FILE

  echo "Running Medium Array (n=10,000,000, threshold=$THRESHOLD)..."
  echo "Medium Array ($threads threads):" >> $RESULTS_FILE
  (time ./mergesort 10000000 $threads $THRESHOLD > /dev/null) 2>> $RESULTS_FILE
  echo "" >> $RESULTS_FILE

  echo "Running Large Array (n=50,000,000, threshold=$THRESHOLD)..."
  echo "Large Array ($threads threads):" >> $RESULTS_FILE
  (time ./mergesort 50000000 $threads $THRESHOLD > /dev/null) 2>> $RESULTS_FILE
  echo "" >> $RESULTS_FILE

done
