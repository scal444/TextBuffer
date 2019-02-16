// implements functionality declared in TextBuffer.h

#include "TextBuffer.h"

#include <deque>
#include <fstream>
#include <sstream>

class Patch
{
    public:
        Patch() = default;
        virtual ~Patch() = default;
        virtual void apply(std::string& stringToPatch) = 0;
};

class InsertionPatch : public Patch
{
    public:
        InsertionPatch(unsigned insertionIndex, std::string&& insertionSubstring)
            : insertionIndex_(insertionIndex), insertionSubstring_(insertionSubstring)
        {}
        InsertionPatch(const InsertionPatch& rhs) = default;
        void apply(std::string& stringToPatch) override
        {

        }
    private:
        unsigned insertionIndex_;
        std::string insertionSubstring_;
};

class DeletionPatch : public Patch
{
    public:
        DeletionPatch(unsigned deletionIndex, unsigned nCharsToDelete)
            : deletionIndex_(deletionIndex), nCharsToDelete_(nCharsToDelete)
        {}
        DeletionPatch(const DeletionPatch& rhs) = default;
        void apply(std::string& stringToPatch) override
        {
            stringToPatch.erase(deletionIndex_, nCharsToDelete_);
        }
    private:
        unsigned deletionIndex_;
        unsigned nCharsToDelete_;
};

class ReplacementPatch : public Patch
{
    public:
        ReplacementPatch(std::string substringToBeReplaced, std::string substringToReplaceWith)
            : substringToBeReplaced_(substringToBeReplaced), substringToReplaceWith_(substringToReplaceWith)
        {}
        ReplacementPatch(const ReplacementPatch& rhs) = default;
        void apply(std::string& stringToPatch) override
        {

        }
    private:
        std::string substringToBeReplaced_;
        std::string substringToReplaceWith_;
};





TextBuffer::TextBuffer(const std::string fileName)
{
    loadFromText(fileName);
}

void TextBuffer::insertSubstring(const std::string subString, const unsigned insertionIndex)
{
    text_.insert(insertionIndex, subString);
}

void TextBuffer::appendSubstring(const std::string subString)
{
    text_.insert(text_.size(), subString);
}
void TextBuffer::eraseCharacters(const unsigned deletionIndex, const unsigned deletionLength)
{
    text_.erase(deletionIndex, deletionLength);
}
void TextBuffer::eraseTrailingCharacters(const unsigned deletionLength)
{
    if (deletionLength >= text_.size())
    {
        text_.clear();
    }
    else
    {
        eraseCharacters(static_cast<unsigned>(text_.size()) - deletionLength, deletionLength);
    }
}
void TextBuffer::replaceSubstring(const std::string stringToBeReplaced,
                                  const std::string stringToReplaceWith)
{
    auto subStringPosition = static_cast<unsigned>(text_.find(stringToBeReplaced));
    if (subStringPosition > 0)
    {
        eraseCharacters(subStringPosition, static_cast<unsigned>(stringToBeReplaced.size()));
        insertSubstring(stringToReplaceWith, subStringPosition);
    }

}
void TextBuffer::saveToText(const std::string fileName)
{
    std::ofstream outFile(fileName);
    outFile << text_;
    //TODO catch opening error
}

void TextBuffer::loadFromText(const std::string fileName)
{
    //TODO catch opening error
    std::ifstream fileStream(fileName);
    std::stringstream stringStream;
    stringStream << fileStream.rdbuf();
    text_ = stringStream.str();
}

void TextBuffer::undo()
{

}

void TextBuffer::redo()
{

}