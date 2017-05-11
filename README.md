# fm_algo

## General Information  ##

--> hypergraph.txt file should be in project folder

--> hypergraph.txt parsing content details
	* The parsing file should contains only two columns
	* The order of representing contents should be followed
		1. ratio factor
		2. Number of nodes/cells (do not use x and y)
		3. Area of the nodes/cells in alphabetical order
		4. Partition 2/B details - group of possible nodes in B
		5. Type 1 Edges - Any Edge connecting two nodes of the graph
		6. Type 2 Edges - Any Edge with steiner points

--> Compiling and execution are performed using the script file
	RUN: ./run.sh

## Implementation Logic: ##
	* the computation of initial gain values at the beginning of a pass; 
	* the retrieval of the best-gain (feasible) move; 
	* the update of all affected gain values after a move is made
	*	Calculation of cumulative gain with ratio factor for pass 1
	* Minimum Cutcost Calculation - Pending

## Reference: ##
	Hypergraph Partitioning by David A. Papa and Igor L. Markov
	University of Michigan, EECS Department, Ann Arbor, MI 48109-2122