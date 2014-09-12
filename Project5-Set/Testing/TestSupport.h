#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "Set.h"

unsigned time();
static int const number_of_tests = 1000;
static int const maximum_set_size = 100;

void randomSet(Set* s);

typedef void(*setFun)(Set*, const Set*);
void checkCase(setFun fun, Set* s1, Set* s2, Set* expect);
bool checkCaseNew(setFun fun, Set* s1, Set* s2, Set* expect);


/*
* create two sets with n random elements
* the sets should have 50% of the elements in common
*/
void createRandomSetN(int n, Set* a, Set* b);
typedef void(*Function)(void);
double timeFunction(Function f);


bool checkLinear(double times [], int sizes [], int first, int last);
bool checkSubLinear(double times [], int sizes [], int first, int last);

enum Tests {
	IS_MEMBER = 0,
	INSERT = 1,
	REMOVE = 2,
	IS_EQUAL_TO = 3,
	IS_SUBSET_OF = 4,
	UNION_IN = 5,
	INTERSECT_FROM = 6,
	SUBTRACT_FROM = 7,

	NUM_TESTS /* MUST BE LAST */
};

typedef enum Scales {
	SUB_LINEAR,
	LINEAR,
	SUPER_LINEAR
} Scales;


void isMemberTimeFun(void);
void isEqualTimeFun(void);
void isSubsetTimeFun(void);
void unionTimeFun(void);
void intersectTimeFun(void);
void subtractTimeFun(void);
Scales determineScaling(Function fun);

