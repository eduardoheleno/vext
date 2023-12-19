gcc src/main.c src/vext.c src/utils.c -o main -L lib/ -I include/ -lncurses && ./main test.txt
