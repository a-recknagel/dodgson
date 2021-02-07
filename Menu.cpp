#include "Menu.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>
#include <future>
#include <algorithm>
#include "Baseline.h"
#include "DepthFirstSearch.h"
#include "UniformCostSearch.h"
#include "SmartCache.h"
#include "IterativeDFS.h"
#include "Logger.h"

using namespace std;

long g_countBefore;
long g_countAfter;

Menu::Menu(Util::preferenceProfile &prefProf) {
	m_continue = true;
	m_heuristic = BASELINE;
	m_pp = &prefProf;
	m_verbose = true;
	m_threaded = false;
	m_scorers = vector<DodgsonScorer*>();
	m_scorers.push_back(new Baseline(*m_pp));
	m_scorers.push_back(new DepthFirstSearch(*m_pp));
	m_scorers.push_back(new UniformCostSearch(*m_pp));
	m_scorers.push_back(new SmartCache(*m_pp));
	m_scorers.push_back(new IterativeDFS(*m_pp));
	/* Add a new scorer here */
	//m_scorers.push_back(new SCORER(*m_pp));
}

Menu::Menu() {
	m_continue = false;
	m_heuristic = BASELINE;
	m_pp = nullptr;
	m_verbose = true;
	m_threaded = false;
	m_scorers = vector<DodgsonScorer*>();
	cout << "Menu needs a preference profile!" << endl;
}

Menu::Menu(string fileName) {
	m_continue = false;
	m_heuristic = BASELINE;
	m_scorers = vector<DodgsonScorer*>();
	load(fileName);
	m_pp = &(m_scorers[0]->getPP());
	m_verbose = true;
	m_threaded = false;
	m_continue = true;
}

void Menu::mainMenu() {
	while(m_continue) {
		cout << "Main Menu" << endl;
		cout << "Choose options:" << endl
			<< "[1]: Save profile" << endl
			<< "[2]: Load profile" << endl
			<< "[3]: Scramble profile" << endl
			<< "[4]: Create new profile" << endl
			<< "[5]: Peek at profile" << endl
			<< "[6]: Toggle verbose mode" << endl
			<< "[7]: Benchmark with max duration" << endl
			<< "[8]: Run evaluation menu" << endl
			<< "[9]: Close program" << endl << endl;

		string input;
		getline(cin, input);
		switch(input[0]) {			
		case '1': save(); mainMenu(); break;
		case '2': load(); mainMenu(); break;
		case '3': scramble(); mainMenu(); break;
		case '4': newPrefProf(); mainMenu(); break;
		case '5': peek(); mainMenu(); break;
		case '6': toggleVerbose(); mainMenu(); break;
		case '7': testAll(); mainMenu(); break;
		case '8': evalMenu(); break;
		case '9': m_continue = false; break;
		default: 
			cout << "Character not recognized." << endl;
			mainMenu(); 
		}
		cin.clear();
	}
}

void Menu::toggleVerbose() {
	m_verbose = !m_verbose;
	cout << "Verbose is now turned " << (m_verbose ? "on" : "off") << endl;
}
	
void Menu::save() {
	int nextIdx;
	ifstream iFile;
	for(nextIdx=0; nextIdx<100; nextIdx++) {
		iFile.open("Data/prefProf_"+to_string(static_cast<long long>(nextIdx))+".txt");
		if(!iFile) break;
	}
	if(nextIdx < 100) {
		for(unsigned int i=0; i<m_scorers.size(); i++) {
			string fileName = "Data/prefProf_"+to_string(static_cast<long long>(nextIdx+i))+".txt";
			m_scorers[i]->writePP(fileName);
			cout << "Successfully saved file as " << fileName << endl;
		}
	}else{
		cout << "Error: Total number of saved profiles is greater than 100."
			 << " You might want to consider deleting some." << endl;
	}
}
	
void Menu::load(string fileName) {
	ifstream iFile(fileName);
	if(iFile) {
		cleanUp();
		m_scorers[0] = new Baseline(fileName);
		m_scorers[1] = new DepthFirstSearch(fileName);
		m_scorers[2] = new UniformCostSearch(fileName);
		m_scorers[3] = new SmartCache(fileName);
		m_scorers[4] = new IterativeDFS(fileName);
		/* Insert option to load new scorer here */
		//m_scorers[5] = new NewScorer(fileName);
		m_pp = &(m_scorers[0]->getPP());
		cout << "Successfully loaded profile from " << fileName << endl;
	}else{
		cout << "Error: There is no file called: " << fileName << endl;
	}
}	

