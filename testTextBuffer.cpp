/* unit tests for TextBuffer class */

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "TextBuffer.h"
#include <stdio.h>
#include <cstdio>

static void generateReferenceFile(const std::string& referenceName)
{
    std::ofstream outStream;
    outStream.open(referenceName);
    outStream << "Hello world\nSecond line";
    outStream.close();
}

static void removeFile(const std::string& referenceName)
{
    std::remove(referenceName.c_str());
}



TEST_CASE("TextBuffer loads")
{
    std::string referenceString = "Hello world\nSecond line";
    std::string loadFilePath = "sample_input.txt";
    generateReferenceFile(loadFilePath);
    SECTION("Load with constructor")
    {
        TextBuffer buffer(loadFilePath);
        REQUIRE(buffer.getString() == referenceString);
    }

    SECTION("Load with member")
    {
        TextBuffer buffer;
        buffer.loadFromText(loadFilePath);
        REQUIRE(buffer.getString() == referenceString);
    }
    removeFile(loadFilePath);
}

TEST_CASE("Textbuffer writes")
{

}

TEST_CASE("TextBuffer basic operations")
{

}

TEST_CASE("TextBuffer buffering operations")
{

}