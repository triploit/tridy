#include <iostream>

#include "runtime.hpp"
#include "cli/cli.hpp"

#include <std/arguments.hpp>
#include <manager/packages/ipackage_manager.hpp>
#include <manager/install/installation_manager.hpp>
#include <algorithm>
#include <manager/dependencies/dependency_manager.hpp>
#include <manager/remove/remove_manager.hpp>
#include <manager/update/update_manager.hpp>

#define _VERSION "0.1.1a"

int main(int argc, char* argv[])
{
    Runtime::init();

    if (tstd::exec("whoami") == "root\n")
    {
        std::cout << "error: never run tridymite as root!" << std::endl;
        Runtime::exit(1);
    }

    CLI cli;
    cli.init(__ARG_NAME,
            __ARG_HELP,
            __ARG_LENGTH,
            "tridymite",
            Version(_VERSION));

    bool working = cli.parseArguments(std::vector<std::string>(argv + 1, argv + argc)); // Parsing the given arguments, returns a boolean (false if there are errors)

    if (!working || argc == 1) // If there are errors or there is only one argument, we're printing the help page.
    {
        if (!working)
            std::cout << std::endl;

        if (cli.argumentGiven("lf"))
            Translation::loadConfig(cli.getParameters("lf")[0]);

        cli.printHelp(argv[0]);
        Runtime::exit(1);
    }
    else
    {
        if (cli.argumentGiven("l"))
        {
            struct stat info;
            if(stat((std::string(getenv("HOME"))+"/.local/").c_str(), &info) != 0)
            {
                std::cout << "error: can't use operator \"-l/--local\": no local installation module in setup activated!" << std::endl;
                Runtime::exit(1);
            }

            Runtime::try_local = true;
            Runtime::tridy_dir = std::string(getenv("HOME")) + "/.local/share/tridymite";

            Runtime::reloadManagers();
        }

        if (cli.argumentGiven("lf")) // Set language file
            Translation::loadConfig(cli.getParameters("lf")[0]); // Loading the language file

        if (cli.argumentGiven("h")) // Printing the help page
            cli.printHelp("");

        if (cli.argumentGiven("lp")) // List all installed packages
        {
            std::vector<std::string> msgs; // Vector of messages

            for (const Package &installed_package : IPackagesManager::getInstalledPackages())
            {
                std::string s;
                if (Runtime::try_local)
                    s = " (local)";

                msgs.push_back(
                        "   "+s+" - "+installed_package.getGitUser()+
                        " -> "+installed_package.getRepoName()+
                        " @ "+installed_package.getServer()+
                        " - v"+installed_package.getVersion().str); // Add every message to vector
            }

            std::sort(msgs.begin(), msgs.end()); // Sort messages alphabet

            if (msgs.size() > 0)
            {
                if (Runtime::try_local)
                    std::cout << "Local packages installed: " << std::endl;
                else
                    std::cout << "Packages installed: " << std::endl;


                for (const std::string &s : msgs) // Printing all messages
                    std::cout << s << std::endl;
            }
            else
            {
                if (Runtime::try_local)
                    std::cout << "No local packages installed." << std::endl;
                else
                    std::cout << "No packages installed." << std::endl;
            }
        }

        if (cli.argumentGiven("d"))
        {
            Package p = tstd::parse_package(cli.getParameters("d")[0]);

            if (IPackagesManager::isPackageInstalled(p))
                std::cout << IPackagesManager::getPackage(p) << std::endl;
            else
            {
                std::cout << "info: show description: couldn't find package " << cli.getParameters("d")[0] << std::endl;
                std::cout << "info: trying to find it online..." << std::endl;

                Package package = DependencyManager::getPackageConfig({p})[0];
                std::cout << package << std::endl;
            }
        }

        if (cli.argumentGiven("version"))
        {
            std::cout << _VERSION << std::endl;
        }

        // Setting the variables

        if (cli.argumentGiven("ua"))
        {
            Runtime::update_all = true;

            Runtime::to_update.insert(Runtime::to_update.end(),
                    IPackagesManager::getInstalledPackages().begin(),
                    IPackagesManager::getInstalledPackages().end());
        }

        if (cli.argumentGiven("us"))
            Runtime::git_user = cli.getParameters("us")[0]; // Setting the standard git user

        if (cli.argumentGiven("s"))
            Runtime::git_server = cli.getParameters("s")[0]; // Setting the standard git server

        if (cli.argumentGiven(("e")))
            Runtime::verbose = true; // Show verbose output

        if (cli.argumentGiven(("n"))) // Don't ask security questions
            Runtime::insecure = true;

        if (cli.argumentGiven("a"))
            Runtime::reinstall = true;

        if (cli.argumentGiven("f"))
            Runtime::force = true;

        if (cli.argumentGiven("nc"))
            Runtime::no_dependencies = true;

        // Adding the packages

        if (cli.argumentGiven("i")) // Check if argument -i/--install is used
        {
            std::vector<Package> pkgs = tstd::parse_package_arguments(cli.getParameters("i")); // Get all parameters and convert them to packages
            Runtime::to_install.insert(Runtime::to_install.end(), pkgs.begin(), pkgs.end()); // Add Packages to install
        }

        if (cli.argumentGiven("u") && !Runtime::update_all)
        {
            std::vector<Package> pkgs = tstd::parse_package_arguments(cli.getParameters("u"));
            Runtime::to_update.insert(Runtime::to_update.end(), pkgs.begin(), pkgs.end());
        }

        if (cli.argumentGiven("r"))
        {
            std::vector<Package> pkgs = tstd::parse_package_arguments(cli.getParameters("r"));
            Runtime::to_remove.insert(Runtime::to_remove.end(), pkgs.begin(), pkgs.end());
        }

        // Running the managers, doing installing, updating etc.

        if (cli.argumentGiven("p")) // Install local path
        {
            std::cout << "info: installing local path: " << cli.getParameters("p")[0] << std::endl;
            chdir(cli.getParameters("p")[0].c_str());
            InstallationManager::localPackage(cli.getParameters("p")[0]);
        }

        if (Runtime::to_install.empty() &&
            Runtime::to_update.empty() &&
            Runtime::to_remove.empty())
            Runtime::exit(0);

        Runtime::to_install = DependencyManager::getPackageConfig(Runtime::to_install);
        Runtime::to_update = DependencyManager::getPackageConfig(Runtime::to_update);

        if (Runtime::update_all)
        {
            for (const Package &installed_package : IPackagesManager::getInstalledPackages())
            {
                for (int i = 0; i < Runtime::to_update.size(); i++)
                {
                    if (Runtime::to_update[i].getName() == installed_package.getName() &&
                        Runtime::to_update[i].getServer() == installed_package.getServer() &&
                        Runtime::to_update[i].getRepoName() == installed_package.getRepoName())
                    {
                        if (Runtime::to_update[i].getVersion() <= installed_package.getVersion())
                        {
                            if (Runtime::reinstall)
                            {
                                std::cout << "info: package " << tstd::package_to_argument(installed_package) << " v" << installed_package.getVersion() << " is already installed. reinstalling." << std::endl;
                            }
                            else
                            {
                                std::cout << "info: package " << tstd::package_to_argument(installed_package) << " v" << installed_package.getVersion() << " is already installed. skipping." << std::endl;
                                Runtime::to_update.erase(Runtime::to_update.begin()+i);
                            }
                        }
                    }
                }
            }
        }

        // Runtime::to_remove = DependencyManager::getPackageConfig(Runtime::to_remove);

        for (int i = 0; i < Runtime::to_remove.size(); i++)
        {
            const Package &to_remove = Runtime::to_remove[i];
            bool found = false;

            for (const Package &p2 : IPackagesManager::getInstalledPackages())
            {
                if (p2.getGitUser() == to_remove.getGitUser() &&
                    p2.getServer() == to_remove.getServer() &&
                    p2.getRepoName() == to_remove.getRepoName())
                {
                    found = true;
                    continue;
                }
            }

            if (!found)
            {
                Runtime::to_remove.erase(Runtime::to_remove.begin()+i);
                std::cout << "info: package " << tstd::package_to_argument(to_remove) << " is not installed and can't be removed." << std::endl;
            }
        }

        for (int i = 0; i < Runtime::to_remove.size(); i++)
        {
            Runtime::to_remove[i] = IPackagesManager::getPackage(Runtime::to_remove[i]);
        }

        std::cout << "info: checking packages..." << std::endl;

        for (int i = 0; i < Runtime::to_install.size(); i++)
        {
            const Package &test = Runtime::to_install[i];

            if (IPackagesManager::isPackageInstalled(test))
            {
                if (Runtime::reinstall)
                {
                    std::cout << "info: package " << tstd::package_to_argument(test) << " v" << test.getVersion() << " is already installed. reinstalling." << std::endl;
                }
                else
                {
                    std::cout << "info: package " << tstd::package_to_argument(test) << " v" << test.getVersion() << " is already installed. skipping." << std::endl;
                    Runtime::to_install.erase(Runtime::to_install.begin()+i);
                }
            }
        }

        if (!Runtime::update_all)
        {
            for (int i = 0; i < Runtime::to_update.size(); i++)
            {
                const Package &test = Runtime::to_update[i];

                if (IPackagesManager::isPackageInstalled(test))
                {
                    if (Runtime::reinstall)
                    {
                        std::cout << "info: package " << tstd::package_to_argument(test) << " v" << test.getVersion() << " is already up-to-date. reinstalling." << std::endl;
                    }
                    else
                    {
                        std::cout << "info: package " << tstd::package_to_argument(test) << " v" << test.getVersion() << " is already up-to-date. skipping." << std::endl;
                        Runtime::to_update.erase(Runtime::to_update.begin()+i);
                    }
                }
            }
        }

        if (Runtime::to_update.size() > 0)
            Runtime::update = true;

        std::cout << "info: searching for dependencies..." << std::endl;

        for (const Package &p : Runtime::to_install) // Check for dependencies
        {
            DependencyManager::checkDependencies(p);
        }

        for (const Package &p : IPackagesManager::getInstalledPackages()) // Check for dependencies
        {
            bool cont = false;

            for (const Package &package_to_install : Runtime::to_install) // Check for dependencies
            {
                if (package_to_install.getGitUser() == p.getGitUser() &&
                    package_to_install.getServer() == p.getServer() &&
                    package_to_install.getRepoName() == p.getRepoName())
                {
                    cont = true;
                    break;
                }
            }

            if (!cont)
                DependencyManager::checkDependencies(p);
        }

        if (Runtime::to_remove.size() > 0 ||
            Runtime::to_install.size() > 0 ||
            Runtime::to_update.size() > 0)
            std::cout << std::endl;

        if (Runtime::to_remove.size() > 0)
        {
            std::cout << "info: following packages will be removed (" << Runtime::to_remove.size() << "): " << std::endl;
            std::cout << "  => ";

            for (const Package &p : Runtime::to_remove)
                std::cout << tstd::package_to_argument(p) << " ";
            std::cout << std::endl << std::endl;
        }

        if (Runtime::to_update.size() > 0)
        {
            std::cout << "info: following packages will be updated (" << Runtime::to_update.size() << "): " << std::endl;
            std::cout << "  => ";

            for (const Package &p : Runtime::to_update)
                std::cout << tstd::package_to_argument(p) << " ";
            std::cout << std::endl << std::endl;
        }

        if (Runtime::to_install.size() > 0)
        {
            std::cout << "info: following packages will be installed (" << Runtime::to_install.size() << "): " << std::endl;
            std::cout << "  => ";

            for (const Package &p : Runtime::to_install)
                std::cout << tstd::package_to_argument(p) << " ";
            std::cout << std::endl << std::endl;
        }

        if (Runtime::to_install.size() == 0 &&
            Runtime::to_update.size() == 0 &&
            Runtime::to_remove.size() == 0)
            Runtime::exit(0);

        if (!tstd::yn_question("do you want to continue?"))
            Runtime::exit(0);

        // Installing/Updating/Removing

        for (const Package &remove_me : Runtime::to_remove) // Iterate through packages, that have to be removed
        {
            RemoveManager::uninstallPackage(remove_me);
        }

        for (const Package &update_me : Runtime::to_update) // Iterate through packages, that have to be updated
        {
            UpdateManager::updatePackage(update_me);
        }

        for (const Package &install_me : Runtime::to_install) // Iterate through packages, that have to be installed
        {
            InstallationManager::installPackage(install_me);
        }

    }

    Runtime::exit(0);
}