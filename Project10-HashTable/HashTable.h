#ifndef _HashTable_h
#define _HashTable_h 1
#include <stdint.h>
#include <assert.h>
#include <vector>

using std::vector;

#include "String.h"

template <bool use_Knuth = false, bool use_SmartHash = false>
struct HashTable {
private:
	using MyType = HashTable<use_Knuth>;

	struct Cell { // used to make linked lists
		Cell* next;
		String val;
		Cell(void) {}
		Cell(const String& s) : val(s) { 
			next = 0;
		}
	};

	static unsigned smartHash(String s) {
		unsigned h = 0;
		for (int k = 0; k < s.size(); k += 1) {
			h = (h << 5) + h + tolower(s[k]); // case insensitive
		}
		return h;
	}

	static unsigned simpleHash(String s) {
		unsigned h = 0;
		for (int k = 0; k < s.size(); k += 1) {
			h = h + tolower(s[k]); // case insensitive
		}
		return h;
	}

	typedef Cell* Chain;
	/* the table is an array of pointers to Cells
	 * i.e., and array of chains */
	Chain* table;
	uint32_t len; // number of keys in the table
	uint32_t cap; // number of chains in the table (i.e., array size)

	static const uint32_t initial_size = 8;

	HashTable(MyType&) {};  // I don't want to make copies of my HashTable in Project10, so I'm "hiding" the copy constructor
	MyType& operator=(MyType&) { return *this; } // ditto the assignment operator, no copies in Project10
public:
	HashTable(void) {
		cap = initial_size;
		len = 0;
		table = new Chain[cap];
		/* we need to initialize all the entries in the table to be null pointers (they're not null by default) */
		for (uint32_t k = 0; k < cap; k += 1) {
			table[k] = 0;
		}
	}

	~HashTable(void) { destroy(); }

	uint32_t size(void) const { return len;  }

	bool isMember(const String& key) {
		Cell* p = locate(key);
		while (p != 0) {
			if (p->val == key) { return true; }
			p = p->next;
		}
		return false;
	}

	void insert(const String& key) {
		if (isMember(key)) { return; }

		if (len > 0.8 * cap) { resize(); }

		Chain& p = locate(key);
		Cell* new_cell = new Cell(key);
		new_cell->next = p;
		p = new_cell;
		len += 1;
	}

	void remove(const String& key) {
		if (!isMember(key)) { return; }
		Chain& p = locate(key);
		Cell* delete_me = p;
		if (p->val == key) { 
			p = delete_me->next; 
			delete delete_me;
		} else {
			while (true) {
				Chain& p = delete_me->next;
				delete_me = delete_me->next;
				if (delete_me->val == key) {
					p = delete_me->next;
					delete delete_me;
					break;
				}
			}
		}
		len -= 1;
	}

	vector<uint32_t> chainLengths(void) {
		vector<uint32_t> result;
		result.reserve(cap);
		for (uint32_t k = 0; k < cap; k += 1) {
			uint32_t len = 0;
			Cell* p = table[k];
			while (p) {
				p = p->next;
				len += 1;
			}
			result.push_back(len);
		}
		return result;
	}

	unsigned longestChain(void) {
		unsigned max_len = 0;
		for (int k = 0; k < cap; k += 1) {
			int chain_len = 0;
			Cell* p = table[k];
			while (p != 0) {
				chain_len += 1;
				p = p->next;
			}
			if (chain_len > max_len) { max_len = chain_len; }
		}
		return max_len;
	}

private:

	Chain& locate(const String& key) const {
		/* Step1: Ensure we know the log of the capacity so that we can use the Knuth multiplication method
		 * to convert hashcodes into array indexes */
		/* it's kinda a pain to calculate the log of the capacity
		 * so I'll recaculate the log (i.e., the shift amount for my Knuth multiplication) only when
		 * capacity has changed since the last time I calculated a shift amount */
		static int last_cap = 0; // static local variables are really globals, they're not stored on the stack
		static int shift = 32; // static local variables are not re-initialized each time the function is called
		if (cap != last_cap) {
			last_cap = cap;
			unsigned c = cap;
			shift = 32;
			while (c > 1) {
				shift = shift - 1;
				c = c / 2;
			}
		}
		assert(1 << (32-shift) == cap); // shift should be the log (base 2) of cap, just checking to make sure

		/* Step 2: Select which Chain in the table this key should be stored in
		 * and return a reference to the first pointer in the Chain */

		/* use the Knuth Multiplication method to convert the HashCode into a table index */
		unsigned k;
		if (use_SmartHash) {
			k = smartHash(key);
		} else {
			k = simpleHash(key);
		}

		if (use_Knuth) {
			k = (k * 0x6b43a9b5) >> shift;
		} else {
			k = k % cap;
		}

		return table[k];
	}

	void destroy(void) {
		for (uint32_t k = 0; k < cap; k += 1) {
			Cell* p = table[k];
			while (p != 0) {
				Cell* q = p;
				p = p->next;
				q->next = 0;
				delete q;
			}
			table[k] = 0; // this line is important if and only if Cell has a destructor. Can you figure out why?
		}
		delete[] table;
	}

	void resize(void) {
		// Get pointers to all of the old chains
		vector<Chain> old_chains;
		for (int i = 0; i < cap; i++) {
			if (table[i] != 0) old_chains.push_back(table[i]);
		}
		delete[] table;

		cap *= 2;
		table = new Chain[cap];
		/* we need to initialize all the entries in the table to be null pointers (they're not null by default) */
		for (uint32_t k = 0; k < cap; k += 1) {
			table[k] = 0;
		}

		/* Now we need to move the links from the old table to the new table */
		for (int i = 0; i < old_chains.size(); i++) {
			Cell* cell = old_chains[i];
			while (cell != 0) {
				Cell* next_cell = cell->next;

				Chain& p = locate(cell->val);
				cell->next = p;
				p = cell;

				cell = next_cell;
			}
		}
	}
};

using BasicHashTable = HashTable<false, false>; 
using KnuthHashTable = HashTable<true, false>;
using BasicSmartHashTable = HashTable<false, true>;
using KnuthSmartHashTable = HashTable<true, true>;

#endif /* _HashTable_h */