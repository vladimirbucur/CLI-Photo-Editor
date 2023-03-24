//Copyright Bucur Vladimir 311CA (vladimirbucur1@gmai.com)

#include "main_functions.h"
#include "auxiliary_functions.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

// Funtie ce returneaza un numar pt fiecare filtru in functie
//de ce este precizat in comanda
int which_filtre(char command[])
{
	if (strcmp(strrchr(command, ' ') + 1, "EDGE") == 0)
		return 1;

	if (strcmp(strrchr(command, ' ') + 1, "SHARPEN") == 0)
		return 2;

	if (strcmp(strrchr(command, ' ') + 1, "BLUR") == 0)
		return 3;

	if (strcmp(strrchr(command, ' ') + 1, "GAUSSIAN_BLUR") == 0)
		return 4;

	return -1;
}

// Functie care se asigura ca valoarea pixelilor e intre 0 si cea
//precizata in parametrii imaginii
double clamp(double x, double min, double max)
{
	if (x < min)
		x = min;
	else if (x > max)
		x = max;
	return x;
}

// Functie ce aplica filtrul edge
pixel **apply_edge(pixel **img, file_image *image_p)
{
	// Se folosesc 3 variabile pentru retinerea sumelor
	double s_r, s_g, s_b;

	// Initializarea matricei kernel specifica acestui filtru
	double e[3][3] = {{-1, -1, -1}, {-1, 8, -1}, {-1, -1, -1}};

	// Se realizeaza o copie a imaginii initiale pentru ca, ajungan
	//la un anumit pixel si folosind vecinii lui pentru inmultire, atunci cand
	//trecem la pixelul urmator sa fim siguri ca acesta are valoarea initiala
	pixel **aux = alloc_matrix(image_p, image_p->height, image_p->width);
	if (!aux) {
		fprintf(stderr, "%s\n", "Eroare alocare memorie\n");
		exit(0);
	}

	for (int i = 0 ; i < image_p->height; i++)
		for (int j = 0 ; j < image_p->width; j++) {
			aux[i][j].r = img[i][j].r;
			aux[i][j].g = img[i][j].g;
			aux[i][j].b = img[i][j].b;
		}

	for (int i = image_p->y1; i < image_p->y2; i++)
		for (int j = image_p->x1; j < image_p->x2; j++)
			if (i != 0 && i != image_p->height - 1 && j != 0 && j !=
				image_p->width - 1) {
				// Acestea se initializeaza la inceputul for-ului pentru a fi
				//siguri ca la fiecare pixele ele au valoarea 0
				s_r = 0;
				s_g = 0;
				s_b = 0;

				s_r += aux[i - 1][j - 1].r * e[0][0];
				s_r += aux[i - 1][j].r * e[0][1];
				s_r += aux[i - 1][j + 1].r * e[0][2];
				s_r += aux[i][j - 1].r * e[1][0];
				s_r += aux[i][j].r * e[1][1];
				s_r += aux[i][j + 1].r * e[1][2];
				s_r += aux[i + 1][j - 1].r * e[2][0];
				s_r += aux[i + 1][j].r * e[2][1];
				s_r += aux[i + 1][j + 1].r * e[2][2];

				s_g += aux[i - 1][j - 1].g * e[0][0];
				s_g += aux[i - 1][j].g * e[0][1];
				s_g += aux[i - 1][j + 1].g * e[0][2];
				s_g += aux[i][j - 1].g * e[1][0];
				s_g += aux[i][j].g * e[1][1];
				s_g += aux[i][j + 1].g * e[1][2];
				s_g += aux[i + 1][j - 1].g * e[2][0];
				s_g += aux[i + 1][j].g * e[2][1];
				s_g += aux[i + 1][j + 1].g * e[2][2];

				s_b += aux[i - 1][j - 1].b * e[0][0];
				s_b += aux[i - 1][j].b * e[0][1];
				s_b += aux[i - 1][j + 1].b * e[0][2];
				s_b += aux[i][j - 1].b * e[1][0];
				s_b += aux[i][j].b * e[1][1];
				s_b += aux[i][j + 1].b * e[1][2];
				s_b += aux[i + 1][j - 1].b * e[2][0];
				s_b += aux[i + 1][j].b * e[2][1];
				s_b += aux[i + 1][j + 1].b * e[2][2];

				s_r = clamp(s_r, 0, image_p->max_value);
				s_g = clamp(s_g, 0, image_p->max_value);
				s_b = clamp(s_b, 0, image_p->max_value);

				img[i][j].r = s_r;
				img[i][j].g = s_g;
				img[i][j].b = s_b;
			}

	// Se elibereaza memoria imaginii auxiliare
	free_matrix(aux, image_p->height);

	return img;
}

