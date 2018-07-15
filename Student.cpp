#include "Student.h"
#include <string>
#include <iostream>
#include "Marks.h"
#include <fstream>
#include <cstdlib>
#include <chrono>
#include <thread>
using namespace std;

Student::Student()
{
    registration_number = 0;
    name = "NULL";
    stream = "NULL";
    status = "NULL";
    pass = false, pass_commerce = false, pass_science = false;
}
/*
Student::Student(Student &s)
{
    registration_number = s.registration_number;
    name = s.name;
    ob(s.ob);
    stream = s.stream;
    status = s.status;
    pass = s.pass;
    pass_science = s.pass_science;
    pass_commerce = s.pass_commerce;
}
*/
void Student::getdetails(int i)
{
    if (i!=0)
        goto foolproof_label;
    cout << "Enter student Registration Number : ";
    cin >> registration_number;
    cin.ignore();
    cout << "Enter name : ";
    getline(cin, name);
    cout << "Enter marks obtained in different subjects :\n";
    ob.getmarks();
    calc_pass();
    foolproof_label:
    cout << "Enter choice of stream\t1 for Science\n\t\t2 for Commerce\n\t\t3 for Arts : ";
    cin >> choice;
    stream_selection();
}

void Student::calc_pass()
{
    float marks_pass, marks_pass_science, marks_pass_commerce;
    ifstream file_pass;
    file_pass.open("selection_criteria_total.dat", ios::in | ios::binary);
    file_pass.read((char*)&marks_pass, sizeof(marks_pass));
    if ((ob.return_total()/5.0) < marks_pass)
        pass = false;
    else
        pass = true;
    file_pass.close();

    file_pass.open("selection_criteria_science.dat", ios::in | ios::binary);
    file_pass.read((char*)&marks_pass_science, sizeof(marks_pass_science));
    if ((ob.return_science()/3.0) < marks_pass_science)
        pass_science = false;
    else
        pass_science = true;
    file_pass.close();

    file_pass.open("selection_criteria_commerce.dat", ios::in | ios::binary);
    file_pass.read((char*)&marks_pass_commerce, sizeof(marks_pass_commerce));
    if ((ob.return_commerce()/2.0) < marks_pass_commerce)
        pass_commerce = false;
    else
        pass_commerce = true;
    file_pass.close();
}

void Student::stream_selection()
{
    if (!pass)
    {
        status = "Failed";
        stream = "Failed";
        cout << "\nSorry, but the choice doesn't matter as the student has failed !! ";
        this_thread::sleep_for(chrono::milliseconds(900));
        return;
    }

    status = "Passed";

    if(choice == 3)
        stream = "Arts";

    else if (choice == 1)
    {
        if (pass_science)
            stream = "Science";
        else
        {
            if (pass_commerce)
            {
                int temp;
                label:
                cout << "\nThis student is not eligible for Science, choose second preference: 2 for Commerce, 3 for Arts : ";
                cin >> temp;
                switch(temp)
                {
                case 2:
                    stream = "Commerce";
                    break;
                case 3:
                    stream = "Arts";
                    break;
                default:
                    cout << "Wrong Input, Choose again";
                    goto label;
                }
            }

            else
            {
                cout << "\n\nThis student is only eligible for Arts, neither Science nor Commerce";
                cout << "\nAnd thus the stream Arts is selected for him.";
                stream = "Arts";
            }
        }
    }

    else if (choice == 2)
    {
        if (pass_commerce)
            stream = "Commerce";
        else
        {
            if (pass_science)
            {
                int temp;
                label1:
                cout << "\nThis student is not eligible for Commerce, choose second preference: 1 for Science, 3 for Arts : ";
                cin >> temp;
                switch(temp)
                {
                case 1:
                    stream = "Science";
                    break;
                case 3:
                    stream = "Arts";
                    break;
                default:
                    cout << "Wrong Input, Choose again";
                    goto label1;
                }
            }

            else
            {
                cout << "\nThis student is only eligible for Arts, neither Science nor Commerce";
                stream = "Arts";
            }
        }
    }
    else
    {
        cout << "\nInvalid Choice of Stream. Please don't intentionally fool around\nIt took me so much time to make it foolproof\n";
        getdetails(1);
    }
}


void Student::display_details()
{
    //cout << "\nDetails of Student : ";
    cout << "\nRegistration Number : " << registration_number;
    cout << "\nName : " << name;
    cout << "\nTotal Marks obtained : " << ob.return_total();
    cout << "\nStatus : " << status;
    cout << "\nStream : " << stream;
}

string Student::return_stream()
{
    return stream;
}
