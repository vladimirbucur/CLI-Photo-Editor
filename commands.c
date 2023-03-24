//Copyright Bucur Vladimir 311CA (vladimirbucur1@gmai.com)

#include "main_functions.h"
#include "auxiliary_functions.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#define MW_LENGTH 2
#define COMMAND_MAX 100

// Functie ce realizeaza citirea parametrilor imaginii sarind comentariile
void scan_image_parameters(FILE *file_in, file_image *image_p)
{
	int i = 0;
	while (i < 3) {
		char c;
		fscanf(file_in, "%c", &c);

		if (c == '#') {
			char comment[COMMAND_MAX];
			fgets(comment, COMMAND_MAX, file_in);
			i--;
		} else {
			// Citeste magic word-ul, latimea si lungimea imaginii si
			//valoarea maxima pe care o poate lua un pixel
			fseek(file_in, -1, 1);
			if (i == 0)
				fscanf(file_in, "%s\n", image_p->magic_world);

			if (i == 1)
				fscanf(file_in, "%d%d\n", &image_p->width, &image_p->height);

			if (i == 2)
				fscanf(file_in, "%d\n", &image_p->max_value);
		}
		i++;
	}
}

// Functie ce realizeaza citirea unei matrice pentru poza in format ascii
void read_matrix_ascii(FILE *file_in, file_image *image_p, pixel **img,
					   int height, int width)
{
	unsigned char aux, aux1, aux2, aux3;
	if (is_colored(image_p) == 1) {
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++) {
				fscanf(file_in, "%hhu", &aux1);
				fscanf(file_in, "%hhu", &aux2);
				fscanf(file_in, "%hhu", &aux3);

				img[i][j].r = (double)aux1;
				img[i][j].g = (double)aux2;
				img[i][j].b = (double)aux3;
			}
	} else {
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++) {
				fscanf(file_in, "%hhu", &aux);
				img[i][j].r = (double)aux;

				img[i][j].g = img[i][j].r;
				img[i][j].b = img[i][j].r;
			}
	}
}

// Functie ce realizeaza citirea unei matrice pentru poza in format binar
void read_matrix_binary(FILE *file_in, file_image *image_p, pixel **img,
						int height, int width)
{
	unsigned char aux, aux1, aux2, aux3;
	if (is_colored(image_p) == 1) {
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++) {
				fread(&aux1, sizeof(char), 1, file_in);
				fread(&aux2, sizeof(char), 1, file_in);
				fread(&aux3, sizeof(char), 1, file_in);

				img[i][j].r = (double)aux1;
				img[i][j].g = (double)aux2;
				img[i][j].b = (double)aux3;
			}
	} else {
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++) {
				fread(&aux, sizeof(char), 1, file_in);
				img[i][j].r = (double)aux;

				img[i][j].g = img[i][j].r;
				img[i][j].b = img[i][j].r;
			}
	}
}

// Functie care realizeaza comanda de load
pixel **load(char file_name[], pixel **img, file_image *image_p)
{
	//verific daca este deja o imagine incarcata, iar daca este,
	//eliberez matricea
	if (img)
		free_matrix(img, image_p->height);

	// deschid fisierul din care trebuie incarcata poza
	FILE *file_in = fopen(file_name, "rb");
	if (!file_in) {
		printf("Failed to load %s\n", file_name);
		return NULL;
	}

	printf("Loaded %s\n", file_name);
	scan_image_parameters(file_in, image_p);

	//incarc imaginea in functie de tipul ei
	if (MW_type(image_p) == 2) {
		img = alloc_matrix(image_p, image_p->height, image_p->width);
		read_matrix_ascii(file_in, image_p, img, image_p->height,
						  image_p->width);
	} else if (MW_type(image_p) == 3) {
		img = alloc_matrix(image_p, image_p->height, image_p->width);
		read_matrix_ascii(file_in, image_p, img, image_p->height,
						  image_p->width);
	} else if (MW_type(image_p) == 5) {
		img = alloc_matrix(image_p, image_p->height, image_p->width);
		read_matrix_binary(file_in, image_p, img, image_p->height,
						   image_p->width);
	} else if (MW_type(image_p) == 6) {
		img = alloc_matrix(image_p, image_p->height, image_p->width);
		read_matrix_binary(file_in, image_p, img, image_p->height,
						   image_p->width);
	}

	fclose(file_in);

	return img;
}

