#ifndef __PROGTEST__

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <climits>
#include <cfloat>
#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <array>
#include <utility>
#include <utility>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <functional>
#include <iterator>
#include <stdexcept>
#include <variant>
#include <optional>
#include <compare>
#include <charconv>
#include <span>
#include <utility>

// ---------------------------------------------------------------------------------------------------------------------

#include "expression.h"

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

using namespace std::literals;

using CValue = std::variant<std::monostate, double, std::string>;

// ---------------------------------------------------------------------------------------------------------------------

constexpr unsigned SPREADSHEET_CYCLIC_DEPS = 0x01,
		SPREADSHEET_FUNCTIONS = 0x02,
		SPREADSHEET_FILE_IO = 0x04,
		SPREADSHEET_SPEED = 0x08,
		SPREADSHEET_PARSER = 0x10;

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

#endif /* __PROGTEST__ */

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief
 * Class to represent and manipulate spreadsheet cell positions.
 *
 * Encapsulates a cell position with a column and row identifier, handling parsing, validation, and serialization.
 * Provides functionality for position adjustments and supports comparison for sorting or map storage.
 */
class CPos {
public:
	using CPosID = int; // Type for storing column and row indices internally.

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Constructs a CPos object by parsing a string identifier of a cell.
     *
     * The identifier must be in a valid format (e.g., "A1"), combining column letters followed by a row number.
     *
     * @param str - string_view representing the cell identifier.
     * @throw std::invalid_argument if the identifier format is invalid.
     */
	explicit CPos(std::string_view str) {
		if (!validate_(str))
			throw std::invalid_argument("Invalid argument: identifier failed validation!");

		parse_(str);
	}

	/**
     * @brief
     * Constructs a CPos object from a string identifier and applies a shift in position.
     *
     * Allows creating a new position shifted from the original by a specified number of columns and rows.
     *
     * @param str - string_view representing the base cell identifier.
     * @param shift - pair of integers representing the column and row shift respectively.
     */
	CPos(std::string_view str, std::pair<int, int> shift) {
		parse_(str);

		pos_data_ = {pos_data_.first + shift.first, pos_data_.second + shift.second};
	}

	/**
     * @brief
     * Default copy constructor.
     *
     * @param other - another CPos object to be copied.
     */
	CPos(const CPos& other) = default;

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Compares two positions for equality.
     *
     * @param lhs - left-hand side CPos object.
     * @param rhs - right-hand side CPos object.
     * @return true if both positions are the same, false otherwise.
     */
	friend bool operator==(const CPos& lhs, const CPos& rhs) {
		return lhs.pos_data_ == rhs.pos_data_;
	}

