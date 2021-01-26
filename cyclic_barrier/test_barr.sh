gcc barrTest.c cyclic_barrier.c cyclic_barrier.h -lpthread -o run_barr

#./run_barr
valgrind ./run_barr