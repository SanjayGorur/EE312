/* 
 * EE312 Simple CRM Project
 *
 * Name: Phillip Lemons
 * EID: pl6664
 * EE312 -- 16495
 * Duncan Horn
 * Project 3, Customer Relationship Management
 */

#include <stdio.h>
#include <assert.h>
#include "String.h"
#include "Invent.h"

typedef enum {
	PURCHASE_FAILURE,
	PURCHASE_SUCCESS,
} PurchaseStatusCode;

typedef struct Item {
	String name;
	int quantity;
} Item;

#define MAX_CUSTOMERS 1000
#define MAX_ITEMS 3
#define DEBUG 0
Customer customers[MAX_CUSTOMERS];
Item inventory[MAX_ITEMS];
int num_customers = 0;
int num_items = 0;

void addToInventory(String* item_name, int item_count);
PurchaseStatusCode makePurchase(String* item_name, int item_count);
void updateCustomers(String* cust_name, String* item_name, int item_count);
void printInventory(void);
void printCustomers(void);
int invGet(String* item_name);
int getBestCustomerIndex(String* item_type);


/* clear the inventory and reset the customer database to empty */
void reset(void) {
	for (int i = 0; i < num_items; i++) {
		Item item = inventory[i];
		StringDestroy(&item.name);
	}
	for (int i = 0; i < num_customers; i++) {
		Customer c = customers[i];
		StringDestroy(&c.name);
	}
	num_items = 0;
	num_customers = 0;
}

/* Summarizes the current state of the database */
void processSummarize() {
	String d_s = StringCreate("Diapers");
	String r_s = StringCreate("Rattles");
	String b_s = StringCreate("Bottles");

	int diapers = invGet(&d_s);
	int rattles = invGet(&r_s);
	int bottles = invGet(&b_s);
	printf("There are %d diapers, %d bottles and %d rattles in inventory\n",
			diapers, bottles, rattles);
	printf("we have had a total of %d different customers\n", num_customers);

	int best_d_cust_index = getBestCustomerIndex(&d_s);
	int best_r_cust_index = getBestCustomerIndex(&r_s);
	int best_b_cust_index = getBestCustomerIndex(&b_s);
	if (best_d_cust_index != -1) {
		StringPrint(&customers[best_d_cust_index].name);
		printf(" has purchased the most diapers (%d)\n", customers[best_d_cust_index].diapers);
	}
	else {
		printf("no one has purchased any diapers\n");
	}

	if (best_b_cust_index != -1) {
		StringPrint(&customers[best_b_cust_index].name);
		printf(" has purchased the most bottles (%d)\n", customers[best_b_cust_index].bottles);
	}
	else {
		printf("no one has purchased any bottles\n");
	}

	if (best_r_cust_index != -1) {
		StringPrint(&customers[best_r_cust_index].name);
		printf(" has purchased the most rattles (%d)\n", customers[best_r_cust_index].rattles);
	}
	else {
		printf("no one has purchased any rattles\n");
	}

	StringDestroy(&b_s);
	StringDestroy(&r_s);
	StringDestroy(&d_s);
}

/* called when a purchase command is read from the input file */
void processPurchase() {
	if (DEBUG) {
		printf("\nProcessing Purchase\n");
	}
	String customer_name;
	String item_name;
	int item_count;
	readString(&customer_name);
	readString(&item_name);
	readNum(&item_count);
	if (DEBUG) {
		StringPrint(&item_name);
		printf(" %d\n", item_count);
	}
	if (item_count == 0) {
		StringDestroy(&customer_name);
		StringDestroy(&item_name);
		return;
	}
	PurchaseStatusCode success = makePurchase(&item_name, item_count);
	if (success == PURCHASE_FAILURE) {
		printf("Sorry ");
		StringPrint(&customer_name);
		printf(", we only have %d ", invGet(&item_name));
		StringPrint(&item_name);
		printf("\n");
	}
	else {
		updateCustomers(&customer_name, &item_name, item_count);
	}

	if (DEBUG) {
		printCustomers();
	}

	StringDestroy(&customer_name);
	StringDestroy(&item_name);
}

/* called when an inventory command is read from the input file */
void processInventory() {
	if (DEBUG) {
		printf("\nProcessing Inventory\n");
	}
	String item_name;
	int item_count;
	readString(&item_name);
	readNum(&item_count);
	addToInventory(&item_name, item_count);
	StringDestroy(&item_name);
	if (DEBUG) {
		printInventory();
	}
}

/* prints the customer database in a somewhat readable fashion */
void printCustomers() {
	for (int i = 0; i < num_customers; i++) {
		Customer c = customers[i];
		StringPrint(&c.name);
		printf(" has purchased %d bottles, %d rattles, %d diapers\n", 
				c.bottles, c.rattles, c.diapers);
	}
}

/* prints the inventory database in a somewhat readable fashion */
void printInventory() {
	for (int i = 0; i < num_items; i++) {
		Item item = inventory[i];
		StringPrint(&item.name);
		printf(" %d\n", item.quantity);
	}
}

/* Function: getBestCustomerIndex
 * -------------------------------
 *  Returns the index of the customer who has bought the most items of item_type
 *
 *  item_type: type of item that you want to find the best customer for
 *
 *  output: - returns the index of the best customer for item_type
 *              + index is in customers array
 */
