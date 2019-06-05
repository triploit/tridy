//
// Created by survari on 29.12.18.
//

#ifndef TRIDYMITE_IPACKAGESMANAGER_HPP
#define TRIDYMITE_IPACKAGESMANAGER_HPP


#include <vector>
#include <package/package.hpp>

class IPackagesManager
{
private:
    static inline std::vector<Package> installed_packages;

public:
    static void load(std::string path, bool clear_all=true);
    static const std::vector<Package> &getInstalledPackages();
    static const Package &getPackage(Package &p);
    static bool isPackageInstalled(const Package &p);
    static bool isPackageInstalledNV(const Package &p);
    static std::string getPackageFile(const Package &p);

    static void setInstalledPackages(const std::vector<Package> &packages);
};


#endif //TRIDYMITE_IPACKAGESMANAGER_HPP
