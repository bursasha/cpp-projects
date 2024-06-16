#include <cassert>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <memory>
#include <functional>

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Abstract base class for printable components.
 *
 * Provides an interface for components that can be printed with a specific format,
 * including a prefix and a flag indicating if the component is the last in a sequence.
 */
class CPrintable {
protected:
	static constexpr const char* DEF_PREFIX_ = "", * ITEM_ = "+-", * PIPE_ = "| ", * GAP_ = "  ", * LAST_ = "\\-";
	static constexpr bool DEF_IS_LAST_ = false;

public:
	CPrintable() = default;

	virtual ~CPrintable() = default;

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Pure virtual function to print the component.
	 *
	 * @param os The output stream.
	 * @param is_last Boolean indicating if this is the last component.
	 * @param prefix The prefix to use for printing.
	 */
	virtual void print(std::ostream& os, bool is_last, const std::string& prefix) const = 0;
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Abstract base class for components.
 *
 * Inherits from CPrintable and provides functionality for managing component specifications.
 */
class CComponent : public CPrintable {
protected:
	std::map<std::string, size_t> specs_by_name_;

public:
	CComponent() = default;

	CComponent(const CComponent& other)
			: specs_by_name_(other.specs_by_name_) {}

	~CComponent() override = default;

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Pure virtual function to create a wrapped copy of the component.
	 *
	 * @return A shared pointer to the wrapped component.
	 */
	[[nodiscard]] virtual std::shared_ptr<CComponent> wrap() const = 0;
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Class representing a CPU component.
 *
 * Inherits from CComponent and provides specific functionality for a CPU with cores and frequency.
 */
class CCPU : public CComponent {
private:
	static constexpr const char* CORE_ = "CORES", * FREQ_ = "FREQUENCY";

public:
	/**
	 * @brief Constructor for creating a CPU with specified cores and frequency.
	 *
	 * @param cores The number of cores.
	 * @param frequency The frequency in MHz.
	 */
	CCPU(size_t cores, size_t frequency) {
		specs_by_name_[CCPU::CORE_] = cores;
		specs_by_name_[CCPU::FREQ_] = frequency;
	}

	CCPU(const CCPU& other) = default;

