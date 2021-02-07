#include "SmartCache.h"
#include <iostream>

void SmartCache::threadedSCD(int a) {
	/* initialising values */
	int n = m_pp->n, m = m_pp->m;
	vector<vector<int> > cache;
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
			return; // suboptimal, but I can't bring myself to edit 'fill'
		}
		fill(posTable, profile, 0, i, cache);
		if(m_abort){
			//cout << this_thread::get_id() << " aborting at " << m_minimum << endl; 
			return; // suboptimal, but I can't bring myself to edit 'fill'
		}		
		if(i > m_minimum) return;
		vector<vector<int> > solution = checkCache(cache, posTable);

		if(solution.size() > 0){
			//cout << "m_count: " << m_count << endl;
			m_mtx.lock();
			if(i <= m_minimum){
				m_minimum = i;
				m_winningPairs.push_back(pair<int, int>(a, i));
			}
			m_mtx.unlock();
			return;
		}
		cache = vector<vector<int> >();
	}	
	return;
}

int SmartCache::getSCD(int a) {
	/* initialising values */
	int n = m_pp->n, m = m_pp->m;
	vector<vector<int> > cache;
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
		fill(posTable, profile, 0, i, cache);
		if(m_abort) return INT_MAX;
		vector<vector<int> > solution = checkCache(cache, posTable);
		if(solution.size() > 0){
			//cout << "m_count: " << m_count << endl;
			m_scDs[a] = i;
			return i;
		}
		cache = vector<vector<int> >();
	}	
	return INT_MAX;
}

vector<vector<int> > SmartCache::checkCache(vector<vector<int> > &cache, vector<int> &posTable) {
	int a = m_pp->data[0][posTable[0]];
	for(auto iter=cache.begin(); iter!=cache.end(); iter++) {
		vector<vector<int> > prefProf = vector<vector<int> >(m_pp->data);

		for(unsigned int i=0; i<iter->size(); i++) {
			for(int j=0; j<iter->at(i); j++) {
				int tmp = prefProf[i][posTable[i] - j];
				prefProf[i][posTable[i] - j] = prefProf[i][posTable[i] - (j + 1)];
				prefProf[i][posTable[i] - (j + 1)] = tmp;
			}
		}	
		if(Util::isCW(prefProf, a)) {
			return prefProf;
		}
	}
	return vector<vector<int> >();
}

void SmartCache::fill(vector<int> &posTable, vector<int> &profile, int depth, int scD, vector<vector<int> > &cache) {
	m_count++;
	if(depth == m_pp->n) {
		if(scD == 0) {
			cache.push_back(vector<int>(profile));
		}
	} else {
		for(int i=0; i<=posTable[depth]; i++) {
			if((scD-i) >= 0) {
				profile[depth] = i;
				fill(posTable, profile, depth+1, scD-i, cache);
			} else break;
		}
	}
}

/*
string SmartCache::getString(vector<int> profile) {
	string ret;
	for(unsigned int i=0; i<profile.size(); i++) {
		ret += to_string(static_cast<long long>(profile[i])) + " ";
	}
	return ret;
}


	Thoughts: What gets generated here? Maybe making a string and sorting them is really bad, since the rows they are
	in represent the agent's index. I think that's what happens, and maybe we dont even need to permutate in the end.
	It should be possible to generate all solutions here. Or in any case, we cannot make them at a later point using 
	this input.  
void SmartCache::fill(int remainder, vector<int> &posTable, vector<int> &swaps, int idx, map<string, vector<int> > &cache) {
	if(remainder == 0) {
		// maybe we are done and everything is added?
		auto iter = swaps.begin();
		for(int i=0; i<idx; i++) { iter++; }
		vector<int> toAdd(swaps.begin(), iter);
		string key = getString(toAdd);
		cout << idx << " - " << key << endl;
		cache.insert(pair<string, vector<int> >(key, toAdd));
	} else {	
		// if we are at the end, try to dump the rest of the remainder.
		if((idx+1) == m_pp->n) {
			if(remainder < posTable[idx]) {	// can that many swaps actually be made in the row we are at?
				swaps[idx] = remainder;
				string key = getString(swaps);			
				cache.insert(pair<string, vector<int> >(key, vector<int>(swaps)));				
			}
		} else {
			// if we are not, try all possible ways.
			// TODO: start at zero, and let it generate all possible solutions dfs style.
			// TODO: check if the check is not too soon. I think <= might be right here.
			for(int i=1; i<=remainder; i++) {
				if(i < posTable[idx]) {		// can that many swaps actually be made in the row we are at?
					swaps[idx] = i;
					fill(remainder-i, posTable, swaps, idx+1, cache);
				} else break;
			}
		}
	}
}*/

/*void SmartCache::sumDown(vector<vector<int> > &input, map<string, vector<int> > &mem){
	for(unsigned int i=0; i<input.size(); i++) {							// for each vector in input..
		for(unsigned int j=0; j<input[i].size()-1; j++) {				// run through it, and..
			for(unsigned int k=j+1; k<input[i].size(); k++) {			// have a second index check, if..
				if(k==1 || (input[i][k] != input[i][k-1])) {	// looking forward, we can create a new..
					vector<int> permutation;					// permutation of the vector.
					for(unsigned int p=0; p<input[i].size(); p++) {		// If we could, copy the vector, but..
						if(p==j || p==k) {
							if(p==j) {							// add the two positions where the
								permutation.push_back(input[i][j]+input[i][k]);
							}									// indices were up, so that it is
						} else {								// one entry shorter.
							permutation.push_back(input[i][p]);	// All others, input normally.
						}
					}
					bubbleSort(permutation);					// Finally, try to add the new, sorted, permutation.
					string rep = Util::toString(permutation);	// But only, .. 
					if(mem.find(rep) != mem.end()) {			// if it is the first of its kind.
						mem.insert(pair<string, vector<int> >(rep, permutation));
					}
				}
			}
		}
	}
}*/

/*
void SmartCache::bubbleSort(vector<int> &toSort) {
	bool finished = false;
	while(!finished) {
		finished = true;
		for(unsigned int i=0; i<toSort.size()-1; i++) {
			if(toSort[i+1] > toSort[i]) {
				finished = false;
				int tmp = toSort[i];
				toSort[i] = toSort[i+1];
				toSort[i+1] = tmp;
			}
		}
	}
}*/
