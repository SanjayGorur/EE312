// Tests for Matrix Multiply

#include "gtest/gtest.h"
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "MatrixMultiply.h"
#include "TestSupport.h"

TEST(stage1, testA) {
	double a[100];
	double b[100];
	double c[100];

	generateRandomMatrix(a, 10, 10);
	generateIdentityMatrix(b, 10, 10);
	multiplyMatrices(a, 10, 10, b, 10, c);
	assertEqualMatrices(a, c, 10, 10);
}

TEST(stage1, testB) {
	double a[100];
	double b[100];
	double c[100];

	generateRandomMatrix(a, 10, 10);
	generateIdentityMatrix(b, 10, 10);
	multiplyMatrices(a, 10, 10, b, 10, c);
	multiplyMatrices(b, 10, 10, a, 10, c);
	assertEqualMatrices(a, c, 10, 10);
}

TEST(stage1, testC) {
	double a[100];
	double b[100];
	double c[100];

	/* these arrays are used only for test C */
	uint32_t perturb_rows[5] = {2, 4, 6, 1, 7};
	double perturb_vals[5] = {1.0, -1.0, 2.0, -2.5, 3.0};
	double expected[] = {
		1, -10, 0, 0, 18, 0, -12, 3, 0, 0,
		0, -6.5, 0, 0, 10.5, 0, -7, 3, 0, 0,
		0, -10, 0, 0, 18, 0, -12, 3, 0, 0,
		0, -10, 0, 1, 18, 0, -12, 3, 0, 0,
		0, -10, 1, 0, 20, 0, -12, 3, 0, 0,
		0, -10, 0, 0, 18, 1, -12, 3, 0, 0,
		0, -10, 0, 0, 20, 0, -13, 3, 0, 0,
		0, -2.5, 0, 0, 4.5, 0, -3, 1, 0, 0,
		0, -10, 0, 0, 18, 0, -12, 3, 1, 0,
		0, -10, 0, 0, 18, 0, -12, 3, 0, 1};


	generateRandomMatrix(a, 10, 10);
	generateIdentityMatrix(b, 10, 10);
	multiplyMatrices(a, 10, 10, b, 10, c);
	multiplyMatrices(b, 10, 10, a, 10, c);
	generateIdentityMatrix(a, 10, 10);
	perturbMatrix(a, 10, 10, perturb_rows, perturb_vals, 5);

	copyMatrix(a, b, 10, 10);
	perturbMatrix(b, 10, 10, perturb_rows, perturb_vals, 5);
	multiplyMatrices(a, 10, 10, b, 10, c);

	assertEqualMatrices(c, expected, 10, 10);
}


TEST(stage2, testA) {
	double a[100];
	double b[100];
	double c[100];
	PtrDouble matrix_list[5];
	uint32_t rows[5];
	uint32_t cols[5];
	int k;

	generateRandomMatrix(a, 10, 10);
	generateIdentityMatrix(b, 10, 10);

	matrix_list[0] = a;
	rows[0] = 10;
	cols[0] = 10;
	k = 1;
	while (k < 5) {
		matrix_list[k] = b;
		rows[k] = 10;
		cols[k] = 10;
		k += 1;
	}
	multiplyMatrixChain(matrix_list, rows, cols, 5, c, 10, 10);
	assertEqualMatrices(a, c, 10, 10);
}

TEST(stage2, testB) {
	double a[100];
	double b[100];
	double c[100];
	PtrDouble matrix_list[5];
	uint32_t rows[5];
	uint32_t cols[5];
	double a_squared[100];
	int k;

	generateRandomMatrix(a, 10, 10);
	generateIdentityMatrix(b, 10, 10);

	matrix_list[0] = a;
	rows[0] = 10;
	cols[0] = 10;
	k = 1;
	while (k < 5) {
		matrix_list[k] = b;
		rows[k] = 10;
		cols[k] = 10;
		k += 1;
	}
	multiplyMatrices(a, 10, 10, a, 10, a_squared);
	matrix_list[2] = a;
	multiplyMatrixChain(matrix_list, rows, cols, 5, c, 10, 10);
	assertEqualMatrices(a_squared, c, 10, 10);
}

