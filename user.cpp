#include "user.h"
using namespace std;

User::User() : name_("unknown"), balance_(0.0), type_(1)
{

}
User::User(std::string name, double balance, int type, unsigned long long password) :
    name_(name), balance_(balance), type_(type), password_(password)
{

}

User::~User()
{

}


std::string User::getName() const
{
    return name_;
}

double User::getBalance() const
{
    return balance_;
}

void User::deductAmount(double amt)
{
    balance_ -= amt;
}

bool User::login(std::string password) {
    if(password.length() <= 8){
        unsigned long long bigNum = 0;
        for(size_t i = 0; i < password.size(); i++){
            bigNum = (128 * bigNum) + (int)(password[i]);
        }
        unsigned int intArr[4] = {0,0,0,0};
        int i = 0;
        while(bigNum > 0){
            intArr[i++] = (bigNum % 65521);
            bigNum /= 65521;
        }
        unsigned long long ans = ((45912 * intArr[3]) + (35511 * intArr[2]) + (65169 * intArr[1]) + (4625 * intArr[0])) % 65521;
        return ans == password_;
    }
    return false;
}

void User::dump(std::ostream& os)
{
    os << name_ << " "  << balance_ << " " << type_ << endl;
}
