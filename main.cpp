/**
  * This is the Project Made by Group - 1
  * This project MUST be compiled in a C++ 11 standardized compiler
  * The extra library that is used except the standard STL is cereal
  * cereal is a library used to serialize and store data in a computer understandable format
  */

#include <iostream>     ///For Basic I/O operations
#include "Student.h"    ///Contains the Student class
#include <stdexcept>    ///For Windows Handlers
#include <string>       ///For string class and functions
#include <windows.h>    ///WinAPI
#include <fstream>      ///File Handling main directive
#include <sstream>      ///File Handling string directive
#include <cstdint>      ///File Handling required directive
#include <cstdlib>      ///For system functions
#include <chrono>       ///For nanoseconds, system_clock, seconds
#include <thread>       ///For sleep_for, sleep_until
#include <vector>       ///For vector
#include <algorithm>    ///For standard algorithms (here required for working with vectors)

#include <cereal/types/vector.hpp>  ///This is cereal's vector conversion file

using namespace std;
using namespace std::this_thread;   ///For sleep_for, sleep_until
using namespace std::chrono;        ///For nanoseconds, system_clock, seconds

///The following structure is used to store the data into the files

struct file_input
{
    vector<Student> svec;

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( svec );
    }
};

/************************************************
 *                                              *
 *  Functions prototypes used in the project    *
 *                                              *
 ************************************************/

string getpassword( const string& prompt);
bool check_password(string original_pass);

void intro();
void byebye();
void admin_portal();
void change_pass();
void change_sel_criteria();
void view_data();
void modify_data();
void enter_data();
bool check_if_present(file_input &stu, int reg_num);
void delete_data();

void student_portal();
void view_all();
void search_particular();

/*********************************************
 *                                           *
 *      MAIN function of the project         *
 *                                           *
 *********************************************/

int main()
{
    intro();    ///Displays intro of the project and some details

    int do_while_counter = 1;
    do
    {   label_main_menu:
        system("cls");
        cout << "\n\n---------------------------------------------------------------------------------\n";
        cout << "\n\t\t\t\tMAIN MENU\n\n";
        cout << "---------------------------------------------------------------------------------\n\n";
        int c;
        cout << "\nEnter :\t1 for Administrator Portal\n\t2 for Student's Portal\n\t0 to exit : ";
        cin >> c;
        switch(c)
        {
        case 1:
            {
                system("cls");
                if(ifstream file_pass{"password.dat", ios::binary | ios::ate})
                {
                    auto size = file_pass.tellg();
                    string str(size, '\0');         // construct string to stream size
                    file_pass.seekg(0);
                    if(file_pass.read(&str[0], size))
                    {
                        int counter = 0;
                        cout << "\nYou have Three tries to get the password correct" << endl << endl;
                        bool pass_check, flag = false;
                        while (counter < 3)
                        {
                            pass_check = check_password(str);
                            if (pass_check == true)
                            {
                                flag = true;
                                cout << "\n\n\t\t\t\t!!!!ACCESS GRANTED!!!!";
                                sleep_for(seconds(1));
                                admin_portal();
                                break;
                            }
                            else
                            {
                                ++counter;
                                cout << "\nWrong password, try again (" << (3-counter) << " tries left)" << endl;
                            }
                        }

                        file_pass.close();

                        if (flag == false)
                        {
                            cout << "\nYou failed to enter correct password in 3 tries\n";
                            cout << "\n\n\t\t\t\t!!!!ACCESS DENIED!!!!";
                            Beep(2000, 1000);
                            this_thread::sleep_for(chrono::milliseconds(400));
                            Beep(2000, 1000);
                            this_thread::sleep_for(chrono::milliseconds(400));
                            Beep(2000, 1000);
                            this_thread::sleep_for(chrono::milliseconds(400));
                            exit(1);
                        }
                    }
                    else
                    {
                        cout << "\nCan't read password";
                        goto label_main_menu;
                    }
                }
                else
                {
                    cout << "\n\"password.dat\" file doesn't exist";
                    goto label_main_menu;
                }
                break;
            }
        case 2:
            {
                system("cls");
                student_portal();
                break;
            }
        case 0:
            {
                system("cls");
                byebye();
                //cout << "\nBye";
                sleep_for(milliseconds(700));
                do_while_counter = 0;
                break;
            }
        default:
            {
                cout << "\nWrong choice" << endl;
                sleep_for(milliseconds(900));
                break;
            }
        }
    }while(do_while_counter);

    return 0;

}

