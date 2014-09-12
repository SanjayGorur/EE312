/*
* Name: Phillip Lemons
* EID: pl6664
* EE312 -- 16495
* Duncan Horn
* Project 1
*
* SpellCheck
*/

#include <stdio.h> // provides declarations for printf and putchar
#include <stdint.h> // provides declarations for int32_t uint32_t and the other (new) standard C types
#include <stdlib.h>

/* All of your code must be in this file. Please no #includes other than standard system headers (ie.., stdio.h, stdint.h)
 *
 * Many students find it helpful to declare global variables (often arrays). You are welcome to use
 * globals if you find them helfpul. Global variables are by no means necessary for this project.
 */
#define TRUE 1
#define FALSE 0

#define LESS_THAN -1
#define GREATER_THAN 1
#define EQUAL 0

/* Binary Search functions */
int32_t binarySearch(char word[], char* ptrs_dict[], int32_t size);
int32_t midpoint(int32_t min, int32_t max);
int32_t makeDictPtrArray(char ***dict_ptrs, char dict[]);

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

/* You must write this function (spellCheck). Do not change the way the function is declared (i.e., it has
 * exactly two parameters, each parameter is a standard (mundane) C string (see SpellCheck.pdf).
 * You are expected to use reasonable programming style. I *insist* that you indent 
 * reasonably and consistently in your code. I strongly encourage you to avoid big functions
 * (while there are always exceptions, a good rule of thumb is about 15 lines in a function).
 * So, plan on implementing spellCheck by writing two or three other "support functions" that
 * help make the actual spell checking easier for you.
 * There are no explicit restictions on using functions from the C standard library. However,
 * for this project you should avoid using functionality from the C++ standard libary. You will
 * almost certainly find it easiest to just write everything you need from scratch!
 */

/* Function: spellCheck
 * ---------------------
 *  Prints the words that are in an article but not in a dictionary (has duplicates).
 *  
 *  article: pointer to an array that contains the article string
 *  dictionary: pointer to an array that contains the dictionary string
 *
 *  output: prints words in the article but not the dictionary
 */
void spellCheck(char article[], char dictionary[]) {
	char *curr_pos_ptr;
	char *word_ptr;
	char **dict_ptrs_arr;
	int32_t size = makeDictPtrArray(&dict_ptrs_arr, dictionary);
	char **curr_dict_ptr;

	curr_pos_ptr = article;
	curr_dict_ptr = dict_ptrs_arr;
	while ((word_ptr = nextWord(&curr_pos_ptr)) != 0) {
		int32_t result = binarySearch(word_ptr, curr_dict_ptr, size);
		if (result == 0) {
			printWord(word_ptr);
		}
	}
	free(dict_ptrs_arr);
}

/* Function: binarySearch
* ---------------------
*  Searches the dictionary of pointers to strings for a string matching word.
*
*  word: pointer to a string containing a word ended by punctuation
*  ptrs_dict: array of pointers that point to the words in the dictionary, 
*             words seperated by '\n'
*  size: size of the ptrs_dict
*
*  output: returns 1 if the word is in the dictionary and a 0 otherwise
*/
int32_t binarySearch(char word[], char* ptrs_dict[], int32_t size) {
	int32_t min = 0;
	int32_t max = size - 1;
	int32_t mid;

	while (max >= min) {
		mid = midpoint(min, max);
		int32_t result = compareWords(word, ptrs_dict[mid]);
		if (result == EQUAL) {
			return 1;
		}
		else if (result == LESS_THAN) {
			min = mid + 1;
		}
		else  if (result == GREATER_THAN) {
			max = mid - 1;
		}
		else {
			printf("WTF?");
		}
	}

	return 0;
}

/* Function: midpoint
* ---------------------
*  Calculates the midpoint between to numbers.
*
*  min: smaller value
*  max: larger value
*
*  output: the midpoint between min and max
*/
int32_t midpoint(int32_t min, int32_t max) {
	int32_t mid;
	mid = min + (max - min) / 2;
	return mid;
}

/* Function: makeDictPtrArray
* ---------------------
*  Creates an array of pointers that point to all the words in the dictionary
*
*  dict_ptrs: pointer to an array of strings (should start empty and be filled by function)
*  dict: pointer to the dictionary string
*
*  output: - returns the size of the newly allocated array
           - allocates a new array for the dictionary pointers
*
*  IMPORTANT:
*      - When this function is called the caller must free dict_ptrs
*        before the end of the program
*/
int32_t makeDictPtrArray(char ***dict_ptrs, char dict[]) {
	char *curr;
	char *word;
	int32_t size = 0;
	curr = dict;
	char **new_dict = (char**) malloc(sizeof(char*));
	if (new_dict == NULL) {
		printf("malloc failed in makeDictPtrArray\n");
		exit(EXIT_FAILURE);
	}
	
	while ((word = nextDictWord(&curr)) != 0) {
		size += 1;
		char **tmp = (char**) realloc(new_dict, size * sizeof(char*));
		if (tmp == NULL) {
			printf("realloc failed in makeDictPtrArray\n");
			printf("returing dict with %d pointers", --size);
			free(tmp);
			break;
		}
		else {
			new_dict = tmp;
			new_dict[size - 1] = word;
		}
	}
	*dict_ptrs = new_dict;
	return size;
}

