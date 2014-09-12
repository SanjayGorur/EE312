/*
 * Project9.c
 *
 * My Name
 * My TA
 * My Section Time
 *
 *
 */


#include <stdio.h>
#include <assert.h>
#include "MazeParams.h"

#define ACCURACY 0.000000000000001

void printArray(int x[], int n);

/* return the smallest of the elements in array x[]
 * there are n elements in x[] (x[0].. x[n-1])
 */
int minIt(int x[], int n) {
	int min = x[0];
	for (int i = 1; i < n; i++) {
		if (x[i] < min)
			min = x[i];
	}
	return min;
}

/* return the smallest of the elements in array x[]
 * there are n elements in x[] (x[0].. x[n-1])
 * solve the problem recursively and 
 * use an "n-1" type of decomposition
 */
int minRec1(int x[], int n) {
	// Base case
	if (n == 1) return x[0];

	// Recursion
	return x[0] > x[n - 1] ? minRec1(++x, --n) : minRec1(x, --n);
}

// debug function
void printArray(int x[], int n) {
	const char* prefix = "[ ";
	for (int k = 0; k < n; k += 1) {
		printf("%s%d", prefix, x[k]);
		prefix = ", ";
		if (k > 20) { break; }
	}
	printf(" ]\n");
}

/*
 * return the smallest of the elements in array x[]
 * there are n elements in x[] (x[0].. x[n-1])
 * n may be either odd or even
 * solve the problem recursively and 
 * use an "n / 2" type of decomposition
 */

// Note: does not work for empty array
int minRec2(int x[], int n) {
	// Base case
	if (n < 2) return x[0];

	// Recursion
	int n1 = minRec2(x + (n / 2), n / 2);
	int n2 = minRec2(x, n - (n / 2));
	return n1 > n2 ? n2 : n1;
}

double abs(double x) {
	return x < 0 ? -x : x;
}

/*
 * calculate and return the square root of x.
 * The other two parameters are estimates of the square root.
 * low_guess is smaller than the actual square root, and high_guess 
 * is larger than the actual square root.
 * Searching for the square root can be done much like searching
 * for a name in a phone book.
 *
 * Since you can't calculate a square root exactly, for this problem
 * you're required to calculate the answer to 15 decimal digits of
 * accuracy.
 */
double sqrtIt(double x, double low_guess, double high_guess) {
	while (high_guess - low_guess > 0) {
		double mid = (high_guess - low_guess) / 2.0 + low_guess;
		if (mid * mid > x) high_guess = mid - ACCURACY;
		else if (mid * mid < x) low_guess = mid + ACCURACY;
		else return mid;
	}
	return low_guess;
}



/*
 * calculate and return the square root of x.
 * The other two parameters are estimates of the square root.
 * low_guess is smaller than the actual square root, and high_guess 
 * is larger than the actual square root.
 * Searching for the square root can be done much like searching
 * for a name in a phone book.
 *
 * Since you can't calculate a square root exactly, for this problem
 * you're required to calculate the answer to 15 decimal digits of
 * accuracy.
 */
double sqrtRec(double x, double low_guess, double high_guess) {
	double mid = (high_guess - low_guess) / 2.0 + low_guess;

	// Base cases
	if (mid * mid == x) return mid;
	else if (high_guess - low_guess < 0) return low_guess;

	// Recursion
	if (mid * mid > x) return sqrtRec(x, low_guess, mid - ACCURACY);
	else if (mid * mid < x) return sqrtRec(x, mid + ACCURACY, high_guess);
	
	fprintf(stderr, "did not reach a proper end case\n returning a bogus value\n");
	return -1;
}


/*
 * using only recursion, write a string comparison function
 * return -1 if str1 is less than str2
 * return 0 if the two strings are equal
 * return +1 if str1 is greater than str2
 * when comparing strings, use the ASCII value to compare each character
 * (i.e., that means 'A' is less than 'a' since it's ASCII value is smaller)
 * The string str1 is less than str2 if either
 * str1[0] < str2[0]
 * or there exists a k such that str1[k] < str2[k] and 
 *   for all j < k str1[j] == str2[j]
 *   and k is less than the length of str1 and str2
 */
int strCompare(char* str1, char* str2) {
	// Base cases
	if (str1[0] == 0 && str2[0] == 0) return 0;
	//else if (str1[0] == 0 && str2[0] != 0) return -1;
	//else if (str1[0] != 0 && str2[0] == 0) return 1;
	else if (str1[0] < str2[0]) return -1;
	else if (str1[0] > str2[0]) return 1;

	// Recursion
	else if (str1[0] == str2[0]) return strCompare(++str1, ++str2);

	fprintf(stderr, "did not reach a proper end case\n returning a bogus value\n");
	return -1;
}

