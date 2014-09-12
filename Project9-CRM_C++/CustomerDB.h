#ifndef _CustomerDB_h
#define _CustomerDB_h

#include "String.h"
#include "Customer.h"

typedef Customer* CustomerPtr;

struct TreeNode {
	Customer val;
	TreeNode* left;
	TreeNode* right;
	TreeNode* parent;

	/* let's name initialization of the nodes easier 
	 * here's a constructor that sets the name of the customer (invokes
	 * the constructor for the customer) and sets the pointers to null */
	TreeNode(String name) : val(name) {
		left = nullptr;
		right = nullptr;
		parent = nullptr;
	}
};

struct TreeScanner {
	TreeNode* p;
	bool done(void) { // returns true if the scan is complete (NOTE: p will be a null pointer
		return p == nullptr;
	}

	Customer& get(void) { // returns a reference to the Customer stored at the current node
		return p->val;
	}

	/* you need to write this function, pretty easy (it's "successor" from your notes) */
	void advance(void); // advances the scanner to the next node in the tree
};

struct CustomerDB {
private:
	TreeNode* root; // a BST of Customer records
	uint32_t length; // number of Customers in the database

public:
	CustomerDB(void);
	~CustomerDB(void);
	int size(void);
	TreeScanner startScan(void);
	Customer& operator[](String name); 
	bool isMember(String name);
	void clear(void);

	uint32_t height(void); // return the height of the tree
	void rebalance(void); // reorganize the nodes in the tree so that the tree has minimal height
};


void processPurchase(void);
void processInventory(void);
void processSummarize(void);
void reset(void);

#endif /* _CustomerDB_h */