// Functie ce aplica filtrul sharpen
pixel **apply_sharpen(pixel **img, file_image *image_p)
{
	// Se folosesc 3 variabile pentru retinerea sumelor
	double s_r, s_g, s_b;

	// Initializarea matricei kernel specifica acestui filtru
	double s[3][3] = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};

	// Se realizeaza o copie a imaginii initiale pentru ca, ajungan
	//la un anumit pixel si folosind vecinii lui pentru inmultire, atunci cand
	//trecem la pixelul urmator sa fim siguri ca acesta are valoarea initiala
	pixel **aux = alloc_matrix(image_p, image_p->height, image_p->width);
	if (!aux) {
		fprintf(stderr, "%s\n", "Eroare alocare memorie\n");
		exit(0);
	}

	for (int i = 0; i < image_p->height; i++)
		for (int j = 0; j < image_p->width; j++) {
			aux[i][j].r = img[i][j].r;
			aux[i][j].g = img[i][j].g;
			aux[i][j].b = img[i][j].b;
		}
	for (int i = image_p->y1; i < image_p->y2; i++)
		for (int j = image_p->x1; j < image_p->x2; j++)
			if (i != 0 && i != image_p->height - 1 && j != 0 && j !=
				image_p->width - 1) {
				// Acestea se initializeaza la inceputul for-ului pentru a fi
				//siguri ca la fiecare pixele ele au valoarea 0
				s_r = 0;
				s_g = 0;
				s_b = 0;

				s_r += aux[i - 1][j - 1].r * s[0][0];
				s_r += aux[i - 1][j].r * s[0][1];
				s_r += aux[i - 1][j + 1].r * s[0][2];
				s_r += aux[i][j - 1].r * s[1][0];
				s_r += aux[i][j].r * s[1][1];
				s_r += aux[i][j + 1].r * s[1][2];
				s_r += aux[i + 1][j - 1].r * s[2][0];
				s_r += aux[i + 1][j].r * s[2][1];
				s_r += aux[i + 1][j + 1].r * s[2][2];

				s_g += aux[i - 1][j - 1].g * s[0][0];
				s_g += aux[i - 1][j].g * s[0][1];
				s_g += aux[i - 1][j + 1].g * s[0][2];
				s_g += aux[i][j - 1].g * s[1][0];
				s_g += aux[i][j].g * s[1][1];
				s_g += aux[i][j + 1].g * s[1][2];
				s_g += aux[i + 1][j - 1].g * s[2][0];
				s_g += aux[i + 1][j].g * s[2][1];
				s_g += aux[i + 1][j + 1].g * s[2][2];

				s_b += aux[i - 1][j - 1].b * s[0][0];
				s_b += aux[i - 1][j].b * s[0][1];
				s_b += aux[i - 1][j + 1].b * s[0][2];
				s_b += aux[i][j - 1].b * s[1][0];
				s_b += aux[i][j].b * s[1][1];
				s_b += aux[i][j + 1].b * s[1][2];
				s_b += aux[i + 1][j - 1].b * s[2][0];
				s_b += aux[i + 1][j].b * s[2][1];
				s_b += aux[i + 1][j + 1].b * s[2][2];

				s_r = clamp(s_r, 0, image_p->max_value);
				s_g = clamp(s_g, 0, image_p->max_value);
				s_b = clamp(s_b, 0, image_p->max_value);

				img[i][j].r = s_r;
				img[i][j].g = s_g;
				img[i][j].b = s_b;
			}
	// Se elibereaza memoria imaginii auxiliare
	free_matrix(aux, image_p->height);

	return img;
}

