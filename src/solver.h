#ifndef WORLD_H
#define WORLD_H

#include "piece.h"
#include "timer.h"

#include <vector>
#include <list>

#include <iostream>
#include <string>

#define NON_FLAG_NODE_NAME '\0'

class CSolver {
public:
	enum store_type {
		sequential, interleaved
	};

	CSolver(int w, int h, int d, store_type storeType = sequential) :
			width(w), height(h), depth(d) {
		sType = storeType;

		nodes = NULL;
		nodeAmount = 0;
		nodeIt = 0;

		spaces = width * height * depth;

		currentSolution = (char*) malloc(w * h * d * sizeof(char));
	}

	~CSolver() {
		for (std::vector<std::list<CPiece*> >::iterator it = pieces.begin(); it != pieces.end(); it++) {
			for (std::list<CPiece*>::iterator it2 = it->begin(); it2 != it->end(); it2++) {
				if (*it2) delete *it2;
			}
		}

		free(currentSolution);
	}

	void addPiece(int w, int h, int d, const char* structure, char alias);
	bool solve(int maxSolutions);

	void printSolutionsLine(int amount = 0) const;
	void printSolutionsHorizontalLevels(int amount = 0) const;
	void printSolutionsVerticalLevels(int perLine, int amount = 0) const;

	std::vector<std::string> solutionsDeepCopy() const {
		return std::vector<std::string>(solutions);
	}

private:
	std::vector<std::list<CPiece*> > pieces; //pieces with all their images
	store_type sType;

	std::string aliases;

	struct position_info {
		short x, y, z;
	};

	//dimensions of the world
	int width;
	int height;
	int depth;

	int maxSolutions;
	char* currentSolution;
	std::vector<std::string> solutions;

	struct header_node;

	//a 1 in the matrix
	struct node {
		node* left;
		node* right;
		node* up;
		node* down;

		header_node* colHeader;
	};

	struct header_node: public node {
		int size;
		short id;

		bool endCol;
		char name;
	};

	void cover(header_node* column) {
		column->right->left = column->left;
		column->left->right = column->right;

		//traverse the column all the way down
		for (node* row = column->down; row != column; row = row->down) {
			//traverse the row to the right and loop to the start
			for (node* col = row->right; col != row; col = col->right) {
				col->up->down = col->down;
				col->down->up = col->up;

				//row->colHeader->size--; //there is one less node in the column
			}
		}
	}

	//reverse of the above
	void uncover(header_node* column) {
		for (node* col = column->up; col != column; col = col->up) {
			for (node* row = col->left; row != col; row = row->left) {
				//row->colHeader->size++;

				row->up->down = row;
				row->down->up = row;
			}
		}

		column->right->left = column;
		column->left->right = column;
	}

	int space(int x, int y, int z) const {
		return (x + y * width) + z * width * height; //consistent with CShape addressing
	}

	position_info location(int cell) const {
		position_info f;

		int slice = cell % height * depth;

		f.x = slice % width;
		f.y = slice / width;
		f.z = cell / (height * depth);

		return f;
	}

	//this is a stack of the chosen row to cover at each level of search(k)
	//all the rows up to the last level can be accessed instantly when it's time to print a solution
	node** partialSolutionRows;

	int spaces;
	int cols;

	header_node* header;
	header_node root;

	int nodeAmount; //amount of nodes, not matrix size
	int nodeIt; //index of last allocated node
	node* nodes; //storage

	node* newNode() {
		assert(nodeIt < nodeAmount);
		return &(nodes[nodeIt++]);
	}

	//where all the magic happens
	void search(int k);

	CTimer timer;
};

#endif /* WORLD_H */