void Menu::load() {
	cout << "Please enter the number of the file you wish to load: ";
	string fileName;
	getline(cin, fileName);
	cout << endl;
	fileName = "Data/prefProf_" + fileName + ".txt";
	load(fileName);
}
	
void Menu::newPrefProf() {
	cleanUp();
	int n, m;
	cout << "Specify the amount of agents in the new preference profile: ";
	string input;
	getline(cin, input);
	n = stoi(input);	
	cout << "\nSpecify the amount of alternatives in the new preference profile: ";
	getline(cin, input);
	m = stoi(input);
	m_pp = new Util::preferenceProfile(n, m);
	cout << "\nSuccessfully build a new preference profile with with dimensions " << n << "x" << m << endl;
	Util::scramble(*m_pp);
	m_continue = true;
	m_heuristic = BASELINE;
	m_scorers = vector<DodgsonScorer*>();
	m_scorers.push_back(new Baseline(*m_pp));
	m_scorers.push_back(new DepthFirstSearch(*m_pp));
	m_scorers.push_back(new UniformCostSearch(*m_pp));
	m_scorers.push_back(new SmartCache(*m_pp));
	m_scorers.push_back(new IterativeDFS(*m_pp));
	/* Insert new scorer for flexible generation here */
	//m_scorers.push_back(new NewScorer(*m_pp));
}

void Menu::scramble() {
	Util::scramble(*m_pp);
	cout << "Succsefully scrambled!" << endl;
}

void Menu::peek() {
	Util::printPP(*m_pp);
}

void Menu::evalMenu() {
	cout << "Entering evaluation mode." << endl;
	cout << "Choose options:" << endl
		 << "[1]: Chose heuristic" << endl
		 << "[2]: Enter menu for normal solving algorithms" << endl
		 << "[3]: Enter menu for threaded solving algorithms" << endl
		 << "[4]: Test all solvers" << endl
		 << "[5]: Examine distribution" << endl
		 << "[6]: Return to main menu" << endl << endl;
	
	string input;
	getline(cin, input);
	switch(input[0]) {	
	case '1': chooseHeuristics(); evalMenu(); break;
	case '2': normalSolve(); break;
	case '3': threadedSolve(); break;
	case '4': test(); evalMenu(); break;
	case '5': analyzeDistribution(); evalMenu(); break;
	case '6': mainMenu(); break;
	default: 
		cout << "Character not recognized." << endl;
		evalMenu();
	}
}
	
void Menu::normalSolve(){
	m_threaded = false;
	cout << "Entering normal solve mode." << endl;
	cout << "Choose options:" << endl
		 << "[1]: Show condorcet winner" << endl
		 << "[2]: Run eval for current profile" << endl
		 << "[3]: Randomized benchmarking" << endl
		 << "[4]: Randomized benchmarking for all scorers" << endl
		 << "[5]: Return to eval menu" << endl << endl;
	
	string input;
	getline(cin, input);
	switch(input[0]) {	
	case '1': getCW(); normalSolve(); break;
	case '2': runEval(); normalSolve(); break;
	case '3': repeatRandEval(); normalSolve(); break;
	case '4': allRepeatRandEval(); normalSolve(); break;
	case '5': evalMenu(); break;
	default: 
		cout << "Character not recognized." << endl;
		normalSolve();
	}
}	

void Menu::threadedSolve(){
	m_threaded = true;
	cout << "Entering threaded solve mode." << endl;
	cout << "Choose options:" << endl
		 << "[1]: Run eval for current profile" << endl
		 << "[2]: Randomized benchmarking" << endl
		 << "[3]: Randomized benchmarking for all scorers" << endl
		 << "[4]: Return to eval menu" << endl << endl;
	
	string input;
	getline(cin, input);
	switch(input[0]) {	
	case '1': runEval(); threadedSolve(); break;
	case '2': repeatRandEval(); threadedSolve(); break;
	case '3': allRepeatRandEval(); threadedSolve(); break;
	case '4': evalMenu(); break;
	default: 
		cout << "Character not recognized." << endl;
		threadedSolve();
	}

}

