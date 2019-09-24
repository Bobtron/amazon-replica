//
// Created by Persona Non Grata on 2/18/19.
//

#include <string>
#include <set>
#include <vector>
#include <map>
#include <iostream>
#include <regex>
#include <cmath>
#include <cstdlib>
#include <set>
#include <algorithm> ///only for stable sort
#include <iomanip>
#include <sstream>
#include <fstream>
#include "product.h"
#include "user.h"
#include "mydatastore.h"
#include "util.h"
#include "review.h"
#include "msort.h"
#include "heap.h"

using namespace std;

MyDataStore::MyDataStore(){
    loggedIn = nullptr;
    index = 0;
    initialized = false;
}
MyDataStore::~MyDataStore(){}

//returns the corresponding products given a keyword
set<Product*> MyDataStore::getProducts(string term){
    set<Product*> returnThis;
    if(productKeywordDatabase.find(term) != productKeywordDatabase.end()){
        returnThis = productKeywordDatabase.find(term)->second;
    }
    return returnThis;
}

/**
 * Adds a product to the data store
 */
void MyDataStore::addProduct(Product* p){
    //adds a product
    //in the map puts all the keywords corresponding to that product
    //as keys and sets the value as a set containing that product
    //as a pointer
    products.insert(p);
    if(productNameDatabase.find(p->getName()) == productNameDatabase.end()){
        productNameDatabase.insert(pair<string, Product*>(p->getName(), p));
    }
    set<string> keywords = p->keywords();
    for(set<string>::iterator it = keywords.begin(); it != keywords.end(); ++it){
        if(productKeywordDatabase.find(*it) == productKeywordDatabase.end()) {
            set<Product*> addThis;
            addThis.insert(p);
            productKeywordDatabase.insert(pair<string, set<Product*> >(*it, addThis));
        }else{
            productKeywordDatabase.find(*it)->second.insert(p);
        }
    }
}

/**
 * Adds a user to the data store
 */
void MyDataStore::addUser(User* u){
    //adds user to set of users, and also to the maps
    //containing the carts for the sure and
    //the map matching a username to the user
    vector<Product*> empty;
    usernameToCart.insert(pair<string, vector<Product*> >(u->getName(), empty));
    users.insert(u);
    usernameToUser.insert(pair<string, User*>(u->getName(), u));
    indexToUser.insert(pair<int, User*>(index, u));
    userToIndex.insert(pair<User*, int>(u, index));
    index += 1;
}

/*
 * Below are the comparator functors needed for implmenting the search
 * functions
 */

struct RatingComp{
    bool operator()(Product* a, Product* b)
    {
        // Complete the code
        return a->getAvgRating() > b->getAvgRating();
    }
};

struct NameComp{
    bool operator()(Product* a, Product* b)
    {
        // Complete the code
        return a->getName() < b->getName();
    }
};

/**
 * Performs a search of products whose keywords match the given "terms"
 *  type 0 = AND search (intersection of results for each term) while
 *  type 1 = OR search (union of results for each term)
 */
vector<Product*> MyDataStore::search(vector<string>& terms, int type) {
    vector<Product*> returnThis;
    //first gets the products corresponding to the search terms and
    //the search type
    bool isRating;

    if(terms.size() < 2){
        return returnThis;
    }

    if(terms[0] == "r"){
        isRating = true;
    }else if(terms[0] == "n"){
        isRating = false;
    }else{
        return returnThis;
    }

    terms.erase(terms.begin());

    set<Product*> temp;
    if (type == 0) {
        temp = getProducts(terms[0]);
        for (size_t i = 1; i < terms.size(); i++) {
            set<Product*> hits = getProducts(terms[i]);
            temp = setIntersection(temp, hits);
        }
    }else {
        for (size_t i = 0; i < terms.size(); i++) {
            set<Product*> hits = getProducts(terms[i]);
            temp = setUnion(temp, hits);
        }
    }


    //then takes the products from map and sticks them into a vector to be returned
    //and also a map that matches the hit number with the product
    searchResults.clear();
    for(set<Product*>::iterator it = temp.begin(); it != temp.end(); ++it){
        returnThis.push_back(*it);
    }

    if(isRating) {
        RatingComp comp;
        mergeSort(returnThis, comp);
    }else{
        NameComp comp;
        mergeSort(returnThis, comp);
    }

    for(size_t i = 0; i < returnThis.size(); i++){
        searchResults.insert(pair<int, Product*>(i + 1, returnThis[i]));
    }
    return returnThis;
}

