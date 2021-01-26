gcc -g testChannel.c channel.c ./array_queue/array_queue.c -lpthread -o run_chan # -fsanitize=address

./run_chan


valgrind ./run_chan