	/**
     * @brief
     * Determines the order between two positions.
     *
     * Useful for storing CPos objects in sorted containers. Orders first by column and then by row.
     *
     * @param lhs - left-hand side CPos object.
     * @param rhs - right-hand side CPos object.
     * @return true if lhs is less than rhs, false otherwise.
     */
	friend bool operator<(const CPos& lhs, const CPos& rhs) {
		if (lhs.pos_data_.first != rhs.pos_data_.first)
			return lhs.pos_data_.first < rhs.pos_data_.first;

		return lhs.pos_data_.second < rhs.pos_data_.second;
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Serializes the position to its string representation (e.g., "A1").
     *
     * @return std::string representing the cell position.
     */
	std::string serialize() const {
		return serializeColID_() + serializeRowID_();
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Retrieves both column and row identifiers as strings.
     *
     * @return pair of strings where the first is the column and the second is the row identifier.
     */
	std::pair<std::string, std::string> IDs() const {
		return {serializeColID_(), serializeRowID_()};
	}

	/**
     * @brief
     * Retrieves numerical equivalents of the column and row identifiers.
     *
     * @return pair of CPosID where the first is the column and the second is the row index.
     */
	std::pair<CPosID, CPosID> numerizedIDs() const {
		return pos_data_;
	}

private:
	/**
     * @brief
     * Enum class to represent the state of the scanning process during validation and parsing of cell identifiers.
     */
	enum class CPosScanState { INIT, COL, ROW };

	// -----------------------------------------------------------------------------------------------------------------

	std::pair<CPosID, CPosID> pos_data_; // Stores parsed column and row indices.

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Validates the format of the cell identifier.
     *
     * Ensures the identifier starts with one or more letters followed by one or more digits.
     *
     * @param str - string_view of the identifier to validate.
     * @return true if the identifier is valid, false otherwise.
     */
	static bool validate_(const std::string_view& str) {
		CPosScanState validation_state = CPosScanState::INIT;
		bool is_col_valid = false, is_row_valid = false;

		for (auto chr : str)
			switch (validation_state) {
				case CPosScanState::INIT:
					if (std::isalpha(chr)) { validation_state = CPosScanState::COL; is_col_valid = true; }
					else return false;
					break;

				case CPosScanState::COL:
					if (std::isdigit(chr)) { validation_state = CPosScanState::ROW; is_row_valid = true; }
					else if (!std::isalpha(chr)) return false;
					break;

				case CPosScanState::ROW:
					if (!std::isdigit(chr)) return false;
					break;
			}

		return validation_state == CPosScanState::ROW && is_col_valid && is_row_valid;
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Parses the string identifier into column and row components.
     *
     * Converts column letters to a numerical index and extracts the row index directly.
     *
     * @param str - string_view of the identifier to parse.
     */
	void parse_(const std::string_view& str) {
		CPosScanState parse_state = CPosScanState::INIT;
		int parsed_col = 0;
		std::string parsed_row;

		for (auto chr : str)
			switch (parse_state) {
				case CPosScanState::INIT:
					parse_state = CPosScanState::COL;
					parsed_col = parsed_col * 26 + (std::toupper(chr) - 'A' + 1);
					break;

				case CPosScanState::COL:
					if (std::isdigit(chr)) { parse_state = CPosScanState::ROW; parsed_row.push_back(chr); }
					else parsed_col = parsed_col * 26 + (std::toupper(chr) - 'A' + 1);
					break;

				case CPosScanState::ROW:
					parsed_row.push_back(chr);
					break;
			}

		pos_data_ = {parsed_col, std::stoi(parsed_row)};
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Serializes the column identifier to its string representation.
     *
     * Converts the numerical column index back to a string of letters.
     *
     * @return std::string representing the column identifier.
     */
	std::string serializeColID_() const {
		std::string serialized_col;

		for (int col_tmp = pos_data_.first; col_tmp > 0; col_tmp = (col_tmp - 1) / 26)
			serialized_col = char('A' + (col_tmp - 1) % 26) + serialized_col;

		return serialized_col;
	}

	/**
     * @brief
     * Serializes the row identifier to its string representation.
     *
     * Converts the numerical row index to a string.
     *
     * @return std::string representing the row identifier.
     */
	std::string serializeRowID_() const {
		return std::to_string(pos_data_.second);
	}
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

class CExprProcessor;

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief
 * Abstract base class for expression units within a spreadsheet cell.
 *
 * Defines the interface for components that can represent parts of expressions,
 * such as constants, cell references, or complex expressions. This class allows
 * for polymorphic handling of different expression types, encapsulation of expression
 * logic, and serialization of expressions with potential positional shifts.
 */
class CExprUnit {
public:
	/**
     * @brief
     * Virtual destructor to allow derived class objects to be deleted properly.
     */
	virtual ~CExprUnit() = default;

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Creates and returns a shared pointer to a new object that is a copy of this expression unit.
     *
     * This function is used for encapsulating the expression logic into a separate object that can
     * be managed independently from the original object.
     *
     * @return std::shared_ptr<CExprUnit> to the newly created copy of this expression unit.
     */
	virtual std::shared_ptr<CExprUnit> encapsulate() const = 0;

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Serializes the expression to a string format with an optional shift in position.
     *
     * This function is essential for saving expressions to a file or displaying them
     * in a UI, with an option to adjust the position references based on the provided shift.
     *
     * @param shift - pair of integers representing the column and row shift respectively.
     * @return std::string representing the serialized expression.
     */
	virtual std::string serialize(std::pair<int, int> shift) const = 0;

	/**
     * @brief
     * Returns a string that identifies the type of the expression unit.
     *
     * @return std::string representing the type of the expression unit.
     */
	virtual std::string type() const = 0;

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Evaluates the result of the expression, considering any positional shifts and external references.
     *
     * This function calculates the value of the expression based on the current state of the spreadsheet,
     * potentially shifting position references and ensuring that cyclic dependencies are managed.
     *
     * @param shift - pair of integers representing the column and row shift respectively.
     * @param wrapped_sheet - shared pointer to a map from cell positions to expression processors, representing the entire spreadsheet.
     * @param visited_pos - reference to a set of positions that tracks visited cells to detect cycles.
     * @return CValue - the result of the expression evaluation.
     */
	virtual CValue result(std::pair<int, int> shift, std::shared_ptr<std::map<CPos, CExprProcessor>> wrapped_sheet,
	                      std::set<CPos>& visited_pos) = 0;
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief
 * Class that processes expressions, managing the operations and values within a stack-based structure.
 *
 * Inherits from CExprBuilder and provides implementations for all virtual methods to manipulate and evaluate
 * expression units. The class supports operations on expressions such as arithmetic calculations, logical comparisons,
 * and function calls. It also handles serialization of its current state.
 */
class CExprProcessor : public CExprBuilder {
public:
	/**
     * @brief
     * Constructs a CExprProcessor with an optional shift.
     *
     * @param shift - Optional initial shift to apply to expression references, default is no shift.
     */
	explicit CExprProcessor(std::pair<int, int> shift = {0, 0})
			: processor_(), shift_(std::move(shift)) {}

	/**
     * @brief
     * Copy constructor.
     *
     * Performs a deep copy of the top-most expression unit in the stack if available.
     *
     * @param other - Reference to the other CExprProcessor from which to copy.
     */
	CExprProcessor(const CExprProcessor& other)
			: processor_(), shift_(other.shift_) {
		if (!other.processor_.empty())
			processor_.push(other.processor_.top()->encapsulate());
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Assignment operator.
     *
     * Clears the current processor stack and copies the top-most expression unit from the other processor if available.
     *
     * @param other - Reference to the other CExprProcessor to assign from.
     * @return Reference to this CExprProcessor.
     */
	CExprProcessor& operator=(const CExprProcessor& other) {
		if (this != & other) {
			shift_ = other.shift_;

			while (!processor_.empty())
				processor_.pop();

			if (!other.processor_.empty())
				processor_.push(other.processor_.top()->encapsulate());
		}
		return * this;
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Adjusts the current shift applied to expression references.
     *
     * This can be used to update the position context as expressions are evaluated in different cells.
     *
     * @param shift - A pair of integers to adjust the current shift.
     * @return Reference to this CExprProcessor.
     */
	CExprProcessor& setShift(std::pair<int, int> shift) {
		shift_ = {shift_.first + shift.first, shift_.second + shift.second};

		return * this;
	}

	// -----------------------------------------------------------------------------------------------------------------

	// Virtual methods from CExprBuilder for various expression operations:
	void opAdd() override;

	void opSub() override;

	void opMul() override;

	void opDiv() override;

	void opPow() override;

	void opNeg() override;

	void opEq() override;

	void opNe() override;

	void opLt() override;

	void opLe() override;

	void opGt() override;

	void opGe() override;

	// -----------------------------------------------------------------------------------------------------------------

	void valNumber(double num) override;

	void valString(std::string str) override;

	void valReference(std::string ref) override;

	void valRange(std::string rng) override;

	// -----------------------------------------------------------------------------------------------------------------

	void funcCall(std::string fn_name, int par_cnt) override;

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Evaluates the current expression and returns the result as a CValue object.
     *
     * Considers the entire current context of the spreadsheet to resolve references and execute functions.
     *
     * @param wrapped_sheet - Shared pointer to the map of CPos to CExprProcessor, representing the spreadsheet.
     * @param visited_pos - Set of visited positions for cycle detection.
     * @return CValue representing the result of the expression evaluation.
     */
	CValue result(std::shared_ptr<std::map<CPos, CExprProcessor>> wrapped_sheet,
	              std::set<CPos>& visited_pos);

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Serializes the state of the processor to a string.
     *
     * Useful for debugging or saving the state of the processor.
     *
     * @return std::string representing the serialized state of the processor.
     */
	std::string serialize() const;

private:
	std::stack<std::shared_ptr<CExprUnit>> processor_; // Stack of expression units being processed.

	// -----------------------------------------------------------------------------------------------------------------

	std::pair<int, int> shift_; // Current shift applied to expression references.

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Helper method to pop the top expression unit from the stack.
     *
     * @return Shared pointer to the CExprUnit popped from the stack.
     */
	std::shared_ptr<CExprUnit> extractExprUnit_() {
		auto expr_unit = processor_.top();
		processor_.pop();

		return expr_unit;
	}

	/**
     * @brief
     * Helper method to push an expression unit onto the stack.
     *
     * @param expr_unit - Shared pointer to the CExprUnit to push onto the stack.
     */
	void pushExprUnit_(std::shared_ptr<CExprUnit> expr_unit) {
		processor_.push(std::move(expr_unit));
	}
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief
 * Represents a simple expression unit that holds a single value.
 *
 * This class extends CExprUnit to encapsulate a CValue object, allowing the representation of
 * constants or simple values within expressions. The class primarily serves as a basic component in
 * more complex expression structures.
 */
class CExprValueUnit : public CExprUnit {
public:
	/**
     * @brief
     * Constructs a CExprValueUnit with a given value.
     *
     * Initializes the unit with a value, typically representing constants or immediate values
     * in an expression. The value is moved into the unit to optimize performance and avoid unnecessary copies.
     *
     * @param val - CValue object representing the value to be encapsulated by this unit.
     */
	explicit CExprValueUnit(CValue val)
			: value_(std::move(val)) {}

	/**
     * @brief
     * Default destructor.
     *
     * Ensures proper cleanup of resources, if necessary, when a CExprValueUnit object is destroyed.
     */
	~CExprValueUnit() override = default;

protected:
	CValue value_; // Holds the actual value represented by this expression unit.
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief
 * Represents a numerical expression unit within a spreadsheet expression structure.
 *
 * This class extends CExprValueUnit specifically for handling numerical values. It encapsulates
 * a single double value and provides methods for encapsulation, serialization, and evaluation
 * specific to numerical constants.
 */
class CExprNumberUnit : public CExprValueUnit {
public:
	/**
     * @brief
     * Constructs a CExprNumberUnit with a specific numerical value.
     *
     * The numerical value is stored as a CValue, leveraging the base class's storage capabilities.
     * This allows the class to be directly usable in expression evaluations, offering a straightforward
     * way to handle numerical constants.
     *
     * @param num - The numerical value to encapsulate in this unit.
     */
	explicit CExprNumberUnit(double num)
			: CExprValueUnit(num) {}

	/**
     * @brief
     * Default destructor.
     *
     * Ensures proper cleanup and deallocation of resources associated with this unit, if necessary.
     */
	~CExprNumberUnit() override = default;

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Creates a new instance of this class and returns it as a shared pointer to CExprUnit.
     *
     * This method allows for creating a copy of the current expression unit, encapsulating its
     * state in a new object. Useful for expression replication and manipulation without altering
     * the original object.
     *
     * @return std::shared_ptr<CExprUnit> pointing to the newly created copy of this unit.
     */
	std::shared_ptr<CExprUnit> encapsulate() const override {
		return std::make_shared<CExprNumberUnit>(* this);
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Serializes the contained numerical value to a string.
     *
     * Converts the numerical value stored within this unit to its string representation, ignoring
     * any potential shifts as they do not affect raw numerical values.
     *
     * @param shift - A pair of integers representing a notional shift (ignored in this implementation).
     * @return std::string representing the serialized numerical value.
     */
	std::string serialize(std::pair<int, int> shift) const override {
		return std::to_string(std::get<double>(value_));
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Returns the type identifier of this expression unit.
     *
     * Identifies this unit as a numerical type ("NUM"), facilitating type checking and dispatch in
     * more complex expression structures.
     *
     * @return std::string "NUM" representing the type of this unit.
     */
	std::string type() const override {
		return TYPE;
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Evaluates and returns the result of this expression unit as a CValue.
     *
     * Since this unit encapsulates a constant numerical value, the evaluation simply returns the
     * contained value, unaffected by shifts or external spreadsheet references.
     *
     * @param shift - A pair of integers representing a notional shift (ignored in this implementation).
     * @param wrapped_sheet - Not used in this implementation.
     * @param visited_pos - Not used in this implementation.
     * @return CValue containing the numerical value of this unit.
     */
	CValue result(std::pair<int, int> shift, std::shared_ptr<std::map<CPos, CExprProcessor>> wrapped_sheet,
	              std::set<CPos>& visited_pos) override {
		return value_;
	}

private:
	static constexpr const char* TYPE = "NUM"; // Static type identifier for this class.
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief
 * Represents a string expression unit within a spreadsheet expression structure.
 *
 * This class extends CExprValueUnit specifically for handling string values. It encapsulates
 * a single string value and provides methods for encapsulation, serialization, and evaluation
 * specific to string constants. This class supports the basic functionalities required to handle
 * string expressions in a spreadsheet environment, such as serialization and result evaluation.
 */
class CExprStringUnit : public CExprValueUnit {
public:
	/**
     * @brief
     * Constructs a CExprStringUnit with a specific string value.
     *
     * The string value is stored as a CValue, leveraging the base class's storage capabilities.
     * This allows the class to be directly usable in expression evaluations, offering a straightforward
     * way to handle string constants.
     *
     * @param str - The string value to encapsulate in this unit.
     */
	explicit CExprStringUnit(std::string str)
			: CExprValueUnit(str) {}

	/**
     * @brief
     * Default destructor.
     *
     * Ensures proper cleanup and deallocation of resources associated with this unit, if necessary.
     */
	~CExprStringUnit() override = default;

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Creates a new instance of this class and returns it as a shared pointer to CExprUnit.
     *
     * This method allows for creating a copy of the current expression unit, encapsulating its
     * state in a new object. Useful for expression replication and manipulation without altering
     * the original object.
     *
     * @return std::shared_ptr<CExprUnit> pointing to the newly created copy of this unit.
     */
	std::shared_ptr<CExprUnit> encapsulate() const override {
		return std::make_shared<CExprStringUnit>(* this);
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Serializes the contained string value to a string.
     *
     * Converts the string value stored within this unit to its string representation, ignoring
     * any potential shifts as they do not affect raw string values.
     *
     * @param shift - A pair of integers representing a notional shift (ignored in this implementation).
     * @return std::string representing the serialized string value.
     */
	std::string serialize(std::pair<int, int> shift) const override {
		return std::get<std::string>(value_);
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Returns the type identifier of this expression unit.
     *
     * Identifies this unit as a string type ("STR"), facilitating type checking and dispatch in
     * more complex expression structures.
     *
     * @return std::string "STR" representing the type of this unit.
     */
	std::string type() const override {
		return TYPE;
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Evaluates and returns the result of this expression unit as a CValue.
     *
     * Since this unit encapsulates a constant string value, the evaluation simply returns the
     * contained value, unaffected by shifts or external spreadsheet references.
     *
     * @param shift - A pair of integers representing a notional shift (ignored in this implementation).
     * @param wrapped_sheet - Not used in this implementation.
     * @param visited_pos - Not used in this implementation.
     * @return CValue containing the string value of this unit.
     */
	CValue result(std::pair<int, int> shift, std::shared_ptr<std::map<CPos, CExprProcessor>> wrapped_sheet,
	              std::set<CPos>& visited_pos) override {
		return value_;
	}

private:
	static constexpr const char* TYPE = "STR"; // Static type identifier for this class.
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief
 * Represents a cell reference expression unit within a spreadsheet expression structure.
 *
 * This class extends CExprValueUnit specifically for handling cell references. It encapsulates
 * a reference to another cell in the form of a string and provides functionality for managing
 * absolute and relative references, serialization, and evaluation of references within the
 * context of a spreadsheet's cell dependency and evaluation logic.
 */
class CExprReferenceUnit : public CExprValueUnit {
public:
	/**
     * @brief
     * Constructs a CExprReferenceUnit with a specific cell reference.
     *
     * Parses the provided string to determine the nature of the reference (absolute or relative)
     * and initializes the unit accordingly.
     *
     * @param ref - The cell reference string to encapsulate in this unit.
     */
	explicit CExprReferenceUnit(std::string ref)
			: CExprValueUnit(std::move(ref)) {
		parse_();
	}

	/**
     * @brief
     * Default destructor.
     *
     * Ensures proper cleanup and deallocation of resources associated with this unit, if necessary.
     */
	~CExprReferenceUnit() override = default;

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Creates a new instance of this class and returns it as a shared pointer to CExprUnit.
     *
     * This method allows for creating a copy of the current expression unit, encapsulating its
     * state in a new object. Useful for expression replication and manipulation without altering
     * the original object.
     *
     * @return std::shared_ptr<CExprUnit> pointing to the newly created copy of this unit.
     */
	std::shared_ptr<CExprUnit> encapsulate() const override {
		return std::make_shared<CExprReferenceUnit>(* this);
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Serializes the contained cell reference to a string, adjusting for any shifts if necessary.
     *
     * Depending on whether the reference is absolute or relative, shifts are applied appropriately
     * to generate the serialized output.
     *
     * @param shift - A pair of integers representing the column and row shifts to apply.
     * @return std::string representing the serialized reference.
     */
	std::string serialize(std::pair<int, int> shift) const override {
		auto result_shift =
				std::make_pair(reference_type_data_.first == CExprReferenceIDType::REL ? shift.first : 0,
				               reference_type_data_.second == CExprReferenceIDType::REL ? shift.second : 0);
		auto result_pos = CPos(std::get<std::string>(value_), result_shift);
		std::string serialized_reference;

		if (reference_type_data_.first == CExprReferenceIDType::ABS)
			serialized_reference += ABS_TYPE;
		serialized_reference += result_pos.IDs().first;

		if (reference_type_data_.second == CExprReferenceIDType::ABS)
			serialized_reference += ABS_TYPE;
		serialized_reference += result_pos.IDs().second;

		return serialized_reference;
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Returns the type identifier of this expression unit.
     *
     * Identifies this unit as a reference type ("REF"), facilitating type checking and dispatch in
     * more complex expression structures.
     *
     * @return std::string "REF" representing the type of this unit.
     */
	std::string type() const override {
		return TYPE;
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Evaluates and returns the result of the reference expression unit as a CValue.
     *
     * The evaluation follows the reference to another cell and computes its value, taking into
     * account cycles and dependencies. Handles shifts appropriately if the reference is relative.
     *
     * @param shift - A pair of integers representing a notional shift to apply to relative references.
     * @param wrapped_sheet - Shared pointer to the map of CPos to CExprProcessor, representing the spreadsheet.
     * @param visited_pos - Set of visited positions for cycle detection.
     * @return CValue containing the result of the evaluation.
     */
	CValue result(std::pair<int, int> shift, std::shared_ptr<std::map<CPos, CExprProcessor>> wrapped_sheet,
	              std::set<CPos>& visited_pos) override {
		auto result_shift =
				std::make_pair(reference_type_data_.first == CExprReferenceIDType::REL ? shift.first : 0,
				               reference_type_data_.second == CExprReferenceIDType::REL ? shift.second : 0);
		auto result_pos = CPos(std::get<std::string>(value_), result_shift);

		if (visited_pos.contains(result_pos))
			return CValue();

		visited_pos.insert(result_pos);

		auto result = wrapped_sheet->contains(result_pos) ?
		              wrapped_sheet->at(result_pos).result(wrapped_sheet, visited_pos) : CValue();

		visited_pos.erase(result_pos);

		return result;
	}

private:
	enum class CExprReferenceIDType { ABS, REL }; // Indicates whether a reference part is absolute or relative.
	enum class CExprReferenceScanState { INIT, COL_ABS, COL, ROW_ABS, ROW }; // States for parsing the reference string.
	constexpr static const char ABS_TYPE = '$'; // Character used to denote absolute references in the string.

	static constexpr const char* TYPE = "REF"; // Static type identifier for this class.

	// -----------------------------------------------------------------------------------------------------------------

	std::pair<CExprReferenceIDType, CExprReferenceIDType> reference_type_data_; // Stores the type of reference for column and row.

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Parses the reference string to determine its structure and relative or absolute nature.
     *
     * Adjusts the internal representation of the value to separate the column and row parts
     * and classify them as absolute or relative as indicated by the presence of the '$' character.
     */
	void parse_() {
		CExprReferenceScanState parse_state = CExprReferenceScanState::INIT;
		std::string parsed_reference;

		for (auto chr : std::get<std::string>(value_))
			switch (parse_state) {
				case CExprReferenceScanState::INIT:
					if (chr == ABS_TYPE) parse_state = CExprReferenceScanState::COL_ABS;
					else {
						parse_state = CExprReferenceScanState::COL;
						reference_type_data_.first = CExprReferenceIDType::REL;
						parsed_reference.push_back(chr);
					}
					break;

				case CExprReferenceScanState::COL_ABS:
					parse_state = CExprReferenceScanState::COL;
					reference_type_data_.first = CExprReferenceIDType::ABS;
					parsed_reference.push_back(chr);
					break;

				case CExprReferenceScanState::COL:
					if (chr == ABS_TYPE) parse_state = CExprReferenceScanState::ROW_ABS;
					else {
						if (std::isdigit(chr)) {
							parse_state = CExprReferenceScanState::ROW;
							reference_type_data_.second = CExprReferenceIDType::REL;
						}
						parsed_reference.push_back(chr);
					}
					break;

				case CExprReferenceScanState::ROW_ABS:
					parse_state = CExprReferenceScanState::ROW;
					reference_type_data_.second = CExprReferenceIDType::ABS;
					parsed_reference.push_back(chr);
					break;

				case CExprReferenceScanState::ROW:
					parsed_reference.push_back(chr);
					break;
			}

		value_ = parsed_reference;
	}
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief
 * Represents a unary operation expression unit within a spreadsheet expression structure.
 *
 * This class extends CExprUnit to handle unary operations, such as negation or logical NOT,
 * depending on the specific operation implemented. It encapsulates a single operand and provides the basic
 * structure for unary operation handling in more complex expressions.
 */
class CExprUnOperationUnit : public CExprUnit {
public:
	/**
     * @brief
     * Constructs a CExprUnOperationUnit with a specific operand.
     *
     * Initializes the unit with a pointer to another CExprUnit, which represents the operand of the unary operation.
     * This setup allows the unary operation unit to manage and apply its operation to the result of the operand expression.
     *
     * @param opd - Shared pointer to the CExprUnit that acts as the operand for this unary operation.
     */
	explicit CExprUnOperationUnit(std::shared_ptr<CExprUnit> opd)
			: operand_(std::move(opd)) {}

	/**
     * @brief
     * Default destructor.
     *
     * Ensures proper cleanup and deallocation of resources associated with this unit, if necessary.
     */
	~CExprUnOperationUnit() override = default;

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Returns the type identifier of this expression unit.
     *
     * Identifies this unit as a generic expression type ("EXPR"), which can be specialized further in derived classes
     * to represent specific unary operations.
     *
     * @return std::string "EXPR" representing the generic type of this unit.
     */
	std::string type() const override {
		return TYPE;
	}

protected:
	std::shared_ptr<CExprUnit> operand_; // Pointer to the operand of this unary operation.

	static constexpr const char* TYPE = "EXPR"; // Static type identifier for this class, indicating a generic expression unit.
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief
 * Represents a negation expression unit within a spreadsheet expression structure.
 *
 * This class extends CExprUnOperationUnit to specifically handle the negation (unary minus) operation.
 * It applies this operation to the operand provided during construction. This allows the class to negate
 * numerical values dynamically based on the evaluation of the operand expression unit.
 */
class CExprNegationUnit : public CExprUnOperationUnit {
public:
	/**
     * @brief
     * Constructs a CExprNegationUnit with a specific operand for negation.
     *
     * Initializes the unit with a pointer to another CExprUnit, which this negation unit will negate.
     * This setup is useful for operations where a single numeric value's sign needs to be inverted.
     *
     * @param opd - Shared pointer to the CExprUnit that acts as the operand for this negation.
     */
	explicit CExprNegationUnit(std::shared_ptr<CExprUnit> opd)
			: CExprUnOperationUnit(std::move(opd)) {}

	/**
     * @brief
     * Default destructor.
     *
     * Ensures proper cleanup and deallocation of resources associated with this unit, if necessary.
     */
	~CExprNegationUnit() override = default;

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Creates a new instance of this class and returns it as a shared pointer to CExprUnit.
     *
     * This method allows for creating a copy of the current negation unit, encapsulating its
     * state in a new object. Useful for expression replication and manipulation without altering
     * the original object.
     *
     * @return std::shared_ptr<CExprUnit> pointing to the newly created copy of this negation unit.
     */
	std::shared_ptr<CExprUnit> encapsulate() const override {
		return std::make_shared<CExprNegationUnit>(* this);
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Serializes the negation operation along with its operand to a string.
     *
     * Constructs a string representation of the negation by prefixing a hyphen to the serialized form
     * of the operand, effectively representing the unary minus operation.
     *
     * @param shift - A pair of integers representing the column and row shifts to apply to any cell references within the operand.
     * @return std::string representing the serialized negation operation.
     */
	std::string serialize(std::pair<int, int> shift) const override {
		return "-" + operand_->serialize(shift);
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Evaluates the result of the negation operation and returns it as a CValue.
     *
     * Computes the value of the operand and applies the negation operation. If the operand evaluates
     * to a numerical value, it is negated. If the operand does not yield a numerical value, the result is undefined.
     *
     * @param shift - A pair of integers representing a notional shift to apply to relative references within the operand.
     * @param wrapped_sheet - Shared pointer to the map of CPos to CExprProcessor, representing the spreadsheet.
     * @param visited_pos - Set of visited positions for cycle detection during evaluation.
     * @return CValue containing the negated result of the operand evaluation.
     */
	CValue result(std::pair<int, int> shift, std::shared_ptr<std::map<CPos, CExprProcessor>> wrapped_sheet,
	              std::set<CPos>& visited_pos) override {
		CValue result = operand_->result(shift, wrapped_sheet, visited_pos);

		if (std::holds_alternative<double>(result))
			return -std::get<double>(result);

		return CValue();
	}
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief
 * Represents a binary operation expression unit within a spreadsheet expression structure.
 *
 * This class extends CExprUnit to specifically handle binary operations, such as addition, subtraction,
 * multiplication, and division. It encapsulates two operands, each of which is also an expression unit,
 * and provides the basic framework for binary operation handling in more complex expressions.
 */
class CExprBiOperationUnit : public CExprUnit {
public:
	/**
     * @brief
     * Constructs a CExprBiOperationUnit with specific left and right operands.
     *
     * Initializes the unit with pointers to two CExprUnit instances, which represent the left-hand side
     * and right-hand side operands of the binary operation. This setup allows the binary operation unit
     * to manage and apply its operation to the results of these operand expressions.
     *
     * @param lhs_opd - Shared pointer to the CExprUnit that acts as the left-hand side operand.
     * @param rhs_opd - Shared pointer to the CExprUnit that acts as the right-hand side operand.
     */
	CExprBiOperationUnit(std::shared_ptr<CExprUnit> lhs_opd, std::shared_ptr<CExprUnit> rhs_opd)
			: lhs_operand_(std::move(lhs_opd)), rhs_operand_(std::move(rhs_opd)) {}

	/**
     * @brief
     * Default destructor.
     *
     * Ensures proper cleanup and deallocation of resources associated with this unit, if necessary.
     */
	~CExprBiOperationUnit() override = default;

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Returns the type identifier of this expression unit.
     *
     * Identifies this unit as a generic expression type ("EXPR"), which can be specialized further in
     * derived classes to represent specific binary operations.
     *
     * @return std::string "EXPR" representing the generic type of this unit.
     */
	std::string type() const override {
		return TYPE;
	}

protected:
	std::shared_ptr<CExprUnit> lhs_operand_, rhs_operand_; // Pointer to the left-hand and right-hand side operands of this binary operation.

	static constexpr const char* TYPE = "EXPR"; // Static type identifier for this class, indicating a generic expression unit.
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief
 * Represents an addition expression unit within a spreadsheet expression structure.
 *
 * This class extends CExprBiOperationUnit to specifically handle addition operations between two operands,
 * which can be any valid expression units. It supports addition of numbers as well as concatenation of strings,
 * and appropriately handles mixed-type operations by converting numbers to strings when necessary.
 */
class CExprAdditionUnit : public CExprBiOperationUnit {
public:
	/**
     * @brief
     * Constructs a CExprAdditionUnit with specific left and right operands.
     *
     * Initializes the unit with pointers to two CExprUnit instances, which represent the left-hand side
     * and right-hand side operands of the addition operation. This setup allows the addition operation unit
     * to manage and apply its operation to the results of these operand expressions.
     *
     * @param lhs_opd - Shared pointer to the CExprUnit that acts as the left-hand side operand.
     * @param rhs_opd - Shared pointer to the CExprUnit that acts as the right-hand side operand.
     */
	CExprAdditionUnit(std::shared_ptr<CExprUnit> lhs_opd, std::shared_ptr<CExprUnit> rhs_opd)
			: CExprBiOperationUnit(std::move(lhs_opd), std::move(rhs_opd)) {}

	/**
	* @brief
	* Default destructor.
	*
	* Ensures proper cleanup and deallocation of resources associated with this unit, if necessary.
	*/
	~CExprAdditionUnit() override = default;

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Creates a new instance of this class and returns it as a shared pointer to CExprUnit.
     *
     * This method allows for creating a copy of the current addition unit, encapsulating its
     * state in a new object. Useful for expression replication and manipulation without altering
     * the original object.
     *
     * @return std::shared_ptr<CExprUnit> pointing to the newly created copy of this addition unit.
     */
	std::shared_ptr<CExprUnit> encapsulate() const override {
		return std::make_shared<CExprAdditionUnit>(* this);
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Serializes the addition operation along with its operands to a string.
     *
     * Constructs a string representation of the addition by wrapping the serialized forms
     * of the left and right operands with parentheses and placing a '+' sign between them.
     *
     * @param shift - A pair of integers representing the column and row shifts to apply to any cell references within the operands.
     * @return std::string representing the serialized addition operation.
     */
	std::string serialize(std::pair<int, int> shift) const override {
		return "(" + lhs_operand_->serialize(shift) + " + " + rhs_operand_->serialize(shift) + ")";
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Evaluates the result of the addition operation and returns it as a CValue.
     *
     * Computes the values of both operands and applies the addition operation. Handles different types of operands appropriately,
     * supporting numeric addition and string concatenation, including conversions between types when necessary.
     *
     * @param shift - A pair of integers representing a notional shift to apply to relative references within the operands.
     * @param wrapped_sheet - Shared pointer to the map of CPos to CExprProcessor, representing the spreadsheet.
     * @param visited_pos - Set of visited positions for cycle detection during evaluation.
     * @return CValue containing the result of the addition operation.
     */
	CValue result(std::pair<int, int> shift, std::shared_ptr<std::map<CPos, CExprProcessor>> wrapped_sheet,
	              std::set<CPos>& visited_pos) override {
		CValue lhs_result = lhs_operand_->result(shift, wrapped_sheet, visited_pos),
				rhs_result = rhs_operand_->result(shift, wrapped_sheet, visited_pos);

		if (std::holds_alternative<double>(lhs_result) && std::holds_alternative<double>(rhs_result))
			return std::get<double>(lhs_result) + std::get<double>(rhs_result);

		else if (std::holds_alternative<std::string>(lhs_result) && std::holds_alternative<std::string>(rhs_result))
			return std::get<std::string>(lhs_result) + std::get<std::string>(rhs_result);

		else if (std::holds_alternative<double>(lhs_result) && std::holds_alternative<std::string>(rhs_result))
			return std::to_string(std::get<double>(lhs_result)) + std::get<std::string>(rhs_result);

		else if (std::holds_alternative<std::string>(lhs_result) && std::holds_alternative<double>(rhs_result))
			return std::get<std::string>(lhs_result) + std::to_string(std::get<double>(rhs_result));

		return CValue();
	}
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief
 * Represents a subtraction expression unit within a spreadsheet expression structure.
 *
 * This class extends CExprBiOperationUnit to specifically handle subtraction operations between two operands,
 * which can be any valid expression units. It allows for the arithmetic subtraction of numeric values,
 * effectively managing the difference between the results of the left-hand and right-hand side operand expressions.
 */
class CExprSubtractionUnit : public CExprBiOperationUnit {
public:
	/**
     * @brief
     * Constructs a CExprSubtractionUnit with specific left and right operands.
     *
     * Initializes the unit with pointers to two CExprUnit instances, which represent the left-hand side
     * and right-hand side operands of the subtraction operation. This setup allows the subtraction operation unit
     * to manage and apply its operation to the results of these operand expressions.
     *
     * @param lhs_opd - Shared pointer to the CExprUnit that acts as the left-hand side operand.
     * @param rhs_opd - Shared pointer to the CExprUnit that acts as the right-hand side operand.
     */
	CExprSubtractionUnit(std::shared_ptr<CExprUnit> lhs_opd, std::shared_ptr<CExprUnit> rhs_opd)
			: CExprBiOperationUnit(std::move(lhs_opd), std::move(rhs_opd)) {}

	/**
     * @brief
     * Default destructor.
     *
     * Ensures proper cleanup and deallocation of resources associated with this unit, if necessary.
     */
	~CExprSubtractionUnit() override = default;

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Creates a new instance of this class and returns it as a shared pointer to CExprUnit.
     *
     * This method allows for creating a copy of the current subtraction unit, encapsulating its
     * state in a new object. Useful for expression replication and manipulation without altering
     * the original object.
     *
     * @return std::shared_ptr<CExprUnit> pointing to the newly created copy of this subtraction unit.
     */
	std::shared_ptr<CExprUnit> encapsulate() const override {
		return std::make_shared<CExprSubtractionUnit>(* this);
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Serializes the subtraction operation along with its operands to a string.
     *
     * Constructs a string representation of the subtraction by wrapping the serialized forms
     * of the left and right operands with parentheses and placing a '-' sign between them.
     *
     * @param shift - A pair of integers representing the column and row shifts to apply to any cell references within the operands.
     * @return std::string representing the serialized subtraction operation.
     */
	std::string serialize(std::pair<int, int> shift) const override {
		return "(" + lhs_operand_->serialize(shift) + " - " + rhs_operand_->serialize(shift) + ")";
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Evaluates the result of the subtraction operation and returns it as a CValue.
     *
     * Computes the values of both operands and applies the subtraction operation. This is straightforward
     * for numeric values, subtracting the right operand from the left operand.
     *
     * @param shift - A pair of integers representing a notional shift to apply to relative references within the operands.
     * @param wrapped_sheet - Shared pointer to the map of CPos to CExprProcessor, representing the spreadsheet.
     * @param visited_pos - Set of visited positions for cycle detection during evaluation.
     * @return CValue containing the result of the subtraction operation.
     */
	CValue result(std::pair<int, int> shift, std::shared_ptr<std::map<CPos, CExprProcessor>> wrapped_sheet,
	              std::set<CPos>& visited_pos) override {
		CValue lhs_result = lhs_operand_->result(shift, wrapped_sheet, visited_pos),
				rhs_result = rhs_operand_->result(shift, wrapped_sheet, visited_pos);

		if (std::holds_alternative<double>(lhs_result) && std::holds_alternative<double>(rhs_result))
			return std::get<double>(lhs_result) - std::get<double>(rhs_result);

		return CValue();
	}
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief
 * Represents a multiplication expression unit within a spreadsheet expression structure.
 *
 * This class extends CExprBiOperationUnit to specifically handle multiplication operations between two operands,
 * which can be any valid expression units. It allows for the arithmetic multiplication of numeric values,
 * effectively managing the product of the results of the left-hand and right-hand side operand expressions.
 */
class CExprMultiplicationUnit : public CExprBiOperationUnit {
public:
	/**
     * @brief
     * Constructs a CExprMultiplicationUnit with specific left and right operands.
     *
     * Initializes the unit with pointers to two CExprUnit instances, which represent the left-hand side
     * and right-hand side operands of the multiplication operation. This setup allows the multiplication operation unit
     * to manage and apply its operation to the results of these operand expressions.
     *
     * @param lhs_opd - Shared pointer to the CExprUnit that acts as the left-hand side operand.
     * @param rhs_opd - Shared pointer to the CExprUnit that acts as the right-hand side operand.
     */
	CExprMultiplicationUnit(std::shared_ptr<CExprUnit> lhs_opd, std::shared_ptr<CExprUnit> rhs_opd)
			: CExprBiOperationUnit(std::move(lhs_opd), std::move(rhs_opd)) {}

	/**
     * @brief
     * Default destructor.
     *
     * Ensures proper cleanup and deallocation of resources associated with this unit, if necessary.
     */
	~CExprMultiplicationUnit() override = default;

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Creates a new instance of this class and returns it as a shared pointer to CExprUnit.
     *
     * This method allows for creating a copy of the current multiplication unit, encapsulating its
     * state in a new object. Useful for expression replication and manipulation without altering
     * the original object.
     *
     * @return std::shared_ptr<CExprUnit> pointing to the newly created copy of this multiplication unit.
     */
	std::shared_ptr<CExprUnit> encapsulate() const override {
		return std::make_shared<CExprMultiplicationUnit>(* this);
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Serializes the multiplication operation along with its operands to a string.
     *
     * Constructs a string representation of the multiplication by wrapping the serialized forms
     * of the left and right operands with parentheses and placing a '*' sign between them.
     *
     * @param shift - A pair of integers representing the column and row shifts to apply to any cell references within the operands.
     * @return std::string representing the serialized multiplication operation.
     */
	std::string serialize(std::pair<int, int> shift) const override {
		return "(" + lhs_operand_->serialize(shift) + " * " + rhs_operand_->serialize(shift) + ")";
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Evaluates the result of the multiplication operation and returns it as a CValue.
     *
     * Computes the values of both operands and applies the multiplication operation. This is straightforward
     * for numeric values, multiplying the right operand by the left operand.
     *
     * @param shift - A pair of integers representing a notional shift to apply to relative references within the operands.
     * @param wrapped_sheet - Shared pointer to the map of CPos to CExprProcessor, representing the spreadsheet.
     * @param visited_pos - Set of visited positions for cycle detection during evaluation.
     * @return CValue containing the result of the multiplication operation.
     */
	CValue result(std::pair<int, int> shift, std::shared_ptr<std::map<CPos, CExprProcessor>> wrapped_sheet,
	              std::set<CPos>& visited_pos) override {
		CValue lhs_result = lhs_operand_->result(shift, wrapped_sheet, visited_pos),
				rhs_result = rhs_operand_->result(shift, wrapped_sheet, visited_pos);

		if (std::holds_alternative<double>(lhs_result) && std::holds_alternative<double>(rhs_result))
			return std::get<double>(lhs_result) * std::get<double>(rhs_result);

		return CValue();
	}
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief
 * Represents a division expression unit within a spreadsheet expression structure.
 *
 * This class extends CExprBiOperationUnit to specifically handle division operations between two operands,
 * which can be any valid expression units. It allows for the arithmetic division of numeric values,
 * effectively managing the quotient of the results of the left-hand and right-hand side operand expressions.
 * Special care is taken to handle division by zero, returning an undefined value in such cases.
 */
class CExprDivisionUnit : public CExprBiOperationUnit {
public:
	/**
     * @brief
     * Constructs a CExprDivisionUnit with specific left and right operands.
     *
     * Initializes the unit with pointers to two CExprUnit instances, which represent the left-hand side
     * and right-hand side operands of the division operation. This setup allows the division operation unit
     * to manage and apply its operation to the results of these operand expressions.
     *
     * @param lhs_opd - Shared pointer to the CExprUnit that acts as the left-hand side operand.
     * @param rhs_opd - Shared pointer to the CExprUnit that acts as the right-hand side operand.
     */
	CExprDivisionUnit(std::shared_ptr<CExprUnit> lhs_opd, std::shared_ptr<CExprUnit> rhs_opd)
			: CExprBiOperationUnit(std::move(lhs_opd), std::move(rhs_opd)) {}

	/**
     * @brief
     * Default destructor.
     *
     * Ensures proper cleanup and deallocation of resources associated with this unit, if necessary.
     */
	~CExprDivisionUnit() override = default;

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Creates a new instance of this class and returns it as a shared pointer to CExprUnit.
     *
     * This method allows for creating a copy of the current division unit, encapsulating its
     * state in a new object. Useful for expression replication and manipulation without altering
     * the original object.
     *
     * @return std::shared_ptr<CExprUnit> pointing to the newly created copy of this division unit.
     */
	std::shared_ptr<CExprUnit> encapsulate() const override {
		return std::make_shared<CExprDivisionUnit>(* this);
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Serializes the division operation along with its operands to a string.
     *
     * Constructs a string representation of the division by wrapping the serialized forms
     * of the left and right operands with parentheses and placing a '/' sign between them.
     *
     * @param shift - A pair of integers representing the column and row shifts to apply to any cell references within the operands.
     * @return std::string representing the serialized division operation.
     */
	std::string serialize(std::pair<int, int> shift) const override {
		return "(" + lhs_operand_->serialize(shift) + " / " + rhs_operand_->serialize(shift) + ")";
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Evaluates the result of the division operation and returns it as a CValue.
     *
     * Computes the values of both operands and applies the division operation. If the divisor (right operand) is non-zero,
     * the division is performed, otherwise, an undefined value is returned to signal an error such as division by zero.
     *
     * @param shift - A pair of integers representing a notional shift to apply to relative references within the operands.
     * @param wrapped_sheet - Shared pointer to the map of CPos to CExprProcessor, representing the spreadsheet.
     * @param visited_pos - Set of visited positions for cycle detection during evaluation.
     * @return CValue containing the result of the division operation or an undefined value in case of division by zero.
     */
	CValue result(std::pair<int, int> shift, std::shared_ptr<std::map<CPos, CExprProcessor>> wrapped_sheet,
	              std::set<CPos>& visited_pos) override {
		CValue lhs_result = lhs_operand_->result(shift, wrapped_sheet, visited_pos),
				rhs_result = rhs_operand_->result(shift, wrapped_sheet, visited_pos);

		if (std::holds_alternative<double>(lhs_result) && std::holds_alternative<double>(rhs_result))
			if (std::get<double>(rhs_result) != 0.)
				return std::get<double>(lhs_result) / std::get<double>(rhs_result);

		return CValue();
	}
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief
 * Represents an exponentiation expression unit within a spreadsheet expression structure.
 *
 * This class extends CExprBiOperationUnit to specifically handle exponentiation operations between two operands,
 * which can be any valid expression units. It allows for the arithmetic exponentiation of numeric values,
 * effectively managing the power operation of the results of the left-hand and right-hand side operand expressions.
 */
class CExprExponentiationUnit : public CExprBiOperationUnit {
public:
	/**
     * @brief
     * Constructs a CExprExponentiationUnit with specific left and right operands.
     *
     * Initializes the unit with pointers to two CExprUnit instances, which represent the left-hand side
     * and right-hand side operands of the exponentiation operation. This setup allows the exponentiation operation unit
     * to manage and apply its operation to the results of these operand expressions.
     *
     * @param lhs_opd - Shared pointer to the CExprUnit that acts as the left-hand side operand.
     * @param rhs_opd - Shared pointer to the CExprUnit that acts as the right-hand side operand.
     */
	CExprExponentiationUnit(std::shared_ptr<CExprUnit> lhs_opd, std::shared_ptr<CExprUnit> rhs_opd)
			: CExprBiOperationUnit(std::move(lhs_opd), std::move(rhs_opd)) {}

	/**
     * @brief
     * Default destructor.
     *
     * Ensures proper cleanup and deallocation of resources associated with this unit, if necessary.
     */
	~CExprExponentiationUnit() override = default;

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Creates a new instance of this class and returns it as a shared pointer to CExprUnit.
     *
     * This method allows for creating a copy of the current exponentiation unit, encapsulating its
     * state in a new object. Useful for expression replication and manipulation without altering
     * the original object.
     *
     * @return std::shared_ptr<CExprUnit> pointing to the newly created copy of this exponentiation unit.
     */
	std::shared_ptr<CExprUnit> encapsulate() const override {
		return std::make_shared<CExprExponentiationUnit>(* this);
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Serializes the exponentiation operation along with its operands to a string.
     *
     * Constructs a string representation of the exponentiation by placing the serialized forms
     * of the left and right operands around a '^' sign, which represents the exponentiation operation.
     *
     * @param shift - A pair of integers representing the column and row shifts to apply to any cell references within the operands.
     * @return std::string representing the serialized exponentiation operation.
     */
	std::string serialize(std::pair<int, int> shift) const override {
		return lhs_operand_->serialize(shift) + " ^ " + rhs_operand_->serialize(shift);
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Evaluates the result of the exponentiation operation and returns it as a CValue.
     *
     * Computes the values of both operands and applies the exponentiation operation. This involves raising the value
     * of the left operand to the power of the right operand, and is handled only for numeric values.
     *
     * @param shift - A pair of integers representing a notional shift to apply to relative references within the operands.
     * @param wrapped_sheet - Shared pointer to the map of CPos to CExprProcessor, representing the spreadsheet.
     * @param visited_pos - Set of visited positions for cycle detection during evaluation.
     * @return CValue containing the result of the exponentiation operation, or an undefined value if one or both operands are not numeric.
     */
	CValue result(std::pair<int, int> shift, std::shared_ptr<std::map<CPos, CExprProcessor>> wrapped_sheet,
	              std::set<CPos>& visited_pos) override {
		CValue lhs_result = lhs_operand_->result(shift, wrapped_sheet, visited_pos),
				rhs_result = rhs_operand_->result(shift, wrapped_sheet, visited_pos);

		if (std::holds_alternative<double>(lhs_result) && std::holds_alternative<double>(rhs_result))
			return std::pow(std::get<double>(lhs_result), std::get<double>(rhs_result));

		return CValue();
	}
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief
 * Represents an equality expression unit within a spreadsheet expression structure.
 *
 * This class extends CExprBiOperationUnit to specifically handle equality operations between two operands,
 * which can be any valid expression units. It allows for the comparison of numeric values as well as strings,
 * effectively managing the equality check of the results of the left-hand and right-hand side operand expressions.
 */
class CExprEqualityUnit : public CExprBiOperationUnit {
public:
	/**
     * @brief
     * Constructs a CExprEqualityUnit with specific left and right operands.
     *
     * Initializes the unit with pointers to two CExprUnit instances, which represent the left-hand side
     * and right-hand side operands of the equality operation. This setup allows the equality operation unit
     * to manage and apply its operation to the results of these operand expressions, determining if they are equal.
     *
     * @param lhs_opd - Shared pointer to the CExprUnit that acts as the left-hand side operand.
     * @param rhs_opd - Shared pointer to the CExprUnit that acts as the right-hand side operand.
     */
	CExprEqualityUnit(std::shared_ptr<CExprUnit> lhs_opd, std::shared_ptr<CExprUnit> rhs_opd)
			: CExprBiOperationUnit(std::move(lhs_opd), std::move(rhs_opd)) {}

	/**
     * @brief
     * Default destructor.
     *
     * Ensures proper cleanup and deallocation of resources associated with this unit, if necessary.
     */
	~CExprEqualityUnit() override = default;

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Creates a new instance of this class and returns it as a shared pointer to CExprUnit.
     *
     * This method allows for creating a copy of the current equality unit, encapsulating its
     * state in a new object. Useful for expression replication and manipulation without altering
     * the original object.
     *
     * @return std::shared_ptr<CExprUnit> pointing to the newly created copy of this equality unit.
     */
	std::shared_ptr<CExprUnit> encapsulate() const override {
		return std::make_shared<CExprEqualityUnit>(*this);
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Serializes the equality operation along with its operands to a string.
     *
     * Constructs a string representation of the equality operation by wrapping the serialized forms
     * of the left and right operands with parentheses and placing an '==' sign between them.
     *
     * @param shift - A pair of integers representing the column and row shifts to apply to any cell references within the operands.
     * @return std::string representing the serialized equality operation.
     */
	std::string serialize(std::pair<int, int> shift) const override {
		return "(" + lhs_operand_->serialize(shift) + " == " + rhs_operand_->serialize(shift) + ")";
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Evaluates the result of the equality operation and returns it as a CValue.
     *
     * Computes the values of both operands and applies the equality operation. This operation checks if both operands are equal,
     * supporting numeric comparisons as well as string comparisons. The result is 1 if they are equal, 0 otherwise.
     *
     * @param shift - A pair of integers representing a notional shift to apply to relative references within the operands.
     * @param wrapped_sheet - Shared pointer to the map of CPos to CExprProcessor, representing the spreadsheet.
     * @param visited_pos - Set of visited positions for cycle detection during evaluation.
     * @return CValue containing 1 if the operands are equal, 0 if they are not, or an undefined value if the operands are not comparable.
     */
	CValue result(std::pair<int, int> shift, std::shared_ptr<std::map<CPos, CExprProcessor>> wrapped_sheet,
	              std::set<CPos>& visited_pos) override {
		CValue lhs_result = lhs_operand_->result(shift, wrapped_sheet, visited_pos),
				rhs_result = rhs_operand_->result(shift, wrapped_sheet, visited_pos);

		if (std::holds_alternative<double>(lhs_result) && std::holds_alternative<double>(rhs_result))
			return std::get<double>(lhs_result) == std::get<double>(rhs_result) ? 1. : 0.;

		else if (std::holds_alternative<std::string>(lhs_result) && std::holds_alternative<std::string>(rhs_result))
			return std::get<std::string>(lhs_result) == std::get<std::string>(rhs_result) ? 1. : 0.;

		return CValue();
	}
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief
 * Represents an inequality expression unit within a spreadsheet expression structure.
 *
 * This class extends CExprBiOperationUnit to specifically handle inequality operations between two operands,
 * which can be any valid expression units. It allows for the comparison of numeric values as well as strings,
 * effectively managing the inequality check of the results of the left-hand and right-hand side operand expressions.
 */
class CExprInequalityUnit : public CExprBiOperationUnit {
public:
	/**
     * @brief
     * Constructs a CExprInequalityUnit with specific left and right operands.
     *
     * Initializes the unit with pointers to two CExprUnit instances, which represent the left-hand side
     * and right-hand side operands of the inequality operation. This setup allows the inequality operation unit
     * to manage and apply its operation to the results of these operand expressions, determining if they are not equal.
     *
     * @param lhs_opd - Shared pointer to the CExprUnit that acts as the left-hand side operand.
     * @param rhs_opd - Shared pointer to the CExprUnit that acts as the right-hand side operand.
     */
	CExprInequalityUnit(std::shared_ptr<CExprUnit> lhs_opd, std::shared_ptr<CExprUnit> rhs_opd)
			: CExprBiOperationUnit(std::move(lhs_opd), std::move(rhs_opd)) {}

	/**
     * @brief
     * Default destructor.
     *
     * Ensures proper cleanup and deallocation of resources associated with this unit, if necessary.
     */
	~CExprInequalityUnit() override = default;

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Creates a new instance of this class and returns it as a shared pointer to CExprUnit.
     *
     * This method allows for creating a copy of the current inequality unit, encapsulating its
     * state in a new object. Useful for expression replication and manipulation without altering
     * the original object.
     *
     * @return std::shared_ptr<CExprUnit> pointing to the newly created copy of this inequality unit.
     */
	std::shared_ptr<CExprUnit> encapsulate() const override {
		return std::make_shared<CExprInequalityUnit>(*this);
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Serializes the inequality operation along with its operands to a string.
     *
     * Constructs a string representation of the inequality operation by wrapping the serialized forms
     * of the left and right operands with parentheses and placing a '!=' sign between them.
     *
     * @param shift - A pair of integers representing the column and row shifts to apply to any cell references within the operands.
     * @return std::string representing the serialized inequality operation.
     */
	std::string serialize(std::pair<int, int> shift) const override {
		return "(" + lhs_operand_->serialize(shift) + " != " + rhs_operand_->serialize(shift) + ")";
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Evaluates the result of the inequality operation and returns it as a CValue.
     *
     * Computes the values of both operands and applies the inequality operation. This operation checks if both operands are not equal,
     * supporting numeric comparisons as well as string comparisons. The result is 1 if they are not equal, 0 otherwise.
     *
     * @param shift - A pair of integers representing a notional shift to apply to relative references within the operands.
     * @param wrapped_sheet - Shared pointer to the map of CPos to CExprProcessor, representing the spreadsheet.
     * @param visited_pos - Set of visited positions for cycle detection during evaluation.
     * @return CValue containing 1 if the operands are not equal, 0 if they are, or an undefined value if the operands are not comparable.
     */
	CValue result(std::pair<int, int> shift, std::shared_ptr<std::map<CPos, CExprProcessor>> wrapped_sheet,
	              std::set<CPos>& visited_pos) override {
		CValue lhs_result = lhs_operand_->result(shift, wrapped_sheet, visited_pos),
				rhs_result = rhs_operand_->result(shift, wrapped_sheet, visited_pos);

		if (std::holds_alternative<double>(lhs_result) && std::holds_alternative<double>(rhs_result))
			return std::get<double>(lhs_result) != std::get<double>(rhs_result) ? 1. : 0.;

		else if (std::holds_alternative<std::string>(lhs_result) && std::holds_alternative<std::string>(rhs_result))
			return std::get<std::string>(lhs_result) != std::get<std::string>(rhs_result) ? 1. : 0.;

		return CValue();
	}
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief
 * Represents a less-than comparison expression unit within a spreadsheet expression structure.
 *
 * This class extends CExprBiOperationUnit to specifically handle less-than comparisons between two operands,
 * which can be any valid expression units. It allows for the comparison of numeric values as well as strings,
 * effectively managing the comparison of the results of the left-hand and right-hand side operand expressions.
 */
class CExprMinorityUnit : public CExprBiOperationUnit {
public:
	/**
     * @brief
     * Constructs a CExprMinorityUnit with specific left and right operands.
     *
     * Initializes the unit with pointers to two CExprUnit instances, which represent the left-hand side
     * and right-hand side operands of the less-than comparison operation. This setup allows the minority operation unit
     * to manage and apply its operation to the results of these operand expressions, determining if one is less than the other.
     *
     * @param lhs_opd - Shared pointer to the CExprUnit that acts as the left-hand side operand.
     * @param rhs_opd - Shared pointer to the CExprUnit that acts as the right-hand side operand.
     */
	CExprMinorityUnit(std::shared_ptr<CExprUnit> lhs_opd, std::shared_ptr<CExprUnit> rhs_opd)
			: CExprBiOperationUnit(std::move(lhs_opd), std::move(rhs_opd)) {}

	/**
     * @brief
     * Default destructor.
     *
     * Ensures proper cleanup and deallocation of resources associated with this unit, if necessary.
     */
	~CExprMinorityUnit() override = default;

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Creates a new instance of this class and returns it as a shared pointer to CExprUnit.
     *
     * This method allows for creating a copy of the current minority unit, encapsulating its
     * state in a new object. Useful for expression replication and manipulation without altering
     * the original object.
     *
     * @return std::shared_ptr<CExprUnit> pointing to the newly created copy of this minority unit.
     */
	std::shared_ptr<CExprUnit> encapsulate() const override {
		return std::make_shared<CExprMinorityUnit>(*this);
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Serializes the less-than comparison operation along with its operands to a string.
     *
     * Constructs a string representation of the less-than comparison by wrapping the serialized forms
     * of the left and right operands with parentheses and placing a '<' sign between them.
     *
     * @param shift - A pair of integers representing the column and row shifts to apply to any cell references within the operands.
     * @return std::string representing the serialized less-than comparison operation.
     */
	std::string serialize(std::pair<int, int> shift) const override {
		return "(" + lhs_operand_->serialize(shift) + " < " + rhs_operand_->serialize(shift) + ")";
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Evaluates the result of the less-than comparison operation and returns it as a CValue.
     *
     * Computes the values of both operands and applies the less-than comparison. This operation checks if the left operand is less than the right operand,
     * supporting numeric comparisons as well as string comparisons. The result is 1 if true, 0 otherwise.
     *
     * @param shift - A pair of integers representing a notional shift to apply to relative references within the operands.
     * @param wrapped_sheet - Shared pointer to the map of CPos to CExprProcessor, representing the spreadsheet.
     * @param visited_pos - Set of visited positions for cycle detection during evaluation.
     * @return CValue containing 1 if the left operand is less than the right operand, 0 otherwise, or an undefined value if the operands are not comparable.
     */
	CValue result(std::pair<int, int> shift, std::shared_ptr<std::map<CPos, CExprProcessor>> wrapped_sheet,
	              std::set<CPos>& visited_pos) override {
		CValue lhs_result = lhs_operand_->result(shift, wrapped_sheet, visited_pos),
				rhs_result = rhs_operand_->result(shift, wrapped_sheet, visited_pos);

		if (std::holds_alternative<double>(lhs_result) && std::holds_alternative<double>(rhs_result))
			return std::get<double>(lhs_result) < std::get<double>(rhs_result) ? 1. : 0.;

		else if (std::holds_alternative<std::string>(lhs_result) && std::holds_alternative<std::string>(rhs_result))
			return std::get<std::string>(lhs_result) < std::get<std::string>(rhs_result) ? 1. : 0.;

		return CValue();
	}
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief
 * Represents a less-than-or-equal-to comparison expression unit within a spreadsheet expression structure.
 *
 * This class extends CExprBiOperationUnit to specifically handle less-than-or-equal-to comparisons between two operands,
 * which can be any valid expression units. It allows for the comparison of numeric values as well as strings,
 * effectively managing the comparison of the results of the left-hand and right-hand side operand expressions
 * to determine if one is less than or equal to the other.
 */
class CExprMinorityEqualityUnit : public CExprBiOperationUnit {
public:
	/**
     * @brief
     * Constructs a CExprMinorityEqualityUnit with specific left and right operands.
     *
     * Initializes the unit with pointers to two CExprUnit instances, which represent the left-hand side
     * and right-hand side operands of the less-than-or-equal-to comparison operation. This setup allows the minority equality operation unit
     * to manage and apply its operation to the results of these operand expressions, determining if one is less than or equal to the other.
     *
     * @param lhs_opd - Shared pointer to the CExprUnit that acts as the left-hand side operand.
     * @param rhs_opd - Shared pointer to the CExprUnit that acts as the right-hand side operand.
     */
	CExprMinorityEqualityUnit(std::shared_ptr<CExprUnit> lhs_opd, std::shared_ptr<CExprUnit> rhs_opd)
			: CExprBiOperationUnit(std::move(lhs_opd), std::move(rhs_opd)) {}

	/**
     * @brief
     * Default destructor.
     *
     * Ensures proper cleanup and deallocation of resources associated with this unit, if necessary.
     */
	~CExprMinorityEqualityUnit() override = default;

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Creates a new instance of this class and returns it as a shared pointer to CExprUnit.
     *
     * This method allows for creating a copy of the current minority equality unit, encapsulating its
     * state in a new object. Useful for expression replication and manipulation without altering
     * the original object.
     *
     * @return std::shared_ptr<CExprUnit> pointing to the newly created copy of this minority equality unit.
     */
	std::shared_ptr<CExprUnit> encapsulate() const override {
		return std::make_shared<CExprMinorityEqualityUnit>(*this);
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Serializes the less-than-or-equal-to comparison operation along with its operands to a string.
     *
     * Constructs a string representation of the less-than-or-equal-to comparison by wrapping the serialized forms
     * of the left and right operands with parentheses and placing a '<=' sign between them.
     *
     * @param shift - A pair of integers representing the column and row shifts to apply to any cell references within the operands.
     * @return std::string representing the serialized less-than-or-equal-to comparison operation.
     */
	std::string serialize(std::pair<int, int> shift) const override {
		return "(" + lhs_operand_->serialize(shift) + " <= " + rhs_operand_->serialize(shift) + ")";
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Evaluates the result of the less-than-or-equal-to comparison operation and returns it as a CValue.
     *
     * Computes the values of both operands and applies the less-than-or-equal-to comparison. This operation checks if the left operand is less than or equal to the right operand,
     * supporting numeric comparisons as well as string comparisons. The result is 1 if true, 0 otherwise.
     *
     * @param shift - A pair of integers representing a notional shift to apply to relative references within the operands.
     * @param wrapped_sheet - Shared pointer to the map of CPos to CExprProcessor, representing the spreadsheet.
     * @param visited_pos - Set of visited positions for cycle detection during evaluation.
     * @return CValue containing 1 if the left operand is less than or equal to the right operand, 0 otherwise, or an undefined value if the operands are not comparable.
     */
	CValue result(std::pair<int, int> shift, std::shared_ptr<std::map<CPos, CExprProcessor>> wrapped_sheet,
	              std::set<CPos>& visited_pos) override {
		CValue lhs_result = lhs_operand_->result(shift, wrapped_sheet, visited_pos),
				rhs_result = rhs_operand_->result(shift, wrapped_sheet, visited_pos);

		if (std::holds_alternative<double>(lhs_result) && std::holds_alternative<double>(rhs_result))
			return std::get<double>(lhs_result) <= std::get<double>(rhs_result) ? 1. : 0.;

		else if (std::holds_alternative<std::string>(lhs_result) && std::holds_alternative<std::string>(rhs_result))
			return std::get<std::string>(lhs_result) <= std::get<std::string>(rhs_result) ? 1. : 0.;

		return CValue();
	}
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief
 * Represents a greater-than comparison expression unit within a spreadsheet expression structure.
 *
 * This class extends CExprBiOperationUnit to specifically handle greater-than comparisons between two operands,
 * which can be any valid expression units. It allows for the comparison of numeric values as well as strings,
 * effectively managing the comparison of the results of the left-hand and right-hand side operand expressions
 * to determine if one is greater than the other.
 */
class CExprMajorityUnit : public CExprBiOperationUnit {
public:
	/**
     * @brief
     * Constructs a CExprMajorityUnit with specific left and right operands.
     *
     * Initializes the unit with pointers to two CExprUnit instances, which represent the left-hand side
     * and right-hand side operands of the greater-than comparison operation. This setup allows the majority operation unit
     * to manage and apply its operation to the results of these operand expressions, determining if one is greater than the other.
     *
     * @param lhs_opd - Shared pointer to the CExprUnit that acts as the left-hand side operand.
     * @param rhs_opd - Shared pointer to the CExprUnit that acts as the right-hand side operand.
     */
	CExprMajorityUnit(std::shared_ptr<CExprUnit> lhs_opd, std::shared_ptr<CExprUnit> rhs_opd)
			: CExprBiOperationUnit(std::move(lhs_opd), std::move(rhs_opd)) {}

	/**
     * @brief
     * Default destructor.
     *
     * Ensures proper cleanup and deallocation of resources associated with this unit, if necessary.
     */
	~CExprMajorityUnit() override = default;

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Creates a new instance of this class and returns it as a shared pointer to CExprUnit.
     *
     * This method allows for creating a copy of the current majority unit, encapsulating its
     * state in a new object. Useful for expression replication and manipulation without altering
     * the original object.
     *
     * @return std::shared_ptr<CExprUnit> pointing to the newly created copy of this majority unit.
     */
	std::shared_ptr<CExprUnit> encapsulate() const override {
		return std::make_shared<CExprMajorityUnit>(*this);
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Serializes the greater-than comparison operation along with its operands to a string.
     *
     * Constructs a string representation of the greater-than comparison by wrapping the serialized forms
     * of the left and right operands with parentheses and placing a '>' sign between them.
     *
     * @param shift - A pair of integers representing the column and row shifts to apply to any cell references within the operands.
     * @return std::string representing the serialized greater-than comparison operation.
     */
	std::string serialize(std::pair<int, int> shift) const override {
		return "(" + lhs_operand_->serialize(shift) + " > " + rhs_operand_->serialize(shift) + ")";
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Evaluates the result of the greater-than comparison operation and returns it as a CValue.
     *
     * Computes the values of both operands and applies the greater-than comparison. This operation checks if the left operand is greater than the right operand,
     supporting numeric comparisons as well as string comparisons. The result is 1 if true, 0 otherwise.
     *
     * @param shift - A pair of integers representing a notional shift to apply to relative references within the operands.
     * @param wrapped_sheet - Shared pointer to the map of CPos to CExprProcessor, representing the spreadsheet.
     * @param visited_pos - Set of visited positions for cycle detection during evaluation.
     * @return CValue containing 1 if the left operand is greater than the right operand, 0 otherwise, or an undefined value if the operands are not comparable.
     */
	CValue result(std::pair<int, int> shift, std::shared_ptr<std::map<CPos, CExprProcessor>> wrapped_sheet,
	              std::set<CPos>& visited_pos) override {
		CValue lhs_result = lhs_operand_->result(shift, wrapped_sheet, visited_pos),
				rhs_result = rhs_operand_->result(shift, wrapped_sheet, visited_pos);

		if (std::holds_alternative<double>(lhs_result) && std::holds_alternative<double>(rhs_result))
			return std::get<double>(lhs_result) > std::get<double>(rhs_result) ? 1. : 0.;

		else if (std::holds_alternative<std::string>(lhs_result) && std::holds_alternative<std::string>(rhs_result))
			return std::get<std::string>(lhs_result) > std::get<std::string>(rhs_result) ? 1. : 0.;

		return CValue();
	}
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief
 * Represents a greater-than-or-equal-to comparison expression unit within a spreadsheet expression structure.
 *
 * This class extends CExprBiOperationUnit to specifically handle greater-than-or-equal-to comparisons between two operands,
 * which can be any valid expression units. It allows for the comparison of numeric values as well as strings,
 * effectively managing the comparison of the results of the left-hand and right-hand side operand expressions
 * to determine if one is greater than or equal to the other.
 */
class CExprMajorityEqualityUnit : public CExprBiOperationUnit {
public:
	/**
     * @brief
     * Constructs a CExprMajorityEqualityUnit with specific left and right operands.
     *
     * Initializes the unit with pointers to two CExprUnit instances, which represent the left-hand side
     * and right-hand side operands of the greater-than-or-equal-to comparison operation. This setup allows the majority equality operation unit
     * to manage and apply its operation to the results of these operand expressions, determining if one is greater than or equal to the other.
     *
     * @param lhs_opd - Shared pointer to the CExprUnit that acts as the left-hand side operand.
     * @param rhs_opd - Shared pointer to the CExprUnit that acts as the right-hand side operand.
     */
	CExprMajorityEqualityUnit(std::shared_ptr<CExprUnit> lhs_opd, std::shared_ptr<CExprUnit> rhs_opd)
			: CExprBiOperationUnit(std::move(lhs_opd), std::move(rhs_opd)) {}

	/**
     * @brief
     * Default destructor.
     *
     * Ensures proper cleanup and deallocation of resources associated with this unit, if necessary.
     */
	~CExprMajorityEqualityUnit() override = default;

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Creates a new instance of this class and returns it as a shared pointer to CExprUnit.
     *
     * This method allows for creating a copy of the current majority equality unit, encapsulating its
     * state in a new object. Useful for expression replication and manipulation without altering
     * the original object.
     *
     * @return std::shared_ptr<CExprUnit> pointing to the newly created copy of this majority equality unit.
     */
	std::shared_ptr<CExprUnit> encapsulate() const override {
		return std::make_shared<CExprMajorityEqualityUnit>(*this);
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Serializes the greater-than-or-equal-to comparison operation along with its operands to a string.
     *
     * Constructs a string representation of the greater-than-or-equal-to comparison by wrapping the serialized forms
     * of the left and right operands with parentheses and placing a '>=' sign between them.
     *
     * @param shift - A pair of integers representing the column and row shifts to apply to any cell references within the operands.
     * @return std::string representing the serialized greater-than-or-equal-to comparison operation.
     */
	std::string serialize(std::pair<int, int> shift) const override {
		return "(" + lhs_operand_->serialize(shift) + " >= " + rhs_operand_->serialize(shift) + ")";
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief
     * Evaluates the result of the greater-than-or-equal-to comparison operation and returns it as a CValue.
     *
     * Computes the values of both operands and applies the greater-than-or-equal-to comparison. This operation checks if the left operand is greater than or equal to the right operand,
     supporting numeric comparisons as well as string comparisons. The result is 1 if true, 0 otherwise.
     *
     * @param shift - A pair of integers representing a notional shift to apply to relative references within the operands.
     * @param wrapped_sheet - Shared pointer to the map of CPos to CExprProcessor, representing the spreadsheet.
     * @param visited_pos - Set of visited positions for cycle detection during evaluation.
     * @return CValue containing 1 if the left operand is greater than or equal to the right operand, 0 otherwise, or an undefined value if the operands are not comparable.
     */
	CValue result(std::pair<int, int> shift, std::shared_ptr<std::map<CPos, CExprProcessor>> wrapped_sheet,
	              std::set<CPos>& visited_pos) override {
		CValue lhs_result = lhs_operand_->result(shift, wrapped_sheet, visited_pos),
				rhs_result = rhs_operand_->result(shift, wrapped_sheet, visited_pos);

		if (std::holds_alternative<double>(lhs_result) && std::holds_alternative<double>(rhs_result))
			return std::get<double>(lhs_result) >= std::get<double>(rhs_result) ? 1. : 0.;

		else if (std::holds_alternative<std::string>(lhs_result) && std::holds_alternative<std::string>(rhs_result))
			return std::get<std::string>(lhs_result) >= std::get<std::string>(rhs_result) ? 1. : 0.;

		return CValue();
	}
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * Adds two operands from the stack and pushes the result back as an addition unit.
 */
void CExprProcessor::opAdd() {
	auto rhs = extractExprUnit_(), lhs = extractExprUnit_();

	pushExprUnit_(CExprAdditionUnit(lhs, rhs).encapsulate());
}

/**
 * Subtracts the second operand from the first, taken from the stack, and pushes the result back as a subtraction unit.
 */
void CExprProcessor::opSub() {
	auto rhs = extractExprUnit_(), lhs = extractExprUnit_();

	pushExprUnit_(CExprSubtractionUnit(lhs, rhs).encapsulate());
}

/**
 * Multiplies two operands from the stack and pushes the result back as a multiplication unit.
 */
void CExprProcessor::opMul() {
	auto rhs = extractExprUnit_(), lhs = extractExprUnit_();

	pushExprUnit_(CExprMultiplicationUnit(lhs, rhs).encapsulate());
}

/**
 * Divides the first operand by the second, both taken from the stack, and pushes the result back as a division unit.
 */
void CExprProcessor::opDiv() {
	auto rhs = extractExprUnit_(), lhs = extractExprUnit_();

	pushExprUnit_(CExprDivisionUnit(lhs, rhs).encapsulate());
}

/**
 * Raises the first operand to the power of the second, both taken from the stack, and pushes the result back as an exponentiation unit.
 */
void CExprProcessor::opPow() {
	auto rhs = extractExprUnit_(), lhs = extractExprUnit_();

	pushExprUnit_(CExprExponentiationUnit(lhs, rhs).encapsulate());
}

/**
 * Negates the top operand on the stack and pushes the result back as a negation unit.
 */
void CExprProcessor::opNeg() {
	pushExprUnit_(CExprNegationUnit(extractExprUnit_()).encapsulate());
}

/**
 * Compares two operands from the stack for equality and pushes the result back as an equality unit.
 */
void CExprProcessor::opEq() {
	auto rhs = extractExprUnit_(), lhs = extractExprUnit_();

	pushExprUnit_(CExprEqualityUnit(lhs, rhs).encapsulate());
}

/**
 * Compares two operands from the stack for inequality and pushes the result back as an inequality unit.
 */
void CExprProcessor::opNe() {
	auto rhs = extractExprUnit_(), lhs = extractExprUnit_();

	pushExprUnit_(CExprInequalityUnit(lhs, rhs).encapsulate());
}

/**
 * Checks if the first operand is less than the second, both taken from the stack, and pushes the result back as a minority unit.
 */
void CExprProcessor::opLt() {
	auto rhs = extractExprUnit_(), lhs = extractExprUnit_();

	pushExprUnit_(CExprMinorityUnit(lhs, rhs).encapsulate());
}

/**
 * Checks if the first operand is less than or equal to the second, both taken from the stack, and pushes the result back as a minority equality unit.
 */
void CExprProcessor::opLe() {
	auto rhs = extractExprUnit_(), lhs = extractExprUnit_();

	pushExprUnit_(CExprMinorityEqualityUnit(lhs, rhs).encapsulate());
}

/**
 * Checks if the first operand is greater than the second, both taken from the stack, and pushes the result back as a majority unit.
 */
void CExprProcessor::opGt() {
	auto rhs = extractExprUnit_(), lhs = extractExprUnit_();

	pushExprUnit_(CExprMajorityUnit(lhs, rhs).encapsulate());
}

/**
 * Checks if the first operand is greater than or equal to the second, both taken from the stack, and pushes the result back as a majority equality unit.
 */
void CExprProcessor::opGe() {
	auto rhs = extractExprUnit_(), lhs = extractExprUnit_();

	pushExprUnit_(CExprMajorityEqualityUnit(lhs, rhs).encapsulate());
}

// ---------------------------------------------------------------------------------------------------------------------

/**
 * Pushes a new number unit onto the stack, encapsulating a numerical value.
 */
void CExprProcessor::valNumber(double num) {
	pushExprUnit_(CExprNumberUnit(num).encapsulate());
}

/**
 * Pushes a new string unit onto the stack, encapsulating a string value.
 */
void CExprProcessor::valString(std::string str) {
	pushExprUnit_(CExprStringUnit(std::move(str)).encapsulate());
}

/**
 * Pushes a new reference unit onto the stack, encapsulating a cell reference.
 */
void CExprProcessor::valReference(std::string ref) {
	pushExprUnit_(CExprReferenceUnit(std::move(ref)).encapsulate());
}

void CExprProcessor::valRange(std::string rng) {}

// ---------------------------------------------------------------------------------------------------------------------

void CExprProcessor::funcCall(std::string fn_name, int par_cnt) {}

// ---------------------------------------------------------------------------------------------------------------------

/**
 * Computes the result of the expression on the top of the stack.
 *
 * Returns the result of the top expression unit using its encapsulated value and logic,
 * such as computing a numeric result, resolving a reference, or performing a dynamic expression evaluation.
 */
CValue CExprProcessor::result(std::shared_ptr<std::map<CPos, CExprProcessor>> wrapped_sheet,
                              std::set<CPos>& visited_pos) {
	return !processor_.empty() ?
	       processor_.top()->result(shift_, std::move(wrapped_sheet), visited_pos) : CValue();
}

// ---------------------------------------------------------------------------------------------------------------------

/**
 * Serializes the entire expression to a string that can be displayed or stored.
 *
 * Returns a string representation of the expression. If the expression includes references or complex expressions,
 * it prepends an '=' sign to indicate that the cell contains an expression rather than a static value.
 */
std::string CExprProcessor::serialize() const {
	std::string serialized_expression;

	if (!processor_.empty() && (processor_.top()->type() == "REF" || processor_.top()->type() == "EXPR"))
		serialized_expression += "=";

	if (!processor_.empty())
		serialized_expression += processor_.top()->serialize(shift_);

	return serialized_expression;
}

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @class CSpreadsheet
 * @brief Manages a spreadsheet with capabilities to handle expressions in cells including dependencies and serialization.
 *
 * The CSpreadsheet class allows for managing a grid where each cell can contain an expression. The class provides
 * methods for cell manipulation, expression evaluation, and spreadsheet I/O operations. It supports detecting cyclic
 * dependencies but can be extended to include additional functionalities such as function handling, optimized calculations,
 * and file operations.
 */
class CSpreadsheet {
public:
	/**
     * @brief Returns the capabilities of this spreadsheet instance.
     * Currently, only cyclic dependencies are handled.
     * @return A bitmask of features supported by the spreadsheet.
     */
	static unsigned capabilities() {
		return SPREADSHEET_CYCLIC_DEPS;
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief Constructor for creating an empty spreadsheet.
     */
	CSpreadsheet()
			: wrapped_sheet_(std::make_shared<std::map<CPos, CExprProcessor>>()) {}

	/**
     * @brief Copy constructor.
     * @param other The spreadsheet to copy from.
     */
	CSpreadsheet(const CSpreadsheet& other)
			: wrapped_sheet_(std::make_shared<std::map<CPos, CExprProcessor>>()) {
		for (auto [pos, processor] : * other.wrapped_sheet_)
			wrapped_sheet_->emplace(pos, processor);
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
     * @brief Assignment operator.
     * @param other The spreadsheet to assign from.
     * @return Reference to this spreadsheet.
     */
	CSpreadsheet& operator=(const CSpreadsheet& other) {
		if (this != & other) {
			wrapped_sheet_->clear();

			for (auto [pos, processor] : * other.wrapped_sheet_)
				wrapped_sheet_->emplace(pos, processor);
		}

		return * this;
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Loads spreadsheet data from an input stream, reconstructing the internal state of the spreadsheet.
	 * This method parses a serialized representation of the spreadsheet where each cell and its contents
	 * are encoded in a structured format. The expected format for each cell is:
	 *
	 *     [CellPosition](ContentLength) Content
	 *
	 * where `CellPosition` is the serialized cell position (e.g., "A1"), `ContentLength` is an integer specifying
	 * the length of the content string that follows, and `Content` is the actual expression or value stored in the cell.
	 *
	 * @param is The input stream to read data from, typically a file or a stringstream containing the spreadsheet data.
	 * @return true if the data was loaded successfully without any format violations, false otherwise.
	 */
	bool load(std::istream& is) {
		if (!is)
			return false;

		auto tmp_wrapped_sheet = std::make_shared<std::map<CPos, CExprProcessor>>();
		char chr;

		while (is.get(chr)) {
			if (chr != '[')
				return false;

			std::string parsed_pos;

			while (is.get(chr) && chr != ']')
				parsed_pos += chr;

			if (!is || chr != ']')
				return false;

			if (is.get() != '(')
				return false;

			int contents_length = 0;

			is >> contents_length;

			if (is.get() != ')')
				return false;

			if (is.get() != ' ')
				return false;

			std::string parsed_expression(contents_length, '\0');

			is.read(&parsed_expression[0], contents_length);

			if (is.get() != '\n' && !is.eof())
				return false;

			try {
				CPos pos(parsed_pos);
				if (!tmp_wrapped_sheet->emplace(pos, CExprProcessor()).second)
					return false;

				parseExpression(parsed_expression, tmp_wrapped_sheet->at(pos));
			} catch (...) {
				return false;
			}
		}

		wrapped_sheet_ = tmp_wrapped_sheet;

		return true;
	}

	/**
	 * @brief Saves the current state of the spreadsheet to an output stream in a serialized format.
	 * The serialization format mirrors the expected input format for loading and consists of entries for each cell:
	 *
	 *     [CellPosition](ContentLength) Content\n
	 *
	 * This method iterates over all cells in the spreadsheet, extracting the position and the serialized form of the expression
	 * in each cell, then formats them according to the serialization scheme described.
	 *
	 * @param os The output stream to write data to, typically a file or a stringstream.
	 * @return true if the data was saved successfully, false if an error occurred with the output stream.
	 */
	bool save(std::ostream& os) const {
		if (!os)
			return false;

		for (const auto& [pos, processor] : * wrapped_sheet_) {
			std::string serialized_pos = pos.serialize(), serialized_processor = processor.serialize();

			os << "[" << serialized_pos << "](" << serialized_processor.length() << ") "
			   << serialized_processor << "\n";
		}

		return true;
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Sets or replaces the contents of a cell with a new expression or value.
	 * This method modifies the cell at the specified position by inserting or updating the expression processor
	 * associated with that position. If the contents start with '=', it is treated as an expression; otherwise,
	 * it is treated as a direct value.
	 *
	 * @param pos The position of the cell to modify, specified as a CPos object.
	 * @param contents The new content for the cell, which may be an expression or a direct value.
	 * @return true if the cell was set successfully and the contents were valid, false if an error occurred, such as a parse error.
	 */
	bool setCell(CPos pos, std::string contents) {
		bool is_set = false;

		try {
			wrapped_sheet_->insert_or_assign(pos, CExprProcessor());
			parseExpression(std::move(contents), wrapped_sheet_->at(pos));

			is_set = true;
		} catch (...) {
			is_set = false;
		}

		return is_set;
	}

	/**
	 * @brief Retrieves the evaluated value of the specified cell.
	 * This method evaluates the expression or returns the value stored in the cell at the specified position.
	 * If the cell is not defined or the evaluation results in an error (such as a cyclic dependency), an undefined value is returned.
	 *
	 * @param pos The position of the cell whose value is to be retrieved.
	 * @return The evaluated value of the cell as a CValue, which could be undefined in case of errors.
	 */
	CValue getValue(CPos pos) {
		std::set<CPos> visited_pos;

		if (wrapped_sheet_->contains(pos))
			return wrapped_sheet_->at(pos).result(wrapped_sheet_, visited_pos);

		return CValue();
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Copies a rectangular block of cells from one area to another within the spreadsheet.
	 * This method is used to replicate the contents and expressions of a block of cells from a source area specified by `src`
	 * to a destination starting at `dst`. The dimensions of the block are defined by `w` (width) and `h` (height).
	 * Expressions are adjusted to maintain relative or absolute references appropriately.
	 *
	 * @param dst The starting position of the destination area, specified as a CPos object.
	 * @param src The starting position of the source area, also specified as a CPos object.
	 * @param w The width of the rectangle to be copied (number of columns).
	 * @param h The height of the rectangle to be copied (number of rows).
	 */
	void copyRect(CPos dst, CPos src, int w = 1, int h = 1) {
		std::map<CPos, CExprProcessor> tmp_sheet;

		for (int col_shift = 0; col_shift < w; ++col_shift) {
			for (int row_shift = 0; row_shift < h; ++row_shift) {
				auto src_pos = CPos(src.serialize(), {col_shift, row_shift});

				if (wrapped_sheet_->contains(src_pos)) {
					auto src_processor = wrapped_sheet_->at(src_pos);
					tmp_sheet.emplace(src_pos, src_processor);
				}
			}
		}

		auto dst_shift = std::make_pair(dst.numerizedIDs().first - src.numerizedIDs().first,
		                                dst.numerizedIDs().second - src.numerizedIDs().second);

		for (auto& [src_pos, src_processor] : tmp_sheet)
			wrapped_sheet_->insert_or_assign(CPos(src_pos.serialize(), dst_shift),
			                                 src_processor.setShift(dst_shift));
	}

private:
	std::shared_ptr<std::map<CPos, CExprProcessor>> wrapped_sheet_; // Stores all cells with their expressions.
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

#ifndef __PROGTEST__

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

bool valueMatch(const CValue& r, const CValue& s) {
	if (r.index() != s.index())
		return false;

	if (r.index() == 0)
		return true;

	if (r.index() == 2)
		return std::get<std::string>(r) == std::get<std::string>(s);

	if (std::isnan(std::get<double>(r)) && std::isnan(std::get<double>(s)))
		return true;

	if (std::isinf(std::get<double>(r)) && std::isinf(std::get<double>(s)))
		return
				(std::get<double>(r) < 0 && std::get<double>(s) < 0)
				|| (std::get<double>(r) > 0 && std::get<double>(s) > 0);

	return fabs(std::get<double>(r) - std::get<double>(s)) <= 1e8 * DBL_EPSILON * fabs(std::get<double>(r));
}

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

int main() {
	CSpreadsheet x0, x1;

	std::ostringstream oss;
	std::istringstream iss;

	std::string data;

	// -----------------------------------------------------------------------------------------------------------------

	assert(x0.setCell(CPos("A1"), "10"));
	assert(x0.setCell(CPos("A2"), "20.5"));
	assert(x0.setCell(CPos("A3"), "3e1"));
	assert(x0.setCell(CPos("A4"), "=40"));
	assert(x0.setCell(CPos("A5"), "=5e+1"));
	assert(x0.setCell(CPos("A6"), "raw text with any characters, including a quote \" or a newline\n"));
	assert(x0.setCell(CPos("A7"), "=\"quoted string, quotes must be doubled: \"\". Moreover, backslashes are needed for C++.\""));

	// -----------------------------------------------------------------------------------------------------------------

	assert(valueMatch(x0.getValue(CPos( "A1")), CValue(10.0)));
	assert(valueMatch(x0.getValue(CPos( "A2")), CValue(20.5)));
	assert(valueMatch(x0.getValue(CPos("A3")), CValue(30.0)));
	assert(valueMatch(x0.getValue(CPos( "A4")), CValue(40.0)));
	assert(valueMatch(x0.getValue(CPos( "A5")), CValue(50.0)));
	assert(valueMatch(x0.getValue(CPos( "A6")), CValue("raw text with any characters, including a quote \" or a newline\n")));
	assert(valueMatch(x0.getValue(CPos( "A7")), CValue("quoted string, quotes must be doubled: \". Moreover, backslashes are needed for C++.")));
	assert(valueMatch(x0.getValue(CPos( "A8")), CValue()));
	assert(valueMatch(x0.getValue(CPos( "AAAA9999")), CValue()));

	// -----------------------------------------------------------------------------------------------------------------

	assert(x0.setCell(CPos("B1"), "=A1+A2*A3"));
	assert(x0.setCell(CPos("B2"), "= -A1 ^ 2 - A2 / 2   "));
	assert(x0.setCell(CPos("B3"), "= 2 ^ $A$1"));
	assert(x0.setCell(CPos("B4"), "=($A1+A$2)^2"));
	assert(x0.setCell(CPos("B5"), "=B1+B2+B3+B4"));
	assert(x0.setCell(CPos("B6"), "=B1+B2+B3+B4+B5"));

	// -----------------------------------------------------------------------------------------------------------------

	assert(valueMatch(x0.getValue(CPos("B1")), CValue(625.0)));
	assert(valueMatch(x0.getValue(CPos("B2")), CValue(-110.25)));
	assert(valueMatch(x0.getValue(CPos("B3")), CValue(1024.0)));
	assert(valueMatch(x0.getValue(CPos("B4")), CValue(930.25)));
	assert(valueMatch(x0.getValue(CPos("B5")), CValue(2469.0)));
	assert(valueMatch(x0.getValue(CPos("B6")), CValue(4938.0)));

	// -----------------------------------------------------------------------------------------------------------------

	assert(x0.setCell(CPos("A1"), "12"));

	// -----------------------------------------------------------------------------------------------------------------

	assert(valueMatch(x0.getValue(CPos("B1")), CValue(627.0)));
	assert(valueMatch(x0.getValue(CPos("B2")), CValue(-154.25)));
	assert(valueMatch(x0.getValue(CPos("B3")), CValue(4096.0)));
	assert(valueMatch(x0.getValue(CPos("B4")), CValue(1056.25)));
	assert(valueMatch(x0.getValue(CPos("B5")), CValue(5625.0)));
	assert(valueMatch(x0.getValue(CPos("B6")), CValue(11250.0)));

	// -----------------------------------------------------------------------------------------------------------------

	x1 = x0;

	// -----------------------------------------------------------------------------------------------------------------

	assert(x0.setCell(CPos("A2"), "100"));
	assert(x1.setCell(CPos("A2"), "=A3+A5+A4"));

	// -----------------------------------------------------------------------------------------------------------------

	assert(valueMatch(x0.getValue(CPos("B1")), CValue(3012.0)));
	assert(valueMatch(x0.getValue(CPos("B2")), CValue(-194.0)));
	assert(valueMatch(x0.getValue(CPos("B3")), CValue(4096.0)));
	assert(valueMatch(x0.getValue(CPos("B4")), CValue(12544.0)));
	assert(valueMatch(x0.getValue(CPos("B5")), CValue(19458.0)));
	assert(valueMatch(x0.getValue(CPos("B6")), CValue(38916.0)));
	assert(valueMatch(x1.getValue(CPos("B1")), CValue(3612.0)));
	assert(valueMatch(x1.getValue(CPos("B2")), CValue(-204.0)));
	assert(valueMatch(x1.getValue(CPos("B3")), CValue(4096.0)));
	assert(valueMatch(x1.getValue(CPos("B4")), CValue(17424.0)));
	assert(valueMatch(x1.getValue(CPos("B5")), CValue(24928.0)));
	assert(valueMatch(x1.getValue(CPos("B6")), CValue(49856.0)));

	// -----------------------------------------------------------------------------------------------------------------

	oss.clear();
	oss.str("");

	// -----------------------------------------------------------------------------------------------------------------

	assert(x0.save(oss));

	// -----------------------------------------------------------------------------------------------------------------

	data = oss.str();

	iss.clear();
	iss.str(data);

	// -----------------------------------------------------------------------------------------------------------------

	assert(x1.load(iss));

	// -----------------------------------------------------------------------------------------------------------------

	assert(valueMatch(x1.getValue(CPos("B1")), CValue(3012.0)));
	assert(valueMatch(x1.getValue(CPos("B2")), CValue(-194.0)));
	assert(valueMatch(x1.getValue(CPos("B3")), CValue(4096.0)));
	assert(valueMatch(x1.getValue(CPos("B4")), CValue(12544.0)));
	assert(valueMatch(x1.getValue(CPos("B5")), CValue(19458.0)));
	assert(valueMatch(x1.getValue(CPos("B6")), CValue(38916.0)));

	// -----------------------------------------------------------------------------------------------------------------

	assert(x0.setCell(CPos("A3"), "4e1"));

	// -----------------------------------------------------------------------------------------------------------------

	assert(valueMatch(x1.getValue(CPos("B1")), CValue(3012.0)));
	assert(valueMatch(x1.getValue(CPos("B2")), CValue(-194.0)));
	assert(valueMatch(x1.getValue(CPos("B3")), CValue(4096.0)));
	assert(valueMatch(x1.getValue(CPos("B4")), CValue(12544.0)));
	assert(valueMatch(x1.getValue(CPos("B5")), CValue(19458.0)));
	assert(valueMatch(x1.getValue(CPos("B6")), CValue(38916.0)));

	// -----------------------------------------------------------------------------------------------------------------

	oss.clear();
	oss.str("");

	// -----------------------------------------------------------------------------------------------------------------

	assert(x0.save(oss));

	// -----------------------------------------------------------------------------------------------------------------

	data = oss.str();

	for (size_t i = 0; i<std::min<size_t>(data.length(), 10); i++)
		data[i] ^= 0x5a;

	iss.clear();
	iss.str(data);

	// -----------------------------------------------------------------------------------------------------------------

	assert(!x1.load(iss));

	// -----------------------------------------------------------------------------------------------------------------

	assert(x0.setCell(CPos("D0"), "10"));
	assert(x0.setCell(CPos("D1"), "20"));
	assert(x0.setCell(CPos("D2"), "30"));
	assert(x0.setCell(CPos("D3"), "40"));
	assert(x0.setCell(CPos("D4"), "50"));
	assert(x0.setCell(CPos("E0"), "60"));
	assert(x0.setCell(CPos("E1"), "70"));
	assert(x0.setCell(CPos("E2"), "80"));
	assert(x0.setCell(CPos("E3"), "90"));
	assert(x0.setCell(CPos("E4"), "100"));
	assert(x0.setCell(CPos("F10"), "=D0+5"));
	assert(x0.setCell(CPos("F11"), "=$D0+5"));
	assert(x0.setCell(CPos("F12"), "=D$0+5"));
	assert(x0.setCell(CPos("F13"), "=$D$0+5"));

	// -----------------------------------------------------------------------------------------------------------------

	x0.copyRect(CPos("G11"), CPos("F10"), 1, 4);

	// -----------------------------------------------------------------------------------------------------------------

	assert(valueMatch(x0.getValue(CPos("F10")), CValue(15.0)));
	assert(valueMatch(x0.getValue(CPos("F11")), CValue(15.0)));
	assert(valueMatch(x0.getValue(CPos("F12")), CValue(15.0)));
	assert(valueMatch(x0.getValue(CPos("F13")), CValue(15.0)));
	assert(valueMatch(x0.getValue(CPos("F14")), CValue()));
	assert(valueMatch(x0.getValue(CPos("G10")), CValue()));
	assert(valueMatch(x0.getValue(CPos("G11")), CValue(75.0)));
	assert(valueMatch(x0.getValue(CPos("G12")), CValue(25.0)));
	assert(valueMatch(x0.getValue(CPos("G13")), CValue(65.0)));
	assert(valueMatch(x0.getValue(CPos("G14")), CValue(15.0)));

	// -----------------------------------------------------------------------------------------------------------------

	x0.copyRect(CPos("G11"), CPos("F10"), 2, 4);

	// -----------------------------------------------------------------------------------------------------------------

	assert(valueMatch(x0.getValue(CPos("F10")), CValue(15.0)));
	assert(valueMatch(x0.getValue(CPos("F11")), CValue(15.0)));
	assert(valueMatch(x0.getValue(CPos("F12")), CValue(15.0)));
	assert(valueMatch(x0.getValue(CPos("F13")), CValue(15.0)));
	assert(valueMatch(x0.getValue(CPos("F14")), CValue()));
	assert(valueMatch(x0.getValue(CPos("G10")), CValue()));
	assert(valueMatch(x0.getValue(CPos("G11")), CValue(75.0)));
	assert(valueMatch(x0.getValue(CPos("G12")), CValue(25.0)));
	assert(valueMatch(x0.getValue(CPos("G13")), CValue(65.0)));
	assert(valueMatch(x0.getValue(CPos("G14")), CValue(15.0)));
	assert(valueMatch(x0.getValue(CPos("H10")), CValue()));
	assert(valueMatch(x0.getValue(CPos("H11")), CValue()));
	assert(valueMatch(x0.getValue(CPos("H12")), CValue()));
	assert(valueMatch(x0.getValue(CPos("H13")), CValue(35.0)));
	assert(valueMatch(x0.getValue(CPos("H14")), CValue()));

	// -----------------------------------------------------------------------------------------------------------------

	assert(x0.setCell(CPos("F0"), "-27"));

	// -----------------------------------------------------------------------------------------------------------------

	assert(valueMatch(x0.getValue(CPos("H14")), CValue(-22.0)));

	// -----------------------------------------------------------------------------------------------------------------

	x0.copyRect(CPos("H12"), CPos("H13"), 1, 2);

	// -----------------------------------------------------------------------------------------------------------------

	assert(valueMatch(x0.getValue(CPos("H12")), CValue(25.0)));
	assert(valueMatch(x0.getValue(CPos("H13")), CValue(-22.0)));
	assert(valueMatch(x0.getValue(CPos("H14")), CValue(-22.0)));

	// -----------------------------------------------------------------------------------------------------------------

	return EXIT_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

#endif /* __PROGTEST__ */
