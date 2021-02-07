#pragma once

#include "Util.h"
#include <vector>
#include <string>
#include <thread>
#include <future>
#include <atomic>
#include <mutex>

using namespace std;

class DodgsonScorer {
public:

	/**
	 * @brief	constructor
	 * @param	prefProf: The Preference Profile used by this instance
	 */
	DodgsonScorer(Util::preferenceProfile &prefProf);

	/**
	 * @brief	constructor
	 * @param	The location in the filesystem from where this instance's 
	 *			Preference Profile and associated dodgson scores is to be 
	 *			fetched from
	 */
	DodgsonScorer(string fileName);
	
	/**
	 * @brief	constructor
	 * @param	The original from whih this instance is copied
	 */
	DodgsonScorer(DodgsonScorer &original);

	/**
	 * @brief	getter for the amount of agents
	 */
	int getN() { return m_pp->n; }
	
	/**
	 * @brief	getter for the amount of alternatives
	 */
	int getM() { return m_pp->m; }

	/**
	 * @brief	getter for the counter of checked pp-instances
	 */
	int getCount() { return m_count; }

	/**
	 * @brief	getter for the preference profile
	 */
	Util::preferenceProfile& getPP() { return *m_pp; }

	/**
	 * @brief	force - set a new random preference profile of dimensions
	 *			n x m, use with caution
	 */
	void setRandPP(int n, int m);

	/**
	 * @brief	Setter for the abort state - remember reset, or it wont
	 *			work in the next iteration!
	 */
	void setAbortState(bool newState) { m_abort = newState; }

	/**
	 * @brief	updates all persistent data according to recent changes
	 */
	void update() {	m_scDs = vector<int>(m_pp->m); }

	/**
	 * @brief	getter for the current dodgson scores
	 */
	vector<int>& getScores() { return m_scDs; }

	/**
	 * @brief	computes the dodgson score for the specified alternative for
	 *			the current preference profile
	 */
	virtual int getSCD(int a) = 0;

	/**
	 * @brief	runs a getSCD(int) function which supports a fast single answer.
	 *			Is only to be implemented for scorers who support threading, i.e.
	 *			have m_supportsThreads = true. 
	 */
	virtual void threadedSCD(int a) = 0;

	/**
	 * @brief	returns true iff this class supports threading
	 */
	bool threadable() { return m_supportsThreads; }

	/**
	 * @brief	overload for runnig getSCD for all alternatives
	 */
	void getSCD();

	/**
	 * @brief	generates a thread for every alternative and returns the one
	 *			with the lowest dodgson score as soon as possible. Other scores
	 *			will potentially not be computed. Uses a simple heuristic to 
	 *			determine the order in which the alternatives are run. Calls
	 *			the virtual getSCD(int a) function for evaluation.
	 * @param maxThreads 
	 *			the maximum amount of threads which will run parallel
	 * @return	
	 *			the dodgson scores of the winning alternatives
	 */
	vector<pair<int,int> > getFastSCD(unsigned int maxThreads);

	/**
	 * @brief	writes the current preference profile and associated scores 
	 *			to the specified textfile 
	 */
	void writePP(string fileName);

	/**
	 * @brief	returns the index of the condorcet winner, -1 if there is none
	 */
	int getCW();

	/**
	 * @brief	prints the preference profile and assotiated dodgson scores to
	 *			the standard output stream
	 */
	void print();


protected:
	Util::preferenceProfile *m_pp;	// a pointer to the used preference profile

	vector<int>				m_scDs;	// saves the dodgson scores whenever they
									// were computed. -1 if not.
	int						m_count;	// counts the number of iterations/
										// recursive steps/loops needed
	bool					m_supportsThreads;	// is set to true if the imple-
												// mentation supports threads
	vector<future<void> >	m_futures;	// contains all currently running 
										// threads
	atomic<int>				m_minimum;	// contains the current minimum while
										// running a parallel search
	vector<pair<int,int> > 	m_winningPairs;	// alternative index and associated
											// dodgson score are saved here 
											// during threaded search
	mutex					m_mtx;		// for blocking access while threading
	bool					m_abort;	// allows for a controlled shut-down of
										// all threaded solvers
};