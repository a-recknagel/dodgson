#include "Logger.h"
#include <iostream>

Logger& Logger::getInstance() {
	// lazy initialization
	static Logger instance;
	return instance;
}

Logger::Logger() {
	// dump old log into archive
	ofstream archive("log_archive.txt", ios_base::binary | ios_base::app);
	ifstream log("log.txt", ios_base::binary);

	archive.seekp(0, ios_base::end);
	archive << "--- writing new log ---" << endl;
	archive << log.rdbuf();

	// initialising
	m_log.open("log.txt");
	m_debugLvl = 3;
}

void Logger::info(string msg) {
	m_log << msg << endl;

	if(m_debugLvl >= 3) 
		cout << msg << endl;
}

void Logger::warn(string msg) {
	m_log << msg << endl;

	if(m_debugLvl >= 2) 
		cout << msg << endl;
}

void Logger::error(string msg) {
	m_log << msg << endl;

	if(m_debugLvl >= 1) 
		cout << msg << endl;
}

Logger::~Logger() {
}
