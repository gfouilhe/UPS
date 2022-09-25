#include <string>
#include "Compte.h" 
#include "Date.h"
#include <iostream>

class Client {

    std::string name;
    Date date_of_birth;
    Compte account;

public:

    Client();
    Client(std::string n, Date date, Compte acc);
    void changeName(std::string name);
    void changeDate(Date date);
    void changeAcc(Compte acc);
    std::string getName();
    Date getDate();
    Compte getAcc();

};