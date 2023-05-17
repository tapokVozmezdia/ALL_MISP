#include <iostream>
#include <string>
#include <fstream>

typedef unsigned int uint;

std::ifstream DatabaseHandler;

// Оценка расходов студента, руб./ месяц на основе "статистических данных"

class DataBase
{
    public:
        DataBase() {

        }

        bool initDatabase() {
            // Инициализация базы данных
            numOfEst = countObjectAmount("Caffe-and-cinema.csv");
            numOfMisc = countObjectAmount("Costs.csv");
            numOfInst = countObjectAmount("Institute.csv");
            numOfTrans = countObjectAmount("Transport.csv");
            if (numOfEst == -1 || numOfInst == -1 || numOfTrans == -1 || numOfMisc == -1) {
                return false;
            }
            caffes = new Caffe[numOfEst];
            cinemas = new Cinema[numOfEst];
            institutes = new Institute[numOfInst];
            transports = new Transport[numOfTrans];
            costs = new MiscCosts[numOfMisc];
            if (caffes == NULL || cinemas == NULL || institutes == NULL
                || transports == NULL || costs == NULL) 
            {
                std::cout <<"Memory alloctation error" << std::endl;
                return false;
            }
            fillEstablishments(caffes, cinemas, numOfEst);
            fillInstitutes(institutes, numOfInst);
            fillTransport(transports, numOfTrans);
            fillMisc(costs, numOfMisc);
            return true;
        }

        void closeDatabase()
        {
            delete caffes;
            delete cinemas;
            delete institutes;
            delete transports;
            delete costs;
        }

        void testPrintCost(std::string caffeName)
        {
            for (int i = 0; i < numOfEst; ++i) {
                if (caffes[i].getCaffeName() == caffeName)
                {
                    std::cout << caffes[i].getCaffeCost();
                }
            }
        }

        uint getOtherMontlyCosts(uint month, const std::string& city,
            uint age)
        {
            /* Получаем из базы данных средние по региону затраты в данном месяце 
            для людей данного возраста
            (по остальным статьям кроме еды, транспорта и развлечений) */
            for (int i = 0; i < numOfMisc; ++i)
            {
                if (costs[i].getCityName() == city && costs[i].getAge() == age)
                {
                    return static_cast<uint>(costs[i].getOtherCosts());
                }
            }
            return 0;
        }

        uint getHomeFoodCost(const std::string& city, uint age) {
            // Получаем из базы данных средние по региону затраты на еду
            for (int i = 0; i < numOfMisc; ++i)
            {
                if (costs[i].getCityName() == city && costs[i].getAge() == age)
                {
                    return static_cast<uint>(costs[i].getMonthlyFoodCosts());
                } 
            }
            return 0;
        }

        uint getCinemaCost(const std::string& city, const std::string& cinema) {
            // Получаем из базы данных стоимость билета на вечерний сеанс в данном кинотетре
            for (int i = 0; i < numOfEst; ++i)
            {
                if (cinemas[i].getCityName() == city && cinemas[i].getCinemaName() == cinema)
                {
                    return static_cast<uint>(cinemas[i].getCinemaCost());
                }
            }
            return 0;
        }

        uint getCoffeeCost(const std::string& city, const std::string& coffee) {
            // Получаем из базы данных средний чек в данном баре
            for (int i = 0; i < numOfEst; ++i)
            {
                if (caffes[i].getCityName() == city && caffes[i].getCaffeName() == coffee)
                {
                    return static_cast<uint>(caffes[i].getCaffeCost());
                }
            }
            return 0;
        }

        uint getWeekandCost(const std::string& city, const std::string& cinema,
                            const std::string& coffee, uint age) {
            /* По выходным дням расходы складываются из стоимости:
            - похода в кино (пешком)
            - похода  в кафе (пешком)
            - завтрака и обеда дома */

            return static_cast<uint>(0.66 * getHomeFoodCost(city, age) + 
                getCinemaCost(city, cinema) + getCoffeeCost(city, coffee) + 0.5);
        }

        uint getInstituteDinnerCost(const std::string& city, const std::string& institute) {
            // Получаем из базы данных среднюю стоимость обеда в столовой данного института
            for (int i = 0; i < numOfInst; ++i)
            {
                if (institutes[i].getCityName() == city && institutes[i].getInstituteName() == institute)
                {
                    return static_cast<uint>(institutes[i].getDinnerCost());
                }
            }
            return 0;
        }

