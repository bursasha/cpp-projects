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
using namespace std;

class CMHD
{
public:
	//Add
	void Add (istringstream& is)
	{
		string station;
		size_t new_line_number = m_Lines.size ();

		while (getline (is, station))
		{
			m_Lines[new_line_number].insert (station); m_StationConnections[station].insert (new_line_number);

			if (m_StationConnections.count (station))
			{
				for (const auto& line_number : m_StationConnections[station])
				{ m_LinesConnections[new_line_number].insert (line_number); m_LinesConnections[line_number].insert (new_line_number); }
			}
		}

		m_LinesConnections[new_line_number];
	}

	//Dest
	set<string> Dest (const string& from, int max_cost)
	{
		set<string> available_stations = { from };
		set<size_t> visited_lines;
		queue<size_t> lines_to_visit;

		if (!m_StationConnections.count (from)) return available_stations;

		for (const auto& line_number : m_StationConnections.at (from))
		{ lines_to_visit.push (line_number); visited_lines.insert (line_number); }

		int remaining_levels = max_cost; size_t remaining_on_level = lines_to_visit.size ();

		while (!lines_to_visit.empty ())
		{
			if (remaining_on_level == 0)
			{
				remaining_levels--; remaining_on_level = lines_to_visit.size ();

				if (remaining_levels < 0) { remaining_levels = 0; remaining_on_level = 0; lines_to_visit = queue<size_t> (); break; }
			}

			available_stations.insert (m_Lines.at (lines_to_visit.front ()).begin (), m_Lines.at (lines_to_visit.front ()).end ());

			for (const auto& line_to_visit : m_LinesConnections.at (lines_to_visit.front ()))
			{
				if (!visited_lines.count (line_to_visit)) { lines_to_visit.push (line_to_visit); visited_lines.insert (line_to_visit); }
			}

			lines_to_visit.pop (); remaining_on_level--;
		}

		return available_stations;
	}

private:
	map<size_t, set<string>> m_Lines;
	map<string, set<size_t>> m_StationConnections;
	map<size_t, set<size_t>> m_LinesConnections;

};


int main ()
{
	CMHD city;
	istringstream iss;
	iss.clear();

	iss . str ( "A\nB\nC\nD\nE\n" );
	city . Add ( iss );
	iss . clear();

	iss . str ( "B\nC\nF\nH\n" );
	city . Add ( iss );
	iss . clear();

	iss . str ( "F\nG\nI\nJ\nK\nN\n" );
	city . Add ( iss );
	iss . clear();

	iss . str ( "H\nL\n" );
	city . Add ( iss );
	iss . clear();

	iss . str ( "L\nM\nN\nO\n" );
	city . Add ( iss );
	iss . clear();

	iss . str ( "P\nQ\nR\nN\nS" );
	city . Add ( iss );
	iss . clear();

	assert ( city . Dest ( "S", 0 ) == set < string > ( {"S", "N", "R", "Q", "P"} ) );

	assert ( city . Dest ( "S", 1 ) == set < string > ( { "S", "N", "R", "Q", "P",
	                                                      "O", "M", "L",
	                                                      "K", "J", "I", "G", "F" } ) );

	assert ( city . Dest ( "N", 0 ) == set < string > ( { "S", "N", "R", "Q", "P",
	                                                      "O", "M", "L",
	                                                      "K", "J", "I", "G", "F" } ) );

	assert ( city . Dest ( "N", 1 ) == set < string > ( { "S", "N", "R", "Q", "P",
	                                                      "O", "M", "L",
	                                                      "K", "J", "I", "G", "F",
	                                                      "H", "F", "C", "B" } ) );

	assert ( city . Dest ( "N", 2 ) == set < string > ( { "S", "N", "R", "Q", "P",
	                                                      "O", "M", "L",
	                                                      "K", "J", "I", "G", "F",
	                                                      "H", "F", "C", "B",
	                                                      "A", "D", "E" } ) );

	assert ( city . Dest ( "unknown", 0 ) == set < string > ( { "unknown" } ) );
	assert ( city . Dest ( "unknown", 1 ) == set < string > ( { "unknown" } ) );
	assert ( city . Dest ( "unknown", 2 ) == set < string > ( { "unknown" }) );

	return 0;
}
