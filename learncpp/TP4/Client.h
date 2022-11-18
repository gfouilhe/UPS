#include <string>
#include "Compte.h" 
#include "Date.h"

class Client {

    string name;
    Date date_of_birth;
    Compte account;

public:

    Client();
    Client(string n, Date date, Compte acc);
    void changeName(string name);
    void changeDate(Date date);
    void changeAcc(Compte acc);
    string getName();
    Date getDate();
    Compte getAcc();
    void afficherClient();
    bool operator==(const Client &cli); 

    friend ostream& operator<<(ostream &out, Client &cli);
};

inline ostream& operator<<(ostream &out, Client &cli){
    out << "Nom : " << cli.name <<endl;
    out << "Date de Naissance : " << cli.date_of_birth << endl;
    out << "Compte : " << cli.account << endl;
    return cout;
};