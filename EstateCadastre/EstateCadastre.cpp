#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Class representing a land record with region, ID, city, and address.
 */
class CLand {
public:
	/**
	 * @brief Constructor for CLand.
	 *
	 * @param region The region where the land is located.
	 * @param id The unique identifier for the land.
	 * @param city The city where the land is located.
	 * @param addr The address of the land.
	 */
	CLand(const std::string& region, const unsigned int& id, const std::string& city, const std::string& addr)
		: ID_(id), Region_(region), City_(city), Addr_(addr) {}

	~CLand() = default;

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Getter for the region of the land.
	 *
	 * @return The region of the land.
	 */
	[[nodiscard]] const std::string& getRegion() const { return Region_; }

	/**
	 * @brief Getter for the ID of the land.
	 *
	 * @return The ID of the land.
	 */
	[[nodiscard]] const unsigned int& getID() const { return ID_; }

	/**
	 * @brief Getter for the city where the land is located.
	 *
	 * @return The city of the land.
	 */
	[[nodiscard]] const std::string& getCity() const { return City_; }

	/**
	 * @brief Getter for the address of the land.
	 *
	 * @return The address of the land.
	 */
	[[nodiscard]] const std::string& getAddr() const { return Addr_; }

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Compare two land records by city and address.
	 *
	 * @param a First land record.
	 * @param b Second land record.
	 * @return True if the first land is less than the second land by city and address, false otherwise.
	 */
	[[nodiscard]] static bool compareByCityAddr(const std::shared_ptr<CLand>& a, const std::shared_ptr<CLand>& b) {
		return std::tie(a->getCity(), a->getAddr()) < std::tie(b->getCity(), b->getAddr());
	}

	/**
	 * @brief Compare two land records by region and ID.
	 *
	 * @param a First land record.
	 * @param b Second land record.
	 * @return True if the first land is less than the second land by region and ID, false otherwise.
	 */
	[[nodiscard]] static bool compareByRegionID(const std::shared_ptr<CLand>& a, const std::shared_ptr<CLand>& b) {
		return std::tie(a->getRegion(), a->getID()) < std::tie(b->getRegion(), b->getID());
	}

	/**
	 * @brief Check if two land records are equal by city and address.
	 *
	 * @param a First land record.
	 * @param b Second land record.
	 * @return True if the land records are equal by city and address, false otherwise.
	 */
	[[nodiscard]] static bool equalByCityAddr(const std::shared_ptr<CLand>& a, const std::shared_ptr<CLand>& b) {
		return a->getCity() == b->getCity() && a->getAddr() == b->getAddr();
	}

	/**
	 * @brief Check if two land records are equal by region and ID.
	 *
	 * @param a First land record.
	 * @param b Second land record.
	 * @return True if the land records are equal by region and ID, false otherwise.
	 */
	[[nodiscard]] static bool equalByRegionID(const std::shared_ptr<CLand>& a, const std::shared_ptr<CLand>& b) {
		return a->getRegion() == b->getRegion() && a->getID() == b->getID();
	}

private:
	unsigned int ID_;
	std::string Region_, City_, Addr_;
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Class representing an owner of land records.
 */
class COwner {
public:
	static constexpr const char* STATE_NAME = "";
	static constexpr const unsigned int STATE_IDX = 0;

	/**
	 * @brief Constructor for COwner.
	 *
	 * @param name The name of the owner.
	 */
	explicit COwner(const std::string& name) : Name_(name), LowerName_(name) {
		std::transform(LowerName_.begin(), LowerName_.end(), LowerName_.begin(),
					   [](unsigned char c) { return std::tolower(c); });
	}

	~COwner() = default;

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Getter for the name of the owner.
	 *
	 * @return The name of the owner.
	 */
	[[nodiscard]] const std::string& getName() const { return Name_; }

	/**
	 * @brief Getter for the lowercase version of the owner's name.
	 *
	 * @return The lowercase name of the owner.
	 */
	[[nodiscard]] const std::string& getLowerName() const { return LowerName_; }

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Compare two owners by their names.
	 *
	 * @param a First owner.
	 * @param b Second owner.
	 * @return True if the first owner's name is less than the second owner's name, false otherwise.
	 */
	[[nodiscard]] static bool compareByName(const std::shared_ptr<COwner>& a, const std::shared_ptr<COwner>& b) {
		return a->getLowerName() < b->getLowerName();
	}

