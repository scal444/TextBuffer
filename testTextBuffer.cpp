/* unit tests for TextBuffer class */

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "TextBuffer.h"
#include <stdio.h>
#include <cstdio>

// -----------------------------------------------------------------------------------
// utility functions for tests
// -----------------------------------------------------------------------------------

// Create a file for testing load functionality
// TODO this should be a general testing utility (e.g. testutils.h)
static void stringToFile(const std::string& fileName, const std::string& fileContent)
{
    std::ofstream outStream;
    outStream.open(fileName);
    outStream << fileContent;
    outStream.close();
}

// wraps file removal functionality
// TODO this should be a general testing utility (e.g. testutils.h)
static void removeFile(const std::string& referenceName)
{
    std::remove(referenceName.c_str());
}

// Loads contents of a file to a string
// TODO this is duplicated in the TextBuffer class and should be in e.g. a stringutils header
static std::string fileToString(const std::string& fileName)
{
    std::ifstream fileStream(fileName);
    std::stringstream stringStream;
    stringStream << fileStream.rdbuf();
    return stringStream.str();
}

// Compares string content of two files
// TODO this should be a general testing utility (e.g. testutils.h)
static bool compareFileContents(const std::string& file1, const std::string& file2)
{
    auto string1 = fileToString(file1);
    auto string2 = fileToString(file2);
    return (string1 == string2);
}

// Convenience function to set up a TextBuffer with a specific text content
static TextBuffer genTextBuffer(const std::string& content)
{
    stringToFile("temp.txt", content);
    TextBuffer buf("temp.txt");
    removeFile("temp.txt");
    return buf;
}
// -----------------------------------------------------------------------------------
// Tests for TextBuffer
// -----------------------------------------------------------------------------------
TEST_CASE("TextBuffer loads")
{
    const std::string referenceString = "Hello world\nSecond line";
    const std::string loadFilePath = "sample_input.txt";
    stringToFile(loadFilePath, referenceString);
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

    SECTION("Throws on failed load")
    {

    }
    removeFile(loadFilePath);
}

TEST_CASE("Textbuffer writes")
{
    const std::string referenceString = "Test write reference";
    const std::string referencePath = "reference.txt";
    const std::string writePath = "write_output.txt";
    stringToFile(referencePath, referenceString);

    TextBuffer buffer(referencePath);
    buffer.saveToText(writePath);

    REQUIRE(compareFileContents(referencePath, writePath));

    removeFile(referencePath);
    removeFile(writePath);
}

TEST_CASE("Textbuffer appending")
{
    std::string startString = "text to append";
    auto buffer = genTextBuffer(startString);
    buffer.appendSubstring(" - appended");
    REQUIRE(buffer.getString() == "text to append - appended");
}

TEST_CASE("TextBuffer insertion")
{
    auto buffer = genTextBuffer("word1 word2");

    SECTION("Inserts correctly")
    {
        buffer.insertSubstring(" word3", 5);
        REQUIRE(buffer.getString() == "word1 word3 word2");
    }

    SECTION("Throws with invalid index")
    {
        REQUIRE_THROWS_AS(buffer.insertSubstring("should throw", 30), std::length_error);
    }
}

TEST_CASE("TextBuffer deleting")
{
    auto buffer = genTextBuffer("word1 word2");

    SECTION("Delete from end")
    {
        buffer.eraseTrailingCharacters(6);
        REQUIRE(buffer.getString() == "word1");
    }

    SECTION("Delete from index")
    {
        buffer.eraseCharacters(5, 3);
        REQUIRE(buffer.getString() == "word1rd2");
    }

    SECTION("Delete past 0")
    {
        buffer.eraseCharacters(5, 10);
        REQUIRE(buffer.getString() == "word1");
    }


    SECTION("Throw with incorrect index")
    {
        REQUIRE_THROWS_AS(buffer.eraseCharacters(30, 5), std::length_error);
    }

    SECTION("Delete entire string")
    {
        buffer.eraseTrailingCharacters(20);
        REQUIRE(buffer.getString().empty());
    }
}

TEST_CASE("TextBuffer substring replace")
{
    auto buffer = genTextBuffer("string1 string2 string3");
    buffer.replaceSubstring("string2", "replacedstring");
    REQUIRE(buffer.getString() == "string1 replacedstring string3");
}


TEST_CASE("TextBuffer buffering operations work")
{
    std::string initString = "string1 string2 string3";
    auto buffer = genTextBuffer(initString);
    SECTION("Undo, redo append")
    {
        buffer.appendSubstring(" string4");
        buffer.undo();
        REQUIRE(buffer.getString() == initString);
        buffer.redo();
        REQUIRE(buffer.getString() == "string1 string2 string3 string4");
    }

    SECTION("Undo, redo insert")
    {

    }

    SECTION("Undo, redo delete from index")
    {

    }

    SECTION("Undo, redo delete from end")
    {

    }

    SECTION("Undo, redo substring replacement")
    {

    }
}

TEST_CASE("undo and redo combinations")
{
    SECTION("Disallow empty redo")
    {

    }

    SECTION("Undo, undo, redo")
    {

    }

    SECTION("Undo")

    SECTION("Reached max undos")
    {

    }

    SECTION("Reached max redos")
    {

    }
}