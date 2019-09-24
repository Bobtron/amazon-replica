#include "product.h"
#include "util.h"
#include <vector>
#include <string>
#include <set>
#include <sstream>
#include <iomanip>
#include "movie.h"

using namespace std;

Movie::Movie(const string category, const string name, double price, int qty, string genre, string rating)
        : Product(category, name, price, qty)
{
    genre_ = genre;
    rating_ = rating;
}

Movie::~Movie(){

}

/**
 * Returns the appropriate keywords that this product should be associated with
 */
set<string> Movie::keywords() const{
    set<string> returnThis = parseStringToWords(name_);
    set<string> genre = parseStringToWords(genre_);
    returnThis.insert(genre.begin(), genre.end());
    return returnThis;
}

/**
 * Allows for a more detailed search beyond simple keywords
 */
bool Movie::isMatch(vector<string>& searchTerms) const{
    return false;
}

/**
 * Returns a string to display the product info for hits of the search
 */
string Movie::displayString() const{
    stringstream ss;
    ss.precision(2);
    ss << name_ << "\nGenre: " << genre_ << " Rating: " + rating_ << "\n";
    ss << showpoint << fixed << price_ << " ";
    ss << qty_ << " left.\nRating: ";
    ss << getAvgRating();
    return ss.str();
}

/**
 * Outputs the product info in the database format
 */
void Movie::dump(ostream& os) const{
    os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ << "\n" << genre_ << "\n" << rating_ << endl;
}