/*
 * if c is not a letter return -1
 * if c is a letter, return the position of c in the alphabet 
 * where 'A' (and 'a') are position 1 and 'Z' (and 'z') are position 26
 *
 * This code is rather ugly as I'm exploiting some detailed knowledge of the ASCII table
 */
int whatLetter(char c) {
	if (c < 'A') { return -1; }
	if (c > 'z') { return -1; }
	if (c > 'Z' && c < 'a') { return -1; }
	return c & ~32 - 64;
}

/*
 * same as before, only this time 
 * ignore anything that is not a letter
 * ignore the case (upper case, lower case)
 * So, strCompare2("Hello", "hello") should return 0 (they're the same)
 * strCompare2("The plane!", "theater") should return 1 since "theplane" is larger than "theater"
 * once again, you can only use recursion, no loops
 */
int strCompare2(char* str1, char* str2) {
	// Ignore anything that is not a character or a 0
	if (str1[0] != 0 && whatLetter(str1[0]) < 0) return strCompare2(++str1, str2);
	else if (str2[0] != 0 && whatLetter(str2[0]) < 0) return strCompare2(str1, ++str2);

	// Base cases
	if (str1[0] == 0 && str2[0] == 0) return 0;
	//else if (str1[0] == 0 && str2[0] != 0) return -1;
	//else if (str1[0] != 0 && str2[0] == 0) return 1;
	else if (whatLetter(str1[0]) < whatLetter(str2[0])) return -1;
	else if (whatLetter(str1[0]) > whatLetter(str2[0])) return 1;
	
	// Recursion
	else if (whatLetter(str1[0]) == whatLetter(str2[0])) return strCompare2(++str1, ++str2);

	fprintf(stderr, "did not reach a proper end case\n returning a bogus value\n");
	return -1;
}



/*
 * the two dimensional array maze[MATRIX_SIZE][MATRIX_SIZE] contains a maze
 * Each square is (initially) either a 1 or a zero.  Each 1 represents a wall
 * (you cannot walk through walls, so you can't go into any square where the 
 * value is 1).  Each 0 represents an open space.  
 *
 * Write an recursive solution to find your way out of the maze
 * Your starting point is at row and col. (params to this function)
 * Your exit point is somewhere in the last row (which is: MATRIX_SIZE - 1)
 *
 * There is a relatively easy recursive solution to this problem, the trick is
 * to think of the solution in the following terms:
 *   "Does the shortest path to the exit go through the current square 
 *   (indicated by row and col)?"
 * With that hint, the base case should be pretty obvious.  In fact,
 * I'd suggest you consider the possibility that the base case is "Yup, the 
 * shortest path to the exit goes through here, I know 'cause I'm already at 
 * the exit!"
 *
 * If you're not at the base case, then ask the following question about each
 * of your four adjacent cells:
 *     Does the shortest path to the exit go through there?
 * If the answer were yes, then leaving the maze would be easy, just go to that 
 * square.
 * So, the recursive maze solving problem has a boolean (true/false) return
 * value.  It returns true if the shortest path to the exit goes through
 * the square (row, col) and returns false otherwise.  When row and col
 * are the starting point, the answer will always be true (this function
 * always return true to main).  However, some of the recursive calls will
 * return false.
 *
 * There is one tricky part to this decomposition.  You need to make certain 
 * that the problem is getting smaller.  The "bigness" or "smallness" of this 
 * problem is the number of squares that have not yet been tested.  Each time
 * you test an adjacent square (making a recursive call to decide if the
 * shortest path to the exit goes through that square), you'll be reducing 
 * the number of squares that have not yet been tested.  Eventually, you must 
 * have tested all the squares and so you'll have to have found a way to the 
 * exit.
 *
 * The easy way to deal with the tricky part is to leave "bread crumbs" behind.
 * Before you recursively check any (or all) of your neighbors to see if you 
 * can find the exit from there -- drop a bread crumb in your current square.
 * Then, don't ever check to see if you can find the exit using a square
 * with a breadcrumb in it (you've already checked that square, or at least are
 * in the process of checking it).
 *
 * If you're trying to see if you can find an exit from some square, and all 
 * the adjacent squares are either walls, or have bread crumbs in them, then
 * you already know the answer -- "The shortest path to the exit does NOT go 
 * through this square".  Pick up your bread crumb and return false.
 * 
 * You can set the value of the current square to "2" to indicate that a bread
 * crumb has been dropped.  Set the square back to 0 when you want to pick 
 * the bread crumb back up.
 * be sure not to change any of the squares that are 1 (the walls).
 *
 * for partial credit, you can leave all your bread crumbs on the floor.
 * for full credit, you must pick up all the bread crumbs EXCEPT those
 * along the shortest path to the exit.
 */

