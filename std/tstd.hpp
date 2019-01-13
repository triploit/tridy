#ifndef TRIDYMITE_TSTD_HPP
#define TRIDYMITE_TSTD_HPP

#include <iostream>
#include <vector>
#include <package/package.hpp>
#include <runtime.hpp>
#include <dirent.h>

namespace tstd
{
    Package parse_package(const std::string &package);

    std::string add_prefix(std::string arg); // Add a prefix (- or --) to argument (like "help" -> "--help"; "i" -> "-i")
    std::string create_url(const Package &p, std::string postfix="", std::string prefix="");
    std::string package_to_argument(const Package &p);
    std::string exec(const char* cmd);

    bool download_file(const std::string &url, const std::string &destination);
    int cursive_file_count(const std::string &path, int count=1);

    std::vector<Package> parse_package_arguments(const std::vector<std::string> &packages);
    std::vector<std::string> split(std::string s, char delim); // Split a string by a delimiter
    std::vector<std::string> get_family(std::string arg, std::vector<std::string> arg_name); // Getting the family of an argument, e.g. "i" is contained in the family of "i,install"
    std::vector<std::string> read_cursive_all_files(std::string path);
};


#endif //TRIDYMITE_TSTD_HPP