int getBestCustomerIndex(String* item_type) {
	String d_s = StringCreate("Diapers");
	String r_s = StringCreate("Rattles");
	String b_s = StringCreate("Bottles");
	int most_items = 0;
	int index = -1;
	for (int i = 0; i < num_customers; i++) {
		Customer* c = &customers[i];
		if (StringIsEqualTo(item_type, &d_s)) {
			if (c->diapers > most_items) {
				most_items = c->diapers;
				index = i;
			}
		}
		else if (StringIsEqualTo(item_type, &r_s)) {
			if (c->rattles > most_items) {
				most_items = c->rattles;
				index = i;
			}
		}
		else if (StringIsEqualTo(item_type, &b_s)) {
			if (c->bottles > most_items) {
				most_items = c->bottles;
				index = i;
			}
		}
	}

	StringDestroy(&d_s);
	StringDestroy(&r_s);
	StringDestroy(&b_s);
	return index;
}

/* Function: invGet
 * -------------------
 *  Lets the inventory act like a dictionary of item quantities where "item_name" maps to the quantity of that item
 *
 *  item_name: name of the item to retrieve
 * 
 *  output: - returns -1 if it couldn't find item_name in the database
 *          - returns quantity of item with name item_name
 */
int invGet(String* item_name) {
	for (int i = 0; i < num_items; i++) {
		Item* item = &inventory[i];
		if (StringIsEqualTo(&item->name, item_name)) {
			return item->quantity;
		}
	}
	if (DEBUG) {
		fprintf(stderr, "Couldn't find item in inventory!\n");
	}
	return 0;
}

/* Function: updateCustomers
 * --------------------------
 *  Updates a customer in the customers database or adds a new one if none exists
 *
 *  cust_name: name of the customer making purchase
 *  item_name: type of item being purchased
 *  item_count: number of items being purchased
 *
 *  output: - Case 1: there is no customer with cust_name in the database
 *              + creates a new customer with name=cust_name and sets the correct item to item_count
 *              + uses a duplicate of cust_name so caller can deallocate original cust_name
 *          - Case 2: there is already a customer with cust_name in the database
 *              + adds item_count to the original item's quantity for the customer
 */
void updateCustomers(String* cust_name, String* item_name, int item_count) {
	String bottles_s = StringCreate("Bottles");
	String rattles_s = StringCreate("Rattles");
	String diapers_s = StringCreate("Diapers");

	for (int i = 0; i < num_customers; i++) {
		Customer* cust = &customers[i];
		if (StringIsEqualTo(&cust->name, cust_name)) {
			if (StringIsEqualTo(item_name, &bottles_s)) {
				cust->bottles += item_count;
			}
			else if (StringIsEqualTo(item_name, &rattles_s)) {
				cust->rattles += item_count;
			}
			else if (StringIsEqualTo(item_name, &diapers_s)) {
				cust->diapers += item_count;
			}
			StringDestroy(&bottles_s);
			StringDestroy(&rattles_s);
			StringDestroy(&diapers_s);
			return;
		}
	}

	if (DEBUG) {
		printf("ADDING CUSTOMER\n");
	}
	Customer new_cust;
	new_cust.name = StringDup(cust_name);
	new_cust.bottles = 0;
	new_cust.diapers = 0;
	new_cust.rattles = 0;

	if (StringIsEqualTo(item_name, &bottles_s)) {
		new_cust.bottles += item_count;
	}
	else if (StringIsEqualTo(item_name, &rattles_s)) {
		new_cust.rattles += item_count;
	}
	else if (StringIsEqualTo(item_name, &diapers_s)) {
		new_cust.diapers += item_count;
	}

	customers[num_customers++] = new_cust;

	StringDestroy(&bottles_s);
	StringDestroy(&rattles_s);
	StringDestroy(&diapers_s);
}

/* Function: addToInventory
 * --------------------------
 *  Creates a new item in the database or increases the quantity of an item.
 *
 *  item_name: type of item being added to the inventory
 *  item_count: number of items being added
 *
 *  output: - Case 1: there is no item with item_name in the database
 *              + creates a new item with name=item_name and quantity=item_count
 *              + uses a duplicate of item_name so caller can deallocate original item_name
 *          - Case 2: there is already an item with item_name in the database
 *              + adds item_count to the original item's quantity
 */
void addToInventory(String* item_name, int item_count) {
	for (int i = 0; i < num_items; i++) {
		Item* item = &inventory[i];
		if (StringIsEqualTo(&item->name, item_name)) {
			item->quantity += item_count;
			return;
		}
	}
	if (DEBUG) {
		printf("ADDING ITEM\n");
	}
	Item new_item;
	new_item.name = StringDup(item_name);
	new_item.quantity = item_count;

	inventory[num_items++] = new_item;
}

/* Function: purchase_from_inventory
 * ------------------------------------
 *  Removes item_count number of items of type item_name from the inventory
 *
 *  item_name: type of item being purchased
 *  item_count: number of items being purchased
 *
 *  output: - returns a PurchaseStatusCode
 *              + PURCHASE_SUCCESS for a successful purchase
 *              + PURCHASE_FAILURE for a failed purchase
 */
PurchaseStatusCode makePurchase(String* item_name, int item_count) {
	for (int i = 0; i < num_items; i++) {
		Item* item = &inventory[i];
		if (StringIsEqualTo(&item->name, item_name)) {
			if (item->quantity >= item_count) {
				item->quantity -= item_count;
				if (DEBUG) {
					printf("PURCHASING ITEMS\n");
					printf("%d\n", item_count);
					StringPrint(item_name);
					printf(" now has %d\n", item->quantity);
				}
				return PURCHASE_SUCCESS;
			}
			else {
				return PURCHASE_FAILURE;
			}
		}
	}
	if (DEBUG) {
		fprintf(stderr, "Could not find item in inventory\n");
	}
	return PURCHASE_FAILURE;
}