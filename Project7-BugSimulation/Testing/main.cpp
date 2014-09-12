/*
 * Project 7 (Bugs) Student Testing Project
 *
 * Duncan Horn
 */

#include "gtest/gtest.h"

// Windows.h has the definition for ARRAYSIZE
#define NO_MIN_MAX
#include <Windows.h>

#include "../Bugs.h"

enum Direction {
	NORTH = 0,      // wrap(x-1), y
	NORTHEAST = 1,  // wrap(x-1), wrap(y+1)
	EAST = 2,       // x,         wrap(y+1)
	SOUTHEAST = 3,  // wrap(x+1), wrap(y+1)
	SOUTH = 4,      // wrap(x+1), y
	SOUTHWEST = 5,  // wrap(x+1), wrap(y-1)
	WEST = 6,       // x,         wrap(y-1)
	NORTHWEST = 7,  // wrap(x-1), wrap(y-1)
};

int world[WORLD_SIZE][WORLD_SIZE];
std::vector<Bug> bug_list;

void moveBugs(void);
void killDeadBugs(void);
void reproduceBugs(void);

int newX(int x, int dir);
int newY(int x, int dir);



static Bug initBug(
    int x = 0, int y = 0,
    int dir = 0,
    int health = START_HEALTH,
    int generation = 0,
    int age = 0)
{
    Bug b = { 0 };
    b.x = x;
    b.y = y;
    b.dir = dir;
    b.health = health;
    b.generation = generation;
    b.age = age;
    memset(b.genes, GENE_TOTAL / ARRAYSIZE(b.genes), ARRAYSIZE(b.genes));

    return b;
}

static void initWorld(void)
{
    // Make the world empty
    memset(world, EMPTY, WORLD_SIZE * WORLD_SIZE);

    int i = 0;
    for (auto &bug : bug_list)
    {
        world[bug.x][bug.y] = i++;
    }
}



/* Makes sure bugs move properly in all directions */
TEST(MovementTests, SimpleMovementTest)
{
    bug_list.clear();

    for (int i = 0; i < 8; i++)
    {
        Bug b = initBug(0, 0, i);
        bug_list.push_back(b);
    }
    initWorld();

    moveBugs();
    ASSERT_EQ(bug_list.size(), 8);
    ASSERT_EQ(world[0][0], EMPTY);

    int i = 0;
    for (auto &bug : bug_list)
    {
        ASSERT_EQ(bug.x, newX(0, i));
        ASSERT_EQ(bug.y, newY(0, i));
        ASSERT_EQ(world[bug.x][bug.y], i++);
    }
}

/* Makes sure bugs eat the food they move on top of */
TEST(MovementTests, EatingTest)
{
    bug_list.clear();
    bug_list.push_back(initBug(0, 0, 2));
    initWorld();

    world[0][1] = FOOD;
    moveBugs();

    ASSERT_EQ(world[0][1], 0);
    ASSERT_EQ(bug_list.size(), 1);
    ASSERT_EQ(bug_list[0].health, START_HEALTH + EAT_HEALTH - MOVE_HEALTH);
}

/* Makes sure bugs update their direction properly for all directions */
TEST(MovementTests, SimpleDirectionTest)
{
    bug_list.clear();

    for (int i = 0; i < 8; i++)
    {
        Bug b = initBug(0, 0, 0);
        memset(b.genes, 0, ARRAYSIZE(b.genes));
        b.genes[i] = GENE_TOTAL;

        bug_list.push_back(b);
    }
    initWorld();

    moveBugs();
    ASSERT_EQ(bug_list.size(), 8);

    // Each bug's direction should now be i
    int i = 0;
    for (auto &bug : bug_list)
    {
        ASSERT_EQ(bug.dir, i++);
    }
}



TEST(BugKillingTests, SimpleBugKillingTest)
{
    bug_list.clear();
    bug_list.push_back(initBug(0, 0, 2, MOVE_HEALTH));
    bug_list.push_back(initBug(0, 0, 4, MOVE_HEALTH * 2));
    initWorld();

    moveBugs();
    killDeadBugs();

    ASSERT_EQ(bug_list.size(), 1);
    ASSERT_EQ(world[0][0], EMPTY);
    ASSERT_EQ(world[0][1], EMPTY);
    ASSERT_EQ(world[1][0], 0);
}

TEST(BugKillingTests, NoSkipBugKillingTest) {
	bug_list.clear();
	bug_list.push_back(initBug(0, 0, NORTH, MOVE_HEALTH));
	bug_list.push_back(initBug(0, 0, SOUTH, MOVE_HEALTH));
	bug_list.push_back(initBug(0, 0, WEST, MOVE_HEALTH * 2));
	initWorld();

	moveBugs();
	killDeadBugs();

	ASSERT_EQ(bug_list.size(), 1);
	ASSERT_EQ(world[0][0], EMPTY);
	ASSERT_EQ(world[WORLD_SIZE-1][0], EMPTY);
	ASSERT_EQ(world[1][0], EMPTY);
	ASSERT_EQ(world[0][WORLD_SIZE-1], 0);
}

TEST(BugKillingTests, RemoveLastBugIfDead) {
	bug_list.clear();
	bug_list.push_back(initBug(0, 0, EAST, 0));
	bug_list.push_back(initBug(0, 1, EAST, MOVE_HEALTH * 3));
	bug_list.push_back(initBug(0, 2, EAST, MOVE_HEALTH * 3));
	bug_list.push_back(initBug(0, 3, EAST, 0));
	initWorld();

	moveBugs();
	killDeadBugs();

	ASSERT_EQ(bug_list.size(), 2);
	ASSERT_EQ(world[0][1], EMPTY);
	ASSERT_GE(world[0][2], 0);
	ASSERT_GE(world[0][3], 0);
	ASSERT_EQ(world[0][4], EMPTY);
}



TEST(ProcreationTests, SimpleProcreationTest)
{
    bug_list.clear();

    for (int i = 0; i < 10; i++)
    {
        // The provided code has "if (...health > REPRODUCE_HEALTH) instead of >=
        bug_list.push_back(initBug(i, i, rand() % 8, REPRODUCE_HEALTH + 2, i, i + 10));
    }
    initWorld();

    reproduceBugs();

    ASSERT_EQ(bug_list.size(), 20);
    for (int i = 10; i < bug_list.size(); i++)
    {
        auto &bug = bug_list[i];
        ASSERT_EQ(bug.x, i - 10);
        ASSERT_EQ(bug.y, i - 10);
        ASSERT_EQ(bug.dir, bug_list[i - 10].dir);
        ASSERT_EQ(bug.health, REPRODUCE_HEALTH / 2 + 1);
        ASSERT_EQ(bug.generation, i - 9);
        ASSERT_EQ(bug.age, 0);
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
}
