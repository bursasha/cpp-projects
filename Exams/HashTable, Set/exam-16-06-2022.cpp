/**
 * Implementujte metodu insert (vkládání),
 * erase (mazání prvků ze stromu a zároveň je potřeba uchovat správné pořádí podle data vkládání)
 * a vlastní destruktor, ostatní metody jsou naimplentované.
 * Kopírující konstruktor i operátor přiřazení zakázán.
 * Není povolené si přidat žádnou členskou proměnnou, ale lze naimplementovat vlastní pomocné metody.
 *
 * Za správnost šablony neručím :-).
 */

#include <string>
#include <cassert>
#include <iomanip>
#include <iostream>

// ---------------------------------------------------------------------------------------------------------------------

using namespace std;

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief A class representing a binary search tree with additional linked list for order of insertion.
 */
class CTree {
public:
    CTree() = default;

    CTree(const CTree & src) = delete;

    CTree & operator=(const CTree & src) = delete;

    /**
     * @brief Destructor to clean up the tree and its nodes.
     */
    ~CTree() { // todo
        for (auto node = m_First; node; ) {
            auto next = node->m_NextOrder;
            delete node;
            node = next;
        }

        m_Root = m_First = m_Last = nullptr;
    }

    // -----------------------------------------------------------------------------------------------------------------

    /**
     * @brief Prints the tree in-order.
     */
    void print() const {
        cout << "-------------------------------------" << endl;
        printHelper(m_Root);
        cout << endl;
    }

    // -----------------------------------------------------------------------------------------------------------------

    /**
     * @brief Checks if a key is present in the tree.
     * @param key The key to check.
     * @return True if the key is present, false otherwise.
     */
    bool isSet(string key) {
        for (auto node = m_Root; node; )
            if (node->m_Key > key) node = node->m_L;
            else if (node->m_Key < key) node = node->m_R;
            else return true;

        return false;
    }

    // -----------------------------------------------------------------------------------------------------------------

    /**
     * @brief Inserts a key into the tree.
     * @param key The key to insert.
     * @return True if the key is inserted successfully, false if the key already exists.
     */
    bool insert(string key) { // todo
        CNode * parent = nullptr, * node = m_Root;

        while (node)
            if (node->m_Key != key) {
                parent = node;
                node = (node->m_Key > key) ? node->m_L : node->m_R;
            } else return false;

        node = new CNode(key);

        if (!parent) m_Root = m_First = node;
        else {
            m_Last->m_NextOrder = (parent->m_Key > key) ? parent->m_L = node : parent->m_R = node;
            node->m_PrevOrder = m_Last;
        }

        m_Last = node;

        return true;
    }

    /**
     * @brief Erases a key from the tree.
     * @param key The key to erase.
     * @return True if the key is erased successfully, false if the key is not found.
     */
    bool erase(string key) { // todo
        CNode * parent = nullptr, * node = m_Root, * parent_rmin = nullptr, * rmin = nullptr;

        while (node)
            if (node->m_Key != key) {
                parent = node;
                node = (node->m_Key > key) ? node->m_L : node->m_R;
            } else break;

        if (!node) return false;

        for (parent_rmin = node, rmin = node->m_R; rmin; )
            if (rmin->m_L) {
                parent_rmin = rmin;
                rmin = rmin->m_L;
            } else break;

        if (rmin) {
            if (rmin->m_R)
                (parent_rmin->m_Key > rmin->m_R->m_Key) ? parent_rmin->m_L = rmin->m_R : parent_rmin->m_R = rmin->m_R;
            else
                (parent_rmin->m_Key > rmin->m_Key) ? parent_rmin->m_L = nullptr : parent_rmin->m_R = nullptr;

            if (parent)
                (parent->m_Key > rmin->m_Key) ? parent->m_L = rmin : parent->m_R = rmin;
            else m_Root = rmin;

            rmin->m_L = node->m_L;
            rmin->m_R = node->m_R;
        } else {
            if (parent) {
                if (node->m_L)
                    (parent->m_Key > node->m_L->m_Key) ? parent->m_L = node->m_L : parent->m_R = node->m_L;
                else
                    (parent->m_Key > node->m_Key) ? parent->m_L = nullptr : parent->m_R = nullptr;
            } else m_Root = node->m_L;
        }

        if (auto previous = node->m_PrevOrder; previous) previous->m_NextOrder = node->m_NextOrder;
        else m_First = node->m_NextOrder;

        if (auto next = node->m_NextOrder; next) next->m_PrevOrder = node->m_PrevOrder;
        else m_Last = node->m_PrevOrder;

        delete node;

        return true;
    }

protected:
    class CNode {
    public:
        CNode(string key) : m_Key(std::move(key)) {}

        // -------------------------------------------------------------------------------------------------------------

        string m_Key;
        CNode * m_L = nullptr, * m_R = nullptr, * m_PrevOrder = nullptr, * m_NextOrder = nullptr;
    };

    // -----------------------------------------------------------------------------------------------------------------

    CNode * m_Root = nullptr, * m_First = nullptr, * m_Last = nullptr;

    // -----------------------------------------------------------------------------------------------------------------

    /**
     * @brief Helper function to print the tree in-order.
     * @param node The node to start printing from.
     */
    void printHelper(CNode * node) const {
        if (node == nullptr) return;

        printHelper(node->m_L);

        cout << " " << node->m_Key;

        printHelper(node->m_R);
    }
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief A test class for CTree to verify the correctness of its operations.
 */
class CTester : public CTree {
  public:
    CTester() = default;

