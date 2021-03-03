```bash
# create executables
gcc parallel_task.c -o parallel -fopenmp -lm
gcc serial.c -o serial -fopenmp -lm

# execute
./serial 2>&1 | tee serial_result.txt
./parallel 2>&1 | tee parallel_result.txt
```
