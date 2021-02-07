#include "UniformCostSearch.h"
#include <iostream>
#include <list>

void UniformCostSearch::threadedSCD(int a) {
	int scD = 0;
	int maxDepth = m_pp->n;

	// get the position of 'a' in the preference profile, save it
	vector<int> posTable;
	for(unsigned int i=0; i<m_pp->data.size(); i++) {
		for(unsigned int j=0; j<m_pp->data[i].size(); j++) {
			if(m_pp->data[i][j] == a) {
				posTable.push_back(j);
				break;
			}
		}
	}
	// semi-associative priority queue
	list<vector<Node>> priorityQueue(1, vector<Node>());
	priorityQueue.front().push_back(Node()); // insert root

	// in each cycle, the current front-entry is popped, and if it is a leaf, it gets checked 
	// if it is a solution. If it is not, its children get added to the list, sorted
	// by their dodgson scores, so that the front-entry is always the entry with the smallest
	// dodgson score.
	int currentScore = 0;
	//cout << "start" << endl;
	while(!priorityQueue.empty()) {
		//if(m_abort) cout << this_thread::get_id() << " aborting at " << m_minimum << endl; 
		if((currentScore > m_minimum) || m_abort) return;
		m_count++;	
		Node smallest = Node(&priorityQueue.front().back());
		priorityQueue.front().pop_back();

		// checking for leaf and success
		if(smallest.swaps.size() == maxDepth) {
			//cout << "first check" << endl;
			if(isCW(smallest.swaps, posTable)){
				//cout << "second check" << endl;
				//cout << "m_count: " << m_count << endl;
				m_mtx.lock();
				if(currentScore <= m_minimum) {
					//cout << "solution update" << endl;
					m_minimum = currentScore;
					m_winningPairs.push_back(pair<int, int>(a, currentScore));
				}
				m_mtx.unlock();
				//cout << "done, return" << endl;
				return;
			}
		} else {
			// if not a leaf, expand the node
			//cout << "expand" << endl;
			auto it = priorityQueue.begin();
			for(int i=0; i<=posTable[smallest.swaps.size()]; i++){
				if(it==priorityQueue.end()) {
					priorityQueue.push_back(vector<Node>());
					it--;
				}
				it->push_back(Node(smallest, i));
				it++;
			}
			//cout << "done expanding" << endl;
		}
		if(priorityQueue.front().empty()){
			priorityQueue.pop_front();
			currentScore++;
		}
	}

	// if no solution was found .. something went wrong
	return;
}

int UniformCostSearch::getSCD(int a) {
	int scD = 0;
	int maxDepth = m_pp->n;

	// get the position of 'a' in the preference profile, save it
	vector<int> posTable;
	for(unsigned int i=0; i<m_pp->data.size(); i++) {
		for(unsigned int j=0; j<m_pp->data[i].size(); j++) {
			if(m_pp->data[i][j] == a) {
				posTable.push_back(j);
				break;
			}
		}
	}

	// semi-associative priority queue
	list<vector<Node>> priorityQueue(1, vector<Node>());
	priorityQueue.front().push_back(Node()); // insert root

	// in each cycle, the current front-entry is popped, and if it is a leaf, it gets checked 
	// if it is a solution. If it is not, its children get added to the list, sorted
	// by their dodgson scores, so that the front-entry is always the entry with the smallest
	// dodgson score.
	while(!priorityQueue.empty()) {
		if(m_abort) return INT_MAX;

		m_count++;	
		Node smallest = Node(&priorityQueue.front().back());
		priorityQueue.front().pop_back();

		// checking for leaf and success
		if(smallest.swaps.size() == maxDepth) {
			if(isCW(smallest.swaps, posTable)){
				//cout << "m_count: " << m_count << endl;
				m_scDs[a] = smallest.scD;
				return smallest.scD;
			}
		} else {
			// if not a leaf, expand the node
			auto it = priorityQueue.begin();
			for(int i=0; i<=posTable[smallest.swaps.size()]; i++){
				if(it==priorityQueue.end()) {
					priorityQueue.push_back(vector<Node>());
					it--;
				}
				it->push_back(Node(smallest, i));
				it++;
			}
		}
		if(priorityQueue.front().empty()) priorityQueue.pop_front();
	}

	// if no solution was found .. something went wrong
	return INT_MAX;
}

bool UniformCostSearch::isCW(vector<int> &swapProfile, vector<int> &posTable) {
	vector<vector<int> > toTest(m_pp->data);
	int a = toTest[0][posTable[0]];
	for(unsigned int i=0; i<swapProfile.size(); i++) {
		for(int j=0; j<swapProfile[i]; j++) {
			toTest[i][posTable[i]-(j)] = toTest[i][posTable[i]-(j+1)];
			toTest[i][posTable[i]-(j+1)] = a;
		}
	}
	return Util::isCW(toTest, a);	
}