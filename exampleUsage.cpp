#include "TextBuffer.h"
#include "bufferUtils.h"
#include <iostream>
int main()
{

    auto buffer = genTextBuffer("initial text");
    std::cout << "Initial text:\n " << buffer.getString() << "\n";

    buffer.appendSubstring(" - appended text");
    std::cout << "Command = buffer.appendSubstring( - appended text):\n";
    std::cout << "Buffer contains : " << buffer.getString() << "\n";

    buffer.eraseCharacters(0, 8);
    std::cout << "Command = buffer.eraseCharacters(0, 8):\n";
    std::cout << "Buffer contains : " << buffer.getString() << "\n";

    buffer.undo();
    std::cout << "Command = buffer.undo():\n";
    std::cout << "Buffer contains : " << buffer.getString() << "\n";

    buffer.redo();
    std::cout << "Command = buffer.redo():\n";
    std::cout << "Buffer contains : " << buffer.getString() << "\n";
    return 0;
}