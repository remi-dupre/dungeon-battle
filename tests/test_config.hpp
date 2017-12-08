#include <cxxtest/TestSuite.h>

#include <map>

#include "../src/config.hpp"
#include "../src/args.hpp"


#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC diagnostic ignored "-Wwritable-strings"


class ConfigTester : public CxxTest::TestSuite
{
public:
    void testFile1()
    {
        Configuration config;
        config.read("tests/config1.ini");

        TS_ASSERT_EQUALS(config.width, 800);
        TS_ASSERT_EQUALS(config.height, 600);
        TS_ASSERT_EQUALS(config.scalefactor, 1);
        TS_ASSERT_EQUALS(config.fullscreen, false);
        TS_ASSERT_EQUALS(config.vsync, true);
        TS_ASSERT_EQUALS(config.maxfps, 60);
    }

    void testFile2()
    {
        Configuration config;
        config.read("tests/config2.ini");

        TS_ASSERT_EQUALS(config.width, 685);
        TS_ASSERT_EQUALS(config.height, 534);
        TS_ASSERT_EQUALS(config.scalefactor, 2);
        TS_ASSERT_EQUALS(config.fullscreen, true);
        TS_ASSERT_EQUALS(config.vsync, true);
        TS_ASSERT_EQUALS(config.maxfps, 120);
    }
};


class ParserTester : public CxxTest::TestSuite
{
public:
    void testArgs1()
    {
        std::map<Option, std::string> options;

        char *argv[] =
        {
            "",
            "--config",
            "config3.ini"
        };

        TS_ASSERT_EQUALS(parse_arguments(options, array_length(argv), argv), 0);
        TS_ASSERT_EQUALS(options[Option::Config], "config3.ini");
    }

    void testArgs2()
    {
        std::map<Option, std::string> options;

        char *argv[] =
        {
            "",
            "--config=config2.ini"
        };

        TS_ASSERT_EQUALS(parse_arguments(options, array_length(argv), argv), 0);
        TS_ASSERT_EQUALS(options[Option::Config], "config2.ini");
    }
};
