#include <iostream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <regex>
#include <string>
#include <set>
#include "util.h"

using namespace std;

std::string convToLower(std::string src)
{
    std::transform(src.begin(), src.end(), src.begin(), ::tolower);
    return src;
}

/** Complete the code to convert a string containing a rawWord
    to a set of words based on the criteria given in the assignment **/
std::set<std::string> parseStringToWords(string rawWords)
{
    /*
     * How to use regular expressions within C++
     * i.e. the implementation specific to each language
     * !!!NOTHING ELSE!!!
     * Is paraphrased from
     * https://regular-expressions.mobi/stdregex.html?wlr=1
     * under "Finding All Regex Matches"
     * (I have used regex before but only in java/python)
     */
    set<string> returnThis;
    regex re("([A-Za-z]{2,})+");
    sregex_iterator next(rawWords.begin(), rawWords.end(), re);
    sregex_iterator stop;
    while(next != stop){
        smatch match = *next;
        returnThis.insert(convToLower(match.str()));
        next++;
    }
    return returnThis;
}

/**************************************************
 * COMPLETED - You may use the following functions
 **************************************************/

// Used from http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// trim from start
std::string &ltrim(std::string &s) {
    s.erase(s.begin(), 
	    std::find_if(s.begin(), 
			 s.end(), 
			 std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
std::string &rtrim(std::string &s) {
    s.erase(
	    std::find_if(s.rbegin(), 
			 s.rend(), 
			 std::not1(std::ptr_fun<int, int>(std::isspace))).base(), 
	    s.end());
    return s;
}

// trim from both ends
std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}
