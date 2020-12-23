gcc latchTest.c -lpthread countDownLatch.c -o test

valgrind ./test