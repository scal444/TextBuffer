//
// Created by kevin on 2/17/19.
//

#ifndef TEXTBUFFER_PATCH_H
#define TEXTBUFFER_PATCH_H

#include <memory>
#include <string>

#include "stringutil.h"

/*! /brief Base class for patch operations on TextBuffer
 *
 *  Patches contain state information that allow them to apply an operation
 *  to a string.
 *
 *  Each patch has a corresponding "reverse" patch that undoes the operation
 */
class Patch
{
public:
    Patch() = default;
    virtual ~Patch() = default;
    //! \brief apply operation to string
    virtual void apply(std::string& stringToPatch) = 0;
    //! \brief Creates patch that will exactly undo this patches operation
    virtual std::unique_ptr<Patch> reverse() const = 0;
};

//! \brief Patch inserts a substring at a location
//!
//! The reverse of this patch is a deletion
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
    // Defined further down when DeletionPatch is defined
    std::unique_ptr<Patch> reverse() const override;

private:
    unsigned insertionIndex_;
    std::string insertionSubstring_;
};

//! \brief Patch deletes a number of characters starting at an index
//!
//! The reverse of this patch is an insertion.
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

//! \brief Patch finds the first instance of a substring and replaces it with another substring
//!
//! The reverse of this patch is another ReplacementPatch with strings reversed
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
