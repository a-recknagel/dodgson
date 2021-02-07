#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "Util.h"
#include "DodgsonScorer.h"
	

class Menu {

#define NUM_THREADS 1000

public:
	static enum Heuristic {BASELINE, DFS, UCS, SMARTCACHE, ICRDFS, NEWSCORER};

	/**
	 * @brief	constuctor for the menu
	 *			needs a preference profile at all time in order to complete its 
	 *			actions
	 */
	Menu(Util::preferenceProfile &prefProf);

	/**
	 * @brief	constuctor for the menu, using a predefined directory.
	 */
	Menu(string fileName);

	/**
	 * @brief	for completeness sake
	 */
	Menu();

	/**
	 * @brief	the main menu, from which all other menus are accessible
	 */
	void mainMenu();


private:
	/**
	 * @brief	saves the currently used preference profile to the hard drive,
	 *			at ../Data/PrefProfxxx.txt , where xxx is the next available
	 *			number found not occupying the directory
	 */
	void save();

	/**
	 * @brief	loads a preference profile from the specified file at 
	 *			../Data/fileName.txt
	 */
	void load(string fileName);

	/**
	 * @brief	overload with argument read in from stdin
	 */
	void load();

	/**
	 * @brief	toggles verbose mode
	 */
	void toggleVerbose();

	/**
	 * @brief	lets you define a new preference profile
	 */
	void newPrefProf();
	
	/**
	 * @brief	scrambles the current preference profile randomly
	 */
	void scramble();
	
	/**
	 * @brief	shows the current preference profile
	 */
	void peek();

	/**
	 * @brief	the evaluation menu, used for evaluating and comparing the 
	 *			perforance of different dodgson scorers
	 */
	void evalMenu();
		
	/**
	 * @brief	runs an evaluation of the current preference profile using the
	 *			current heuristic. This function can take A LOT of time,
	 *			depending on the size of the used preference profile and
	 *			heuristcs.
	 *			Displays the computed scores, the original preference profile,
	 *			which heuristics were used, and needed time.
	 */
	void runEval();

	/**
	 * @brief	runs a threaded variant for the evaluation function. If none
	 *			is available, a normal evaluation will be run after showing 
	 *			a warning. 16.6.: Not neccessary, m_threaded takes care
	 */
	//void threadEval();

	/**
	 * @brief	tests all algorithms for their performance. A maximum wait time
	 *			can be specified, and each algorithm will try to solve problems
	 *			up to that size. This function includes unsafe force exits, so
	 *			a restart of the program after calling it might be wise.
	 */
	void testAll();

	/**
	 * @brief	allows access to the options to run 'normal' solving alogoithms
	 */
	void normalSolve();

	/**
	 * @brief	allows access to the options to run threaded solving alogoithms
	 */
	void threadedSolve();

	/**
	 * @brief	repeatedly runs the current setup for evaluation and presents
	 *			the benchmarks as result. 16.6.: Does not really make sense.
	 *			Should always run a randomized benchmarking.
	 */
	//void repeatEval();

	/**
	 * @brief	same as run eval, but randomizes the preference profile between
	 *			each test. This should be used for actual evaluation, but it
	 *			needs to be considered, that a (usually very small) part of the
	 *			computing time is used for randomisation. Runs threaded search 
	 *			if m_threaded is set.
	 */
	void repeatRandEval();

	/**
	 * @brief	runs a random eval with as many repetitions as specified for all
	 *			known scorers. Runs threaded search if m_threaded is set.
	 */
	void allRepeatRandEval();

	/**
	 * @brief	capsules the "evaluate n times with random preference profiles"
	 *			function. Runs threaded search if m_threaded is set.
	 */
	void randEval(int num, int seed);

	/** 
	 * @brief	fill containers with swap counts for all possible best case 
	 *			permutations.
	 * @param	curve offers buckets for all possible permutations of a 
	 *			best-case profile
	 * @param	sum contains the number of swaps made in the current permutation
	 * @param	current contains the depth of the permutation in the profile
	 * @param	n is the dimension of the profile (as n=m, only one is needed)
	 */
	void fillBuckets(vector<int> &curve, int sum, int current, int n, int scD);

	/**
	 * @brief	visualizes the way in which Dodgson-scores are distributed in
	 *			in a best-case symmetrical search space. Profile base is n.
	 *			The solution is printed to the standard output stream.
	 */ 
	void analyzeDistribution();

	/**
	 * @brief	tests the validity of all dodgson scorers by generating a number
	 *			of random preference profiles and running all known scorers on
	 *			them, comparing their solutions.
	 */
	void test();
	
	/**
	 * @brief	runs a threaded variant of the dodgson search algorithm for the
	 *			currently selected scorer. 16.6.: Not neccessary, m_threaded 
	 *			takes care
	 */
	//void threadedScoring();

	/**
	 * @brief	randomized variant. 16.6.: Not neccessary, m_threaded takes care
	 */
	//void threadedScoringBenchmark();

	/**
	 * @brief	prints the condorcet winner for the current preference profile
	 */
	void getCW();

	/**
	 * @brief	uses the specified heuristic for new single evaluations
	 */
	void chooseHeuristics();

	/**
	 * @brief	deletes all member variables
	 */
	void cleanUp();

	bool					 m_continue;	// set false to terminate menu

	Util::preferenceProfile* m_pp;			// a pointer to the used preference
											// profile

	vector<DodgsonScorer*>	 m_scorers;		// container of the used wrappers 
											// for the current preference profile.
											// TODO needs to be taken care of
											// in destructor

	Heuristic				 m_heuristic;	// currently used evaluation 
											// heuristic

	bool					 m_verbose;		// if true, more detailed output is
											// generated
	bool					 m_threaded;	// true, iff threaded variants for
											// the algorithms are to be run
};