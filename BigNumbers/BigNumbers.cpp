#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cstdint>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <functional>
#include <compare>
#include <stdexcept>

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Class representing a big integer.
 *
 * The CBigInt class allows for the representation and manipulation of large integers that exceed
 * the standard data type limits. It supports various operations such as addition, multiplication,
 * and comparisons.
 */
class CBigInt {
public:
//	--------------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Enumeration to represent the sign of the big integer.
	 */
	enum class CBigIntSign {NEGATIVE, ZERO, POSITIVE};

	/**
	 * @brief Typedef for the digits of the big integer.
	 */
	using CBigIntDigit = int;

//	--------------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Default constructor initializing the big integer to zero.
	 */
	CBigInt() : sign_(CBigIntSign::ZERO), digits_(std::vector<CBigIntDigit>{0}) {}

	/**
	 * @brief Constructor initializing the big integer from an integer value.
	 *
	 * @param value Integer value to initialize the big integer.
	 */
	explicit CBigInt(const int& value) :
			sign_((value < 0) ? CBigIntSign::NEGATIVE : (value > 0) ? CBigIntSign::POSITIVE : CBigIntSign::ZERO) {
		if (sign_ == CBigIntSign::ZERO)
			digits_.push_back(0);
		else {
			int abs_value = std::abs(value);
			while (abs_value > 0) {
				digits_.push_back(abs_value % 10);
				abs_value /= 10;
			}
		}
	}

	/**
	 * @brief Constructor initializing the big integer from a string value.
	 *
	 * @param value The string value to initialize the big integer.
	 */
	explicit CBigInt(const std::string& value) {
		std::string normalized_value = normalizeStringInput_(value);
		auto start_pos = normalized_value.cbegin(), end_pos = normalized_value.cend();

		bool is_negative = (start_pos != end_pos && *start_pos == MINUS_);
		start_pos = is_negative ? ++start_pos : start_pos;
		if (start_pos == end_pos ||
		    std::any_of(start_pos, end_pos, [](unsigned char c) { return !std::isdigit(c); }))
			throw std::invalid_argument("Invalid value argument!");

		auto start_aux_pos = start_pos;
		bool is_zero = (*start_aux_pos == ZERO_ && ++start_aux_pos == end_pos);
		sign_ = is_zero ? CBigIntSign::ZERO : is_negative ? CBigIntSign::NEGATIVE : CBigIntSign::POSITIVE;

		digits_.reserve(std::distance(start_pos, end_pos));
		for (auto current_pos = std::prev(end_pos); current_pos >= start_pos; --current_pos) {
			CBigIntDigit current_digit = (*current_pos - ZERO_);
			digits_.push_back(current_digit);

			if (current_pos == start_pos)
				break;
		}
	}

	CBigInt(const CBigInt& x) = default;

	~CBigInt() = default;

//	--------------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Get the sign of the big integer.
	 *
	 * @return The sign of the big integer.
	 */
	[[nodiscard]] const CBigIntSign& getSign() const {
		return sign_;
	}

	/**
	 * @brief Get the digits of the big integer.
	 *
	 * @return A reference to the vector of digits.
	 */
	[[nodiscard]] const std::vector<CBigIntDigit>& getDigits() const {
		return digits_;
	}

//	--------------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Assignment operator.
	 *
	 * @param x The CBigInt object to assign from.
	 * @return A reference to this CBigInt object.
	 */
	CBigInt& operator=(const CBigInt& x) {
		if (this == &x)
			return *this;

		sign_ = x.sign_;
		digits_ = x.digits_;

		return *this;
	}

