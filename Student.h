#ifndef STUDENT_H
#define STUDENT_H
#include <string>
#include "Marks.h"

#include <cereal/types/string.hpp>
#include <cereal/types/complex.hpp>

#include <cereal/archives/portable_binary.hpp>

class Student
{
    public:
        Student();
        //Student(Student &s);
        void getdetails(int i = 0);
        void calc_pass();
        void stream_selection();
        void display_details();
        int return_regnum() const {return registration_number;};
        std::string return_stream();
        friend class cereal::access;

    protected:

    private:
        int registration_number;
        std::string name;
        Marks ob;
        std::string stream;
        int choice;
        std::string status;
        bool pass, pass_science, pass_commerce;

        template <class Archive>
        void serialize(Archive &ar)
        {
            ar(registration_number, name, ob, stream, choice, status, pass, pass_science, pass_commerce);
        }
};

#endif // STUDENT_H
