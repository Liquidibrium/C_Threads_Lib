gcc -g testChannel.c channel.c ./array_queue/array_queue.c -lpthread -o chanTest # -fsanitize=address

./chanTest
echo " "

valgrind ./chanTest