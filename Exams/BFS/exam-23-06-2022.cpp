#include <sstream>
#include <string>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include <memory>
#include <cassert>

// ---------------------------------------------------------------------------------------------------------------------

using namespace std;

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Class for managing delivery connections and calculating coverage.
 */
class CDelivery {
public:
	// ctor // todo
	CDelivery() = default;

	// dtor // todo
	~CDelivery() = default;

	// -----------------------------------------------------------------------------------------------------------------

	// todo
	/**
	 * @brief Add a connection between two locations.
	 * @param from The starting location.
	 * @param to The destination location.
	 * @return Reference to the current CDelivery object.
	 */
	CDelivery & addConn(const string & from, const string & to) {
		connections_[from].insert(to);
		connections_[to].insert(from);

		return * this;
	}

	// todo
	/**
	 * @brief Find the coverage of delivery locations starting from given depots.
	 * @param depots The set of depot locations.
	 * @return Vector representing the cumulative coverage at each level.
	 */
	vector<int> findCoverage(const set<string> & depots) const {
		map<string, int> costed;
		map<int, set<string>> cost_cities;
		queue<string> planned;

		for (const string & depot : depots) {
			if (!connections_.contains(depot))
				throw invalid_argument("");

			costed[depot] = 0;
			cost_cities[0].insert(depot);
			planned.push(depot);
		}

		while (!planned.empty()) {
			string city = std::move(planned.front());
			planned.pop();

			if (int cost = costed.at(city); !connections_.at(city).empty())
				for (const string & neighbor : connections_.at(city))
					if (!costed.contains(neighbor)) {
						costed[neighbor] = cost + 1;
						cost_cities[cost + 1].insert(neighbor);
						planned.push(neighbor);
					}
		}

		vector<int> coverage;

		for (auto it = cost_cities.begin(); it != cost_cities.end(); ++it)
			if (it == cost_cities.begin()) coverage.push_back(static_cast<int>(it->second.size()));
			else coverage.push_back(coverage.back() + static_cast<int>(it->second.size()));

		return coverage;
	}

private:
	// todo
	map<string, set<string>> connections_; // Map of connections between locations
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

int main() {
	CDelivery t0;
	vector<int> r;

	// -----------------------------------------------------------------------------------------------------------------

	t0
	.addConn("Austin", "Berlin")
	.addConn("Chicago", "Berlin")
	.addConn("Chicago", "Dallas")
	.addConn("Dallas", "Essen")
	.addConn("Essen", "Austin")
	.addConn("Frankfurt", "Essen")
	.addConn("Gyor", "Frankfurt")
	.addConn("Helsinki", "Istanbul")
	.addConn("Istanbul", "Jakarta");

	// -----------------------------------------------------------------------------------------------------------------

	r = t0.findCoverage(set<string>{"Berlin"});
	assert ( r == (vector<int>{1, 3, 5, 6, 7}) );
	/* result:
	 * [0]: Berlin = 1
	 * [1]: Austin, Berlin, Chicago = 3
	 * [2]: Austin, Berlin, Chicago, Dallas, Essen = 5
	 * [3]: Austin, Berlin, Chicago, Dallas, Essen, Frankfurt = 6
	 * [4]: Austin, Berlin, Chicago, Dallas, Essen, Frankfurt, Gyor = 7
	 */

	r = t0.findCoverage(set<string>{"Berlin", "Essen"});
	assert ( r == (vector<int>{2, 6, 7}) );

	r = t0.findCoverage(set<string>{"Helsinki"});
	assert ( r == (vector<int>{1, 2, 3}) );

	r = t0.findCoverage(set<string>{"Istanbul"});
	assert ( r == (vector<int>{1, 3}) );

	r = t0.findCoverage(set<string>{"Austin", "Jakarta"});
	assert ( r == (vector<int>{2, 5, 9, 10}) );

	r = t0.findCoverage(set<string>{"Chicago", "Gyor", "Helsinki", "Jakarta"});
	assert ( r == (vector<int>{4, 8, 10}) );

	try {
		r = t0.findCoverage(set<string>{"Incorrect city"});
		assert ( "No invalid_argument exception caught!" == nullptr );
	} catch ( const invalid_argument & e ) {}

	// -----------------------------------------------------------------------------------------------------------------

	return EXIT_SUCCESS;
}
