#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <vector>
#include <set>
#include <list>
#include <map>
#include <array>
#include <deque>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <compare>
#include <algorithm>
#include <cassert>
#include <memory>
#include <iterator>
#include <functional>
#include <stdexcept>

// ---------------------------------------------------------------------------------------------------------------------

using namespace std::literals;

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Class representing a dummy character.
 */
class CDummy {
private:
	char m_C;

public:
	CDummy(char c)
			: m_C(c) {}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Equality operator for CDummy.
	 * @param other The other CDummy object to compare.
	 * @return True if equal, false otherwise.
	 */
    bool operator==(const CDummy& other) const = default;

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Output stream operator for CDummy.
	 * @param os Output stream.
	 * @param x CDummy object.
	 * @return Reference to the output stream.
	 */
    friend std::ostream& operator<<(std::ostream& os, const CDummy& x) {
		return os << '\'' << x.m_C << '\'';
    }
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

 #define TEST_EXTRA_INTERFACE

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Class to perform self-match on a sequence of elements.
 * @tparam T Type of elements in the sequence.
 */
template <typename T>
class CSelfMatch {
private:
	std::vector<T> sequence_;

	// -----------------------------------------------------------------------------------------------------------------

	std::map<size_t, std::map<size_t, std::set<size_t>>> subsequence_occurrences_;

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Calculate the occurrences of all subsequences in the sequence.
	 */
	void calculateSubsequenceOccurrences() {
		subsequence_occurrences_.clear();

		size_t sequence_length = sequence_.size();

		for (size_t length = 1; length <= sequence_length; ++length)
			for (size_t begin = 0, end = begin + length - 1; begin + length <= sequence_length; ++begin, ++end) {
				bool is_occurred = false;

				for (auto& [boundary, occurrences] : subsequence_occurrences_[length]) {
					if (equal(sequence_.begin() + boundary, sequence_.begin() + boundary + length,
					          sequence_.begin() + begin, sequence_.begin() + end + 1)) {
						is_occurred = true;
						occurrences.insert(begin);

						break;
					}
				}

				if (!is_occurred)
					subsequence_occurrences_[length][begin] = {begin};
			}
	}

public:
	/**
	 * @brief Constructor using initializer list.
	 * @param il Initializer list of elements.
	 */
    CSelfMatch(const std::initializer_list<T>& il)
			: sequence_(il), subsequence_occurrences_() {
		calculateSubsequenceOccurrences();
	}

	/**
	 * @brief Constructor using an iterable container.
	 * @tparam IterableContainer Type of the iterable container.
	 * @param ic Iterable container of elements.
	 */
    template <typename IterableContainer>
    CSelfMatch(const IterableContainer& ic)
			: sequence_(ic.begin(), ic.end()), subsequence_occurrences_() {
	    calculateSubsequenceOccurrences();
	}

