#include "DodgsonScorer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>

DodgsonScorer::DodgsonScorer(Util::preferenceProfile &pp) {
	m_count = 0;
	m_supportsThreads = false;
	m_minimum = INT_MAX;
	m_futures = vector<future<void> >();
	m_pp = &pp;
	m_scDs = vector<int>(pp.m, -1);
	m_abort = false;
}

DodgsonScorer::DodgsonScorer(string fileName) {
	m_count = 0;
	m_supportsThreads = false;
	m_minimum = INT_MAX;
	m_futures = vector<future<void> >();
	m_abort = false;
	string line;
	ifstream file(fileName);
	if(file.is_open()) {
		vector<string> tokens;
		int n, m;

		// get n and m
		getline(file, line);
		Util::split(line, tokens, ' ');
		n = stoi(tokens[0]);
		m = stoi(tokens[1]);
		m_pp = new Util::preferenceProfile(n, m);

		// fill the profile itself
		for(int i=0; i<n; i++) {
			getline(file, line);
			Util::split(line, tokens, ' ');
			for(int j=0; j<m; j++) {
				m_pp->data[i][j] = stoi(tokens[j]);
			}
		}

		// get possible dodgson scores
		getline(file, line);
		Util::split(line, tokens, ' ');
		for(unsigned int i=0; i<m_scDs.size(); i++) {
			m_scDs[i] = stoi(tokens[i+1]);
		}
	}
	file.close();	
}

DodgsonScorer::DodgsonScorer(DodgsonScorer &original) {
	m_count = 0;
	m_supportsThreads = false;
	m_minimum = INT_MAX;
	m_futures = vector<future<void> >();
	m_pp = &(original.getPP());
	m_scDs = original.getScores();
	m_abort = false;
}

void DodgsonScorer::setRandPP(int n, int m) {
	// a delete of the old profile data should be here
	m_pp = new Util::preferenceProfile(n, m);
	Util::scramble(*m_pp);
	m_scDs = vector<int>(m, -1);
}

void DodgsonScorer::getSCD() {
	m_count = 0;
	for(int i=0; i<m_pp->m; i++) {
		if(m_abort) return;
		m_scDs[i] = getSCD(i);
	}
}

vector<pair<int,int> > DodgsonScorer::getFastSCD(unsigned int maxThreads) {
	m_count = 0;
	int minSCD = INT_MAX;
	m_winningPairs = vector<pair<int,int> >();

	if(m_supportsThreads) {
		// initialize
		vector<int> runOrder, tmp(m_pp->m, 0);
		for(vector<int> row : m_pp->data) {
			for(unsigned int i=0; i<row.size(); i++) {
				tmp[row[i]] += i;
			}
		}
		for(int i=0; i<m_pp->m; i++) {
			int max = -1, idx = 0;
			for(unsigned int j=0; j<tmp.size(); j++) {
				if(tmp[j] > max) {
					max = tmp[j];
					idx = j;
				}
			}
			runOrder.push_back(idx);
			tmp[idx] = -1;
		}
		m_minimum = INT_MAX;
		m_futures = vector<future<void> >();
		// done

		// starting threads in heuristic order until all alternatives got checked
		while(!runOrder.empty()) {
			if(m_futures.size() < maxThreads) {
				// starts a new thread with the next probable alternative in line
				m_futures.push_back(future<void>(async(launch::async, &DodgsonScorer::threadedSCD, this, runOrder.back())));
				runOrder.pop_back();
			} else {
				// if the maximum number of threads is running, check if any are already
				// finished. If yes, throw them out to make place for new ones.
				// ERROR: somewhere here, things get deleted that shouldn't. It can be circumvented
				// by setting maxThreads unreasonably high, so that this code never gets entered.
				// But in the long run, for optimazitions sake, it needs to be fixed.
				for(unsigned int i=0; i<m_futures.size(); i++) {
					if(m_futures[i].wait_for(chrono::milliseconds(10))==std::future_status::ready) {
						m_futures.erase(m_futures.begin()+i);
						i--;
					}
				}				
			}
		}
		for(unsigned int i=0; i<m_futures.size(); i++) {
			
			m_futures[i].get();
		}
		// done, the lowest scd is saved in m_minimum. Now clean up the collector
		for(unsigned int i=0; i<m_winningPairs.size() && !m_abort; i++) {
			if(m_winningPairs[i].second != m_minimum) {
				m_winningPairs.erase(m_winningPairs.begin()+i);
				i--;
			}
		}
		// finished fastSCD for threading variant

	} else {
		getSCD();
		m_minimum = INT_MAX;
		for(unsigned int i=0; i<m_scDs.size(); i++) {
			if(m_scDs[i] < m_minimum) { m_minimum = m_scDs[i]; }
		}
		for(unsigned int i=0; i<m_scDs.size(); i++) {
			if(m_scDs[i] == m_minimum) { m_winningPairs.push_back(pair<int, int>(i, m_minimum)); }
		}	
		// finished "fastSCD" for non-threading variant

	}

	// I even get to use lambda functions
	sort(m_winningPairs.begin(), m_winningPairs.end(), [](pair<int, int> a, pair<int, int> b)
	{
		return a.first < b.first;
	});
	return m_winningPairs;
}


int DodgsonScorer::getCW() {
	int n = m_pp->n;
	int m = m_pp->m;
	vector<vector<int> > tmp = vector<vector<int> >(m, vector<int>(m, 0));
	
	// filling the tmp register recording pairwise wins
	for(int i=0; i<n; i++) {
		for(int j=0; j<m; j++)
			for(int k=j; k<m; k++)
				tmp[m_pp->data[i][j]][m_pp->data[i][k]]++;
		// checking if cw condition is already met
		if(i > (n/2)) {
			for(int p=0; p<m; p++) {
				bool check = true;
				for(int q=0; check && (q<m); q++)
					if(tmp[p][q] <= (n/2))
						check = false;
		
				// if yes, we have a winner
				if(check) return p;
			}
		}
	}
	for(int i=0; i<m; i++){
		cout << endl;
		for(int j=0; j<m; j++)
			cout << tmp[i][j];
	}
	cout << endl << endl;
	// if never, we have none
	return -1;
}

void DodgsonScorer::writePP(string fileName) {
	stringstream line;
	ofstream file(fileName);
	if(file.is_open()) {
		int n = m_pp->n;
		int m = m_pp->m;

		// write n and m
		line << n << " " << m << '\n';
		file << line.str();

		// write the profile itself
		for(int i=0; i<n; i++) {
			line.str("");
			for(int j=0; j<m; j++) {
				line << m_pp->data[i][j] << " "; 
			}
			line.str().pop_back();
			line << '\n';
			file << line.str();
		}

		// write possible dodgson scores
		line.str("");
		for(unsigned int i=0; i<m_scDs.size(); i++) {
			line << m_scDs[i] << " ";
		}
		line.str().pop_back();
		file << "scDs: " << line.str();
	}
	file.close();
}

void DodgsonScorer::print() {
	Util::printPP(*m_pp);

	cout << endl << "|";
	for(unsigned int i=0; i<m_scDs.size(); i++) {
		cout << "  " << i << "  |";
	}
	cout << endl << "+";
	for(unsigned int i=0; i<m_scDs.size(); i++) {
		cout << "-----+";
	}
	cout << endl << "| ";
	for(unsigned int i=0; i<m_scDs.size(); i++) {
		cout << (m_scDs[i] < 0 ? "" : " ") << m_scDs[i] << "  | ";
	}
	cout << endl;
}