TEST(stage2, testC) {
	double a[100];
	double b[100];
	double c[100];
	PtrDouble matrix_list[5];
	uint32_t rows[5];
	uint32_t cols[5];
	double a_squared[100];
	double a_cubed[100];
	int k;

	generateRandomMatrix(a, 10, 10);
	generateIdentityMatrix(b, 10, 10);

	matrix_list[0] = a;
	rows[0] = 10;
	cols[0] = 10;
	k = 1;
	while (k < 5) {
		matrix_list[k] = b;
		rows[k] = 10;
		cols[k] = 10;
		k += 1;
	}

	multiplyMatrices(a, 10, 10, a, 10, a_squared);
	multiplyMatrices(a_squared, 10, 10, a, 10, a_cubed);

	matrix_list[2] = a;
	matrix_list[4] = a;

	multiplyMatrixChain(matrix_list, rows, cols, 5, c, 10, 10);
	assertEqualMatrices(a_cubed, c, 10, 10);
}

TEST(stage3, testA) {
	double a[100];
	double b[50];
	double c[50];
	PtrDouble matrix_list[5];
	uint32_t rows[5];
	uint32_t cols[5];
	double d[100];
	double expected[100];
	uint32_t i, j, k; // uninteresting loop variables

	generateIdentityMatrix(a, 10, 10);
	generateIdentityMatrix(b, 10, 5);
	generateIdentityMatrix(c, 5, 10);

	/* a is 10x10,
	* ab is 10x5
	* abc is 10x10
	* abcb is 10x5
	* abcbc is 10x10
	*/

	matrix_list[0] = a;
	rows[0] = 10;
	cols[0] = 10;
	k = 1;
	while (k < 5) {
		matrix_list[k] = b;
		rows[k] = 10;
		cols[k] = 5;
		k += 2;
	}

	k = 2;
	while (k < 5) {
		matrix_list[k] = c;
		rows[k] = 5;
		cols[k] = 10;
		k += 2;
	}
	multiplyMatrixChain(matrix_list, rows, cols, 5, d, 10, 10);

	copyMatrix(expected, a, 10, 10);
	i = 5;
	while (i < 10) {
		j = 5;
		while (j < 10) {
			expected[(i * 10) + j] = 0.0;
			j += 1;
		}
		i += 1;
	}

	assertEqualMatrices(expected, d, 10, 10);
}

TEST(stage4, test_hard_coded_matrix) {
	double a[8] = {7, 3,
		           2, 5,
				   6, 8,
				   9, 0,};

	double b[6] = {7, 4, 9,
		           8, 1, 5};

	double c[12];

	double expected[12] = {73,  31, 78,
						   54,  13, 43,
						   106, 32, 94,
	                       63,  36, 81};

	multiplyMatrices(a, 4, 2, b, 3, c);
	assertEqualMatrices(expected, c, 4, 3);
}

TEST(stage4, test_single_number_matrix) {
	double a[1] = { 5 };
	double b[1] = { 4 };
	double c[1];

	double expected[1] = { 20 };

	multiplyMatrices(a, 1, 1, b, 1, c);
	assertEqualMatrices(expected, c, 1, 1);
}

TEST(stage4, test_empty_matrix) {
	double a[5] = {};
	double b[5] = {};
	double c[5];

	double expected[5] = {};

	multiplyMatrices(a, 0, 0, b, 0, c);
	assertEqualMatrices(expected, c, 0, 0);
}

TEST(stage4, test_zero_matrix) {
	double a[5] = { 5, 2, 7, 1, 1 };
	double b[5] = { 0,
					0,
					0,
					0,
					0 };
	double c[1];

	double expected[5] = { 0 };

	multiplyMatrices(a, 1, 5, b, 1, c);
	assertEqualMatrices(expected, c, 1, 1);
}

TEST(stage4, test_alloc_memory_no_overflow) {
	double a[10000];
	double b[100];
	double c[50000];

	generateRandomMatrix(a, 1000, 10);
	generateRandomMatrix(b, 10, 10);
	generateRandomMatrix(c, 10, 50);

	double d[50000];

	PtrDouble matrix_list[3];
	uint32_t rows[3];
	uint32_t cols[3];

	matrix_list[0] = a;
	rows[0] = 1000;
	cols[0] = 10;
	matrix_list[1] = b;
	rows[1] = 10;
	cols[1] = 10;
	matrix_list[2] = c;
	rows[2] = 50;
	cols[2] = 10;

	multiplyMatrixChain(matrix_list, rows, cols, 3, d, 10, 10);
}

TEST(stage4, test_negative_numbers) {
	double a[9] = { -3, 4, 2,
					-5, 7, -2,
					43, 7, 1 };
	double b[6] = { 15, -3,
					2,  1,
					5,  7 };
	double c[6];

	double expected[6] = { -27, 27,
						   -71, 8,
						   664, -115 };

	multiplyMatrices(a, 3, 3, b, 2, c);
	assertEqualMatrices(expected, c, 3, 2);
}

// extern void use_secret_tests();
void main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	// use_secret_tests();
	RUN_ALL_TESTS();
}