#include<iostream>
#include<cassert>
#include<vector>
#include<set>
#include<map>
#include<string>
#include<unordered_map>
#include<memory>
#include<unordered_set>
#include<queue>
#include<list>
#include<sstream>
#include<cstring>

// ---------------------------------------------------------------------------------------------------------------------

using namespace std;

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Class representing a package in a package management system.
 */
class CPkg {
public:
    // todo constructor
    /**
     * @brief Constructor for CPkg.
     * @param name The name of the package.
     */
    CPkg(const string & name)
            : name_(name), dependency_names_() {}

    ~CPkg() = default;

    // -----------------------------------------------------------------------------------------------------------------

    /**
     * @brief Comparison operator for CPkg, used for sorting packages.
     * @param lhs The left-hand side CPkg object.
     * @param rhs The right-hand side CPkg object.
     * @return True if lhs is less than rhs, false otherwise.
     */
    friend bool operator<(const CPkg & lhs, const CPkg & rhs) {
        return lhs.name_ < rhs.name_;
    }

    // -----------------------------------------------------------------------------------------------------------------

    /**
     * @brief Getter for the package name.
     * @return The name of the package.
     */
    const string & name() const {
        return name_;
    }

    /**
     * @brief Getter for the package dependencies.
     * @return A set of dependency names.
     */
    const set<string> & dependency_names() const {
        return dependency_names_;
    }

    // -----------------------------------------------------------------------------------------------------------------

    // todo addDep(string)
    /**
    * @brief Add a dependency to the package.
    * @param name The name of the dependency to add.
    * @return Reference to the current CPkg object.
    */
    CPkg & addDep(string name) {
        dependency_names_.insert(std::move(name));

        return * this;
    }

private:
    // todo
    string name_; // The name of the package
    set<string> dependency_names_; // The dependencies of the package
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Class representing a package management system.
 */
class CPkgSys {
public:
    CPkgSys() = default;

    ~CPkgSys() = default;

    // -----------------------------------------------------------------------------------------------------------------

    // todo operator <<
    /**
     * @brief Stream insertion operator for CPkgSys.
     * @param os The output stream.
     * @param system The CPkgSys object.
     * @return The output stream.
     */
    friend ostream & operator<<(ostream & os, const CPkgSys & system) {
        bool is_first = true;

        for (const auto & package : system.packages_)
            if (package.second) {
                if (!is_first)
                    os << ", ";

                is_first = false;
                os << package.first.name();
            }

        return os;
    }

    // -----------------------------------------------------------------------------------------------------------------

    // todo addPkg(CPkg)
    /**
     * @brief Add a package to the system.
     * @param package The package to add.
     * @return Reference to the current CPkgSys object.
     */
    CPkgSys & addPkg(const CPkg & package) {
        packages_.insert(make_pair(package, false));

        return * this;
    }

