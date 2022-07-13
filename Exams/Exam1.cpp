#include<iostream>
#include<cassert>
#include <utility>
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

using namespace std;

class CPkg
{
public:
	CPkg (string name) : m_Name (std::move(name)), m_Dependencies () { }

	CPkg& addDep (const string& dependency)
	{
		m_Dependencies.insert (dependency);

		return *this;
	}

	string getPkgName () const { return m_Name; }

	unordered_set<string> getPkgDependencies () const { return m_Dependencies; }

private:
	string m_Name;
	unordered_set<string> m_Dependencies;

};

class CPkgSys
{
public:
	CPkgSys& addPkg (const CPkg& package)
	{
		m_PkgAvailable.emplace (package.getPkgName (), package);

		return *this;
	}

	set<string> install (const list<string>& list_to_install)
	{
		set<string> new_installed;
		set<string> tmp_all_installed = m_PkgInstalled;
		queue<string> queue_to_install;

		for (const auto& pkg : list_to_install)
		{
			if (!tmp_all_installed.count (pkg)) queue_to_install.push (pkg);
		}

		while (!queue_to_install.empty ())
		{
			string pkg_to_install = queue_to_install.front ();
			queue_to_install.pop ();
			if (!m_PkgAvailable.count (pkg_to_install)) throw invalid_argument ("Package not found.");

			auto nbrs_to_install = m_PkgAvailable.at (pkg_to_install).getPkgDependencies ();
			for (const auto& pkg : nbrs_to_install)
			{
				if (!tmp_all_installed.count (pkg)) queue_to_install.push (pkg);
			}

			tmp_all_installed.insert (pkg_to_install);
			new_installed.insert (pkg_to_install);
		}

		m_PkgInstalled = tmp_all_installed;

		return new_installed;
	}


	friend ostream& operator << (ostream& os, const CPkgSys& x)
	{
		bool first = true;

		for (const auto& pkg : x.m_PkgInstalled)
		{
			if (!first) os << ", ";
			first = false;
			os << pkg;
		}

		return os;
	}

private:
	unordered_map<string, CPkg> m_PkgAvailable;
	set<string> m_PkgInstalled;

};

int main ()
{
	CPkgSys s;
	stringstream ss;
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

	ss << s;
	assert(ss.str() == "");
	ss.clear();
	ss.str("");

	set<string> t1 = s.install(list<string> {"sudo"});
	assert(t1 == (set<string> {"asm", "c", "c++", "fortran", "git", "kekw", "sudo"}));
	set<string> t2 = s.install(list<string> {"ssh", "c++"});
	assert(t2 == (set<string> {"apt", "ssh"}));

	ss << s;
	assert(ss.str() == "apt, asm, c, c++, fortran, git, kekw, ssh, sudo");
	ss.clear();
	ss.str("");

	try
	{
		set<string> e = s.install(list<string> {"karel", "cython"});
		assert("Sem ses nemel dostat debilku" == nullptr);
	}
	catch(const invalid_argument & e)
	{
		assert(strcmp("Package not found.", e.what()) == 0);
	}

	set<string> t3 = s.install(list<string> {"karel", "fortran", "git"});
	assert(t3 == (set<string> {"bash", "karel", "python", "sadge"}));

	s.addPkg(CPkg("java").addDep("utils"))
			.addPkg(CPkg("utils").addDep("VB"))
			.addPkg(CPkg("VB").addDep("java"));

	set<string> t4 = s.install(list<string> {"java", "perl"});
	assert(t4 == (set<string> {"VB", "java", "perl", "utils"}));

	ss << s;
	assert(ss.str() == "VB, apt, asm, bash, c, c++, fortran, git, java, karel, kekw, perl, python, sadge, ssh, sudo, utils");
	ss.clear();
	ss.str("");

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

	set<string> t5 = k.install(list<string> {"asm"});
	assert(t5 == (set<string> {"asm"}));
	set<string> t6 = k.install(list<string> {"python", "ssh"});
	assert(t6 == (set<string> {"apt", "bash", "git", "python", "sadge", "ssh", "sudo"}));

	try
	{
		set<string> t7 = k.install(list<string> {"perl", "c"});
		assert("Sem ses nemel dostat debilku" == nullptr);
	}
	catch(const invalid_argument & e)
	{
		assert(strcmp("Package not found.", e.what()) == 0);
	}

	set<string> t8 = k.install(list<string> {"c", "ssh", "karel"});

	assert(t8 == (set<string> {"c", "karel", "kekw"}));

	ss << k;
	assert(ss.str() == "apt, asm, bash, c, git, karel, kekw, python, sadge, ssh, sudo");
	ss.clear();
	ss.str("");

}
