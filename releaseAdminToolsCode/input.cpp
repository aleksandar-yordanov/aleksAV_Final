#include "input.h"

int input::mainInput()
{
    std::string state;
    std::cout << "Administration panel options:\n-- User Management    (1) --\n-- License Management (2) --\n-- Exit               (0) --\n> Enter Choice Here:";
    std::cin >> state;
    try
    {
        int retState = std::stoi(state);
        if(!(retState < 0 || retState > 2))
        {
            return retState;
        }
        else
        {
            std::cout << "-- Invalid Input --" << std::endl;
            return 0;
        }
    }
    catch (std::exception& e)
    {
        std::cout << "-- Invalid Input --" << std::endl;
        return 0;
    }
}

int input::licenseInput()
{
    std::string state;
    std::cout << "Upgrade user options:\n-- No licese        (1) --\n-- Standard license (2) --\n-- Premium license  (3) --\n-- Exit             (0) --\n> Enter Choice Here:";
    std::cin >> state;
    try
    {
        int retState = std::stoi(state);
        if(!(retState < 0 || retState > 3))
        {
            return retState;
        }
        else
        {
            std::cout << "-- Invalid Input --" << std::endl;
            return 0;
        }
    }
    catch(std::exception& e)
    {
        std::cout << "-- Invalid Input --" << std::endl;
        return 0;
    }
}
