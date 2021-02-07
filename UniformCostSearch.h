#pragma once

#include "dodgsonscorer.h"
#include "Util.h"

using namespace std;

class UniformCostSearch : public DodgsonScorer {
public:

	/**
	 * @brief	the structure used to represent a node in the traversing algorithm
	 * @member	depth is the depth of this node relative to the root, with root 
	 *			having depth = 0
	 */
	struct Node {
		vector<int> swaps;
		int scD;
		
		Node(Node &father, int newSwap) { 
			swaps = vector<int>(father.swaps);
			swaps.push_back(newSwap);
			scD = father.scD + newSwap;
		}

		Node(Node* clone) { 
			swaps = vector<int>(clone->swaps);
			scD = clone->scD;
		}
		
		Node() { scD = 0; }
		
		/**
		 * @brief	simple arithmetic
		 */
		bool operator < (Node &other) {
			return scD < other.scD;
		}
		bool operator > (Node &other) {
			return scD > other.scD;
		}
		bool operator <= (Node &other) {
			return scD <= other.scD;
		}
		bool operator >= (Node &other) {
			return scD >= other.scD;
		}
		bool operator == (Node &other) {
			return scD == other.scD;
		}
	};


	/*****************
	 * constructors  *
	 * calling their *
	 * super-part    *
	 *****************/
	//UniformCostSearch(void);
	//~UniformCostSearch(void);	
	UniformCostSearch(Util::preferenceProfile &prefProf)	: DodgsonScorer(prefProf) {
		m_supportsThreads = true;
	}
	UniformCostSearch(string fileName)						: DodgsonScorer(fileName) {
		m_supportsThreads = true;
	}
	UniformCostSearch(DodgsonScorer &original)				: DodgsonScorer(original) {
		m_supportsThreads = true;
	}

	/**
	 * @brief	implementing a uniform cost search		
	 */
	virtual int getSCD(int a);

	/*
	 * @brief	Implementation that supports threading.
	 */
	virtual void threadedSCD(int a);

private:
	/**
	 * @brief	constructs a preference profile (actually, just the 2D data array,
	 *			not the full object) and runs Util::isCW on it
	 */
	bool isCW(vector<int> &swapProfile, vector<int> &posTable);
};

