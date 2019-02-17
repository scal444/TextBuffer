/* Defines TextBuffer class
 *
 *
 */

#ifndef TEXTBUFFER_TEXTBUFFER_H
#define TEXTBUFFER_TEXTBUFFER_H

#include <deque>
#include <memory>
#include <string>

#include "Patch.h"


class TextBuffer
{
    // FIXME This should be larger, and/or an optional parameter of the constructor.
    // Using a constant for now to keep the requested interface
    static constexpr int maxBufferPatchNumber_ = 5;

    public:
        TextBuffer() = default;
        //! \brief Load text from file
        explicit TextBuffer(std::string fileName);
        //! \brief Inserts substring at the specified string index
        //! \throws length_error if index is out of bounds
        void insertSubstring(const unsigned insertionIndex, const std::string subString);
        //! \brief Inserts substring at end of current string
        void appendSubstring(std::string subString);
        //! \brief Erase n characters starting from selected index
        //! Handles deletion lengths greater than the remaining string size
        //! \throws lengtH_error if index is out of bounds
        void eraseCharacters(unsigned deletionIndex, unsigned deletionLength);
        //! \brief Erase n characters from end of string.
        //! If the deletion length is greater than the string size, clears the string
        void eraseTrailingCharacters(unsigned deletionLength);
        //! \brief Finds first instance of a substring within a string, replaces with second string
        //! Does nothing if string to substitute does not exist.
        void replaceSubstring(std::string stringToBeReplaced,
                              std::string stringToReplaceWith);
        //! Saves current string to text file
        void saveToText(std::string fileName);
        //! Loads string from text file. Replaces current string
        void loadFromText(std::string fileName);
        //! \brief Undo last operation. Does nothing if no operations are in the undo buffer
        void undo();
        //! \brief Redo last undone operation. Does nothing of no operations are in the redo buffer
        void redo();
        //! \brief Get copy of text
        std::string getString() const {return text_;};
    private:
        //! \Brief Prevent deques from overflowing maximum size by popping off the least recent patch
        //! if undo buffer has reached maximum size.
        void checkAndMaintainBufferSize();
        std::string text_;
        std::deque<std::unique_ptr<Patch>> patchesForUndo_;
        std::deque<std::unique_ptr<Patch>> patchesForRedo_;
};

#endif //TEXTBUFFER_TEXTBUFFER_H
