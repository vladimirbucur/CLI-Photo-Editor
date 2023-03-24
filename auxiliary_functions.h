//Copyright Bucur Vladimir 311CA (vladimirbucur1@gmai.com)

#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

void filename(char command[], char name[]);

int MW_type(file_image *image_p);

int is_colored(file_image *image_p);

pixel **alloc_matrix(file_image *image_p, int height, int width);

void free_matrix(pixel **img, int height);

int no_word(char command[]);

int is_all_selected(file_image *image_p);

void swap_int(int *a, int *b);

void swap_double(double *a, double *b);
