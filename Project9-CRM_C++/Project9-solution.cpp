#include <stdio.h>
#include <iostream>
#include "String.h"
#include "Customer.h"
#include "CustomerDB.h"

void readString(String&);
void readNum(int&);

CustomerDB database;

int num_bottles = 0;
int num_rattles = 0;
int num_diapers = 0;

std::ostream &operator<<(std::ostream &stream, const String &str) {
	stream << str.c_str();
	return stream;
}


/* clear the inventory and reset the customer database to empty */
void reset(void) {
	database.clear();
	num_bottles = 0;
	num_rattles = 0;
	num_diapers = 0;
}

/*
* selectInventItem is a convenience function that allows you
* to obtain a pointer to the inventory record using the item type name
* word must be "Bottles", "Diapers" or "Rattles"
* for example the expression *selectInventItem("Bottles") returns the
* current global variable for the number of bottls in the inventory
*/
int *selectInventItem(String word) {
	if (word == "Bottles") {
		return &num_bottles;
	} else if (word == "Diapers") {
		return &num_diapers;
	} else if (word == "Rattles") {
		return &num_rattles;
	}

	/* NOT REACHED */
	return 0;
}

/*
* this overload of selectInventItem is similar to selectInventItem above, however
* this overload takes a Customer as a second argument and selects the data member
* of that Customer that matches "word". The function returns a pointer to one of the three data
* members from the specified customer.
*/
int *selectInventItem(String word, Customer& cust) {
	if (word == "Bottles") {
		return &cust.bottles;
	} else if (word == "Diapers") {
		return &cust.diapers;
	} else if (word == "Rattles") {
		return &cust.rattles;
	}

	/* NOT REACHED */
	return 0;
}


/*
* findMax searches through the CustomerDB "database" and returns the Customer
* who has purchased the most items of the specified type.
* type must be one of "Bottles", "Rattles" or "Diapers".
*
* Note: if two or more Customers are tied for having purchased the most of that item type
* then findMax returns the first Customer in the CustomerDB who has purchased that maximal
* quantity.
*
* Note: in the special case (invalid case) where there are zero Customers in the
* CustomerDB, fundMax returns a null pointer (0)
*/
Customer *findMax(String type) {
	Customer* result = 0;
	int max = 0;
	for (TreeScanner k = database.startScan(); !k.done(); k.advance()) {
		Customer& cust = k.get();
		int *p = selectInventItem(type, cust);
		if (*p > max) {
			max = *p;
			result = &cust;
		}
	}

	return result;
}

void processPurchase() {
	String name;
	String type;
	int val;

	readString(name);
	readString(type);
	readNum(val);

	int *item = selectInventItem(type);
	if (*item < val) {
		std::cout << "Sorry " << name << ", we only have " << *item << " " << type << std::endl;
	} else if (val > 0) {
		*item -= val;
		*selectInventItem(type, database[name]) += val;
	}
}

char *types[3] = {"Diapers", "Bottles", "Rattles"};
char *types_lower[3] = {"diapers", "bottles", "rattles"};
void processSummarize() {
	printf("current height is %d\n", database.height());
	database.rebalance();
	printf("new height is %d\n", database.height());

	std::cout << "There are " << num_diapers << " diapers, " << num_bottles << " bottles and "
		<< num_rattles << " rattles in inventory" << std::endl;
	std::cout << "we have had a total of " << database.size() << " different customers" << std::endl;

	for (int i = 0; i < 3; i++) {
		Customer *cust = findMax(types[i]);
		if (cust) {
			std::cout << cust->name << " has purchased the most " << types_lower[i] << " ("
				<< *selectInventItem(types[i], *cust) << ")" << std::endl;
		} else {
			std::cout << "no one has purchased any " << types_lower[i] << std::endl;
		}
	}
}

void processInventory() {
	String type;
	int val;

	readString(type);
	readNum(val);

	if (val > 0) {
		*selectInventItem(type) += val;
	}
}