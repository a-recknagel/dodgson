#include "Baseline.h"
#include <iostream>
#include <climits>

void Baseline::threadedSCD(int a) {}

int Baseline::getSCD(int a) {
	int scD = INT_MAX;
	vector<pair<vector<vector<int> >, int> > cache = vector<pair<vector<vector<int> >, int> > ();
	createCache(a, 0, m_pp->data, 0, cache);

	vector<vector<int> > winner = vector<vector<int> >();
	for(unsigned int i=0; i<cache.size(); i++) {
		if(m_abort) return INT_MAX;
		if(Util::isCW(cache[i].first, a) && (scD > cache[i].second)) {
			scD = cache[i].second;
			winner = cache[i].first;
		}
	}
	m_scDs[a] = scD;	
	//cout << "m_count: " << m_count << endl;
	return scD;
}

void Baseline::createCache(int a, int depth, vector<vector<int> > current, int scD, vector<pair<vector<vector<int> >, int> > &cache) {
	if(m_abort) return;
	
	m_count++;
	int swaps = 0;
	for(unsigned int i=0; i<current[depth].size(); i++) {
		if(current[depth][i] == a) swaps = i;
	}
	while(swaps>=0) {
		if(depth==(current.size()-1)) {
			cache.push_back(pair<vector<vector<int> >, int>(current, scD));
		}
		else 
			createCache(a, depth+1, current, scD, cache);

		if(swaps > 0) {
			scD++;
			int tmp = current[depth][swaps];
			current[depth][swaps] = current[depth][swaps-1];
			current[depth][swaps-1] = tmp;
		}
		swaps--;
	}
}