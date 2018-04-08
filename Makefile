CC = gcc -Wall -Wextra -ansi -Werror
Lib = librairies/
Test = tests/

all : honshu_A honshu_B

vg_a: honshu_A u_a
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./unit_A

vg_b: honshu_B
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./honshu_B

u_a: test.o tuile.o grille.o $(Test)tests_unitaires_A.c
	$(CC) $^ -g -o unit_A -lcunit

tests_a: honshu_A
	./honshu_A

honshu_A: test.o tuile.o grille.o $(Test)tests_A.c
	$(CC) $^ -g -o $@ -lcunit

honshu_B:  test.o tuile.o grille.o $(Lib)game.c
	$(CC) $^ -g -o $@ -lncurses

%.o : $(Lib)%.c $(Lib)%.h
	$(CC) -c -g $< -o $@

doxygen: Doxygen
Doxygen :
	doxygen Doxyfile

clean: Clean
Clean :
	if [ -d latex ]; then rm -rf latex; fi
	if [ -d honshu_B ]; then rm honshu_B; fi
	if [ -d html ]; then rm -rf html; fi
	if [ -e honshu_A ]; then rm honshu_A; fi
	find ./ -maxdepth 1 -type f -name "*.o" -delete 
	find ./ -type f -name "*.swp" -delete 
