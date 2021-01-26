gcc -Wall tpTest.c thread_pool.c ../blocking_queue/linked_queue/linked_queue.c ../blocking_queue/blocking_queue.c -lpthread -o run_tpool

valgrind  --tool=helgrind  ./run_tpool

valgrind  ./run_tpool 
