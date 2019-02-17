// implements functionality declared in TextBuffer.h

#include "TextBuffer.h"
#include "stringutil.h"
#include <deque>
#include <fstream>
#include <sstream>
#include <stdexcept>



TextBuffer::TextBuffer(const std::string fileName)
{
    loadFromText(fileName);
}

void TextBuffer::insertSubstring(const std::string subString, const unsigned insertionIndex)
{
    if (insertionIndex > text_.size())
    {
        throw std::length_error("Attempted to insert past index bounds");
    }
    stringutil::insertCharacters(text_, insertionIndex, subString);
    auto patch = std::make_unique<DeletionPatch>(insertionIndex, subString.size());
    patchesForUndo_.push_front(std::move(patch));
    checkAndMaintainBufferSize();
}

// Implemented in terms of insertSubstring to avoid duplication
void TextBuffer::appendSubstring(const std::string subString)
{
    const auto insertionPoint = text_.size();
    insertSubstring(subString, static_cast<int>(insertionPoint));
}
void TextBuffer::eraseCharacters(const unsigned deletionIndex, const unsigned deletionLength)
{
    if (deletionIndex > text_.size())
    {
        throw std::length_error("Attempted to delete past index bounds");
    }
    const auto textToErase = text_.substr(deletionIndex, deletionLength);
    stringutil::deleteCharacters(text_, deletionIndex, deletionLength);
    auto patch = std::make_unique<InsertionPatch>(deletionIndex, textToErase);
    patchesForUndo_.push_front(std::move(patch));

}
void TextBuffer::eraseTrailingCharacters(const unsigned deletionLength)
{
    if (deletionLength >= text_.size())
    {
        auto stringToDelete = text_;
        text_.clear();
        auto patch = std::make_unique<InsertionPatch>(0, stringToDelete);
        patchesForUndo_.push_front(std::move(patch));
    }
    else
    {
        eraseCharacters(static_cast<unsigned>(text_.size()) - deletionLength, deletionLength);
    }
}
void TextBuffer::replaceSubstring(const std::string stringToBeReplaced,
                                  const std::string stringToReplaceWith)
{
    auto subStringPosition = stringutil::replaceSubstring(text_, stringToBeReplaced, stringToReplaceWith);
    if (subStringPosition > 0)
    {
        // For patch, do reverse replacement
        auto patch = std::make_unique<ReplacementPatch>(stringToReplaceWith, stringToBeReplaced);
        patchesForUndo_.push_front(std::move(patch));
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
    if (!patchesForUndo_.empty())
    {
        auto &patchToApply = patchesForUndo_.front();
        patchToApply->apply(text_);
        patchesForRedo_.push_front(std::move(*patchesForUndo_.begin()));
        patchesForUndo_.pop_front();
    }
}

void TextBuffer::redo()
{
    if (!patchesForRedo_.empty())
    {
        auto &patchToApply = patchesForRedo_.front();
        patchToApply->apply(text_);
        patchesForUndo_.push_front(std::move(*patchesForRedo_.begin()));
        patchesForRedo_.pop_front();
        checkAndMaintainBufferSize();
    }
}

void TextBuffer::checkAndMaintainBufferSize()
{
    if (patchesForUndo_.size() > maxBufferPatchNumber_)
    {
        patchesForUndo_.pop_back();
    }
}
