#pragma once

#include "dodgsonscorer.h"
#include "Util.h"

using namespace std;

class SmartCache : public DodgsonScorer {
public:
	/*****************
	 * constructors  *
	 * calling their *
	 * super-part    *
	 *****************/
	//SmartCache(void);
	//~SmartCache(void);	
	SmartCache(Util::preferenceProfile &prefProf)	: DodgsonScorer(prefProf) {
		m_supportsThreads = true;
	}
	SmartCache(string fileName)						: DodgsonScorer(fileName) {
		m_supportsThreads = true;
	}
	SmartCache(DodgsonScorer &original)				: DodgsonScorer(original) {
		m_supportsThreads = true;
	}

	/**
	 * @brief	Smartly fills a cache with all possible solutions, and then works
	 *			through them. Smart means, that the entries in the cache (which
	 *			efficiently represent the searchspace) are ordered by their 
	 *			associated dodgson score, from low to high. This has as a conse-
	 *			quence, that the frist found solution is at least as good as any
	 *			solution found at a later point in the cache. This enables 'good'-
	 *			case scenarios, which are rather common in this problem.
	 */
	virtual int getSCD(int a);

	/*
	 * @brief	Implementation that supports threading.
	 */
	virtual void threadedSCD(int a);

private:
	/**
	 * @brief	Fills the cache with all possible swapprofiles for the dodsgon Score which
	 *			was entered in the initial call. Check for duplicates is no longer 
	 *			neccessary, as I found a way to create all unique profiles without 
	 *			redundancy.
	 */
	void fill(vector<int> &posTable, vector<int> &profile, int depth, int scD, vector<vector<int> > &cache);

	/**
	 * @brief	Checks the cache for the first winning solution and returns it.
	 *			If there is none, something went wrong. 
	 */
	vector<vector<int> > checkCache(vector<vector<int> > &cache, vector<int> &posTable);
	
	/**
	 * @brief	Snipped which builds a string representation of a vector. Is just
	 *			needed to make debugging a tad faster and easier.
	 */
	//string getString(vector<int> profile);
};