/*
 * Project4.cpp
 *
 * Name: Phillip Lemons
 * EID: pl6664
 * EE312 -- 16495
 * TA: Duncan Horn
 * Section Time: Wed. 11 - 12
 * Project 4, Set ADT
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <cstring>
#include "Set.h"

#define DEBUG 0

/* Returns the index of the element or -1 if the element is not present
 * O(log(N))
 */
int binarySearch(const Set* set, int num) {
	int min = 0;
	int max = set->len - 1;

	while (max >= min) {
		int mid = min + (max - min) / 2;
		if (set->elements[mid] == num) {
			return mid;
		}
		else if (set->elements[mid] < num) {
			min = mid + 1;
		}
		else {
			max = mid - 1;
		}
	}
	return -1;
}

/* Returns the index of the smallest element that is bigger than num 
 * O(log(N))
 */
int alternateBinarySearch(const Set* set, int num) {
	int min = 0;
	int max = set->len - 1;

	while (max >= min) {
		int mid = min + (max - min) / 2;
		if (set->elements[mid] < num) {
			min = mid + 1;
		}
		else {
			max = mid - 1;
		}
	}
	return min;
}

/* done for you already 
 * O(1)
 */
void destroySet(Set* self) {
	free(self->elements);
}

/* done for you already 
 * O(1)
 */
void createEmptySet(Set* self) {
	self->len = 0;
	self->capacity = 0;
	self->elements = nullptr;
}

/* done for you already 
 * O(1)
 */
void createSingletonSet(Set* self, int x) {
	self->elements = (int*) malloc(sizeof(int));
	self->elements[0] = x;
	self->len = 1;
	self->capacity = 1;
}

/* done for you already 
 * O(N) where N = number of elements in other
 */
void createCopySet(Set* self, const Set* other) {
	self->elements = (int*) malloc(other->capacity * sizeof(int));
	for (int k = 0; k < other->len; k += 1) {
		self->elements[k] = other->elements[k];
	}
	self->len = other->len;
	self->capacity = other->capacity;
}

/* done for you already
 * O(N) where N = number of elements in other
 */
void assignSet(Set* self, const Set* other) {
	if (self == other) { return; }
	
	destroySet(self);
	createCopySet(self, other);
}

/* return true if x is an element of self
 * O(log(N))
 */
bool isMemberSet(const Set* self, int x) {
	if (binarySearch(self, x) != -1) {
		return true;
	}
	return false;
}

/*
 * add x as a new member to this set. 
 * If x is already a member, then self should not be changed
 * Be sure to restore the design invariant property that elemnts[] remains sorted
 * (yes, you can assume it is sorted when the function is called, that's what an invariant is all about)
 */
// O(N)
void insertSet(Set* self, int x) {
	if (isMemberSet(self, x)) return;  // O(log(N))
	if (self->capacity == self->len) { // O(1)
		if (self->capacity == 0) self->capacity = 1;
		else self->capacity *= 2;

		int* tmp = (int*)realloc(self->elements, self->capacity * sizeof(int));
		if (tmp == nullptr) {
			fprintf(stderr, "Heap out of memory!!!!\n");
			return;
		}
		self->elements = tmp;
	}

	if (self->len == 0) { // O(1)
		// assert capacity == 1
		self->elements[0] = x;
		self->len += 1;
	}
	else { // O(log(N)) + O(N) because memmove is O(N) in worst case
		int i = alternateBinarySearch(self, x);

		int* dest = (&self->elements[i]) + 1;
		int* source = (&self->elements[i]);
		int size = (self->len - i) * sizeof(int);
		memmove((&self->elements[i]) + 1, &self->elements[i], size);
		self->elements[i] = x;
		self->len += 1;
	}
}

// convenience function used for testing purposes
// adds all the items in arr to the set
void insertManySet(Set* self, int arr[], int n) {
	for (int i = 0; i < n; i++) {
		insertSet(self, arr[i]);
	}
}

// NOTE: this does not check for x already being in set. USE WITH CAUTION
// O(1)
void appendSet(Set* self, int x) {
	if (self->capacity == self->len) {
		if (self->capacity == 0) self->capacity = 1;
		else self->capacity *= 2;

		int* tmp = (int*)realloc(self->elements, self->capacity * sizeof(int));
		if (tmp == nullptr) {
			fprintf(stderr, "Heap out of memory!!!!\n");
			return;
		}
		self->elements = tmp;
	}

	self->elements[self->len] = x;
	self->len += 1;
}


/*
 * don't forget: it is OK to try to remove an element
 * that is NOT in the set.  
 * If 'x' is not in the set 'self', then
 * removeSet should do nothing (it's not an error)
 * Otherwise, ('x' IS in the set), remove x. Be sure to update self->length
 * It is not necessary (nor recommended) to call malloc -- if removing an element means the 
 * array on the heap is "too big", that's almost certainly OK, and reallocating a smaller array 
 * is almost definitely NOT worth the trouble
 */
// worst case is O(N)
void remove(Set* self, int index) {
	int* dest = &self->elements[index];
	int* source = (&self->elements[index]) + 1;
	int size = (self->len - index - 1) * sizeof(int);
	memmove(dest, source, size);
	self->len -= 1;
}