        uint getTransportCost(const std::string& city, const std::string& homeAddress,
                            const std::string& institute) {
            // Получаем из базы данных стоимость кратчайшей дороги до инстиутта
            for (int i = 0; i < numOfTrans; ++i)
            {
                if (transports[i].getCityName() == city && transports[i].getDistrictName() == homeAddress
                    && transports[i].getDestinationInstitute() == institute)
                {
                    return static_cast<uint>(transports[i].getTransportCost());
                }
            }
            return 0;
        }

        uint getWorkdayCost(const std::string& city, const std::string& homeAddress,
                            const std::string& institute, uint age) {
        /* По рабочим дням расходы складываются из стоимости:
            - дороги до института и обратно
            - обеда в институтской столовой
            - завтрака и обеда дома */



            return static_cast<uint>(2 * getTransportCost(city, homeAddress, institute) +
                getInstituteDinnerCost(city, institute) + 0.66 * getHomeFoodCost(city, age) + 0.5);
        }

        uint getWorkdays(uint month) {
        // Получаем число рабочих дней данном в месяце
            if (month == 1)
            {
                return 17;
            }
            if (month == 2 || month == 6)
            {
                return 19;
            }
            if (month == 3 || month == 5 || month == 9)
            {
                return 20;
            }
            if (month == 4 || month > 10)
            {
                return 21;
            }
            return 22;
        }

