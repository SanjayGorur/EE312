//#include <stdafx.h>
#include "Bugs.h"
#include <assert.h>

int time_step = 0;
int average_age = 0;
int average_generation = 0;
int percent_straight;
int percent_right;
int percent_left;
int percent_back;
int total_left = 0;
int total_right = 0;
int total_straight = 0;
int total_back = 0;

/* DONE: you don't need to use this function in your solution
 * however this function is used by createWorld to generate random bugs */
Bug createBug(int x, int y) {
	Bug b;
	b.age = 0;
	b.generation = 0;
	b.x = x;
	b.y = y;
	b.dir = rand() % 8;
	b.health = START_HEALTH;
	for (int k = 0; k < 8; k += 1) {
		if (DISTRIBUTED_GENES) {
			b.genes[k] = GENE_TOTAL / 8;
		} else {
			if (k == 0) {
				b.genes[k] = GENE_TOTAL;
			} else {
				b.genes[k] = 0;
			}
		}
	}
	total_straight += b.genes[0];
	total_right += b.genes[1] + b.genes[2] + b.genes[3];
	total_left += b.genes[5] + b.genes[6] + b.genes[7];
	total_back += b.genes[4];
	return b;
}

/* DONE: */
void createWorld(void) {
	/* empty the world */
	for (int i = 0; i < WORLD_SIZE; i += 1) {
		for (int j = 0; j < WORLD_SIZE; j += 1) {
			world[i][j] = EMPTY;
		}
	}

	/* create food */
	for (int k = 0; k < INITIAL_FOOD; k += 1) {
		int x = rand() % WORLD_SIZE;
		int y = rand() % WORLD_SIZE;
		world[x][y] = FOOD;
	}
	
	/* create bugs */
	while (bug_list.size() < INITIAL_BUGS) {
		int x = rand() % WORLD_SIZE;
		int y = rand() % WORLD_SIZE;
		if (world[x][y] == EMPTY) {
			world[x][y] = bug_list.size();
			bug_list.push_back(createBug(x, y));
		}
	}	
}

/*
 * DONE: keep in mind that bugs that "fall off" the edge of the world
 * automatically reappear on the opposite edge (i.e, the world is round,
 * well, if you really want to get topographical, our bugs live on a donut.
 */ 
int wrap(int y) {
	int out;
	if (y < 0) { out = WORLD_SIZE - 1; }
	else if (y == WORLD_SIZE) { out = 0; }
	else { out = y; }
	assert(out >= 0);
	assert(out < WORLD_SIZE);
	return out;
}

/* DONE: you can use this function if you want, it's not required that you use it */
int newX(int x, int dir) {
	switch (dir) {
		case 0: // move up
		case 1:
		case 7:
			return wrap(x - 1);
			break; // not reached
		case 3: // move down
		case 4:
		case 5:
			return wrap(x + 1);
			break; // not reached
		case 2: // move left or right
		case 6:
			return x;
	}
	/* NOT REACHED */
	return 0;
}


/* DONE: you can use this function if you want, it's not required that you use it */
int newY(int y, int dir) {
	switch (dir) {
	case 1:
	case 2:
	case 3:
		return wrap(y + 1);
	case 5:
	case 6:
	case 7:
		return wrap(y - 1);
	case 0:
	case 4:
		return y;
	}
	/* NOT REACHED */
	return 0;
}


/* DONE: 
 * Add NUM_FOOD food objects in random locations
 * if you drop food on a bug, be sure to feed the bug
 */
void addFood(void) {
	for (int food = 0; food < NUM_FOOD; food += 1) {
		int x = rand() % WORLD_SIZE;
		int y = rand() % WORLD_SIZE;
		switch (world[x][y]) {
		case EMPTY: // empty, add the food
			world[x][y] = FOOD;
			break;
		case FOOD: // already has food, do nothing
			break;
		default: // bug, feed the bug
			bug_list[world[x][y]].health += EAT_HEALTH;
		}
	}	
	
}

