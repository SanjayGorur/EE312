#include <stdint.h>
#include <stdio.h>
#include "MatrixMultiply.h"


void generateIdentityMatrix(double a [], const uint32_t rows, const uint32_t cols);

void perturbMatrix(double a [], const uint32_t rows, const uint32_t cols,
				   uint32_t src_row [], double perturb_values [],
				   const uint32_t num_perturbations);

/*
* generate a matrix with random elements.
* the matrix should be stored in array a in row-major order
* the parameters 'rows' and 'cols' specify the size of the matrix
*/
void generateRandomMatrix(double a [], const uint32_t rows, const uint32_t cols);

int assertEqualMatrices(double a [], double b [], const uint32_t rows, const uint32_t cols);

void copyMatrix(double dest [], double src [], const uint32_t rows, const uint32_t cols);

void printMatrix(double a [], const uint32_t rows, const uint32_t cols);