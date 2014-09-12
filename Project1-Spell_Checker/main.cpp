/*
* Name: Phillip Lemons
* EID: pl6664
* EE312 -- 16495
* Duncan Horn
* Project 1
*
* SpellCheck
*/

#define _CRT_SECURE_NO_WARNINGS 1 // this #define is for VS2012, which doesn't like the standard C library

#include <stdio.h> // declaraations of standard input/output functions
#include <stdint.h> // declarations of standard integer types (e.g., int32_t)
#include <stdlib.h> // declarations of general functions in the standard library (e.g., abort, exit)


#define ONE_MB (1 << 20)
#define BUFFER_SIZE ONE_MB

#define TRUE 1
#define FALSE 0

#define LESS_THAN -1
#define GREATER_THAN 1
#define EQUAL 0

// returns the size of an array (make sure to use the array and not a pointer to the array like in a function
#define ARR_SIZE(x) (sizeof(x) / sizeof(x[0]))

char dictionary[BUFFER_SIZE]; // a large buffer to hold a dictionary (1MB)
char article[BUFFER_SIZE]; // a large buffer to hold an article (1MB)

void spellCheck(char[], char[]); // this is the function you write (put your function in Project1.cpp

/* Binary Search functions */
int32_t binarySearch(char[], char*[], int32_t);
int32_t midpoint(int32_t, int32_t);
int32_t makeDictPtrArray(char***, char[]);

/* Word functions */
char* nextWord(char**);
char* nextDictWord(char**);
int32_t compareWords(char*, char*);
int32_t isWord(char*);
void printWord(char*);

/* Util functions */
char toLowercase(char);
int32_t isLetter(char);
void goToEnd(char**);

/* Tests
 * --------------------------------------
 *   Word Tests:
 *       - isWordTest
 *       - printWordTest
 *       - nextWordTest
 *       - nextDictWordTest
 *       - nextWordTestReturn0
 *       - nextDictWordTestReturn0
 *       - nextWordTestIgnorePunc
 *       - nextDictWordTestIgnorePunc
 *       - nextWordTestRemoveSingles
 *       - compareWordsTest
 *
 *   Binary Search tests:
 *       - makeDictPtrsTest
 *       - midpointTest
 *       - (binarySearchTest)
 *
 *   Util Tests:
 *       - isLetterTest
 *       - toLowercaseTest
 *       - (goToEndTest)
 *
 *   Base Tests:
 *       - fruityTest
 *       - dogTest
 *       - punctuationTests
 *       - dictTest
 *
 *   General Tests:
 *       - generalTest1
 *       - generalTest2
 */

/*
 * Read a .txt file into one of the two buffers
 * this function does NOT confirm that the file is correctly formatted
 */
void readFile(const char file_name[], char buffer[]) {
	FILE* file = fopen(file_name, "r");
	if (file == 0) {
		printf("unable to open file \"%s\"\n", file_name);
		abort();
	}

	/* clear (set to zeros) the buffer */
	for (uint32_t k = 0; k < BUFFER_SIZE; k += 1) {
		buffer[k] = 0;
	}

	/* read the file (this is a binary read, which is OK) */
	uint32_t bytes_read = fread(buffer, 1, BUFFER_SIZE, file);
	buffer[bytes_read] = 0;
}

void isWordTest(void) {
	printf("\n\n******* Starting Word Test #1 (isWord) *******\n");
	char *words[] = { "Hello", "a", "*#(@)%d", "I*this\nstuff", "A bear", "Cool *(#*$" };
	int32_t expected[] = { TRUE, FALSE, FALSE, FALSE, FALSE, TRUE };

	for (int i = 0; i < ARR_SIZE(expected); i++) {
		if (expected[i] == isWord(words[i])) {
			printf("Test #%d Good\n", i);
		}
		else {
			printf("Test #%d Bad\n", i);
		}
	}

	printf("****DONE****\n");
}

/* Should print:
 *     Hello
 *     Short
 *     Word
 *     cool
 *     Awesome
 */
void printWordTest(void) {
	printf("\n\n******* Starting Word Test #2 (printWord) *******\n");
	char *words[] = { "Hello", "Short*stuff", "Word\n", "cool\nbeans", "Awesome sauce" };

	for (int i = 0; i < ARR_SIZE(words); i++) {
		printWord(words[i]);
	}

	printf("****DONE****\n");
}

/* Should print:
 *     Word: crevice
 *     Word: tortilla
 *     Word: ukulele
 */
void nextWordTest(void) {
	char *current;
	char *word[20];
	printf("\n\n******* Starting Word Test #3 (nextWord) *******\n");
	char article[] = "crevice tortilla ukulele";

	current = article;
	for (int i = 0; i < 3; i++) {
		word[i] = nextWord(&current);
		printf("Word: ");
		printWord(word[i]);
	}

	printf("****DONE****\n");
}