void Menu::runEval() {
	cout << "Computing scores for scorer number " << m_heuristic+1 << " ..." << endl;

	clock_t t = clock();
	vector<pair<int, int> > solution;
	if(m_threaded) 
		solution = m_scorers[m_heuristic]->getFastSCD(NUM_THREADS);	
	else
		m_scorers[m_heuristic]->getSCD();

	t = clock() - t;
	cout << "\nTotal duration: " << t << " clock-ticks, or " << ((float)t)/CLOCKS_PER_SEC << " seconds" << endl;
	cout << "Results:" << endl;
	if(m_threaded) {
		cout << "Winner(s):\n";
		for(pair<int, int> p : solution) {
			cout << "Alternative " << p.first << " with a dodgson score of " << p.second << ".\n";
		}
	} else
		m_scorers[m_heuristic]->print();

	cout << endl << "------------------------------------------------" << endl << endl;	
}

void Menu::repeatRandEval() {
	string input;
	cout << "Amount of runs: ";
	getline(cin, input);
	cout << "Agents: " << m_pp->n << ", Alternatives: " << m_pp->m << endl;
	int numRuns = stoi(input);
	int seed = (int)time(0);
	
	randEval(numRuns, seed);
}

void Menu::allRepeatRandEval() {
	Heuristic initialState = m_heuristic;
	string input;
	cout << "Amount of runs: ";
	getline(cin, input);
	cout << "Agents: " << m_pp->n << ", Alternatives: " << m_pp->m << endl;
	int numRuns = stoi(input);
	int seed = (int)time(0);

	for(unsigned int i=0; i<m_scorers.size(); i++) {
		m_heuristic = Heuristic(i);
		randEval(numRuns, seed);
	}
	m_heuristic = initialState;
}

void Menu::randEval(int num, int seed) {
	int numRuns = num;
	vector<clock_t> benchmark;
	int min, max, median;
	float mean = 0, var = 0, count = 0, sDeviat;

	if (m_verbose) cout << "\nResult in clock-ticks:\n+---------------" << endl;
	for(int i=0; i<numRuns; i++) {
		clock_t t = clock();
		if(m_threaded) 
			m_scorers[m_heuristic]->getFastSCD(NUM_THREADS);
		else 
			m_scorers[m_heuristic]->getSCD();	

		benchmark.push_back(clock() - t);
		
		Util::scramble(*m_pp, seed++);
		mean += benchmark[i];
		count += m_scorers[m_heuristic]->getCount();
		if(m_verbose) cout << "|  " << benchmark[i] << endl;
	}
	
	sort(benchmark.begin(), benchmark.end());
	min = benchmark.front();
	max = benchmark.back();
	median = benchmark[benchmark.size()/2];
	mean /= numRuns;
	count /= numRuns;
	for(unsigned int i=0; i<benchmark.size(); i++) {
		var += ((((float)benchmark[i]) - mean) * (((float)benchmark[i]) - mean));
	}
	var /= numRuns;
	sDeviat = sqrt(var);
	cout << "+---------------\nMean: " << mean << ", Variance: " << (int)var << ", Standard deviation: " << (int)sDeviat << endl;
	cout << "Median: " << median << ", Min: " << min << ", Max: " << max << endl;
	cout << "Average number of instances checked: " << count << endl;
	cout << "Clocks per second on this system: " << CLOCKS_PER_SEC << endl << endl;
}

