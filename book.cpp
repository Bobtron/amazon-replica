#include <sstream>
#include <iomanip>
#include "book.h"
#include "product.h"
#include "util.h"
#include <vector>
#include <string>
#include <set>

using namespace std;

Book::Book(const string category, const string name, double price, int qty, string isbn, string author)
: Product(category, name, price, qty)
{
    isbn_ = isbn;
    author_ = author;
}

Book::~Book(){

}

/**
 * Returns the appropriate keywords that this product should be associated with
 */
set<string> Book::keywords() const{
    set<string> returnThis = parseStringToWords(name_);
    set<string> author = parseStringToWords(author_);
    returnThis.insert(author.begin(), author.end());
    returnThis.insert(isbn_);
    return returnThis;
}

/**
 * Allows for a more detailed search beyond simple keywords
 */
bool Book::isMatch(vector<string>& searchTerms) const{
    return false;
}

/**
 * Returns a string to display the product info for hits of the search
 */
string Book::displayString() const{
    stringstream ss;
    ss.precision(2);
    ss << name_ << "\nAuthor: " << author_ << " ISBN: " << isbn_ << "\n";
    ss << showpoint << fixed << price_ << " ";
    ss << qty_ << " left.\nRating: ";
    ss << getAvgRating();
    return ss.str();
}

/**
 * Outputs the product info in the database format
 */
void Book::dump(ostream& os) const{
    os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ << "\n" << isbn_ << "\n" << author_ << endl;
}