/* Should print:
*     Word: apple
*     Word: banana
*     Word: orange
*/
void nextDictWordTest(void) {
	char *current;
	char *word[20];
	printf("\n\n******* Starting Word Test #4 (nextDictWord) *******\n");
	char dictionary[] = "apple\nbanana\norange\n";

	current = dictionary;
	for (int i = 0; i < 3; i++) {
		word[i] = nextDictWord(&current);
		printf("Word: ");
		printWord(word[i]);
	}

	printf("****DONE****\n");
}

/* Should print:
 *     Word: apple
 *     Word: banana
 *     Word: orange
 *     Word: potato
 */
void nextWordTestReturn0(void) {
	char *current;
	char *word;
	printf("\n\n******* Starting Word Test #5 (nextWordReturn0) *******\n");
	char article[] = "apple banana orange potato";

	current = article;
	while ((word = nextWord(&current)) != 0) {
		printf("Word: ");
		printWord(word);
	}

	printf("****DONE****\n");
}

/* Should print:
*     Word: maple
*     Word: outrage
*     Word: oversight
*     Word: purge
*/
void nextDictWordTestReturn0(void) {
	char *current;
	char *word;
	printf("\n\n******* Starting Word Test #6 (nextDictWordReturn0) *******\n");
	char dictionary[] = "maple\noutrage\noversight\npurge\n";

	current = dictionary;
	while ((word = nextDictWord(&current)) != 0) {
		printf("Word: ");
		printWord(word);
	}

	printf("****DONE****\n");
}

/* Should print:
 *     Word: Hey
 *     Word: man
 *     Word: we
 *     Word: just
 *     Word: hope
 *     Word: that
 *     Word: you
 *     Word: can
 *     Word: show
 *     Word: us
 *     Word: how
 *     Word: to
 *     Word: groove
 */
void nextWordTestIgnorePunc(void) {
	char *current;
	char *word;
	printf("\n\n******* Starting Word Test #7 (nextWordIgnorePunc) *******\n");
	char article[] = "Hey man, we just]'].' hope!(#%&)@$ that \" you  can show us how to groove... !93214@)";

	current = article;
	while ((word = nextWord(&current)) != 0) {
		printf("Word: ");
		printWord(word);
	}
	printf("****DONE****\n");
}

/* Should print:
*     Word: stuff
*     Word: Mother
*     Word: cool
*/
void nextDictWordTestIgnorePunc(void) {
	char *current;
	char *word;
	printf("\n\n******* Starting Word Test #8 (nextDictWordIgnorePunc) *******\n");
	char dictionary[] = "st*(*#2uff\nMoth'er\n*,.>'cool\n";

	current = dictionary;
	while ((word = nextDictWord(&current)) != 0) {
		printf("Word: ");
		while (*word != '\n') {
			if (isLetter(*word)) {
				putchar(*word);
			}
			word++;
		}
		putchar('\n');
		fflush(stdout);
	}
	printf("****DONE****\n");
}

/* Should print:
 *     Word: me
 *     Word: good
 */
void nextWordTestRemoveSingles(void) {
	char *current;
	char *word;
	printf("\n\n******* Starting Word Test #9 (nextWordRemoveSingles) *******\n");
	char article[] = "A e i o U me good f j k u";

	current = article;
	while ((word = nextWord(&current)) != 0) {
		printf("Word: ");
		printWord(word);
	}
	printf("****DONE****\n");
}

void compareWordsTest(void) {
	printf("\n\n******* Starting Word Test #10 (compareWordsTest) *******\n");
	char *d_words[] = { "Hello\n", "Hell\n", "HelloMan\n", "Hello\nMan\nYou\n", "Man\nHello", "Good'bye\n",
		"Good''*bye\n", "Alimony" };
	char *a_words[] = { "Hello", "Hello", "Hello", "Hello\n", "Hello", "Goodbye", "Goodbye", "Manner" };
	int32_t expected[] = { EQUAL, LESS_THAN, GREATER_THAN, EQUAL, GREATER_THAN, EQUAL, EQUAL, LESS_THAN };

	for (int i = 0; i < ARR_SIZE(expected); i++) {
		if (expected[i] == compareWords(a_words[i], d_words[i])) {
			printf("Test #%d Good\n", i);
		}
		else {
			printf("Test #%d Bad\n", i);
		}
	}

	printf("****DONE****\n");
}