/* Function: nextDictWord
* ---------------------
*  Goes to the next word in the dictionary and returns a pointer to that word
*
*  current: pointer to the current position pointer
*
*  output: - returns 0 if there are no more words or a pointer to the next word
*          - changes the current pointer to just after the returned word
*/
char* nextDictWord(char**current) {
	char *output;

	if (**current == 0 || *(*current + 1) == 0) {
		output = 0;
	}

	// Checks if pointer is already at a word
	if (isWord(*current)) {
		output = *current;
		while (**current != '\n') {
			*current += 1;
		}
	}

	else {
		while (!isWord(*current)) {
			// If we reach the end return a 0
			if (**current == 0) {
				output = 0;
				return output;
			}
			*current += 1;
		}
		output = *current;
		while (**current != '\n') {
			*current += 1;
		}
	}
	return output;
}

/* Function: nextWord
* ---------------------
*  Goes to the next word in the article and returns a pointer to that word
*
*  current: pointer to the current position pointer
*
*  output: - returns 0 if there are no more words or a pointer to the next word
*          - changes the current pointer to just after the returned word
*/
char* nextWord(char **current) {
	char *output;

	// If at end of article or dictionary return 0
	// NOTE: even if there is a single letter before the end it can't be a word
	//       because a word must contain at least 2 letters
	if (**current == 0 || *(*current + 1) == 0) {
		output = 0;
	}

	// Checks if pointer is already at a word
	if (isWord(*current)) {
		output = *current;
		goToEnd(current);
	}

	else {
		while (!isWord(*current)) {
			// If we reach the end return a 0
			if (**current == 0) {
				output = 0;
				return output;
			}
			*current += 1;
		}
		output = *current;
		goToEnd(current);
	}
	return output;
}

/* Function: compareWords
* ---------------------
*  Compares two words to see if they are equal or whether the first word would be earlier
*  or later in an alphebatized dictionary relative to the second word.
*
*  a_word: pointer to the article word
*  d_word: pointer to the dictionary word
*
*  output: - returns EQUAL if the words are equal
*          - returns LESS_THAN if the second word is before the first word in
*            an alphebatized dictionary
*          - returns GREATER_THAN if the second word is after the first word in
*            an alphebatized dictionary
*/
int32_t compareWords(char *a_word, char *d_word) {
	int32_t output;
	char *a_ptr, *d_ptr;
	a_ptr = a_word;
	d_ptr = d_word;
	while (TRUE) {
		if (*d_ptr == '\n' && !isLetter(*a_ptr)) {
			output = EQUAL;
			break;
		}
		while (!isLetter(*d_ptr) && *d_ptr != '\n') d_ptr += 1;
		if (*d_ptr == '\n' && isLetter(*a_ptr)) {
			output = LESS_THAN;
			break;
		}
		if (isLetter(*d_ptr) && !isLetter(*a_ptr)) {
			output = GREATER_THAN;
			break;
		}
		if (toLowercase(*d_ptr) < toLowercase(*a_ptr)) {
			output = LESS_THAN;
			break;
		}
		if (toLowercase(*d_ptr) > toLowercase(*a_ptr)) {
			output = GREATER_THAN;
			break;
		}

		a_ptr++;
		d_ptr++;
	}
	return output;
}

/* Function: isWord
* ---------------------
*  Determines if a pointer is pointing to a word as defined by the specs
*
*  word: pointer to a character string
*
*  output: - returns TRUE if word points to a word as defined by the specs
*          - returns FALSE if word does not point to a word
*/
int32_t isWord(char *word) {
	int32_t output = FALSE;
	if (isLetter(word[0]) && isLetter(word[1])) {
		output = TRUE;
	}
	return output;
}

/* Function: printWord
* ---------------------
*  Prints a word by printing every character up to a non-letter
*
*  string: pointer to a character string
*
*  output: prints out a word up to a non-letter
*/
void printWord(char *string) {
	if (string == 0) {
		return;
	}
	while (isLetter(*string)) {
		putchar(*string);
		string += 1;
	}
	putchar('\n');
	fflush(stdout);
}

/* Function: toLowercase
* ---------------------
*  Returns a lowercase letter given a letter that may be either
*  uppercase or lowercase
*
*  c: a character
*
*  output: returns a lowercase character
*/
char toLowercase(char c) {
	if (!isLetter(c)) {
		// printf("That is not a letter");
		return 0;
	}
	else if (c >= 'a' && c <= 'z') {
		return c;
	}
	else {
		return c + ('a' - 'A');
	}
}

/* Function: isLetter
* ---------------------
*  Checks if a character is a letter (a-z or A-Z)
*
*  c: a character
*
*  output: returns TRUE if c is a letter and FALSE otherwise
*/
int32_t isLetter(char c) {
	int32_t output = FALSE;
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
		output = TRUE;
	}
	return output;
}

/* Function: goToEnd
* ---------------------
*  Moves the current pointer to the end of the string.
*
*  current: pointer to the current position pointer
*/
void goToEnd(char **current) {
	while (isLetter(**current)) {
		*current += 1;
	}
}