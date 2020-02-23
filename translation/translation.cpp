#include <iostream>
#include <yaml-cpp/yaml.h>

#include <runtime.hpp>
#include "translation.hpp"
#include <tstd.hpp>

#define _RED "\033[1;31m"
#define _YELLOW "\033[1;33m"
#define _BLUE "\033[1;34m"
#define _NONE "\033[00m"

std::string Translation::get(const std::string &s, const bool &nl, const bool &bnl)
{
    if (sentences[s])
    {
        std::string str = std::string(sentences[s].as<std::string>());

        if (nl)
            str = str + "\n";
        if (bnl)
            str = "\n" + str;

        str = tstd::replace(str, "[=>]", "\033[1;32m=>\033[00m");
        str = tstd::replace(str, "[!!!]", _RED);
        str = tstd::replace(str, "[!!]", _YELLOW);
        str = tstd::replace(str, "[!]", _BLUE);
        str = tstd::replace(str, "[!]", _BLUE);
        str = tstd::replace(str, "[.]", _NONE);

        return str;
    }
    else
    {
        if (!fallback[s])
        {
            std::cout << "error: fatal error in tridymite: sentences \"" << s << "\" doesn't exist in fallback!" << std::endl;
            Runtime::exit(1, false);
        }
        else
        {
            std::string str = std::string(fallback[s].as<std::string>());

            if (nl)
                str = str + "\n";
            if (bnl)
                str = "\n" + str;

            str = tstd::replace(str, "[=>]", "\033[1;32m=>\033[00m");
            str = tstd::replace(str, "[!!!]", _RED);
            str = tstd::replace(str, "[!!]", _YELLOW);
            str = tstd::replace(str, "[!]", _BLUE);
            str = tstd::replace(str, "[!]", _BLUE);
            str = tstd::replace(str, "[.]", _NONE);

            return "[?] "+str;
        }
    }

    return "<NOT FOUND>";
}

