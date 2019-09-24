#include <iostream>
#include <fstream>
#include <set>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include "product.h"
#include "db_parser.h"
#include "product_parser.h"
#include "util.h"
#include "mydatastore.h"

using namespace std;
struct ProdNameSorter {
    bool operator()(Product* p1, Product* p2) {
        return (p1->getName() < p2->getName());
    }
};
void displayProducts(vector<Product*>& hits);

int main(int argc, char* argv[])
{
    if(argc < 2) {
        cerr << "Please specify a database file" << endl;
        return 1;
    }

    /****************
     * Declare your derived DataStore object here replacing
     *  DataStore type to your derived type
     ****************/
    MyDataStore ds;



    // Instantiate the individual section and product parsers we want
    ProductSectionParser* productSectionParser = new ProductSectionParser;
    productSectionParser->addProductParser(new ProductBookParser);
    productSectionParser->addProductParser(new ProductClothingParser);
    productSectionParser->addProductParser(new ProductMovieParser);
    UserSectionParser* userSectionParser = new UserSectionParser;
    ReviewSectionParser* reviewSectionParser = new ReviewSectionParser;

    // Instantiate the parser
    DBParser parser;
    parser.addSectionParser("products", productSectionParser);
    parser.addSectionParser("users", userSectionParser);
    parser.addSectionParser("reviews", reviewSectionParser);

    // Now parse the database to populate the DataStore
    if( parser.parse(argv[1], ds) ) {
        cerr << "Error parsing!" << endl;
        return 1;
    }

    cout << "=====================================" << endl;
    cout << "Menu:                                " << endl;
    cout << "  LOGIN username password            " << endl;
    cout << "  LOGOUT                             " << endl;
    cout << "  AND r/n term term ...              " << endl;
    cout << "  OR r/n term term ...               " << endl;
    cout << "  ADD search_hit_number              " << endl;
    cout << "  VIEWCART                           " << endl;
    cout << "  BUYCART                            " << endl;
    cout << "  ADDREV search_hit_number rating date review_text" << endl;
    cout << "  VIEWREV search_hit_number          " << endl;
    cout << "  REC                                " << endl;
    cout << "  QUIT new_db_filename               " << endl;
    cout << "=====================================" << endl;

    vector<Product*> hits;
    bool done = false;
    while(!done) {
        cout << "\nEnter command: " << endl;
        string line;
        getline(cin,line);
        stringstream ss(line);
        string cmd;
        if((ss >> cmd)) {
            if( cmd == "AND") {
                string term;
                vector<string> terms;
                while(ss >> term) {
                    term = convToLower(term);
                    terms.push_back(term);
                }
                hits = ds.search(terms, 0);
                displayProducts(hits);
            }
            else if ( cmd == "OR" ) {
                string term;
                vector<string> terms;
                while(ss >> term) {
                    term = convToLower(term);
                    terms.push_back(term);
                }
                hits = ds.search(terms, 1);
                displayProducts(hits);
            }
            else if ( cmd == "QUIT") {
                string filename;
                if(ss >> filename) {
                    ofstream ofile(filename.c_str());
                    ds.dump(ofile);
                    ofile.close();
                }
                done = true;
            }
	    /* Add support for other commands here */

	    /*
	     * Below calls each of the commands corresponding
	     * in the mydatabase class
	     */
            else if ( cmd == "ADD") {
                int result;
                if(ss >> result){
                    ds.addingToCart(result);
                }
            }
            else if ( cmd == "VIEWCART") {
                ds.viewingToCart();
            }
            else if ( cmd == "BUYCART") {
                ds.buyingToCart();
            }
            else if ( cmd == "LOGIN"){
                string username;
                string password;
                if(ss >> username){
                    if(ss >> password){
                        ds.login(username, password);
                    }else{//if the user did not enter anything for the password
                        cout << "Invalid login credentials" << endl;
                    }
                }else{//if the user did not enter anything for the user
                    cout << "Invalid login credentials" << endl;
                }
            }
            else if ( cmd == "LOGOUT"){
                ds.logout();
            }
            else if ( cmd == "ADDREV"){
                int hitNum;
                int rating;
                string date;
                string reviewText = "";
                if(ss >> hitNum){
                    if(ss >> rating){
                        if(ss >> date){
                            string temp;
                            while(ss >> temp){
                                reviewText += temp + " ";
                            }
                            if(reviewText.size() > 0) {
                                reviewText = reviewText.substr(0, reviewText.size() - 1);
                            }
                            ds.addReview(hitNum, rating, date, reviewText);
                        }
                    }
                }
            }
            else if ( cmd == "VIEWREV"){
                int hitNum;
                if(ss >> hitNum){
                    ds.viewReview(hitNum);
                }
            }
            else if (cmd == "REC"){
                ds.rec();
            }
            else {
                cout << "Unknown command" << endl;
            }
        }

    }
    //Valgrind's bane
    ds.memDestroyer();
    return 0;
}

void displayProducts(vector<Product*>& hits)
{
    int resultNo = 1;
    //THE SORTING IS DONE IN THE MY DATASTORE
//    std::sort(hits.begin(), hits.end(), ProdNameSorter());
    for(vector<Product*>::iterator it = hits.begin(); it != hits.end(); ++it) {
        cout << "Hit " << setw(3) << resultNo << endl;
        cout << (*it)->displayString() << endl;
        cout << endl;
        resultNo++;
    }
}
