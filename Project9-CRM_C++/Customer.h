#ifndef _Customer_h
#define _Customer_h

#include "String.h"

/*
 * Node that I've made the Customer's name a constant, so once the name
 * is set, you cannot change it. Lots of comments about this below 
 */
struct Customer {
	const String name;
	int rattles;
	int bottles;
	int diapers;


	/*
	* I've made the "name" component of a Customer a constant
	* that means, you can't re-assign the name. You can initialize
	* the name to whatever you want, but you can't reassign it (recall
	* C++ distinguishes between initialization and reassignment)
	* Since the assignment operator (operator=) is a reassignment op
	* we can't initialize the customer's name using
	*	this->name = n;
	* C++ interprets that line as a reassignment of the name, which is
	* no illegal (since I made name a constant)
	* instead, I have to initialize the name inside a Customer using goofy
	* C++ syntax that is designed to ensure we mean initialization
	* (and not reassignment -- C++ distinguishes between the two)
	* That syntax is used below. Note that the name field has its
	* constructor invoked (it happens to be a copy constructor, but
	* its a constructor) outside of the function body -- before the function
	* body. That syntax is intended to make you think that, "Oh I must
	* be initializing the string"
	*
	* Anyway, there is no way to change a Customer's name. The name is
	* set when the Customer is first created/initialized.
	* I made this design choice because our tree will be organized by
	* customer name. If you were to change the name of the customer, then
	* the tree would have to re-organize itself (which is really hard to
	* do). The easiest fix (by far) is to not permit the names to be changed */
	Customer(String n) : name(n) {
		bottles = 0;
		rattles = 0;
		diapers = 0;
	}
};


#endif /* _Customer_h */