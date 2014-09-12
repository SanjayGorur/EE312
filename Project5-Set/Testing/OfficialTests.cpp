// Tests for Set

#include "gtest/gtest.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "TestSupport.h"
#include "SecretTests.h"



TEST(STAGE2, SpecialCaseTests) {
	Set empty;
	Set universal;
	int i;
	Set s;
	Set r;

	createEmptySet(&empty);
	createEmptySet(&universal);
	createEmptySet(&r);
	for (i = 0; i < maximum_set_size; i += 1) {
		insertSet(&universal, i);
	}
	ASSERT_TRUE(checkCaseNew(&subtractFromSet, &universal, &universal, &empty));
	ASSERT_TRUE(checkCaseNew(&unionInSet, &universal, &universal, &universal));
	ASSERT_TRUE(checkCaseNew(&intersectFromSet, &universal, &universal, &universal));
	ASSERT_TRUE(checkCaseNew(&intersectFromSet, &universal, &empty, &empty));
	ASSERT_TRUE(checkCaseNew(&intersectFromSet, &empty, &universal, &empty));
	ASSERT_TRUE(checkCaseNew(&unionInSet, &universal, &empty, &universal));
	ASSERT_TRUE(checkCaseNew(&unionInSet, &empty, &universal, &universal));
	ASSERT_TRUE(checkCaseNew(&unionInSet, &empty, &empty, &empty));
	ASSERT_TRUE(checkCaseNew(&subtractFromSet, &empty, &empty, &empty));
	ASSERT_TRUE(checkCaseNew(&intersectFromSet, &empty, &empty, &empty));

	createEmptySet(&s);
	assert(isEmptySet(&s));
	for (i = 0; i < 10; i += 1) {
		insertSet(&s, i);
	}
	assert(s.len == 10);
	for (i = 0; i < 10; i += 1) {
		assert(isMemberSet(&s, i));
	}
	for (i = 0; i < 10; i += 1) {
		removeSet(&s, i);
		removeSet(&s, i);
		assert(s.len == 9 - i);
	}
	assert(isEmptySet(&s));
	for (i = 0; i < number_of_tests; i += 1) {
		randomSet(&s);
		assert(isSubsetOf(&empty, &s));
		assert(!isSubsetOf(&s, &empty));
		assert(isSubsetOf(&s, &universal));
		assert(!isSubsetOf(&universal, &s));

		ASSERT_TRUE(checkCaseNew(&intersectFromSet, &empty, &s, &empty));
		ASSERT_TRUE(checkCaseNew(&intersectFromSet, &s, &empty, &empty));
		ASSERT_TRUE(checkCaseNew(&intersectFromSet, &universal, &s, &s));
		ASSERT_TRUE(checkCaseNew(&intersectFromSet, &s, &universal, &s));

		ASSERT_TRUE(checkCaseNew(&unionInSet, &universal, &s, &universal));
		ASSERT_TRUE(checkCaseNew(&unionInSet, &s, &universal, &universal));

		ASSERT_TRUE(checkCaseNew(&subtractFromSet, &s, &empty, &s));

		assignSet(&r, &universal);
		subtractFromSet(&r, &s); // r = u - s;
		ASSERT_TRUE(checkCaseNew(&subtractFromSet, &universal, &r, &s)); // (u - (u - s) == s)
		ASSERT_TRUE(checkCaseNew(&unionInSet, &s, &r, &universal)); // s + (u - s) == u
		ASSERT_TRUE(checkCaseNew(&unionInSet, &r, &s, &universal)); // (u - s) + s == u
	}
	printf("The special case tests have been passed\n");
	destroySet(&empty);
	destroySet(&universal);
	destroySet(&s);
	destroySet(&r);
}


