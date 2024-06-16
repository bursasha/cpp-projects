#include <iostream>
#include <string>
#include <cassert>
#include <sstream>

// ---------------------------------------------------------------------------------------------------------------------

using namespace std;

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Class representing a binary search tree with additional ordering of nodes.
 */
class CTree {
public:
    CTree() = default;

    CTree(const CTree & src) = delete;

    CTree & operator=(const CTree & src) = delete;

    /**
     * @brief Destructor for CTree. Deletes all nodes in the tree.
     */
    ~CTree() { // TODO
        for (auto node = m_First; node; ) {
            auto next = node->m_NextOrder;
            delete node;
            node = next;
        }

        m_Root = m_First = m_Last = nullptr;
    }

    // -----------------------------------------------------------------------------------------------------------------

    /**
     * @brief Checks if a key is present in the tree.
     * @param key The key to check.
     * @return True if the key is found, false otherwise.
     */
    bool isSet(const string & key) { // TODO
        for (auto node = m_Root; node; ) {
            if (node->m_Key > key)
                node = node->m_L;
            else if (node->m_Key < key)
                node = node->m_R;
            else
                return true;
        }

        return false;
    }

    // -----------------------------------------------------------------------------------------------------------------

    /**
     * @brief Inserts a key-value pair into the tree.
     * @param key The key to insert.
     * @param val The value to associate with the key.
     * @return True if the insertion is successful, false if the key already exists.
     */
    bool insert(const string & key, const string & val) { // TODO
        auto prnt = m_Root;

        for (auto node = m_Root; node; ) {
            if (node->m_Key != key) {
                prnt = node;
                node = (node->m_Key > key) ? node = node->m_L : node = node->m_R;
            } else
                return false;
        }

        auto chld = new CNode(key, val);

        if (!prnt)
            m_Root = m_First = chld;
        else
            m_Last->m_NextOrder = (prnt->m_Key > key) ? prnt->m_L = chld : prnt->m_R = chld;

        m_Last = chld;

        return true;
    }

    // -----------------------------------------------------------------------------------------------------------------

    /**
     * @brief Outputs the tree in an ordered format.
     * @param os The output stream.
     * @param src The tree to output.
     * @return The output stream.
     */
    friend ostream & operator<<(ostream & os, const CTree & src) { // TODO
        os << "{";

        for (auto node = src.m_First; node; node = node->m_NextOrder) {
            if (node != src.m_First)
                os << ", ";

            os << node->m_Key << " => " << node->m_Val;
        }

        return os << "}";
    }

protected:
    /**
     * @brief Class representing a node in the binary search tree.
     */
    class CNode {
    public:
        /**
         * @brief Constructor for CNode.
         * @param key The key of the node.
         * @param val The value of the node.
         */
        CNode(const string & key, const string & val)
                : m_Key(key), m_Val(val) {}

        // -------------------------------------------------------------------------------------------------------------

        string m_Key, m_Val; // Key-value pair stored in the node
        CNode * m_L = nullptr, * m_R = nullptr, * m_NextOrder = nullptr; // Pointers to left, right, and next node in order
    };

    // -----------------------------------------------------------------------------------------------------------------

    CNode * m_Root = nullptr, * m_First = nullptr, * m_Last = nullptr; // Pointers to root, first, and last node in order

    // -----------------------------------------------------------------------------------------------------------------

    friend int main();
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

int main() {
    CTree t;
    stringstream ss;

    // -----------------------------------------------------------------------------------------------------------------

    ss << t;

    assert(ss.str() == "{}");

    ss.clear();
    ss.str("");

    // -----------------------------------------------------------------------------------------------------------------

    assert(t.insert("PA1", "done"));

    assert(t.m_First == t.m_Last);
    assert(t.m_First->m_Key == "PA1");

    assert(!t.isSet("UOS"));

    assert(t.insert("PA2", "fail"));
    assert(t.insert("UOS", "funny"));

    // -----------------------------------------------------------------------------------------------------------------

    ss << t;

    assert(ss.str() == "{PA1 => done, PA2 => fail, UOS => funny}");

    ss.clear();
    ss.str("");

    // -----------------------------------------------------------------------------------------------------------------

    assert(t.m_Root->m_L== nullptr);
    assert(t.m_Last->m_Key == "UOS");
    assert(t.m_Root->m_R->m_Key == "PA2");
    assert(t.m_Root->m_R->m_L == nullptr);
    assert(t.m_Root->m_R->m_R->m_Key == "UOS");
    assert(t.m_First->m_NextOrder->m_NextOrder == t.m_Last);

    assert(t.isSet("PA2"));

    // -----------------------------------------------------------------------------------------------------------------

    assert(t.insert("CAO", "lul"));
    assert(t.insert("LIN", "F"));

    assert(t.m_Root->m_L->m_Key == "CAO");
    assert(t.m_Root->m_L->m_L == nullptr);
    assert(t.m_Root->m_L->m_R->m_Key == "LIN");
    assert(t.m_Last == t.m_Root->m_L->m_R );
    assert(t.m_Root->m_L->m_R->m_L == nullptr);
    assert(t.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder == t.m_Root->m_L->m_R);

    assert(t.isSet("LIN"));

    // -----------------------------------------------------------------------------------------------------------------

    ss << t;

    assert(ss.str() == "{PA1 => done, PA2 => fail, UOS => funny, CAO => lul, LIN => F}");

    ss.clear();
    ss.str("");

    // -----------------------------------------------------------------------------------------------------------------

    assert(t.insert("SAP", "shit"));

    assert(t.m_Root->m_R->m_R->m_L->m_Key == "SAP");
    assert(t.m_Last == t.m_Root->m_R->m_R->m_L);

    // -----------------------------------------------------------------------------------------------------------------

    ss << t;

    assert(ss.str() == "{PA1 => done, PA2 => fail, UOS => funny, CAO => lul, LIN => F, SAP => shit}");

    ss.clear();
    ss.str("");

    // -----------------------------------------------------------------------------------------------------------------

    assert(!t.isSet("PA3"));
    assert(t.isSet("LIN"));
    assert(t.isSet("SAP"));

    // -----------------------------------------------------------------------------------------------------------------

    return 0;
}
