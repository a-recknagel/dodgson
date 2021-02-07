#pragma once

#include "dodgsonscorer.h"
#include "Util.h"

using namespace std;

class IterativeDFS : public DodgsonScorer {
public:
	/*****************
	 * constructors  *
	 * calling their *
	 * super-part    *
	 *****************/
	//IterativeDFS(void);
	//~IterativeDFS(void);	
	IterativeDFS(Util::preferenceProfile &prefProf)		: DodgsonScorer(prefProf) {
		m_supportsThreads = true;
	}
	IterativeDFS(string fileName)						: DodgsonScorer(fileName) {
		m_supportsThreads = true;
	}
	IterativeDFS(DodgsonScorer &original)				: DodgsonScorer(original) {
		m_supportsThreads = true;
	}

	/**
	 * @brief	implementing a scorer running an algorithm similar to an iterative
	 *			deepening depth first search. There are some substantial differences,
	 *			but on the surface they look and behave very similar.
	 */
	virtual int getSCD(int a);

	/*
	 * @brief	Implementation that supports threading.
	 */
	virtual void threadedSCD(int a);

private:
	/**
	 * @brief	Runs the iterative cost-raise depth first search. 
	 *			The initial call starts with the posTable of the alternative which
	 *			is to be examined, an empty container for the profile of size 'n',
	 *			a depth of 0 and a a variable scD, which is intended to start at 0
	 *			and then incremented with each run, until the theoretical max-score
	 *			of (n/2+1)*m is reached.
	 */
	bool runICRDFS(vector<int> &posTable, vector<int> &profile, int depth, int scD);

	/**
	 * @brief	Transforms a swap profile into a preference profile. The latter is
	 *			then sent to the Util::isCW function. The posTable is neccessary for
	 *			for the transformation, and the current alternative can be derived
	 *			from it.
	 */
	bool checkSwapProf(vector<int> &swapProf, vector<int> &posTable);
	
	/**
	 * @brief	Snipped which builds a string representation of a vector. Is just
	 *			needed to make debugging a tad faster and easier.
	 */
	//string getString(vector<int> profile);
};