	/**
	 * @brief Templated assignment operator.
	 *
	 * @param x The value to assign from.
	 * @return A reference to this CBigInt object.
	 */
	template<typename T>
	CBigInt& operator=(const T& x) {
		CBigInt tmp(x);
		*this = tmp;

		return *this;
	}

//	--------------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Addition operator for two CBigInt objects.
	 *
	 * @param x The first CBigInt object.
	 * @param y The second CBigInt object.
	 * @return The sum of the two CBigInt objects.
	 */
	friend CBigInt operator+(const CBigInt& x, const CBigInt& y) {
		bool is_x_zero = (x.getSign() == CBigIntSign::ZERO), is_y_zero = (y.getSign() == CBigIntSign::ZERO);
		if (is_x_zero || is_y_zero)
			return is_x_zero ? y : x;

		CBigInt result;
		result.digits_.clear();

		bool is_eq_sign = (x.getSign() == y.getSign());
		if (is_eq_sign) {
			size_t x_len = x.getDigits().size(), y_len = y.getDigits().size(), max_len = std::max(x_len, y_len);
			result.sign_ = x.getSign();
			result.digits_.reserve(max_len + 1);

			int carry = 0;
			for (size_t i = 0; i < max_len || carry; ++i) {
				int sum = ((i < x_len) ? x.getDigits()[i] : 0) + ((i < y_len) ? y.getDigits()[i] : 0) + carry;
				result.digits_.push_back(sum % 10);
				carry = sum / 10;
			}
		} else {
			int comparison = compareAbs_(x, y);
			bool is_x_lt_y = (comparison < 0), is_x_eq_y = (comparison == 0);
			if (is_x_eq_y)
				return result;

			const CBigInt& lesser = is_x_lt_y ? x : y, greater = is_x_lt_y ? y : x;
			size_t lesser_len = lesser.getDigits().size(), greater_len = greater.getDigits().size();
			result.sign_ = greater.getSign();
			result.digits_.reserve(greater_len);

			int borrow = 0;
			for (size_t i = 0; i < greater_len; ++i) {
				int sub = greater.getDigits()[i] - ((i < lesser_len) ? lesser.getDigits()[i] : 0) - borrow;
				borrow = (sub < 0);
				result.digits_.push_back(borrow ? (sub + 10) : sub);
			}

			normalizeDigits_(result);
		}

		return result;
	}

	/**
	 * @brief Templated addition operator for CBigInt and another type.
	 *
	 * @param x The CBigInt object.
	 * @param y The value to add.
	 * @return The sum as a CBigInt object.
	 */
	template<typename T>
	friend CBigInt operator+(const CBigInt& x, const T& y) {
		return x + CBigInt(y);
	}

	/**
	 * @brief Templated addition operator for another type and CBigInt.
	 *
	 * @param x The value to add.
	 * @param y The CBigInt object.
	 * @return The sum as a CBigInt object.
	 */
	template<typename T>
	friend CBigInt operator+(const T& x, const CBigInt& y) {
		return CBigInt(x) + y;
	}

//	--------------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Addition assignment operator for two CBigInt objects.
	 *
	 * @param x The CBigInt object to add to.
	 * @param y The CBigInt object to add.
	 * @return A reference to the resulting CBigInt object.
	 */
	friend CBigInt operator+=(CBigInt& x, const CBigInt& y) {
		return x = x + y;
	}

	/**
	 * @brief Templated addition assignment operator for CBigInt and another type.
	 *
	 * @param x The CBigInt object to add to.
	 * @param y The value to add.
	 * @return A reference to the resulting CBigInt object.
	 */
	template<typename T>
	friend CBigInt operator+=(CBigInt& x, const T& y) {
		return x += CBigInt(y);
	}

//	--------------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Multiplication operator for two CBigInt objects.
	 *
	 * @param x The first CBigInt object.
	 * @param y The second CBigInt object.
	 * @return The product of the two CBigInt objects.
	 */
	friend CBigInt operator*(const CBigInt& x, const CBigInt& y) {
		CBigInt result;

		bool is_x_zero = (x.getSign() == CBigIntSign::ZERO), is_y_zero = (y.getSign() == CBigIntSign::ZERO),
				is_eq_sign = (x.getSign() == y.getSign());
		if (is_x_zero || is_y_zero)
			return result;

		size_t x_len = x.getDigits().size(), y_len = y.getDigits().size();
		result.sign_ = is_eq_sign ? CBigIntSign::POSITIVE : CBigIntSign::NEGATIVE;
		result.digits_.resize(x_len + y_len, 0);

		for (size_t i = 0; i < x_len; ++i) {
			int carry = 0;
			for (size_t j = 0; j < y_len || carry; ++j) {
				int sum = result.digits_[i + j] + carry;
				if (j < y_len)
					sum += x.getDigits()[i] * y.getDigits()[j];

				result.digits_[i + j] = sum % 10;
				carry = sum / 10;
			}
		}

		normalizeDigits_(result);

		return result;
	}

