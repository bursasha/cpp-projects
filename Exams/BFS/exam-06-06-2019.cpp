#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include <vector>
#include <list>
#include <deque>
#include <queue>
#include <stack>

// ---------------------------------------------------------------------------------------------------------------------

using namespace std;

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Class representing a train network with stations and connections.
 */
class CTrain {
public:
    CTrain() = default;

    ~CTrain() = default;

    // -----------------------------------------------------------------------------------------------------------------

    // todo
    /**
     * @brief Add stations and their connections to the train network.
     * @param is Input stream containing station names in order of their connections.
     */
    void Add(istringstream & is) {
        for (string prev_station, curr_station; getline(is, curr_station);
                prev_station = curr_station, curr_station.clear()) {
            stations.insert(curr_station);

            if (!prev_station.empty()) {
                station_neighbors[prev_station].insert(curr_station);
                station_neighbors[curr_station].insert(prev_station);
            }

            if (!station_neighbors.contains(curr_station))
                station_neighbors[curr_station] = {};
        }
    }

    // todo
    /**
     * @brief Count the number of disconnected groups of stations in the train network.
     * @return The number of disconnected groups of stations.
     */
    int Count() const {
        int links = 0;

        set<string> all = stations, visited;
        queue<string> plan;
        string station;

        if (!all.empty()) {
            station = *all.begin();
            ++links;
            all.erase(station);
            visited.insert(station);
            plan.push(station);
        }

        while (!plan.empty()) {
            station = plan.front();
            plan.pop();

            if (station_neighbors.contains(station))
                for (const auto & neighbor : station_neighbors.at(station))
                    if (!visited.contains(neighbor)) {
                        all.erase(neighbor);
                        visited.insert(neighbor);
                        plan.push(neighbor);
                    }

            if (plan.empty() && !all.empty()) {
                station = *all.begin();
                ++links;
                all.erase(station);
                visited.insert(station);
                plan.push(station);
            }
        }

        return links;
    }

private:
    // todo
    set<string> stations; // Set of all station names
    map<string, set<string>> station_neighbors; // Map of station names to their connected neighbors
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

int main() {
    CTrain t0;
    istringstream iss;

    // -----------------------------------------------------------------------------------------------------------------

    iss . clear();
    iss . str("Newton\nBlack Hill\nWood Side\nGreen Hill\nLakeside\n");
    t0 . Add(iss);
    cout << "res : " << t0.Count() << "  ref : 1" << endl;

    // -----------------------------------------------------------------------------------------------------------------

    iss . clear();
    iss . str("Little Newton\nLittle Burnside\nCastle Hill\nNewton Crossroad\nLakeside Central\n");
    t0 . Add(iss);
    cout << "res : " << t0.Count() << "  ref : 2" << endl;

    // -----------------------------------------------------------------------------------------------------------------

    iss . clear();
    iss . str("Waterton West\nWaterton Central\nWaterton East\nWaterton Woods\nLittle Waterton\nWaterton West\n");
    t0 . Add(iss);
    cout << "res : " << t0.Count() << "  ref : 3" << endl;

    // -----------------------------------------------------------------------------------------------------------------

    iss . clear();
    iss . str("Little Waterton\nLakeside Central\n");
    t0 . Add(iss);
    cout << "res : " << t0.Count() << "  ref : 2" << endl;

    // -----------------------------------------------------------------------------------------------------------------

    iss . clear();
    iss . str("Great Newton\nLittle Burnside\nGreen Hill\nWood Side\n");
    t0 . Add(iss);
    cout << "res : " << t0.Count() << "  ref : 1" << endl;

    // -----------------------------------------------------------------------------------------------------------------

    iss . clear();
    iss . str("Sodor Ironworks\nSodor Steamworks\nKnapford\nMaron\n");
    t0 . Add(iss);
    cout << "res : " << t0.Count() << "  ref : 2" << endl;

    // -----------------------------------------------------------------------------------------------------------------

    iss . clear();
    iss . str("Tidmouth\nGordons Hill\nSuderry\nKnapford\nGreat Waterton\nBrendam Docks\n");
    t0 . Add(iss);
    cout << "res : " << t0.Count() << "  ref : 2" << endl;

    // -----------------------------------------------------------------------------------------------------------------

    iss . clear();
    iss . str("Newton\nNewton Crossroad\nTidmouth\nBrendam Docks\n");
    t0 . Add(iss);
    cout << "res : " << t0.Count() << "  ref : 1" << endl;

    // -----------------------------------------------------------------------------------------------------------------

    return EXIT_SUCCESS;
}
