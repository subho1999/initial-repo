#include "Marks.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

Marks::Marks()
{
    science = 0; maths = 0; english = 0; language = 0; sst = 0;
    science_marks = 0;
    commerce_marks = 0;
    total_marks = 0;
}
/*
Marks::Marks(Marks &m)
{
    science = m.science;
    maths = m.maths;
    english = m.english;
    language = m.language;
    sst = m.sst;
    science_marks = m.science_marks;
    commerce_marks = m.commerce_marks;
    total_marks = m.total_marks;
}
*/
void Marks::getmarks()
{
    cout << "Enter marks of\tScience : ";
    cin >> science;
    cout << "\t\tMaths : ";
    cin >> maths;
    cout <<"\t\tEnglish : ";
    cin >> english;
    cout << "\t\tLanguage 2 : ";
    cin >> language;
    cout << "\t\tSocial Studies : ";
    cin >>sst;
    calcmarks();
}

void Marks::calcmarks()
{
    science_marks = science + maths + english;
    commerce_marks = maths + sst;
    total_marks = science + maths + english + language + sst;
}

float Marks::return_commerce()
{
    return commerce_marks;
}

float Marks::return_science()
{
    return science_marks;
}

float Marks::return_total()
{
    return total_marks;
}
