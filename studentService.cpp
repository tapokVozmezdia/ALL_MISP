
#include "studentService.h"
#include <sstream>

bool StudentService::isMonth(int month) const
{
    if (month < 13 && month > 0)
    {
        return true;
    }
    else
    {
        std::cout << "Invalid input, please retry" << std::endl;
    }
    return false;
}

void errorCheck(uint *ptr, std::string msg)
{
    if (*ptr == -1)
    {
        *ptr = 0;
        std::cout << "Error, data not found : " << msg << std::endl;
    }
    return;
}

StudentService::StudentService(DataBase* base)
{
    this->dataBase = base;
}

uint StudentService::getWorkdayCost() const 
{
    return static_cast<uint>(2 * transportCost + uniDinCost + 0.66 * 
    homeFoodCost / (StudentService::getWorkdays()) + 0.5);
}
uint StudentService::getWeekandCost() const 
{
    return static_cast<uint>(0.66 * homeFoodCost / (StudentService::getDaysCount()
    - StudentService::getWorkdays()) + cinemaCost + caffeeCost + 0.5);
}

const uint StudentService::getWorkdays() const
{
    if (month == 1)
    {
        return 17;
    }
    if (month == 2)
    {
        return 18;
    }
    if (month == 3 || month == 10)
    {
        return 22;
    }
    if (month == 4 || month == 5)
    {
        return 20;
    }
    if (month == 6 || month == 7 || month == 9 || month == 11 || month == 12)
    {
        return 21;
    }
    return 23;
}

const uint StudentService::getDaysCount() const
{
    if (month == 2)
    {
        return 28;
    }
    if (month < 8)
    {
        return (month % 2) + 30;
    }
    return ((month + 1) % 2) + 30;
}

const uint StudentService::getCosts(const std::string& city, const std::string& homeAddress, 
              const std::string& institute, const std::string& cinema,
              const std::string& coffee, const uint age) {

    const uint daysCount = StudentService::getDaysCount();
    const uint workDays = StudentService::getWorkdays();
    const uint weekends = daysCount - workDays;

    std::string t_est_costs = dataBase->getEstCost(city, cinema, coffee);
    std::stringstream s(t_est_costs);
    std::string t_caffeeCost;
    std::string t_cinemaCost;
    std::string t_homeFoodCost;
    std::string t_otherFoodCost;
    getline(s, t_caffeeCost, 'I');
    getline(s, t_cinemaCost);
    caffeeCost = static_cast<uint>(stoi(t_caffeeCost));
    cinemaCost = static_cast<uint>(stoi(t_cinemaCost));
    std::string t_misc_costs = dataBase->getMiscCosts(city, age);
    std::stringstream ss(t_misc_costs);
    getline(ss, t_homeFoodCost, 'I');
    getline(ss, t_otherFoodCost);
    homeFoodCost = static_cast<uint>(stoi(t_homeFoodCost));
    otherFoodCost = static_cast<uint>(stoi(t_otherFoodCost));
    transportCost = dataBase->getTransportCost(city, homeAddress, institute);
    uniDinCost = dataBase->getInstituteDinnerCost(city, institute);
    errorCheck(&cinemaCost, "cinema");
    errorCheck(&caffeeCost, "caffe");
    errorCheck(&transportCost, "transport");
    errorCheck(&uniDinCost, "institute");
    errorCheck(&homeFoodCost, "home food");
    errorCheck(&otherFoodCost, "other costs");

    return workDays * StudentService::getWorkdayCost() +
        weekends * StudentService::getWeekandCost() + 
        otherFoodCost;
}

void StudentService::getMonth() {

    int month = 0;
    do
    {
        std::cout << "What month is it?" << std::endl;
        std::cin >> month;
    } while (!StudentService::isMonth(month));
    this->month = month;
}
/*void StudentService::linkDataBase(DataBase *base)
{
    this->dataBase = base;
}*/

void StudentService::setMonth(uint month)
{
    this->month = month;
}

uint setAge()
{
    uint age;
    std::cout << "Your age is: ";
    std::cin >> age;
    return age;
}

std::string setCity()
{
    std::string city;
    std::cout << "Your city is: ";
    std::cin >> city;
    return city;
}

std::string setAdress()
{
    std::string adress;
    std::cout << "Your adress is: ";
    std::cin >> adress;
    return adress;
}

std::string setInstitute()
{
    std::string institute;
    std::cout << "Your institute is: ";
    std::cin >> institute;
    return institute;
}

std::string setCoffee()
{
    std::string caffe;
    std::cout << "Your caffe is: ";
    std::cin >> caffe;
    return caffe;
}

std::string setCinema()
{
    std::string cinema;
    std::cout << "Your cinema is: ";
    std::cin >> cinema;
    return cinema;
}

void fillStudent(Student *student)
{
    student->city = setCity();
    student->adress = setAdress();
    student->institute = setInstitute();
    student->caffe = setCoffee();
    student->cinema = setCinema();
    student->age = setAge();
}