#ifndef MYDATASTORE_H
#define MYDATASTORE_H
#include <string>
#include <set>
#include <vector>
#include <map>
#include "product.h"
#include "user.h"
#include "util.h"
#include "datastore.h"
#include "review.h"

class MyDataStore : public DataStore{
public:
    MyDataStore();
    ~MyDataStore();

    /**
     * Adds a product to the data store
     */
    void addProduct(Product* p);

    /**
     * Adds a user to the data store
     */
    void addUser(User* u);

    /**
     * Performs a search of products whose keywords match the given "terms"
     *  type 0 = AND search (intersection of results for each term) while
     *  type 1 = OR search (union of results for each term)
     */
    std::vector<Product*> search(std::vector<std::string>& terms, int type);

    /**
     * Adds a given item to the cart given its index of the search position
     */
    void addingToCart(int resultIndex);

    /**
     * Returns the cart by reference
     */
    void viewingToCart();

    /**
     * Buys the shit in the cart from bezos
     */
    void buyingToCart();

    /**
     * Reproduce the database file from the current Products and User values
     */
    void dump(std::ostream& ofile);

    void addReview(const std::string& prodName,
                   int rating,
                   const std::string& username,
                   const std::string& date,
                   const std::string& review_text);

    void login(std::string username, std::string password);

    void logout();

    void addReview(int searchHitNum,
                   int rating,
                   const std::string& date,
                   const std::string& review_text);

    void viewReview(int searchHitNum);

    void rec();

    std::vector<std::pair<std::string, double> > makeSuggestion(std::string currentUser);

    void initializeSimilarityMatrix();

    /**
     * Deallocates memory
     */
    void memDestroyer();

private:
    //first stores set of users and products
    std::set<User*> users;
    std::set<Product*> products;

    //a variable to store the current searchresult and the hit# along with
    //the product that it corresponds to
    std::map<int, Product*> searchResults;

    //contains a map, with the key being the product name, and value
    //being a pointer to the product
    std::map<std::string, Product*> productNameDatabase;

    //contains a map, with a keyword being the key and the value
    //being a set of products that correspond to the item
    std::map<std::string, std::set<Product*> > productKeywordDatabase;

    //contains a map, with the key being a string representation of a
    //username, while the value is a vector contain the items in the cart
    std::map<std::string, std::vector<Product*> > usernameToCart;

    //contains a map, with the key being a string representation of a
    //username, with value being a user
    std::map<std::string, User*> usernameToUser;

    //checks if graphs adajecncy matrixes have been initialized
    bool initialized;
    //is the runnig index for what number to assign each user to
    int index;
    //maps the number to each user and vice versa
    //for quick referencing in logn time
    std::map<int, User*> indexToUser;
    std::map<User*, int> userToIndex;

    ///each of these are same graph, just keeps track of differnt things
    //this one is the number of same product reviewed
    std::vector< std::vector<int> > numCorresRev;
    //this one is the sum of all the similaries and differentsec
    std::vector< std::vector<double> > totalRatingRev;
    //this one keeps track of the averages
    std::vector< std::vector<double> > avgRatingRev;

    //current user
    User* loggedIn;

    std::set<Product*> getProducts(std::string term);
};


#endif //MYDATASTORE_H