void Menu::test() {
	string input;
	cout << "Amount of runs: ";
	getline(cin, input);
	int numRuns = stoi(input);
	int seed = (int)time(0), errors = 0;
	vector<Util::preferenceProfile> profiles;

	m_threaded = true;
	cout << "Now running threaded algorithms . . .\n"; 
	for(int i=0; i<numRuns; i++) {
		if(m_verbose) cout << "\nRun number " << i+1 << "/" << numRuns << "\n****************************************\n";
		vector<pair<int, int> > currentScores, lastScores;
		int lastSeed = 0;
		Util::scramble(*m_pp, seed++);
		for(unsigned int j=0; j<m_scorers.size(); j++) {
			if(m_verbose) cout << "Running scorer " << j+1 << endl;
			currentScores = m_scorers[j]->getFastSCD(NUM_THREADS);
			for(unsigned int k=0; k<lastScores.size(); k++) {
				if((lastScores[k].first != currentScores[k].first) || (lastScores[k].second != currentScores[k].second)) {
					errors++;
					if(m_verbose) cout << "\nDiscrepancy for scorers " << j << " and " << j+1 << endl;
					if(seed != lastSeed && (seed+1) != lastSeed) {
						lastSeed = seed;
						Util::preferenceProfile clone;
						Util::clonePP(*m_pp, clone);
						profiles.push_back(clone);
					}
				}
			}
			if(m_verbose) { 
				cout << "Finished with winner(s):" << endl;
				for(pair<int, int> p : currentScores) {
					cout << "Alternative " << p.first << " with a dodgson score of " << p.second << ".\n";
				}
			}
			lastScores = vector<pair<int, int> >(currentScores);
		}
	}

	m_threaded = false;
	cout << "Now running normal algorithms . . .\n"; 
	for(int i=0; i<numRuns; i++) {
		if(m_verbose) cout << "\nRun number " << i+1 << "/" << numRuns << "\n****************************************\n";
		vector<int> currentScores, lastScores;
		int lastSeed = 0;
		Util::scramble(*m_pp, seed++);
		for(unsigned int j=0; j<m_scorers.size(); j++) {
			if(m_verbose) cout << "Running scorer " << j+1 << endl;
			m_scorers[j]->getSCD();
			currentScores = vector<int>(m_scorers[j]->getScores());
			for(unsigned int k=0; k<lastScores.size(); k++) {
				if(currentScores[k] != lastScores[k]) {
					errors++;
					if(m_verbose) cout << "\nDiscrepancy for scorers " << j << " and " << j+1 << endl;
					if(seed != lastSeed && (seed+1) != lastSeed) {
						lastSeed = seed;
						Util::preferenceProfile clone;
						Util::clonePP(*m_pp, clone);
						profiles.push_back(clone);
					}
				}
			}
			lastScores = vector<int>(currentScores);
			if(m_verbose) cout << "Finished" << endl;
		}
	}

	cout << "A total of " << errors << " discrepancies occured in ";
	cout << (errors == 0 ? 0 : profiles.size()+1 ) << " of the " << numRuns << " runs\n";
	if(m_verbose) for(Util::preferenceProfile profile : profiles) Util::printPP(profile);
}

void Menu::getCW() {
	cout << "Condorcet winner: " << m_scorers[m_heuristic]->getCW() << endl;
}

void Menu::chooseHeuristics() {
	/* Insert option to choose the new scorer here */
	cout << "Choose heuristic (only first character is read):" << endl
		 << "[1]: Baseline" << endl
		 << "[2]: DFS" << endl
		 << "[3]: UCS" << endl
		 << "[4]: Smart Cache" << endl
		 << "[5]: Iterative DFS" << endl << endl;
		 //<< "[6]: New Scorer?" << endl << endl;
	string input;
	getline(cin, input);
	switch(input[0]) {	
	case '1': m_heuristic = BASELINE; break;
	case '2': m_heuristic = DFS; break;
	case '3': m_heuristic = UCS; break;
	case '4': m_heuristic = SMARTCACHE; break;
	case '5': m_heuristic = ICRDFS; break;
	//case '6': m_heuristic = NEWSCORER; break;
	default: 
		cout << "Character not recognized." << endl;
	}
}

void Menu::fillBuckets(vector<int> &curve, int sum, int current, int n, int scD) {
	if(current != n) {
		for(int i=0; i<=current; i++) {
			fillBuckets(curve, sum+i, current+1, n, scD);
		}
	} else {
		// when current == n is reached, one permutation is finished, 
		// and its repective bucket can be incremented
		curve[sum]++;		
		if(sum <= scD) g_countBefore++;
		else g_countAfter++;		
	}
}

void Menu::analyzeDistribution() {

	string input;
	cout << "Profile dimension: ";
	getline(cin, input);
	int n = stoi(input);

	// the maximum number of possible swaps is pre-determinable as follows:
	// if the dimension of profile is n x n, then the maximum number of possible
	// swaps is = sum (1 .. n) + 1   (+1 for the 'no swap at all' container)
	int vecSize = 1, scD = 0;
	for(int i=0; i<n; i++)		vecSize += i;
	for(int i=1; i<=n/2; i++)	scD += i;

	vector<int> curve(vecSize, 0);
	g_countAfter = 0;
	g_countBefore = 0;
	fillBuckets(curve, 0, 0, n, scD);
	
	// curve filled with values, now print
	int biggest = 0;
	for(unsigned int i=0; i<curve.size(); i++) {
		if(curve[i] > biggest) biggest = curve[i];
	}
	cout << "\nDistribution for n=m=" << n << endl;
	cout << "Number of profiles before minimal scd: " << g_countBefore << endl;
	cout << "Number of profiles after minimal scd: " << g_countAfter << endl;
	cout << "Total count: " << g_countBefore+g_countAfter << endl;
	cout << "Percentage saved: " << ((double)g_countAfter)/(((double)g_countBefore)+((double)g_countAfter))*100 << "%" << endl;
	cout << "\nMinimal scD indicated by arrow" << endl;
	for(unsigned int i=0; i<curve.size(); i++) {
		cout << "[" << i << "]: ";
		int max = ((int)(65 * (((float)curve[i]) / ((float)biggest))));
		for(int j=0; j<max; j++) {
			cout << "|";
		}		
		if(i == scD) cout << " <--  ";
		cout << endl;
	}
	cout << endl;
}

