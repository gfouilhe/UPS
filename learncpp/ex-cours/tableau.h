class Tableau{
    int taille;
    int nbElem;
    int *valeurs;
public:
    Tableau(int t = 10);
    Tableau(const Tableau &tab);
    ~Tableau();
    void ajouter(int n);
}