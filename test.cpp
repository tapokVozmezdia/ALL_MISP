#include <iostream>
#include "studentService.h"

int main()
{
    //DataBase newBase1("Caffe-and-cinema.csv", "Costs.csv", "Institute.csv", "Transport.csv");
    //std::cout << newBase1.getCinemaCost("Moscow", "Almaz") << std::endl;
    
    std::cout << "Would you like to set custom paths for the datatables?" << std::endl;
    std::cout << "Press 1 to continue with default" << std::endl;
    std::cout << "Press 2 to set custom parameters" << std::endl;
    uint flag;
    std::string ca_path = "Caffe-and-cinema.csv";
    std::string co_path = "Costs.csv";
    std::string in_path = "Institute.csv";
    std::string tr_path = "Transport.csv";
    do
    {
        std::cin >> flag;
        if (flag == 2)
        {
            std::cout << "!!NOTE: enter only names of the files, without the full path!!" 
            << std::endl;
            std::cout << "Path for *caffe and cinemas* table: ";
            std::cin >> ca_path;
            std::cout << "Path for *costs* table: ";
            std::cin >> co_path;
            std::cout << "Path for *transport* table: ";
            std::cin >> tr_path;
            std::cout << "Path for *institutes* table: ";
            std::cin >> in_path;
            std::cout << std::endl;
        }
    } while(flag != 1 && flag!= 2);
    DataBase newBase(ca_path, co_path, in_path, tr_path);
    StudentService service(&newBase);
    service.getMonth();
    Student newStudent;
    fillStudent(&newStudent);
    const uint totalCost = service.getCosts(newStudent.city, newStudent.adress, newStudent.institute,
        newStudent.cinema, newStudent.caffe, newStudent.age);
    std::cout << "Total cost is: " << totalCost << std::endl;
    return 0;
}