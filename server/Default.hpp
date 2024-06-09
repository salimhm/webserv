#pragma once

#include <iostream>

class Default
{
    private:

    public:
        Default();
        ~Default();

        const std::string generateErrorPage(const std::string& errorCode);
        const std::string generateIndexPage();
};
const std::string getErrorMessage(const std::string& errorCode);