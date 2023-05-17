#include <string>

typedef unsigned int uint;

class DataBase {
    public:
        DataBase(const std::string& cacPath,const  std::string& newCostsPath,
            const std::string& newInstitutesPath, const std::string& newTransportsPath);
        std::string getMiscCosts(const std::string&, const uint);
        std::string getEstCost(const std::string&, const std::string&, const std::string&);
        uint getInstituteDinnerCost(const std::string&, const std::string&);
        uint getTransportCost(const std::string&, const std::string&, const std::string&);

    private:
        std::string caffeAndCinemaPath;
        std::string costsPath;
        std::string institutesPath;
        std::string transportsPath;
};