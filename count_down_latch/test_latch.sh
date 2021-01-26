gcc latchTest.c -lpthread count_down_latch.c -o run_latch

valgrind ./run_latch