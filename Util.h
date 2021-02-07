#pragma once

#include <vector>
#include <string>
#include <ctime>

using namespace std;
class Util {
public:

	/**
	 * @brief	the structure used to represent a preference profile in this
	 *			project
	 * @member	n is the amount of agents
	 * @member	m is the amount of alternatives
	 */
	struct preferenceProfile {
		int n, m;
		vector<vector<int> > data;
		preferenceProfile() {
			n = 0;
			m = 0;
			data = vector<vector<int> >(0);
		}
		preferenceProfile(int n, int m) {
			this->n = n;
			this->m = m;
			data = vector<vector<int> >(n);
			for(unsigned int i=0; i<data.size(); i++) {
				data[i] = vector<int>(m, i);
			}
		}
		void alter(int n, int m) {
			this->n = n;
			this->m = m;
			data = vector<vector<int> >(n);
			for(unsigned int i=0; i<data.size(); i++) {
				data[i] = vector<int>(m, i);
			}
		}
	};

	/**
	 * @brief	clones a preferenceProfile
	 */
	static void clonePP(const preferenceProfile &original, preferenceProfile &clone);

	/**
	 * @brief	returns true, iff the alternative is the condorcet winner in
	 *			the specified preference profile
	 */
	static bool isCW(vector<vector<int> > &data, int alternative);
	
	/**
	 * @brief	overload for preference profile
	 */
	static bool isCW(preferenceProfile &p, int alternative);

	/**
	 * @brief	fills the preference profile with a random element from the 
	 *			space of all possible preference profile (fancy speech for
	 *			"it gets scrambled")
	 */
	static void scramble(preferenceProfile &p, int seed = (int)time(0));

	/**
	 * @brief	splits a string at each instance of the delimination character.
	 *			The solution is stored in the token container
	 */
	static void split(string toSplit, vector<string> &tokens, char delim);

	/**
	 * @brief	prints a formated representation of this profile's data to the
	 *			standart output stream
	 */
	static void printPP(const preferenceProfile &p);

	/**
	 * @brief	returns a string representation of the given integer-vector
	 */
	static string toString(const vector<int> &vec);

private:

	static int numDigits(int q);

	static string chars(int s, char c);
};

