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
using namespace std;

class CDelivery
{
public:
	CDelivery () : c_Conns () {}

	CDelivery& addConn (const string& a, const string& b)
	{
		c_Conns[a].insert (b); c_Conns[b].insert (a);

		return *this;
	}

	vector<int> findCoverage (const set<string>& depots) const
	{
		vector<int> coverage;
		set<string> visited_depots;
		queue<string> depots_to_visit;
		int remaining = 0, remaining_next = (int)depots.size ();

		for (const auto& depot : depots)
		{
			if (!c_Conns.count (depot)) throw invalid_argument ("");
			depots_to_visit.push (depot); visited_depots.insert (depot);
		}

		while (!depots_to_visit.empty ())
		{
			if (remaining == 0)
			{
				if (coverage.empty ()) coverage.push_back (remaining_next);
				else coverage.push_back (coverage.back () + remaining_next);

				remaining = remaining_next; remaining_next = 0;
			}

			for (const auto& depot : c_Conns.at (depots_to_visit.front ()))
			{
				if (visited_depots.count (depot)) continue;

				depots_to_visit.push (depot); visited_depots.insert (depot);
				remaining_next++;
			}

			depots_to_visit.pop ();
			remaining--;
		}

		return coverage;
	}

private:
	map<string, set<string>> c_Conns;

};

int main ()
{
	CDelivery t0;
	vector<int> r;
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

	try
	{
		r = t0.findCoverage(set<string>{"Incorrect city"});
		assert ( "No invalid_argument exception caught!" == nullptr );
	}
	catch ( const invalid_argument & e ) {}



	CDelivery t1;
	t1
		.addConn("A", "B")
		.addConn("B", "C")
		.addConn("B", "L")
		.addConn("B", "D")
		.addConn("C", "F")
		.addConn("D", "E")
		.addConn("D", "H")
		.addConn("D", "L")
		.addConn("L", "H")
		.addConn("L", "I")
		.addConn("L", "M")
		.addConn("F", "E")
		.addConn("E", "H")
		.addConn("E", "G")
		.addConn("G", "H")
		.addConn("H", "K")
		.addConn("I", "K")
		.addConn("M", "K")
		.addConn("M", "N")
		.addConn("K", "N")
		.addConn("K", "J")
		.addConn("K", "O")
		.addConn("O", "V")
		.addConn("V", "W")
		.addConn("V", "X")
		.addConn("X", "W")
		.addConn("X", "Y")
		.addConn("W", "Y")
		.addConn("P", "Q")
		.addConn("Q", "S")
		.addConn("S", "T")
		.addConn("T", "R")
		.addConn("T", "U")
		.addConn("R", "U");

	r = t1.findCoverage(set<string>{"A"});
	assert ( r == (vector<int>{1, 2, 5, 10, 13, 15, 16, 18, 19}) );

	r = t1.findCoverage(set<string>{"E", "N", "Q"});
	assert ( r == (vector<int>{3, 11, 18, 22, 24, 25}) );

	return EXIT_SUCCESS;
}