///The next function uses WriteConsole which is standard for Windows systems

string getpassword( const string& prompt = "Enter password> " )
{
    string result;

    // Set the console mode to no-echo, not-line-buffered input
    DWORD mode, count;
    HANDLE ih = GetStdHandle( STD_INPUT_HANDLE  );
    HANDLE oh = GetStdHandle( STD_OUTPUT_HANDLE );
    if (!GetConsoleMode( ih, &mode ))
        throw runtime_error("getpassword: You must be connected to a console to use this program.\n");

    SetConsoleMode( ih, mode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT) );

    // Get the password string
    WriteConsoleA( oh, prompt.c_str(), prompt.length(), &count, NULL );
    char c;
    while (ReadConsoleA( ih, &c, 1, &count, NULL) && (c != '\r') && (c != '\n'))
    {
        if (c == '\b')
        {
            if (result.length())
            {
                WriteConsoleA( oh, "\b \b", 3, &count, NULL );
                result.erase( result.end() -1 );
            }
        }
        else
        {
            WriteConsoleA( oh, "*", 1, &count, NULL );
            result.push_back( c );
        }
    }

    // Restore the console mode
    SetConsoleMode( ih, mode );

    return result;
}

bool check_password(string original_pass)
{
    string password = getpassword( "Enter Administrator password> " );

    if (password == original_pass)
    {   //cout << "\nAccess Granted";
        return true;
    }
    else
    {
        //cout << "\nAccess Denied";
        return false;
    }
}

/*********************************************
 *                                           *
 *      Function defining ADMIN portal       *
 *                                           *
 *********************************************/

void admin_portal()
{
    int do_while_counter = 1;
    do
    {
        system("cls");
        cout << "\n\n---------------------------------------------------------------------------------\n";
        cout << "\n\t\t\t\tADMINISTRATOR PORTAL\n\n";
        cout << "---------------------------------------------------------------------------------\n\n";
        cout << "\nEnter :\t1 for Configuration Menu\n\t2 for Data Manipulation Menu\n\t3 to return back to MAIN MENU : ";
        int c;
        cin >> c;
        switch(c)
        {
        case 1:
            {
                int do_while_counter_config = 1;
                do
                {
                    system("cls");
                    cout << "\n\n---------------------------------------------------------------------------------\n";
                    cout << "\n\t\t\t\tConfiguration Menu\n\n";
                    cout << "---------------------------------------------------------------------------------\n\n";
                    cout << "\nEnter :\t1 to change password\n\t2 to change selection criteria\n\t3 to go back to ADMIN MENU : ";
                    int c_config;
                    cin >> c_config;
                    switch(c_config)
                    {
                    case 1:
                        {
                            change_pass();
                            break;
                        }
                    case 2:
                        {
                            change_sel_criteria();
                            break;
                        }
                    case 3:
                        {
                            do_while_counter_config = 0;
                            break;
                        }
                    default:
                        {
                            cout << "\nWrong Input";
                            sleep_for(milliseconds(900));
                            break;
                        }
                    }
                }while(do_while_counter_config);
                break;
            }

        case 2:
            {
                int do_while_counter_data = 1;
                do
                {
                    system("cls");
                    cout << "\n\n---------------------------------------------------------------------------------\n";
                    cout << "\n\t\t\t\tData Manipulation Screen\n\n";
                    cout << "---------------------------------------------------------------------------------\n\n";
                    cout << "\nEnter :\t1 to View Student Records\n\t2 to Enter New Student Records";
                    cout << "\n\t3 to Modify Stored Student Records \n\t4 to Delete Student Record(s)";
                    cout << "\n\t5 to go back to ADMIN MENU : ";
                    int c_data;
                    cin >> c_data;
                    switch(c_data)
                    {
                    case 1:
                        {
                            view_data();
                            break;
                        }
                    case 2:
                        {
                            enter_data();
                            break;
                        }
                    case 3:
                        {
                            modify_data();
                            break;
                        }
                    case 4:
                        {
                            delete_data();
                            break;
                        }
                    case 5:
                        {
                            do_while_counter_data = 0;
                            break;
                        }
                    default:
                        {
                            cout << "\nWrong choice";
                            sleep_for(milliseconds(900));
                            break;
                        }
                    }
                }while(do_while_counter_data);
                break;
            }
        case 3:
            {
                do_while_counter = 0;
                break;
            }
        default:
            {
                cout << "\nWrong Choice";
                sleep_for(seconds(1));
                break;
            }
        }
    }while (do_while_counter);
    return;
}