/**
     * Adds a given item to the cart given its index of the search position
     */
void MyDataStore::addingToCart(int resultIndex){
    //gets the cart given the username
    //and gets the item given the resultindex
    //and if allso good sticks in at the end of the cart
    if(loggedIn != nullptr){
        if(searchResults.find(resultIndex) != searchResults.end()){
            usernameToCart.find(loggedIn->getName())->second.push_back(searchResults.find(resultIndex)->second);
        }else{
            cout << "Invalid request" << endl;
        }
    }else{
        cout << "No current user" << endl;
    }
}

/**
 * Prints the cart
 */
void MyDataStore::viewingToCart(){
    //goes thru the cart and prints things out
    if(loggedIn != nullptr){
        vector<Product*> products = usernameToCart.find(loggedIn->getName())->second;
        for(size_t i = 0; i < products.size(); i++){
            cout << "Item " << i + 1 << endl << products[i]->displayString() << endl;
        }
    }else{
        cout << "No current user" << endl;
    }
}

/**
 * Buys the shit in the cart from bezos
 */
void MyDataStore::buyingToCart(){
    //goes thru the cart and tries to buy it
    //if possible, takes one away from the quantity, then takes the money
    //from the user and removes it from the cart
    if(loggedIn != nullptr){
        for(size_t i = 0; i < usernameToCart.find(loggedIn->getName())->second.size(); i++){
            Product* current = usernameToCart.find(loggedIn->getName())->second[i];
            if(current->getQty() > 0 and loggedIn->getBalance() >= current->getPrice()){
                current->subtractQty(1);
                loggedIn->deductAmount(current->getPrice());
                usernameToCart.find(loggedIn->getName())->second.erase(usernameToCart.find(loggedIn->getName())->second.begin() + i--);
            }
        }
    }else{
        cout << "No current user" << endl;
    }
}

/**
 * Reproduce the database file from the current Products and User values
 */
void MyDataStore::dump(ostream& ofile){
    //straightforward, prints the products, then tells the products
    //and users to print themselves
    ofile << "<products>" << endl;
    for(set<Product*>::iterator it = products.begin(); it != products.end(); ++it){
        (*it)->dump(ofile);
    }
    ofile << "</products>\n<users>" << endl;
    for(set<User*>::iterator it = users.begin(); it != users.end(); ++it){
        (*it)->dump(ofile);
    }
    ofile << "</users>\n<reviews>" << endl;
    for(set<Product*>::iterator it = products.begin(); it != products.end(); ++it){
        (*it)->dumpReviews(ofile);
    }
    ofile << "</reviews>" << endl;
}

