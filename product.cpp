#include <sstream>
#include <iomanip>
#include <vector>
#include <map>
#include "review.h"
#include "product.h"
#include "msort.h"

using namespace std;

Product::Product(const std::string category, const std::string name, double price, int qty) :
    name_(name),
    price_(price),
    qty_(qty),
    category_(category),
    total_ratings_(0),
    num_ratings_(0)
{

}

Product::~Product()
{

}


double Product::getPrice() const
{
    return price_;
}

std::string Product::getName() const
{
    return name_;
}

void Product::subtractQty(int num)
{
    qty_ -= num;
}

int Product::getQty() const
{
    return qty_;
}

/**
 * default implementation...can be overriden in a future
 * assignment
 */
bool Product::isMatch(std::vector<std::string>& searchTerms) const
{
    return false;
}

void Product::dump(std::ostream& os) const
{
    os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ << endl;
}

vector<pair<User*, int>> Product::addReview(Review review) {
    vector<pair<User*, int>> returnThis;
    for(size_t i = 0; i < reviews_.size(); i++){
        returnThis.push_back(pair<User*,int>(reviews_[i].user, reviews_[i].rating));
    }
    reviews_.push_back(review);
    num_ratings_++;
    total_ratings_ += review.rating;
    return returnThis;
}

map<User*, int> Product::returnAllReviews() {
    map<User*, int> returnThis;
    for(size_t i = 0; i < reviews_.size(); i++){
        returnThis.insert(pair<User*, int>(reviews_[i].user, reviews_[i].rating));
    }
    return returnThis;
}

void Product::dumpReviews(std::ostream &os) {
    for(size_t i = 0; i < reviews_.size(); i++){
        Review r = reviews_[i];
        os << getName() << "\n" << r.rating << " " << r.username << " " << r.date << " " << r.reviewText << endl;
    }
}

double Product::getAvgRating() const{
    if(num_ratings_ == 0){
        return 0;
    }
    return (double)(total_ratings_) / (double)(num_ratings_);
}

struct DateComp{
    bool operator()(Review a, Review b)
    {
        if(a.year > b.year){
            return true;
        }else if(b.year > a.year){
            return false;
        }else{
            if(a.month > b.month){
                return true;
            }else if(b.month > a.month){
                return false;
            }else{
                if(a.day > b.day){
                    return true;
                }
                return false;
            }
        }
    }
};

void Product::printReviews(){
    DateComp comp;
    mergeSort(reviews_, comp);
    for(size_t i = 0; i < reviews_.size(); i++){
        cout << reviews_[i].date << " " << reviews_[i].rating << " " << reviews_[i].username << " " << reviews_[i].reviewText << endl;
    }
}



