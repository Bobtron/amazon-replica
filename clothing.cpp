#include "product.h"
#include "util.h"
#include <vector>
#include <string>
#include <set>
#include <sstream>
#include <iomanip>
#include "clothing.h"

using namespace std;

Clothing::Clothing(const string category, const string name, double price, int qty, string size, string brand)
        : Product(category, name, price, qty)
{
    size_ = size;
    brand_ = brand;
}

Clothing::~Clothing(){

}

/**
 * Returns the appropriate keywords that this product should be associated with
 */
set<string> Clothing::keywords() const{
    set<string> returnThis = parseStringToWords(name_);
    set<string> brand = parseStringToWords(brand_);
    returnThis.insert(brand.begin(), brand.end());
    return returnThis;
}

/**
 * Allows for a more detailed search beyond simple keywords
 */
bool Clothing::isMatch(vector<string>& searchTerms) const{
    return false;
}

/**
 * Returns a string to display the product info for hits of the search
 */
string Clothing::displayString() const{
    stringstream ss;
    ss.precision(2);
    ss << name_ << "\nSize: " << size_ + " Brand: " << brand_ + "\n";
    ss << showpoint << fixed << price_ << " ";
    ss << qty_ << " left.\nRating: ";
    ss << getAvgRating();
    return ss.str();
}

/**
 * Outputs the product info in the database format
 */
void Clothing::dump(ostream& os) const{
    os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ << "\n" << size_ << "\n" << brand_ << endl;
}
