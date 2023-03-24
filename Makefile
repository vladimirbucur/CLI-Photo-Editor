build:
	gcc -Wall -Wextra -std=c99 main.c commands.c rotate.c filtres.c auxiliary.c -o image_editor -lm

run:
	valgrind ./image_editor

clean:
	rm image_editor

.PHONY: clean