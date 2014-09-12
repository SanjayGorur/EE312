#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>
#include "String.h"
#include "CustomerDB.h"

/* DONE */
CustomerDB::CustomerDB(void) {
	root = nullptr;
	length = 0;
}

/* DONE */
int CustomerDB::size(void) {
	return length;
}

/* DONE */
void recursiveDelete(TreeNode* p) {
	if (p == nullptr) { return; }
	recursiveDelete(p->left);
	recursiveDelete(p->right);
	delete p;
}

/* DONE */
CustomerDB::~CustomerDB(void) {
	recursiveDelete(root);
}

/* Clears the database */
void CustomerDB::clear(void) {
	recursiveDelete(root);
	root = nullptr;
	length = 0;
}


/* NOTE: You may want to write other functions, I found successor and smallest to be useful
 * but that's entirely up to you. If you write functions, make sure they're local to this
 * file, do not modify any of the .h files -- don't make your functions member functions */

// finds the smallest node in the subtree of root
TreeNode* smallest(TreeNode* root) {
	if (root == 0) { return nullptr; }
	while (root->left != 0) {
		root = root->left;
	}
	return root;
}

TreeNode* successor(TreeNode* node) {
	if (node == nullptr) {
		return nullptr;
	}

	if (node->right != nullptr) {
		node = smallest(node->right);
	}
	else {
		TreeNode* parent = node->parent;
		while (parent != nullptr && node == parent->right) {
			node = parent;
			parent = node->parent;
		}
		node = parent;
	}
	return node;
}

/* Returns a TreeScanner with the first value set to the smallest node */
TreeScanner CustomerDB::startScan() {
	TreeScanner result;
	result.p = smallest(root);
	return result;
}

/* advances the scanner to the next node */
void TreeScanner::advance(void) {
	if (p == nullptr) { return; } // Empty tree or already finished

	p = successor(p);
}

// Inserts the leaf into the tree made from root
TreeNode* insert(TreeNode* root, TreeNode* leaf) {
	if (root == nullptr) {
		root = leaf;
		return root;
	}

	TreeNode* parent = nullptr;
	TreeNode* p = root;
	while (p != nullptr) {
		parent = p;
		if (leaf->val.name < p->val.name) {
			p = p->left;
		}
		else {
			p = p->right;
		}
		if (p != nullptr) {
		}
	}

	if (parent == leaf) return root;
	if (leaf->val.name < parent->val.name) {
		parent->left = leaf;
	}
	else {
		parent->right = leaf;
	}
	leaf->parent = parent;

	return root;
}

/* TODO */
Customer& CustomerDB::operator[](String name) {
	/* Step 1: try to find a customer with the name in the tree 
	 * if you find a node with the right name, then return 
	 * the customer stored in that node (return a reference) */
	TreeNode* node = root;
	while (node != nullptr) {
		Customer& c = node->val;
		if (c.name == name) {
			return c;
		}
		else if (c.name < name) {
			node = node->right;
		}
		else if (c.name > name) {
			node = node->left;
		}
	}

	/* Step 2: if you can't find a customer with a matching name
	 * then create a new leaf node and add the leaf node to the tree */
	TreeNode* leaf = new TreeNode(name);
	assert(leaf->left == nullptr);
	assert(leaf->right == nullptr);
	root = insert(root, leaf);

	/* Step 3: once you've added the new leaf, return the customer 
	 * stored in the new leaf */
	length += 1;
	return leaf->val;
}

bool CustomerDB::isMember(String name) {
	/* return true if there's a node in the tree that matches the name
	* and return false otherwise */
	TreeNode* node = root;
	while (node != nullptr) {
		Customer& c = node->val;
		if (c.name == name) {
			return true;
		}
		else if (c.name < name) {
			node = node->right;
		}
		else if (c.name > name) {
			node = node->left;
		}
	}

	// only reaches if customer is not in database
	return false;
}

/* return the height of the tree */
uint32_t treeHeight(TreeNode* root) {
	/* an empty tree (root == null) has a height of */
	/* this function is most easily written using recursion */
	if (root == nullptr) return 0;

	uint32_t t1 = treeHeight(root->left) + 1;
	uint32_t t2 = treeHeight(root->right) + 1;
	return std::max(t1, t2);
}

/* DONE */
uint32_t CustomerDB::height(void) {
	return treeHeight(root);
}

// finds the median of a vector of tree nodes
int findMedian(std::vector<TreeNode*> v) {
	TreeNode** right = &v[v.size()-1];
	TreeNode** left = &v[0];
	int index = 0;
	while (left < right) {
		left += 1;
		right -= 1;
		index += 1;
	}
	return index;
}

TreeNode* createTree(std::vector<TreeNode*> v, TreeNode* parent) {
	if (v.empty()) {
		return nullptr;
	}
	if (v.size() == 1) {
		v[0]->parent = parent;
		return v[0];
	}

	int median_i = findMedian(v);
	TreeNode* median = v[median_i];

	// Split the vector in two
	std::vector<TreeNode*>::const_iterator first_l = v.begin();
	std::vector<TreeNode*>::const_iterator last_l = v.begin() + median_i;
	std::vector<TreeNode*> left_v(first_l, last_l);

	std::vector<TreeNode*>::const_iterator first_r = v.begin() + median_i + 1;
	std::vector<TreeNode*>::const_iterator last_r = v.end();
	std::vector<TreeNode*> right_v(first_r, last_r);

	median->parent = parent;
	median->left = createTree(left_v, median);
	median->right = createTree(right_v, median);

	return median;
}

void CustomerDB::rebalance(void) {
	/* this is the hardest function to write in this assignment and writing it 
	 * well is the "bonus point" in this assignment
	 *
	 * proceed in two steps, first put all the nodes of the tree into 
	 * an array (or vector). You can use an array of Customer or
	 * you can do what I did, and use an array of TreeNode* pointers
	 * whatever you do, be sure the array you create is sorted by customer name
	 *
	 * In the second step, use a recursive function (not this one, write 
	 * a separate function that is recursive). That function should take your
	 * array as its input and it should return a pointer to the root of
	 * a tree where the median value in the array has been made into the root
	 * of the tree. If you recursively perform that task to build the 
	 * left and right subtrees, then the tree you create will be perfectly 
	 * balanced (will have the minimum possible height)
	 */
	std::vector<TreeNode*> t_nodes;
	TreeScanner tscan = startScan();
	while (!tscan.done()) {
		t_nodes.push_back(tscan.p);
		tscan.advance();
	}

	for (int i = 0; i < t_nodes.size(); i++) {
		t_nodes[i]->left = nullptr;
		t_nodes[i]->right = nullptr;
	}
	root = createTree(t_nodes, nullptr);
}