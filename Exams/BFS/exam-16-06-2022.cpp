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
 * @brief Class representing a delivery network.
 */
class CDelivery {
public:
	// ctor // todo
	CDelivery() = default;

	// dtor // todo
	~CDelivery() = default;

	// -----------------------------------------------------------------------------------------------------------------

	// add connection from -> to, fluent interface // todo
	/**
	 * @brief Add a connection from one location to another.
	 * @param from The starting location.
	 * @param to The destination location.
	 * @return Reference to the CDelivery object for fluent interface.
	 */
	CDelivery & addConn(const string & from, const string & to) {
		connections_[from].insert(to);

		return * this;
	}

	// returns a map containing shortest path to each customer from any of the depots
	// map key is the customer name, value is a list of nodes on the shortest path (depot -> customer)
	// if there is no path, the value is an empty list // todo
	/**
	 * @brief Serve customers by finding the shortest path from depots to each customer.
	 * @param customers A set of customer locations.
	 * @param depots A set of depot locations.
	 * @return A map containing the shortest path to each customer from any of the depots.
	 *         The map key is the customer name, and the value is a list of nodes on the shortest path.
	 *         If there is no path, the value is an empty list.
	 */
	map<string, list<string>> serveCustomers(const set<string> & customers, const set<string> & depots) const {
		map<string, map<string, string>> customer_paths;

		for (const string & customer : customers) {
			map<string, string> paths;
			queue<string> planned;

			for (const string & depot : depots) {
				paths[depot] = "";
				planned.push(depot);
			}

			for (bool is_found = false; !planned.empty() && !is_found; ) {
				string city = std::move(planned.front());
				planned.pop();

				if (!connections_.at(city).empty())
					for (const string & neighbor : connections_.at(city))
						if (!paths.contains(neighbor)) {
							paths[neighbor] = city;

							if (neighbor == customer) {
								is_found = true;
								break;
							}

							planned.push(neighbor);
						}
			}

			if (paths.contains(customer))
				customer_paths[customer] = std::move(paths);
		}

		map<string, list<string>> customer_shortest_paths;

		for (const string & customer : customers) {
			list<string> shortest_path;

			if (customer_paths.contains(customer))
				for (string city = customer; !city.empty(); city = customer_paths.at(customer).at(city))
					shortest_path.push_front(city);

			customer_shortest_paths[customer] = std::move(shortest_path);
		}

		return customer_shortest_paths;
	}

private:
	// todo
	map<string, set<string>> connections_; // Map of locations and their connected locations
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

int main() {
	CDelivery t0;
	map<string, list<string>> r;

	// -----------------------------------------------------------------------------------------------------------------

	t0.addConn("Austin", "Berlin");
	t0.addConn("Chicago", "Berlin");
	t0.addConn("Berlin", "Dallas");
	t0.addConn("Dallas", "Essen");
	t0.addConn("Essen", "Austin");
	t0.addConn("Frankfurt", "Gyor");
	t0.addConn("Gyor", "Helsinki");
	t0.addConn("Helsinki", "Frankfurt");

	// -----------------------------------------------------------------------------------------------------------------

	r = t0.serveCustomers(set<string>{"Berlin", "Gyor"}, set<string>{"Essen", "Helsinki"});
	assert ( r == (map<string, list<string>>{ {"Berlin", {"Essen", "Austin", "Berlin"}}, {"Gyor", {"Helsinki", "Frankfurt", "Gyor"}} }) );

	// -----------------------------------------------------------------------------------------------------------------

	r = t0.serveCustomers(set<string>{"Austin", "Gyor", "Chicago"}, set<string>{"Austin", "Dallas"});
	assert ( r == (map<string, list<string>>{ {"Austin", {"Austin"}}, {"Chicago", { }}, {"Gyor", { }} }) );

	// -----------------------------------------------------------------------------------------------------------------

	t0.addConn("Chicago", "Helsinki");

	// -----------------------------------------------------------------------------------------------------------------

	r = t0.serveCustomers(set<string>{"Austin", "Gyor", "Chicago"}, set<string>{"Austin", "Dallas"});
	assert ( r == (map<string, list<string>>{ {"Austin", {"Austin"}}, {"Chicago", { }}, {"Gyor", { }} }) );

	// -----------------------------------------------------------------------------------------------------------------

	t0.addConn("Berlin", "Chicago");

	// -----------------------------------------------------------------------------------------------------------------

	r = t0.serveCustomers(set<string>{"Austin", "Gyor", "Chicago"}, set<string>{"Austin", "Dallas"});
	assert ( r == (map<string, list<string>>{ {"Austin", {"Austin"}}, {"Chicago", {"Austin", "Berlin", "Chicago"}}, {"Gyor", {"Austin", "Berlin", "Chicago", "Helsinki", "Frankfurt", "Gyor"}} }) );

	// -----------------------------------------------------------------------------------------------------------------

	t0.addConn("Essen", "Frankfurt");

	// -----------------------------------------------------------------------------------------------------------------

	r = t0.serveCustomers(set<string>{"Austin", "Gyor", "Chicago"}, set<string>{"Austin", "Dallas"});
	assert ( r == (map<string, list<string>>{ {"Austin", {"Austin"}}, {"Chicago", {"Austin", "Berlin", "Chicago"}}, {"Gyor", {"Dallas", "Essen", "Frankfurt", "Gyor"}} }) );

	// -----------------------------------------------------------------------------------------------------------------

	return EXIT_SUCCESS;
}
