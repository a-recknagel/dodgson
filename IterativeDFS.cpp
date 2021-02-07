#include "IterativeDFS.h"
#include <iostream>

void IterativeDFS::threadedSCD(int a) {
	/* initialising values */
	int n = m_pp->n, m = m_pp->m;
	vector<int> profile(n, 0);
	vector<int> posTable;
	for(unsigned int i=0; i<m_pp->data.size(); i++) {
		for(unsigned int j=0; j<m_pp->data[i].size(); j++) {
			if(m_pp->data[i][j] == a) {
				posTable.push_back(j);
				break;
			}
		}
	}

	/* Running algorithm for all possible scores */
	for(int i=0; (i<=m_minimum) && (i<(n/2+1)*m); i++) {
		if(m_abort){
			//cout << this_thread::get_id() << " aborting at " << m_minimum << endl; 
			return; // suboptimal, but I can't bring myself to edit 'runICRDFS'
		}
		if(runICRDFS(posTable, profile, 0, i)){
			//cout << "m_count: " << m_count << endl;
			m_mtx.lock();
			if(i <= m_minimum){
				m_minimum = i;
				m_winningPairs.push_back(pair<int, int>(a, i));
			}
			m_mtx.unlock();
			return;
		}
	}	
	return;
}

int IterativeDFS::getSCD(int a) {
	/* initialising values */
	int n = m_pp->n, m = m_pp->m;
	vector<int> profile(n, 0);
	vector<int> posTable;
	for(unsigned int i=0; i<m_pp->data.size(); i++) {
		for(unsigned int j=0; j<m_pp->data[i].size(); j++) {
			if(m_pp->data[i][j] == a) {
				posTable.push_back(j);
				break;
			}
		}
	}

	/* Running algorithm for all possible scores */
	for(int i=0; i<(n/2+1)*m; i++) {
		if(m_abort) return INT_MAX;
		if(runICRDFS(posTable, profile, 0, i)){
			//cout << "m_count: " << m_count << endl;	
			m_scDs[a] = i;
			return i;
		}
	}	
	return INT_MAX;
}
	
bool IterativeDFS::checkSwapProf(vector<int> &swapProf, vector<int> &posTable) {
	int a = m_pp->data[0][posTable[0]];
	vector<vector<int> > prefProf = vector<vector<int> >(m_pp->data);
	for(unsigned int i=0; i<swapProf.size(); i++) {
			
		for(int j=0; j<swapProf.at(i); j++) {
			int tmp = prefProf[i][posTable[i] - j];
			prefProf[i][posTable[i] - j] = prefProf[i][posTable[i] - (j + 1)];
			prefProf[i][posTable[i] - (j + 1)] = tmp;
		}
	}
	return Util::isCW(prefProf, a);
}

bool IterativeDFS::runICRDFS(vector<int> &posTable, vector<int> &profile, int depth, int scD) {
	if(m_abort) return true;
	
	m_count++;
	if(depth == m_pp->n) {
		if(scD == 0) {
			return checkSwapProf(profile, posTable);
		}
		return false;
	} else {
		for(int i=0; i<=posTable[depth]; i++) {
			if((scD-i) >= 0) {
				profile[depth] = i;
				if(runICRDFS(posTable, profile, depth+1, scD-i)) return true;
			} else break;
		}
		return false;
	}
}