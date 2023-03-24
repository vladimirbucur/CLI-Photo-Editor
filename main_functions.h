//Copyright Bucur Vladimir 311CA (vladimirbucur1@gmai.com)

#pragma once

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>

#define MW_LENGTH 2

typedef struct {
	char magic_world[MW_LENGTH + 1];
	int width;
	int height;
	int max_value;
	int x1, y1, x2, y2;
} file_image;

typedef struct {
	double r, g, b;
} pixel;

void scan_image_parameters(FILE *file_in, file_image *image_p);

void read_matrix_ascii(FILE *file_in, file_image *image_p, pixel **img,
					   int height, int width);

void read_matrix_binary(FILE *file_in, file_image *image_p, pixel **img,
						int height, int width);

pixel **load(char file_name[], pixel **img, file_image *image_p);

int check_parameters(file_image *image_p, int x1, int y1, int x2, int y2);

void select_matrix(file_image *image_p, char command[]);

void select_all(file_image *image_p);

pixel **matrix_transpose(pixel **img, file_image *image_p);

pixel **reverse_columns(pixel **img, file_image *image_p);

pixel **reverse_rows(pixel **img, file_image *image_p);

int angle(char command[]);

pixel **rotate_90(pixel **img, file_image *image_p);

void rotate_180(pixel **img, file_image *image_p);

pixel **rotate_270(pixel **img, file_image *image_p);

pixel **rotate(pixel **img, file_image *image_p, char command[]);

pixel **crop(pixel **img, file_image *image_p);

int which_filtre(char command[]);

double clamp(double x, double min, double max);

pixel **apply_edge(pixel **img, file_image *image_p);

pixel **apply_sharpen(pixel **img, file_image *image_p);

pixel **apply_box_blur(pixel **img, file_image *image_p);

pixel **apply_gaussian_blur(pixel **img, file_image *image_p);

void filtres(pixel **img, file_image *image_p, char command[]);

int ascii(char command[]);

void print_matrix_ascii(FILE *file_out, file_image *image_p, pixel **img,
						int height, int width);

void print_matrix_binary(FILE *file_out, file_image *image_p, pixel **img,
						 int height, int width);

void save(char file_name[], pixel **img, file_image *image_p, char command[]);
