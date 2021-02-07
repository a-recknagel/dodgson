#pragma once

#include "dodgsonscorer.h"
#include "Util.h"

using namespace std;

class DepthFirstSearch : public DodgsonScorer {
public:
	/*****************
	 * constructors  *
	 * calling their *
	 * super-part    *
	 *****************/
	//DepthFirstSearch(void);
	//~DepthFirstSearch(void);	
	DepthFirstSearch(Util::preferenceProfile &prefProf)	: DodgsonScorer(prefProf) {}
	DepthFirstSearch(string fileName)					: DodgsonScorer(fileName) {}
	DepthFirstSearch(DodgsonScorer &original)			: DodgsonScorer(original) {}

	/**
	 * @brief	implementing a dodgson score baseline		
	 */
	virtual int getSCD(int a);

	/*
	 * @brief	Implementation that supports threading. If threading cannot work
	 *			in this scorer, it remains empty.
	 */
	virtual void threadedSCD(int a);

private:
	/**
	 * @brief	performs a depth first search on a profile, treating each possible 
	 *			outcome as a node and cycling through them in classic dfs-fashion
	 *
	 *			minProfile is not neccessary for computing the dodgson score, it  
	 *			can be removed after debugging
	 */
	void search(int a, int depth, int scD, vector<vector<int> > profile, int &minSCD, vector<vector<int> > &minProfile);
};

