gcc TestFuture.c future.c -lpthread  -o run_future

valgrind ./run_future
#valgrind --tool=helgrind ./run_future