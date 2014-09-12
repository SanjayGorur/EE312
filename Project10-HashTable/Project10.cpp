#include <stdio.h>
#include <stdint.h>
#include <vector>
#include <cctype>
#include "HashTable.h"

/*
 * NOTE: the HashTable::resize function (which you need to write for part one)
 * is in the HashTable.h file, near the end of the file. 
 */

using std::vector;
extern vector<String> dictionary; // this vector is a global variable defined in main.cpp (I'm declaring it here)

class ArticleScanner {
private:
	char* curr_pos;
	String word;

public:
	ArticleScanner(char* ptr) {
		curr_pos = ptr;
		word = nextWord();
	}

	~ArticleScanner() {};

	String get(void) {
		return word;
	}

	void advance(void) {
		word = nextWord();
	}

	bool done() {
		return (word == String(""));
	}

private:
	String nextWord() {
		char* word_ptr;
		int word_len = 0;

		if (*curr_pos == 0 || *(curr_pos + 1) == 0) return String("");

		// Checks if pointer is already at a word
		if (isWord(curr_pos)) {
			word_ptr = curr_pos;
			word_len = goToEnd();
		}
		else {
			while (!isWord(curr_pos)) {
				// If we reach the end return a null string
				if (*curr_pos == 0) return String("");
				curr_pos += 1;
			}
			word_ptr = curr_pos;
			word_len = goToEnd();
		}

		char* new_word = new char[word_len + 1];
		for (int i = 0; i < word_len; i++) {
			new_word[i] = word_ptr[i];
		}
		new_word[word_len] = 0;

		String output(new_word);
		delete[] new_word;
		return output;
	}

	bool isLetter(char c) {
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
	}

	bool isWord(char *word) {
		return isLetter(word[0]) && isLetter(word[1]);
	}

	int goToEnd() {
		int word_len = 0;
		while (isLetter(*curr_pos)) {
			word_len += 1;
			curr_pos += 1;
		}
		return word_len;
	}
};

ArticleScanner startScan(char* article) {
	return ArticleScanner(article);
}


/* find words in the article and check to see if there's a matching
 * word in the dictionary. Use a hash table for the dictionary
 * (yes, I realize you could just use the vector, and that you could even
 * binary search the vector, but this project is about getting some
 * practice with HashTables....)
 * print out all misspelled words.
 *
 * For the standard project, a word from the article must appear exactly the 
 * same way in the dictionary -- i.e., the comparison is case sensitive and
 * "Apple" does not match "apple".
 *
 * If you want to try the bonus, modify the hash table's hash function so that
 * it is case insensitive and modify the String's operator== so that it is
 * also case insensitive.
 */
void spellCheck(char article[]) {
	KnuthSmartHashTable h_table;
	for (int i = 0; i < dictionary.size(); i++) {
		h_table.insert(dictionary[i]);
	}

	for (ArticleScanner k = startScan(article); !k.done(); k.advance()) {
		if (!h_table.isMember(k.get())) {
			printf("%s\n", k.get().c_str());
		}
	}
}