	/**
	 * @brief Constructor using iterators.
	 * @tparam Iterator Type of the iterator.
	 * @param begin Beginning iterator.
	 * @param end End iterator.
	 */
    template <typename Iterator>
    CSelfMatch(const Iterator& begin, const Iterator& end)
			: sequence_(begin, end), subsequence_occurrences_() {
	    calculateSubsequenceOccurrences();
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Add elements to the end of the sequence.
	 * @tparam Args Variadic template arguments.
	 * @param args Elements to add.
	 */
	template <typename... Args>
	void push_back(Args&&... args) {
		(sequence_.push_back(std::forward<Args>(args)), ...);

		calculateSubsequenceOccurrences();
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Get the length of the longest subsequence that occurs at least N times.
	 * @param N Minimum number of occurrences.
	 * @return Length of the longest subsequence.
	 */
    [[nodiscard]] size_t sequenceLen(size_t N) const {
	    if (N < 1)
		    throw std::invalid_argument("Invalid argument: condition n >= 1 is not met!");

	    size_t max_subsequence_length = 0;

	    for (const auto& [subsequence_length, subsequence_data] : subsequence_occurrences_)
		    for (const auto& [subsequence_boundary, subsequence_occurrences] : subsequence_data)
			    if (subsequence_occurrences.size() >= N)
				    max_subsequence_length = std::max(max_subsequence_length, subsequence_length);


	    return max_subsequence_length;
    }

	/**
	 * @brief Find all occurrences of the longest subsequence that occurs at least N times.
	 * @tparam N Minimum number of occurrences.
	 * @return Vector of arrays representing the positions of the subsequences.
	 */
    template <size_t N>
    [[nodiscard]] std::vector<std::array<size_t, N>> findSequences() const {
	    if (N < 1)
		    throw std::invalid_argument("Invalid argument: condition n >= 1 is not met!");

	    size_t max_subsequence_length = sequenceLen(N);

	    std::vector<std::array<size_t, N>> subsequence_occurrence_combos;

	    if (max_subsequence_length > 0)
		    for (const auto& [subsequence_boundary, subsequence_occurrences] :
					subsequence_occurrences_.at(max_subsequence_length))
			    if (subsequence_occurrences.size() >= N) {
				    std::vector<size_t> boundaries(subsequence_occurrences.begin(), subsequence_occurrences.end());
				    std::set<std::vector<size_t>> unique_subsequence_occurrence_combos;

				    do {
					    if (auto boundaries_permutation = std::vector<size_t>(boundaries.begin(), boundaries.begin() + N);
								std::is_sorted(boundaries_permutation.begin(), boundaries_permutation.end()) &&
								unique_subsequence_occurrence_combos.insert(boundaries_permutation).second) {
						    std::array<size_t, N> subsequence_occurrence_combo{};
						    std::copy(boundaries_permutation.begin(), boundaries_permutation.end(), subsequence_occurrence_combo.begin());

						    subsequence_occurrence_combos.push_back(subsequence_occurrence_combo);
					    }
				    } while (std::next_permutation(boundaries.begin(), boundaries.end()));
			    }

	    return subsequence_occurrence_combos;
	}
};

template <typename IterableContainer>
CSelfMatch(const IterableContainer&) -> CSelfMatch<typename IterableContainer::value_type>;

template <typename Iterator>
CSelfMatch(const Iterator&, const Iterator&) -> CSelfMatch<typename std::iterator_traits<Iterator>::value_type>;

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Compare two vectors of arrays to check if they match in position.
 * @tparam N Size of the arrays.
 * @param a First vector of arrays.
 * @param b Second vector of arrays.
 * @return True if the positions match, false otherwise.
 */
template<size_t N>
bool positionMatch(std::vector<std::array<size_t, N>> a, std::vector<std::array<size_t, N>> b) {
	std::sort(a.begin(), a.end());
	std::sort(b.begin(), b.end());

	return a == b;
}

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

int main() {
	CSelfMatch<char> x0("aaaaaaaaaaa"s);

	assert(x0.sequenceLen(2) == 10);
	assert(positionMatch(x0.findSequences<2>(), std::vector<std::array<size_t, 2>>{{0, 1}}));

	// -----------------------------------------------------------------------------------------------------------------

	CSelfMatch<char> x1("abababababa"s);

	assert(x1.sequenceLen(2) == 9);
	assert(positionMatch(x1.findSequences<2>(), std::vector<std::array<size_t, 2>>{{0, 2}}));

	// -----------------------------------------------------------------------------------------------------------------

	CSelfMatch<char> x2("abababababab"s);

	assert(x2.sequenceLen(2) == 10);
	assert(positionMatch(x2.findSequences<2>(), std::vector<std::array<size_t, 2> >{{0, 2}}));

	// -----------------------------------------------------------------------------------------------------------------

	CSelfMatch<char> x3("aaaaaaaaaaa"s);

	assert(x3.sequenceLen(3) == 9);
	assert(positionMatch(x3.findSequences<3>(), std::vector<std::array<size_t, 3>>{{0, 1, 2}}));

	// -----------------------------------------------------------------------------------------------------------------

	CSelfMatch<char> x4("abababababa"s);

	assert(x4.sequenceLen(3) == 7);
	assert(positionMatch(x4.findSequences<3>(), std::vector<std::array<size_t, 3>>{{0, 2, 4}}));

	// -----------------------------------------------------------------------------------------------------------------

	CSelfMatch<char> x5("abababababab"s);

	assert(x5.sequenceLen(3) == 8);
	assert(positionMatch(x5.findSequences<3>(), std::vector<std::array<size_t, 3>>{{0, 2, 4}}));

	// -----------------------------------------------------------------------------------------------------------------

	CSelfMatch<char> x6("abcdXabcd"s);

	assert(x6.sequenceLen(1) == 9);
	assert(positionMatch(x6.findSequences<1>(), std::vector<std::array<size_t, 1>>{{0}}));

	// -----------------------------------------------------------------------------------------------------------------

	CSelfMatch<char> x7("abcdXabcd"s);

	assert(x7.sequenceLen(2) == 4);
	assert(positionMatch(x7.findSequences<2>(), std::vector<std::array<size_t, 2>>{{0, 5}}));

	// -----------------------------------------------------------------------------------------------------------------

	CSelfMatch<char> x8("abcdXabcdeYabcdZabcd"s);

	assert(x8.sequenceLen(2) == 4);
	assert(positionMatch(x8.findSequences<2>(), std::vector<std::array<size_t, 2>>{{0, 5}, {0, 11}, {0, 16}, {5, 11}, {5, 16}, {11, 16}}));

	// -----------------------------------------------------------------------------------------------------------------

	CSelfMatch<char> x9("abcdXabcdYabcd"s);

	assert(x9.sequenceLen(3) == 4);
	assert(positionMatch(x9.findSequences<3>(), std::vector<std::array<size_t, 3>>{{0, 5, 10}}));

	// -----------------------------------------------------------------------------------------------------------------

	CSelfMatch<char> x10("abcdefghijklmn"s);

	assert(x10.sequenceLen(2) == 0);
	assert(positionMatch(x10.findSequences<2>(), std::vector<std::array<size_t, 2>>{}));

	// -----------------------------------------------------------------------------------------------------------------

	CSelfMatch<char> x11("abcXabcYabcZdefXdef"s);

	assert(x11.sequenceLen(2) == 3);
	assert(positionMatch(x11.findSequences<2>(), std::vector<std::array<size_t, 2>>{{0, 4}, {0, 8}, {4, 8}, {12, 16}}));

	// -----------------------------------------------------------------------------------------------------------------

	CSelfMatch<int> x12{1, 2, 3, 1, 2, 4, 1, 2};

	assert(x12.sequenceLen(2) == 2);
	assert(positionMatch(x12.findSequences<2>(), std::vector<std::array<size_t, 2>>{{0, 3}, {0, 6}, {3, 6}}));

	assert(x12.sequenceLen(3) == 2);
	assert(positionMatch(x12.findSequences<3>(), std::vector<std::array<size_t, 3>>{{0, 3, 6}}));

	// -----------------------------------------------------------------------------------------------------------------

	std::initializer_list<CDummy> init13{'a', 'b', 'c', 'd', 'X', 'a', 'b', 'c', 'd', 'Y', 'a', 'b', 'c', 'd'};
	CSelfMatch<CDummy> x13(init13.begin(), init13.end());

	assert(x13.sequenceLen(2) == 4);
	assert(positionMatch(x13.findSequences<2>(), std::vector<std::array<size_t, 2>>{{0, 5}, {0, 10}, {5, 10}}));

	// -----------------------------------------------------------------------------------------------------------------

	std::initializer_list<int> init14{1, 2, 1, 1, 2, 1, 0, 0, 1, 2, 1, 0, 1, 2, 0, 1, 2, 0, 1, 1, 1, 2, 0, 2, 0, 1, 2, 1, 0};
	CSelfMatch<int> x14(init14.begin(), init14.end());

	assert(x14.sequenceLen(2) == 5);
	assert(positionMatch(x14.findSequences<2>(), std::vector<std::array<size_t, 2>>{{11, 14}, {7, 24}}));

	// -----------------------------------------------------------------------------------------------------------------

	std::initializer_list<int> init15{1, 2, 1, 1, 2, 1, 0, 0, 1, 2, 1, 0, 1, 2, 0, 1, 2, 0, 1, 1, 1, 2, 0, 2, 0, 1, 2, 1, 0};
	CSelfMatch<int> x15(init15.begin(), init15.end());

	assert(x15.sequenceLen(3) == 4);
	assert(positionMatch(x15.findSequences<3>(), std::vector<std::array<size_t, 3>>{{3, 8, 25}}));

	// -----------------------------------------------------------------------------------------------------------------

	#ifdef TEST_EXTRA_INTERFACE

		CSelfMatch y0("aaaaaaaaaaa"s);

		assert(y0.sequenceLen(2) == 10);

		// -------------------------------------------------------------------------------------------------------------

		std::string s1("abcd");
		CSelfMatch y1(s1.begin(), s1.end());

		assert(y1.sequenceLen(2) == 0);

		// -------------------------------------------------------------------------------------------------------------

		CSelfMatch y2(""s);
		y2.push_back('a', 'b', 'c', 'X');
		y2.push_back('a');
		y2.push_back('b', 'c');

		assert(y2.sequenceLen(2) == 3);

	#endif /* TEST_EXTRA_INTERFACE */

	// -----------------------------------------------------------------------------------------------------------------

	return EXIT_SUCCESS;
}
