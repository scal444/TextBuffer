//
// Created by kevin on 2/17/19.
//

#ifndef TEXTBUFFER_PATCH_H
#define TEXTBUFFER_PATCH_H

#include <memory>
#include <string>

#include "stringutil.h"

class Patch
{
public:
    Patch() = default;
    virtual ~Patch() = default;
    virtual void apply(std::string& stringToPatch) = 0;
    virtual std::unique_ptr<Patch> reverse() const = 0;
};

class InsertionPatch : public Patch
{
public:
    InsertionPatch(const unsigned insertionIndex, std::string insertionSubstring)
            : insertionIndex_(insertionIndex), insertionSubstring_(std::move(insertionSubstring))
    {}
    InsertionPatch(const InsertionPatch& rhs) = default;
    void apply(std::string& stringToPatch) override
    {
        stringutil::insertCharacters(stringToPatch, insertionIndex_, insertionSubstring_);
    }
    std::unique_ptr<Patch> reverse() const override;

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
        deletedString_ = stringToPatch.substr(deletionIndex_, nCharsToDelete_);
        stringutil::deleteCharacters(stringToPatch, deletionIndex_, nCharsToDelete_);
    }

    std::unique_ptr<Patch> reverse() const override
    {
        return std::make_unique<InsertionPatch>(deletionIndex_, deletedString_);
    }
private:
    std::string deletedString_;
    unsigned deletionIndex_;
    unsigned nCharsToDelete_;
};

class ReplacementPatch : public Patch
{
public:
    ReplacementPatch(std::string substringToBeReplaced, std::string substringToReplaceWith)
            : substringToBeReplaced_(std::move(substringToBeReplaced)),
              substringToReplaceWith_(std::move(substringToReplaceWith))
    {}
    ReplacementPatch(const ReplacementPatch& rhs) = default;
    void apply(std::string& stringToPatch) override
    {
        stringutil::replaceSubstring(stringToPatch, substringToBeReplaced_, substringToReplaceWith_);
    }
    std::unique_ptr<Patch> reverse() const override
    {
        return std::make_unique<ReplacementPatch>(substringToReplaceWith_, substringToBeReplaced_);
    }
private:
    std::string substringToBeReplaced_;
    std::string substringToReplaceWith_;
};


inline std::unique_ptr<Patch> InsertionPatch::reverse() const
{
    return std::make_unique<DeletionPatch>(insertionIndex_, insertionSubstring_.size());
}

#endif //TEXTBUFFER_PATCH_H
