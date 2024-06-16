#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <map>
#include <set>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <algorithm>
#include <memory>

// ---------------------------------------------------------------------------------------------------------------------

using namespace std;

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Class for managing transportation lines and finding destinations within a cost limit.
 */
class CTransport {
public:
    // def constructor // todo
    CTransport() = default;

    // destructor // todo
    ~CTransport() = default;

    // -----------------------------------------------------------------------------------------------------------------

    // todo
    /**
     * @brief Adds a line of stations to the transport network.
     * @param is Input stream containing the line of stations.
     */
    void AddLine(istream & is) {
        for (string prev_station, curr_station; getline(is, curr_station);
                prev_station = curr_station, curr_station.clear()) {
            if (!prev_station.empty()) {
                stations[prev_station].insert(curr_station);
                stations[curr_station].insert(prev_station);
            }

            if (!stations.contains(curr_station)) stations[curr_station] = {};
        }
    }

    // todo
    /**
     * @brief Finds all destinations that can be reached from a starting station within a given cost.
     * @param from The starting station.
     * @param max_cost The maximum travel cost.
     * @return A set of reachable stations.
     */
    set<string> FindDest(const string & from, int max_cost) const {
        queue<string> plan;
        set<string> visited;
        map<string, int> costs;

        plan.push(from);
        visited.insert(from);
        costs.insert(make_pair(from, 0));

        while (!plan.empty()) {
            auto curr_station = plan.front();
            plan.pop();

            if (int curr_cost = costs.at(curr_station); stations.contains(curr_station))
                for (const auto & next_station : stations.at(curr_station))
                    if (!visited.contains(next_station) && (curr_cost + 1) <= max_cost) {
                        plan.push(next_station);
                        visited.insert(next_station);
                        costs.insert(make_pair(next_station, curr_cost + 1));
                    }
        }

        return visited;
    }

private:
    // todo
    map<string, set<string>> stations; // Map storing the connections between stations
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

int main() {
    istringstream iss;
    CTransport t0;

    // -----------------------------------------------------------------------------------------------------------------

    iss.clear();
    iss.str ("Newton\n" "Black Hill\n" "Wood Side\n" "Green Hill\n" "Lakeside\n");
    t0.AddLine(iss);

    // -----------------------------------------------------------------------------------------------------------------

    iss.clear();
    iss.str("Great Newton\n" "Little Burnside\n" "Green Hill\n" "Wood Side\n");
    t0.AddLine(iss);

    // -----------------------------------------------------------------------------------------------------------------

    iss.clear();
    iss.str("Little Newton\n" "Little Burnside\n" "Castle Hill\n" "Newton Crossroad\n" "Lakeside Central\n");
    t0.AddLine(iss);

    // -----------------------------------------------------------------------------------------------------------------

    iss.clear();
    iss.str("Tidmouth\n" "Gordon's Hill\n" "Suderry\n" "Knapford\n" "Great Waterton\n" "Brendam Docks\n");
    t0.AddLine(iss);

    // -----------------------------------------------------------------------------------------------------------------

    iss.clear();
    iss.str ("Sodor Ironworks\n" "Sodor Streamwork\n" "Knapford\n" "Maron\n");
    t0.AddLine(iss);

    // -----------------------------------------------------------------------------------------------------------------

    iss.clear();
    iss.str("Lakeside Central\n" "Little Waterton\n");
    t0.AddLine(iss);

    // -----------------------------------------------------------------------------------------------------------------

    iss.clear();
    iss.str("Waterton Central\n" "Waterton East\n" "Waterton Woods\n" "Little Waterton\n" "Waterton West\n"
            "Waterton Central\n");
    t0.AddLine(iss);

    // -----------------------------------------------------------------------------------------------------------------

    assert (t0.FindDest ("Maron", 0) == (set<string> {"Maron"}) );
    assert (t0.FindDest ("Maron", 1) == (set<string> {"Knapford", "Maron"}) );
    assert (t0.FindDest ("Maron", 2) == (set<string> {"Great Waterton", "Knapford", "Maron", "Sodor Streamwork", "Suderry"}) );
    assert (t0.FindDest ("Maron", 3) == (set<string> {"Brendam Docks", "Gordon's Hill", "Great Waterton", "Knapford", "Maron",
                                                      "Sodor Ironworks", "Sodor Streamwork", "Suderry"}) );
    assert (t0.FindDest ("Maron", 4) == (set<string> {"Brendam Docks", "Gordon's Hill", "Great Waterton", "Knapford", "Maron",
                                                      "Sodor Ironworks", "Sodor Streamwork", "Suderry", "Tidmouth"}) );
    assert (t0.FindDest ("Maron", 5) == (set<string> {"Brendam Docks", "Gordon's Hill", "Great Waterton", "Knapford", "Maron",
                                                      "Sodor Ironworks", "Sodor Streamwork", "Suderry", "Tidmouth"}) );

    // -----------------------------------------------------------------------------------------------------------------

    assert (t0.FindDest ("Waterton East", 0) == (set<string> {"Waterton East"}) );
    assert (t0.FindDest ("Waterton East", 1) == (set<string> {"Waterton Central", "Waterton East", "Waterton Woods"}) );
    assert (t0.FindDest ("Waterton East", 2) == (set<string> {"Little Waterton", "Waterton Central", "Waterton East",
                                                              "Waterton West", "Waterton Woods"}) );
    assert (t0.FindDest ("Waterton East", 3) == (set<string> {"Lakeside Central", "Little Waterton", "Waterton Central",
                                                              "Waterton East", "Waterton West", "Waterton Woods"}) );
    assert (t0.FindDest ("Waterton East", 4) == (set<string> {"Lakeside Central", "Little Waterton", "Newton Crossroad",
                                                              "Waterton Central", "Waterton East", "Waterton West",
                                                              "Waterton Woods"}) );
    assert (t0.FindDest ("Waterton East", 5) == (set<string> {"Castle Hill", "Lakeside Central", "Little Waterton", "Newton Crossroad",
                                                              "Waterton Central", "Waterton East", "Waterton West", "Waterton Woods"}) );
    assert (t0.FindDest ("Waterton East", 6) == (set<string> {"Castle Hill", "Lakeside Central", "Little Burnside", "Little Waterton",
                                                              "Newton Crossroad", "Waterton Central", "Waterton East", "Waterton West",
                                                              "Waterton Woods"}) );
    assert (t0.FindDest ("Waterton East", 9) == (set<string> {"Black Hill", "Castle Hill", "Great Newton", "Green Hill", "Lakeside",
                                                              "Lakeside Central", "Little Burnside", "Little Newton", "Little Waterton", "Newton Crossroad",
                                                              "Waterton Central", "Waterton East", "Waterton West", "Waterton Woods", "Wood Side"}) );
    assert (t0.FindDest ("Waterton East", 10) == (set<string> {"Black Hill", "Castle Hill", "Great Newton", "Green Hill", "Lakeside",
                                                               "Lakeside Central", "Little Burnside", "Little Newton", "Little Waterton",
                                                               "Newton", "Newton Crossroad", "Waterton Central", "Waterton East",
                                                               "Waterton West", "Waterton Woods", "Wood Side"}) );
    assert (t0.FindDest ("Waterton East", 11) == (set<string> {"Black Hill", "Castle Hill", "Great Newton", "Green Hill",
                                                               "Lakeside", "Lakeside Central", "Little Burnside",
                                                               "Little Newton", "Little Waterton", "Newton",
                                                               "Newton Crossroad", "Waterton Central", "Waterton East",
                                                               "Waterton West", "Waterton Woods", "Wood Side"}) );

    // -----------------------------------------------------------------------------------------------------------------

    iss.clear();
    iss.str("Newton\n" "Newton Crossroad\n" "Tidmouth\n" "Brendam Docks\n");
    t0.AddLine(iss);

    // -----------------------------------------------------------------------------------------------------------------

    assert (t0.FindDest ("Waterton East", 0) == (set<string> {"Waterton East"}) );
    assert (t0.FindDest ("Waterton East", 1) == (set<string> {"Waterton Central", "Waterton East", "Waterton Woods"}) );
    assert (t0.FindDest ("Waterton East", 2) == (set<string> {"Little Waterton", "Waterton Central", "Waterton East",
                                                              "Waterton West", "Waterton Woods"}) );
    assert (t0.FindDest ("Waterton East", 3) == (set<string> {"Lakeside Central", "Little Waterton", "Waterton Central",
                                                              "Waterton East", "Waterton West", "Waterton Woods"}) );
    assert (t0.FindDest ("Waterton East", 4) == (set<string> {"Lakeside Central", "Little Waterton", "Newton Crossroad",
                                                              "Waterton Central", "Waterton East", "Waterton West",
                                                              "Waterton Woods"}) );
    assert (t0.FindDest ("Waterton East", 5) == (set<string> {"Castle Hill", "Lakeside Central", "Little Waterton", "Newton", "Newton Crossroad", "Tidmouth",
                                                              "Waterton Central", "Waterton East", "Waterton West", "Waterton Woods"}) );
    assert (t0.FindDest ("Waterton East", 6) == (set<string> {"Black Hill", "Brendam Docks", "Castle Hill", "Gordon's Hill", "Lakeside Central", "Little Burnside", "Little Waterton",
                                                              "Newton", "Newton Crossroad", "Tidmouth", "Waterton Central", "Waterton East", "Waterton West",
                                                              "Waterton Woods"}) );
    assert (t0.FindDest ("Waterton East", 9) == (set<string> {"Black Hill","Brendam Docks", "Castle Hill", "Gordon's Hill", "Great Newton", "Great Waterton", "Green Hill", "Knapford", "Lakeside",
                                                              "Lakeside Central", "Little Burnside", "Little Newton", "Little Waterton", "Maron", "Newton", "Newton Crossroad",
                                                              "Sodor Streamwork", "Suderry", "Tidmouth", "Waterton Central", "Waterton East", "Waterton West", "Waterton Woods", "Wood Side"}) );
    assert (t0.FindDest ("Waterton East", 10) == (set<string> {"Black Hill", "Brendam Docks", "Castle Hill", "Gordon's Hill", "Great Newton", "Great Waterton", "Green Hill", "Knapford", "Lakeside",
                                                               "Lakeside Central", "Little Burnside", "Little Newton", "Little Waterton", "Maron",
                                                               "Newton", "Newton Crossroad", "Sodor Ironworks", "Sodor Streamwork", "Suderry", "Tidmouth", "Waterton Central", "Waterton East",
                                                               "Waterton West", "Waterton Woods", "Wood Side"}) );
    set<string> test = set<string> {"Black Hill", "Brendam Docks", "Castle Hill", "Gordon's Hill", "Great Newton", "Great Waterton", "Green Hill", "Knapford", "Lakeside",
                                    "Lakeside Central", "Little Burnside", "Little Newton", "Little Waterton", "Maron",
                                    "Newton", "Newton Crossroad", "Sodor Ironworks", "Sodor Streamwork", "Suderry", "Tidmouth", "Waterton Central", "Waterton East",
                                    "Waterton West", "Waterton Woods", "Wood Side"};
    set<string> test2 = t0.FindDest ("Waterton East", 11);
    assert (test2 == test );
    assert (t0.FindDest ("Waterton East", 1000000) == (set<string> {"Black Hill", "Brendam Docks", "Castle Hill", "Gordon's Hill", "Great Newton", "Great Waterton", "Green Hill", "Knapford", "Lakeside",
                                                                    "Lakeside Central", "Little Burnside", "Little Newton", "Little Waterton", "Maron",
                                                                    "Newton", "Newton Crossroad", "Sodor Ironworks", "Sodor Streamwork", "Suderry", "Tidmouth", "Waterton Central", "Waterton East",
                                                                    "Waterton West", "Waterton Woods", "Wood Side"}) );

    // -----------------------------------------------------------------------------------------------------------------

    assert (t0.FindDest ("Suderry Woodside", 0) == (set<string> {"Suderry Woodside"}) );
    assert (t0.FindDest ("Suderry Woodside", 666) == (set<string> {"Suderry Woodside"}) );

    // -----------------------------------------------------------------------------------------------------------------

    return EXIT_SUCCESS;
}
