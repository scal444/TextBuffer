//
// Created by kevin on 2/17/19.
//


#ifndef TEXTBUFFER_STRINGUTIL_H
#define TEXTBUFFER_STRINGUTIL_H

#include <string>


// Functions for string manipulation
// Given own namespace for clarity, because some patch and buffer operations have similar names
namespace stringutil
{
/* Deletes deletes n characters from string starting from deletionIndex */
void deleteCharacters(std::string &stringToModify,
                      unsigned deletionIndex,
                      unsigned deletionLength);

/* inserts substring starting from insertionIndex */
void insertCharacters(std::string &stringToModify,
                      unsigned insertionIndex,
                      const std::string &stringToInsert);

/* Finds first instance of substring within stringtomodify and replaces with new substring. Does nothing
* if substring does not exist.
* /returns index where substring was found, 0 if not found
*/
unsigned replaceSubstring(std::string &stringToModify,
                          const std::string &substringToBeReplaced,
                          const std::string &substringToReplaceWith);

} // namespace stringutil
#endif //TEXTBUFFER_STRINGUTIL_H