/* TODO... finish this function as described below
 * move bugs 
 * if they move onto food, feed them
 * if they move onto another bug, no big deal.  Two bugs
 * can be in the same square.  Let's say bug#2 and bug #6 are in the same
 * square.  Well, both bugs are still in the bug_list, so they
 * will still be able to move next time step.
 * Since the world[][] array can only hold either a 2 or a 6, we'll just
 * put one of them there.  The easiest thing to do is just put which
 * ever bug moves into the square first into the square.  When the next
 * bug moves into the square, update his x and y (in bug_list) so that
 * he is in the square, leave the other bug alone (i.e., it's also in
 * the same square) and then set world[x][y] = 6 (the bug # of the second
 * bug).  No harm done.  The graphics will draw only the second bug
 * but who cares.  The next time step they'll move into different squares
 * (probably) and no one will notice.
 * NOTE: only the first bug to move into the square gets to eat the food
 * there.
 */
void moveBugs(void) {
	int total_age = 0;
	int total_gen = 0;
	
	/* update each bug in turn (but don't kill them) */
	for (int k = 0; k < bug_list.size(); k += 1) {

		/* TODO: Move bug k one step foward in its current direction */
		int prev_x = bug_list[k].x;
		int prev_y = bug_list[k].y;
		int prev_dir = bug_list[k].dir;
		bug_list[k].x = newX(prev_x, prev_dir);
		bug_list[k].y = newY(prev_y, prev_dir);
		assert(bug_list[k].dir == prev_dir);

		/* TODO: see if we stepped on food! */
		if (world[bug_list[k].x][bug_list[k].y] == FOOD) {
			bug_list[k].health += EAT_HEALTH;
		}

		/* TODO: update the world: NOTE: we may put this bug on top of another one
		 * but that is OK, as long as the other one is in the bug list */
		//world[newX(bug_list[k].x, (bug_list[k].dir + 4) % 8)][newY(bug_list[k].y, (bug_list[k].dir + 4) % 8)] = EMPTY;
		if (world[prev_x][prev_y] == k) {
			world[prev_x][prev_y] = EMPTY;
		}
		world[bug_list[k].x][bug_list[k].y] = k;

		/* TODO: update this bug's health due to movement cost */
		bug_list[k].health -= MOVE_HEALTH;
		
		/* TODO: pick a new direction for the bug, based on its genes */
		int rand_num = rand() % GENE_TOTAL;
		int gene_index = 0;
		int gene_weight = bug_list[k].genes[gene_index];
		while (rand_num > gene_weight) { // TODO Should this be > or >=
			rand_num -= gene_weight;
			gene_index += 1;
			gene_weight = bug_list[k].genes[gene_index];
		}
		bug_list[k].dir = (bug_list[k].dir + gene_index) % 8;
		if (bug_list[k].dir < 0) {
			printf("!!!! [ERROR] dir: %d, gene_index: %d\n", bug_list[k].dir, gene_index);
		}

		/* DONE: update this bug's age for statistics */
		bug_list[k].age += 1;
		total_age += bug_list[k].age;
		total_gen += bug_list[k].generation;
	}

	/* DONE: some statistics... */
	average_age = total_age / bug_list.size();
	average_generation = total_gen / bug_list.size();
}

/* TODO: write this function 
 * look through the bug_list for dead bugs (bugs with zero 
 * health) and kill them
 *
 * To kill a bug, mark it's position in the world as EMPTY
 * then remove it from the bug_list.
 *
 * To remove a bug from the bug_list, use the following simple
 * trick.  Swap the bug with the last bug in the list, then
 * do "pop_back" on the bug_list vector.
 * Hey, since we're going to do pop_back anyway, we don't really
 * need to swap with the last bug.  We just need to copy the 
 * last bug into our position.
 *
 * WATCH OUT: there's a trap.  Be careful to account for the possibility
 * that the "last bug" is also dead.  Don't miss out on killing it
 * because you moved that bug forward in the bug_list
 *
 * WATCH OUT: There's a really common mistake here. Keep in mind that
 * the world[][] array stores bug numbers -- i.e., indices into the bug_list
 * array. If you move a bug from one position in the bug_list to another
 * then you need to update the index for that bug in the world. Everyone
 * seems to remember to take care of updating the world square for the
 * dead bug, but when they move a bug from the end of the list to the middle
 * they often forget to update the bug number stored in the world[][]
 *
 * Be sure to update the global statistics (total_straight, total_right, total_left and total_back)
 * for each bug that you kill.
 */
