#include <iostream>
#include <cfloat>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <vector>

// ---------------------------------------------------------------------------------------------------------------------

using namespace std;

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Class for analyzing salaries across different regions.
 */
class SalaryAnalyzer {
public:
    SalaryAnalyzer() = default;

    ~SalaryAnalyzer() = default;

    // -----------------------------------------------------------------------------------------------------------------

    // prvni parametr nazev regionu, druhy set s potomky/subregiony // todo
    /**
     * @brief Define a region and its subregions.
     * @param region The name of the region.
     * @param subregions The set of subregions.
     */
    void defineRegion(const string & region, const set<string> & subregions) {
        region_cadastre[region].insert(subregions.begin(), subregions.end());
    }

    // prida do evidence regionu jednoho cloveka s danym platem // todo
    /**
     * @brief Add a salary entry to a specific region.
     * @param region The name of the region.
     * @param salary The salary to add.
     */
    void addSalary(const string & region, int salary) {
        salary_cadastre[region].push_back(salary);
    }

    // -----------------------------------------------------------------------------------------------------------------

    // vrati prumernou mzdu ze zadanych regionu // todo
    /**
     * @brief Calculate the average salary across specified regions.
     * @param regions The set of regions to consider.
     * @return The average salary.
     */
    double averageSalary(const set<string> & regions) const {
        double total = 0;

        queue<string> plan;
        set<string> visited;

        for (const auto & r : regions) {
            plan.push(r);
            visited.insert(r);
        }

        while (!plan.empty()) {
            string region = plan.front();
            plan.pop();

            if (region_cadastre.contains(region))
                for (const auto & r : region_cadastre.at(region))
                    if (!visited.contains(r)) {
                        plan.push(r);
                        visited.insert(r);
                    }

            if (salary_cadastre.contains(region))
                total += accumulate(salary_cadastre.at(region).begin(), salary_cadastre.at(region).end(), 0);
        }

        return total / static_cast<double>(visited.size());
    }

private:
    map<string, set<string>> region_cadastre; // Map of regions and their subregions
    map<string, vector<int>> salary_cadastre; // Map of regions and their salary entries
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

int main() {
    SalaryAnalyzer sa;

    sa.defineRegion("CZ", {"Praha", "Brno", "Ostrava"});
    sa.defineRegion("Praha", {"Praha 1", "Praha 2", "Praha 3", "Dejvice"});
    sa.defineRegion("Dejvice", {"FIT", "FEL"});

    sa.addSalary("Praha", 50000);
    sa.addSalary("Praha", 100000);
    sa.addSalary("FIT", 80000);

    auto res1 = sa.averageSalary({"CZ", "FIT", "Praha 2"});
    auto res2 = sa.averageSalary({"CZ"});

    assert(res1 - res2 < DBL_EPSILON);

    return EXIT_SUCCESS;
}
