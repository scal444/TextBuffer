//
// Created by kevin on 2/17/19.
//

#ifndef TEXTBUFFER_PATCH_H
#define TEXTBUFFER_PATCH_H


#include <string>

#include "stringutil.h"

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
    InsertionPatch(const unsigned insertionIndex, const std::string& insertionSubstring)
            : insertionIndex_(insertionIndex), insertionSubstring_(insertionSubstring)
    {}
    InsertionPatch(const InsertionPatch& rhs) = default;
    void apply(std::string& stringToPatch) override
    {
        stringutil::insertCharacters(stringToPatch, insertionIndex_, insertionSubstring_);
    }
private:
    unsigned insertionIndex_;
    std::string insertionSubstring_;
};

class DeletionPatch : public Patch
{
public:
    DeletionPatch(const unsigned deletionIndex, const unsigned nCharsToDelete)
            : deletionIndex_(deletionIndex), nCharsToDelete_(nCharsToDelete)
    {}
    DeletionPatch(const DeletionPatch& rhs) = default;
    void apply(std::string& stringToPatch) override
    {
        stringutil::deleteCharacters(stringToPatch, deletionIndex_, nCharsToDelete_);
    }
private:
    unsigned deletionIndex_;
    unsigned nCharsToDelete_;
};

class ReplacementPatch : public Patch
{
public:
    ReplacementPatch(const std::string& substringToBeReplaced, const std::string& substringToReplaceWith)
            : substringToBeReplaced_(substringToBeReplaced), substringToReplaceWith_(substringToReplaceWith)
    {}
    ReplacementPatch(const ReplacementPatch& rhs) = default;
    void apply(std::string& stringToPatch) override
    {
        stringutil::replaceSubstring(stringToPatch, substringToBeReplaced_, substringToReplaceWith_);
    }
private:
    std::string substringToBeReplaced_;
    std::string substringToReplaceWith_;
};


#endif //TEXTBUFFER_PATCH_H