// Checks current location to see if all adjacent locations are walls or breadcrumbs
// if current loc is boxed in return 0
// if open space adjacent return 1
int checkCurrentLoc(int row, int col) {
	if (maze[row + 1][col] > 0 &&
		maze[row][col + 1] > 0 &&
		maze[row - 1][col] > 0 &&
		maze[row][col - 1] > 0) {
		return 0;
	}
	else {
		return 1;
	}
}

int solveMazeRec(int row, int col) {
	// base cases
	// We are at the end!
	if (row == MATRIX_SIZE - 1 && maze[row][col] == 0) {
		maze[row][col] = 2;
		return 1;
	}
	// this is a bread crumb
	else if (maze[row][col] == 2) return 0;
	// This is a wall
	else if (maze[row][col] == 1) return 0;
	// We are boxed in so this is not correct path
	else if (!checkCurrentLoc(row, col)) return 0;

	// drop a bread crumb
	maze[row][col] = 2;

	// Recursion
	int m0 = solveMazeRec(row + 1, col);
	if (m0 == 1) return 1; // fixed problem where program stopped working even when it found end
	int m1 = solveMazeRec(row, col + 1);
	int m2 = solveMazeRec(row - 1, col);
	int m3 = solveMazeRec(row, col - 1);
	if (m0 == 0 && m1 == 0 && m2 == 0 && m3 == 0) {
		// pickup breadcrumb and return 0
		maze[row][col] = 0;
		return 0;
	}
	else return 1;
}


/**********************
 * adjacentCell and isOK are functions provided to help you write
 * solveMazeIt()
 */

/*
 * OK, we're currently at maze[row][col] and we're considering moving
 * in direction dir.  
 * Set trow and tcol (local variables inside the calling function)
 * to the row and column that we would move to IF we moved in
 * that direction
 *
 * For example, there are two good ways to use this function.
 * 1. to actually move one step in a direction use:
 *       adjacentCell(row, col, dir, &row, &col);
 *    That will set row and col to new values.  The new values will
 *    be one square away from the old values.
 *
 * 2. to set trow and tcol to a square that is adjacent to row and col use:
 *       adjacentCell(row, col, dir, &trow, &tcol);
 *    That will not change row and col, but will change trow and tcol.
 *    This is useful if you aren't sure if you can actually move in this 
 *    direction yet (e.g., maze[trow][tcol] may be a wall!).  So, you set
 *    trow and tcol, and then check to see if it's OK to move there
 */
void adjacentCell(int row, int col, int dir, int* trow, int* tcol) {
	*trow = row;
	*tcol = col;
	switch(dir) {
	case 0: // UP
		*trow = *trow - 1;
		break;
	case 1: // RIGHT
		*tcol = *tcol + 1;
		break;
	case 2: // DOWN
		*trow = *trow + 1;
		break;
	case 3: // LEFT
		*tcol = *tcol - 1;
		break;
	}
}


/* 
 * return false if there is a wall in the square for row and col
 * return true if it's not a wall.
 */
int isOK(int row, int col) {
	return (row > 0 && row < MATRIX_SIZE
		&& col > 0 && col < MATRIX_SIZE
		&& maze[row][col] != 1);
}

/*
 * return the value of the direction that is one turn to the right
 */
int turnRight(int dir) {
	return (dir + 1) % 4;
}

/*
 * return the value of the direction that is one turn to the left
 */
int turnLeft(int dir) {
	return (dir + 3) % 4;
}

