/*
 * Project 4/5 Secret Tests
 */
#pragma once

#define TEST05_MASK             (0x0001)
#define TEST06_MASK             (0x0002)
#define TEST07_MASK             (0x0004)
#define TEST08_MASK             (0x0008)
#define TEST09_MASK             (0x0010)
#define TEST10_MASK             (0x0020)
#define UNION_MASK              (0x0040)
#define INTERSECTION_MASK       (0x0080)
#define SUBTRACTION_MASK        (0x0100)

#define ALL_TESTS               (0x01FF)

void use_secret_tests(void);
void use_secret_tests(int mask);
