#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <climits>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <functional>
#include <iterator>
#include <compare>

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Class representing a date.
 */
class CDate {
public:
	/**
	 * @brief Constructor to initialize a date.
	 * @param y Year.
	 * @param m Month.
	 * @param d Day.
	 */
	CDate(int y, int m, int d)
			: m_Y(y), m_M(m), m_D(d) {}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Default three-way comparison operator for CDate.
	 */
	std::strong_ordering operator<=>(const CDate& rhs) const = default;

	/**
	 * @brief Stream output operator for CDate.
	 */
	friend std::ostream& operator<<(std::ostream& os, const CDate& d) {
		return os << d.m_Y << '-' << d.m_M << '-' << d.m_D;
	}

private:
	int m_Y, m_M, m_D;
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Enum class representing sorting keys.
 */
enum class ESortKey {NAME, BIRTH_DATE, ENROLL_YEAR};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Class representing a student.
 */
class CStudent {
public:
	/**
	 * @brief Constructor to initialize a student.
	 * @param name Name of the student.
	 * @param born Birth date of the student.
	 * @param enrolled Enrollment year of the student.
	 */
	CStudent(const std::string& name, const CDate& born, int enrolled)
			: name_(name), birth_date_(born), enroll_year_(enrolled), id_(next_id_++) {
		processName(name, processed_name_);
	}

	~CStudent() = default;

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Get the name of the student.
	 * @return Name of the student.
	 */
	[[nodiscard]] const std::string& getName() const {
		return name_;
	}

	/**
	 * @brief Get the processed name of the student.
	 * @return Processed name of the student.
	 */
	[[nodiscard]] const std::multiset<std::string>& getProcessedName() const {
		return processed_name_;
	}

	/**
	 * @brief Get the birth date of the student.
	 * @return Birth date of the student.
	 */
	[[nodiscard]] const CDate& getBirthDate() const {
		return birth_date_;
	}

	/**
	 * @brief Get the enrollment year of the student.
	 * @return Enrollment year of the student.
	 */
	[[nodiscard]] int getEnrollYear() const {
		return enroll_year_;
	}

