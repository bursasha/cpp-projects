#include <iostream>
#include <cassert>
#include <functional>
#include <sstream>

// ---------------------------------------------------------------------------------------------------------------------

using namespace std;

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

template<typename T> class CHash {
public:
    /**
     * @brief Constructs a new hash table with the given size.
     * @param size The size of the hash table.
     */
    CHash(size_t size)
            : m_Size(size), m_Table(new TItem *[m_Size]), m_FirstOrder(nullptr), m_LastOrder(nullptr) {
        for (size_t idx = 0; idx < m_Size; ++idx)
            m_Table[idx] = nullptr;
    }

    CHash(const CHash &src) = delete;

    /**
     * @brief Destructor to clean up the hash table and its elements.
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

    CHash &operator=(const CHash &src) = delete;

    // -----------------------------------------------------------------------------------------------------------------

    /**
     * @brief Inserts a key-value pair into the hash table.
     * @param key The key to insert.
     * @param val The value associated with the key.
     * @return True if the key-value pair is inserted successfully, false if the key already exists.
     */
    bool Ins(const string &key, const string &val) {
        if (IsSet(key))
            return false;

        size_t idx = HashFn(key, m_Size);
        auto new_item = new TItem(key, val);

        new_item->m_NextHash = m_Table[idx];
        m_Table[idx] = new_item;

        if (!m_FirstOrder)
            m_FirstOrder = new_item;

        if (m_LastOrder)
            m_LastOrder->m_NextOrder = new_item;

        new_item->m_PrevOrder = m_LastOrder;
        m_LastOrder = new_item;

        return true;
    }

    /**
     * @brief Deletes a key-value pair from the hash table.
     * @param key The key to delete.
     * @return True if the key-value pair is deleted successfully, false if the key is not found.
     */
    bool Del(const string &key) { // todo
        size_t idx = HashFn(key, m_Size);

        for (TItem *prev_hash_item = nullptr, *item = m_Table[idx]; item; prev_hash_item = item, item = item->m_NextHash)
            if (item->m_Key == key) {
                if (prev_hash_item)
                    prev_hash_item->m_NextHash = item->m_NextHash;
                else
                    m_Table[idx] = item->m_NextHash;

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
     * @return True if the key is found, false otherwise.
     */
    bool IsSet(const string &key) const {
        for (auto item = m_Table[HashFn(key, m_Size)]; item; item = item->m_NextHash)
            if (item->m_Key == key)
                return true;

        return false;
    }

    // -----------------------------------------------------------------------------------------------------------------

    /**
     * @brief Rehashes the hash table to a new size.
     * @param resize The new size of the hash table.
     */
    void Rehash(size_t resize) { // todo
        auto **new_table = new TItem *[resize];

        for (size_t idx = 0; idx < resize; ++idx)
            new_table[idx] = nullptr;

        for (auto item = m_FirstOrder; item; item = item->m_NextOrder) {
            size_t reidx = HashFn(item->m_Key, resize);

            item->m_NextHash = new_table[reidx];
            new_table[reidx] = item;
        }

        m_Size = resize;

        delete[] m_Table;
        m_Table = new_table;
    }

    // -----------------------------------------------------------------------------------------------------------------

    /**
     * @brief Applies a function to each key-value pair in the hash table in insertion order.
     * @param function The function to apply.
     */
    template<typename F> void ForEach(F function) const {
        for (auto item = m_FirstOrder; item; item = item->m_NextOrder)
            function(item);
    }

private:
    /**
     * @brief Structure representing an item in the hash table.
     */
    struct TItem {
        TItem(const string &key, const T &val)
            : m_Key(key), m_Val(val), m_NextHash(nullptr), m_NextOrder(nullptr), m_PrevOrder(nullptr) {}

        // -------------------------------------------------------------------------------------------------------------

        string m_Key;
        T m_Val;
        TItem *m_NextHash, *m_NextOrder, *m_PrevOrder;
    };

    // -----------------------------------------------------------------------------------------------------------------

    size_t m_Size;
    TItem **m_Table, *m_FirstOrder, *m_LastOrder;

    // -----------------------------------------------------------------------------------------------------------------

    friend int main();

    // -----------------------------------------------------------------------------------------------------------------

    /**
     * @brief Hash function to compute the index for a given key.
     * @param x The key to hash.
     * @param modulus The modulus value to use for computing the index.
     * @return The computed index.
     */
    static size_t HashFn(const string &x, size_t modulus) {
        size_t h = 5381;

        for (char c : x)
            h += (h << 5) + c;

        return h % modulus;
    }
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

int main() {
    CHash<string> x(100);

    assert(sizeof(CHash<string>) == sizeof(CHash<string>::TItem *) * 3 + sizeof(size_t));
    assert(sizeof(CHash<string>::TItem) == sizeof(CHash<string>::TItem *) * 3 + sizeof(string) * 2);

    // Insertion tests
    assert(x.Ins("peg", "foo"));
    assert(x.Ins("bwc", "bar"));
    assert(x.Ins("jdr", "test"));
    assert(x.Ins("asa", "butterfield"));
    assert(x.Ins("xyz", "baz"));
    assert(x.Ins("abc", "def"));

    // Deletion tests
    assert(x.Del("peg"));
    assert(x.IsSet("peg") == false);
    assert(x.IsSet("bwc"));
    assert(x.Del("asa"));
    assert(x.IsSet("asa") == false);
    assert(x.IsSet("jdr"));
    assert(x.IsSet("bwc"));

    // Attempt to delete non-existing keys
    assert(x.Del("nonexist") == false);
    assert(x.Del("another") == false);

    // Deleting the first and last elements
    assert(x.Del("bwc"));
    assert(x.IsSet("bwc") == false);
    assert(x.Del("abc"));
    assert(x.IsSet("abc") == false);

    // Rehash tests
    x.Rehash(50);
    assert(x.IsSet("xyz"));
    assert(x.IsSet("jdr"));
    assert(x.IsSet("peg") == false);
    assert(x.IsSet("asa") == false);
    assert(x.IsSet("bwc") == false);
    assert(x.IsSet("abc") == false);

    // Additional Rehash tests
    x.Rehash(200);
    assert(x.IsSet("xyz"));
    assert(x.IsSet("jdr"));

    // Inserting more elements after rehash
    assert(x.Ins("new1", "val1"));
    assert(x.Ins("new2", "val2"));
    assert(x.IsSet("new1"));
    assert(x.IsSet("new2"));

    // Deleting elements after rehash
    assert(x.Del("new1"));
    assert(x.IsSet("new1") == false);
    assert(x.IsSet("new2"));

    // Final state checks
    ostringstream oss;
    x.ForEach([&oss](auto item) { oss << item->m_Val << endl; });
    assert(oss.str() == "test\nbaz\nval2\n");

    return 0;
}
