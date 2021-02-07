#pragma once

#include "DodgsonScorer.h"
#include <string>
#include "Util.h"

using namespace std;

class Baseline : public DodgsonScorer {
public:
	/*****************
	 * constructors  *
	 * calling their *
	 * super-part    *
	 *****************/
	//Baseline(void);
	//~Baseline(void);	
	Baseline(Util::preferenceProfile &prefProf)	: DodgsonScorer(prefProf) {}
	Baseline(string fileName)					: DodgsonScorer(fileName) {}
	Baseline(DodgsonScorer &original)			: DodgsonScorer(original) {}

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
	 * @brief	creates the cache filled with all realisticly needed
	 *			permutations for the evaluation of alternative 'a'
	 *			The recursive call is made for each row, cyclying through
	 *			agents profiles with 'depth' as index
	 */
	void createCache(int a, int depth, vector<vector<int> > current, int scD, vector<pair<vector<vector<int> >, int> >  &cache); 
};