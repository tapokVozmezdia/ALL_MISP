#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "studentService.h"

void notFoundError(const std::string& parameter)
{
    std::cout << "Data not found in given table : parameter " << parameter << std::endl;
}

std::ifstream DatabaseHandler;
typedef unsigned int uint;

DataBase::DataBase(const std::string& cacPath, const std::string& newCostsPath,
    const std::string& newInstitutesPath,const std::string& newTransportsPath)
{
    this->caffeAndCinemaPath = cacPath;
    this->costsPath = newCostsPath;
    this->institutesPath = newInstitutesPath;
    this->transportsPath = newTransportsPath;
    //std::cout << costsPath << std::endl;
}

std::string DataBase::getMiscCosts(const std::string& city, const uint age)
{
    DatabaseHandler.open(costsPath);
    //std::cout << costsPath << std::endl;
    if (!DatabaseHandler.is_open())
    {
        std::cout << "File opening error COSTS " << std::endl;
        //std::cout << costsPath << std::endl;
        return 0;
    }
    std::string parameter = "COSTS";
    std::string dump;
    std::string line;
    std::string t_city;
    std::string t_age;
    std::string t_cost;
    getline(DatabaseHandler, line); // пропускаем 1-ую строку
    while(getline(DatabaseHandler, line))
    {
        //std :: cout << line << std::endl;
        std::stringstream s(line);
        getline(s, t_city, ',');
        getline(s, t_age, ',');
        int i_age = stoi(t_age);
        //std::cout << age << " " << i_age << city << t_city << std::endl;
        if (i_age == age && t_city == city)
        {

            getline(s, dump, ',');
            getline(s, t_cost);
            DatabaseHandler.close();
            return dump + "I" + t_cost;
            //std::cout << i_age << std::endl;
        }
        getline(s, dump);
    }
    notFoundError(parameter);
    DatabaseHandler.close();
    return "-1";
}

std::string DataBase::getEstCost(const std::string& city, const std::string& estCin, const std::string& estCaf)
{
    DatabaseHandler.open(caffeAndCinemaPath);
    if (!DatabaseHandler.is_open())
    {
        std::cout << "File opening error" << std::endl;
        return 0;
    }
    std::string parameter;
    parameter = "Est costs";
    std::string dump;
    std::string line;
    std::string t_city;
    std::string t_coffee;
    std::string t_cinema;
    std::string t_cin_cost;
    std::string t_caf_cost;
    std::string p_cin_cost;
    std::string p_caf_cost;
    int score = 0;
    getline(DatabaseHandler, line); // пропускаем 1-ую строку
    while(getline(DatabaseHandler, line))
    {
        //std :: cout << line << std:: endl;
        std::stringstream s(line);
        getline(s, t_city, ',');
        getline(s, dump, ',');
        getline(s, t_coffee, ',');
        getline(s, t_caf_cost, ',');
        getline(s, t_cinema, ',');
        getline(s, t_cin_cost);
        if (estCaf == t_coffee)
        {
            score++;
            p_caf_cost = t_caf_cost;
        }
        if (estCin == t_cinema)
        {
            score++;
            p_cin_cost = t_cin_cost;
        }
        if (score == 2)
        {
            DatabaseHandler.close();
            return p_caf_cost + "I" + p_cin_cost;
        }
    }
    notFoundError(parameter);
    DatabaseHandler.close();
    return "-1";
}

uint DataBase::getInstituteDinnerCost(const std::string& city, const std::string& institute)
{
    DatabaseHandler.open(institutesPath);
    if (!DatabaseHandler.is_open())
    {
        std::cout << "File opening error" << std::endl;
        return 0;
    }
    std::string parameter = "institute cost";
    std::string line;
    std::string t_city;
    std::string t_inst;
    std::string t_cost;
    getline(DatabaseHandler, line); //Скип первой строки
    while(getline(DatabaseHandler, line))
    {
        std::stringstream s(line);
        getline(s, t_city, ',');
        getline(s, t_inst, ',');
        getline(s, t_cost);
        int i_cost = stoi(t_cost);
        if (institute == t_inst && t_city == city)
        {
            DatabaseHandler.close();
            return static_cast<uint>(i_cost);
        }
    }
    notFoundError(parameter);
    DatabaseHandler.close();
    return -1;
}

uint DataBase::getTransportCost(const std::string& city, const std::string& homeAddress,
            const std::string& institute)
{
    DatabaseHandler.open(transportsPath);
    if (!DatabaseHandler.is_open())
    {
        std::cout << "File opening error" << std::endl;
        return 0;
    }
    std::string parameter = "transport cost";
    std::string line;
    std::string t_city;
    std::string t_dist;
    std::string t_inst;
    std::string t_cost;
    getline(DatabaseHandler, line); //Скип первой строки
    while(getline(DatabaseHandler, line))
    {
        //std::cout << line << std::endl;
        std::stringstream s(line);
        getline(s, t_city, ',');
        getline(s, t_dist, ',');
        getline(s, t_inst, ',');
        getline(s, t_cost);
        //std::cout << "BEFORE COST" << std::endl;
        //std::cout << t_cost << std::endl;
        int i_cost = stoi(t_cost);
        //std::cout << "READ COST" << std::endl;
        if (institute == t_inst && city == t_city && t_dist == homeAddress)
        {
            DatabaseHandler.close();
            return static_cast<uint>(i_cost);
        }
    }
    notFoundError(parameter);
    DatabaseHandler.close();
    return -1;
}