void MyDataStore::addReview(const std::string& prodName,
               int rating,
               const std::string& username,
               const std::string& date,
               const std::string& review_text){
    if(not initialized){
        initializeSimilarityMatrix();
        initialized = true;
    }

    vector<int> monthLengths = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    regex r("^\\d{4}(?:-\\d\\d){2}$");
    //the regex checks if its in a yyyy-mm-dd match
    //and the regex_search checks that the date
    //matches with the pattern
    if (rating <= 5 and rating >= 1 and regex_search(date, r)){
        //now to verify the date
        //gets the months and days and verifies that they are within the limits
        int month = stoi(date.substr(5, 2));
        int day = stoi(date.substr(8, 2));
        if(month <= 12 and month >= 1) {
            if(day >= 1 and day <= monthLengths[month - 1]) {
                if(usernameToUser.find(username) != usernameToUser.end()) {
                    User* user = usernameToUser.find(username)->second;
                    Review r(rating, username, date, review_text, user);
                    if (productNameDatabase.find(prodName) != productNameDatabase.end()) {
                        ///
                        vector<pair<User*, int>> addSimilarities = productNameDatabase.find(prodName)->second->addReview(r);
                        for(size_t i = 0; i < addSimilarities.size(); i++){
                            User* oldUser = addSimilarities[i].first;
                            int oldRating = addSimilarities[i].second;
                            int newUserIndex = userToIndex.find(user)->second;
                            int oldUserIndex = userToIndex.find(oldUser)->second;
                            if(numCorresRev[newUserIndex][oldUserIndex] == 0){
                                totalRatingRev[newUserIndex][oldUserIndex] = 0;
                                totalRatingRev[oldUserIndex][newUserIndex] = 0;
                            }
                            numCorresRev[newUserIndex][oldUserIndex]++;
                            numCorresRev[oldUserIndex][newUserIndex]++;
                            double similarityScore = (double)(abs(rating - oldRating)) / 4.0;
                            totalRatingRev[newUserIndex][oldUserIndex] += similarityScore;
                            totalRatingRev[oldUserIndex][newUserIndex] += similarityScore;
                            avgRatingRev[newUserIndex][oldUserIndex] = totalRatingRev[newUserIndex][oldUserIndex] / numCorresRev[newUserIndex][oldUserIndex];
                            avgRatingRev[oldUserIndex][newUserIndex] = avgRatingRev[newUserIndex][oldUserIndex];
                        }
                    }
                }
            }
        }
    }
}

void MyDataStore::login(std::string username, string password){
    if(usernameToUser.find(username) != usernameToUser.end()){
        if(usernameToUser.find(username)->second->login(password)) {
            loggedIn = usernameToUser.find(username)->second;
            cout << "User logged in" << endl;
        }else{
            cout << "Invalid login credentials" << endl;
        }
    }else{
        cout << "Invalid login credentials" << endl;
    }
}

void MyDataStore::logout(){
    loggedIn = nullptr;
}

void MyDataStore::addReview(int searchHitNum,
               int rating,
               const std::string& date,
               const std::string& review_text){
    if(loggedIn != nullptr){
        if(searchResults.find(searchHitNum) != searchResults.end()){
            addReview(searchResults.find(searchHitNum)->second->getName(), rating, loggedIn->getName(), date, review_text);
        }
    }
}

void MyDataStore::viewReview(int searchHitNum){
    if(searchResults.find(searchHitNum) != searchResults.end()){
        searchResults.find(searchHitNum)->second->printReviews();
    }
}

void MyDataStore::rec() {
    ///Checks that someone is logged in
    if(loggedIn == nullptr){
        cout << "No recommendations available" << endl;
        return;
    }
    ///if so, gets the suggestions then outputs to file
    vector<pair<string, double> > results = makeSuggestion(loggedIn->getName());
    if(results.size() == 0){
        cout << "No recommendations available" << endl;
        return;
    }

    string str = "rec.txt";
    ofstream ofile(str.c_str());

    stringstream ss;
    ss.precision(2);
    ss << loggedIn->getName() << endl;

    for(size_t i = 0; i < results.size(); i++){
        ss << fixed << results[i].second << " " << results[i].first << endl;
    }

    ofile << ss.str();
    cout << ss.str();

    ofile.close();
}

///these comparators compare either putting the smalles string first
bool compare_as_strings(pair<string, double> i, pair<string, double> j){
    return i.first < j.first;
}
///or by putting the highest weight first
bool compare_as_weights(pair<string, double> i, pair<string, double> j){
    return i.second > j.second;
}