    // todo install(list<string>)
    /**
     * @brief Install a list of packages, resolving dependencies.
     * @param package_names A list of package names to install.
     * @return A set of installed package names.
     * @throws invalid_argument If a package is not found.
     */
    set<string> install(const list<string> & package_names) {
        map<CPkg, bool> packages = packages_;
        set<string> prepared_names;
        queue<string> planned_names;

        for (const string & package_name : package_names) {
            if (!packages.contains(package_name))
                throw invalid_argument("Package not found.");

            if (!packages.at(package_name)) {
                packages[package_name] = true;
                prepared_names.insert(package_name);
                planned_names.push(package_name);
            }
        }

        while (!planned_names.empty()) {
            string package_name = std::move(planned_names.front());
            planned_names.pop();

            if (auto package_it = packages.find(package_name); !package_it->first.dependency_names().empty())
                for (const string & dependency_name : package_it->first.dependency_names()) {
                    if (!packages.contains(dependency_name))
                        throw invalid_argument("Package not found.");

                    if (!packages.at(dependency_name)) {
                        packages[dependency_name] = true;
                        prepared_names.insert(dependency_name);
                        planned_names.push(dependency_name);
                    }
                }
        }

        swap(packages_, packages);

        return prepared_names;
    }

private:
    map<CPkg, bool> packages_; // Map of packages and their installation status
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

int main() {
    stringstream ss;

    // -----------------------------------------------------------------------------------------------------------------

    CPkgSys s;

    s.addPkg(CPkg("ssh").addDep("sudo").addDep("apt"))
     .addPkg(CPkg("sudo").addDep("git").addDep("c++"));

    s.addPkg(CPkg("apt"))
     .addPkg(CPkg("c++").addDep("c").addDep("asm").addDep("fortran"));

    s.addPkg(CPkg("git"))
     .addPkg(CPkg("c").addDep("kekw"))
     .addPkg(CPkg("kekw"))
     .addPkg(CPkg("asm"))
     .addPkg(CPkg("fortran"));

    s.addPkg(CPkg("python").addDep("bash").addDep("sadge"))
     .addPkg(CPkg("karel").addDep("python"))
     .addPkg(CPkg("bash").addDep("sadge"))
     .addPkg(CPkg("sadge"))
     .addPkg(CPkg("cython").addDep("dev"));

    s.addPkg(CPkg("perl"));

    // -----------------------------------------------------------------------------------------------------------------

    ss << s;
    assert(ss.str() == "");
    ss.clear();
    ss.str("");

    // -----------------------------------------------------------------------------------------------------------------

    set<string> t1 = s.install(list<string> {"sudo"});
    assert(t1 == (set<string> {"asm", "c", "c++", "fortran", "git", "kekw", "sudo"}));

    set<string> t2 = s.install(list<string> {"ssh", "c++"});
    assert(t2 == (set<string> {"apt", "ssh"}));

    // -----------------------------------------------------------------------------------------------------------------

    ss << s;
    assert(ss.str() == "apt, asm, c, c++, fortran, git, kekw, ssh, sudo");
    ss.clear();
    ss.str("");

    // -----------------------------------------------------------------------------------------------------------------

    try {
        set<string> e = s.install(list<string> {"karel", "cython"});
        assert("Sem ses nemel dostat debilku" == nullptr);
    } catch (const invalid_argument & e) {
        assert(strcmp("Package not found.", e.what()) == 0);
    }

    // -----------------------------------------------------------------------------------------------------------------

    set<string> t3 = s.install(list<string> {"karel", "fortran", "git"});
    assert(t3 == (set<string> {"bash", "karel", "python", "sadge"}));

    // -----------------------------------------------------------------------------------------------------------------

    s.addPkg(CPkg("java").addDep("utils"))
     .addPkg(CPkg("utils").addDep("VB"))
     .addPkg(CPkg("VB").addDep("java"));

    // -----------------------------------------------------------------------------------------------------------------

    set<string> t4 = s.install(list<string> {"java", "perl"});
    assert(t4 == (set<string> {"VB", "java", "perl", "utils"}));

    // -----------------------------------------------------------------------------------------------------------------

    ss << s;
    assert(ss.str() == "VB, apt, asm, bash, c, c++, fortran, git, java, karel, kekw, perl, python, sadge, ssh, sudo, utils");
    ss.clear();
    ss.str("");

    // -----------------------------------------------------------------------------------------------------------------

    CPkgSys k;

    k.addPkg(CPkg("ssh").addDep("sudo").addDep("apt"))
     .addPkg(CPkg("sudo").addDep("git"));

    k.addPkg(CPkg("apt"));

    k.addPkg(CPkg("git"))
     .addPkg(CPkg("c").addDep("kekw"))
     .addPkg(CPkg("kekw"))
     .addPkg(CPkg("asm"))
     .addPkg(CPkg("fortran"));

    k.addPkg(CPkg("python").addDep("bash").addDep("sadge"))
     .addPkg(CPkg("karel").addDep("python"))
     .addPkg(CPkg("bash").addDep("sadge"))
     .addPkg(CPkg("sadge"));

    k.addPkg(CPkg("perl").addDep("no"));

    // -----------------------------------------------------------------------------------------------------------------

    set<string> t5 = k.install(list<string> {"asm"});
    assert(t5 == (set<string> {"asm"}));

    set<string> t6 = k.install(list<string> {"python", "ssh"});
    assert(t6 == (set<string> {"apt", "bash", "git", "python", "sadge", "ssh", "sudo"}));

    // -----------------------------------------------------------------------------------------------------------------

    try {
        set<string> t7 = k.install(list<string> {"perl", "c"});
        assert("Sem ses nemel dostat debilku" == nullptr);
    } catch (const invalid_argument & e) {
        assert(strcmp("Package not found.", e.what()) == 0);
    }

    // -----------------------------------------------------------------------------------------------------------------

    set<string> t8 = k.install(list<string> {"c", "ssh", "karel"});
    assert(t8 == (set<string> {"c", "karel", "kekw"}));

    // -----------------------------------------------------------------------------------------------------------------

    ss << k;
    assert(ss.str() == "apt, asm, bash, c, git, karel, kekw, python, sadge, ssh, sudo");
    ss.clear();
    ss.str("");

    // -----------------------------------------------------------------------------------------------------------------

    return EXIT_SUCCESS;
}