// Functie ce aplica filtrul box blur
pixel **apply_box_blur(pixel **img, file_image *image_p)
{
	// Se folosesc 3 variabile pentru retinerea sumelor
	double s_r, s_g, s_b;

	// Initializarea matricei kernel specifica acestui filtru
	double b_b[3][3] = {{1.0 / 9, 1.0 / 9, 1.0 / 9},
						{1.0 / 9, 1.0 / 9, 1.0 / 9},
						{1.0 / 9, 1.0 / 9, 1.0 / 9}};

	// Se realizeaza o copie a imaginii initiale pentru ca, ajungan
	//la un anumit pixel si folosind vecinii lui pentru inmultire, atunci cand
	//trecem la pixelul urmator sa fim siguri ca acesta are valoarea initiala
	pixel **aux = alloc_matrix(image_p, image_p->height, image_p->width);
	if (!aux) {
		fprintf(stderr, "%s\n", "Eroare alocare memorie\n");
		exit(0);
	}

	for (int i = 0; i < image_p->height; i++)
		for (int j = 0; j < image_p->width; j++) {
			aux[i][j].r = img[i][j].r;
			aux[i][j].g = img[i][j].g;
			aux[i][j].b = img[i][j].b;
		}
	for (int i = image_p->y1; i < image_p->y2; i++)
		for (int j = image_p->x1; j < image_p->x2; j++)
			if (i != 0 && i != image_p->height - 1 && j != 0 && j !=
				image_p->width - 1) {
				// Acestea se initializeaza la inceputul for-ului pentru a fi
				//siguri ca la fiecare pixele ele au valoarea 0
				s_r = 0;
				s_g = 0;
				s_b = 0;

				s_r += aux[i - 1][j - 1].r * b_b[0][0];
				s_r += aux[i - 1][j].r * b_b[0][1];
				s_r += aux[i - 1][j + 1].r * b_b[0][2];
				s_r += aux[i][j - 1].r * b_b[1][0];
				s_r += aux[i][j].r * b_b[1][1];
				s_r += aux[i][j + 1].r * b_b[1][2];
				s_r += aux[i + 1][j - 1].r * b_b[2][0];
				s_r += aux[i + 1][j].r * b_b[2][1];
				s_r += aux[i + 1][j + 1].r * b_b[2][2];

				s_g += aux[i - 1][j - 1].g * b_b[0][0];
				s_g += aux[i - 1][j].g * b_b[0][1];
				s_g += aux[i - 1][j + 1].g * b_b[0][2];
				s_g += aux[i][j - 1].g * b_b[1][0];
				s_g += aux[i][j].g * b_b[1][1];
				s_g += aux[i][j + 1].g * b_b[1][2];
				s_g += aux[i + 1][j - 1].g * b_b[2][0];
				s_g += aux[i + 1][j].g * b_b[2][1];
				s_g += aux[i + 1][j + 1].g * b_b[2][2];

				s_b += aux[i - 1][j - 1].b * b_b[0][0];
				s_b += aux[i - 1][j].b * b_b[0][1];
				s_b += aux[i - 1][j + 1].b * b_b[0][2];
				s_b += aux[i][j - 1].b * b_b[1][0];
				s_b += aux[i][j].b * b_b[1][1];
				s_b += aux[i][j + 1].b * b_b[1][2];
				s_b += aux[i + 1][j - 1].b * b_b[2][0];
				s_b += aux[i + 1][j].b * b_b[2][1];
				s_b += aux[i + 1][j + 1].b * b_b[2][2];

				s_r = clamp(s_r, 0, image_p->max_value);
				s_g = clamp(s_g, 0, image_p->max_value);
				s_b = clamp(s_b, 0, image_p->max_value);

				img[i][j].r = s_r;
				img[i][j].g = s_g;
				img[i][j].b = s_b;
			}

	// Se elibereaza memoria imaginii auxiliare
	free_matrix(aux, image_p->height);

	return img;
}