vector<pair<string, double> > MyDataStore::makeSuggestion(string currentUser){
    vector<pair<string, double> > returnThis;
    if(usernameToUser.find(currentUser) != usernameToUser.end()) {
        ///heap is now used for djistras algorithm
        ///users are represented as integers
        int logInIndex = userToIndex.find(usernameToUser.find(currentUser)->second)->second;
        Heap<int, equal_to < int>, less < double >, hash < int >> h;
        vector<double> results;
        for (size_t i = 0; i < avgRatingRev[logInIndex].size(); i++) {
            results.push_back(avgRatingRev[logInIndex][i]);
            h.push(avgRatingRev[logInIndex][i], i);
        }
        while (not h.empty()) {
            int shortest = h.top();
            h.pop();
            for (size_t i = 0; i < avgRatingRev[shortest].size(); i++) {
                ///if we find a shorter path, then we will amend the key to become
                ///the shorter path, and then put the shorter path in the results
                if (results[i] > avgRatingRev[shortest][i] + results[shortest]) {
                    h.decreaseKey(avgRatingRev[shortest][i] + results[shortest], i);
                    results[i] = avgRatingRev[shortest][i] + results[shortest];
                }
            }
        }

        ///now we iterate over all the products to calculate the score for each one
        ///since we already have the similarites calculated
        for(set<Product*>::iterator it = products.begin(); it != products.end(); ++it){
            Product* curr = *it;
            map<User*, int> allReviews = curr->returnAllReviews();
            ///each products keeps track of the user, and their respective review
            if(allReviews.find(indexToUser.find(logInIndex)->second) == allReviews.end()){
                double rp = 0;
                double w = 0;
                ///then product iterates over all the reviews and adds the respective score
                ///to the weighted calculation
                for(map<User*,int>::iterator jt = allReviews.begin(); jt != allReviews.end(); ++jt){
                    int indexB = userToIndex.find(jt->first)->second;
                    rp += (1 - results[indexB]) * jt->second;
                    w += (1 - results[indexB]);
                }
                if(w == 0){
                    returnThis.push_back(pair<string,double>(curr->getName(), 0.0));
                }else{
                    returnThis.push_back(pair<string,double>(curr->getName(), rp/w));
                }
            }
        }
    }

    ///sorts by the string length first just so then if the weights are the same
    ///stable sorts will guarantee that the strings are already in alphabetical order
    stable_sort(returnThis.begin(), returnThis.end(), compare_as_strings);
    stable_sort(returnThis.begin(), returnThis.end(), compare_as_weights);

    return returnThis;
}

void MyDataStore::initializeSimilarityMatrix() {
  ///ok i concede that variable names need work
  ///but all this does is that it initializes
  ///the adjacency matrixes for the graphs
  ///all to one
  
    vector< vector<int> > bob(index);
    for(size_t i = 0; i < bob.size(); i++){
        vector<int> addThis(index);
        bob[i] = addThis;
    }
    numCorresRev = bob;
    vector< vector<double> > billy(index);
    for(size_t i = 0; i < billy.size(); i++){
        vector<double> addThis(index);
        billy[i] = addThis;
    }
    totalRatingRev = billy;
    vector< vector<double> > joe(index);
    for(size_t i = 0; i < joe.size(); i++){
        vector<double> addThis(index);
        joe[i] = addThis;
    }
    avgRatingRev = joe;
    for(int i = 0; i < index; i++){
        for(int j = 0; j < index; j++){
            totalRatingRev[i][j] = 1;
            avgRatingRev[i][j] = 1;
        }
    }
}

void MyDataStore::memDestroyer() {
    //gets the set and sticks them into vectors for them to be deleted
    //since its not good form to modify shit when its getting iterated over
    vector<Product*> delet_this1;
    vector<User*> delet_this2;
    for(set<Product*>::iterator it = products.begin(); it != products.end(); ++it){
        delet_this1.push_back(*it);
    }
    for(set<User*>::iterator it = users.begin(); it != users.end(); ++it){
        delet_this2.push_back(*it);
    }
    for(size_t i = 0; i < delet_this1.size(); i++){
        delete delet_this1[i];
    }
    for(size_t i = 0; i < delet_this2.size(); i++){
        delete delet_this2[i];
    }
}