// Functie ce intoarce 1 daca parametrii comenzii select sunt buni si 0 altfel
int check_parameters(file_image *image_p, int x1, int y1, int x2, int y2)
{
	int a1, a2, b1, b2;
	a1 = x1;
	b1 = y1;
	a2 = x2;
	b2 = y2;

	// Inainte de a fi retinute coordonatele, se verifica daca sunt valide
	// Coordonatele latimii, respectiv lungimii ce sunt egale sau depasirea
	// dimensiunilor pozei duc la un set invalid de coordonate
	if (a1 == a2 || b1 == b2 || a1 > image_p->width || b1 > image_p->height ||
		a2 > image_p->width  || y2 > image_p->height) {
		printf("Invalid set of coordinates\n");
		return 0;
	}

	// Daca s-au validat coordonatele, se interschimba cele corespunzatoare
	// aceleasi dimensiuni (in cazul in care x1 > x2 sau y1 > y2)
	if (a1 > a2)
		swap_int(&a1, &a2);
	if (b1 > b2)
		swap_int(&b1, &b2);

	//printf("Selected %d %d %d %d\n", a1, b1, a2, b2);
	image_p->x1 = a1;
	image_p->y1 = b1;
	image_p->x2 = a2;
	image_p->y2 = b2;
	return 1;
}

void select_matrix(file_image *image_p, char command[])
{
	// desfac comanda citita pentru a obtine parametrii
	//verific daca e o comanda valida
	char *p = strtok(command, "\n ");
	int x1, x2, y1, y2, no_p = 0;

	while (p) {
		no_p++;
		// verific daca coordonatele sunt pozitive
		for (unsigned int i = 0; i < strlen(p); i++) {
			if (p[i] == '-' && isdigit(p[i + 1]) > 0) {
				printf("Invalid set of coordinates\n");
				return;
			}

			// daca un parametru nu este numar, comanda este invalida
			if (no_p > 1 && isdigit(p[i]) == 0) {
				printf("Invalid command\n");
				return;
			}
		}
		if (no_p == 2)
			x1 = atoi(p);
		else if (no_p == 4)
			x2 = atoi(p);
		else if (no_p == 3)
			y1 = atoi(p);
		else if (no_p == 5)
			y2 = atoi(p);
		p = strtok(NULL, "\n ");
	}

	// daca nu sunt fix 4 parametrii comanda este invalida
	if (no_p != 5) {
		printf("Invalid command\n");
		return;
	}

	if (x1 > x2)
		swap_int(&x1, &x2);
	if (y1 > y2)
		swap_int(&y1, &y2);

	if (check_parameters(image_p, x1, y1, x2, y2))
		printf("Selected %d %d %d %d\n", x1, y1, x2, y2);
}

// Functie ce realizeaza comanda de select all
void select_all(file_image *image_p)
{
	image_p->x1 = 0;
	image_p->y1 = 0;
	image_p->x2 = image_p->width;
	image_p->y2 = image_p->height;
}

// Functie ce realizeaza comanda de crop
pixel **crop(pixel **img, file_image *image_p)
{
	if (is_all_selected(image_p)) {
		printf("Image cropped\n");
		return img;
	}

	// Se aloca memorie pt o imagine auxiliara in care se va retine
	//noua imagine
	pixel **aux = NULL;
	aux = alloc_matrix(image_p, image_p->y2 - image_p->y1,
					   image_p->x2 - image_p->x1);
	if (!aux) {
		fprintf(stderr, "%s\n", "Eroare alocare memorie\n");
		exit(0);
	}

	int  h = 0, w = 0;
	for (int i = image_p->y1; i < image_p->y2; i++) {
		w = 0;
		for (int j = image_p->x1; j < image_p->x2; j++) {
			aux[h][w].r = img[i][j].r;
			aux[h][w].g = img[i][j].g;
			aux[h][w].b = img[i][j].b;
			w++;
		}
		h++;
	}

	// Se elibereaza memoria pentru imaginea initiala
	free_matrix(img, image_p->height);

	// Se retin noile dimensiuni ale imaginii
	image_p->height = image_p->y2 - image_p->y1;
	image_p->width = image_p->x2 - image_p->x1;

	// Se aloca memorie pentru noua imagine
	img = NULL;
	img = alloc_matrix(image_p, image_p->height, image_p->width);
	if (!img) {
		fprintf(stderr, "%s\n", "Eroare alocare memorie\n");
		exit(0);
	}

	for (int i = 0; i < image_p->height; i++)
		for (int j = 0; j < image_p->width; j++) {
			img[i][j].r = aux[i][j].r;
			img[i][j].g = aux[i][j].g;
			img[i][j].b = aux[i][j].b;
		}
	free_matrix(aux, image_p->height);

	printf("Image cropped\n");

	return img;
}

