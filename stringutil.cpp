//
// Created by kevin on 2/17/19.
//

#include "stringutil.h"

namespace stringutil
{

void deleteCharacters(std::string &stringToModify,
                      const unsigned deletionIndex,
                      const unsigned deletionLength)
{
    stringToModify.erase(deletionIndex, deletionLength);
}

void insertCharacters(std::string &stringToModify,
                      const unsigned insertionIndex,
                      const std::string &stringToInsert)
{
    stringToModify.insert(insertionIndex, stringToInsert);
}


unsigned replaceSubstring(std::string &stringToModify,
                          const std::string &substringToBeReplaced,
                          const std::string &substringToReplaceWith)
{
    auto subStringPosition = static_cast<unsigned>(stringToModify.find(substringToBeReplaced));
    if (subStringPosition > 0)
    {
        deleteCharacters(stringToModify, subStringPosition, static_cast<unsigned>(substringToBeReplaced.size()));
        insertCharacters(stringToModify, subStringPosition, substringToReplaceWith);
    }
    return subStringPosition;

}
} // namespace stringutil
