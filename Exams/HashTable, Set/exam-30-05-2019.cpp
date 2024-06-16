#include <string>
#include <iostream>

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
 * @brief Class representing a hash table with insertion order preservation.
 */
class CHash {
public:
    /**
     * @brief Constructs a hash table with the given size.
     * @param size The size of the hash table.
     */
    explicit CHash(int size)
            : m_Size(size), m_Table(new TItem *[m_Size]), m_FirstOrder(nullptr), m_LastOrder(nullptr) {
        for (int table_idx = 0; table_idx < m_Size; table_idx++)
            m_Table[table_idx] = nullptr;
    }

    /**
     * @brief Destructor to clean up the hash table and its items.
     */
    ~CHash() {
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
     * @brief Inserts a new key-value pair into the hash table.
     * @param key The key of the item.
     * @param val The value of the item.
     * @return True if the item was inserted, false if the key already exists.
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

    /**
     * @brief Deletes an item from the hash table by key.
     * @param key The key of the item to delete.
     * @return True if the item was deleted, false if the key was not found.
     */
    bool Del(string key) { // todo
        unsigned int hash_idx = HashFn(key);

        for (TItem *prev_hash_item = nullptr, *item = m_Table[hash_idx]; item;
                prev_hash_item = item, item = item->m_NextHash)
            if (item->m_Key == key) {
                if (prev_hash_item)
                    prev_hash_item->m_NextHash = item->m_NextHash;
                else
                    m_Table[hash_idx] = item->m_NextHash;

                if (TItem *prev_ord_item = item->m_PrevOrder; prev_ord_item)
                    prev_ord_item->m_NextOrder = item->m_NextOrder;
                else
                    m_FirstOrder = item->m_NextOrder;

                if (TItem *next_ord_item = item->m_NextOrder; next_ord_item)
                    next_ord_item->m_PrevOrder = item->m_PrevOrder;
                else
                    m_LastOrder = item->m_PrevOrder;

                delete item;

                return true;
            }

        return false;
    }

    // -----------------------------------------------------------------------------------------------------------------

    /**
     * @brief Checks if a key is present in the hash table.
     * @param key The key to check.
     * @return True if the key is present, false otherwise.
     */
    bool IsSet(string key) const {
        for (auto item = m_Table[HashFn(key)]; item; item = item->m_NextHash)
            if (item->m_Key == key)
                return true;

        return false;
    }

    // -----------------------------------------------------------------------------------------------------------------

    /**
     * @brief Applies a function to each item in the hash table in insertion order.
     * @param f The function to apply.
     */
    template <typename func> void ForEach(func f) {
        cout << "-------------------------------------------" << endl;

        for (auto item = m_FirstOrder; item; item = item->m_NextOrder)
            f(item);
    }

    // -----------------------------------------------------------------------------------------------------------------

    /**
     * @brief Prints the contents of the hash table.
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
    unsigned int m_Size; // The size of the hash table
    TItem **m_Table, *m_FirstOrder, *m_LastOrder; // The hash table, first and last order pointers

    // -----------------------------------------------------------------------------------------------------------------

    /**
     * @brief Computes the hash index for a given string.
     * @param str The string to hash.
     * @return The computed hash index.
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

    assert(hashtable.Ins("h1", "car") == true);

    assert(hashtable.Ins("h1", "phone") == false);

    assert(hashtable.Ins("h2", "field") == true);

    assert(hashtable.Ins("h3", "house") == true);

    assert(hashtable.Ins("h4", "tree") == true);

    // -----------------------------------------------------------------------------------------------------------------

    hashtable.ForEach([](auto item) { cout << item->m_Key << " => " << item->m_Val << endl; });

    // hashtable.Print();

    // -----------------------------------------------------------------------------------------------------------------

    assert(hashtable.Del("h4") == true);

    // -----------------------------------------------------------------------------------------------------------------

    hashtable.ForEach([](auto item) { cout << item->m_Key << " => " << item->m_Val << endl; });

    // hashtable.Print();

    // -----------------------------------------------------------------------------------------------------------------

    return 0;
}