    void test() {
      CTester t0;

      assert(t0.insert("PA1")==true);
      assert(t0.m_First->m_Key=="PA1" && t0.m_First->m_NextOrder == nullptr);
      assert(t0.isSet("PA1")==true);
      t0.print();

      assert(t0.insert("UOS")==true);
        t0.print();
      assert(t0.insert("PA2")==true);
        t0.print();
      assert(t0.isSet("PA2")==true);
      assert(t0.isSet("PA3")==false);

      assert(t0.insert("PA2")==false);
      assert(t0.insert("CAO")==true);
        t0.print();
      assert(t0.insert("LIN")==true);
        t0.print();
      assert(t0.insert("AAG")==true);
        t0.print();
      assert(t0.insert("AG1")==true);
        t0.print();
      assert(t0.insert("ZDM")==true);
        t0.print();

      assert(t0.m_First->m_Key == "PA1"
             && t0.m_First->m_NextOrder->m_Key == "UOS"
             && t0.m_First->m_NextOrder->m_NextOrder->m_Key == "PA2"
             && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "CAO"
             && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "LIN"
             && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "AAG"
             && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "AG1"
             && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "ZDM");

      assert(t0.m_Last->m_Key == "ZDM"
             && t0.m_Last->m_PrevOrder->m_Key == "AG1"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_Key == "AAG"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "LIN"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "CAO"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA2"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "UOS"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA1"
             );

      assert(t0.erase("")==false);
      assert(t0.erase("ZDM")==true);
        t0.print();

      assert(t0.m_First->m_Key == "PA1"
             && t0.m_First->m_NextOrder->m_Key == "UOS"
             && t0.m_First->m_NextOrder->m_NextOrder->m_Key == "PA2"
             && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "CAO"
             && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "LIN"
             && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "AAG"
             && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "AG1");
      assert(t0.m_Last->m_Key == "AG1"
             && t0.m_Last->m_PrevOrder->m_Key == "AAG"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_Key == "LIN"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "CAO"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA2"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "UOS"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA1");
      assert(t0.isSet("ZDM")==false);

      assert(t0.erase("AAG")==true);
        t0.print();
      assert(t0.m_First->m_Key == "PA1"
             && t0.m_First->m_NextOrder->m_Key == "UOS"
             && t0.m_First->m_NextOrder->m_NextOrder->m_Key == "PA2"
             && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "CAO"
             && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "LIN"
             && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "AG1");
      assert(t0.m_Last->m_Key == "AG1"
             && t0.m_Last->m_PrevOrder->m_Key == "LIN"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_Key == "CAO"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA2"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "UOS"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA1");
      assert(t0.isSet("AAG")==false);

      assert(t0.erase("CAO")==true);
        t0.print();
      assert(t0.m_First->m_Key == "PA1"
             && t0.m_First->m_NextOrder->m_Key == "UOS"
             && t0.m_First->m_NextOrder->m_NextOrder->m_Key == "PA2"
             && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "LIN"
             && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "AG1");
      assert(t0.m_Last->m_Key == "AG1"
             && t0.m_Last->m_PrevOrder->m_Key == "LIN"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_Key == "PA2"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "UOS"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA1");
      assert(t0.isSet("CAO")==false);

      assert(t0.erase("UOS")==true);
        t0.print();
      assert(t0.m_First->m_Key == "PA1"
             && t0.m_First->m_NextOrder->m_Key == "PA2"
             && t0.m_First->m_NextOrder->m_NextOrder->m_Key == "LIN"
             && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "AG1");
      assert(t0.m_Last->m_Key == "AG1"
             && t0.m_Last->m_PrevOrder->m_Key == "LIN"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_Key == "PA2"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA1");
      assert(t0.isSet("UOS")==false);

      assert(t0.erase("UOS")==false);
      assert(t0.m_First->m_Key == "PA1"
             && t0.m_First->m_NextOrder->m_Key == "PA2"
             && t0.m_First->m_NextOrder->m_NextOrder->m_Key == "LIN"
             && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "AG1");
      assert(t0.m_Last->m_Key == "AG1"
             && t0.m_Last->m_PrevOrder->m_Key == "LIN"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_Key == "PA2"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA1");
      assert(t0.isSet("UOS")==false);

      assert(t0.erase("LIN")==true);
        t0.print();
      assert(t0.m_First->m_Key == "PA1"
             && t0.m_First->m_NextOrder->m_Key == "PA2"
             && t0.m_First->m_NextOrder->m_NextOrder->m_Key == "AG1");
      assert(t0.m_Last->m_Key == "AG1"
             && t0.m_Last->m_PrevOrder->m_Key == "PA2"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_Key == "PA1");
      assert(t0.isSet("LIN")==false);

      assert(t0.erase("PA1")==true);
        t0.print();
      assert(t0.m_First->m_Key == "PA2"
             && t0.m_First->m_NextOrder->m_Key == "AG1");
      assert(t0.m_Last->m_Key == "AG1"
             && t0.m_Last->m_PrevOrder->m_Key == "PA2");
      assert(t0.isSet("PA1")==false);

      assert(t0.erase("PA2")==true);
        t0.print();
      assert(t0.m_First->m_Key == "AG1");
      assert(t0.m_Last->m_Key == "AG1");
      assert(t0.isSet("PA2")==false);

      assert(t0.erase("AG1")==true);
        t0.print();
      assert(t0.m_First == t0.m_Last);
      assert(t0.isSet("AG1")==false);
    }
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

int main() {
  CTester t;
  t.test();

  return EXIT_SUCCESS;
}
