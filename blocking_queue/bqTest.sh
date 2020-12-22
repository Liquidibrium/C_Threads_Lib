gcc working_pool.c ./blocking_queue.c ./linked_queue/linked_queue.c -lpthread -Wall -o bq 

./bq

valgrind ./bq

valgrind --tool=helgrind ./bq