void Menu::testAll() {
	string input;
	cout << "Maximum wait-time in seconds: ";
	getline(cin, input);
	int waitTime = stoi(input);
	chrono::milliseconds checkIntervall = chrono::milliseconds(100); // something small. Must be less than waittime
	cout << endl;
    ofstream out("log.txt", ofstream::app);
	time_t t = time(0);
    struct tm * now = localtime( & t );
    out << "\n\n\n******Starting benchmark at " 
		<< (now->tm_year + 1900) << '-' 
        << (now->tm_mon + 1) << '-'
        <<  now->tm_mday
        << " with threshold " << input << " seconds******" << endl;

	for(int counter = 0; counter < 100; counter++) {
		out  << endl << endl << "Run number " << counter << ", starting with normal runs" << endl;  


		// for normal solving: 
		for(unsigned int i=0; i<m_scorers.size(); i++) {

			out << "scorer: " << i << endl << endl;
			float sumAvg = 0;
			int j;
			for(j=3; j<100; j++) {
				int k;
				vector<float> times;
				for(k=3; k<1000; k++) {
						
					// create new profile
					m_pp->alter(j, k);
					Util::scramble(*m_pp);
					m_scorers[i]->update();

					// start test
					clock_t t = clock();
					if(m_verbose) cout << "starting thread\n";
					bool timeUp = false;
					m_scorers[i]->setAbortState(false);
					auto future = async(launch::async, static_cast<void (DodgsonScorer::*)(void)>(&DodgsonScorer::getSCD), m_scorers[i]);

					// check if finished or time up
					while(future.wait_for(checkIntervall) != std::future_status::ready && !timeUp) {	
						timeUp = (((float)(clock() - t))/CLOCKS_PER_SEC) > waitTime;
						if(timeUp){
							if(m_verbose) cout << "Time is up, aborting .. " << endl;
							m_scorers[i]->setAbortState(true);		
						}
						if(m_verbose) cout << "current: " << (((float)(clock() - t))/CLOCKS_PER_SEC) << ", max: " << waitTime << endl;
					} 
					future.get();
					if(m_verbose) cout << "thread ended\n";

					// success:
					if(!timeUp) {
						times.push_back(((float)(clock() - t))/CLOCKS_PER_SEC);
						if(m_verbose) cout << "Scorer " << i << " solved a " << m_pp->n << "x" << m_pp->m << " profile in " << times.back() << " seconds.\n\n";
					} 
					// fail:
					else {
						if(times.size() != 0) {
							cout << "Scorer " << i << " failed at a " << m_pp->n << "x" << m_pp->m << " profile after " << times.back() << " seconds.\n\n";
							
							// the wonders of lambda
							sort(times.begin(), times.end(), [](float a, float b) {	return a < b; });
							float sum = 0;
							for(float f : times) {sum += f; cout << f << endl;}
							out << m_pp->n << "/" << m_pp->m 
								<< "  min: " << times.front() 
								<< "  median: " << times[times.size()/2] 
								<< "  max: " << times.back()  
								<< "  avg: " << sum/times.size() << endl;
							sumAvg += sum/times.size();
						} else cout << "Scorer "  << i << "immediate fail at " << m_pp->n << "x" << m_pp->m << endl;
						break;
					}
				}
				// stop condition, maybe just restrict max value if too slow
				if(k<=4) {
					cout << "Stop testing for n = " << j << endl << endl;
					break;
				}
			}
			out << "Total average: " << sumAvg/j << endl << endl;
		}
		cout << "finished normal runs, now testing for threaded variants\n\n\n";
		
		// for threaded solving: 
		for(unsigned int i=0; i<m_scorers.size(); i++) {
			if(m_scorers[i]->threadable()) {
				out << "scorer: " << i << endl << endl;
				float sumAvg = 0;
				int j;
				for(j=3; j<100; j++) {
					int k;
					vector<float> times;
					for(k=3; k<1000; k++) {
						
						// create new profile
						m_pp->alter(j, k);
						Util::scramble(*m_pp);
						m_scorers[i]->update();

						// start test
						clock_t t = clock();
						if(m_verbose) cout << "starting thread\n";
						bool timeUp = false;
						m_scorers[i]->setAbortState(false);
						auto future = async(launch::async, &DodgsonScorer::getFastSCD, m_scorers[i], NUM_THREADS);

						// check if finished or time up
						while(future.wait_for(checkIntervall) != std::future_status::ready && !timeUp) {	
							timeUp = (((float)(clock() - t))/CLOCKS_PER_SEC) > waitTime;
							if(timeUp){
								if(m_verbose) cout << "Time is up, aborting .. " << endl;
								m_scorers[i]->setAbortState(true);		
							}
							if(m_verbose) cout << "current: " << (((float)(clock() - t))/CLOCKS_PER_SEC) << ", max: " << waitTime << endl;
						} 
						future.get();
						if(m_verbose) cout << "thread ended\n";

						// success:
						if(!timeUp) {
							times.push_back(((float)(clock() - t))/CLOCKS_PER_SEC);
							if(m_verbose) cout << "Scorer " << i << " solved a " << m_pp->n << "x" << m_pp->m << " profile in " << times.back() << " seconds.\n\n";
						} 
						// fail:
						else {
							if(times.size() != 0) {
								cout << "Scorer " << i << " failed at a " << m_pp->n << "x" << m_pp->m << " profile after " << times.back() << " seconds.\n\n";
							
								// the wonders of lambda
								sort(times.begin(), times.end(), [](float a, float b) {	return a < b; });
								float sum = 0;
								for(float f : times) {sum += f; cout << f << endl;}
								out << m_pp->n << "/" << m_pp->m 
									<< "  min: " << times.front() 
									<< "  median: " << times[times.size()/2] 
									<< "  max: " << times.back()  
									<< "  avg: " << sum/times.size() << endl;
								sumAvg += sum/times.size();
							} else cout << "Scorer "  << i << "immediate fail at " << m_pp->n << "x" << m_pp->m << endl;
							break;
						}
					}
					// stop condition, maybe just restrict max value if too slow
					if(k<=4) {
						cout << "Stop testing for n = " << j << endl << endl;
						break;
					}
				}
				out << "Total average: " << sumAvg/j << endl << endl;
			}
		}
		cout << "finished threaded runs, starting next run\n\n\n";
	}
	out.close();
}

