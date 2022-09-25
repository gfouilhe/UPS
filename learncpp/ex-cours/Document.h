#include <string>
#include <iostream>

using namespace std;

class Document {

protected:
    string titre;
    string auteur;
    string *mots;
    int taille, nbElem;
public:
    Document();
    Document(string = "", string = "", int =100);
    ~Document();
    void ajouter(string &);
}