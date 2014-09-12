/* Project2.cpp
 *
 * Name: Phillip Lemons
 * EID: pl6664
 * EE312 -- 16495
 * Duncan Horn
 * Project 2, Matrix Multiplication
 */

#include <stdio.h>
#include <stdint.h>
#include "MatrixMultiply.h"

void multiplyMatrices(
        double a[],
        const uint32_t a_rows,
        const uint32_t a_cols,
        double b[],
        const uint32_t b_cols,
        double c[]) {
	for (uint32_t i = 0; i < a_rows; i++) {
		for (uint32_t j = 0; j < b_cols; j++) {
			double sum = 0;
			for (uint32_t k = 0; k < a_cols; k++) {
				sum += a[i * a_cols + k] * b[k * b_cols + j];
			}
			c[i * b_cols + j] = sum;
		}
	}
}

#define READY_FOR_STAGE2
#ifdef READY_FOR_STAGE2

/* these three variables are used in Stage 2 to implement allocateSpace */
#define TOTAL_SPACE 10000
double memory_pool[TOTAL_SPACE];
uint32_t top_of_pool = 0;

/* you need to write this function. clearly it doesn't currently do anything */
PtrDouble allocateSpace(uint32_t size) {
	if (top_of_pool + size > TOTAL_SPACE) {
		fprintf(stderr, "memory_pool depleted... could not allocate memory\n");
		return NULL;
	}
	PtrDouble new_ptr = &memory_pool[0] + top_of_pool;
	top_of_pool += size;
	return new_ptr;
}

void deallocateSpace(int32_t size) {
	if (size == TOTAL_SPACE) {
		top_of_pool = 0;
		return;
	}

	if (top_of_pool - size < 0) {
		fprintf(stderr, "Too much space deallocated... setting top_of_pool to 0\n");
		top_of_pool = 0;
		return;
	}

	top_of_pool -= size;
}

void multiplyMatrixChain(
    PtrDouble matrix_list[],
    uint32_t rows[],
    uint32_t cols[],
    uint32_t num_matrices,
    double out[],
    uint32_t out_rows,
    uint32_t out_cols) {

	/* The loop works by continually multiplying the a_mat
	* and b_mat matrix to obtain the c_mat matrix.
	* After each iteration, the previous "c_mat" becomes the
	* next "a_mat" matrix.  the b_mat matrix will be the
	* next matrix in the matrix_list array.
	*
	* For example, on the first time through a_mat is matrix_list[0]
	* b_mat is matrix_list[1]
	* c_mat (the output) will be a temporary matrix that you must allocate
	* space for.
	* After multiplying a_mat and b_mat together, you need to
	* take the result and multiply it by matrix_list[2],
	* so a_mat will be set to c_mat, and b_mat will be matrix_list[2]
	* you must again allocate a new temporary array to hold the result.
	*
	* There's no guarantee that the temporary arrays are the same size,
	* so there's no practical way to reuse the temporary arrays.
	* Therefore, allocate a new array each iteration.  We'll clean up when we're done.
	*/

	double* a_mat = matrix_list[0];
	uint32_t a_rows = rows[0]; // number of rows in a matrix
	uint32_t a_cols = cols[0]; // number of cols in a matrix
	uint32_t next_src = 1; // index of next matrix to multiply by
	double* b_mat;
	double* c_mat;
	uint32_t b_cols;
	while (next_src < num_matrices) {
		/* Each iteration of the loop perform the following
		 * set c_mat = a_mat * b_mat where
		 * a_mat is the result from the previous iteration of the loop (or matrix_list[0] on the first iteration)
		 * b_mat is the next matrix in the matrix list
		 * c_mat is allocated space (using our handy-dandy memory allocator function) */
		b_mat = matrix_list[next_src];
		b_cols = cols[next_src];

		if (next_src < num_matrices - 1) {
			c_mat = allocateSpace(a_rows * b_cols);
		}
		else {
			c_mat = out;
		}

		multiplyMatrices(a_mat, a_rows, a_cols, b_mat, b_cols, c_mat);

		a_mat = c_mat;
		a_cols = b_cols;

		next_src += 1;
	}

	/* deallocate all memory (this only takes one line, 'cause we have such a simple memory allocator) */
	deallocateSpace(TOTAL_SPACE);
}

#endif /* READY_FOR_STAGE_2 */