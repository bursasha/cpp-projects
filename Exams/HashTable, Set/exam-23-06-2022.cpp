#include <iostream>
#include <vector>
#include <cassert>

// ---------------------------------------------------------------------------------------------------------------------

using namespace std;

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Class representing a social network.
 */
class CNetwork {
public:
	/**
	 * @brief Class representing a person in the network.
	 */
	class CPerson {
	public:
		/**
		 * @brief Constructs a person with the given name.
		 * @param name The name of the person.
		 */
		CPerson(const string & name) : m_Name(name), m_Next(nullptr) {}

		// -------------------------------------------------------------------------------------------------------------

		string m_Name; // The name of the person
		CPerson * m_Next; // Pointer to the next person in the hash bucket
		vector<CPerson *> m_Friends; // List of friends of the person
	};

	// -----------------------------------------------------------------------------------------------------------------

	size_t m_Size; // The size of the hash table
	CPerson ** m_Table; // he hash table

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Constructs a network with the given size.
	 * @param size The size of the hash table.
	 */
	CNetwork(size_t size) : m_Size(size), m_Table(new CPerson * [m_Size]) {
		for (size_t i = 0; i < m_Size; ++i) m_Table[i] = nullptr;
	}

	/**
	 * @brief Copy constructor.
	 * @param src The source network to copy from.
	 */
	CNetwork(const CNetwork & src) { // todo
		m_Size = src.m_Size;
		m_Table = new CPerson * [m_Size];

		for (size_t i = 0; i < m_Size; ++i)
			for (CPerson * src_person = src.m_Table[i], * previous = nullptr; src_person;
					src_person = src_person->m_Next) {
				auto person = new CPerson(src_person->m_Name);

				if (!previous) m_Table[i] = person;
				else previous->m_Next = person;

				previous = person;
			}

		for (size_t i = 0; i < m_Size; ++i)
			for (auto src_person = src.m_Table[i], person = m_Table[i]; src_person;
					src_person = src_person->m_Next, person = person->m_Next)
				for (auto src_friend : src_person->m_Friends)
					person->m_Friends.push_back(Find(src_friend->m_Name));
	}

	CNetwork & operator=(const CNetwork &) = delete;

