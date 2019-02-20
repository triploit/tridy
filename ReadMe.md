# Tridymite

Tridymite is a package manager for github repositories. It allows developers and users to easily install and manage their packages and let them create their own in their repositories.

**It only works on Linux.**

### To-Do

* fill the documentation for tridymite source
* add implementation for predefined types of `package.sh`

## Table of Contents

-   [Introduction for Users](#introduction-for-users)
    -   [Installing Tridymite](#installing-tridymite)
    -   [Installing Packages](#installing-packages)
        -   [Installing Packages from Different Servers](#installing-packages-from-different-servers)
        -   [Installing Packages Locally](#installing-packages-locally)
    -   [Removing Packages](#removing-packages)
    -   [Updating Packages](#updating-packages)
    -   [Installing Packages from Source](#installing-packages-from-source)
    -   [Extra Features](#extra-features)
-   [Tridymite for Package Developers](#tridymite-for-package-developers)
-   [Tridymite for Developers](#tridymite-for-developers)
    

## Introduction for Users

### Installing Tridymite

First, clone the repository and change to it's directory.

```bash
git clone https://github.com/triploit/tridymite
cd tridymite/
```

After that, just run the installation script:

```bash
sudo bash ./install.sh
```

You will be lead through the setup.

### Installing Packages

Installing of Packages is really easy. The only difficulty is: You have to know where the package lies. Tridymite is a package manager to install packages of git repositories. This works with **all** git servers. Therefor you have to say where the packages lies and which user the package owns.

For example, to install a the package AML from our organisation:

```bash
tridy -i triploit:aml@github.com
```

The syntax of a package argument is: `user:package@server`. You can leave out the server, If you want to install a package from `github.com`, because GitHub is the standard git server.

So, this will also work:

```bash
tridy -i triploit:aml
```

#### Installing Packages from Different Servers

If you exchange the `github.com` with (for example) `bitbucket.org`, it will also work, but the repository and the user have to exist on `bitbucket.org`:

```bash
tridy -i triploit:aml@bitbucket.org
```

(This won't work, because we don't have a bitbucket account.)

#### Installing Packages Locally

Not all packages have to be installed globally for all users. If a package supports the local installation option with relative paths, it can be installed with the option `-l` or `--local`:

```bash
tridy -i triploit:aml -l
```

AML can be installed as a local package, with the local option it will be installed in the `~/.local/` folder, so it's only accessable for one user.

### Removing Packages

If you want to remove a package, it can be done with the option `-r` or `--remove`:

```bash
tridy -r triploit:aml
```

This will remove AML, even if it's locally installed.

### Updating Packages

Updating packages can be done with the option `-u` or `--update`:

```bash
tridy -u triploit:aml
```

Or, if you want to update all packages, use `-ua` or `--update-all`:

```bash
tridy -ua
```

**Note:** Use `-l` in combination with `-u` or `-ua` to uptdate locally installed packages.

### Intalling Packages from Source

If you have the source code of a tridymite package, you can just use the option `-p` or `--path`:

```bash
tridy -p source/to/package
```

This method also **needs a package config.** For more information about creating a package, see [Tridymite for Package Developers](#tridymite-for-package-developers).

### Extra Features

#### Listing All Installed Packages

For this, use the option `-lp` or `--list-packages`:

```bash
tridy -lp
```

Note: There is a difference in local and global packages. The option above will only show the global packages. To see the local packages, add the option `-l` or `--local`:

```bash
tridy -l -lp
```

This will show all the packages, that were installed locally.

#### Showing Infromation about a Package

Before you install a package (or after you installed a package) you can view all information, the author has given. Just use the option `-d` or `--description`:

```bash
tridy -d triploit:tridymite
```

# Tridymite for Package Developers

Creating packages is really easy. You just have to add 2 files to your project. First, you have to add a `pkg/` directory **to the root directory of your project**. In this directory, you have to create 2 files:

### `package.yaml`

The `package.yaml` provides information (like author, version, name, ...) to tridymite.

Here is an example:

```yaml
name: "The Test Package"
version: "0.0.1a"
description: "Short description of the package."
information: |
    Really long text... 
    Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod
    tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam 
    et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum 
    dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod 
    tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam 
    et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum 
    dolor sit amet.

authors:
    - your_github_name
    - second

products:
    - ./prog: "/usr/share/bin/prog"
```

#### Name

The field `name` contains the written out name of the projects (please don't use shortcuts).

#### Version

The version has the following syntax: `[main version].[subversion].[revision][build]` Notice that `build` is always a letter, like in:

```
0.0.1a
0.0.1b
0.1.2a
```

#### Description and Information

Information is a long explanation of the project. Descritpion is just a short description.

#### Authors

`authors` contains all contributors that worked on the package/program.

#### Products

This part is **really important**! After the build of the project, here is the part, where the magic is done. Put all your binary files and configuration files in the following order:

```yaml
products:
    - from: "to"
```

Example:

```yaml
products:
    - binary: "/usr/share/bin/binary"
```

You can also use "relative paths". That allows the user, to install the package locally, not globally. Let's say, we have a package with only one binary file. Then, you should let the user decide if he want's to install it locally, or not. For this, we added some variables for paths:

| Variable | Redirection |
| --- | --- |
| `$usr` | `/usr/ for global` installations, `~/.local/usr/` for local ones |
| `$share` | `/usr/share/` for global installations, `~/.local/usr/share/` for local ones |
| `$bin` | `/usr/bin/` for global installations, `~/.local/usr/bin/` for local ones |

Tridymite prepares this option at the installation.

### `package.sh`

The file `package.sh` is just the build file. Write your commands to execute Makefiles/build your projects or prepare the installation here.

It's not allowed, to execute command as root. Also there have to be a function called `build` . For Tridymite it's like a main function in other programming languages.

Here is an example:

```bash
function build {
    echo "building..."
    g++ main.cpp -o bin
}
```

Else you can add own functions and use it in the `build` function.

#### A Look Into The Future

Currently we're working on an option, to generate standartized build-scripts, so you can leave out the `package.sh`. For this, we might add an option to the `package.yaml`, like `type: ...` or `build: ...`. It will look something like this:

```yaml
name: "Lorem ipsum"
version: "0.0.1a"
build: make
```

In the folder `pkg/conf/deptypes` you can see files, with predefined content for make and cmake files. These will be put into the `package.sh` and that will be then executed. So the `build: <type>` option will take the file in the configuration folder of tridymite, take it's contents and put it into the `package.sh` (if none exists). This would add the possibility to add more predefined build-scripts.