        uint getDaysCount(uint month) {
            // Вычисляем количество дней в месяце
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

        uint getCosts(uint month, const std::string& city, const std::string& homeAddress, 
                    const std::string& institute, const std::string& cinema,
                    const std::string& coffee, uint age) {
            /* Расходы:
            - по рабочим дням
            - по выходным дням
            - прочие расходы */

            const uint daysCount = getDaysCount(month);
            const uint workDays = getWorkdays(month);
            const uint weekends = daysCount - workDays;

            return workDays * getWorkdayCost(city, homeAddress, institute, age) +
                weekends * getWeekandCost(city, cinema, coffee, age) + 
                getOtherMontlyCosts(month, city, age);
        }

        bool availableCinema(std::string cinema, std::string city)
        {
            for(int i = 0; i < numOfEst; ++i)
            {
                if (cinemas[i].getCinemaName() == cinema && cinemas[i].getCityName() == city)
                {
                    return true;
                }
            }
            std::cout <<"This cinema theater is not mentioned in the table, please retry" << std::endl;
            return false;
        }

        std::string getCinema(const std::string& city) {
            // Аналогично getInstitute
            std::string cinema;
            do {
                std::cout << "Enter the name of the cinema you want to go to" << std::endl;
                std::cin >> cinema;
            } while(availableCinema(cinema, city) == false);
            return cinema;
        }

        bool availableCaffe(std::string caffe, std::string city)
        {
            for(int i = 0; i < numOfEst; ++i)
            {
                if (caffes[i].getCaffeName() == caffe && caffes[i].getCityName() == city)
                {
                    return true;
                }
            }
            std::cout <<"This caffe is not mentioned in the table, please retry" << std::endl;
            return false;
        }

        std::string getCoffee(const std::string& city) {
            // Аналогично getInstitute

            std::string caffe;
            do {
                std::cout << "Enter the name of the caffe you want to get coffee in" << std::endl;
                std::cin >> caffe;
            } while(availableCaffe(caffe, city) == 0);
            return caffe;
        }

        bool availableInstitute(std::string institute, std::string city)
        {
            for(int i = 0; i < numOfInst; ++i)
            {
                if (institutes[i].getInstituteName() == institute && 
                    institutes[i].getCityName() == city)
                {
                    return true;
                }
            }
            std::cout <<"This institute is not mentioned in the table, please retry" << std::endl;
            return false;
        }

        std::string getInstitute(const std::string& city) {
            // Ввод названия института
            std::string institute;
            do {
                std::cout << "Enter the name of your institute" << std::endl;
                std::cin >> institute;
            } while(availableInstitute(institute, city) == 0);
            return institute;
        }

        bool availableAdress(std::string adress, std::string city)
        {
            for(int i = 0; i < numOfEst; ++i)
            {
                if ((caffes[i].getDistrictName() == adress) && (caffes[i].getCityName() == city) || 
                    (cinemas[i].getDistrictName() == adress) && (cinemas[i].getCityName() == city))
                {
                    return true;
                }
            }
            for(int i = 0; i < numOfMisc; ++i)
            {
                if (transports[i].getDistrictName() == adress && transports[i].getCityName() == city)
                {
                    return true;
                }
            }
            std::cout <<"This adress is not mentioned in the table, please retry" << std::endl;
            return false;
        }

        std::string getAddress(const std::string& city) {
            // Запрашиваем ввод адреса

            std::string adress;
            do {
                std::cout << "Enter your adress" << std::endl;
                std::cin >> adress;
            } while(availableAdress(adress, city) == 0);
            return adress;
        }

        bool availableAge(int age)
        {
            for(int i = 0; i < numOfMisc; ++i)
            {
                if (costs[i].getAge() == age)
                {
                    return true;
                }
            }
            std::cout <<"This age is not mentioned in the table, please retry" << std::endl;
            return false;
        }

        void printAllAges()
        {
            for (int i = 0; i < numOfMisc; ++i)
            {
                std:: cout << costs[i].getAge() << std::endl;
            }
        }

        void printAllCinemas()
        {
            for (int i = 0; i < numOfEst; ++i)
            {
                std:: cout << cinemas[i].getCinemaName() << std::endl;
            }
        }

        uint getAge() {
            // Запрашиваем ввод возраста для расчёта расходов 

            int age = 0;
            do {
                std::cout << "What is your age?" << std::endl;
                std::cin >> age;
            } while(availableAge(age) == false);
            
            return age;
        }

        bool availableCity(std::string city)
        {
            for(int i = 0; i < numOfEst; ++i)
            {
                if (caffes[i].getCityName() == city || cinemas[i].getCityName() == city)
                {
                    return true;
                }
            }
            for(int i = 0; i < numOfMisc; ++i)
            {
                if (costs[i].getCityName() == city)
                {
                    return true;
                }
            }
            for(int i = 0; i < numOfInst; ++i)
            {
                if (institutes[i].getCityName() == city)
                {
                    return true;
                }
            }
            for(int i = 0; i < numOfTrans; ++i)
            {
                if (transports[i].getCityName() == city)
                {
                    return true;
                }
            }
            std::cout <<"This city is not mentioned in the table, please retry" << std::endl;
            return false;
        }

        std::string getCity() {
            // Запрашиваем ввод города

            std::string city;
            do 
            {
                std::cout << "What is your city?" << std::endl;
                std::cin >> city;
            } while(availableCity(city) == false);
            return city;
        }

        bool isMonth(int month) {

            if (month > 0 && month < 13) {
                return true;
            }
            else {
                std::cout << "Incorrect input, please retry" << std::endl;
                return false;
            }

            // Проверка: может ли введенное число интерпретироваться как месяц?
        }

        uint getMonth() {
            // Запрашиваем ввод месяца для расчёта расходов 

            int month = 0;
            do
            {
                std::cout << "What month is it?" << std::endl;
                std::cin >> month;
            } while(!isMonth(month));

            return month;
        }

    private:

        class TableObject
        {
            public:
                std::string getCityName()
                {
                    return cityName;
                }
                void setCityName(std::string newCityName)
                {
                    cityName = newCityName;
                }
            private:
                std::string cityName;
        };

        class Caffe: public TableObject
        {
            public:
                std::string getCaffeName()
                {
                    return caffeName;
                }
                int getCaffeCost()
                {
                    return caffeCost;
                }
                std::string getDistrictName()
                {
                    return districtName;
                }
                void setCaffeCost(int cost)
                {
                    caffeCost = cost;
                }
                void setDistrictName(std::string newDistrictName)
                {
                    districtName = newDistrictName;
                }
                void setCaffeName(std::string newCaffeName)
                {
                    caffeName = newCaffeName;
                }
            private:
                int caffeCost;
                std::string districtName;
                std::string caffeName;
        };
        
        class Cinema: public TableObject
        {
            public:
                int getCinemaCost()
                {
                    return cinemaCost;
                }
                std::string getDistrictName()
                {
                    return districtName;
                }
                std::string getCinemaName()
                {
                    return cinemaName;
                }
                void setCinemaCost(int cost)
                {
                    cinemaCost = cost;
                }
                void setDistrictName(std::string newDistrictName)
                {
                    districtName = newDistrictName;
                }
                void setCinemaName(std::string newCinemaName)
                {
                    cinemaName = newCinemaName;
                }
            private:
                int cinemaCost;
                std::string districtName;
                std::string cinemaName;
        };

        class Institute: public TableObject
        {
            public:
                int getDinnerCost()
                {
                    return dinnerCost;
                }
                std::string getInstituteName()
                {
                    return instituteName;
                }
                void setDinnerCost(int newCost)
                {
                    dinnerCost = newCost;
                }
                void setInstituteName(std::string newName)
                {
                    instituteName = newName;
                }
            private:
                std::string instituteName;
                int dinnerCost;
        };

        class Transport: public TableObject
        {
            public:
                int getTransportCost()
                {
                    return transportCost;
                }
                std::string getDistrictName()
                {
                    return disctrictName;
                }
                std::string getDestinationInstitute()
                {
                    return destinationInstitute;
                }
                void setTransportCost(int newCost)
                {
                    transportCost = newCost;
                }
                void setDistrictName(std::string newDisctrictName)
                {
                    disctrictName = newDisctrictName;
                }
                void setDestinationInstitute(std::string newDestinationInstitute)
                {
                    destinationInstitute = newDestinationInstitute;
                }
            private:
                int transportCost;
                std::string disctrictName;
                std::string destinationInstitute;
        }; 

        class MiscCosts: public TableObject
        {
            public:
                int getAge()
                {
                    return age;
                }
                int getMonthlyFoodCosts()
                {
                    return monthlyFoodCost;
                }
                int getOtherCosts()
                {
                    return otherCosts;
                }
                void setAge(int newAge)
                {
                    age = newAge;
                }
                void setMonthlyFoodCosts(int newMonthlyFoodCosts)
                {
                    monthlyFoodCost = newMonthlyFoodCosts;
                }
                void setOtherCosts(int newOtherCosts)
                {
                    otherCosts = newOtherCosts;
                }
            private:
                int age;
                int monthlyFoodCost;
                int otherCosts;
        };

        int countObjectAmount(std::string path) {
            DatabaseHandler.open(path);
            if (!DatabaseHandler.is_open()) {
                std::cout << "File opening error" << std::endl;
                return -1;
            }
            std::string line;
            uint amount = 0;
            while (getline(DatabaseHandler, line)) {
                ++amount;
            }
            DatabaseHandler.close();
            if (amount == 1) {
                std::cout << "Empty datatable given" << std::endl;
                return -1;
            }
            return amount - 1;
        }

        void fillEstablishments(Caffe *caffes, Cinema *cinemas, uint numOfEst)
        {
            DatabaseHandler.open("Caffe-and-cinema.csv");
            if (!DatabaseHandler.is_open()) {
                std::cout << "File opening error" << std::endl;
                return;
            }
            std::string line;
            uint lineNum = 0;
            std::string words[6];
            int flag = 0;
            while(getline(DatabaseHandler, line)) {
                if (flag == 0) {
                    flag = 1;
                    continue;
                }
                char ch = ' ';
                int i = 0;
                char word[128];
                uint symNum = 0;
                uint wordNum = 0;
                while(i < line.length()) {
                    //std::cout << line[i];
                    if (line[i] == ',' && wordNum != 5) {
                        word[symNum] = '\0';
                        words[wordNum] = word;
                        //std::cout<<word<<" ";
                        wordNum++;
                        symNum = 0;
                    }
                    else {
                        word[symNum] = line[i];
                        symNum++;
                    }
                    ++i;
                }
                word[symNum] = '\0';
                words[wordNum] = word;
                //std::cout<<word<<" ";
                //std::cout << std::endl;
                caffes[lineNum].setCityName(words[0]);
                caffes[lineNum].setDistrictName(words[1]);
                caffes[lineNum].setCaffeName(words[2]);
                caffes[lineNum].setCaffeCost(stoi(words[3]));
                cinemas[lineNum].setCityName(words[0]);
                cinemas[lineNum].setDistrictName(words[1]);
                cinemas[lineNum].setCinemaName(words[4]);
                cinemas[lineNum].setCinemaCost(stoi(words[5]));
                lineNum++;
            }
            DatabaseHandler.close();
        }

        void fillInstitutes(Institute *institutes, uint numOfInst)
        {
            DatabaseHandler.open("Institute.csv");
            if (!DatabaseHandler.is_open()) {
                std::cout << "File opening error" << std::endl;
                return;
            }
            std::string line;
            uint lineNum = 0;
            std::string words[3];
            int flag = 0;
            while(getline(DatabaseHandler, line)) {
                if (flag == 0) {
                    flag = 1;
                    continue;
                }
                char ch = ' ';
                int i = 0;
                char word[128];
                uint symNum = 0;
                uint wordNum = 0;
                while(i < line.length()) {
                    if (line[i] == ',' && wordNum != 2) {
                        word[symNum] = '\0';
                        words[wordNum] = word;
                        wordNum++;
                        symNum = 0;
                    }
                    else {
                        word[symNum] = line[i];
                        symNum++;
                    }
                    ++i;
                }
                word[symNum] = '\0';
                words[wordNum] = word;
                institutes[lineNum].setCityName(words[0]);
                institutes[lineNum].setInstituteName(words[1]);
                institutes[lineNum].setDinnerCost(stoi(words[2]));
                lineNum++;
            }
            DatabaseHandler.close();
        }

        void fillTransport(Transport *transports, uint numOfTrans)
        {
            DatabaseHandler.open("Transport.csv");
            if (!DatabaseHandler.is_open()) {
                std::cout << "File opening error" << std::endl;
                return;
            }
            std::string line;
            uint lineNum = 0;
            std::string words[4];
            int flag = 0;
            while(getline(DatabaseHandler, line)) {
                if (flag == 0) {
                    flag = 1;
                    continue;
                }
                char ch = ' ';
                int i = 0;
                char word[128];
                uint symNum = 0;
                uint wordNum = 0;
                while(i < line.length()) {
                    if (line[i] == ',' && wordNum != 3) {
                        word[symNum] = '\0';
                        words[wordNum] = word;
                        wordNum++;
                        symNum = 0;
                    }
                    else {
                        word[symNum] = line[i];
                        symNum++;
                    }
                    ++i;
                }
                word[symNum] = '\0';
                words[wordNum] = word;
                transports[lineNum].setCityName(words[0]);
                transports[lineNum].setDistrictName(words[1]);
                transports[lineNum].setDestinationInstitute(words[2]);
                transports[lineNum].setTransportCost(stoi(words[3]));
                lineNum++;
            }
            DatabaseHandler.close();
        }

        void fillMisc(MiscCosts *misc, uint numOfMisc)
        {
            DatabaseHandler.open("Costs.csv");
            if (!DatabaseHandler.is_open()) {
                std::cout << "File opening error" << std::endl;
                return;
            }
            std::string line;
            uint lineNum = 0;
            std::string words[4];
            int flag = 0;
            while(getline(DatabaseHandler, line)) {
                if (flag == 0) {
                    flag = 1;
                    continue;
                }
                char ch = ' ';
                int i = 0;
                char word[128];
                uint symNum = 0;
                uint wordNum = 0;
                while(i < line.length()) {
                    if (line[i] == ',' && wordNum != 3) {
                        word[symNum] = '\0';
                        words[wordNum] = word;
                        wordNum++;
                        symNum = 0;
                    }
                    else {
                        word[symNum] = line[i];
                        symNum++;
                    }
                    ++i;
                }
                word[symNum] = '\0';
                words[wordNum] = word;
                misc[lineNum].setCityName(words[0]);
                misc[lineNum].setAge(stoi(words[1]));
                misc[lineNum].setMonthlyFoodCosts(stoi(words[2]));
                misc[lineNum].setOtherCosts(stoi(words[3]));
                lineNum++;
            }
            DatabaseHandler.close();
        }

        uint numOfEst = 0; //Establishments = Caffes and cinemas
        uint numOfMisc = 0;
        uint numOfInst = 0;
        uint numOfTrans = 0;

        Caffe *caffes;
        Cinema *cinemas;
        Institute *institutes;
        Transport *transports;
        MiscCosts *costs;
};

int main() { 
    DataBase DB;
    DB.initDatabase();
    //DB.printAllAges();
    const uint month = DB.getMonth();
    const uint age = DB.getAge();
    const std::string city = DB.getCity();
    const std::string homeAddress = DB.getAddress(city);
    const std::string institute = DB.getInstitute(city);
    const std::string coffee = DB.getCoffee(city);
    //DB.printAllCinemas();
    const std::string cinema = DB.getCinema(city);

    const uint montlyCosts = DB.getCosts(month, city, homeAddress, institute, 
        cinema, coffee, age);

    std::cout << "Your total monthly expenses: " << montlyCosts << std::endl;
    DB.closeDatabase();

    //DB.testPrintCost("Capuchinoff");
    return 0;
}
