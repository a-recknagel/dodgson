#pragma once

#include <string>
#include <fstream>
	
using namespace std;

class Logger{
public:
	/**
	 * @brief	returns the runtime instance of the Logging-class, and creates
	 *			a new one if it wasn't created yet
	 */
	static Logger& getInstance();

	/**
	 * @brief	writes an info message to the log, and prints it to the console,
	 *			depending on the debug-Level. 3 is required in order for infos
	 *			to be shown at runtime
	 */
	void info(string msg);

	/**
	 * @brief	writes a warning message to the log, and prints it to the console,
	 *			depending on the debug-Level. 2 is required in order for warnings
	 *			to be shown at runtime
	 */
	void warn(string msg);

	/**
	 * @brief	writes an error message to the log, and prints it to the console,
	 *			depending on the debug-Level. 1 is required in order for errors
	 *			to be shown at runtime
	 */
	void error(string msg);

	/**
	 * @brief	sets the debug level to the new value
	 */
	void setDBG(unsigned int debugLvl);

private:
	
	/**
	 * @brief	constructor, dumps the old log into the archive log and 
	 *			initializes a new one for the current session
	 */
	Logger();

	/**
	 * @brief	not to use
	 */
	Logger(Logger const& log);
     
	/**
	 * @brief	not to use
	 */
	Logger operator=(Logger const& log);

	/**
	 * @brief	destructor, closes all streams and destroys the instance.
	 */
	~Logger();

	unsigned int   m_debugLvl;	// variable containing the current debug level

	ofstream	   m_log;		// stream to the current logfile
};


