#include <iostream>

using namespace std;

class Date {
    int month;
    int day;
    int year;
public:
    Date();
    Date(int m, int d, int y);
    int getDay();
    int getMonth();
    int getYear();
    bool operator==(const Date &d);
    void afficherDate();

        friend ostream& operator<<(ostream &out, Date &d);
};

inline ostream& operator<<(ostream &out, Date &d){
    out << d.day << "/" << d.month << "/" << d.year << endl;
    return cout;
};
