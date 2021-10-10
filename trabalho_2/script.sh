../../../../../Downloads/jdk-16.0.2/bin/javac Main.java

echo "compiled"

for THREAD in 1 2 4 10 20 50 100
do
    echo "Threads, Execution Time" >> "../stats/stats_${THREAD}.csv"
    for I in $(seq 1 5)
    do
        ../../../../../Downloads/jdk-16.0.2/bin/java Main $THREAD 10000  ../input_file_2000_100000.txt "../out/output_file_${THREAD}_${I}" >> "../stats/stats_${THREAD}.csv"

        rm -rf "../out/output_file_${THREAD}_${I}" # deleting the output file

        echo "Thread: $THREAD executed ${I} times"
    done
done