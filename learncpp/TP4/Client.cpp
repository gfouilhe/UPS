#include <string>
#include <iostream>
#include "Client.h"

using namespace std;

Client::Client() {
    this->name = "";
    this->date_of_birth = Date();
    this->account = Compte();
}
Client::Client(std::string n, Date date, Compte acc) {
    this->name = n;
    this->date_of_birth = date;
    this->account = acc;
}
void Client::changeName(std::string name) {
    this->name = name;
}
void Client::changeDate(Date date) {
    this->date_of_birth = date;
}
void Client::changeAcc(Compte acc) {
    this->account = acc;
}
std::string Client::getName() {
    return this->name;
}
Date Client::getDate() {
    return this->date_of_birth;
}
Compte Client::getAcc() {
    return this->account;
}

void Client::afficherClient() {
    cout << "Name : " << this->name << endl;
    Date dob = this->date_of_birth;
    cout << "Date of Birth : "; dob.afficherDate(); cout <<endl;
    Compte acc = this->account;
    acc.afficherSolde();
}

bool Client::operator==(const Client &cli) {
    return this->name == cli.name && this->date_of_birth == cli.date_of_birth && this->account == cli.account;
}