#include <iostream>
#include <cassert>
#include <functional>
#include <sstream>

// ---------------------------------------------------------------------------------------------------------------------

using namespace std;

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief A hash table with ordered iteration capability.
 */
class CHash {
public:
    /**
     * @brief Constructor to initialize the hash table with a given size.
     * @param size The size of the hash table.
     */
    CHash(size_t size)
            : m_Size(size), m_Table(new TItem *[m_Size]), m_FirstOrder(nullptr), m_LastOrder(nullptr) { // todo
        for (size_t idx = 0; idx < m_Size; ++idx)
            m_Table[idx] = nullptr;
    }

    /**
     * @brief Copy constructor to create a deep copy of an existing hash table.
     * @param src The source hash table to copy from.
     */
    CHash(const CHash &src)
            : m_Size(src.m_Size), m_Table(new TItem *[m_Size]), m_FirstOrder(nullptr), m_LastOrder(nullptr) { // todo
        for (size_t idx = 0; idx < m_Size; ++idx)
            m_Table[idx] = nullptr;

        for (auto src_item = src.m_FirstOrder; src_item; src_item = src_item->m_NextOrder)
            Ins(src_item->m_Key, src_item->m_Val);
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

    /**
     * @brief Assignment operator to copy the content of an existing hash table to another.
     * @param src The source hash table to copy from.
     * @return A reference to the current hash table.
     */
    CHash &operator=(const CHash &src) { // todo
        if (this != &src) {
            for (auto item = m_FirstOrder; item;) {
                auto next_item = item->m_NextOrder;
                delete item;
                item = next_item;
            }

            m_Size = src.m_Size;

            delete[] m_Table;
            m_Table = new TItem *[m_Size];

            m_FirstOrder = m_LastOrder = nullptr;

            for (size_t idx = 0; idx < m_Size; ++idx)
                m_Table[idx] = nullptr;

            for (auto src_item = src.m_FirstOrder; src_item; src_item = src_item->m_NextOrder)
                Ins(src_item->m_Key, src_item->m_Val);
        }

        return *this;
    }

    // -----------------------------------------------------------------------------------------------------------------

    /**
     * @brief Inserts a key-value pair into the hash table.
     * @param key The key to insert.
     * @param val The value associated with the key.
     * @return True if the key-value pair is inserted successfully, false if the key already exists.
     */
    bool Ins(const string &key, const string &val) { // todo
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

        for (TItem *prev_hash_item = nullptr, *item = m_Table[idx]; item;
                prev_hash_item = item, item = item->m_NextHash)
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
    bool IsSet(const string &key) const { // todo
        for (auto item = m_Table[HashFn(key, m_Size)]; item; item = item->m_NextHash)
            if (item->m_Key == key)
                return true;

        return false;
    }

    // -----------------------------------------------------------------------------------------------------------------

    /**
     * @brief Applies a function to each key-value pair in the hash table in insertion order.
     * @param function The function to apply.
     */
    template<typename F> void ForEach(F function) const { // todo
        for (auto item = m_FirstOrder; item; item = item->m_NextOrder)
            function(item);
    }

private:
    /**
     * @brief Structure representing an item in the hash table.
     */
    struct TItem {
        TItem(const string &key, const string &val)
                : m_Key(key), m_Val(val), m_NextHash(nullptr), m_NextOrder(nullptr), m_PrevOrder(nullptr) {}

        ~TItem() = default;

        // -------------------------------------------------------------------------------------------------------------

        string m_Key, m_Val; // Key-value pair stored in the item
        TItem *m_NextHash, *m_NextOrder, *m_PrevOrder; // Pointers for hash table chaining and ordered iteration
    };

    // -----------------------------------------------------------------------------------------------------------------

    size_t m_Size; // Size of the hash table
    TItem **m_Table, *m_FirstOrder, *m_LastOrder; // Pointer to the array of hash table buckets, Pointers for ordered iteration

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

        for (string::size_type c : x)
            h += (h << 5) + c;

        return h % modulus;
    }
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

int main() {
  CHash x ( 100 );

  assert ( sizeof ( CHash ) == sizeof ( CHash::TItem * ) * 3 + sizeof ( size_t ) );

  assert ( sizeof ( CHash::TItem ) == sizeof ( CHash::TItem * ) * 3 + sizeof ( string ) * 2 );

  assert ( x . Ins ( "peg", "foo" )
           && x . m_Table[1] -> m_Key == "peg"
           && x . m_Table[1] -> m_Val == "foo"
           && x . m_FirstOrder == x . m_Table[1]
           && x . m_FirstOrder -> m_NextOrder == nullptr
           && x . m_LastOrder == x . m_Table[1]
           && x . m_LastOrder -> m_PrevOrder == nullptr );

  assert ( x . Ins ( "bwc", "bar" )
           && x . m_Table[45] -> m_Key == "bwc"
           && x . m_Table[45] -> m_Val == "bar"
           && x . m_FirstOrder == x . m_Table[1]
           && x . m_FirstOrder -> m_NextOrder == x . m_Table[45]
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder == nullptr
           && x . m_LastOrder == x . m_Table[45]
           && x . m_LastOrder -> m_PrevOrder == x . m_Table[1]
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder == nullptr );

  assert ( x . Ins ( "jdr", "test" )
           && x . m_Table[45] -> m_Key == "jdr"
           && x . m_Table[45] -> m_Val == "test"
           && x . m_Table[45] -> m_NextHash -> m_Key == "bwc"
           && x . m_Table[45] -> m_NextHash -> m_Val == "bar"
           && x . m_FirstOrder == x . m_Table[1]
           && x . m_FirstOrder -> m_NextOrder == x . m_Table[45] -> m_NextHash
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder == x . m_Table[45]
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == nullptr
           && x . m_LastOrder == x . m_Table[45]
           && x . m_LastOrder -> m_PrevOrder == x . m_Table[45] -> m_NextHash
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[1]
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == nullptr );

  assert ( x . Ins ( "peg", "something" ) == false );

  assert ( x . Ins ( "peg", "foo" ) == false );

  assert ( x . Ins ( "bwc", "test" ) == false );

  assert ( x . Ins ( "jdr", "mudr" ) == false );

  assert ( x . Ins ( "asa", "butterfield" )
           && x . m_Table[22] -> m_Key == "asa"
           && x . m_Table[22] -> m_Val == "butterfield"
           && x . m_FirstOrder == x . m_Table[1]
           && x . m_FirstOrder -> m_NextOrder == x . m_Table[45] -> m_NextHash
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder == x . m_Table[45]
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == x . m_Table[22]
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == nullptr
           && x . m_LastOrder == x . m_Table[22]
           && x . m_LastOrder -> m_PrevOrder == x . m_Table[45]
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[45] -> m_NextHash
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[1]
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == nullptr );

  assert ( x . IsSet ( "peg" ) );

  assert ( x . IsSet ( "asa" ) );

  assert ( x . IsSet ( "bwc" ) );

  assert ( x . IsSet ( "jdr" ) );

  assert ( x . IsSet ( "whf" ) == false );

  assert ( x . IsSet ( "xns" ) == false );

  assert ( x . IsSet ( "abc" ) == false );

  assert ( x . IsSet ( "ooj" ) == false );

  ostringstream oss;

  x . ForEach ( [&oss] ( auto item ) { oss << item->m_Val << endl; } );

  assert ( oss . str () == "foo\nbar\ntest\nbutterfield\n" );

  CHash n ( x );

  assert ( n . m_Table[1] -> m_Key == "peg"
           && n . m_Table[1] -> m_Val == "foo"
           && n . m_Table[45] -> m_Key == "jdr"
           && n . m_Table[45] -> m_Val == "test"
           && n . m_Table[45] -> m_NextHash -> m_Key == "bwc"
           && n . m_Table[45] -> m_NextHash -> m_Val == "bar"
           && n . m_Table[22] -> m_Key == "asa"
           && n . m_Table[22] -> m_Val == "butterfield"
           && n . m_FirstOrder == n . m_Table[1]
           && n . m_FirstOrder -> m_NextOrder == n . m_Table[45] -> m_NextHash
           && n . m_FirstOrder -> m_NextOrder -> m_NextOrder == n . m_Table[45]
           && n . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == n . m_Table[22]
           && n . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == nullptr
           && n . m_LastOrder == n . m_Table[22]
           && n . m_LastOrder -> m_PrevOrder == n . m_Table[45]
           && n . m_LastOrder -> m_PrevOrder -> m_PrevOrder == n . m_Table[45] -> m_NextHash
           && n . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == n . m_Table[1]
           && n . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == nullptr );

  assert ( n . Del ( "jdr" )
           && n . m_Table[45] -> m_Key == "bwc"
           && n . m_Table[45] -> m_Val == "bar"
           && n . m_FirstOrder == n . m_Table[1]
           && n . m_FirstOrder -> m_NextOrder == n . m_Table[45]
           && n . m_FirstOrder -> m_NextOrder -> m_NextOrder == n . m_Table[22]
           && n . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == nullptr
           && n . m_LastOrder == n . m_Table[22]
           && n . m_LastOrder -> m_PrevOrder == n . m_Table[45]
           && n . m_LastOrder -> m_PrevOrder -> m_PrevOrder == n . m_Table[1]
           && n . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == nullptr );

  assert ( n . IsSet ( "jdr" ) == false );

  assert ( x . m_Table[1] -> m_Key == "peg"
           && x . m_Table[1] -> m_Val == "foo"
           && x . m_Table[45] -> m_Key == "jdr"
           && x . m_Table[45] -> m_Val == "test"
           && x . m_Table[45] -> m_NextHash -> m_Key == "bwc"
           && x . m_Table[45] -> m_NextHash -> m_Val == "bar"
           && x . m_Table[22] -> m_Key == "asa"
           && x . m_Table[22] -> m_Val == "butterfield"
           && x . m_FirstOrder == x . m_Table[1]
           && x . m_FirstOrder -> m_NextOrder == x . m_Table[45] -> m_NextHash
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder == x . m_Table[45]
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == x . m_Table[22]
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == nullptr
           && x . m_LastOrder == x . m_Table[22]
           && x . m_LastOrder -> m_PrevOrder == x . m_Table[45]
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[45] -> m_NextHash
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[1]
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == nullptr );

  assert ( n . Del ( "404" ) == false );

  assert ( n . Del ( "system32" ) == false );

  assert ( n . Del ( "root" ) == false );

  n = n;

  assert ( n . Ins ( "xns", "test" )
           && n . m_Table[22] -> m_Key == "xns"
           && n . m_Table[22] -> m_Val == "test"
           && n . m_Table[22] -> m_NextHash -> m_Key == "asa"
           && n . m_Table[22] -> m_NextHash -> m_Val == "butterfield"
           && n . m_FirstOrder == n . m_Table[1]
           && n . m_FirstOrder -> m_NextOrder == n . m_Table[45]
           && n . m_FirstOrder -> m_NextOrder -> m_NextOrder == n . m_Table[22] -> m_NextHash
           && n . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == n . m_Table[22]
           && n . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == nullptr
           && n . m_LastOrder == n . m_Table[22]
           && n . m_LastOrder -> m_PrevOrder == n . m_Table[22] -> m_NextHash
           && n . m_LastOrder -> m_PrevOrder -> m_PrevOrder == n . m_Table[45]
           && n . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == n . m_Table[1]
           && n . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == nullptr );

  x = n;

  assert ( x . m_Table[22] -> m_Key == "xns"
           && x . m_Table[22] -> m_Val == "test"
           && x . m_Table[22] -> m_NextHash -> m_Key == "asa"
           && x . m_Table[22] -> m_NextHash -> m_Val == "butterfield"
           && x . m_FirstOrder == x . m_Table[1]
           && x . m_FirstOrder -> m_NextOrder == x . m_Table[45]
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder == x . m_Table[22] -> m_NextHash
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == x . m_Table[22]
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == nullptr
           && x . m_LastOrder == x . m_Table[22]
           && x . m_LastOrder -> m_PrevOrder == x . m_Table[22] -> m_NextHash
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[45]
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[1]
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == nullptr );

  assert ( x . Ins ( "whf", "FIT" )
           && x . m_Table[22] -> m_Key == "whf"
           && x . m_Table[22] -> m_Val == "FIT"
           && x . m_Table[22] -> m_NextHash -> m_Key == "xns"
           && x . m_Table[22] -> m_NextHash -> m_Val == "test"
           && x . m_Table[22] -> m_NextHash -> m_NextHash -> m_Key == "asa"
           && x . m_Table[22] -> m_NextHash -> m_NextHash -> m_Val == "butterfield"
           && x . m_FirstOrder == x . m_Table[1]
           && x . m_FirstOrder -> m_NextOrder == x . m_Table[45]
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder == x . m_Table[22] -> m_NextHash -> m_NextHash
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == x . m_Table[22] -> m_NextHash
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == x . m_Table[22]
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == nullptr
           && x . m_LastOrder == x . m_Table[22]
           && x . m_LastOrder -> m_PrevOrder == x . m_Table[22] -> m_NextHash
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[22] -> m_NextHash -> m_NextHash
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[45]
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[1]
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == nullptr );

  assert ( n . m_Table[22] -> m_Key == "xns"
           && n . m_Table[22] -> m_Val == "test"
           && n . m_Table[22] -> m_NextHash -> m_Key == "asa"
           && n . m_Table[22] -> m_NextHash -> m_Val == "butterfield"
           && n . m_FirstOrder == n . m_Table[1]
           && n . m_FirstOrder -> m_NextOrder == n . m_Table[45]
           && n . m_FirstOrder -> m_NextOrder -> m_NextOrder == n . m_Table[22] -> m_NextHash
           && n . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == n . m_Table[22]
           && n . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == nullptr
           && n . m_LastOrder == n . m_Table[22]
           && n . m_LastOrder -> m_PrevOrder == n . m_Table[22] -> m_NextHash
           && n . m_LastOrder -> m_PrevOrder -> m_PrevOrder == n . m_Table[45]
           && n . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == n . m_Table[1]
           && n . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == nullptr );

  assert ( x . Del ( "xns" )
           && x . m_Table[22] -> m_Key == "whf"
           && x . m_Table[22] -> m_Val == "FIT"
           && x . m_Table[22] -> m_NextHash -> m_Key == "asa"
           && x . m_Table[22] -> m_NextHash -> m_Val == "butterfield"
           && x . m_FirstOrder == x . m_Table[1]
           && x . m_FirstOrder -> m_NextOrder == x . m_Table[45]
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder == x . m_Table[22] -> m_NextHash
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == x . m_Table[22]
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == nullptr
           && x . m_LastOrder == x . m_Table[22]
           && x . m_LastOrder -> m_PrevOrder == x . m_Table[22] -> m_NextHash
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[45]
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[1]
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == nullptr );

  assert ( x . Del ( "whf" )
           && x . m_Table[22] -> m_Key == "asa"
           && x . m_Table[22] -> m_Val == "butterfield"
           && x . m_FirstOrder == x . m_Table[1]
           && x . m_FirstOrder -> m_NextOrder == x . m_Table[45]
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder == x . m_Table[22]
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == nullptr
           && x . m_LastOrder == x . m_Table[22]
           && x . m_LastOrder -> m_PrevOrder == x . m_Table[45]
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[1]
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == nullptr );

  assert ( x . IsSet ( "asa" ) );

  assert ( x . IsSet ( "xns" ) == false );

  assert ( x . IsSet ( "whf" ) == false );

  assert ( x . Del ( "asa" )
           && x . m_Table[22] == nullptr );

  assert ( x . Del ( "peg" )
           && x . m_Table[1] == nullptr );

  assert ( x . Del ( "bwc" )
           && x . m_Table[45] == nullptr );

  assert ( x . m_FirstOrder == nullptr
           && x . m_LastOrder == nullptr );

  return 0;
}
