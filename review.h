#ifndef REVIEW_H
#define REVIEW_H
#include <string>
#include "user.h"

// NOTE: You may add any data members or functions you deem useful

struct Review
{
    Review() :  rating(0), username(), date(), reviewText()
    { }
    Review(int rate,
           const std::string& u,
           const std::string& d,
           const std::string& review_text,
           User* use) :

        rating(rate),
        username(u),
        date(d),
        reviewText(review_text),
        user(use)
    {
        year = std::stoi(date.substr(0, 4));
        month = std::stoi(date.substr(5, 2));
        day = std::stoi(date.substr(8, 2));
    }

    int rating;
    std::string username;
    std::string date;
    std::string reviewText;
    User* user;
    int year;
    int month;
    int day;
};

#endif
