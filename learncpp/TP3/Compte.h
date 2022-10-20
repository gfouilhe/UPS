class Compte {

    float solde;

public:

    Compte();
    Compte(float s);
    void depot(float m);
    void retrait(float m);
    void afficherSolde();
    void virement(Compte &c, float m);
    float getSolde();
    bool operator==(const Compte &c);

};

