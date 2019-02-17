/* unit tests for TextBuffer class */

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "TextBuffer.h"
#include "bufferUtils.h"


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
        buffer.insertSubstring(5, " word3");
        REQUIRE(buffer.getString() == "word1 word3 word2");
    }

    SECTION("Throws with invalid index")
    {
        REQUIRE_THROWS_AS(buffer.insertSubstring(30, "should throw"), std::length_error);
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
        buffer.insertSubstring(0, "string4 ");
        buffer.undo();
        REQUIRE(buffer.getString() == initString);
        buffer.redo();
        REQUIRE(buffer.getString() == "string4 string1 string2 string3");
    }

    SECTION("Undo, redo delete from index")
    {
        buffer.eraseCharacters(8, 8);
        buffer.undo();
        REQUIRE(buffer.getString() == initString);
        buffer.redo();
        REQUIRE(buffer.getString() == "string1 string3");
    }

    SECTION("Undo, redo delete from end")
    {
        buffer.eraseTrailingCharacters(8);
        buffer.undo();
        REQUIRE(buffer.getString() == initString);
        buffer.redo();
        REQUIRE(buffer.getString() == "string1 string2");
    }

    SECTION("Undo, redo substring replacement")
    {
        buffer.replaceSubstring("string2", "string4");
        buffer.undo();
        REQUIRE(buffer.getString() == initString);
        buffer.redo();
        REQUIRE(buffer.getString() == "string1 string4 string3");
    }
}

TEST_CASE("undo and redo combinations")
{
    std::string initString = "string1 string2 string3";
    auto buffer = genTextBuffer(initString);
    SECTION("Handle empty redo")
    {
        REQUIRE_NOTHROW(buffer.undo());
        REQUIRE(buffer.getString() == initString);
    }

    SECTION("Handle empty undo")
    {
        REQUIRE_NOTHROW(buffer.redo());
        REQUIRE(buffer.getString() == initString);
    }

    SECTION("Chaining undos and redos")
    {
        buffer.appendSubstring(" string4");
        buffer.eraseCharacters(0, 8);
        REQUIRE(buffer.getString() == "string2 string3 string4");
        buffer.undo();
        REQUIRE(buffer.getString() == "string1 string2 string3 string4");
        buffer.redo();
        REQUIRE(buffer.getString() == "string2 string3 string4");
        buffer.undo();
        REQUIRE(buffer.getString() == "string1 string2 string3 string4");
        buffer.undo();
        REQUIRE(buffer.getString() == initString);
    }

    SECTION("Reached max undos")
    {
        for (unsigned i = 0; i < 10; i++)
        {
            buffer.eraseTrailingCharacters(1);
        }
        // In our implementation, max buffer size is 5, so only 5 undos should happen
        for (unsigned i = 0; i < 10; i++)
        {
            buffer.undo();
        }
        REQUIRE(buffer.getString() == "string1 string2 st");

    }

}