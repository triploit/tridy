//
// Created by survari on 16.04.19.
//

#ifndef TRIDYMITE_PRETYPE_HPP
#define TRIDYMITE_PRETYPE_HPP


#include <script/script.hpp>
#include <variable/variable.hpp>

class PreType
{
private:
    std::string name;
    std::string full_file_name;
    std::vector<std::string> var_names;

public:
    PreType(const std::string &file, const std::string &name);

    void runScript(const std::vector<Variable> &variables, const std::string &token);

    const std::string &getName() const;
    const std::string &getFullFileName() const;
    const std::vector<std::string> &getNeededVariables();
};


#endif //TRIDYMITE_PRETYPE_HPP
