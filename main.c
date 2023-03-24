//Copyright Bucur Vladimir 311CA (vladimirbucur1@gmai.com)

#include "main_functions.h"
#include "auxiliary_functions.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#define COMMAND_MAX 100
#define FILE_NAME_MAX 101

int main(void)
{
	pixel **img = NULL;
	file_image image_p;
	char command[COMMAND_MAX + 1], file_name[FILE_NAME_MAX + 1];

	// Se citesc si se efectueaza comenzi pana la intalnirea "EXIT"
	// La fiecare input se verifica daca este o comanda existenta,
	//iar daca este se executaT"
	fgets(command, COMMAND_MAX, stdin);
	while (!((strstr(command, "EXIT") - command) == 0)) {
		strcpy(command + strlen(command) - 1, command + strlen(command));
		if ((strstr(command, "LOAD") - command) == 0) {
			filename(command, file_name);
			img = load(file_name, img, &image_p);
			if (img)
				select_all(&image_p);
		} else if (img) {
			// Inainte de toate se verifica daca este o vreo imagine uploadata,
			//iar in caz contrar se afiseaza mesajul "No image loaded"
			if ((strstr(command, "SAVE") - command) == 0) {
				filename(command, file_name);
				save(file_name, img, &image_p, command);
			} else if ((strstr(command, "SELECT ALL") - command) == 0) {
				select_all(&image_p);
				printf("Selected ALL\n");
			} else if ((strstr(command, "SELECT") - command) == 0) {
				select_matrix(&image_p, command);
			} else if ((strstr(command, "CROP") - command) == 0) {
				img = crop(img, &image_p);
				select_all(&image_p);
				//printf("%d %d\n", image_p.height, image_p.width);
			} else if ((strstr(command, "ROTATE") - command) == 0) {
				img = rotate(img, &image_p, command);
			} else if ((strstr(command, "APPLY") - command) == 0) {
				filtres(img, &image_p, command);
			} else {
				// In cazul in care inputul nu este bun se afiseaza
				//"Invalid command"
				printf("Invalid command\n");
			}
		} else {
			printf("No image loaded\n");
		}
		fgets(command, COMMAND_MAX, stdin);
	}
	if (img)
		free_matrix(img, image_p.height);
	else
		printf("No image loaded\n");
	return 0;
}
