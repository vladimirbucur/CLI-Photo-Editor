//Copyright Bucur Vladimir 311CA (vladimirbucur1@gmai.com)

#include "main_functions.h"
#include "auxiliary_functions.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#define COMMAND_MAX 100

// Functie ce retine in vectorul name numele fisierului mentionat in comanda
void filename(char command[], char name[])
{
	int no = 0;
	char aux[COMMAND_MAX], *word;
	strcpy(aux, command);
	word = strtok(aux, " ");
	while (word) {
		no++;
		if (no == 2)
			strcpy(name, word);
		word = strtok(0, " ");
	}
}

// Functie ce returneaza un numar specific pentru fiecare magic word in parte
int MW_type(file_image *image_p)
{
	if (strcmp(image_p->magic_world, "P2") == 0)
		return 2;

	if (strcmp(image_p->magic_world, "P3") == 0)
		return 3;

	if (strcmp(image_p->magic_world, "P5") == 0)
		return 5;

	return 6;
}

// Functie ce returneaza 1 in caz ca imaginea este colorata si 0 altfel
int is_colored(file_image *image_p)
{
	if (MW_type(image_p) == 2 || MW_type(image_p) == 5)
		return 0;
	return 1;
}

// Functie ce aloca memorie pentru o matrice in functie daca imaginea
//este colorata sau nu, careia i se specifica dimensiunile
pixel **alloc_matrix(file_image *image_p, int height, int width)
{
	if (is_colored(image_p) == 1)
		width *= 3;

	pixel **img = NULL;
	img = (pixel **)malloc(height * sizeof(pixel *));
	for (int i = 0; i < height; i++)
		img[i] = (pixel *)malloc(width * sizeof(pixel));
	if (!img) {
		fprintf(stderr, "%s\n", "Eroare alocare memorie\n");
		exit(0);
	}
	return img;
}

// Functie care elibereaza memoria unei matrice cu lungimea data
void free_matrix(pixel **img, int height)
{
	for (int i = 0; i < height; i++)
		free(img[i]);
	free(img);
}

// Functie care intoarce numarul de cuvinte dintr-o comanda
int no_word(char command[])
{
	int no = 0;
	char aux[COMMAND_MAX], *word;
	strcpy(aux, command);
	word = strtok(aux, " ");
	while (word) {
		no++;
		word = strtok(0, " ");
	}
	return no;
}

// Functie ce intoarce 1 in caz ca toata poza este selectata si 0 altfel
int is_all_selected(file_image *image_p)
{
	if (((image_p->x2 - image_p->x1) != image_p->width) ||
		((image_p->y2 - image_p->y1) != image_p->height))
		return 0;

	return 1;
}

// Functie care interschimba valorile a 2 varianbile int
void swap_int(int *a, int *b)
{
	int aux = *a;
	*a = *b;
	*b = aux;
}

// Functie care interschimba valorile a 2 varianbile double
void swap_double(double *a, double *b)
{
	double aux = *a;
	*a = *b;
	*b = aux;
}