	/**
	 * @brief Check if two owners are equal by their names.
	 *
	 * @param a First owner.
	 * @param b Second owner.
	 * @return True if the owners are equal by their names, false otherwise.
	 */
	[[nodiscard]] static bool equalByName(const std::shared_ptr<COwner>& a, const std::shared_ptr<COwner>& b) {
		return a->getLowerName() == b->getLowerName();
	}

private:
	std::string Name_, LowerName_;
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

using CRecord = std::pair<std::shared_ptr<CLand>, std::shared_ptr<COwner>>;
using CLandCadastre = std::vector<CRecord>;

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Class representing an iterator for land records.
 */
class CIterator {
public:
	/**
	 * @brief Constructor for CIterator.
	 *
	 * @param cadastre The list of land records to iterate over.
	 */
	explicit CIterator(const CLandCadastre& cadastre)
		: Cadastre_(cadastre), CadastreIt_(Cadastre_.begin()), CadastreAuxIt_(Cadastre_.end()) {}

	/**
	 * @brief Constructor for CIterator with an empty iterator option.
	 *
	 * @param cadastre The list of land records.
	 * @param is_empty Flag to create an empty iterator.
	 */
	CIterator(const CLandCadastre cadastre, bool is_empty)
		: Cadastre_(cadastre), CadastreIt_(Cadastre_.end()), CadastreAuxIt_(Cadastre_.begin()) {}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Check if the iterator is at the end of the list.
	 *
	 * @return True if the iterator is at the end, false otherwise.
	 */
	bool atEnd() const { return CadastreIt_ == Cadastre_.end(); }

	/**
	 * @brief Move the iterator to the next element.
	 */
    void next() { ++CadastreIt_; }

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Get the city of the current land record.
	 *
	 * @return The city of the current land record.
	 */
    std::string city() const { return CadastreIt_->first->getCity(); }

	/**
	 * @brief Get the address of the current land record.
	 *
	 * @return The address of the current land record.
	 */
    std::string addr() const { return CadastreIt_->first->getAddr(); }

	/**
	 * @brief Get the region of the current land record.
	 *
	 * @return The region of the current land record.
	 */
    std::string region() const { return CadastreIt_->first->getRegion(); }

	/**
	 * @brief Get the ID of the current land record.
	 *
	 * @return The ID of the current land record.
	 */
    unsigned id() const { return CadastreIt_->first->getID(); }

