#pragma once
#ifndef __LOGGER_H_
#define __LOGGER_H_ 1
/**
<<<<<<< HEAD
 * \brief Quick and dirty toy logger. Better that nothing
 * 
 * Requires to call Init, to setup the logger.
 * Change the log level with SetLevel to any Log::LogLevel enumration value. By default is at Debug Level.
 * If the output stream is a file, is recomend to use RAII (opening it at the begin of the main function)
 * to avoid problems of not closing the file. The Logger not cloes the file or open it, only writes to it.
 *
 * Check src/tests/Log.cpp as a example of usage
 */

#include <iostream>
#include <memory>
=======
 * \brief Quick and dirty toy logger. Better than nothing
 * 
 * Requires to call Init, to setup the logger.
 * Change the log level with SetLevel to any Log::LogLevel enumeration value. By default it is at Debug Level.
 * If the output stream is a file, it is recommended to use RAII (opening it at the begin of the main function)
 * to avoid problems of not closing the file. The Logger does not close the file or open it, only writes to it.
 *
 * Check src/tests/Log.cpp for an example of usage
 */

#include <iostream>
#include <string>
#include <memory>
#include <cassert>
>>>>>>> b27385e1b5d96c34ecc6ea1671f1fe756d99e0f7

namespace Log {

	/**
	 * Logging levels
	 */
<<<<<<< HEAD
	enum class LogLevel {
=======
	enum LogLevel {
>>>>>>> b27385e1b5d96c34ecc6ea1671f1fe756d99e0f7
		OFF = -1,
		ERROR = 0,
		WARN = 1,
		INFO = 2,
		DEBUG = 3,
	};

<<<<<<< HEAD
	/**
	 * \brief Desired Logging level to show.
	 * If N in "debug(N)" is <= log_level, when will be show
	 * By default is at Debug level, displaying all log messages
	 */
	static LogLevel log_level = LogLevel::DEBUG;

	/**
	 * \brief Output stream to use
	 * If is a file, the external code must open it and close it
	 */
	static std::ostream* out;
	
	/**
	 * Initializes the Logger
	 * \param level Logger level. By default is at Debug level
	 * \param sout Output Streambuffer were to write. By default uses std::clog
	 */
	static void Init(LogLevel level = LogLevel::DEBUG) {
		Log::log_level = level;
		Log::out = &std::clog;
	}
			
	/**
	 * Initializes the Logger
	 * \param level Logger level. By default is at Debug level
	 * \param sout Output Streambuffer were to write. By default uses std::clog
	 */
	static void Init(std::ostream& sout, LogLevel level = LogLevel::DEBUG) {
		Log::log_level = level;
		Log::out = &sout;
	}
			
	/**
	 * Changes the actual logging level
	 */
	static void Level( LogLevel level) {
		Log::log_level = level;
	}

	class Print {
		public:

			/**
			 * /brief Builds a instance of the logger
			 * /param level Logging level of the message
			 */
			Print( LogLevel level ) : 
					output( level <= Log::log_level ), level(level) {
				
				if( output ) {
					switch (level) {
						case LogLevel::ERROR:
							*Log::out << "[ERROR] ";
							break;

						case LogLevel::WARN:
							*Log::out << "[WARNING] ";
							break;

						case LogLevel::INFO:
							*Log::out << "[INFO] ";
							break;

						case LogLevel::DEBUG:
							*Log::out << "[DEBUG] ";
							break;

						default:
							break;
					}
					// Auto ident more if is more severe
					//logger::out << std::string( static_cast<int>(logger::log_level) -
					// static_cast<int>(level), '\t');
				}
			}

			/**
			 * \brief Dectructor of the class. Here writes the outpot
			 */
			~Print() {
				if (output) {
					*Log::out << std::endl;
					Log::out->flush();
				}
			}

			/**
			 * \brief Opertaor << to write strings at the C++ way. Allow to chain multiple strings or values
			 */
			template<typename T>
				Print& operator<<( T t) {
					if( output ) {
						*Log::out << t;
						return *this;
=======

	class Print {
		private:
			bool output;    /// Enable output ?
			LogLevel level; /// Level of the message

			/**
			 * \brief Desired Logging level to show.
			 * If N in "debug(N)" is <= log_level, then it will be shown
			 * By default is at Debug level, displaying all log messages
			 */
			static LogLevel log_level;

			/**
			 * \brief Output stream to use
			 * If it is a file, the external code must open it and close it
			 */
			static std::ostream* out;

		public:
			/**
			 * Initializes the Logger
			 * \param level Logger level. By default it is at Debug level
			 * \param sout Output Streambuffer where to write. By default uses std::clog
			 */
			static void Init(LogLevel level = LogLevel::DEBUG) {
				log_level = level;
				out = &std::clog;
			}

			/**
			 * Initializes the Logger
			 * \param level Logger level. By default it is at Debug level
			 * \param sout Output Streambuffer where to write. By default uses std::clog
			 */
			static void Init(std::ostream& sout, LogLevel level = LogLevel::DEBUG) {
				log_level = level;
				out = &sout;
			}

			/**
			 * Changes the actual logging level
			 */
			static void Level( LogLevel level) {
				log_level = level;
			}

			/**
			 * /brief Builds an instance of the logger
			 * /param level Logging level of the message
			 */
			Print( LogLevel level ) : 
				output( level <= log_level ), level(level) {

					if( output ) {
						switch (level) {
							case LogLevel::ERROR:
								*out << "[ERROR] ";
								break;

							case LogLevel::WARN:
								*out << "[WARNING] ";
								break;

							case LogLevel::INFO:
								*out << "[INFO] ";
								break;

							case LogLevel::DEBUG:
								*out << "[DEBUG] ";
								break;

							default:
								break;
						}
						// Auto ident more if is more severe
						//*out << std::string( static_cast<int>(logger::log_level) -
						// static_cast<int>(level), '\t');
					}
				}

			/**
			 * \brief Destructor of the class. Here writes the output
			 */
			~Print() {
				if (output) {
					*out << std::endl;
					out->flush();
				}
			}

			/**
			 * \brief Operator << to write strings in the C++ way. Allows chaining multiple strings or values
			 */
			Print& operator<<(const std::string& str) {
				if( output && out != nullptr ) {
					*out << str;
				}
				return *this;
			}

			/**
			 * \brief Opertaor << to write anything at the C++ way. Allow to chain multiple strings or values
			 */
			template<typename T>
				Print& operator<<( T t) {
					if( output && out != nullptr ) {
						*out << t;
>>>>>>> b27385e1b5d96c34ecc6ea1671f1fe756d99e0f7
					}
					return *this;
				}

<<<<<<< HEAD
		private:
			bool output;    /// Enable output ?
			LogLevel level; /// Level of the message

=======
>>>>>>> b27385e1b5d96c34ecc6ea1671f1fe756d99e0f7
	};

} // END OF NAMESPACE logger

// Macros to type less
#define LOG_DEBUG Log::Print(Log::LogLevel::DEBUG)
#define LOG       Log::Print(Log::LogLevel::INFO)
#define LOG_WARN  Log::Print(Log::LogLevel::WARN)
#define LOG_ERROR Log::Print(Log::LogLevel::ERROR)


#endif // __LOGGER_H_