/*********************************************
 *                                           *
 *     Function defining STUDENT portal      *
 *                                           *
 *********************************************/

void student_portal()
{
    int do_while_counter = 1;
    do
    {
        system("cls");
        cout << "\n\n---------------------------------------------------------------------------------\n";
        cout << "\n\t\t\t\tSTUDENT PORTAL\n\n";
        cout << "---------------------------------------------------------------------------------\n\n";
        cout << "\nEnter :\t1 for Viewing All Results\n\t2 for Viewing Particular Result\n\t3 to return back to MAIN MENU : ";
        int c;
        cin >> c;
        switch(c)
        {
        case 1:
            {
                view_all();
                break;
            }
        case 2:
            {
                search_particular();
                break;
            }
        case 3:
            {
                do_while_counter = 0;
                break;
            }
        default:
            {
                cout << "\nWrong Choice";
                sleep_for(seconds(1));
                break;
            }
        }
    }while (do_while_counter);
}

///Function to change the administrator password

void change_pass()
{
    system("cls");
    cout << "\n\nChange Password Screen" << endl << endl;
    string new_pass;
    new_pass = getpassword("Enter New Password> ");
    ofstream f;
    f.open("password.dat", ios::out|ios::binary);
    f.write(new_pass.c_str(), new_pass.length());
    f.close();
    cout << "\nPassword Changed";
    sleep_for(milliseconds(700));
}

///Function to change selection criteria

void change_sel_criteria()
{
    int do_while_sel_crit = 1;
    do
    {
        system("cls");
        cout << "\n\nChange Selection Criteria Screen\n\n";
        cout << "\nEnter :\t1 to Change Pass Percentage\n\t2 to change Science Qualification Percentage";
        cout << "\n\t3 to change Commerce Qualification Percentage\n\t4 to go back to CONFIGURATION MENU : ";
        int c_sel_crit = 1;
        cin >> c_sel_crit;
        switch(c_sel_crit)
        {
        case 1:
            {
                system("cls");
                fstream total_read;
                total_read.open("selection_criteria_total.dat", ios::binary | ios::in | ios::out);
                if(!total_read.is_open())
                {
                    cout << "\nFile " << "\"selection_criteria_total.dat\"" << " does not exist !! ";
                    cout << "\nPlease Verify and try again";
                    sleep_for(milliseconds(1700));
                    break;
                }
                float total;
                total_read.read((char*)&total, sizeof(total));
                cout << "Current Pass Percentage : " << total;

                cout << "\nPlease Enter new Pass percentage : ";
                cin >> total;
                total_read.seekg(0, ios::beg);
                total_read.write((char*)&total, sizeof(total));
                cout << "\nPass Percentage successfully changed to " << total;
                total_read.close();
                sleep_for(seconds(2));
                break;
            }
        case 2:
            {
                system("cls");
                fstream science_read;
                science_read.open("selection_criteria_science.dat", ios::binary | ios::in | ios::out);
                if(!science_read.is_open())
                {
                    cout << "\nFile " << "\"selection_criteria_science.dat\"" << " does not exist !! ";
                    cout << "\nPlease Verify and try again";
                    sleep_for(milliseconds(1700));
                    break;
                }
                float science;
                science_read.read((char*)&science, sizeof(science));
                cout << "Current Science Selection Percentage : " << science;

                cout << "\nPlease Enter new Science Selection percentage : ";
                cin >> science;
                science_read.seekg(0, ios::beg);
                science_read.write((char*)&science, sizeof(science));
                cout << "\nScience Selection Percentage successfully changed to " << science;
                science_read.close();
                sleep_for(seconds(2));
                break;
            }
        case 3:
            {
                system("cls");
                fstream commerce_read;
                commerce_read.open("selection_criteria_commerce.dat", ios::binary | ios::in | ios::out);
                if(!commerce_read.is_open())
                {
                    cout << "\nFile " << "\"selection_criteria_commerce.dat\"" << " does not exist !! ";
                    cout << "\nPlease Verify and try again";
                    sleep_for(milliseconds(1700));
                    break;
                }
                float commerce;
                commerce_read.read((char*)&commerce, sizeof(commerce));
                cout << "Current Commerce Selection Percentage : " << commerce;

                cout << "\nPlease Enter new Commerce Selection percentage : ";
                cin >> commerce;
                commerce_read.seekg(0, ios::beg);
                commerce_read.write((char*)&commerce, sizeof(commerce));
                cout << "\nCommerce Selection Percentage successfully changed to " << commerce;
                commerce_read.close();
                sleep_for(seconds(2));
                break;
            }
        case 4:
            {
                do_while_sel_crit = 0;
                break;
            }
        default:
            {
                cout << "\nWrong Choice";
                sleep_for(milliseconds(900));
                break;
            }
        }
    }while (do_while_sel_crit);
    return;
}