void update_stats(Bug b) {
	total_straight -= b.genes[0];
	total_right -= b.genes[1] + b.genes[2] + b.genes[3];
	total_left -= b.genes[5] + b.genes[6] + b.genes[7];
	total_back -= b.genes[4];
}

// Returns total number of bugs killed when function is finished
int kill_bug(int i) {
	// TODO still need to check if last bug in list is dead before switching
	int x = bug_list[i].x;
	int y = bug_list[i].y;
	world[x][y] = EMPTY;

	// i is the last index of the list
	if (i == bug_list.size() - 1) {
		update_stats(bug_list[i]);
		bug_list.pop_back();
		return 1;
	}

	// switch bugs
	update_stats(bug_list[i]);
	Bug tmp = bug_list[bug_list.size()-1];
	bug_list[i] = tmp;
	world[tmp.x][tmp.y] = i;

	// kill bug
	bug_list.pop_back();

	// Check to see if switched bug is dead
	if (tmp.health <= 0) {
		return kill_bug(i) + 1;
	}
	else {
		return 1;
	}
}

void killDeadBugs(void) {
	int i = 0;
	while (i < bug_list.size()) {
		if (bug_list[i].health <= 0) {
			i -= kill_bug(i); // makes sure no bugs are skipped, might be overly defensive
			if (i < 0) {
				i = -1;
			}
		}
		i++;
	}
}

/* TODO: Finish this function 
 * Look at all the bugs in the bug_list.  For each bug
 * that has enough health to reproduce, reproduce a new bug.
 * Put the new bug in the same square and have it head in the
 * same direction.
 *
 * Give both the new bug and the old bug 1/2 the energy of the
 * old bug.
 *
 * Mutate the genes of the new bug as follows.  Choose two genes
 * at random.  Increment one of those genes (by one) and decrement
 * the other gene (by one).
 * YOU MUST ensure 
 * 1. that no genes ever get smaller than zero
 * 2. the total of the eight genes is always GENE_TOTAL
 *
 * Be sure to put the new bug in the bug_list (no need to update
 * the world, since you have two bugs in the same square anyway).
 */
void reproduceBugs(void) {
	int num_bugs = bug_list.size();
	for (int k = 0; k < num_bugs; k += 1) {
		if (bug_list[k].health > REPRODUCE_HEALTH) {
			/* TODO: make "b" a copy of ourself, but with a generation count 1 greater than our own */
			Bug b = bug_list[k]; // set b equal to something, maybe b = bug_list[k]. Will that work?
			b.age = 0;
			b.generation += 1;

			/* TODO: reduce our health by half and assign b half our health */
			bug_list[k].health /= 2;
			b.health /= 2;

			/* TODO: mutate Bug b's genes based on the instructions in the handout */
			int rand_n;
			for (rand_n = rand() % 8; b.genes[rand_n] < 1; rand_n = rand() % 8);
			b.genes[rand_n] -= 1;
			rand_n = rand() % 8;
			b.genes[rand_n] += 1;
			
			/* DONE: update global statistics based on the new genes in b */
			total_straight += b.genes[0];
			total_right += b.genes[1] + b.genes[2] + b.genes[3];
			total_left += b.genes[5] + b.genes[6] + b.genes[7];
			total_back += b.genes[4];

			/* DONE: add b to the global bug list */
			bug_list.push_back(b);
		}
	}	
}

void timeStep() {
	time_step += 1;
	addFood();
	moveBugs();
	killDeadBugs();
	reproduceBugs();
	percent_straight = 100 * total_straight / bug_list.size() / GENE_TOTAL;
	percent_left = 100 * total_left / bug_list.size() / GENE_TOTAL;
	percent_right = 100 * total_right / bug_list.size() / GENE_TOTAL;
	percent_back = 100 * total_back / bug_list.size() / GENE_TOTAL;
}	
