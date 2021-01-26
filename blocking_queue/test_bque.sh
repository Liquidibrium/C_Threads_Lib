gcc working_pool.c ./blocking_queue.c ./linked_queue/linked_queue.c -lpthread -Wall -o run_bque 

./run_bque

valgrind ./run_bque

valgrind --tool=helgrind ./run_bque