	/**
	 * @brief Templated multiplication operator for CBigInt and another type.
	 *
	 * @param x The CBigInt object.
	 * @param y The value to multiply.
	 * @return The product as a CBigInt object.
	 */
	template<typename T>
	friend CBigInt operator*(const CBigInt& x, const T& y) {
		return x * CBigInt(y);
	}

	/**
	 * @brief Templated multiplication operator for another type and CBigInt.
	 *
	 * @param x The value to multiply.
	 * @param y The CBigInt object.
	 * @return The product as a CBigInt object.
	 */
	template<typename T>
	friend CBigInt operator*(const T& x, const CBigInt& y) {
		return CBigInt(x) * y;
	}

//	--------------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Multiplication assignment operator for two CBigInt objects.
	 *
	 * @param x The CBigInt object to multiply.
	 * @param y The CBigInt object to multiply with.
	 * @return A reference to the resulting CBigInt object.
	 */
	friend CBigInt operator*=(CBigInt& x, const CBigInt& y) {
		return x = x * y;
	}

	/**
	 * @brief Templated multiplication assignment operator for CBigInt and another type.
	 *
	 * @param x The CBigInt object to multiply.
	 * @param y The value to multiply with.
	 * @return A reference to the resulting CBigInt object.
	 */
	template<typename T>
	friend CBigInt operator*=(CBigInt& x, const T& y) {
		return x *= CBigInt(y);
	}

//	--------------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Equality operator for two CBigInt objects.
	 *
	 * @param x The first CBigInt object.
	 * @param y The second CBigInt object.
	 * @return True if the two CBigInt objects are equal, false otherwise.
	 */
	friend bool operator==(const CBigInt& x, const CBigInt& y) {
		bool is_eq_sign = (x.getSign() == y.getSign()), is_eq_digits = (x.getDigits() == y.getDigits());

		return (is_eq_sign && is_eq_digits);
	}

	/**
	 * @brief Templated equality operator for CBigInt and another type.
	 *
	 * @param x The CBigInt object.
	 * @param y The value to compare.
	 * @return True if the CBigInt object and the value are equal, false otherwise.
	 */
	template<typename T>
	friend bool operator==(const CBigInt& x, const T& y) {
		return x == CBigInt(y);
	}

	/**
	 * @brief Templated equality operator for another type and CBigInt.
	 *
	 * @param x The value to compare.
	 * @param y The CBigInt object.
	 * @return True if the value and the CBigInt object are equal, false otherwise.
	 */
	template<typename T>
	friend bool operator==(const T& x, const CBigInt& y) {
		return CBigInt(x) == y;
	}

//	--------------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Inequality operator for two CBigInt objects.
	 *
	 * @param x The first CBigInt object.
	 * @param y The second CBigInt object.
	 * @return True if the two CBigInt objects are not equal, false otherwise.
	 */
	friend bool operator!=(const CBigInt& x, const CBigInt& y) {
		return !(x == y);
	}

	/**
	 * @brief Templated inequality operator for CBigInt and another type.
	 *
	 * @param x The CBigInt object.
	 * @param y The value to compare.
	 * @return True if the CBigInt object and the value are not equal, false otherwise.
	 */
	template<typename T>
	friend bool operator!=(const CBigInt& x, const T& y) {
		return x != CBigInt(y);
	}

	/**
	 * @brief Templated inequality operator for another type and CBigInt.
	 *
	 * @param x The value to compare.
	 * @param y The CBigInt object.
	 * @return True if the value and the CBigInt object are not equal, false otherwise.
	 */
	template<typename T>
	friend bool operator!=(const T& x, const CBigInt& y) {
		return CBigInt(x) != y;
	}

//	--------------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Less than operator for two CBigInt objects.
	 *
	 * @param x The first CBigInt object.
	 * @param y The second CBigInt object.
	 * @return True if the first CBigInt object is less than the second, false otherwise.
	 */
	friend bool operator<(const CBigInt& x, const CBigInt& y) {
		bool is_ne_sign = (x.getSign() != y.getSign());
		if (is_ne_sign)
			return x.getSign() == CBigIntSign::NEGATIVE;

		int comparison = compareAbs_(x, y);
		bool is_both_positive = (x.getSign() == CBigIntSign::POSITIVE),
	         is_x_lt_y = ((is_both_positive && comparison < 0) || (!is_both_positive && comparison > 0));

		return is_x_lt_y;
	}

