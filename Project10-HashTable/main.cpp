#define _CRT_SECURE_NO_WARNINGS 1 // this #define is for VS2012, which doesn't like the standard C library
#include <stdio.h> // declarations of fopen and fgets
#include <vector>
#include <math.h>
#include "String.h"
#include "HashTable.h"

using std::vector;

vector<String> dictionary; // dictionary is a collection of words built using the dictionary from Project 1

extern vector<String> dictionary; // this vector is a global variable defined in main.cpp (I'm declaring it here)
void checkBasicSimple(const vector<String>&); // checks the stats for one of the six combos required for part one (included below)
void checkKnuthSimple(const vector<String>&); // checks the stats for one of the six combos required for part one (included below)
void checkBasicSmart(const vector<String>&); // checks the stats for one of the six combos required for part one (included below)
void checkKnuthSmart(const vector<String>&); // checks the stats for one of the six combos required for part one (included below)

void partOne(void) {
	checkBasicSimple(dictionary);
	checkKnuthSimple(dictionary);
	checkBasicSmart(dictionary);
	checkKnuthSmart(dictionary);
}

void printStats(const vector<uint32_t>& x) {
	/* find the mean and max */
	double sum = 0.0;
	uint32_t max = 0;
	for (uint32_t k = 0; k < x.size(); k += 1) {
		sum += x[k];
		if (x[k] > max) { max = x[k]; }
	}
	double mean = sum / x.size();

	/* find the variance */
	double vsum = 0.0;
	for (uint32_t k = 0; k < x.size(); k += 1) {
		double t = x[k] - mean;
		vsum += t * t;
	}
	double variance = vsum / x.size();
	double std_dev = sqrt(variance);

	printf("Max was %d, mean was %g and standard deviation was %g\n",
		   max, mean, std_dev);
}


/* this is a template for checking one of our six pairings of HashFunction (simple, or smart)
* and array indexing (mod table size, or Knuth Multiplication).
* This happens to be for the pairing of Mod table size with simple hash */
void checkBasicSimple(const vector<String>& words) {
	BasicHashTable h;
	for (const String& s : words) {
		h.insert(s);
	}

	printf("results for SimpleHash multiplication:\n");
	printStats(h.chainLengths());
	printf("\n");
}

void checkKnuthSimple(const vector<String>& words) {
	KnuthHashTable h;
	for (const String& s : words) {
		h.insert(s);
	}

	printf("results for SimpleHash with Knuth multiplication:\n");
	printStats(h.chainLengths());
	printf("\n");
}


void checkBasicSmart(const vector<String>& words) {
	BasicSmartHashTable h;
	for (const String& s : words) {
		h.insert(s);
	}

	printf("results for SmartHash multiplication:\n");
	printStats(h.chainLengths());
	printf("\n");
}

void checkKnuthSmart(const vector<String>& words) {
	KnuthSmartHashTable h;
	for (const String& s : words) {
		h.insert(s);
	}

	printf("results for SmartHash with Knuth multiplication:\n");
	printStats(h.chainLengths());
	printf("\n");
}


#define ONE_MB (1 << 20)
#define BUFFER_SIZE ONE_MB

char article[BUFFER_SIZE]; // a large buffer to hold an article (1MB)

void spellCheck(char []); // this is the function you write (put your function in Project10.cpp

/*
* Read a .txt file into one of the two buffers
* this function does NOT confirm that the file is correctly formatted
*/
void readFile(const char file_name [], char buffer []) {
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

void partTwo(void) {
	readFile("greek-finances.txt", article);
	spellCheck(article);
	printf("****DONE****\n");
}

char dictionary_buffer[ONE_MB];
int main(void) {
	FILE* dict_file = fopen("american-english.txt", "r");
	if (dict_file == 0) {
		printf("unable to open dictionary file \"american-english.txt\"\n");
		return -1;
	}
	/* read dictionary */
	int result = fread(dictionary_buffer, sizeof(char), sizeof(dictionary_buffer), dict_file);

	char* p = dictionary_buffer;
	while (*p != 0) {
		char* word_start = p;
		while (*p != 0 && *p != '\n') { p += 1; }
		if (*p == '\n') { *p = 0; p += 1; }
		dictionary.push_back(String(word_start));
	}
	printf("looks like I found %d words in that dictionary\n", dictionary.size());

	partOne();
	partTwo();

}

