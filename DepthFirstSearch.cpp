#include "DepthFirstSearch.h"
#include <iostream>
#include <climits>

void DepthFirstSearch::threadedSCD(int a) {}

int DepthFirstSearch::getSCD(int a) {
	int scD = INT_MAX;
	vector<vector<int> > minProfile;
	search(a, 0, 0, m_pp->data, scD, minProfile);
	
	//cout << "m_count: " << m_count << endl;
	m_scDs[a] = scD;	
	return scD;
}

void DepthFirstSearch::search(int a, int depth, int scD, vector<vector<int> > profile, int &minSCD, vector<vector<int> > &minProfile) {	
	if(m_abort) return;	
	
	m_count++;
	int swaps = 0;
	for(unsigned int i=0; i<profile[depth].size(); i++) {
		if(profile[depth][i] == a) swaps = i;
	}
	while(swaps>=0) {
		if(depth==(profile.size()-1)) {
			if(Util::isCW(profile, a) && (scD<minSCD)) {
				minSCD = scD;
				minProfile = profile;
			}
		}
		else 
			search(a, depth+1, scD, profile, minSCD, minProfile);
		
		if(swaps > 0) {
			scD++;
			int tmp = profile[depth][swaps];
			profile[depth][swaps] = profile[depth][swaps-1];
			profile[depth][swaps-1] = tmp;
		}
		swaps--;
	}
}