///Function related to Admin section. Views all the records shown in the file

void view_data()
{
    system("cls");
    cout << "\n\n---------------------------------------------------------------------------------\n";
    cout << "\n\t\t\t\tView Data Screen\n\n";
    cout << "---------------------------------------------------------------------------------\n\n";
    ifstream file("records.dat", ios::in);
    if(!file.is_open())
    {
        cout << "\n\nFile \"" << "records.dat" << "\" does not exist! Please check";
        sleep_for(milliseconds(1700));
        return;
    }
    if(file.eof())
    {
        cout << "\nThe file\"" << "records.dat" <<"\" is empty";
        sleep_for(milliseconds(1700));
        return;
    }
    cout << "\nViewing all data in \"" << "records.dat" << "\" file : " << endl;

    file_input stu;
    cereal::PortableBinaryInputArchive input(file);
    input(stu);
    vector<Student>::iterator iter;

    int i = 0;
    for(iter = stu.svec.begin(); iter != stu.svec.end(); ++iter, ++i)
    {
        cout << "\n\nDetails of student " << (i+1) << " : ";
        (*iter).display_details();
    }
    cout << "\n\n";
    system("pause");
    file.close();
}

///Function related to Admin section. Appends new records to the file

void enter_data()
{
    system("cls");
    cout << "\n\n---------------------------------------------------------------------------------\n";
    cout << "\n\t\t\t\tEnter Data Screen\n\n";
    cout << "---------------------------------------------------------------------------------\n\n";
    fstream file("records.dat", ios::in);

    /*
    if(!file.is_open())
    {
        cout << "\n\nUnable to create file \"" << "records.dat" << "\"!";
        sleep_for(milliseconds(1700));
        return;
    }

    file_input stu;
    cereal::PortableBinaryInputArchive input(file);
    input(stu);
    file.close();
    */

    file_input stu;
    if(file.is_open())
    {
        cereal::PortableBinaryInputArchive input(file);
        input(stu);
        file.close();
    }
    else
    {
        cout << "\nFile doesn't exist or it can't be opened \n";
        cout << "\nDo you want to enter data from scratch (y/n) ? : ";
        char y;
        cin >> y;
        if (y == 'n' || y == 'N')
            return;
    }

    char x = 'y';
    Student ob;
    while (x == 'y'||x == 'Y')
    {
        system("cls");
        cout << "\n\n---------------------------------------------------------------------------------\n";
        cout << "\n\t\t\t\tEnter Data Screen\n\n";
        cout << "---------------------------------------------------------------------------------\n\n";
        cout << "\nEnter new Record : \n";
        ob.getdetails();

        if(check_if_present(stu, ob.return_regnum()))
        {
            cout << "\n\nAnother record with the same registration number is also present! Please enter correctly" << endl;
        }
        else
            stu.svec.push_back(ob);
        cout << "\nDo you want to enter more (y/n)? : ";
        cin >> x;
        file.open("records.dat", ios::out);
        cereal::PortableBinaryOutputArchive output(file);
        output(stu);
        file.close();
    }
}