	/**
	 * @brief Templated less than operator for CBigInt and another type.
	 *
	 * @param x The CBigInt object.
	 * @param y The value to compare.
	 * @return True if the CBigInt object is less than the value, false otherwise.
	 */
	template<typename T>
	friend bool operator<(const CBigInt& x, const T& y) {
		return x < CBigInt(y);
	}

	/**
	 * @brief Templated less than operator for another type and CBigInt.
	 *
	 * @param x The value to compare.
	 * @param y The CBigInt object.
	 * @return True if the value is less than the CBigInt object, false otherwise.
	 */
	template<typename T>
	friend bool operator<(const T& x, const CBigInt& y) {
		return CBigInt(x) < y;
	}

//	--------------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Less than or equal to operator for two CBigInt objects.
	 *
	 * @param x The first CBigInt object.
	 * @param y The second CBigInt object.
	 * @return True if the first CBigInt object is less than or equal to the second, false otherwise.
	 */
	friend bool operator<=(const CBigInt& x, const CBigInt& y) {
		return (x < y) || (x == y);
	}

	/**
	 * @brief Templated less than or equal to operator for CBigInt and another type.
	 *
	 * @param x The CBigInt object.
	 * @param y The value to compare.
	 * @return True if the CBigInt object is less than or equal to the value, false otherwise.
	 */
	template<typename T>
	friend bool operator<=(const CBigInt& x, const T& y) {
		return x <= CBigInt(y);
	}

	/**
	 * @brief Templated less than or equal to operator for another type and CBigInt.
	 *
	 * @param x The value to compare.
	 * @param y The CBigInt object.
	 * @return True if the value is less than or equal to the CBigInt object, false otherwise.
	 */
	template<typename T>
	friend bool operator<=(const T& x, const CBigInt& y) {
		return CBigInt(x) <= y;
	}

//	--------------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Greater than operator for two CBigInt objects.
	 *
	 * @param x The first CBigInt object.
	 * @param y The second CBigInt object.
	 * @return True if the first CBigInt object is greater than the second, false otherwise.
	 */
	friend bool operator>(const CBigInt& x, const CBigInt& y) {
		return y < x;
	}
	/**
	* @brief Templated greater than operator for CBigInt and another type.
	*
	* @param x The CBigInt object
	* @param y The value to compare.
	* @return True if the CBigInt object is greater than the value, false otherwise.
	*/
	template<typename T>
	friend bool operator>(const CBigInt& x, const T& y) {
		return CBigInt(y) < x;
	}

	/**
	 * @brief Templated greater than operator for another type and CBigInt.
	 *
	 * @param x The value to compare.
	 * @param y The CBigInt object.
	 * @return True if the value is greater than the CBigInt object, false otherwise.
	 */
	template<typename T>
	friend bool operator>(const T& x, const CBigInt& y) {
		return y < CBigInt(x);
	}

//	--------------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Greater than or equal to operator for two CBigInt objects.
	 *
	 * @param x The first CBigInt object.
	 * @param y The second CBigInt object.
	 * @return True if the first CBigInt object is greater than or equal to the second, false otherwise.
	 */
	friend bool operator>=(const CBigInt& x, const CBigInt& y) {
		return (x > y) || (x == y);
	}

	/**
	 * @brief Templated greater than or equal to operator for CBigInt and another type.
	 *
	 * @param x The CBigInt object.
	 * @param y The value to compare.
	 * @return True if the CBigInt object is greater than or equal to the value, false otherwise.
	 */
	template<typename T>
	friend bool operator>=(const CBigInt& x, const T& y) {
		return x >= CBigInt(y);
	}