// Functie care returneaza 1 daca atunci cand sa dat comanda SAVE s-a
//precizat sa se salveze in format ascii, 0 in caz contrar si -1 in cazul
//in care comanda nu este buna
int ascii(char command[])
{
	if (no_word(command) == 3) {
		if (strcmp(strrchr(command, ' ') + 1, "ascii") == 0)
			return 1;
		return -1;
	}
	return 0;
}

// Functie care realizeaza afisarea unei matrice in format ascii
void print_matrix_ascii(FILE *file_out, file_image *image_p, pixel **img,
						int height, int width)
{
	// Se verifica daca imaginea este color sau nu si pt fiecare caz se
	//parcurge imaginea apoi se afiseaza fiecare pixel
	unsigned char aux, aux1, aux2, aux3;
	if (is_colored(image_p) == 1) {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				aux1 = (unsigned char)round(img[i][j].r);
				aux2 = (unsigned char)round(img[i][j].g);
				aux3 = (unsigned char)round(img[i][j].b);

				fprintf(file_out, "%hhu ", aux1);
				fprintf(file_out, "%hhu ", aux2);
				fprintf(file_out, "%hhu ", aux3);
			}
			fprintf(file_out, "\n");
		}
	} else {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				aux = (unsigned char)round(img[i][j].r);

				fprintf(file_out, "%hhu ", aux);
			}
			fprintf(file_out, "\n");
		}
	}
}

// Functie care realizeaza afisarea unei matrice in format binar
void print_matrix_binary(FILE *file_out, file_image *image_p, pixel **img,
						 int height, int width)
{
	// Se verifica daca imaginea este color sau nu si pt fiecare caz se
	//parcurge imaginea apoi se afiseaza fiecare pixel
	unsigned char aux, aux1, aux2, aux3;
	if (is_colored(image_p) == 1) {
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++) {
				aux1 = (unsigned char)round(img[i][j].r);
				aux2 = (unsigned char)round(img[i][j].g);
				aux3 = (unsigned char)round(img[i][j].b);

				fwrite(&aux1, sizeof(char), 1, file_out);
				fwrite(&aux2, sizeof(char), 1, file_out);
				fwrite(&aux3, sizeof(char), 1, file_out);
			}
	} else {
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++) {
				aux = (unsigned char)round(img[i][j].r);

				fwrite(&aux, sizeof(char), 1, file_out);
			}
	}
}

// Functie ce realizeaza comanda de save
void save(char file_name[], pixel **img, file_image *image_p, char command[])
{
	// Imaginea se afiseaza conform conventiei: mai intai parametrii
	//apoi matricea de pixeli
	// Aceasta afisare se face in functie de formatul dorit si in functie
	//de prezenta cuvantului ascii in comanda sau nu
	FILE *file_out;
	if (ascii(command) == -1) {
		printf("Invalid command\n");
		return;
	}

	if (ascii(command) == 1) {
		file_out = fopen(file_name, "wt");
		if (is_colored(image_p))
			fprintf(file_out, "P3\n%d %d\n%d\n", image_p->width,
					image_p->height, image_p->max_value);

		else
			fprintf(file_out, "P2\n%d %d\n%d\n", image_p->width,
					image_p->height, image_p->max_value);

		print_matrix_ascii(file_out, image_p, img, image_p->height,
						   image_p->width);

		fclose(file_out);
	} else if (ascii(command) == 0) {
		file_out = fopen(file_name, "wt");
		if (is_colored(image_p))
			fprintf(file_out, "P6\n%d %d\n%d\n", image_p->width,
					image_p->height, image_p->max_value);
		else
			fprintf(file_out, "P5\n%d %d\n%d\n", image_p->width,
					image_p->height, image_p->max_value);

		fclose(file_out);

		file_out = fopen(file_name, "ab");
		print_matrix_binary(file_out, image_p, img, image_p->height,
							image_p->width);
		fclose(file_out);
	}
	printf("Saved %s\n", file_name);
}