///Function related to enter_data() function. Checks whether an already entered file is present or not

bool check_if_present(file_input &stu, int regnum)
{
    /*
    fstream file("records.dat", ios::in);
    if (!file.is_open())
        return false;
    file_input stu;
    cereal::PortableBinaryInputArchive input(file);
    input(stu);
    file.close();
    */

    vector<Student>::iterator iter;

    bool flag = false;
    for(iter = stu.svec.begin(); iter != stu.svec.end(); ++iter)
    {
        if((*iter).return_regnum() == regnum)
            flag = true;
    }

    return flag;
}

///Function related to Admin section. Modifies particular records in the file

void modify_data()
{
    char ch = 'y';
    while(ch == 'y' || ch == 'Y')
    {
        system("cls");
        cout << "\n\n---------------------------------------------------------------------------------\n";
        cout << "\n\t\t\t\tModify Data Screen\n\n";
        cout << "---------------------------------------------------------------------------------\n\n";
        fstream file;
        file_input stu;
        file.open("records.dat", ios::in);
        cereal::PortableBinaryInputArchive input(file);
        input(stu);
        file.close();
        cout << "\nEnter the registration number of the student you want to modify : ";
        int rnum;
        cin >> rnum;
        Student ob;
        auto it = find_if(std::begin(stu.svec), std::end(stu.svec), [rnum](Student const & x) { return x.return_regnum() == rnum; });
        if (it == stu.svec.end())
        {
            cout << "\n\nNo record with registration number = " << rnum << " is found!";
        }
        else
        {
            cout << "\n\nRecord found : \n\n";
            (*it).display_details();
            cout << "\n\nEnter new data to replace it with :\n";
            Student ob;
            ob.getdetails();
            *it = ob;
        }
        cout << "\nWanna search again ? (y/n)";
        cin >> ch;
        file.open("records.dat", ios::out);
        cereal::PortableBinaryOutputArchive output(file);
        output(stu);
        file.close();
    }
}

///Function related to Admin section. Deletes particular records from the file

void delete_data()
{
    char ch = 'y';
    while (ch == 'y' || ch == 'Y')
    {
        system("cls");
        cout << "\n\n---------------------------------------------------------------------------------\n";
        cout << "\n\t\t\t\tDelete Data Screen\n\n";
        cout << "---------------------------------------------------------------------------------\n\n";
        fstream file;
        file_input stu;
        file.open("records.dat", ios::in);
        cereal::PortableBinaryInputArchive input(file);
        input(stu);
        file.close();
        cout << "\nEnter the registration number of the student you want to delete : ";
        int rnum;
        cin >> rnum;
        Student ob;
        auto it = find_if(std::begin(stu.svec), std::end(stu.svec), [rnum](Student const & x) { return x.return_regnum() == rnum; });
        if (it == stu.svec.end())
        {
            cout << "\nNo record with registration number = " << rnum << " is found!\n";
        }
        else
        {
            cout << "\nRecord found\nDeleting the record : \n";
            (*it).display_details();
            stu.svec.erase(it);
            cout << "\n\nData Deleted\n";
        }
        cout << "\nWanna search again ? (y/n)";
        cin >> ch;
        file.open("records.dat", ios::out);
        cereal::PortableBinaryOutputArchive output(file);
        output(stu);
        file.close();
    }
}

///Function related to Student section. Views all records present in the file