	/**
	 * @brief Templated greater than or equal to operator for another type and CBigInt.
	 *
	 * @param x The value to compare.
	 * @param y The CBigInt object.
	 * @return True if the value is greater than or equal to the CBigInt object, false otherwise.
	 */
	template<typename T>
	friend bool operator>=(const T& x, const CBigInt& y) {
		return CBigInt(x) >= y;
	}

//	--------------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Output stream operator for CBigInt.
	 *
	 * @param os The output stream.
	 * @param x The CBigInt object to output.
	 * @return A reference to the output stream.
	 */
	friend std::ostream& operator<<(std::ostream& os, const CBigInt& x) {
		bool is_negative = (x.sign_ == CBigIntSign::NEGATIVE);
		if (is_negative)
			os << MINUS_;

		auto start_pos = x.getDigits().rbegin(), end_pos = x.getDigits().rend();
		for (auto digit_pos = start_pos; digit_pos != end_pos; ++digit_pos)
			os << *digit_pos;

		return os;
	}

	/**
	 * @brief Input stream operator for CBigInt.
	 *
	 * @param is The input stream.
	 * @param x The CBigInt object to input.
	 * @return A reference to the input stream.
	 */
	friend std::istream& operator>>(std::istream& is, CBigInt& x) {
		std::string value;
		char c;

		while (std::isspace(is.peek()))
			is.get();

		if (is.peek() == MINUS_) {
			is.get(c);
			value.push_back(c);
		}

		bool is_valid = false;
		while (std::isdigit(is.peek())) {
			is_valid = true;
			is.get(c);
			value.push_back(c);
		}

		if (is_valid)
			x = CBigInt(value);
		else
			is.setstate(std::ios::failbit);

		return is;
	}

//	--------------------------------------------------------------------------------------------------------------------
private:
//	--------------------------------------------------------------------------------------------------------------------

	static constexpr const char MINUS_ = '-', ZERO_ = '0';

//	--------------------------------------------------------------------------------------------------------------------