	~CCPU() override = default;

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Print the CPU details.
	 *
	 * @param os The output stream.
	 * @param is_last Boolean indicating if this is the last component.
	 * @param prefix The prefix to use for printing.
	 */
	void print(std::ostream& os, bool is_last, const std::string& prefix) const override {
		os
			<< prefix
			<< (is_last ? CPrintable::LAST_ : CPrintable::ITEM_)
			<< "CPU, " << cores() << " cores @ " << frequency() << "MHz"
			<< std::endl;
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Create a wrapped copy of the CPU.
	 *
	 * @return A shared pointer to the wrapped CPU.
	 */
	[[nodiscard]] std::shared_ptr<CComponent> wrap() const override {
		return std::make_shared<CCPU>(*this);
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Get the number of cores.
	 *
	 * @return The number of cores.
	 */
	[[nodiscard]] size_t cores() const {
		return specs_by_name_.at(CCPU::CORE_);
	}

	/**
	 * @brief Get the frequency.
	 *
	 * @return The frequency in MHz.
	 */
	[[nodiscard]] size_t frequency() const {
		return specs_by_name_.at(CCPU::FREQ_);
	}
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Class representing a memory component.
 *
 * Inherits from CComponent and provides specific functionality for memory with a specified volume.
 */
class CMemory : public CComponent {
private:
	static constexpr const char* VOL_ = "VOLUME";

public:
	/**
	 * @brief Constructor for creating memory with specified volume.
	 *
	 * @param volume The volume in MiB.
	 */
	explicit CMemory(size_t volume) {
		specs_by_name_[CMemory::VOL_] = volume;
	}

	CMemory(const CMemory& other) = default;

	~CMemory() override = default;

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Print the memory details.
	 *
	 * @param os The output stream.
	 * @param is_last Boolean indicating if this is the last component.
	 * @param prefix The prefix to use for printing.
	 */
	void print(std::ostream& os, bool is_last, const std::string& prefix) const override {
		os
			<< prefix
			<< (is_last ? CPrintable::LAST_ : CPrintable::ITEM_)
			<< "Memory, " << volume() << " MiB"
			<< std::endl;
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Create a wrapped copy of the memory.
	 *
	 * @return A shared pointer to the wrapped memory.
	 */
	[[nodiscard]] std::shared_ptr<CComponent> wrap() const override {
		return std::make_shared<CMemory>(*this);
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Get the volume of memory.
	 *
	 * @return The volume in MiB.
	 */
	[[nodiscard]] size_t volume() const {
		return specs_by_name_.at(CMemory::VOL_);
	}
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Class representing a disk component.
 *
 * Inherits from CComponent and provides specific functionality for disks, including partitions and type (SSD or magnetic).
 */
class CDisk : public CComponent {
public:
	enum CDiskType {SSD, MAGNETIC};

private:
	static constexpr const char* VOL_ = "VOLUME";

	// -----------------------------------------------------------------------------------------------------------------

	CDiskType type_;
	std::vector<std::pair<size_t, std::string>> partitions_;

public:
	/**
	 * @brief Constructor for creating a disk with specified type and volume.
	 *
	 * @param type The type of disk (SSD or magnetic).
	 * @param volume The volume in GiB.
	 */
	CDisk(CDisk::CDiskType type, size_t volume)
			: type_(type), partitions_() {
		specs_by_name_[CDisk::VOL_] = volume;
	}

	CDisk(const CDisk& other)
			: CComponent(other), type_(other.type()), partitions_(other.partitions()) {}

	~CDisk() override = default;

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Print the disk details and its partitions.
	 *
	 * @param os The output stream.
	 * @param is_last Boolean indicating if this is the last component.
	 * @param prefix The prefix to use for printing.
	 */
	void print(std::ostream& os, bool is_last, const std::string& prefix) const override {
		os
			<< prefix
			<< (is_last ? CPrintable::LAST_ : CPrintable::ITEM_)
			<< serialized_type() << ", " << volume() << " GiB"
			<< std::endl;

		for (size_t idx = 0; idx < partitions().size(); ++idx) {
			bool is_last_partition = (idx == partitions().size() - 1);

			os
				<< prefix
				<< (is_last ? CPrintable::GAP_ : CPrintable::PIPE_)
				<< (is_last_partition ? CPrintable::LAST_ : CPrintable::ITEM_)
				<< "[" << idx << "]: " << partitions().at(idx).first << " GiB, " << partitions().at(idx).second
				<< std::endl;
		}
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Create a wrapped copy of the disk.
	 *
	 * @return A shared pointer to the wrapped disk.
	 */
	[[nodiscard]] std::shared_ptr<CComponent> wrap() const override {
		return std::make_shared<CDisk>(*this);
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Add a partition to the disk.
	 *
	 * @param size The size of the partition in GiB.
	 * @param name The name of the partition.
	 * @return A reference to the disk with the added partition.
	 */
	CDisk& addPartition(size_t size, const std::string& name) {
		partitions_.emplace_back(size, name);

		return *this;
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Get the volume of the disk.
	 *
	 * @return The volume in GiB.
	 */
	[[nodiscard]] size_t volume() const {
		return specs_by_name_.at(CDisk::VOL_);
	}

	/**
	 * @brief Get the type of the disk.
	 *
	 * @return The type of the disk.
	 */
	[[nodiscard]] CDiskType type() const {
		return type_;
	}

	/**
	 * @brief Get the serialized type of the disk as a string.
	 *
	 * @return The serialized type of the disk.
	 */
	[[nodiscard]] std::string serialized_type() const {
		std::string serialized_type;

		switch (type_) {
			case CDiskType::SSD:
				serialized_type = "SSD";
				break;

			case CDiskType::MAGNETIC:
				serialized_type = "HDD";
				break;
		}

		return serialized_type;
	}

	/**
	 * @brief Get the partitions of the disk.
	 *
	 * @return A vector of partitions.
	 */
	[[nodiscard]] const std::vector<std::pair<size_t, std::string>>& partitions() const {
		return partitions_;
	}
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Class representing a computer.
 *
 * Inherits from CPrintable and provides functionality for managing addresses and components.
 */
class CComputer : public CPrintable {
private:
	std::string name_;
	std::vector<std::string> addresses_;
	std::vector<std::shared_ptr<CComponent>> wrapped_components_;

public:
	/**
	 * @brief Constructor for creating a computer with a specified name.
	 *
	 * @param name The name of the computer.
	 */
	explicit CComputer(std::string name)
			: name_(std::move(name)), addresses_(), wrapped_components_() {}

	CComputer(const CComputer& other)
			: name_(other.name()), addresses_(other.addresses()) {
		for (const auto& wrapped_component : other.wrapped_components_)
			addComponent(*wrapped_component);
	}

	~CComputer() override = default;

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Output stream operator for printing the computer.
	 *
	 * @param os The output stream.
	 * @param c The computer to print.
	 * @return A reference to the output stream.
	 */
	friend std::ostream& operator<<(std::ostream& os, const CComputer& c) {
		c.print(os, CPrintable::DEF_IS_LAST_, CPrintable::DEF_PREFIX_);

		return os;
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Print the computer details, including its addresses and components.
	 *
	 * @param os The output stream.
	 * @param is_last Boolean indicating if this is the last component.
	 * @param prefix The prefix to use for printing.
	 */
	void print(std::ostream& os, bool is_last, const std::string& prefix) const override {
		bool is_in_network = !(prefix.empty()), are_components = !(wrapped_components().empty());

		os
			<< (is_in_network ? (is_last ? CPrintable::LAST_ : CPrintable::ITEM_) : prefix)
			<< "Host: " << name()
			<< std::endl;

		for (size_t idx = 0; idx < addresses().size(); ++idx) {
			bool is_last_address = (idx == addresses().size() - 1);

			os
				<< prefix
				<< (is_last_address && !are_components ? CPrintable::LAST_ : CPrintable::ITEM_)
				<< addresses().at(idx)
				<< std::endl;
		}

		for (size_t idx = 0; idx < wrapped_components().size(); ++idx) {
			bool is_last_component = (idx == wrapped_components().size() - 1);

			wrapped_components().at(idx)
				->print(os, is_last_component, prefix);
		}
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Add an address to the computer.
	 *
	 * @param address The address to add.
	 * @return A reference to the computer with the added address.
	 */
	CComputer& addAddress(const std::string& address) {
		addresses_.emplace_back(address);

		return *this;
	}

	/**
	 * @brief Add a component to the computer.
	 *
	 * @param component The component to add.
	 * @return A reference to the computer with the added component.
	 */
	CComputer& addComponent(const CComponent& component) {
		wrapped_components_.emplace_back(component.wrap());

		return *this;
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Get the name of the computer.
	 *
	 * @return The name of the computer.
	 */
	[[nodiscard]] const std::string& name() const {
		return name_;
	}

	/**
	 * @brief Get the addresses of the computer.
	 *
	 * @return A vector of addresses.
	 */
	[[nodiscard]] const std::vector<std::string>& addresses() const {
		return addresses_;
	}

	/**
	 * @brief Get the components of the computer.
	 *
	 * @return A vector of components.
	 */
	[[nodiscard]] const std::vector<std::shared_ptr<CComponent>>& wrapped_components() const {
		return wrapped_components_;
	}
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Class representing a network.
 *
 * Inherits from CPrintable and provides functionality for managing computers.
*/
class CNetwork : public CPrintable {
private:
	std::string name_;
	std::vector<std::shared_ptr<CComputer>> wrapped_computers_;
	std::map<std::string, std::shared_ptr<CComputer>> wrapped_computers_by_name_;

public:
	/**
	 * @brief Constructor for creating a network with a specified name.
	 *
	 * @param name The name of the network.
	 */
	explicit CNetwork(std::string name)
			: name_(std::move(name)), wrapped_computers_(), wrapped_computers_by_name_() {}

	CNetwork(const CNetwork& other)
			: name_(other.name()) {
		for (const auto& wrapped_computer : other.wrapped_computers_)
			addComputer(*wrapped_computer);
	}

	~CNetwork() override = default;

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Output stream operator for printing the network.
	 *
	 * @param os The output stream.
	 * @param n The network to print.
	 * @return A reference to the output stream.
	 */
	friend std::ostream& operator<<(std::ostream& os, const CNetwork& n) {
		n.print(os, CPrintable::DEF_IS_LAST_, CPrintable::DEF_PREFIX_);

		return os;
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Print the network details, including its computers.
	 *
	 * @param os The output stream.
	 * @param is_last Boolean indicating if this is the last component.
	 * @param prefix The prefix to use for printing.
	 */
	void print(std::ostream& os, bool is_last, const std::string& prefix) const override {
		os
			<< "Network: " << name()
			<< std::endl;

		for (size_t idx = 0; idx < wrapped_computers().size(); ++idx) {
			bool is_last_computer = (idx == wrapped_computers().size() - 1);

			wrapped_computers().at(idx)
				->print(os, is_last_computer, (is_last_computer ? CPrintable::GAP_ : CPrintable::PIPE_));
		}
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Add a computer to the network.
	 *
	 * @param computer The computer to add.
	 * @return A reference to the network with the added computer.
	 */
	CNetwork& addComputer(const CComputer& computer) {
		auto wrapped_computer = std::make_shared<CComputer>(computer);

		wrapped_computers_.emplace_back(wrapped_computer);
		wrapped_computers_by_name_.emplace(wrapped_computer->name(), wrapped_computer);

		return *this;
	}

	/**
	 * @brief Find a computer by name in the network.
	 *
	 * @param computer_name The name of the computer to find.
	 * @return A shared pointer to the found computer, or nullptr if not found.
	 */
	[[nodiscard]] std::shared_ptr<CComputer> findComputer(const std::string& computer_name) const {
		return wrapped_computers_by_name_.contains(computer_name) ?
			wrapped_computers_by_name_.at(computer_name) : nullptr;
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Get the name of the network.
	 *
	 * @return The name of the network.
	 */
	[[nodiscard]] const std::string& name() const {
		return name_;
	}

	/**
	 * @brief Get the computers in the network.
	 *
	 * @return A vector of computers.
	 */
	[[nodiscard]] const std::vector<std::shared_ptr<CComputer>>& wrapped_computers() const {
		return wrapped_computers_;
	}
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

template<typename T_>
std::string toString(const T_& x) {
	std::ostringstream oss;
	oss << x;

	return oss.str();
}

int main() {
	CNetwork n("FIT network");

    // -----------------------------------------------------------------------------------------------------------------

	n
		.addComputer(
		    CComputer("progtest.fit.cvut.cz")
				.addAddress("147.32.232.142")
				.addComponent(CCPU(8, 2400))
				.addComponent(CCPU(8, 1200))
				.addComponent(
					CDisk(CDisk::MAGNETIC, 1500)
						.addPartition(50, "/")
						.addPartition(5, "/boot")
						.addPartition(1000, "/var")
				)
				.addComponent(
					CDisk(CDisk::SSD, 60)
						.addPartition(60, "/data")
				)
				.addComponent(CMemory(2000))
				.addComponent(CMemory(2000))
		)
		.addComputer(
	        CComputer("courses.fit.cvut.cz")
				.addAddress("147.32.232.213")
				.addComponent(CCPU(4, 1600))
				.addComponent(CMemory(4000))
				.addComponent(
					CDisk(CDisk::MAGNETIC, 2000)
						.addPartition(100, "/")
						.addPartition(1900, "/data"))
		)
		.addComputer(
	        CComputer("imap.fit.cvut.cz")
				.addAddress("147.32.232.238")
				.addComponent(CCPU(4, 2500))
				.addAddress("2001:718:2:2901::238")
				.addComponent(CMemory(8000))
		);

	// -----------------------------------------------------------------------------------------------------------------

	assert(
		toString(n)
		==
		"Network: FIT network\n"
		"+-Host: progtest.fit.cvut.cz\n"
		"| +-147.32.232.142\n"
		"| +-CPU, 8 cores @ 2400MHz\n"
		"| +-CPU, 8 cores @ 1200MHz\n"
		"| +-HDD, 1500 GiB\n"
		"| | +-[0]: 50 GiB, /\n"
		"| | +-[1]: 5 GiB, /boot\n"
		"| | \\-[2]: 1000 GiB, /var\n"
		"| +-SSD, 60 GiB\n"
		"| | \\-[0]: 60 GiB, /data\n"
		"| +-Memory, 2000 MiB\n"
		"| \\-Memory, 2000 MiB\n"
		"+-Host: courses.fit.cvut.cz\n"
		"| +-147.32.232.213\n"
		"| +-CPU, 4 cores @ 1600MHz\n"
		"| +-Memory, 4000 MiB\n"
		"| \\-HDD, 2000 GiB\n"
		"|   +-[0]: 100 GiB, /\n"
		"|   \\-[1]: 1900 GiB, /data\n"
		"\\-Host: imap.fit.cvut.cz\n"
		"  +-147.32.232.238\n"
		"  +-2001:718:2:2901::238\n"
		"  +-CPU, 4 cores @ 2500MHz\n"
		"  \\-Memory, 8000 MiB\n"
	);

	// -----------------------------------------------------------------------------------------------------------------

	CNetwork x = n;

	// -----------------------------------------------------------------------------------------------------------------

	auto c = x.findComputer("imap.fit.cvut.cz");

	// -----------------------------------------------------------------------------------------------------------------

	assert(
		toString(*c)
		==
		"Host: imap.fit.cvut.cz\n"
		"+-147.32.232.238\n"
		"+-2001:718:2:2901::238\n"
		"+-CPU, 4 cores @ 2500MHz\n"
		"\\-Memory, 8000 MiB\n"
	);

	// -----------------------------------------------------------------------------------------------------------------

	c
		->addComponent(
			CDisk(CDisk::MAGNETIC, 1000)
				.addPartition(100, "system")
				.addPartition(200, "WWW")
				.addPartition(700, "mail")
		);

	// -----------------------------------------------------------------------------------------------------------------

	assert(
		toString(x)
		==
		"Network: FIT network\n"
		"+-Host: progtest.fit.cvut.cz\n"
		"| +-147.32.232.142\n"
		"| +-CPU, 8 cores @ 2400MHz\n"
		"| +-CPU, 8 cores @ 1200MHz\n"
		"| +-HDD, 1500 GiB\n"
		"| | +-[0]: 50 GiB, /\n"
		"| | +-[1]: 5 GiB, /boot\n"
		"| | \\-[2]: 1000 GiB, /var\n"
		"| +-SSD, 60 GiB\n"
		"| | \\-[0]: 60 GiB, /data\n"
		"| +-Memory, 2000 MiB\n"
		"| \\-Memory, 2000 MiB\n"
		"+-Host: courses.fit.cvut.cz\n"
		"| +-147.32.232.213\n"
		"| +-CPU, 4 cores @ 1600MHz\n"
		"| +-Memory, 4000 MiB\n"
		"| \\-HDD, 2000 GiB\n"
		"|   +-[0]: 100 GiB, /\n"
		"|   \\-[1]: 1900 GiB, /data\n"
		"\\-Host: imap.fit.cvut.cz\n"
		"  +-147.32.232.238\n"
		"  +-2001:718:2:2901::238\n"
		"  +-CPU, 4 cores @ 2500MHz\n"
		"  +-Memory, 8000 MiB\n"
		"  \\-HDD, 1000 GiB\n"
		"    +-[0]: 100 GiB, system\n"
		"    +-[1]: 200 GiB, WWW\n"
		"    \\-[2]: 700 GiB, mail\n"
	);

	// -----------------------------------------------------------------------------------------------------------------

	assert(
		toString(n)
		==
		"Network: FIT network\n"
		"+-Host: progtest.fit.cvut.cz\n"
		"| +-147.32.232.142\n"
		"| +-CPU, 8 cores @ 2400MHz\n"
		"| +-CPU, 8 cores @ 1200MHz\n"
		"| +-HDD, 1500 GiB\n"
		"| | +-[0]: 50 GiB, /\n"
		"| | +-[1]: 5 GiB, /boot\n"
		"| | \\-[2]: 1000 GiB, /var\n"
		"| +-SSD, 60 GiB\n"
		"| | \\-[0]: 60 GiB, /data\n"
		"| +-Memory, 2000 MiB\n"
		"| \\-Memory, 2000 MiB\n"
		"+-Host: courses.fit.cvut.cz\n"
		"| +-147.32.232.213\n"
		"| +-CPU, 4 cores @ 1600MHz\n"
		"| +-Memory, 4000 MiB\n"
		"| \\-HDD, 2000 GiB\n"
		"|   +-[0]: 100 GiB, /\n"
		"|   \\-[1]: 1900 GiB, /data\n"
		"\\-Host: imap.fit.cvut.cz\n"
		"  +-147.32.232.238\n"
		"  +-2001:718:2:2901::238\n"
		"  +-CPU, 4 cores @ 2500MHz\n"
		"  \\-Memory, 8000 MiB\n"
	);

	// -----------------------------------------------------------------------------------------------------------------

	return EXIT_SUCCESS;
}