// Functie ce aplica filtrul gaussian blur
pixel **apply_gaussian_blur(pixel **img, file_image *image_p)
{
	// Se folosesc 3 variabile pentru retinerea sumelor
	double s_r, s_g, s_b;

	// Initializarea matricei kernel specifica acestui filtru
	double g_b[3][3] = {{1.0 / 16, 2.0 / 16, 1.0 / 16},
						{2.0 / 16, 4.0 / 16, 2.0 / 16},
						{1.0 / 16, 2.0 / 16, 1.0 / 16}};

	// Se realizeaza o copie a imaginii initiale pentru ca, ajungan
	//la un anumit pixel si folosind vecinii lui pentru inmultire, atunci cand
	//trecem la pixelul urmator sa fim siguri ca acesta are valoarea initiala
	pixel **aux = alloc_matrix(image_p, image_p->height, image_p->width);
	if (!aux) {
		fprintf(stderr, "%s\n", "Eroare alocare memorie\n");
		exit(0);
	}

	if (!aux) {
		fprintf(stderr, "%s\n", "Eroare alocare memorie\n");
		exit(0);
	}
	for (int i = 0; i < image_p->height; i++)
		for (int j = 0; j < image_p->width; j++) {
			aux[i][j].r = img[i][j].r;
			aux[i][j].g = img[i][j].g;
			aux[i][j].b = img[i][j].b;
		}
	for (int i = image_p->y1; i < image_p->y2; i++)
		for (int j = image_p->x1; j < image_p->x2; j++)
			if (i != 0 && i != image_p->height - 1 && j != 0 && j !=
				image_p->width - 1) {
				// Acestea se initializeaza la inceputul for-ului pentru a fi
				//siguri ca la fiecare pixele ele au valoarea 0
				s_r = 0;
				s_g = 0;
				s_b = 0;

				s_r += aux[i - 1][j - 1].r * g_b[0][0];
				s_r += aux[i - 1][j].r * g_b[0][1];
				s_r += aux[i - 1][j + 1].r * g_b[0][2];
				s_r += aux[i][j - 1].r * g_b[1][0];
				s_r += aux[i][j].r * g_b[1][1];
				s_r += aux[i][j + 1].r * g_b[1][2];
				s_r += aux[i + 1][j - 1].r * g_b[2][0];
				s_r += aux[i + 1][j].r * g_b[2][1];
				s_r += aux[i + 1][j + 1].r * g_b[2][2];

				s_g += aux[i - 1][j - 1].g * g_b[0][0];
				s_g += aux[i - 1][j].g * g_b[0][1];
				s_g += aux[i - 1][j + 1].g * g_b[0][2];
				s_g += aux[i][j - 1].g * g_b[1][0];
				s_g += aux[i][j].g * g_b[1][1];
				s_g += aux[i][j + 1].g * g_b[1][2];
				s_g += aux[i + 1][j - 1].g * g_b[2][0];
				s_g += aux[i + 1][j].g * g_b[2][1];
				s_g += aux[i + 1][j + 1].g * g_b[2][2];

				s_b += aux[i - 1][j - 1].b * g_b[0][0];
				s_b += aux[i - 1][j].b * g_b[0][1];
				s_b += aux[i - 1][j + 1].b * g_b[0][2];
				s_b += aux[i][j - 1].b * g_b[1][0];
				s_b += aux[i][j].b * g_b[1][1];
				s_b += aux[i][j + 1].b * g_b[1][2];
				s_b += aux[i + 1][j - 1].b * g_b[2][0];
				s_b += aux[i + 1][j].b * g_b[2][1];
				s_b += aux[i + 1][j + 1].b * g_b[2][2];

				s_r = clamp(s_r, 0, image_p->max_value);
				s_g = clamp(s_g, 0, image_p->max_value);
				s_b = clamp(s_b, 0, image_p->max_value);

				img[i][j].r = s_r;
				img[i][j].g = s_g;
				img[i][j].b = s_b;
			}

	// Se elibereaza memoria imaginii auxiliare
	free_matrix(aux, image_p->height);
	return img;
}

// Functie ce aplica filtrele
void filtres(pixel **img, file_image *image_p, char command[])
{
	// Daca nr de cuvinte din comanda este diferit de 2, comanda nu este buna
	if (no_word(command) != 2) {
		printf("Invalid command\n");
	} else {
		// In functie de nr returnat de functia which_filtre se aplica
		//filtrul corespunzator
		if (which_filtre(command) != -1) {
			if (is_colored(image_p) == 1) {
				if (which_filtre(command) == 1) {
					img = apply_edge(img, image_p);
					printf("APPLY EDGE done\n");
				} else if (which_filtre(command) == 2) {
					img = apply_sharpen(img, image_p);
					printf("APPLY SHARPEN done\n");
				} else if (which_filtre(command) == 3) {
					img = apply_box_blur(img, image_p);
					printf("APPLY BLUR done\n");
				} else if (which_filtre(command) == 4) {
					img = apply_gaussian_blur(img, image_p);
					printf("APPLY GAUSSIAN_BLUR done\n");
				}
			} else {
				// Daca imaginea nu este color se afiseaza un mesaj de eroare
				printf("Easy, Charlie Chaplin\n");
			}
		} else {
			// Daca parametrul precizat nu este unul din cele 4 acceptate se
			//afiseaza un mesaj de eroara
			printf("APPLY parameter invalid\n");
		}
	}
}
