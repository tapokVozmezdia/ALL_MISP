#include <iostream>
#include "dataBase.h"

typedef unsigned int uint;

typedef struct {
    std::string city;
    std::string adress;
    std::string institute;
    std::string caffe;
    std::string cinema;
    uint age;
} Student;

uint setAge();
std::string setCity();
std::string setAdress();
std::string setInstitute();
std::string setCoffee();
std::string setCinema();
void fillStudent(Student *student);

class StudentService
{
    public:
        StudentService(DataBase* base);
        const uint getWorkdays() const;
        const uint getDaysCount() const;
        const uint getCosts(const std::string&, const std::string&, 
              const std::string&, const std::string&, const std::string&, 
              const uint);
        void getMonth();
        void setMonth(uint month);
        bool isMonth(int) const;
        uint getWorkdayCost() const;
        uint getWeekandCost() const;
        //void setOtherMonthlyCosts(const uint);
        //void linkDataBase(DataBase*);
    private:
        uint month = 1;
        uint cinemaCost;
        uint caffeeCost;
        uint transportCost;
        uint uniDinCost;
        uint homeFoodCost;
        uint otherFoodCost;
        DataBase *dataBase = nullptr;
};