	/**
	 * @brief Get the ID of the student.
	 * @return ID of the student.
	 */
	[[nodiscard]] size_t getId() const {
		return id_;
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Equality operator.
	 * @param s Another student to compare with.
	 * @return True if equal, false otherwise.
	 */
	bool operator==(const CStudent& s) const {
		return (name_ == s.name_ && birth_date_ == s.birth_date_ && enroll_year_ == s.enroll_year_);
	}

	/**
	 * @brief Inequality operator.
	 * @param s Another student to compare with.
	 * @return True if not equal, false otherwise.
	 */
	bool operator!=(const CStudent& s) const {
		return !(*this == s);
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Process the name by converting to uppercase and splitting by spaces.
	 * @param src_name Original name.
	 * @param dst_processed_name Processed name.
	 */
	static void processName(const std::string& src_name, std::multiset<std::string>& dst_processed_name) {
		auto name_part_start_pos = src_name.begin(), name_part_end_pos = src_name.begin();

		while (name_part_end_pos != src_name.end()) {
			name_part_start_pos = std::find_if_not(name_part_end_pos, src_name.end(), ::isspace);
			name_part_end_pos = std::find_if(name_part_start_pos, src_name.end(), ::isspace);

			if (name_part_start_pos != name_part_end_pos) {
				std::string name_part(name_part_start_pos, name_part_end_pos);

				std::transform(name_part.begin(), name_part.end(), name_part.begin(),
				               [](auto c) { return std::toupper(c); });
				dst_processed_name.insert(name_part);
			}
		}
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Compare students by name.
	 * @param lhs Left-hand side student.
	 * @param rhs Right-hand side student.
	 * @return True if lhs name is less than rhs name.
	 */
	[[nodiscard]] static bool compareByName(const CStudent& lhs, const CStudent& rhs) {
		return lhs.getName() < rhs.getName();
	}

	/**
	 * @brief Compare students by processed name.
	 * @param lhs Left-hand side student.
	 * @param rhs Right-hand side student.
	 * @return True if lhs processed name is less than rhs processed name.
	 */
	[[nodiscard]] static bool compareByProcessedName(const CStudent& lhs, const CStudent& rhs) {
		return lhs.getProcessedName() < rhs.getProcessedName();
	}

	/**
	 * @brief Compare students by birth date.
	 * @param lhs Left-hand side student.
	 * @param rhs Right-hand side student.
	 * @return True if lhs birth date is less than rhs birth date.
	 */
	[[nodiscard]] static bool compareByBirthDate(const CStudent& lhs, const CStudent& rhs) {
		return lhs.getBirthDate() < rhs.getBirthDate();
	}

	/**
	 * @brief Compare students by enrollment year.
	 * @param lhs Left-hand side student.
	 * @param rhs Right-hand side student.
	 * @return True if lhs enrollment year is less than rhs enrollment year.
	 */
	[[nodiscard]] static bool compareByEnrollYear(const CStudent& lhs, const CStudent& rhs) {
		return lhs.getEnrollYear() < rhs.getEnrollYear();
	}

	/**
	 * @brief Compare students by ID.
	 * @param lhs Left-hand side student.
	 * @param rhs Right-hand side student.
	 * @return True if lhs ID is less than rhs ID.
	 */
	[[nodiscard]] static bool compareById(const CStudent& lhs, const CStudent& rhs) {
		return lhs.getId() < rhs.getId();
	}

private:
	std::string name_;
	std::multiset<std::string> processed_name_;
	CDate birth_date_;
	int enroll_year_;
	size_t id_;

	// -----------------------------------------------------------------------------------------------------------------

	static inline size_t next_id_ = 0;
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Comparator for students based on name, birth date, and enrollment year.
 */
struct CStudentComparator {
	bool operator()(const std::shared_ptr<CStudent>& lhs, const std::shared_ptr<CStudent>& rhs) const {
		if (lhs->getName() != rhs->getName())
			return CStudent::compareByName(*lhs, *rhs);
		else if (lhs->getBirthDate() != rhs->getBirthDate())
			return CStudent::compareByBirthDate(*lhs, *rhs);
		else
			return CStudent::compareByEnrollYear(*lhs, *rhs);
	}
};

// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Comparator for students based on processed name.
 */
struct CStudentProcessedNameComparator {
	using is_transparent = void;

	// -----------------------------------------------------------------------------------------------------------------

	bool operator()(const std::shared_ptr<CStudent>& lhs, const std::shared_ptr<CStudent>& rhs) const {
		if (lhs->getProcessedName() != rhs->getProcessedName())
			return CStudent::compareByProcessedName(*lhs, *rhs);
		else
			return CStudent::compareById(*lhs, *rhs);
	}

	// -----------------------------------------------------------------------------------------------------------------

	bool operator()(const std::shared_ptr<CStudent>& lhs, const std::multiset<std::string>& rhs_processed_name) const {
		return lhs->getProcessedName() < rhs_processed_name;
	}

	bool operator()(const std::multiset<std::string>& lhs_processed_name, const std::shared_ptr<CStudent>& rhs) const {
		return lhs_processed_name < rhs->getProcessedName();
	}
};

// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Comparator for students based on birth date.
 */
struct CStudentBirthDateComparator {
	using is_transparent = void;

	// -----------------------------------------------------------------------------------------------------------------

	bool operator()(const std::shared_ptr<CStudent>& lhs, const std::shared_ptr<CStudent>& rhs) const {
		if (lhs->getBirthDate() != rhs->getBirthDate())
			return CStudent::compareByBirthDate(*lhs, *rhs);
		else
			return CStudent::compareById(*lhs, *rhs);
	}

	// -----------------------------------------------------------------------------------------------------------------

	bool operator()(const std::shared_ptr<CStudent>& lhs, const CDate& rhs_birth_date) const {
		return lhs->getBirthDate() < rhs_birth_date;
	}

	bool operator()(const CDate& lhs_birth_date, const std::shared_ptr<CStudent>& rhs) const {
		return lhs_birth_date < rhs->getBirthDate();
	}
};

// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Comparator for students based on enrollment year.
 */
struct CStudentEnrollYearComparator {
	using is_transparent = void;

	// -----------------------------------------------------------------------------------------------------------------

	bool operator()(const std::shared_ptr<CStudent>& lhs, const std::shared_ptr<CStudent>& rhs) const {
		if (lhs->getEnrollYear() != rhs->getEnrollYear())
			return CStudent::compareByEnrollYear(*lhs, *rhs);
		else
			return CStudent::compareById(*lhs, *rhs);
	}

	// -----------------------------------------------------------------------------------------------------------------

	bool operator()(const std::shared_ptr<CStudent>& lhs, int rhs_enroll_year) const {
		return lhs->getEnrollYear() < rhs_enroll_year;
	}

	bool operator()(int lhs_enroll_year, const std::shared_ptr<CStudent>& rhs) const {
		return lhs_enroll_year < rhs->getEnrollYear();
	}
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Class to filter students based on different criteria.
 */
class CFilter {
public:
	/*
	 * @brief Constructor to initialize the filter.
	 */
	CFilter()
			: filtered_students_(),
			  is_configured_(false), is_processed_names_(false),
			  is_lower_birth_date_(false), is_upper_birth_date_(false),
			  is_lower_enroll_year_(false), is_upper_enroll_year_(false),
			  processed_names_(),
			  lower_birth_date_(0, 0, 0), upper_birth_date_(0, 0, 0),
			  lower_enroll_year_(), upper_enroll_year_() {}

	~CFilter() = default;

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Set the filter to search by name.
	 * @param name Name to search for.
	 * @return Reference to the filter.
	 */
	CFilter& name(const std::string& name) {
		updateProcessedNames_(name);
		is_processed_names_ = true;
		is_configured_ = true;

		return *this;
	}

	/**
	 * @brief Set the filter to search for students born after a specific date.
	 * @param date Date to search after.
	 * @return Reference to the filter.
	 */
	CFilter& bornAfter(const CDate& date) {
		lower_birth_date_ = date;
		is_lower_birth_date_ = true;
		is_configured_ = true;

		return *this;
	}

	/**
	 * @brief Set the filter to search for students born before a specific date.
	 * @param date Date to search before.
	 * @return Reference to the filter.
	 */
	CFilter& bornBefore(const CDate& date) {
		upper_birth_date_ = date;
		is_upper_birth_date_ = true;
		is_configured_ = true;

		return *this;
	}

	/**
	 * @brief Set the filter to search for students enrolled after a specific year.
	 * @param year Year to search after.
	 * @return Reference to the filter.
	 */
	CFilter& enrolledAfter(int year) {
		lower_enroll_year_ = year;
		is_lower_enroll_year_ = true;
		is_configured_ = true;

		return *this;
	}

	/**
	 * @brief Set the filter to search for students enrolled before a specific year.
	 * @param year Year to search before.
	 * @return Reference to the filter.
	 */
	CFilter& enrolledBefore(int year) {
		upper_enroll_year_ = year;
		is_upper_enroll_year_ = true;
		is_configured_ = true;

		return *this;
	}

	/**
	 * @brief Set the default students to filter from.
	 * @param students Set of students to filter.
	 * @return Reference to the filter.
	 */
	CFilter& defaultStudents(const std::set<std::shared_ptr<CStudent>, CStudentComparator>& students) {
		if (!is_configured_)
			filtered_students_ = students;

		return *this;
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Apply the filter to the given sets of students.
	 * @param students_by_processed_name Set of students by processed name.
	 * @param students_by_birth_date Set of students by birth date.
	 * @param students_by_enroll_year Set of students by enrollment year.
	 * @return Reference to the filter.
	 */
	CFilter& filter(const std::set<std::shared_ptr<CStudent>, CStudentProcessedNameComparator>& students_by_processed_name,
					const std::set<std::shared_ptr<CStudent>, CStudentBirthDateComparator>& students_by_birth_date,
					const std::set<std::shared_ptr<CStudent>, CStudentEnrollYearComparator>& students_by_enroll_year) {
		if (is_configured_)
			applyFiltering_(students_by_processed_name, students_by_birth_date, students_by_enroll_year);

		return *this;
	}

	/**
	 * @brief Get the filtered students.
	 * @return List of filtered students.
	 */
	[[nodiscard]] std::list<std::shared_ptr<CStudent>> filteredStudents() const {
		return {filtered_students_.cbegin(), filtered_students_.cend()};
	}

private:
	std::set<std::shared_ptr<CStudent>, CStudentComparator> filtered_students_;

	// -----------------------------------------------------------------------------------------------------------------

	bool is_configured_, is_processed_names_,
		 is_lower_birth_date_, is_upper_birth_date_, is_lower_enroll_year_, is_upper_enroll_year_;

	// -----------------------------------------------------------------------------------------------------------------

	std::list<std::multiset<std::string>> processed_names_;
	CDate lower_birth_date_, upper_birth_date_;
	int lower_enroll_year_, upper_enroll_year_;

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Update the processed names list.
	 * @param name Name to process and add to the list.
	 */
	void updateProcessedNames_(const std::string& name) {
		std::multiset<std::string> processed_name;

		CStudent::processName(name, processed_name);

		auto processed_name_pos =
				std::find_if(processed_names_.begin(), processed_names_.end(),
							 [processed_name](const auto& pn) { return pn == processed_name; });

		if (processed_name_pos == processed_names_.end())
			processed_names_.push_back(processed_name);
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Apply the filtering to the sets of students.
	 * @param students_by_processed_name Set of students by processed name.
	 * @param students_by_birth_date Set of students by birth date.
	 * @param students_by_enroll_year Set of students by enrollment year.
	 */
	void applyFiltering_(const std::set<std::shared_ptr<CStudent>, CStudentProcessedNameComparator>& students_by_processed_name,
	                     const std::set<std::shared_ptr<CStudent>, CStudentBirthDateComparator>& students_by_birth_date,
	                     const std::set<std::shared_ptr<CStudent>, CStudentEnrollYearComparator>& students_by_enroll_year) {
		if (!((is_lower_birth_date_ && is_upper_birth_date_ && (lower_birth_date_ >= upper_birth_date_)) ||
				(is_lower_enroll_year_ && is_upper_enroll_year_ && (lower_enroll_year_ >= upper_enroll_year_)) ||
				(is_lower_enroll_year_ && lower_enroll_year_ < 0) || (is_upper_enroll_year_ && upper_enroll_year_ < 0))) {
			if (is_processed_names_)
				for (const auto& processed_name: processed_names_) {
					auto start_pos = students_by_processed_name.lower_bound(processed_name),
						 end_pos = students_by_processed_name.upper_bound(processed_name);

					for (auto student_pos = start_pos; student_pos != end_pos; ++student_pos) {
						if ((is_lower_birth_date_ && (*student_pos)->getBirthDate() <= lower_birth_date_) ||
								(is_upper_birth_date_ && (*student_pos)->getBirthDate() >= upper_birth_date_) ||
						            (is_lower_enroll_year_ && (*student_pos)->getEnrollYear() <= lower_enroll_year_) ||
						                (is_upper_enroll_year_ && (*student_pos)->getEnrollYear() >= upper_enroll_year_))
							continue;

						filtered_students_.insert(*student_pos);
					}
				}
			else {
				auto start_birth_date_pos = (is_lower_birth_date_ ?
				                             students_by_birth_date.lower_bound(lower_birth_date_) : students_by_birth_date.begin()),
						end_birth_date_pos = (is_upper_birth_date_ ?
						                      students_by_birth_date.upper_bound(upper_birth_date_) : students_by_birth_date.end()),
						start_enroll_year_pos = (is_lower_enroll_year_ ?
						                         students_by_enroll_year.lower_bound(lower_enroll_year_) : students_by_enroll_year.begin()),
						end_enroll_year_pos = (is_upper_enroll_year_ ?
						                       students_by_enroll_year.upper_bound(upper_enroll_year_) : students_by_enroll_year.end());

				size_t birth_date_count = std::distance(start_birth_date_pos, end_birth_date_pos),
					   enroll_year_count = std::distance(start_enroll_year_pos, end_enroll_year_pos);

				for (auto student_pos = (birth_date_count < enroll_year_count ? start_birth_date_pos : start_enroll_year_pos);
						student_pos != (birth_date_count < enroll_year_count ? end_birth_date_pos : end_enroll_year_pos); ++student_pos)
					if (!((is_lower_enroll_year_ && (*student_pos)->getEnrollYear() <= lower_enroll_year_) ||
						    (is_upper_enroll_year_ && (*student_pos)->getEnrollYear() >= upper_enroll_year_) ||
								(is_lower_birth_date_ && (*student_pos)->getBirthDate() <= lower_birth_date_) ||
									(is_upper_birth_date_ && (*student_pos)->getBirthDate() >= upper_birth_date_)))
							filtered_students_.insert(*student_pos);
			}
		}
	}
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/*
 * @brief Constructor to initialize the sorter.
*/
class CSort {
public:
	CSort()
			: sorted_students_(), is_configured_(false), keys_() {}

	~CSort() = default;

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Add a sorting key.
	 * @param key Sorting key.
	 * @param ascending True for ascending order, false for descending order.
	 * @return Reference to the sorter.
	 */
	CSort& addKey(ESortKey key, bool ascending) {
		updateKeys_(key, ascending);
		is_configured_ = true;

		return *this;
	}

	/**
	 * @brief Set the default students to sort from.
	 * @param students List of students to sort.
	 * @return Reference to the sorter.
	 */
	CSort& defaultStudents(const std::list<std::shared_ptr<CStudent>>& students) {
		sorted_students_ = students;

		return *this;
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Apply the sorting.
	 * @return Reference to the sorter.
	 */
	CSort& sort() {
		if (is_configured_)
			applyKeySorting_();
		else
			applyIdSorting_();

		return *this;
	}

	/**
	 * @brief Get the sorted students.
	 * @return List of sorted students.
	 */
	[[nodiscard]] std::list<std::shared_ptr<CStudent>> sortedStudents() const {
		return {sorted_students_.cbegin(), sorted_students_.cend()};
	}

private:
	std::list<std::shared_ptr<CStudent>> sorted_students_;

	// -----------------------------------------------------------------------------------------------------------------

	bool is_configured_;

	// -----------------------------------------------------------------------------------------------------------------

	std::list<std::pair<ESortKey, bool>> keys_;

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Update the sorting keys.
	 * @param key Sorting key.
	 * @param ascending True for ascending order, false for descending order.
	 */
	void updateKeys_(ESortKey key, bool ascending) {
		auto key_pos = std::find_if(keys_.begin(), keys_.end(),
									[key](const auto& k) { return k.first == key; });

		if (key_pos != keys_.end())
			keys_.erase(key_pos);

		keys_.emplace_back(key, ascending);
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Apply sorting based on the keys.
	 */
	void applyKeySorting_() {
		sorted_students_.sort([this](const auto& lhs, const auto& rhs) {
			for (const auto& [key, ascending]: keys_) {
				bool is_lesser = false, is_greater = false;

				if (key == ESortKey::NAME) {
					is_lesser = CStudent::compareByName(*lhs, *rhs);
					is_greater = CStudent::compareByName(*rhs, *lhs);
				} else if (key == ESortKey::BIRTH_DATE) {
					is_lesser = CStudent::compareByBirthDate(*lhs, *rhs);
					is_greater = CStudent::compareByBirthDate(*rhs, *lhs);
				} else if (key == ESortKey::ENROLL_YEAR) {
					is_lesser = CStudent::compareByEnrollYear(*lhs, *rhs);
					is_greater = CStudent::compareByEnrollYear(*rhs, *lhs);
				}

				if (is_lesser != is_greater)
					return (ascending ? is_lesser : is_greater);
			}

			return CStudent::compareById(*lhs, *rhs);
		});
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Apply sorting based on IDs.
	 */
	void applyIdSorting_() {
		sorted_students_.sort([](const auto& lhs, const auto& rhs)
			{ return CStudent::compareById(*lhs, *rhs); });
	}
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Class representing the study department.
 */
class CStudyDept {
public:
	CStudyDept() = default;

	~CStudyDept() = default;

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Add a student to the department.
	 * @param s Student to add.
	 * @return True if the student is added, false otherwise.
	 */
	bool addStudent(const CStudent& s) {
		auto new_student = std::make_shared<CStudent>(s);

		if (findStudent_(new_student))
			return false;

		applyStudentAdding_(new_student);

		return true;
	}

	/**
	 * @brief Delete a student from the department.
	 * @param s Student to delete.
	 * @return True if the student is deleted, false otherwise.
	 */
	bool delStudent(const CStudent& s) {
		auto existing_student = std::make_shared<CStudent>(s);

		if (existing_student = findStudent_(existing_student); !existing_student)
			return false;

		applyStudentDeleting_(existing_student);

		return true;
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Search for students based on a filter and sorter.
	 * @param filter Filter to apply.
	 * @param sorter Sorter to apply.
	 * @return List of searched students.
	 */
	std::list<CStudent> search(const CFilter& filter, const CSort& sorter) const {
		std::list<CStudent> searched_students;

		applyStudentSearching_(filter, sorter, searched_students);

		return searched_students;
	}

	/**
	 * @brief Suggest student names based on a partial name.
	 * @param name Partial name to suggest from.
	 * @return Set of suggested names.
	 */
	std::set<std::string> suggest(const std::string& name) const {
		std::set<std::string> suggested_names;

		applyStudentNamesSuggesting_(name, suggested_names);

		return suggested_names;
	}

private:
	std::set<std::shared_ptr<CStudent>, CStudentComparator> students_;

	// -----------------------------------------------------------------------------------------------------------------

	std::set<std::shared_ptr<CStudent>, CStudentProcessedNameComparator> students_by_processed_name_;
	std::set<std::shared_ptr<CStudent>, CStudentBirthDateComparator> students_by_birth_date_;
	std::set<std::shared_ptr<CStudent>, CStudentEnrollYearComparator> students_by_enroll_year_;

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Find a student in the department.
	 * @param s Student to find.
	 * @return Shared pointer to the found student, or nullptr if not found.
	 */
	std::shared_ptr<CStudent> findStudent_(const std::shared_ptr<CStudent>& s) const {
		auto student_pos = students_.find(s);

		return (student_pos != students_.cend() ? *student_pos : nullptr);
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Add a student to the department.
	 * @param new_student Shared pointer to the new student.
	 */
	void applyStudentAdding_(const std::shared_ptr<CStudent>& new_student) {
		students_.insert(new_student);
		students_by_processed_name_.insert(new_student);
		students_by_birth_date_.insert(new_student);
		students_by_enroll_year_.insert(new_student);
	}

	/**
	 * @brief Delete a student from the department.
	 * @param existing_student Shared pointer to the existing student.
	 */
	void applyStudentDeleting_(const std::shared_ptr<CStudent>& existing_student) {
		students_.erase(existing_student);
		students_by_processed_name_.erase(existing_student);
		students_by_birth_date_.erase(existing_student);
		students_by_enroll_year_.erase(existing_student);
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Apply searching for students based on a filter and sorter.
	 * @param src_filter Filter to apply.
	 * @param src_sorter Sorter to apply.
	 * @param dst_searched_students List of searched students.
	 */
	void applyStudentSearching_(CFilter src_filter, CSort src_sorter, std::list<CStudent>& dst_searched_students) const {
		std::list<std::shared_ptr<CStudent>> searched_raw_students =
			src_sorter
				.defaultStudents(
					src_filter
						.defaultStudents(students_)
						.filter(students_by_processed_name_, students_by_birth_date_, students_by_enroll_year_)
						.filteredStudents()
				)
				.sort()
				.sortedStudents();

		std::transform(searched_raw_students.cbegin(), searched_raw_students.cend(),
		               std::back_inserter(dst_searched_students), [](const auto& srs) { return *srs; });

	}

	/**
	 * @brief Suggest student names based on a partial name.
	 * @param src_name Partial name to suggest from.
	 * @param dst_suggested_names Set of suggested names.
	 */
	void applyStudentNamesSuggesting_(const std::string& src_name, std::set<std::string>& dst_suggested_names) const {
		std::multiset<std::string> processed_name;
		std::set<std::string> unique_processed_name;

		CStudent::processName(src_name, processed_name);
		unique_processed_name.insert(processed_name.begin(), processed_name.end());

		for (const auto& student : students_)
			if (std::includes(student->getProcessedName().begin(), student->getProcessedName().end(),
			                  unique_processed_name.begin(), unique_processed_name.end()))
				dst_suggested_names.insert(student->getName());
	}
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

int main() {
	CStudyDept x0;

	// -----------------------------------------------------------------------------------------------------------------

	assert(
		CStudent("James Bond", CDate(1980, 4, 11), 2010)
		==
		CStudent("James Bond", CDate(1980, 4, 11), 2010)
	);

	assert(!(
		CStudent("James Bond", CDate(1980, 4, 11), 2010)
		!=
		CStudent("James Bond", CDate(1980, 4, 11), 2010)
	));

	assert(
		CStudent("James Bond", CDate(1980, 4, 11), 2010)
		!=
		CStudent("Peter Peterson", CDate(1980, 4, 11), 2010)
	);

	assert(!(
		CStudent("James Bond", CDate(1980, 4, 11), 2010)
		==
		CStudent("Peter Peterson", CDate(1980, 4, 11), 2010)
	));

	assert(
		CStudent("James Bond", CDate(1980, 4, 11), 2010)
		!=
		CStudent("James Bond", CDate(1997, 6, 17), 2010)
	);

	assert(!(
		CStudent("James Bond", CDate(1980, 4, 11), 2010)
		==
		CStudent("James Bond", CDate(1997, 6, 17), 2010)
	));

	assert(
		CStudent("James Bond", CDate(1980, 4, 11), 2010)
		!=
		CStudent("James Bond", CDate(1980, 4, 11), 2016)
	);

	assert(!(
		CStudent("James Bond", CDate(1980, 4, 11), 2010)
		==
		CStudent("James Bond", CDate(1980, 4, 11), 2016)
	));

	assert(
		CStudent("James Bond", CDate(1980, 4, 11), 2010)
		!=
		CStudent("Peter Peterson", CDate(1980, 4, 11), 2016)
	);

	assert(!(
		CStudent("James Bond", CDate(1980, 4, 11), 2010)
		==
		CStudent("Peter Peterson", CDate(1980, 4, 11), 2016)
	));

	assert(
		CStudent("James Bond", CDate(1980, 4, 11), 2010)
		!=
		CStudent("Peter Peterson", CDate(1997, 6, 17), 2010)
	);

	assert(!(
		CStudent("James Bond", CDate(1980, 4, 11), 2010)
		==
		CStudent("Peter Peterson", CDate(1997, 6, 17), 2010)
	));

	assert(
		CStudent("James Bond", CDate(1980, 4, 11), 2010)
		!=
		CStudent("James Bond", CDate(1997, 6, 17), 2016)
	);

	assert(!(
		CStudent("James Bond", CDate(1980, 4, 11), 2010)
		==
		CStudent("James Bond", CDate(1997, 6, 17), 2016)
	));

	assert(
		CStudent("James Bond", CDate(1980, 4, 11), 2010)
		!=
		CStudent("Peter Peterson", CDate(1997, 6, 17), 2016)
	);

	assert(!(
		CStudent("James Bond", CDate(1980, 4, 11), 2010)
		==
		CStudent("Peter Peterson", CDate(1997, 6, 17), 2016)
	));

	// -----------------------------------------------------------------------------------------------------------------

	assert(x0.addStudent(CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014)));

	assert(x0.addStudent(CStudent("John Taylor", CDate(1981, 6, 30), 2012)));

	assert(x0.addStudent(CStudent("Peter Taylor", CDate(1982, 2, 23), 2011)));

	assert(x0.addStudent(CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017)));

	assert(x0.addStudent(CStudent("James Bond", CDate(1981, 7, 16), 2013)));

	assert(x0.addStudent(CStudent("James Bond", CDate(1982, 7, 16), 2013)));

	assert(x0.addStudent(CStudent("James Bond", CDate(1981, 8, 16), 2013)));

	assert(x0.addStudent(CStudent("James Bond", CDate(1981, 7, 17), 2013)));

	assert(x0.addStudent(CStudent("James Bond", CDate(1981, 7, 16), 2012)));

	assert(x0.addStudent(CStudent("Bond James", CDate(1981, 7, 16), 2013)));

	// -----------------------------------------------------------------------------------------------------------------

	assert(
		x0
		  .search(
			CFilter(),
			CSort()
		  )
		==
		(std::list<CStudent> {
	             CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014),
	             CStudent("John Taylor", CDate(1981, 6, 30), 2012),
	             CStudent("Peter Taylor", CDate(1982, 2, 23), 2011),
	             CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017),
	             CStudent("James Bond", CDate(1981, 7, 16), 2013),
	             CStudent("James Bond", CDate(1982, 7, 16), 2013),
	             CStudent("James Bond", CDate(1981, 8, 16), 2013),
	             CStudent("James Bond", CDate(1981, 7, 17), 2013),
	             CStudent("James Bond", CDate(1981, 7, 16), 2012),
	             CStudent("Bond James", CDate(1981, 7, 16), 2013)
             }
		)
	);

	assert(
		x0
		  .search(
			CFilter(),
			CSort()
				.addKey(ESortKey::NAME, true)
		  )
		==
		(std::list<CStudent> {
				CStudent("Bond James", CDate(1981, 7, 16), 2013),
				CStudent("James Bond", CDate(1981, 7, 16), 2013),
				CStudent("James Bond", CDate(1982, 7, 16), 2013),
				CStudent("James Bond", CDate(1981, 8, 16), 2013),
				CStudent("James Bond", CDate(1981, 7, 17), 2013),
				CStudent("James Bond", CDate(1981, 7, 16), 2012),
				CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014),
				CStudent("John Taylor", CDate(1981, 6, 30), 2012),
				CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017),
				CStudent("Peter Taylor", CDate(1982, 2, 23), 2011)
	        }
		)
	);

	assert(
		x0
		  .search(
			CFilter(),
			CSort()
				.addKey(ESortKey::NAME, false)
		  )
		==
		(std::list<CStudent> {
				CStudent("Peter Taylor", CDate(1982, 2, 23), 2011),
				CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017),
				CStudent("John Taylor", CDate(1981, 6, 30), 2012),
				CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014),
				CStudent("James Bond", CDate(1981, 7, 16), 2013),
				CStudent("James Bond", CDate(1982, 7, 16), 2013),
				CStudent("James Bond", CDate(1981, 8, 16), 2013),
				CStudent("James Bond", CDate(1981, 7, 17), 2013),
				CStudent("James Bond", CDate(1981, 7, 16), 2012),
				CStudent("Bond James", CDate(1981, 7, 16), 2013)
	        }
		)
	);

	// -----------------------------------------------------------------------------------------------------------------

	assert(
		x0
		  .search(
			CFilter(),
			CSort()
				.addKey(ESortKey::ENROLL_YEAR, false)
				.addKey(ESortKey::BIRTH_DATE, false)
		        .addKey(ESortKey::NAME, true)
		  )
		==
		(std::list<CStudent> {
				CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017),
				CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014),
				CStudent("James Bond", CDate(1982, 7, 16), 2013),
				CStudent("James Bond", CDate(1981, 8, 16), 2013),
				CStudent("James Bond", CDate(1981, 7, 17), 2013),
				CStudent("Bond James", CDate(1981, 7, 16), 2013),
				CStudent("James Bond", CDate(1981, 7, 16), 2013),
				CStudent("James Bond", CDate(1981, 7, 16), 2012),
				CStudent("John Taylor", CDate(1981, 6, 30), 2012),
				CStudent("Peter Taylor", CDate(1982, 2, 23), 2011)
			}
		)
	);

	assert(
		x0
		  .search(
			CFilter()
				.name("james bond"),
			CSort()
				.addKey(ESortKey::ENROLL_YEAR, false)
		        .addKey(ESortKey::BIRTH_DATE, false)
				.addKey(ESortKey::NAME, true)
		  )
		==
		(std::list<CStudent> {
				CStudent("James Bond", CDate(1982, 7, 16), 2013),
				CStudent("James Bond", CDate(1981, 8, 16), 2013),
				CStudent("James Bond", CDate(1981, 7, 17), 2013),
				CStudent("Bond James", CDate(1981, 7, 16), 2013),
				CStudent("James Bond", CDate(1981, 7, 16), 2013),
				CStudent("James Bond", CDate(1981, 7, 16), 2012)
			}
		)
	);

	assert(
		x0
		  .search(
			CFilter()
			    .bornAfter(CDate(1980, 4, 11))
		        .bornBefore(CDate(1983, 7, 13))
		        .name("John Taylor")
		        .name("james BOND"),
			CSort()
				.addKey(ESortKey::ENROLL_YEAR, false)
		        .addKey(ESortKey::BIRTH_DATE, false)
		        .addKey(ESortKey::NAME, true)
		  )
		==
		(std::list<CStudent> {
				CStudent("James Bond", CDate(1982, 7, 16), 2013),
				CStudent("James Bond", CDate(1981, 8, 16), 2013),
				CStudent("James Bond", CDate(1981, 7, 17), 2013),
				CStudent("Bond James", CDate(1981, 7, 16), 2013),
				CStudent("James Bond", CDate(1981, 7, 16), 2013),
				CStudent("James Bond", CDate(1981, 7, 16), 2012),
				CStudent("John Taylor", CDate(1981, 6, 30), 2012)
			}
		)
	);

	assert(
		x0
		  .search(
			CFilter()
				.name("james"),
			CSort()
				.addKey(ESortKey::NAME, true)
		  )
		==
		(std::list<CStudent>{})
	);

	// -----------------------------------------------------------------------------------------------------------------

	assert(
		x0.suggest("peter")
		==
		(std::set<std::string> {
				"John Peter Taylor",
				"Peter John Taylor",
				"Peter Taylor"
			}
		)
	);

	assert(
		x0.suggest("bond")
		==
		(std::set<std::string> {
				"Bond James",
				"James Bond"
			}
		)
	);

	assert(
		x0.suggest("peter joHn")
		==
		(std::set<std::string> {
				"John Peter Taylor",
				"Peter John Taylor"
			}
		)
	);

	assert(
		x0.suggest("peter joHn bond")
		==
		(std::set<std::string>{})
	);

	assert(
		x0.suggest("pete")
		==
		(std::set<std::string>{})
	);

	assert(
		x0.suggest("peter joHn PETER")
		==
		(std::set<std::string> {
				"John Peter Taylor",
				"Peter John Taylor"
			}
		)
	);

	// -----------------------------------------------------------------------------------------------------------------

	assert(!x0.addStudent(CStudent("James Bond", CDate(1981, 7, 16), 2013)));

	assert(x0.delStudent(CStudent("James Bond", CDate(1981, 7, 16), 2013)));

	// -----------------------------------------------------------------------------------------------------------------

	assert(
		x0
		  .search(
			CFilter()
				.bornAfter(CDate(1980, 4, 11))
				.bornBefore(CDate(1983, 7, 13))
				.name("John Taylor")
				.name("james BOND"),
			CSort()
				.addKey(ESortKey::ENROLL_YEAR, false)
				.addKey(ESortKey::BIRTH_DATE, false)
				.addKey(ESortKey::NAME, true)
		  )
	    ==
		(std::list<CStudent> {
				CStudent("James Bond", CDate(1982, 7, 16), 2013),
				CStudent("James Bond", CDate(1981, 8, 16), 2013),
				CStudent("James Bond", CDate(1981, 7, 17), 2013),
				CStudent("Bond James", CDate(1981, 7, 16), 2013),
				CStudent("James Bond", CDate(1981, 7, 16), 2012),
				CStudent("John Taylor", CDate(1981, 6, 30), 2012)
			}
		)
	);

	// -----------------------------------------------------------------------------------------------------------------

	assert(!x0.delStudent(CStudent("James Bond", CDate(1981, 7, 16), 2013)));

	// -----------------------------------------------------------------------------------------------------------------

	return EXIT_SUCCESS;
}
