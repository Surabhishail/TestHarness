///////////////////////////////////////////////////////////////////////////////
// TestHarness.h :Execute test cases from test script suite using test engine//
// Version     : 1.0                                                         //
// Language    : C++,Visual Studio 2017                                      //
// Author      : Surabhi Shail                                               //
//               Graduate Student at Syracuse University                     //
//               Major-Computer Science                                      //
// Application : For automated testing                                       //
///////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <iomanip>
#include <string>
#include "testHarness.h"
using namespace TestHarness;

//----< display title with test count >--------------------------------

unsigned int test::count = 0;

void test::title(const std::string& msg)
{
	tout << "  test #" << ++count << ": " << msg;
	size_t len = tout.str().size();
	tout << "\n " << std::string(len, '-') << std::endl;
	tout.write();
}
//----< display passed message >---------------------------------------

bool test::passed(const std::string& msg)
{
	tout << "  -- " << msg << " passed --\n\n";
	tout.write();
	return true;
}
//----< display failed message >---------------------------------------

bool test::failed(const std::string& msg)
{
	tout << "  -- " << msg << " failed --\n\n";
	tout.write();
	return false;
}
//----< check for success >--------------------------------------------

bool test::check(bool predicate, const std::string& msg)
{
	if (predicate)
		return passed(msg);
	else
		return failed(msg);
}
//
//----< constructor takes title string for initial display >-----------

tester::tester(output& out, const std::string& msg)
{
	out << "\n  " << msg;
	out << "\n " << std::string(msg.length() + 2, '=') << "\n\n";
	out.write();
}
//----< put a test reference into container >--------------------------

void tester::registerTest(test *tf)
{
	_test.push_back(tf);
}
//----< execute all tests in container >-------------------------------

bool tester::execute(output& out)
{
	failed = 0;
	for (int i = 0; i < (int)_test.size(); i++)
	{
		if (!(*_test[i])())
			failed++;
	}
	out << "\n" << std::setw(4) << (unsigned)_test.size() << " tests executed";
	out << "\n" << std::setw(4) << failed << " tests failed\n\n";
	return (failed == 0);
}


#ifdef TEST_TESTHARNESS

class test1 : public test
{
public:
	test1(output& out) : test(out) {}
	bool operator()()
	{
		title("test1");
		return passed("test1");
	}
};

class test2 : public test
{
public:
	test2(output& out) : test(out) {}
	bool operator()()
	{
		title("test2");
		return failed("test2");
	}
};

class test3 : public test
{
public:
	test3(output& out) : test(out) {}
	bool operator()()
	{
		title("test3");
		return check(2 == 3, "test3");
	}
};

class test4 : public test
{
public:
	test4(output& out) : test(out) {}
	bool operator()()
	{
		title("container output");

		typedef std::vector<std::string> vec;
		vec v;
		v.push_back("ComputerScience");
		v.push_back("Code");
		v.push_back("Successfull_Execution");
		showCont<vec> show;
		tout << show(v);
		return check(v[2] == "Successfull_Execution", "test4");
	}
};
//
#include <sstream>

int main()
{
	stdOutput out;
	tester t1(out, "Testing Tester and Test");

	t1.registerTest(new test1(out));
	t1.registerTest(new test2(out));
	t1.registerTest(new test3(out));
	t1.registerTest(new test4(out));
	t1.execute(out);

	nullOutput null;
	tester t2(out, "Testing null output tests");
	t2.registerTest(new test1(null));
	t2.registerTest(new test2(null));
	t2.registerTest(new test3(null));
	t1.registerTest(new test4(null));
	t2.execute(out);
}

#endif