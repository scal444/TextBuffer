/* Defines TextBuffer class
 *
 *
 */

#ifndef TEXTBUFFER_TEXTBUFFER_H
#define TEXTBUFFER_TEXTBUFFER_H

#include <deque>
#include <string>

class Patch;

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
        void insertSubstring(std::string subString, unsigned insertionIndex);
        //! \brief Inserts substring at end of current string
        void appendSubstring(std::string subString);
        //! \brief Erase n characters starting from selected index
        void eraseCharacters(unsigned deletionIndex, unsigned deletionLength);
        //! \brief Erase n characters from end of string
        void eraseTrailingCharacters(unsigned deletionLength);
        //! \brief Finds first instance of a substring within a string, replaces with second string
        //! Does nothing if string to substitute does not exist.
        void replaceSubstring(std::string stringToBeReplaced,
                              std::string stringToReplaceWith);
        //! Saves current string to text file
        void saveToText(std::string fileName);
        //! Loads string from text file. Replaces current string
        void loadFromText(std::string fileName);
        //! \brief Undo last operation
        void undo();
        //! \brief Redo last undone operation
        void redo();
        //! \brief Get copy of text buffer
        std::string getString() const {return text_;};
    private:
        std::string text_;
        std::deque<Patch *> patchesForUndo_;
        std::deque<Patch *> patchesForRedo_;
};

#endif //TEXTBUFFER_TEXTBUFFER_H