//void Menu::threadEval() {
//	cout << "Computing scores for scorer number " << m_heuristic << " ..." << endl;
//
//	clock_t t = clock();
//	pair<int, int> solution = m_scorers[m_heuristic]->getFastSCD(NUM_THREADS);
//	t = clock() - t;
//
//	cout << "\nTotal duration: " << t << " clock-ticks, or " << ((float)t)/CLOCKS_PER_SEC << " seconds" << endl;
//	cout << "Results: " << endl;
//	cout << "Alternative " << solution.second << " wins with a dodgson score of " << solution.first << ".\n";
//	cout << endl << "------------------------------------------------" << endl << endl;	
//}

//void Menu::repeatEval() {
//	string input;
//	cout << "Amount of runs: ";
//	getline(cin, input);
//	int numRuns = stoi(input), sum = 0, min = INT_MAX, max = 0;
//	vector<clock_t> benchmark;
//
//	cout << "\nResult in clock-ticks:\n+---------------" << endl;
//	for(int i=0; i<numRuns; i++) {
//		clock_t t = clock();
//		m_scorers[m_heuristic]->getSCD();
//		min = min > (clock() - t) ? (clock() - t) : min;
//		max = max < (clock() - t) ? (clock() - t) : max;
//		benchmark.push_back(clock() - t);
//		sum += benchmark[i];
//		cout << "|  " << benchmark[i] << endl;
//	}
//	cout << "+---------------\nAverage number of ticks: " << ((float)sum)/((float)numRuns) << endl << endl;
//	cout << "min: " << min << ", max:" << max << endl;
//	cout << "Clocks per second on this system: " << CLOCKS_PER_SEC << endl;
//}


void Menu::cleanUp() {
	// TODO: clean up scorers and anything
	// else that comes to mind. Not really
	// that much though, as the only object
	// on the heap is the profile, which
	// gets recycled for every scramble,
	// alteration or new loading.
}