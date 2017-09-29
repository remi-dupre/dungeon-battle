/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/TestMain.h>
#include <cxxtest/ErrorPrinter.h>

int main( int argc, char *argv[] ) {
 int status;
    CxxTest::ErrorPrinter tmp;
    CxxTest::RealWorldDescription::_worldName = "cxxtest";
    status = CxxTest::Main< CxxTest::ErrorPrinter >( tmp, argc, argv );
    return status;
}
bool suite_ConfigTester_init = false;
#include "/home/remi/code/dungeon-battle/tests/test.hpp"

static ConfigTester suite_ConfigTester;

static CxxTest::List Tests_ConfigTester = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_ConfigTester( "tests/test.hpp", 17, "ConfigTester", suite_ConfigTester, Tests_ConfigTester );

static class TestDescription_suite_ConfigTester_testFile1 : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_ConfigTester_testFile1() : CxxTest::RealTestDescription( Tests_ConfigTester, suiteDescription_ConfigTester, 20, "testFile1" ) {}
 void runTest() { suite_ConfigTester.testFile1(); }
} testDescription_suite_ConfigTester_testFile1;

static class TestDescription_suite_ConfigTester_testFile2 : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_ConfigTester_testFile2() : CxxTest::RealTestDescription( Tests_ConfigTester, suiteDescription_ConfigTester, 33, "testFile2" ) {}
 void runTest() { suite_ConfigTester.testFile2(); }
} testDescription_suite_ConfigTester_testFile2;

static ParserTester suite_ParserTester;

static CxxTest::List Tests_ParserTester = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_ParserTester( "tests/test.hpp", 48, "ParserTester", suite_ParserTester, Tests_ParserTester );

static class TestDescription_suite_ParserTester_testArgs1 : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_ParserTester_testArgs1() : CxxTest::RealTestDescription( Tests_ParserTester, suiteDescription_ParserTester, 51, "testArgs1" ) {}
 void runTest() { suite_ParserTester.testArgs1(); }
} testDescription_suite_ParserTester_testArgs1;

static class TestDescription_suite_ParserTester_testArgs2 : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_ParserTester_testArgs2() : CxxTest::RealTestDescription( Tests_ParserTester, suiteDescription_ParserTester, 66, "testArgs2" ) {}
 void runTest() { suite_ParserTester.testArgs2(); }
} testDescription_suite_ParserTester_testArgs2;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