/*
 * the two dimensional array maze[MATRIX_SIZE][MATRIX_SIZE] contains a maze
 * Each square is (initially) either a 1 or a zero.  Each 1 represents a wall
 * (you cannot walk through walls, so you can't go into any square where the value
 * is 1).  Each 0 represents an open space.  
 *
 * write an iterative solution to find your way out of the maze
 * Your starting point is at row and col. (params to this function)
 * Your exit point is somewhere in the last row (which is: MATRIX_SIZE - 1)
 * The maze can be solved by following the right hand wall.  In fact, there
 * is exactly one path between any two points in the maze (well, between any two
 * points that are not walls).
 *
 * The algorithm is not too bad, although it is certainly not trivial
 * Initially, you'll be headed DOWN (direction 2)
 * Each iteration do one of the following.  Note that only one of two cases
 * can possibly apply (the properties of the maze guarantee that).
 *    case 1: you can move in the current direction
 *       In this case, you should take one step in the current direction
 *       and then turn right.  
 *    case 2: you cannot move in the current direction
 *       If there's a wall directly in front of you, then to follow the right
 *       hand wall, you'd need to turn left (placing your hand on the wall that
 *       used to be directly in front of you).  So, if you discover this case
 *       then turn left.  Don't move, just turn left.
 * If you were walking down a straight corridor (with walls on both sides)
 * then you'd alternate case 1 and case 2.  On the first iteration, you'd
 * take a step, and then turn right (case 1).  This causes you to walk
 * one position down the hallway but then turn to face the wall.
 * On the next iteration, you'll be facing the wall, so you can't walk
 * forward and therefore (case 2) you'll turn left.  On the third iteration
 * you'll be able to walk down the hallway again.
 *
 * If you follow those two cases, then you'll eventually find your way out
 * of the maze.  To confirm that you're making it out, leave a "bread crumb" 
 * behind each square along the path to the exit.
 * For partial credit, place a bread crumb in every square you visit.
 * For full credit, pick up all the breadcrumbs when you backtrack.  
 * Backtracking is when you go back to a square you've already been before.  
 * If the square you're about to step into has a breadcrumb, then pick up 
 * the bread crumb in the square you're at now.
 * You indicate "bread crumbs" by setting the square equal to "2"
 */
void solveMazeIt(int row, int col) {
	int dir = 2; // 0 is up, 1 is right, 2 is down, 3 is left.
	maze[row][col] = 2; // drop a bread crumb in the starting square
	while (row < MATRIX_SIZE - 1) { // the exit is the only open square 
				// in the last row

		/* the rest of this loop is yours */
		break;
	}
}


/*
 * This struct is used in part5, martian currency
 * A Martian indicates a collection of coins. The components indicate the number of pennies
 * nicks and dodeks in that collection. The value (in cents) of a Martian is the weighted sum
 * i.e., if you had two pennies, three nicks and four dodeks, you'd have 2 + 15 + 48 = 65 cents
 */

struct Martian {
	int pennies;
	int nicks;
	int dodeks;
};

/*
 * using recursion, with no loops or globals, write a function that calculates the optimal
 * (fewest total coins) change for a given amount of money. Return a Martian struct that indicates
 * this optimal collection of coins.
 */
int sum_change(Martian m) {
	int sum = 0;
	sum += m.pennies;
	sum += 5 * m.nicks;
	sum += 12 * m.dodeks;
	return sum;
}

int total_coins(Martian m) {
	int tot = 0;
	tot += m.dodeks;
	tot += m.nicks;
	tot += m.pennies;
	return tot;
}

Martian add_martians(Martian m1, Martian m2) {
	Martian result;
	result.dodeks = m1.dodeks + m2.dodeks;
	result.nicks = m1.nicks + m2.nicks;
	result.pennies = m1.pennies + m2.pennies;
	return result;
}

Martian change(int cents) {
	Martian m = { 0, 0, 0 };
	// base case
	if (cents == 0) { return m; }
	if (cents == 1) {
		m.pennies = 1;
		return m;
	}
	if (cents == 5) {
		m.nicks = 1;
		return m;
	}
	if (cents == 12) {
		m.dodeks = 1;
		return m;
	}
	
	int min_coins = -1;
	Martian min_m;

	if (cents > 12) {
		Martian md = { 0, 0, 1 };
		md = add_martians(md, change(cents - 12));
		int md_coins = total_coins(md);
		min_coins = md_coins;
		min_m = md;
	}
	if (cents > 5) {
		Martian mn = { 0, 1, 0 };
		mn = add_martians(mn, change(cents - 5));
		int mn_coins = total_coins(mn);
		if (min_coins != -1) {
			if (mn_coins < min_coins) {
				min_coins = mn_coins;
				min_m = mn;
			}
		}
		else {
			min_coins = mn_coins;
			min_m = mn;
		}
	}
	Martian mp = { 1, 0, 0 };
	mp = add_martians(mp, change(cents - 1));
	int mp_coins = total_coins(mp);
	if (min_coins != -1) {
		if (mp_coins < min_coins) {
			min_coins = mp_coins;
			min_m = mp;
		}
	}
	else {
		min_coins = mp_coins;
		min_m = mp;
	}

	return min_m;
}

/* 
 * without using recursion, solve the Martian change problem described above.
 */
Martian changeIt(int cents) {
	Martian m = { 0, 0, 0 };
	return m;
}