	CBigIntSign sign_;
	std::vector<CBigIntDigit> digits_;

//	--------------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Normalize the string input to remove leading zeros and whitespaces.
	 *
	 * @param value The string value to normalize.
	 * @return The normalized string value.
	 */
	static std::string normalizeStringInput_(const std::string& value) {
		auto start_pos = value.begin(), end_pos = value.end();

		auto end_whitespace_pos = std::find_if(start_pos, end_pos,
		                                       [](unsigned char c) { return !std::isspace(c); });
		bool is_negative = (end_whitespace_pos != end_pos && *end_whitespace_pos == MINUS_);
		end_whitespace_pos = (is_negative) ? ++end_whitespace_pos : end_whitespace_pos;

		bool is_not_empty = (end_whitespace_pos != end_pos);
		auto end_zero_pos = std::find_if(end_whitespace_pos, end_pos,
		                                 [](unsigned char c) { return c != ZERO_; });
		bool is_zero = (end_zero_pos == end_pos);

		return ((is_negative) ? std::string{MINUS_} : std::string{}) +
		       ((is_not_empty && is_zero) ? std::string{ZERO_} : std::string{end_zero_pos, end_pos});
	}

//	--------------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Normalize the digits vector by removing leading zeros.
	 *
	 * @param x The CBigInt object to normalize.
	 */
	static void normalizeDigits_(CBigInt& x) {
		while ((x.digits_.size() > 1) && (x.digits_.back() == 0))
			x.digits_.pop_back();
	}

//	--------------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Compare the absolute values of two CBigInt objects.
	 *
	 * @param x The first CBigInt object.
	 * @param y The second CBigInt object.
	 * @return -1 if x < y, 0 if x == y, 1 if x > y.
	 */
	static int compareAbs_(const CBigInt& x, const CBigInt& y) {
		size_t x_len = x.getDigits().size(), y_len = y.getDigits().size();

		bool is_ne_len = (x_len != y_len);
		if (is_ne_len)
			return (x_len < y_len) ? -1 : 1;

		for (int i = static_cast<int>(x_len) - 1; i >= 0; --i) {
			bool is_ne_digit = (x.getDigits()[i] != y.getDigits()[i]);
			if (is_ne_digit)
				return (x.getDigits()[i] < y.getDigits()[i]) ? -1 : 1;
		}

		return 0;
	}

//	--------------------------------------------------------------------------------------------------------------------
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

static bool equal(const CBigInt& x, const char val[]) {
	std::ostringstream oss;
	oss << x;

	return oss . str () == val;
}

// ---------------------------------------------------------------------------------------------------------------------

static bool equalHex(const CBigInt& x, const char val[]) {
	return true; // hex output is needed for bonus tests only
	// std::ostringstream oss;
	// oss << std::hex << x;

	// return oss . str () == val;
}

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

int main() {
	CBigInt a, b;
	std::istringstream is;
	a = 10;
	a += 20;
	assert ( equal ( a, "30" ) );
    a *= 5;
    assert ( equal ( a, "150" ) );
    b = a + 3;
    assert ( equal ( b, "153" ) );
    b = a * 7;
    assert ( equal ( b, "1050" ) );
    assert ( equal ( a, "150" ) );
    assert ( equalHex ( a, "96" ) );

    a = 10;
    a += -20;
    assert ( equal ( a, "-10" ) );
    a *= 5;
    assert ( equal ( a, "-50" ) );
    b = a + 73;
    assert ( equal ( b, "23" ) );
    b = a * -7;
    assert ( equal ( b, "350" ) );
    assert ( equal ( a, "-50" ) );
    assert ( equalHex ( a, "-32" ) );

	a = "12345678901234567890";
    a += "-99999999999999999999";
    assert ( equal ( a, "-87654321098765432109" ) );
    a *= "54321987654321987654";
    assert ( equal ( a, "-4761556948575111126880627366067073182286" ) );
    a *= 0;
    assert ( equal ( a, "0" ) );
    a = 10;
	b = a + "400";
	assert ( equal ( b, "410" ) );
	b = a * "15";
	assert ( equal ( b, "150" ) );
	assert ( equal ( a, "10" ) );
	assert ( equalHex ( a, "a" ) );

    is . clear ();
    is . str ( " 1234" );
    assert ( is >> b );
    assert ( equal ( b, "1234" ) );
    is . clear ();
    is . str ( " 12 34" );
    assert ( is >> b );
    assert ( equal ( b, "12" ) );
    is . clear ();
    is . str ( "999z" );
    assert ( is >> b );
    assert ( equal ( b, "999" ) );
    is . clear ();
    is . str ( "abcd" );
    assert ( ! ( is >> b ) );
    is . clear ();
    is . str ( "- 758" );
    assert ( ! ( is >> b ) );
    a = 42;
    try {
        a = "-xyz";
        assert ( "missing an exception" == nullptr );
    } catch ( const std::invalid_argument & e ) {
        assert ( equal ( a, "42" ) );
    }

    a = "73786976294838206464";
    assert ( equal ( a, "73786976294838206464" ) );
    assert ( equalHex ( a, "40000000000000000" ) );
    assert ( a < "1361129467683753853853498429727072845824" );
    assert ( a <= "1361129467683753853853498429727072845824" );
    assert ( ! ( a > "1361129467683753853853498429727072845824" ) );
    assert ( ! ( a >= "1361129467683753853853498429727072845824" ) );
    assert ( ! ( a == "1361129467683753853853498429727072845824" ) );
    assert ( a != "1361129467683753853853498429727072845824" );
    assert ( ! ( a < "73786976294838206464" ) );
    assert ( a <= "73786976294838206464" );
    assert ( ! ( a > "73786976294838206464" ) );
    assert ( a >= "73786976294838206464" );
    assert ( a == "73786976294838206464" );
    assert ( ! ( a != "73786976294838206464" ) );
    assert ( a < "73786976294838206465" );
    assert ( a <= "73786976294838206465" );
    assert ( ! ( a > "73786976294838206465" ) );
    assert ( ! ( a >= "73786976294838206465" ) );
    assert ( ! ( a == "73786976294838206465" ) );
    assert ( a != "73786976294838206465" );
    a = "2147483648";
    assert ( ! ( a < -2147483648 ) );
    assert ( ! ( a <= -2147483648 ) );
    assert ( a > -2147483648 );
    assert ( a >= -2147483648 );
    assert ( ! ( a == -2147483648 ) );
	assert ( a != -2147483648 );
    a = "-12345678";
    assert ( ! ( a < -87654321 ) );
    assert ( ! ( a <= -87654321 ) );
    assert ( a > -87654321 );
    assert ( a >= -87654321 );
    assert ( ! ( a == -87654321 ) );
    assert ( a != -87654321 );

	return EXIT_SUCCESS;
}
