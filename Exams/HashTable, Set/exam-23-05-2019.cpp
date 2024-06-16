#include <string>
#include <iostream>
#include <utility>

// ---------------------------------------------------------------------------------------------------------------------

using namespace std;

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Structure representing an item in the hash table.
 */
struct TItem {
    TItem(string key, string val, TItem *nextHash, TItem *nextOrd, TItem *prevOrd)
            : m_Key(std::move(key)), m_Val(std::move(val)),
              m_NextHash(nextHash), m_NextOrder(nextOrd), m_PrevOrder(prevOrd) {}

    string m_Key, m_Val;
    TItem *m_NextHash, *m_NextOrder, *m_PrevOrder;
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Class representing a hash table with insertion order tracking.
 */
class CHash {
public:
    /**
     * @brief Constructs a new hash table with the given size.
     * @param size The size of the hash table.
     */
    explicit CHash(int size)
            : m_Size(size), m_Table(new TItem *[m_Size]), m_FirstOrder(nullptr), m_LastOrder(nullptr) {
        for (int table_idx = 0; table_idx < m_Size; table_idx++)
            m_Table[table_idx] = nullptr;
    }

    /**
     * @brief Destructor to clean up the hash table and its elements.
     */
    ~CHash() { // todo
        for (auto item = m_FirstOrder; item;) {
            auto next_item = item->m_NextOrder;
            delete item;
            item = next_item;
        }

        m_Size = 0;

        delete[] m_Table;
        m_Table = nullptr;

        m_FirstOrder = m_LastOrder = nullptr;
    }

    // -----------------------------------------------------------------------------------------------------------------

    CHash(CHash &) = delete;

    CHash &operator=(CHash &) = delete;

    // -----------------------------------------------------------------------------------------------------------------

    /**
     * @brief Inserts a key-value pair into the hash table.
     * @param key The key to insert.
     * @param val The value associated with the key.
     * @return True if the key-value pair is inserted successfully, false if the key already exists.
     */
    bool Ins(string key, string val) { // todo
        if (IsSet(key))
            return false;

        unsigned int hash_idx = HashFn(key);
        auto new_item = new TItem(std::move(key), std::move(val),
                                  m_Table[hash_idx], nullptr, m_LastOrder);

        m_Table[hash_idx] = new_item;

        if (!m_FirstOrder)
            m_FirstOrder = new_item;

        if (m_LastOrder)
            m_LastOrder->m_NextOrder = new_item;

        m_LastOrder = new_item;

        return true;
    }

    // -----------------------------------------------------------------------------------------------------------------

    /**
     * @brief Checks if a key is present in the hash table.
     * @param key The key to check.
     * @return True if the key is found, false otherwise.
     */
    bool IsSet(string key) const { // todo
        for (auto item = m_Table[HashFn(key)]; item; item = item->m_NextHash)
            if (item->m_Key == key)
                return true;

        return false;
    }

    // -----------------------------------------------------------------------------------------------------------------

    /**
     * @brief Applies a function to each key-value pair in the hash table in insertion order.
     * @param f The function to apply.
     */
    template <typename func> void ForEach(func f) {
        for (auto item = m_FirstOrder; item; item = item->m_NextOrder)
            f(item);
    }

    // -----------------------------------------------------------------------------------------------------------------

    /**
     * @brief Prints the current state of the hash table.
     */
    void Print() const {
        cout << "-------------------------------------------" << endl;

        for (int table_idx = 0; table_idx < m_Size; table_idx++) {
            cout << "HashTable[" << table_idx << "]: ";

            for (auto item = m_Table[table_idx]; item; item = item->m_NextHash)
                cout << "{" << item->m_Key << ", " << item->m_Val << "} -> ";

            cout << "NULL." << endl;
        }
    }

private:
    unsigned int m_Size;
    TItem **m_Table, *m_FirstOrder, *m_LastOrder;

    // -----------------------------------------------------------------------------------------------------------------

    /**
     * @brief Hash function to compute the index for a given key.
     * @param str The key to hash.
     * @return The computed index.
     */
    unsigned int HashFn(const string &str) const {
        std::hash<std::string> hasher;

        return hasher(str) % m_Size;
    }
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

int main(int argc, char **argv) {
    CHash hashtable(100);

    // -----------------------------------------------------------------------------------------------------------------

    hashtable.Ins("h1", "car");

    hashtable.Ins("h1", "phone");

    hashtable.Ins("h2", "field");

    hashtable.Ins("h3", "house");

    hashtable.Ins("h4", "tree");

    // -----------------------------------------------------------------------------------------------------------------

    hashtable.ForEach([](auto item) { cout << item->m_Key << " => " << item->m_Val << endl; });

    // hashtable.Print();

    // -----------------------------------------------------------------------------------------------------------------

    return 0;
}