	/**
	 * @brief Get the owner of the current land record.
	 *
	 * @return The owner of the current land record.
	 */
    std::string owner() const {
		return (CadastreAuxIt_ != Cadastre_.end()) ? CadastreAuxIt_->second->getName() : CadastreIt_->second->getName();
	}

private:
	CLandCadastre Cadastre_;
	CLandCadastre::const_iterator CadastreIt_, CadastreAuxIt_;
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Class representing a land register for managing land records and their owners.
 */
class CLandRegister {
public:
	CLandRegister() : LandsByCityAddr_(), LandsByRegionID_(), OwnersByName_() {
		auto state = std::make_shared<COwner>(COwner::STATE_NAME);
		OwnersByName_.emplace_back(state, CLandCadastre{});
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Add a new land record to the register.
	 *
	 * @param city The city where the land is located.
	 * @param addr The address of the land.
	 * @param region The region where the land is located.
	 * @param id The unique identifier for the land.
	 * @return True if the land is successfully added, false otherwise.
	 */
	bool add(const std::string& city, const std::string& addr, const std::string& region, unsigned int id) {
		auto land = std::make_shared<CLand>(region, id, city, addr);
		auto state_it = OwnersByName_.begin();
		auto state = state_it->first;

		auto land_by_city_addr_it = std::lower_bound(LandsByCityAddr_.begin(), LandsByCityAddr_.end(),
			std::make_pair(land, nullptr),
			[](const auto& a, const auto& b) { return CLand::compareByCityAddr(a.first, b.first); });
		auto land_by_region_id_it = std::lower_bound(LandsByRegionID_.begin(), LandsByRegionID_.end(),
			std::make_pair(land, nullptr),
			[](const auto& a, const auto& b) { return CLand::compareByRegionID(a.first, b.first); });
		if ((land_by_city_addr_it != LandsByCityAddr_.end() && CLand::equalByCityAddr(land, land_by_city_addr_it->first)) ||
				(land_by_region_id_it != LandsByRegionID_.end() && CLand::equalByRegionID(land, land_by_region_id_it->first)))
			return false;

		LandsByCityAddr_.emplace(land_by_city_addr_it, land, state);
		LandsByRegionID_.emplace(land_by_region_id_it, land, state);
		state_it->second.emplace_back(land, state);

		return true;
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Delete a land record by city and address.
	 *
	 * @param city The city where the land is located.
	 * @param addr The address of the land.
	 * @return True if the land is successfully deleted, false otherwise.
	 */
    bool del(const std::string& city, const std::string& addr) {
	    auto land_tmp = std::make_shared<CLand>("", 0, city, addr);

	    auto land_it = std::lower_bound(LandsByCityAddr_.begin(), LandsByCityAddr_.end(),
			std::make_pair(land_tmp, nullptr),
			[](const auto& a, const auto& b) { return CLand::compareByCityAddr(a.first, b.first); });
	    if (land_it == LandsByCityAddr_.end() || !CLand::equalByCityAddr(land_tmp, land_it->first))
		    return false;
	    auto land = land_it->first;
	    auto owner = land_it->second;
	    LandsByCityAddr_.erase(land_it);

	    land_it = std::lower_bound(LandsByRegionID_.begin(), LandsByRegionID_.end(),
	        std::make_pair(land, nullptr),
			[](const auto& a, const auto& b) { return CLand::compareByRegionID(a.first, b.first); });
	    LandsByRegionID_.erase(land_it);

	    auto owner_it = std::lower_bound(OwnersByName_.begin(), OwnersByName_.end(),
	        std::make_pair(owner, nullptr),
	        [](const auto& a, const auto& b) { return COwner::compareByName(a.first, b.first); });
	    land_it = std::find_if(owner_it->second.begin(), owner_it->second.end(),
			[&land](const auto& x) { return CLand::equalByCityAddr(land, x.first); });
		owner_it->second.erase(land_it);

		return true;
	}

	/**
	 * @brief Delete a land record by region and ID.
	 *
	 * @param region The region where the land is located.
	 * @param id The unique identifier for the land.
	 * @return True if the land is successfully deleted, false otherwise.
	 */
    bool del(const std::string& region, unsigned int id) {
	    auto land_tmp = std::make_shared<CLand>(region, id, "", "");

	    auto land_it = std::lower_bound(LandsByRegionID_.begin(), LandsByRegionID_.end(),
	        std::make_pair(land_tmp, nullptr),
	        [](const auto& a, const auto& b) { return CLand::compareByRegionID(a.first, b.first); });
	    if (land_it == LandsByRegionID_.end() || !CLand::equalByRegionID(land_tmp, land_it->first))
		    return false;
	    auto land = land_it->first;
	    auto owner = land_it->second;
	    LandsByRegionID_.erase(land_it);

	    land_it = std::lower_bound(LandsByCityAddr_.begin(), LandsByCityAddr_.end(),
	        std::make_pair(land, nullptr),
	        [](const auto& a, const auto& b) { return CLand::compareByCityAddr(a.first, b.first); });
	    LandsByCityAddr_.erase(land_it);

	    auto owner_it = std::lower_bound(OwnersByName_.begin(), OwnersByName_.end(),
	        std::make_pair(owner, nullptr),
	        [](const auto& a, const auto& b) { return COwner::compareByName(a.first, b.first); });
	    land_it = std::find_if(owner_it->second.begin(), owner_it->second.end(),
	        [&land](const auto& x) { return CLand::equalByCityAddr(land, x.first); });
	    owner_it->second.erase(land_it);

	    return true;
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Get the owner of a land record by city and address.
	 *
	 * @param city The city where the land is located.
	 * @param addr The address of the land.
	 * @param owner Output parameter to store the name of the owner.
	 * @return True if the owner is found, false otherwise.
	 */
    bool getOwner(const std::string& city, const std::string& addr, std::string& owner) const {
	    auto land_tmp = std::make_shared<CLand>("", 0, city, addr);

	    auto land_it = std::lower_bound(LandsByCityAddr_.begin(), LandsByCityAddr_.end(),
	        std::make_pair(land_tmp, nullptr),
	        [](const auto& a, const auto& b) { return CLand::compareByCityAddr(a.first, b.first); });
	    if (land_it == LandsByCityAddr_.end() || !CLand::equalByCityAddr(land_tmp, land_it->first))
		    return false;

		owner = land_it->second->getName();

		return true;
	}

	/**
	 * @brief Get the owner of a land record by region and ID.
	 *
	 * @param region The region where the land is located.
	 * @param id The unique identifier for the land.
	 * @param owner Output parameter to store the name of the owner.
	 * @return True if the owner is found, false otherwise.
	 */
    bool getOwner(const std::string& region, unsigned int id, std::string& owner) const {
	    auto land_tmp = std::make_shared<CLand>(region, id, "", "");

	    auto land_it = std::lower_bound(LandsByRegionID_.begin(), LandsByRegionID_.end(),
	        std::make_pair(land_tmp, nullptr),
	        [](const auto& a, const auto& b) { return CLand::compareByRegionID(a.first, b.first); });
	    if (land_it == LandsByRegionID_.end() || !CLand::equalByRegionID(land_tmp, land_it->first))
		    return false;

	    owner = land_it->second->getName();

	    return true;
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Assign a new owner to a land record by city and address.
	 *
	 * @param city The city where the land is located.
	 * @param addr The address of the land.
	 * @param owner The new owner's name.
	 * @return True if the owner is successfully assigned, false otherwise.
	 */
    bool newOwner(const std::string& city, const std::string& addr, const std::string& owner) {
	    auto land_tmp = std::make_shared<CLand>("", 0, city, addr);
	    auto new_owner_tmp = std::make_shared<COwner>(owner);

	    auto land_it = std::lower_bound(LandsByCityAddr_.begin(), LandsByCityAddr_.end(),
	        std::make_pair(land_tmp, nullptr),
	        [](const auto& a, const auto& b) { return CLand::compareByCityAddr(a.first, b.first); });
	    if (land_it == LandsByCityAddr_.end() || !CLand::equalByCityAddr(land_tmp, land_it->first) ||
				COwner::equalByName(new_owner_tmp, land_it->second))
		    return false;
	    auto land = land_it->first;
	    auto old_owner = land_it->second;
	    auto new_owner = new_owner_tmp;
	    land_it->second = new_owner;

	    land_it = std::lower_bound(LandsByRegionID_.begin(), LandsByRegionID_.end(),
	        std::make_pair(land, nullptr),
	        [](const auto& a, const auto& b) { return CLand::compareByRegionID(a.first, b.first); });
	    land_it->second = new_owner;

	    auto old_owner_it = std::lower_bound(OwnersByName_.begin(), OwnersByName_.end(),
	        std::make_pair(old_owner, nullptr),
	        [](const auto& a, const auto& b) { return COwner::compareByName(a.first, b.first); });
	    land_it = std::find_if(old_owner_it->second.begin(), old_owner_it->second.end(),
			[&land](const auto& x) { return CLand::equalByCityAddr(land, x.first); });
	    old_owner_it->second.erase(land_it);

	    auto new_owner_it = std::lower_bound(OwnersByName_.begin(), OwnersByName_.end(),
	        std::make_pair(new_owner_tmp, nullptr),
	        [](const auto& a, const auto& b) { return COwner::compareByName(a.first, b.first); });
	    if (new_owner_it == OwnersByName_.end() || !COwner::equalByName(new_owner_tmp, new_owner_it->first))
		    new_owner_it = OwnersByName_.emplace(new_owner_it, new_owner_tmp, CLandCadastre{});
		new_owner_it->second.emplace_back(land, new_owner);

		return true;
	}

	/**
	 * @brief Assign a new owner to a land record by region and ID.
	 *
	 * @param region The region where the land is located.
	 * @param id The unique identifier for the land.
	 * @param owner The new owner's name.
	 * @return True if the owner is successfully assigned, false otherwise.
	 */
    bool newOwner(const std::string& region, unsigned int id, const std::string& owner) {
	    auto land_tmp = std::make_shared<CLand>(region, id, "", "");
	    auto new_owner_tmp = std::make_shared<COwner>(owner);

	    auto land_it = std::lower_bound(LandsByRegionID_.begin(), LandsByRegionID_.end(),
	        std::make_pair(land_tmp, nullptr),
	        [](const auto& a, const auto& b) { return CLand::compareByRegionID(a.first, b.first); });
	    if (land_it == LandsByRegionID_.end() || !CLand::equalByRegionID(land_tmp, land_it->first) ||
				COwner::equalByName(new_owner_tmp, land_it->second))
		    return false;
	    auto land = land_it->first;
	    auto old_owner = land_it->second;
	    auto new_owner = new_owner_tmp;
	    land_it->second = new_owner;

	    land_it = std::lower_bound(LandsByCityAddr_.begin(), LandsByCityAddr_.end(),
	        std::make_pair(land, nullptr),
	        [](const auto& a, const auto& b) { return CLand::compareByCityAddr(a.first, b.first); });
	    land_it->second = new_owner;

	    auto old_owner_it = std::lower_bound(OwnersByName_.begin(), OwnersByName_.end(),
	        std::make_pair(old_owner, nullptr),
	        [](const auto& a, const auto& b) { return COwner::compareByName(a.first, b.first); });
	    land_it = std::find_if(old_owner_it->second.begin(), old_owner_it->second.end(),
	        [&land](const auto& x) { return CLand::equalByCityAddr(land, x.first); });
	    old_owner_it->second.erase(land_it);

	    auto new_owner_it = std::lower_bound(OwnersByName_.begin(), OwnersByName_.end(),
	        std::make_pair(new_owner, nullptr),
	        [](const auto& a, const auto& b) { return COwner::compareByName(a.first, b.first); });
	    if (new_owner_it == OwnersByName_.end() || !COwner::equalByName(new_owner, new_owner_it->first))
		    new_owner_it = OwnersByName_.emplace(new_owner_it, new_owner, CLandCadastre{});
	    new_owner_it->second.emplace_back(land, new_owner);

	    return true;
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Get the count of land records owned by a specific owner.
	 *
	 * @param owner The name of the owner.
	 * @return The count of land records owned by the owner.
	 */
    size_t count(const std::string& owner) const {
		size_t owner_land_count;

	    auto owner_tmp = std::make_shared<COwner>(owner);

	    auto owner_it = std::lower_bound(OwnersByName_.begin(), OwnersByName_.end(),
	        std::make_pair(owner_tmp, nullptr),
	        [](const auto& a, const auto& b) { return COwner::compareByName(a.first, b.first); });
		if (owner_it == OwnersByName_.end() || !COwner::equalByName(owner_tmp, owner_it->first))
		    owner_land_count = 0;
		else
			owner_land_count = owner_it->second.size();

		return owner_land_count;
	}

	/**
	 * @brief List land records sorted by city and address.
	 *
	 * @return An iterator for the list of land records sorted by city and address.
	 */
    CIterator listByAddr() const { return CIterator(LandsByCityAddr_); }

	/**
	 * @brief List land records owned by a specific owner.
	 *
	 * @param owner The name of the owner.
	 * @return An iterator for the list of land records owned by the specified owner.
	 */
    CIterator listByOwner(const std::string& owner) const {
		auto owner_tmp = std::make_shared<COwner>(owner);

	    auto owner_it = std::lower_bound(OwnersByName_.begin(), OwnersByName_.end(),
	        std::make_pair(owner_tmp, nullptr),
	        [](const auto& a, const auto& b) { return COwner::compareByName(a.first, b.first); });
		if (owner_it == OwnersByName_.end() || !COwner::equalByName(owner_tmp, owner_it->first))
			return CIterator(CLandCadastre{CRecord{nullptr, owner_tmp}}, true);
		else
			return CIterator(owner_it->second);
	}

private:
	CLandCadastre LandsByCityAddr_, LandsByRegionID_;
	std::vector<std::pair<std::shared_ptr<COwner>, CLandCadastre>> OwnersByName_;
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

static void test0 ()
{
  CLandRegister x;
  std::string owner;

  assert ( x . add ( "Prague", "Thakurova", "Dejvice", 12345 ) );
  assert ( x . add ( "Prague", "Evropska", "Vokovice", 12345 ) );
  assert ( x . add ( "Prague", "Technicka", "Dejvice", 9873 ) );
  assert ( x . add ( "Plzen", "Evropska", "Plzen mesto", 78901 ) );
  assert ( x . add ( "Liberec", "Evropska", "Librec", 4552 ) );
  CIterator i0 = x . listByAddr ();
  assert ( ! i0 . atEnd ()
           && i0 . city () == "Liberec"
           && i0 . addr () == "Evropska"
           && i0 . region () == "Librec"
           && i0 . id () == 4552
           && i0 . owner () == "" );
  i0 . next ();
  assert ( ! i0 . atEnd ()
           && i0 . city () == "Plzen"
           && i0 . addr () == "Evropska"
           && i0 . region () == "Plzen mesto"
           && i0 . id () == 78901
           && i0 . owner () == "" );
  i0 . next ();
  assert ( ! i0 . atEnd ()
           && i0 . city () == "Prague"
           && i0 . addr () == "Evropska"
           && i0 . region () == "Vokovice"
           && i0 . id () == 12345
           && i0 . owner () == "" );
  i0 . next ();
  assert ( ! i0 . atEnd ()
           && i0 . city () == "Prague"
           && i0 . addr () == "Technicka"
           && i0 . region () == "Dejvice"
           && i0 . id () == 9873
           && i0 . owner () == "" );
  i0 . next ();
  assert ( ! i0 . atEnd ()
           && i0 . city () == "Prague"
           && i0 . addr () == "Thakurova"
           && i0 . region () == "Dejvice"
           && i0 . id () == 12345
           && i0 . owner () == "" );
  i0 . next ();
  assert ( i0 . atEnd () );

  assert ( x . count ( "" ) == 5 );
  CIterator i1 = x . listByOwner ( "" );
  assert ( ! i1 . atEnd ()
           && i1 . city () == "Prague"
           && i1 . addr () == "Thakurova"
           && i1 . region () == "Dejvice"
           && i1 . id () == 12345
           && i1 . owner () == "" );
  i1 . next ();
  assert ( ! i1 . atEnd ()
           && i1 . city () == "Prague"
           && i1 . addr () == "Evropska"
           && i1 . region () == "Vokovice"
           && i1 . id () == 12345
           && i1 . owner () == "" );
  i1 . next ();
  assert ( ! i1 . atEnd ()
           && i1 . city () == "Prague"
           && i1 . addr () == "Technicka"
           && i1 . region () == "Dejvice"
           && i1 . id () == 9873
           && i1 . owner () == "" );
  i1 . next ();
  assert ( ! i1 . atEnd ()
           && i1 . city () == "Plzen"
           && i1 . addr () == "Evropska"
           && i1 . region () == "Plzen mesto"
           && i1 . id () == 78901
           && i1 . owner () == "" );
  i1 . next ();
  assert ( ! i1 . atEnd ()
           && i1 . city () == "Liberec"
           && i1 . addr () == "Evropska"
           && i1 . region () == "Librec"
           && i1 . id () == 4552
           && i1 . owner () == "" );
  i1 . next ();
  assert ( i1 . atEnd () );

  assert ( x . count ( "CVUT" ) == 0 );
  CIterator i2 = x . listByOwner ( "CVUT" );
  assert ( i2 . atEnd () );

  assert ( x . newOwner ( "Prague", "Thakurova", "CVUT" ) );
  assert ( x . newOwner ( "Dejvice", 9873, "CVUT" ) );
  assert ( x . newOwner ( "Plzen", "Evropska", "Anton Hrabis" ) );
  assert ( x . newOwner ( "Librec", 4552, "Cvut" ) );
  assert ( x . getOwner ( "Prague", "Thakurova", owner ) && owner == "CVUT" );
  assert ( x . getOwner ( "Dejvice", 12345, owner ) && owner == "CVUT" );
  assert ( x . getOwner ( "Prague", "Evropska", owner ) && owner == "" );
  assert ( x . getOwner ( "Vokovice", 12345, owner ) && owner == "" );
  assert ( x . getOwner ( "Prague", "Technicka", owner ) && owner == "CVUT" );
  assert ( x . getOwner ( "Dejvice", 9873, owner ) && owner == "CVUT" );
  assert ( x . getOwner ( "Plzen", "Evropska", owner ) && owner == "Anton Hrabis" );
  assert ( x . getOwner ( "Plzen mesto", 78901, owner ) && owner == "Anton Hrabis" );
  assert ( x . getOwner ( "Liberec", "Evropska", owner ) && owner == "Cvut" );
  assert ( x . getOwner ( "Librec", 4552, owner ) && owner == "Cvut" );
  CIterator i3 = x . listByAddr ();
  assert ( ! i3 . atEnd ()
           && i3 . city () == "Liberec"
           && i3 . addr () == "Evropska"
           && i3 . region () == "Librec"
           && i3 . id () == 4552
           && i3 . owner () == "Cvut" );
  i3 . next ();
  assert ( ! i3 . atEnd ()
           && i3 . city () == "Plzen"
           && i3 . addr () == "Evropska"
           && i3 . region () == "Plzen mesto"
           && i3 . id () == 78901
           && i3 . owner () == "Anton Hrabis" );
  i3 . next ();
  assert ( ! i3 . atEnd ()
           && i3 . city () == "Prague"
           && i3 . addr () == "Evropska"
           && i3 . region () == "Vokovice"
           && i3 . id () == 12345
           && i3 . owner () == "" );
  i3 . next ();
  assert ( ! i3 . atEnd ()
           && i3 . city () == "Prague"
           && i3 . addr () == "Technicka"
           && i3 . region () == "Dejvice"
           && i3 . id () == 9873
           && i3 . owner () == "CVUT" );
  i3 . next ();
  assert ( ! i3 . atEnd ()
           && i3 . city () == "Prague"
           && i3 . addr () == "Thakurova"
           && i3 . region () == "Dejvice"
           && i3 . id () == 12345
           && i3 . owner () == "CVUT" );
  i3 . next ();
  assert ( i3 . atEnd () );

  assert ( x . count ( "cvut" ) == 3 );
  CIterator i4 = x . listByOwner ( "cVuT" );
  assert ( ! i4 . atEnd ()
           && i4 . city () == "Prague"
           && i4 . addr () == "Thakurova"
           && i4 . region () == "Dejvice"
           && i4 . id () == 12345
           && i4 . owner () == "CVUT" );
  i4 . next ();
  assert ( ! i4 . atEnd ()
           && i4 . city () == "Prague"
           && i4 . addr () == "Technicka"
           && i4 . region () == "Dejvice"
           && i4 . id () == 9873
           && i4 . owner () == "CVUT" );
  i4 . next ();
  assert ( ! i4 . atEnd ()
           && i4 . city () == "Liberec"
           && i4 . addr () == "Evropska"
           && i4 . region () == "Librec"
           && i4 . id () == 4552
           && i4 . owner () == "Cvut" );
  i4 . next ();
  assert ( i4 . atEnd () );

  assert ( x . newOwner ( "Plzen mesto", 78901, "CVut" ) );
  assert ( x . count ( "CVUT" ) == 4 );
  CIterator i5 = x . listByOwner ( "CVUT" );
  assert ( ! i5 . atEnd ()
           && i5 . city () == "Prague"
           && i5 . addr () == "Thakurova"
           && i5 . region () == "Dejvice"
           && i5 . id () == 12345
           && i5 . owner () == "CVUT" );
  i5 . next ();
  assert ( ! i5 . atEnd ()
           && i5 . city () == "Prague"
           && i5 . addr () == "Technicka"
           && i5 . region () == "Dejvice"
           && i5 . id () == 9873
           && i5 . owner () == "CVUT" );
  i5 . next ();
  assert ( ! i5 . atEnd ()
           && i5 . city () == "Liberec"
           && i5 . addr () == "Evropska"
           && i5 . region () == "Librec"
           && i5 . id () == 4552
           && i5 . owner () == "Cvut" );
  i5 . next ();
  assert ( ! i5 . atEnd ()
           && i5 . city () == "Plzen"
           && i5 . addr () == "Evropska"
           && i5 . region () == "Plzen mesto"
           && i5 . id () == 78901
           && i5 . owner () == "CVut" );
  i5 . next ();
  assert ( i5 . atEnd () );

  assert ( x . del ( "Liberec", "Evropska" ) );
  assert ( x . del ( "Plzen mesto", 78901 ) );
  assert ( x . count ( "cvut" ) == 2 );
  CIterator i6 = x . listByOwner ( "cVuT" );
  assert ( ! i6 . atEnd ()
           && i6 . city () == "Prague"
           && i6 . addr () == "Thakurova"
           && i6 . region () == "Dejvice"
           && i6 . id () == 12345
           && i6 . owner () == "CVUT" );
  i6 . next ();
  assert ( ! i6 . atEnd ()
           && i6 . city () == "Prague"
           && i6 . addr () == "Technicka"
           && i6 . region () == "Dejvice"
           && i6 . id () == 9873
           && i6 . owner () == "CVUT" );
  i6 . next ();
  assert ( i6 . atEnd () );

  assert ( x . add ( "Liberec", "Evropska", "Librec", 4552 ) );
}

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

static void test1 ()
{
  CLandRegister x;
  std::string owner;

  assert ( x . add ( "Prague", "Thakurova", "Dejvice", 12345 ) );
  assert ( x . add ( "Prague", "Evropska", "Vokovice", 12345 ) );
  assert ( x . add ( "Prague", "Technicka", "Dejvice", 9873 ) );
  assert ( ! x . add ( "Prague", "Technicka", "Hradcany", 7344 ) );
  assert ( ! x . add ( "Brno", "Bozetechova", "Dejvice", 9873 ) );
  assert ( !x . getOwner ( "Prague", "THAKUROVA", owner ) );
  assert ( !x . getOwner ( "Hradcany", 7343, owner ) );
  CIterator i0 = x . listByAddr ();
  assert ( ! i0 . atEnd ()
           && i0 . city () == "Prague"
           && i0 . addr () == "Evropska"
           && i0 . region () == "Vokovice"
           && i0 . id () == 12345
           && i0 . owner () == "" );
  i0 . next ();
  assert ( ! i0 . atEnd ()
           && i0 . city () == "Prague"
           && i0 . addr () == "Technicka"
           && i0 . region () == "Dejvice"
           && i0 . id () == 9873
           && i0 . owner () == "" );
  i0 . next ();
  assert ( ! i0 . atEnd ()
           && i0 . city () == "Prague"
           && i0 . addr () == "Thakurova"
           && i0 . region () == "Dejvice"
           && i0 . id () == 12345
           && i0 . owner () == "" );
  i0 . next ();
  assert ( i0 . atEnd () );

  assert ( x . newOwner ( "Prague", "Thakurova", "CVUT" ) );
  assert ( ! x . newOwner ( "Prague", "technicka", "CVUT" ) );
  assert ( ! x . newOwner ( "prague", "Technicka", "CVUT" ) );
  assert ( ! x . newOwner ( "dejvice", 9873, "CVUT" ) );
  assert ( ! x . newOwner ( "Dejvice", 9973, "CVUT" ) );
  assert ( ! x . newOwner ( "Dejvice", 12345, "CVUT" ) );
  assert ( x . count ( "CVUT" ) == 1 );
  CIterator i1 = x . listByOwner ( "CVUT" );
  assert ( ! i1 . atEnd ()
           && i1 . city () == "Prague"
           && i1 . addr () == "Thakurova"
           && i1 . region () == "Dejvice"
           && i1 . id () == 12345
           && i1 . owner () == "CVUT" );
  i1 . next ();
  assert ( i1 . atEnd () );

  assert ( ! x . del ( "Brno", "Technicka" ) );
  assert ( ! x . del ( "Karlin", 9873 ) );
  assert ( x . del ( "Prague", "Technicka" ) );
  assert ( ! x . del ( "Prague", "Technicka" ) );
  assert ( ! x . del ( "Dejvice", 9873 ) );
}

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

int main ( void )
{
	test0 ();
	test1 ();

	return EXIT_SUCCESS;
}
