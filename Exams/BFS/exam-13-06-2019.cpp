#include <iostream>
#include <sstream>
#include <map>
#include <set>
#include <cassert>
#include <queue>

// ---------------------------------------------------------------------------------------------------------------------

using namespace std;

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Class representing a train station network.
 */
class CTrain {
public:
    // todo
    CTrain() = default;

    // todo
    ~CTrain() = default;

    // -----------------------------------------------------------------------------------------------------------------

    // todo
    /**
     * @brief Add a line of stations to the network.
     * @param is Input stream containing station names.
     */
    void AddLine(istream & is) {
        for (string previous, current; getline(is, current); previous = current, current.clear()) {
            if (!previous.empty()) {
                stations[previous].insert(current);
                stations[current].insert(previous);
            }

            if (!stations.contains(current)) stations[current] = {};
        }
    }

    // todo
    /**
     * @brief Find meeting points with minimum travel cost from given starting stations.
     * @param from Vector of starting station names.
     * @param min_cost Reference to store the minimum travel cost.
     * @return Set of station names that are meeting points.
     */
    set<string> MeetingPoints(const vector<string> & from, int & min_cost) {
        if (from.size() == 1) {
            min_cost = 0;
            return {from.front()};
        }

        map<string, map<string, int>> from_costs;

        for (const auto & from_station : from) {
            set<string> visited;
            map<string, int> costs;
            queue<string> plan;

            visited.insert(from_station);
            costs[from_station] = 0;
            plan.push(from_station);

            while (!plan.empty()) {
                string curr_station = plan.front();
                plan.pop();

                if (int curr_cost = costs.at(curr_station);
                        stations.contains(curr_station) && !stations.at(curr_station).empty())
                    for (const auto & next_station : stations.at(curr_station))
                        if (!visited.contains(next_station)) {
                            visited.insert(next_station);
                            costs[next_station] = curr_cost + 1;
                            plan.push(next_station);
                        }
            }

            from_costs[from_station] = costs;
        }

        map<int, set<string>> meet_stations;

        for (const auto & [meet_station, next_stations] : stations) {
            int meet_cost = 0;
            bool is_valid = true;

            for (const auto & [from_station, meet_costs] : from_costs)
                if (meet_costs.contains(meet_station)) meet_cost += meet_costs.at(meet_station);
                else is_valid = false;

            if (is_valid) meet_stations[meet_cost].insert(meet_station);
        }

        if (!meet_stations.empty()) {
            min_cost = meet_stations.begin()->first;
            return meet_stations.begin()->second;
        }

        return {};
    }

private:
    // todo
    map<string, set<string>> stations; // Map of stations and their connected stations
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

int main() {
    CTrain t0;
    istringstream iss;

    // -----------------------------------------------------------------------------------------------------------------

    iss . clear ();
    iss . str ( "1\n" "2\n" "3\n" "4\n" "5\n" "6\n" "7\n" "8\n" );
    t0 . AddLine ( iss );

    // -----------------------------------------------------------------------------------------------------------------

    iss . clear ();
    iss . str ( "12\n" "11\n" "4\n" "9\n" "10\n" );
    t0 . AddLine ( iss );

    // -----------------------------------------------------------------------------------------------------------------

    iss . clear ();
    iss . str ( "15\n" "11\n" "13\n" "14\n" );
    t0 . AddLine ( iss );

    // -----------------------------------------------------------------------------------------------------------------

    iss . clear ();
    iss . str ( "7\n" "16\n" "17\n" );
    t0 . AddLine ( iss );

    // -----------------------------------------------------------------------------------------------------------------

    iss . clear ();
    iss . str ( "20\n" "19\n" "17\n" "18\n" );
    t0 . AddLine ( iss );

    // -----------------------------------------------------------------------------------------------------------------

    iss . clear ();
    iss . str ( "21\n" "22\n" "23\n" "25\n" "24\n" "21\n" );
    t0 . AddLine ( iss );

    // -----------------------------------------------------------------------------------------------------------------

    iss . clear ();
    iss . str ( "26\n" "27\n" "28\n" );
    t0 . AddLine ( iss );

    // -----------------------------------------------------------------------------------------------------------------

    iss . clear ();
    iss . str ( "27\n" "29\n" "30\n" );
    t0 . AddLine ( iss );

    // -----------------------------------------------------------------------------------------------------------------

    int cost = 5381;

    assert ( t0 . MeetingPoints ({ "2", "5", "11" }, cost ) == set <string> ({ "4" })
              && cost == 4 );
    assert ( t0 . MeetingPoints ({ "12", "10" }, cost ) == set <string> ({ "10", "9", "4", "11", "12" })
              && cost == 4 );
    assert ( t0 . MeetingPoints ({ "2", "9", "19" }, cost ) == set <string> ({ "4" })
              && cost == 9 );
    assert ( t0 . MeetingPoints ({ "15", "12", "13" }, cost ) == set <string> ({ "11" })
              && cost == 3 );

    // -----------------------------------------------------------------------------------------------------------------

    cost = 5381;

    assert ( t0 . MeetingPoints ({ "21", "26" }, cost ) == set <string> ()
              && cost == 5381 );

    // -----------------------------------------------------------------------------------------------------------------

    cost = 1256;

    assert ( t0 . MeetingPoints ({ "10", "28" }, cost ) == set <string> ()
              && cost == 1256 );
    assert ( t0 . MeetingPoints ({ "21", "25" }, cost ) == set <string> ({ "21", "24", "25" })
              && cost == 2 );
    assert ( t0 . MeetingPoints ({ "21", "21" }, cost ) == set <string> ({ "21" })
              && cost == 0 );
    assert ( t0 . MeetingPoints ({ "23", "21" }, cost ) == set <string> ({ "21", "22", "23" })
              && cost == 2 );
    assert ( t0 . MeetingPoints ({ "12", "20" }, cost ) == set <string> ({ "12", "11", "4", "5", "6", "7", "16", "17", "19", "20" })
              && cost == 9 );

    assert ( t0 . MeetingPoints ({ "50" }, cost ) == set <string> ({ "50" })
              && cost == 0 );
    assert ( t0 . MeetingPoints ({ "13", "10" }, cost ) == set<string> ({ "10", "11", "13", "4", "9" })
              && cost == 4 );
    assert ( t0 . MeetingPoints ({ "17", "19" }, cost ) == set<string> ({ "17", "19" })
              && cost == 1 );

    // -----------------------------------------------------------------------------------------------------------------

    cost = 1999;

    assert ( t0 . MeetingPoints ({ "29", "25" }, cost ) == set<string> ()
              && cost == 1999 );
    assert ( t0 . MeetingPoints ({ "8", "5" }, cost ) == set<string> ({ "5", "6", "7", "8" })
              && cost == 3 );
    assert ( t0 . MeetingPoints ({ "4", "4" }, cost ) == set<string> ({ "4" })
              && cost == 0 );

    // -----------------------------------------------------------------------------------------------------------------

    return EXIT_SUCCESS;
}