void Translation::loadConfig(std::string path)
{
    fallback = YAML::Load(R"V0G0N(
general.loading_packages: "[!]info:[.] loading packages ..."
general.package_not_found: "[!!!]error:[.] package %s not found!"
general.package_not_found_good: "[!!!]error:[.] package not found:"
general.continue_question: "do you want to continue? (y=yes, n=no)" # don't change up the 'y' and 'n', but translate 'yes' and 'no'!
general.aborted: "aborted."

# cli

cli.argument.not_found: "[!!!]error:[.] argument \"%s\" not found!"
cli.argument.no_passed: "[!!!]error:[.] no argument passed!"
cli.argument.too_many: "[!!!]error:[.] too many parameters for argument \"%s\""
cli.argument.too_few: "[!!!]error:[.] too few parameters for argument \"%s\""
cli.version: "version" # "version <version>" will be printed
cli.unfinished_arguments: "[!!!]error:[.] cli: the declerations of the arguments are unfinished!"

# manager/dependencies

manager.dependencies.cant_remove_dependency: "[!]info:[.] can't remove %s: is a dependency of:"
manager.dependencies.added_dependency: "[!]info:[.] added dependency:"

# manager/install

manager.install.linking_files: "linking files ..."
manager.install.linking: "linking " # "linking <path>" will be printed
manager.install.counting_files: "counting files ..."
manager.install.file_warning: "[!!]warning:[.] this package contains more than 100 files."
manager.install.continue_counting: "continue counting ..."
manager.install.moving_files: "copying %d files ..."
manager.install.local_directory_not_found: "[!!!]error:[.] couldn't find the directory for local packages."
manager.install.global_installation_question: "do you want to continue and install it globally? (y=yes, n=no)" # don't change up the 'y' and 'n', but translate 'yes' and 'no'!
manager.install.aborting_installation: "aborted package installation."
manager.install.moving_force: " => copying (FORCE) %s to %s"
manager.install.moving: " => copying %s to %s"
manager.install.product_not_existing: "[!!!]error:[.] package %s: products: \"%s\" doesn't exist!"
manager.install.file_conflict: "[!!!]error:[.] package %s: products: conflict:\n    - \"%s\" already exists in filesystem!"
manager.install.moving_error_unknown: "[!!!]error:[.] copying products: \"%s\": unknown error!"
manager.install.directory_into_file: "[!!!]error:[.] package %s: products: \"%s\": can't move a directory into a file!"
manager.install.file_neither_exists: "[!!!]error:[.] copying products: \"%s\": unknown error! both files (\"%s\" and \"%s\") don't exist!"
manager.install.downloading: "downloading ..."
manager.install.unzipping: "unzipping ..."
manager.install.couldnt_unzip: "[!!!]error:[.] couldn't unzip file!"
manager.install.install_unzip: "[!]info:[.] install the unzip tool."
manager.install.on_which_account: "on which git account will it lie?"
manager.install.on_which_branch: "on which branch will it lie?"
manager.install.on_which_server: "on which git server will it lie?"
manager.install.what_name: "what's the name of the repository?"
manager.install.added: "added!"
manager.install.dependency_searching: "[!]info:[.] searching for dependencies ..."
manager.install.local_reinstall: "[!]info:[.] (local) package %s v%s is already installed. reinstalling."
manager.install.local_skip: "[!]info:[.] (local) package %s v%s is already installed. skipping."
manager.install.new_local_installation: "new local installation"
manager.install.now_installing: "now installing" # "now installing <project>"" will be printed
manager.install.building: "building ..."
manager.install.installing_version: "installing version" # "installing version <version>" will be printed
manager.install.skipping: "skipping ..."
manager.install.new_installation: "new installation"
manager.install.doesnt_support_local: "[!!!]error:[.] this package doesn't supports local installations!"
manager.install.pretype_doesnt_exit: "[!!!]error:[.] the pretype \"%s\" doesn't exists on this computer!"
manager.install.skip_and_continue: "do you want to skip this package and proceed with the other installations? (y=yes, n=no)" # don't change up the 'y' and 'n', but translate 'yes' and 'no'!
manager.install.no_build_script: "[!!!]error:[.] no build script exists."

# manager/remove

manager.remove.removing_links: "removing links ..."
manager.remove.local_directory_not_found: "[!!!]error:[.] couldn't find the directory for local packages."
manager.remove.remove_globally_instead: "do you want to continue and remove it globally? (y=yes, n=no)" # don't change up the 'y' and 'n', but translate 'yes' and 'no'!
manager.remove.unlinking: "unlinking" # "unlinking <file>" will be printed
manager.remove.removing_products: "removing products ..."
manager.remove.removing: "removing"
manager.remove.now_uninstalling: "now uninstalling" # "now uninstalling <packages>" will be printed
manager.remove.removing_path: "removing" # "removing <path>" will be printed
manager.remove.package_not_found: "[!!]warning:[.] removing: package %s not found. skipping."

# manager/update

manager.update.updating: "updating"
manager.update.updating_package: "updating package" # "updating package <package>" will be printed

# script

script.parse_file.file_not_found: "[!!!]error:[.] can't go on without a file."
script.parse_file.to_many_brackets: "[!!!]error:[.] line %d: to many \"}\" in file %s!"
script.parse_file.unfinished_declaration: "[!!!]error:[.] line %d: unfinished declaration of function in build script."
script.unsave_script: "[!!!]error:[.] unsave script: script uses sudo command!"
script.function_not_found: "[!!!]error:[.] can't run function: \"%s\": function not found!"
script.error_at_script: "[!!!]error:[.] error at build script. aborting now."

# std

arguments.help.help: "shows this help page"
arguments.help.install: "install a package"
arguments.help.remove: "remove a package"
arguments.help.update: "update a package"
arguments.help.update_all: "update all installed packages"
arguments.help.path: "installing a package from the source in the given directory"
arguments.help.insecure: "installing packages without asking security questions (not implemented yet)"
arguments.help.server: "setting the standard server for all package arguments"
arguments.help.user: "setting the user for all package arguments"
arguments.help.version: "show the version of tridymite"
arguments.help.language_file: "loading all messages from the given language file"
arguments.help.list_packages: "list all installed packages"
arguments.help.packages: "get information of package"
arguments.help.description: "shows description of a package"
arguments.help.already_installed: "don't skip already installed packages"
arguments.help.no_deps: "don't check dependencies"
arguments.help.force: "don't look for file conflicts"
arguments.help.local: "if possible, try to install a package locally"
arguments.help.get: "download a package archive into the current working directory"
arguments.help.search_installed: "search for an installed package"
arguments.help.search_online: "search for an installed package"
arguments.help.create: "create a new package from the source of the current directory"
arguments.help.keep_tmp: "keep tmp-folder after exit"
arguments.help.download_to_path: "extracts package-source-code to a path (similiar to '--get', but extracts contents of repo-zip)"
arguments.help.update_path: "reinstalls all packages in path (extended --download-to-path)"

arguments.usage.help: ""
arguments.usage.install: "<package> ..."
arguments.usage.remove: "<package> ..."
arguments.usage.update: "<package> ..."
arguments.usage.update_all: ""
arguments.usage.path: "<path>"
arguments.usage.insecure: ""
arguments.usage.server: "<server>"
arguments.usage.user: "<username>"
arguments.usage.version: ""
arguments.usage.language_file: "<file>"
arguments.usage.list_packages: ""
arguments.usage.packages: "<package>"
arguments.usage.description: "<package>"
arguments.usage.already_installed: ""
arguments.usage.no_deps: ""
arguments.usage.force: ""
arguments.usage.local: ""
arguments.usage.get: "<package>"
arguments.usage.search_installed: "<string>"
arguments.usage.search_online: "<string> ..."
arguments.usage.create: ""
arguments.usage.keep_tmp: ""
arguments.usage.download_to_path: "<path> <package> ..."
arguments.usage.update_path: "<path>"

arguments.package: "<package> = <git-user>:<git-repo>[@<git-server>] (git-server is %s by default)"

std.package.package_incomplete: "[!!!]error:[.] parsing package: \"%s\" is incomplete."
std.package.package_structure: "[!]info:[.] packages are build like \"user:repository@gitserver.abc\""
std.config.no_servers_defined: "[!!!]error:[.] no git servers specified in config file!"
std.config.server_not_defined: "[!!!]error:[.] git server \"%s\" is not configured in config file!"
std.neither_y_or_no: "please type y or n!"
std.big_file: "the file you want to download is really big" # "the file you want to download is really big (<size> GB)." will be printed
std.object_not_found: "[!!!]error:[.] object %s not found!"

# runtime

runtime.not_installed: "[!!!]error:[.] it seems tridymite isn't installed yet.\nIf you have the source code here, try running the install.sh script as root."
runtime.tmp_permission_error: "[!!!]error:[.] couldn't create tmp-directory!:"
runtime.clean_files.error: "[!!!]error:[.] couldn't delete file: %s"
runtime.clean_directories.error: "[!!!]error:[.] couldn't delete directory: %s"

runtime.clear_up_all_tmps: "[!!!]error:[.] error at cleaning up temporary files or directories."
runtime.exit: "exiting with code %d"

runtime.config_not_found: "[!!!]error:[.] tridymite's config file wasn't found!"
runtime.tridy_not_installed: "[!!!]error:[.] it seems tridymite isn't installed yet."
runtime.run_installer: "If you have the source code here, try running the install.sh script as root."
runtime.catch_ctrl_c: "catched ^C\ndon't do that again!"

# main

main.no_permission_to_create_directory: "[!!!]error:[.] no permission to create directory: "
main.no_permission_to_move_directory: "[!!!]error:[.] no permission to move directory: "
main.never_root: "[!!!]error:[.] never run tridymite as root!"
main.no_local_module: "[!!!]error:[.] can't use operator \"-l/--local\": no local installation module in setup activated!"
main.list_local_packages: "Local packages installed:"
main.list_packages: "Packages installed:"
main.no_packages_installed: "No packages installed."
main.no_local_packages_installed: "No local packages installed."
main.downloading: "[!]info:[.] downloading" # "[!]info:[.] downloading <package> ..." will be printed
main.file_exists_overwrite: "[!!]warning:[.] this file already exists.\ndo you want to overwrite it?"
main.finished: "[!]info:[.] finished."
main.package_not_found: "[!!!]error:[.] package not found!"
main.description_package_not_found: "[!]info:[.] show description: couldn't find package" # "[!]info:[.] show description: couldn't find package <package>" will be printed
main.trying_find_online: "[!]info:[.] trying to find it online..."
main.installing_local_path: "[!]info:[.] installing local path:"
main.package_installed_reinstall: "[!]info:[.] package %s v%s is already installed. reinstalling."
main.package_installed_skipping: "[!]info:[.] package %s v%s is already installed. skipping."
main.package_up_to_date_reinstall: "[!]info:[.] package %s v%s is already up-to-date. reinstalling."
main.package_up_to_date_skipping: "[!]info:[.] package %s v%s is up-to-date. skipping."
main.not_installed_cant_removed: "[!]info:[.] package %s is not installed and can't be removed."
main.checking_packages: "[!]info:[.] checking packages..."
main.searching_dependencies: "[!]info:[.] searching for dependencies..."
main.following_removed: "[!]info:[.] following packages will be removed" # "[!]info:[.] following packages will be removed (<number>):" will be printed
main.following_updated: "[!]info:[.] following packages will be updated" # "[!]info:[.] following packages will be updated (<number>):" will be printed
main.following_installed: "[!]info:[.] following packages will be installed" # "[!]info:[.] following packages will be installed (<number>):" will be printed

main.so.too_few_parameters: "[!!!]error:[.] too few parameters! usage:\n    ... -so <server> <term> ..."
main.so.no_git_servers: "[!!!]error:[.] no git-servers set in config file!"
main.so.server_not_defined: "[!!!]error:[.] server %s not defined in config!"
main.so.servant_not_implemented: "[!!!]error:[.] the online search function for server %s is not implemented!"
main.so.no_servant: "[!!!]error:[.] no servant defined."
main.so.error_at_servant: "[!!!]error:[.] error happened at servant!"
main.so.error: "[!!!]error:[.] an error happened!"
main.so.invalid_pat: "[!!!]error:[.] invalid personal access token!"
main.so.found_matches: "Found %d matches:"
main.so.found_match: "Found %d match:"
main.so.no_found: "No packages found."
main.so.incomplete_package: "[!!!]error:[.] error in servant: servant responds incomplete package!"

main.create.info_file_not_found: "[!!!]error:[.] file not found!"
main.create.name: "What's the name of the project?"
main.create.version: "What is the current version of the project (N.N.NL; N=Number, L=Letter(a-z))?"
main.create.description: "Describe your project in a short sentence."
main.create.info_file: "Do you have a text file with information about your project? (Like a short version of a ReadMe.md.)"
main.create.info_path: "Please give me the path to the file."
main.create.cant_write: "[!!!]error:[.] can't write to file!"
main.create.authors: "Give me the authors of the project (write \"(end)\" if you are finished)."
main.create.tridy_knows: "Tridymite knows these kind of package-building : "
main.create.one_of_them: "Is your package one of them?"
main.create.type_name: "Please enter the name."
main.create.wrong_word: "I think you maybe entered a wrong word. Please try again."
main.create.enter_value: "Please enter value for" # "Please enter value for <name>." will be printed
main.create.products_now: "Do you want to set the products now?"
main.create.end_to_end: "Write \"(end)\" to end."
main.create.file_take_from: "I'll take file from ... "
main.create.file_move_to: "and will move it to ... "
main.create.links_now: "Do you want to set the links now?"
main.create.linking_to: "and will link it to ... "
main.create.finished: "Finished!"
main.create.generating_now: "Okay, that's all! I will generate now the package file for this directory!"
main.create.already_exists_overwrite: "There is already a package file! Do you want to overwrite it?"
main.create.sure: "Are you really sure?"
main.create.ok: "Okay!"

main.all_work_done: "Okay! All work done!"
main.arrow: " [=>] "
main.installed_o: "Installed %d package."
main.installed_t: "Installed %d packages."
main.updated_o: "Updated %d package."
main.updated_t: "Updated %d packages."
main.removed_o: "Removed %d package."
main.removed_t: "Removed %d packages."

# package

package.no_option_set: "[!!!]error:[.] package %s: option missing (%s)!"
package.products_have_to_be_sequence: "[!!!]error:[.] package %s: products have to be a sequence!"
package.links_have_to_be_sequence: "[!!!]error:[.] package %s: links have to be a sequence!"
package.no_tridymite_package: "[!!!]error:[.] package %s: dependency %s seems not to be a tridymite package!"
package.nopkg: "[!!!]error:[.] package %s: dependencies: nopkg is not allowed yet!"
)V0G0N");

    if (std::ifstream(path).is_open())
    {
        sentences = YAML::LoadFile(path);
    }
    else
    {
        std::cout << "warning: language file " << path << " not found!" << std::endl;
        std::cout << "Using fallback..." << std::endl;
    }
}