	/**
	 * @brief Destructor to clean up the network and its people.
	 */
	~CNetwork() { // todo
		for (size_t i = 0; i < m_Size; ++i)
			for (auto person = m_Table[i]; person; ) {
				auto next = person->m_Next;
				delete person;
				person = next;
			}

		m_Size = 0;

		delete[] m_Table;
		m_Table = nullptr;
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Hash function to compute the index for a given string.
	 * @param s The string to hash.
	 * @return The computed index.
	 */
	size_t hashFn(const string & s) const {
		hash<string> h;

		return h(s) % m_Size;
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Prints the current state of the hash table.
	 */
	void print() const {
		cout << "--------------------------------------------HASHTABLE--------------------------------------------" << endl;
		for (size_t i = 0; i < m_Size; ++i) {
			cout << "HashTable[" << i << "]: ";

			for (auto person = m_Table[i]; person; person = person->m_Next) {
				if (person != m_Table[i]) cout << " -> ";

				cout << "{" << person->m_Name << ", " << "[";

				for (size_t j = 0; j < person->m_Friends.size(); ++j) {
					if (j > 0) cout << ", ";

					cout << person->m_Friends[j]->m_Name;
				}

				cout << "]}";
			}

			cout << endl;
		}
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Inserts a person into the network.
	 * @param name The name of the person to insert.
	 * @return True if the person is inserted successfully, false if the person already exists.
	 */
	bool Ins(const string & name) {
		if (Find(name)) return false;

		size_t index = hashFn(name);
		cout << "Name: " << name << ", HashIndex = " << index << endl;
		auto person = new CPerson(name);

		person->m_Next = m_Table[index];
		m_Table[index] = person;

		return true;
	}

	/**
	 * @brief Finds a person in the network by name.
	 * @param name The name of the person to find.
	 * @return A pointer to the person if found, nullptr otherwise.
	 */
	CPerson * Find(const string & name) const {
		for (auto person = m_Table[hashFn(name)]; person; person = person->m_Next)
			if (person->m_Name == name) return person;

		return nullptr;
	}

	/**
	 * @brief Makes two people friends in the network.
	 * @param name1 The name of the first person.
	 * @param name2 The name of the second person.
	 * @return True if the friendship is created successfully, false if either person does not exist.
	 */
	bool Friends(const string & name1, const string & name2) {
		auto person1 = Find(name1), person2 = Find(name2);

		if (!person1 || !person2) return false;

		person1->m_Friends.push_back(person2);
		person2->m_Friends.push_back(person1);

		return true;
	}
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Tests the state of the network against expected values.
 * @param t The network to test.
 */
void testNetwork(const CNetwork & t) {
	assert ( t.m_Size == 4 );

	assert ( t.m_Table[0]->m_Name == "Fred" );
	assert ( t.m_Table[0]->m_Next->m_Name == "Dave" );
	assert ( t.m_Table[0]->m_Next->m_Next->m_Name == "Bob" );
	assert ( t.m_Table[0]->m_Next->m_Next->m_Next == nullptr );
	assert ( t.m_Table[1]->m_Name == "Helen" );
	assert ( t.m_Table[1]->m_Next->m_Name == "Grace" );
	assert ( t.m_Table[1]->m_Next->m_Next->m_Name == "Carol" );
	assert ( t.m_Table[1]->m_Next->m_Next->m_Next->m_Name == "Alice" );
	assert ( t.m_Table[1]->m_Next->m_Next->m_Next->m_Next == nullptr );
	assert ( t.m_Table[2] == nullptr );
	assert ( t.m_Table[3]->m_Name == "Evelyn" );
	assert ( t.m_Table[3]->m_Next == nullptr );

	assert ( t.m_Table[0]->m_Friends.size() == 1 );
	assert ( t.m_Table[0]->m_Friends[0]->m_Name == "Grace" );
	assert ( t.m_Table[0]->m_Next->m_Friends.size() == 2 );
	assert ( t.m_Table[0]->m_Next->m_Friends[0]->m_Name == "Evelyn" );
	assert ( t.m_Table[0]->m_Next->m_Friends[1]->m_Name == "Bob" );
	assert ( t.m_Table[0]->m_Next->m_Next->m_Friends.size() == 3 );
	assert ( t.m_Table[0]->m_Next->m_Next->m_Friends[0]->m_Name == "Alice" );
	assert ( t.m_Table[0]->m_Next->m_Next->m_Friends[1]->m_Name == "Grace" );
	assert ( t.m_Table[0]->m_Next->m_Next->m_Friends[2]->m_Name == "Dave" );
	assert ( t.m_Table[1]->m_Friends.size() == 0 );
	assert ( t.m_Table[1]->m_Next->m_Friends.size() == 2 );
	assert ( t.m_Table[1]->m_Next->m_Friends[0]->m_Name == "Fred" );
	assert ( t.m_Table[1]->m_Next->m_Friends[1]->m_Name == "Bob" );
	assert ( t.m_Table[1]->m_Next->m_Next->m_Friends.size() == 2 );
	assert ( t.m_Table[1]->m_Next->m_Next->m_Friends[0]->m_Name == "Alice" );
	assert ( t.m_Table[1]->m_Next->m_Next->m_Friends[1]->m_Name == "Evelyn" );
	assert ( t.m_Table[1]->m_Next->m_Next->m_Next->m_Friends.size() == 2 );
	assert ( t.m_Table[1]->m_Next->m_Next->m_Next->m_Friends[0]->m_Name == "Bob" );
	assert ( t.m_Table[1]->m_Next->m_Next->m_Next->m_Friends[1]->m_Name == "Carol" );
	assert ( t.m_Table[3]->m_Friends.size() == 2 );
	assert ( t.m_Table[3]->m_Friends[0]->m_Name == "Dave" );
	assert ( t.m_Table[3]->m_Friends[1]->m_Name == "Carol" );

	assert ( t.m_Table[0]->m_Friends[0] == t.m_Table[1]->m_Next );
	assert ( t.m_Table[0]->m_Next->m_Friends[0] == t.m_Table[3] );
	assert ( t.m_Table[0]->m_Next->m_Friends[1] == t.m_Table[0]->m_Next->m_Next );
	assert ( t.m_Table[0]->m_Next->m_Next->m_Friends[0] == t.m_Table[1]->m_Next->m_Next->m_Next );
	assert ( t.m_Table[0]->m_Next->m_Next->m_Friends[1] == t.m_Table[1]->m_Next );
	assert ( t.m_Table[0]->m_Next->m_Next->m_Friends[2] == t.m_Table[0]->m_Next );
	assert ( t.m_Table[1]->m_Next->m_Friends[0] == t.m_Table[0] );
	assert ( t.m_Table[1]->m_Next->m_Friends[1] == t.m_Table[0]->m_Next->m_Next );
	assert ( t.m_Table[1]->m_Next->m_Next->m_Friends[0] == t.m_Table[1]->m_Next->m_Next->m_Next );
	assert ( t.m_Table[1]->m_Next->m_Next->m_Friends[1] == t.m_Table[3] );
	assert ( t.m_Table[1]->m_Next->m_Next->m_Next->m_Friends[0] == t.m_Table[0]->m_Next->m_Next );
	assert ( t.m_Table[1]->m_Next->m_Next->m_Next->m_Friends[1] == t.m_Table[1]->m_Next->m_Next );
	assert ( t.m_Table[3]->m_Friends[0] == t.m_Table[0]->m_Next );
	assert ( t.m_Table[3]->m_Friends[1] == t.m_Table[1]->m_Next->m_Next );
}

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

int main() {
	CNetwork * t0 = new CNetwork(4);

	assert ( t0->Ins("Alice") == true );
	assert ( t0->Ins("Bob") == true );
	assert ( t0->Ins("Carol") == true );
	assert ( t0->Ins("Dave") == true );
	assert ( t0->Ins("Evelyn") == true );
	assert ( t0->Ins("Fred") == true );
	assert ( t0->Ins("Grace") == true );
	assert ( t0->Ins("Helen") == true );
	assert ( t0->Ins("Alice") == false );

	assert ( t0->Friends("Alice", "Bob") == true );
	assert ( t0->Friends("Alice", "Carol") == true );
	assert ( t0->Friends("Evelyn", "Dave") == true );
	assert ( t0->Friends("Carol", "Evelyn") == true );
	assert ( t0->Friends("Grace", "Fred") == true );
	assert ( t0->Friends("Bob", "Grace") == true );
	assert ( t0->Friends("Bob", "Dave") == true );
	assert ( t0->Friends("Alice", "Incorrect name") == false );
	assert ( t0->Friends("Incorrect name", "Bob") == false );
	assert ( t0->Friends("Incorrect name", "Incorrect name") == false );

	t0->print();

	testNetwork(*t0);
	cout << "Base network test passed" << endl;

	CNetwork * t1 = new CNetwork(*t0);
	testNetwork(*t1);
	cout << "Copy constructor test passed" << endl;

	delete t0;
	testNetwork(*t1);
	cout << "Deep copy test passed" << endl;

	delete t1;
	cout << "Run with valgrind to check destructor" << endl;

	return EXIT_SUCCESS;
}
