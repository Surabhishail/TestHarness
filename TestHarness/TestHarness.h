#ifndef TESTHARNESS_H
#define TESTHARNESS_H
///////////////////////////////////////////////////////////////////////////////
// TestHarness.h :Execute test cases from test script suite using test engine//
// Version     : 1.0                                                         //
// Language    : C++,Visual Studio 2017                                      //
// Author      : Surabhi Shail                                               //
//               Graduate Student at Syracuse University                     //
//               Major-Computer Science                                      //
// Application : For automated testing                                       //
///////////////////////////////////////////////////////////////////////////////
/*
 * Module Operations:
 * ==================
 * Provides a test aggregator, e.g., a container of test objects, and a base
 * class for all test objects.
 *
 * Public Interface:
 * =================
 * tester t;                              // create a tester
 *
 * class myTest : public test             // create a test
 * {                                      //   use title pass, fail, and check
 * public:                                //   functions in body of operator()()
 *   myTest(output& out) : test(out) {}   //   to display title and
 *   bool operator()()                    //   return pass/fail results
 *   { ... }
 * }
 *
 * nullOutput null;                       // create channel to discard output
 * stdOutput  out;                        // send output to std out
 * null.showAll(true);                    // don't discard null output
 *
 * t.register(new myTest(out));           // give tester a test
 * t.register(new yourTest(null));        //   and another test
 * t.execute();                           // execute all tester's tests
 */
 /*
  * Maintenance History:
  * ====================
  * ver 1.1 : 29 Feb 04
  *   - added passed, failed, and check functions
  *     that make test writing a bit more productive
  * ver 1.0 : 21 Feb 04
  *   - first release
  *
  * Build Process:
  * ==============
  * Required Files:
  *   TestHarness.h, TestHarness.cpp
  *
  * Compiler Command:
  *   cl /DTEST_TESTHARNESS /EHsc TestHarness.cpp;
  */

  //
#include <vector>
#include <iostream>
#include <sstream>
#include <string>

namespace TestHarness
{
	///////////////////////////////////////////////////////////////////////
	// Test I/O classes

	class output : public std::ostringstream
	{
	public:
		output() : _showAll(false) {}
		virtual ~output() {}
		virtual void write() = 0;
		void showAll(bool show) { _showAll = show; }
	protected:
		bool _showAll;
	};

	class stdOutput : public output
	{
	public:
		~stdOutput() { std::cout << str(); std::cout.flush(); }
		void write() { std::cout << str(); str(""); std::cout.flush(); }
	};

	class nullOutput : public output
	{
	public:
		~nullOutput()
		{
			if (_showAll)
			{
				std::cout << str();
				std::cout.flush();
			}
		}
		void write()
		{
			if (_showAll)
			{
				std::cout << str();
				str("");
				std::cout.flush();
			}
		}
	};
	//
	  /////////////////////////////////////////////////////////////////////
	  // generic function to display STL container contents
	  //   - works for vectors, sets, and lists

	template <typename T, typename Output = stdOutput>
	class showCont
	{
	public:
		std::string operator()(const T& cont)
		{
			Output out;
			out << "  ";
			typename T::const_iterator it;
			for (it = cont.begin(); it != cont.end(); it++)
				out << *it << " ";
			out << "\n\n";
			out.write();
			return out.str();
		}
	};

	///////////////////////////////////////////////////////////////////////
	// test class is base for all concrete tests

	class test
	{
	public:
		test(output& out) : tout(out) {}
		virtual bool operator()() = 0;
		virtual ~test() {};
		void title(const std::string& t);
		bool passed(const std::string& msg);
		bool failed(const std::string& msg);
		bool check(bool predicate, const std::string& msg);
	protected:
		output& tout;
		static unsigned int count;
	};

	///////////////////////////////////////////////////////////////////////
	// tester class is a test aggregator, e.g., a container for tests

	class tester
	{
	public:
		tester(output& out, const std::string& msg);
		bool execute(output& out);
		void registerTest(test *to);
	private:
		std::vector<test*> _test;
		unsigned failed;
	};
}

#endif