// worst case is O(N) + O(log(N))
void removeSet(Set* self, int x) {
	// len = self->len
	int index;
	if ((index = binarySearch(self, x)) == -1) return;
	remove(self, index);
	// assert self->len == len - 1
}

/* done for you already */
void displaySet(const Set* self) {
	int k;
	
	printf("{");

	if (self->len == 0) { 
		printf("}"); 
	}
	else {
		for (k = 0; k < self->len; k += 1) {
			if (k < self->len - 1) {
				printf("%d,", self->elements[k]);
			} else {
				printf("%d}", self->elements[k]);
			}
		}
	}
}

/* return true if self and other have exactly the same elements */
// O(N)
bool isEqualToSet(const Set* self, const Set* other) {
	if (self->len != other->len) return false;
	for (int i = 0; i < self->len; i++) {
		if (self->elements[i] != other->elements[i]) return false;
	}
	return true;
}

/* return true if every element of self is also an element of other */
// O(M + N) where M = # elements in other N = # elements in self
bool isSubsetOf(const Set* self, const Set* other) {
	if (self->len > other->len) return false;

	int i = 0;
	int j = 0;
	for (; i < self->len; i++) {
		while (true) {
			if (self->elements[i] == other->elements[j]) break;
			if (other->elements[j] > self->elements[i]) return false;
			j++;
			if (j == other->len) return false;
		}
	}
	return true;
}

/* done for you */
bool isEmptySet(const Set* self) {
	return self->len == 0;
}

/* remove all elements from self that are not also elements of other */
// O(M + N) where M = # elements in other N = # elements in self
void intersectFromSet(Set* self, const Set* other) {
	if (isEmptySet(other)) {
		destroySet(self);
		createEmptySet(self);
		return;
	}
	Set new_set; createEmptySet(&new_set); // O(1)

	int i = 0;
	int j = 0;
	for (; i < self->len; i++) {
		while (true) {
			if (self->elements[i] == other->elements[j]) {
				appendSet(&new_set, self->elements[i]);
				break;
			}
			if (other->elements[j] > self->elements[i]) break;
			j++;
			if (j == other->len) break;
		}
		if (j >= other->len) break;
	}

	destroySet(self);
	self->capacity = new_set.capacity;
	self->elements = new_set.elements;
	self->len = new_set.len;
}

/* remove all elements from self that are also elements of other */
// O(M + N) where M = # elements in other N = # elements in self
void subtractFromSet(Set* self, const Set* other) {
	if (isEmptySet(other)) {
		return;
	}
	Set new_set; createEmptySet(&new_set); // O(1)

	int i = 0;
	int j = 0;
	for (; i < self->len; i++) {
		while (true) {
			if (self->elements[i] == other->elements[j]) {
				break;
			}
			if (other->elements[j] > self->elements[i]) {
				appendSet(&new_set, self->elements[i]);
				break;
			}
			j++;
			if (j == other->len) break;
		}
		if (j >= other->len) break;
	}

	while (i < self->len) {
		appendSet(&new_set, self->elements[i]);
		i++;
	}

	destroySet(self);
	self->capacity = new_set.capacity;
	self->elements = new_set.elements;
	self->len = new_set.len;
}

/* add all elements of other to self (obviously, without creating duplicate elements) */
// O(M + N) where M = # elements in other N = # elements in self
void unionInSet(Set* self, const Set* other) {
	if (isEmptySet(other)) {
		return;
	}
	else if (isEmptySet(self)) {
		assignSet(self, other);
		return;
	}
	Set new_set; createEmptySet(&new_set); // O(1)
	if (DEBUG) {
		printf("Self: ");
		displaySet(self);
		printf("\n");

		printf("Other: ");
		displaySet(other);
		printf("\n");
	}

	int i = 0;
	int j = 0;
	for (; i < self->len; i++) {
		while (true) {
			if (other->elements[j] < self->elements[i]) {
				appendSet(&new_set, other->elements[j]);
			}
			if (self->elements[i] == other->elements[j]) {
				appendSet(&new_set, self->elements[i]);
			}
			if (other->elements[j] > self->elements[i]) {
				if (new_set.len == 0) appendSet(&new_set, self->elements[i]);
				if (new_set.elements[new_set.len - 1] != self->elements[i]) appendSet(&new_set, self->elements[i]);
				break;
			}
			j++;
			if (j == other->len) {
				break;
			}
		}
		if (j >= other->len) break;
	}

	while (j < other->len) {
		appendSet(&new_set, other->elements[j]);
		j++;
	}

	if (new_set.elements[new_set.len - 1] == self->elements[i]) {
		i++;
	}

	while (i < self->len) {
		appendSet(&new_set, self->elements[i]);
		i++;
	}

	if (DEBUG) {
		printf("New set: ");
		displaySet(&new_set);
		printf("\n");
	}

	destroySet(self);
	self->capacity = new_set.capacity;
	self->elements = new_set.elements;
	self->len = new_set.len;
}