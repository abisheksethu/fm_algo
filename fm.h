#ifndef _FM_H_

#define _FM_H_

#include <iostream>
#include <vector>
#include <list>
#include <stdio.h>
#include <string.h>

using namespace std;

#define NODES             (8)
#define MAX_LENGTH_LINE   (20)
#define PMAX			(NODES-1)
#define SUCCESSFUL		(1)
#define UNSUCCESSFUL	(0)
#define FAILURE			(-1)
#define DEBUG			(1)

struct nodes {
	char label;
	int gain;
	unsigned int area;
	int partition;
	unsigned int fs;
  	unsigned int te;
  	unsigned int temp_lock;
};

struct cumulativeGain {
	char fixed_cell;
	int gain;
	double ratio_factor;
	int cumulative_gain;
};

/*******Forward Declaration******/
int b_criterion(int, int);
void printBucket(std::vector<std::list<nodes> >);
char bestGainCal(std::vector<std::list<nodes> > *);
int initNodes(void);
void updateGain(void);
void updatePartitionArea(void);
int updateNodes(void);
int fixCellUpdation(char);
double ratioFactorCalculation(char fixed_cell);
/*******Forward Declaration******/


/*****Global Values*******/
double ratio = 0;
unsigned int partition1_area = 0;
unsigned int partition_max_area = 0;
unsigned int partition2_area = 0;
unsigned int total_area = 0;
struct nodes cell[NODES];
struct cumulativeGain gain_list[NODES];
unsigned int vertices = 0;
/*****Global Values*******/

#endif /* */