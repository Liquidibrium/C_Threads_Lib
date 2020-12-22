gcc TestFuture.c future.c -lpthread  -o futTest

valgrind ./futTest
valgrind --tool=helgrind ./futTest