//Copyright Bucur Vladimir 311CA (vladimirbucur1@gmai.com)

#include "main_functions.h"
#include "auxiliary_functions.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#define COMMAND_MAX 100

// Functie care realizeaza transpusa unei matrice sau in cazul
//in care este selectata doar o parte din poza, o roteste la 90
//de grade si apoi o copiaza inapoi in poza
pixel **matrix_transpose(pixel **img, file_image *image_p)
{
	// Salvez extremele zonei selectate intr-un vector
	int *c = (int *)malloc(4 * sizeof(int));
	if (is_all_selected(image_p)) {
		c[0] = 0;
		c[1] = 0;
		c[2] = image_p->width;
		c[3] = image_p->height;
	} else {
		c[0] = image_p->x1;
		c[1] = image_p->y1;
		c[2] = image_p->x2;
		c[3] = image_p->y2;
	}
	pixel **aux = NULL;
	int *arr_r, *arr_b, *arr_g;
	if (is_all_selected(image_p)) {
		aux = alloc_matrix(image_p, image_p->width, image_p->height);
		if (!aux) {
			fprintf(stderr, "%s\n", "Eroare alocare memorie\n");
			exit(0);
		}
	} else {
		arr_r = (int *)calloc((c[2] - c[0]) * (c[3] - c[1]), sizeof(int));
		arr_g = (int *)calloc((c[2] - c[0]) * (c[3] - c[1]), sizeof(int));
		arr_b = (int *)calloc((c[2] - c[0]) * (c[3] - c[1]), sizeof(int));
	}

	int index = 0;
	if (is_all_selected(image_p)) {
		for (int i = c[1]; i < c[3]; i++)
			for (int j = c[0]; j < c[2]; j++) {
				aux[j][i].r = img[i][j].r;
				aux[j][i].g = img[i][j].g;
				aux[j][i].b = img[i][j].b;
			}
	} else {
		for (int i = c[2] - 1; i >= c[0]; i--)
			for (int j = c[1]; j < c[3]; j++) {
				arr_r[index] = img[j][i].r;
				arr_g[index] = img[j][i].g;
				arr_b[index++] = img[j][i].b;
			}
	}

	if (is_all_selected(image_p)) {
		free_matrix(img, image_p->height);
		img = NULL;
		img = alloc_matrix(image_p, image_p->width, image_p->height);
		if (!img) {
			fprintf(stderr, "%s\n", "Eroare alocare memorie\n");
			exit(0);
		}

		for (int i = 0; i < image_p->width; i++)
			for (int j = 0; j < image_p->height; j++) {
				img[i][j].r = aux[i][j].r;
				img[i][j].g = aux[i][j].g;
				img[i][j].b = aux[i][j].b;
			}

		free_matrix(aux, image_p->width);
		free(c);

		return img;
	}

	index = 0;
	for (int i = c[1]; i < c[3]; i++)
		for (int j = c[0]; j < c[2]; j++) {
			img[i][j].r = arr_r[index];
			img[i][j].g = arr_g[index];
			img[i][j].b = arr_b[index++];
		}
	free(arr_r);
	free(arr_g);
	free(arr_b);
	free(c);

	return img;
}

// Functie care inverseaza ordinea coloanelor
pixel **reverse_columns(pixel **img, file_image *image_p)
{
	int c1 = image_p->x1, c2 = image_p->x2 - 1;

	for (int j = 0; j < (image_p->x2 - image_p->x1) / 2; j++) {
		for (int i = image_p->y1; i < image_p->y2; i++) {
			swap_double(&img[i][c1].r, &img[i][c2].r);
			swap_double(&img[i][c1].g, &img[i][c2].g);
			swap_double(&img[i][c1].b, &img[i][c2].b);
		}
		c1++;
		c2--;
	}
	return img;
}

// Functie care inverseaza ordinea liniilor
pixel **reverse_rows(pixel **img, file_image *image_p)
{
	int r1 = image_p->y1, r2 = image_p->y2 - 1;

	for (int i = 0; i < (image_p->y2 - image_p->y1) / 2; i++) {
		for (int j = image_p->x1; j < image_p->x2; j++) {
			swap_double(&img[r1][j].r, &img[r2][j].r);
			swap_double(&img[r1][j].g, &img[r2][j].g);
			swap_double(&img[r1][j].b, &img[r2][j].b);
		}
		r1++;
		r2--;
	}
	return img;
}

// Functie ce returneaza (ungiul precizat in comanda)
//la cere vrem sa rotim imaginea
int angle(char command[])
{
	int no_p = 0, val;
	char aux[COMMAND_MAX], *word;
	strcpy(aux, command);
	word = strtok(aux, " ");
	while (word) {
		no_p++;
		for (unsigned int i = 0; i < strlen(word); i++) {
			// daca un parametru nu este numar, comanda este invalida
			if ((no_p == 2 && word[0] == '-' && (i > 0 && isdigit(word[i]) ==
				0)) || (no_p == 2 && (word[0] != '-' && isdigit(word[i]) == 0)))
				return -1;
		}
		if (no_p == 2)
			val = atoi(word);
		word = strtok(0, " ");
	}
	if (no_p != 2)
		return -1;

	return val;
}

// Functie ce roteste zona selectata la 90 de grade
pixel **rotate_90(pixel **img, file_image *image_p)
{
	if (is_all_selected(image_p)) {
		img = reverse_rows(img, image_p);
		img = matrix_transpose(img, image_p);
		swap_int(&image_p->height, &image_p->width);
		select_all(image_p);
	} else {
		img = matrix_transpose(img, image_p);
		img = matrix_transpose(img, image_p);
		img = matrix_transpose(img, image_p);
	}
	return img;
}

// Functie ce roteste zona selectata la 270 de grade
pixel **rotate_270(pixel **img, file_image *image_p)
{
	if (is_all_selected(image_p)) {
		img = reverse_columns(img, image_p);
		img = matrix_transpose(img, image_p);
		swap_int(&image_p->height, &image_p->width);
		select_all(image_p);
	} else {
		img = matrix_transpose(img, image_p);
	}
	return img;
}

// Functie ce roteste zona selectata la 180 de grade
void rotate_180(pixel **img, file_image *image_p)
{
	img = reverse_columns(img, image_p);
	img = reverse_rows(img, image_p);
}

// Functie ce realizeaza comanda rotate
pixel **rotate(pixel **img, file_image *image_p, char command[])
{
	if (!is_all_selected(image_p) && (image_p->y2 - image_p->y1
		!= image_p->x2 - image_p->x1)) {
		printf("The selection must be square\n");
	// Rotirea la +90 sau -270 de grade este similara
	} else if (angle(command) == 90 || angle(command) == -270) {
		img = rotate_90(img, image_p);
		printf("Rotated %d\n", angle(command));
	//Rotirea la +/- 180 de grade este similara
	} else if (angle(command) == 180 || angle(command) == -180) {
		rotate_180(img, image_p);
		printf("Rotated %d\n", angle(command));
	// Rotirea la 270 sau -90 de grade este similara
	} else if (angle(command) == 270 || angle(command) == -90) {
		img = rotate_270(img, image_p);
		printf("Rotated %d\n", angle(command));
	// La rotirea la 0/360 de grade imaginea ramane pe loc
	} else if (angle(command) == 360 || angle(command) == -360 ||
			   angle(command) == 0) {
		printf("Rotated %d\n", angle(command));
	} else {
		printf("Unsupported rotation angle\n");
	}
	return img;
}
