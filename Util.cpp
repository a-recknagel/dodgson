#include "Util.h"
#include <list>
#include <cstdlib>
#include <iostream>
#include <cmath>


void Util::clonePP(const preferenceProfile &original, preferenceProfile &clone) {
	
	// straight forward
	int n = original.n;
	int m = original.m;
	clone = preferenceProfile(n, m);
	for(int i=0; i<n; i++) 
		for(int j=0; j<m; j++) 
			clone.data[i][j] = original.data[i][j];	
}

bool Util::isCW(vector<vector<int> > &data, int alternative) {
	int n = data.size();
	int m = data[0].size();
	vector<int> winCount(m, 0);
	for(int i=0; i<n; i++) {

		// count wins in current column
		bool before = false;
		for(int j=0; j<m; j++) {
			if(before) 
				winCount[data[i][j]]++;
			if(data[i][j]==alternative)
				before = true;
		}

		// test if condorcet condition is met
		bool win = true;
		for(unsigned int k=0; k<winCount.size(); k++) {
			if(k!=alternative) {
				if(winCount[k]<((n/2)+1)) {
					win=false;
					break;
				}
			}
		}
		if(win)
			return true;
	}

	// if it wasn't in the final round, the alternative is no condorcet winner
	return false;
}

bool Util::isCW(preferenceProfile &p, int alternative) {
	return isCW(p.data, alternative);
}

void Util::scramble(preferenceProfile &p, int seed) {
	srand(seed);
	for(int i=0; i<p.n; i++) {
		vector<int> values(p.m);
		for(unsigned int k=0; k<values.size(); k++) 
			values[k] = k;
		for(unsigned int j=0; j<values.size(); j++) {

			// shoot for a new value
			int idx = rand()%values.size();

			// if it wasn't already picked, pick it and block it
			if(values[idx]!=-1) {
				p.data[i][j] = values[idx];
				values[idx] = -1;
			}

			// if it was blocked, try again
			else{
				j--;
			}
		}
	}
}

void Util::split(string toSplit, vector<string> &tokens, char delim) {
	tokens = vector<string>();
	int idx = 0;
	for(unsigned int i=0; i<toSplit.length(); i++) {
		
		// if the deliminator is encountered, push the indexed substring
		if(toSplit[i] == delim) {
			tokens.push_back(toSplit.substr(idx, i-idx));
			idx = i+1;
		}
	}

	// don't forget the tail
	if(idx != toSplit.size())
		tokens.push_back(toSplit.substr(idx, string::npos));
}

void Util::printPP(const preferenceProfile &p) {
	int n = p.n;
	int m = p.m;
	cout << "agents:       " << n << endl;
	cout << "alternatives: " << m << endl << endl;
	int numSpaces = numDigits(m > n ? m : n) + 1;
	
	// first three rows, agents and format
	for(int i=0; i<n; i++) {
		cout << "| " << i << chars(numSpaces - numDigits(i), ' '); 
	}
	cout << "|" << endl;
	for(int i=0; i<2; i++) {
		for(int j=0; j<n; j++) {
			cout << "+" << chars(numSpaces+1, '-');
		}
		cout << "+" << endl;
	}
	
	// now for the actual profile
	for(int i=0; i<m; i++) {
		for(int j=0; j<n; j++) {
			cout << "| " << p.data[j][i] << chars(numSpaces - numDigits(p.data[j][i]), ' '); 
		}
		cout << "|" << endl;
		for(int k=0; k<n; k++) {
		cout << "+" << chars(numSpaces+1, '-');
		}
		cout << "+" << endl;
	}
}

string Util::toString(const vector<int> &vec) {
	string ret = "";
	for(unsigned int i=0; i<vec.size(); i++) {
		ret+= std::to_string((static_cast<long long>(vec[i])))+", ";
	}
	return ret;
}

int Util::numDigits(int q) {
	if(q == 0) return 1;
	return (int)(log10((float)(q))) + 1;
}

string Util::chars(int s, char c) {
	string str = "";
	for(int i=0; i<s; i++)
		str += c;
	return str;
}