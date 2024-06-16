#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <memory>

// ---------------------------------------------------------------------------------------------------------------------

using namespace std;

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Class for managing teleportation connections and finding optimal travel paths.
 */
class CTeleport {
public:
    // ctor // todo
    CTeleport() = default;

    // dtor // todo
    ~CTeleport() = default;

    // -----------------------------------------------------------------------------------------------------------------

    // todo
    /**
     * @brief Adds a teleport connection between two locations with specified departure and arrival times.
     * @param from The starting location.
     * @param to The destination location.
     * @param departure The departure time.
     * @param arrival The arrival time.
     * @return Reference to the current CTeleport object.
     */
    CTeleport & Add(const string & from, const string & to, unsigned departure, unsigned arrival) {
        CConnection connection = {from, to};
        CSchedule schedule = {departure, arrival};

        connections_[connection.first].insert(connection.second);
        reversed_connections_[connection.second].insert(connection.first);
        schedules_[connection].insert(schedule);

        if (!min_arrivals_.contains(connection.second) || min_arrivals_.at(connection.second) > schedule.second)
            min_arrivals_[connection.second] = schedule.second;

        return * this;
    }

    // todo
    /**
     * @brief Optimizes the teleportation network (currently does nothing).
     * @return Reference to the current CTeleport object.
     */
    CTeleport & Optimize() {
        return * this;
    }

    // todo
    /**
     * @brief Finds the earliest arrival time from a starting location to a destination after a given departure time.
     * @param from The starting location.
     * @param to The destination location.
     * @param departed The departure time.
     * @return The earliest arrival time.
     * @throws invalid_argument if a valid path cannot be found.
     */
    unsigned FindWay(const string & from, const string & to, unsigned departed) {
        if (from == to)
            return 0;

        if (!connections_.contains(from) || !reversed_connections_.contains(to))
            throw invalid_argument("");

        map<CConnection, set<CSchedule>> used;
        queue<pair<CConnection, CSchedule>> planned;

        if (!connections_.at(from).empty())
            for (const string & neighbor : connections_.at(from))
                if (CConnection connection = {from, neighbor}; !schedules_.at(connection).empty())
                    for (const CSchedule & schedule : schedules_.at(connection))
                        if (schedule.first >= departed) {
                            if (connection.second == to && min_arrivals_.at(to) == schedule.second)
                                return schedule.second;

                            used[connection].emplace(schedule);
                            planned.emplace(connection, schedule);
                        }

        while (!planned.empty()) {
            pair<CConnection, CSchedule> teleport = std::move(planned.front());
            planned.pop();

            if (connections_.contains(teleport.first.second) && !connections_.at(teleport.first.second).empty())
                for (const string & neighbor : connections_.at(teleport.first.second))
                    if (CConnection connection = {teleport.first.second, neighbor}; !schedules_.at(connection).empty())
                        for (const CSchedule & schedule : schedules_.at(connection))
                            if ((!used.contains(connection) || !used.at(connection).contains(schedule))
                                    && schedule.first >= teleport.second.second) {
                                if (connection.second == to && min_arrivals_.at(to) == schedule.second)
                                    return schedule.second;

                                used[connection].emplace(schedule);
                                planned.emplace(connection, schedule);
                            }
        }

        if (none_of(reversed_connections_.at(to).begin(), reversed_connections_.at(to).end(),
            [& used, & to] (const string & previous) { return used.contains({previous, to}); }))
            throw invalid_argument("");

        unsigned min_arrival = UINT_MAX;

        for (const string & previous : reversed_connections_.at(to))
            if (CConnection connection = {previous, to}; used.contains(connection))
                for (const CSchedule & schedule : used.at(connection))
                    if (min_arrival > schedule.second)
                        min_arrival = schedule.second;

        return min_arrival;
    }

private:
    // todo
    using CConnection = pair<string, string>; // Type alias for connection between two locations
    using CSchedule = pair<unsigned, unsigned>; // Type alias for schedule with departure and arrival times

    // -----------------------------------------------------------------------------------------------------------------

    map<string, set<string>> connections_, reversed_connections_; // Map of connections from a location, Map of reversed connections to a location
    map<CConnection, set<CSchedule>> schedules_; // Map of schedules for each connection
    map<string, unsigned> min_arrivals_; // Map of minimum arrival times for each location
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

int main() {
    CTeleport t;

    // -----------------------------------------------------------------------------------------------------------------

    t . Add ( "Prague", "Vienna", 0, 7 )
      . Add ( "Vienna", "Berlin", 9, 260 )
      . Add ( "Vienna", "London", 8, 120 )
      . Add ( "Vienna", "Chicago", 4, 3 )
      . Add ( "Prague", "Vienna", 10, 10 ) . Optimize ( );

    // -----------------------------------------------------------------------------------------------------------------

    assert ( t . FindWay ( "Prague", "Vienna", 0 ) == 7 );
    assert ( t . FindWay ( "Prague", "Vienna", 1 ) == 10 );
    assert ( t . FindWay ( "Prague", "London", 0 ) == 120 );
    assert ( t . FindWay ( "Vienna", "Chicago", 4 ) == 3 );

    // -----------------------------------------------------------------------------------------------------------------

    try {
        t . FindWay ( "Prague", "London", 2 );
        assert ( "Missing exception" == nullptr );
    } catch ( const invalid_argument & e ) { }
    catch ( ... ) { assert ( "Invalid exception" == nullptr ); }

    // -----------------------------------------------------------------------------------------------------------------

    try {
        t . FindWay ( "Prague", "Chicago", 0 );
        assert ( "Missing exception" == nullptr );
    } catch ( const invalid_argument & e ) { }
    catch ( ... ) { assert ( "Invalid exception" == nullptr ); }

    // -----------------------------------------------------------------------------------------------------------------

    t . Add ( "Dallas", "Atlanta", 150, 30 )
      . Add ( "Berlin", "Helsinki", 1080, 2560 )
      . Add ( "Chicago", "Frankfurt", 50, 0 )
      . Add ( "Helsinki", "Vienna", 3200, 3 )
      . Add ( "Chicago", "London", 10, 12 )
      . Add ( "London", "Atlanta", 20, 40 )
      . Add ( "Vienna", "Atlanta", 10, 50 )
      . Add ( "Prague", "Vienna", 1, 6 )
      . Add ( "Berlin", "Helsinki", 265, 265 )
      . Add ( "Berlin", "London", 259, 0 ) . Optimize ( );

    // -----------------------------------------------------------------------------------------------------------------

    assert ( t . FindWay ( "Prague", "Frankfurt", 0 ) == 0 );
    assert ( t . FindWay ( "Prague", "Atlanta", 0 ) == 40 );
    assert ( t . FindWay ( "Prague", "Atlanta", 10 ) == 50 );

    // -----------------------------------------------------------------------------------------------------------------

    return EXIT_SUCCESS;
}