TEST(STAGE2, AlgebraicTests){
	Set empty;
	Set universal;
	int i;
	Set s;
	Set t;
	Set u;
	Set v;
	Set w;

	createEmptySet(&empty);
	createEmptySet(&universal);
	for (i = 0; i < maximum_set_size; i += 1) {
		insertSet(&universal, i);
	}

	createEmptySet(&s);
	createEmptySet(&t);
	createEmptySet(&u);
	createEmptySet(&v);
	createEmptySet(&w);

	for (i = 0; i < number_of_tests; i += 1) {
		randomSet(&u);
		randomSet(&v);
		randomSet(&w);

		/* u + v == v + u */
		assignSet(&s, &u);
		unionInSet(&s, &v);
		assignSet(&t, &v);
		unionInSet(&t, &u);
		ASSERT_TRUE(isEqualToSet(&s, &t));

		/* u + (v + w) == (u + v) + w */
		assignSet(&t, &v);
		unionInSet(&t, &w);
		assignSet(&s, &u);
		unionInSet(&s, &t);
		assignSet(&t, &u);
		unionInSet(&t, &v);
		unionInSet(&t, &w);
		ASSERT_TRUE(isEqualToSet(&s, &t));

		/* u * v == v * u */
		assignSet(&s, &u);
		intersectFromSet(&s, &v);
		assignSet(&t, &v);
		intersectFromSet(&t, &u);
		ASSERT_TRUE(isEqualToSet(&s, &t));

		/* u * (v * w) == (u * v) * w */
		assignSet(&t, &v);
		intersectFromSet(&t, &w);
		assignSet(&s, &u);
		intersectFromSet(&s, &t);
		assignSet(&t, &u);
		intersectFromSet(&t, &v);
		intersectFromSet(&t, &w);
		ASSERT_TRUE(isEqualToSet(&s, &t));

		/* u - v == u - (u * v) */
		assignSet(&s, &u);
		intersectFromSet(&s, &v);
		assignSet(&t, &u);
		subtractFromSet(&t, &s);
		assignSet(&s, &u);
		subtractFromSet(&s, &v);
		ASSERT_TRUE(isEqualToSet(&s, &t));

		/* additional tests, not implemented
		assert(w * (u - v) == w * u - w * v);
		assert(u * (v + w) == (u * v) + (u * w));
		assert(universal - (u * v) == (universal - u) + (universal - v));
		assert(universal - (u + v) == (universal - u) * (universal - v));
		*/
	}
	printf("The algebraic tests have been passed\n");
	destroySet(&empty);
	destroySet(&universal);
	destroySet(&s);
	destroySet(&t);
	destroySet(&u);
	destroySet(&v);
	destroySet(&w);
}



TEST(Stage1, RelationalTests) {
	Set s;
	Set t;
	int i;

	createEmptySet(&s);
	createEmptySet(&t);
	for (i = 0; i < number_of_tests; i += 1) {
		randomSet(&s);
		assignSet(&t, &s);
		ASSERT_TRUE(isSubsetOf(&s, &t));
		ASSERT_TRUE(isSubsetOf(&t, &s));
		ASSERT_TRUE(isEqualToSet(&s, &t));
		ASSERT_TRUE(isEqualToSet(&t, &s));
		insertSet(&s, rand() % maximum_set_size + maximum_set_size);
		ASSERT_TRUE(isSubsetOf(&t, &s));
		ASSERT_TRUE(!isSubsetOf(&s, &t));
	}
	printf("The relational tests are complete\n");
	destroySet(&s);
	destroySet(&t);
}

TEST(Stage1, MembershipTests) {
	int32_t x[100];
	Set universal; createEmptySet(&universal);
	for (int32_t k = 0; k < 100; k += 1) {
		insertSet(&universal, k);
		x[k] = k;
	}

	Set s;
	createCopySet(&s, &universal);
	for (uint32_t reps = 0; reps < 50; reps += 1) {
		/* choose a random element in array x */
		uint32_t k = rand() % (100 - reps);
		int32_t val = x[k];
		/* remove that random value from x by copying the last element into position k */
		x[k] = x[100 - reps - 1];
		ASSERT_TRUE(isMemberSet(&s, val));
		removeSet(&s, val);
		ASSERT_FALSE(isMemberSet(&s, val));
	}
	printf("The membership tests are complete\n");

}

extern void use_secret_tests();
void main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	use_secret_tests(ALL_TESTS);
	RUN_ALL_TESTS();
}