#ifndef MARKS_H
#define MARKS_H
#include <fstream>
#include <cereal/types/string.hpp>
#include <cereal/types/complex.hpp>

#include <cereal/archives/binary.hpp>

class Marks
{
    public:
        Marks();
        //Marks(Marks &m);
        void getmarks();
        void calcmarks();
        float return_science();
        float return_commerce();
        float return_total();
        friend class cereal::access;

    protected:

    private:
        float science, maths, english, language, sst;
        float science_marks, commerce_marks, total_marks;

        template <class Archive>
        void serialize(Archive &ar)
        {
            ar(science, maths, english, language, sst, science_marks, commerce_marks, total_marks);
        }
};

#endif // MARKS_H