void makeDictPtrsTest(void) {
	printf("\n\n******* Starting Binary Search Test #1 (makeDictPtrs) *******\n");
	char dictionary[] = "apple\nbanana\norange\npear\n";
	char **ptr_dict;
	int32_t size = makeDictPtrArray(&ptr_dict, dictionary);
	char **curr_ptr;
	curr_ptr = ptr_dict;
	printf("Size: %d\n", size);
	for (int i = 0; i < 4; i++) {
		printWord(*curr_ptr);
		curr_ptr++;
	}
	free(ptr_dict);
	printf("****DONE****\n");
}

void midpointTest(void) {
	printf("\n\n******* Starting Binary Search Test #2 (midpoint) *******\n");
	int32_t x_arr[] = { 5, -1, 100 };
	int32_t y_arr[] = { 10, 2, 200 };
	int32_t expected[] = { 7, 0, 150 };

	for (int i = 0; i < ARR_SIZE(expected); i++) {
		if (expected[i] == midpoint(x_arr[i], y_arr[i])) {
			printf("Test #%d Good\n", i);
		}
		else {
			printf("Test #%d Bad\n", i);
		}
	}

	printf("****DONE****\n");
}

void isLetterTest(void) {
	printf("\n\n******* Starting Util Test #1 (isLetter) *******\n");
	char letters[] = { 'A', 'a', 'Z', '9', '*' };
	int32_t expected[] = { TRUE, TRUE, TRUE, FALSE, FALSE };

	for (int i = 0; i < ARR_SIZE(expected); i++) {
		if (expected[i] == isLetter(letters[i])) {
			printf("Test #%d Good\n", i);
		}
		else {
			printf("Test #%d Bad\n", i);
		}
	}

	printf("****DONE****\n");
}

void toLowercaseTest(void) {
	printf("\n\n******* Starting Util Test #2 (toLowercase) *******\n");
	char letters[] = { 'A', 'a', 'Z', '9', '*' };
	int32_t expected[] = { 'a', 'a', 'z', 0, 0 };

	for (int i = 0; i < ARR_SIZE(expected); i++) {
		if (expected[i] == toLowercase(letters[i])) {
			printf("Test #%d Good\n", i);
		}
		else {
			printf("Test #%d Bad\n", i);
		}
	}

	printf("****DONE****\n");
}

void fruityTest(void) {
	printf("\n\n******* Starting Base Test #1 (fruity) *******\n");
	char dictionary[] = "apple\nbanana\norange\npear\n";
	char article[] = "I ate an apple and a pear\n";
	spellCheck(article, dictionary); // should print: ate, an, and 
	printf("****DONE****\n");
}

void dogTest(void) {
	printf("\n\n******* Starting Base Test #2 (dogs) *******\n");
	char dictionary[] = "Beagle\nBulldog\nCollie\nPoodle\nretriever\n";
	char article[] = "bulldog dalmation beagle Retriever poodles";
	spellCheck(article, dictionary); // should print: dalmation poodles
	printf("****DONE****\n");
}

void punctuationTests(void) {
	printf("\n\n******* Starting Base Test #3 (punctuation) *******\n");
	char dictionary[] = "but\ncan\ncan't\ndo\ndon't\nthink\n"; // the dictionary can contain punctuation
	char article[] = "I think I can, but I can't. I think I do, but I don't";
	spellCheck(article, dictionary); // should print: don
	printf("****DONE****\n");
}

/* Should print:
*     nt
*     not
*     in
*     dict
*/
void dictTest(void) {
	printf("\n\n******* Starting Base Test #4 (dictTest) *******\n");
	char dictionary[] = "didn't\ndid'nt\n";
	char article[] = "didnt, nt, not in dict\n";
	spellCheck(article, dictionary); // should print nt not in dict
	printf("****DONE****\n");
}

void generalTest1(void) {
	printf("\n\n******* Starting General Test #1 *******\n");
	readFile("greek-finances.txt", article);
	readFile("american-english.txt", dictionary);
	spellCheck(article, dictionary);
	printf("****DONE****\n");
}

void generalTest2(void) {
	printf("\n\n******* Starting General Test #2 *******\n");
	readFile("bobsledding.txt", article);
	readFile("american-english.txt", dictionary);
	spellCheck(article, dictionary);
	printf("****DONE****\n");
}

int main(void) {
	/* Word Tests */
	isWordTest();
	printWordTest();
	nextWordTest();
	nextDictWordTest();
	nextWordTestReturn0();
	nextDictWordTestReturn0();
	nextWordTestIgnorePunc();
	nextDictWordTestIgnorePunc();
	nextWordTestRemoveSingles();
	compareWordsTest();

	/* Binary Search Tests */
	makeDictPtrsTest();
	midpointTest();

	/* Util Tests */
	isLetterTest();
	toLowercaseTest();

	/* Base Tests */
	fruityTest();
	dogTest();
	punctuationTests();
	dictTest();

	/* General Tests */
	generalTest1();
	generalTest2();
}	
