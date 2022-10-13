#include "Date.h"
#include <iostream>

using namespace std;

Date::Date() { 
    this->month = 1 ;
    this->day = 1 ;
    this->year = 2000;
    }

Date::Date(int m, int d, int y) { 
    this->month = m ;
    this->day = d ;
    this->year = y;
    }

int Date::getMonth() {return this->month;}

int Date::getDay() {return this->day;}

int Date::getYear() {return this->year;}



