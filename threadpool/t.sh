gcc -Wall tpTest.c ThreadPool.c ../blocking_queue/linked_queue/linked_queue.c ../blocking_queue/blocking_queue.c -lpthread -o tp

valgrind  --tool=helgrind  ./tp

valgrind  ./tp 