void view_all()
{
    system("cls");
    cout << "\n\n---------------------------------------------------------------------------------\n";
    cout << "\n\t\t\t\tView Records Screen\n\n";
    cout << "---------------------------------------------------------------------------------\n\n";
    ifstream file("records.dat", ios::in);
    if(!file.is_open())
    {
        cout << "\n\nFile \"" << "records.dat" << "\" does not exist! Please check";
        sleep_for(milliseconds(1700));
        return;
    }
    if(file.eof())
    {
        cout << "\nThe file\"" << "records.dat" <<"\" is empty";
        sleep_for(milliseconds(1700));
        return;
    }
    cout << "\nViewing all data in \"" << "records.dat" << "\" file : " << endl;

    file_input stu;
    cereal::PortableBinaryInputArchive input(file);
    input(stu);
    vector<Student>::iterator iter;

    int i = 0;
    for(iter = stu.svec.begin(); iter != stu.svec.end(); ++iter, ++i)
    {
        cout << "\n\nDetails of student " << (i+1) << " : ";
        (*iter).display_details();
    }
    cout << "\n\n";
    system("pause");
    file.close();
}

///Function related to Student section. Views the searched for record from the file

void search_particular()
{
    char ch = 'y';
    while(ch == 'y' || ch == 'Y')
    {
        system("cls");
        cout << "\n\n---------------------------------------------------------------------------------\n";
        cout << "\n\t\t\t\tSearch Particular Record Screen\n\n";
        cout << "---------------------------------------------------------------------------------\n\n";
        fstream file;
        file_input stu;
        file.open("records.dat", ios::in);
        cereal::PortableBinaryInputArchive input(file);
        input(stu);
        file.close();
        cout << "\nEnter your registration number : ";
        int rnum;
        cin >> rnum;
        Student ob;
        auto it = find_if(std::begin(stu.svec), std::end(stu.svec), [rnum](Student const & x) { return x.return_regnum() == rnum; });
        if (it == stu.svec.end())
        {
            cout << "\n\nSorry, your record with registration number = " << rnum << " does not exist!";
            cout << "\nPlease check your registration number and search again !";
        }
        else
        {
            cout << "\n\nYour Record found : \n\n";
            (*it).display_details();
        }
        cout << "\nWant to search again ? (y/n)";
        cin >> ch;
    }
}

///Function to print Intro

void intro()
{
    cout << endl << endl << endl;
    cout << "			        WELCOME TO STUDENT RECORD MANAGEMENT" << endl;
    cout << "=====================================================================================================" << endl;
    sleep_for(milliseconds(1000));
	cout << endl;
	cout << "                         PROJECT" << endl;
	cout << "                         MADE BY :";
	cout << " Subhabrata Ghosh" << endl;
	cout << "                                   Ankit Adak" << endl;
	cout << "                                   Arkopravo Das" << endl;
	cout << "                                   Kingsuk Bag" << endl << endl;
	cout << "=====================================================================================================" << endl << endl;
	sleep_for(milliseconds(1000));
	cout << "                         SCHOOL  : ";
	cout << "D.A.V. Public School" << endl;
	cout << "                                   D.V.C. M.T.P.S. Colony " << endl;
	cout << "                                   Po-722183 " << endl;
	cout << "                                   Bankura,West Bengal,INDIA " << endl << endl;
	cout << "=====================================================================================================" << endl;
    sleep_for(milliseconds(2700));
}

void byebye()
{
    cout << endl << endl << endl;
    cout << "                                              !! THANK YOU !!" << endl ;
    cout << "====================================================================================================="<< endl << endl;
    sleep_for(milliseconds(1300));
    cout << "                                            ! Hasta la Vista, Baby !" << endl;
    cout << "====================================================================================================="<< endl << endl << endl;
    sleep_for(milliseconds(1300));
    cout << "                         PROJECT" << endl;
	cout << "                         MADE BY :";
	cout << " Subhabrata Ghosh" << endl;
	cout << "                                   Ankit Adak" << endl;
	cout << "                                   Arkopravo Das" << endl;
	cout << "                                   Kingsuk Bag" << endl << endl;
	cout << "====================================================================================================="<< endl << endl << endl;
}
