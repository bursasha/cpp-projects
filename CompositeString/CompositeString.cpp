#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <memory>
#include <stdexcept>
#include <utility>
#endif /* __PROGTEST__ */

// ---------------------------------------------------------------------------------------------------------------------

class CSource {
public:
	explicit CSource(std::shared_ptr<char[]> data)
			: next_(nullptr), data_(std::move(data)) {}

	~CSource() = default;

	// -----------------------------------------------------------------------------------------------------------------

	[[nodiscard]] CSource* getNext() const {
		return next_;
	}

	[[nodiscard]] std::shared_ptr<char[]> getData() const {
		return data_;
	}

	[[nodiscard]] const char* getRawData() const {
		return data_.get();
	}

	// -----------------------------------------------------------------------------------------------------------------

	CSource& setNext(CSource* next) {
		next_ = next;

		return *this;
	}

private:
	CSource* next_;
	std::shared_ptr<char[]> data_;
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

class CPatch {
public:
	CPatch()
			: offset_(0), length_(0), end_index_(0), data_(nullptr) {}

	explicit CPatch(size_t offset, size_t length, size_t end_index, std::shared_ptr<char[]> data)
			: offset_(offset), length_(length), end_index_(end_index), data_(std::move(data)) {}

	~CPatch() = default;

	// -----------------------------------------------------------------------------------------------------------------

	[[nodiscard]] size_t getOffset() const {
		return offset_;
	}

	[[nodiscard]] size_t getLength() const {
		return length_;
	}

	[[nodiscard]] size_t getEndIndex() const {
		return end_index_;
	}

	[[nodiscard]] std::shared_ptr<char[]> getData() const {
		return data_;
	}

	[[nodiscard]] const char* getRawData() const {
		return data_.get();
	}

	// -----------------------------------------------------------------------------------------------------------------

	CPatch& setOffset(size_t offset) {
		offset_ = offset;

		return *this;
	}

	CPatch& setLength(size_t length) {
		length_ = length;

		return *this;
	}

	CPatch& setEndIndex(size_t end_index) {
		end_index_ = end_index;

		return *this;
	}

	// -----------------------------------------------------------------------------------------------------------------

	[[nodiscard]] char* toStr() const {
		char* str = new char[length_ + 1];
		std::memcpy(str, data_.get() + offset_, length_);
		str[length_] = '\0';

		return str;
	}

private:
	size_t offset_, length_, end_index_;
	std::shared_ptr<char[]> data_;
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

//class CPatchStr {
//public:
//	// ✅
//	CPatchStr()
//			: patches_(nullptr), patch_count_(0), patch_capacity_(0) {}
//
//	// ✅
//	CPatchStr(const char* str)
//			: patches_(nullptr), patch_count_(0), patch_capacity_(0) {
//		if (str && *str != '\0') {
//			auto new_data = createData_(str);
//			auto new_source = new CSource(new_data);
//			auto new_patch = CPatch(0, std::strlen(str), std::strlen(str), new_data);
//
//			appendSource_(new_source);
//			appendPatch_(&new_patch);
//		}
//	}
//
//	// ✅
//	CPatchStr(const CPatchStr& src)
//			: patches_(nullptr), patch_count_(0), patch_capacity_(0) {
//		if (this != &src)
//			copyPatches_(src.patches_, src.patch_count_, src.patch_capacity_);
//	}
//
//	// ✅
//	~CPatchStr() {
//		deletePatches_();
//		deleteSources_();
//	}
//
//	// -----------------------------------------------------------------------------------------------------------------
//
//	// ✅
//    CPatchStr& operator=(const CPatchStr& src) {
//	    if (this != &src)
//		    copyPatches_(src.patches_, src.patch_count_, src.patch_capacity_);
//
//	    return *this;
//	}
//
//	// -----------------------------------------------------------------------------------------------------------------
//
//	// ✅
//	CPatchStr subStr(size_t from, size_t len) const {
//		size_t to = from + len, str_length = (patch_count_ ? patches_[patch_count_ - 1].getEndIndex() : 0);
//
//		if (from >= str_length || to > str_length)
//			throw std::out_of_range("subStr: index out of range!");
//
//		CPatchStr substr;
//
//		if (from == to)
//			return substr;
//
//		int patch_start_index = findPatchIndex_(from), patch_end_index = findPatchIndex_(to - 1);
//
////		std::cout << "SUBSTR START" << std::endl;
////		for (size_t i = 0; i < patch_count_; ++i)
////			std::cout << "PATCH N" << i << ": Offset=" << patches_[i].getOffset() << ", Length=" << patches_[i].getLength() << ", TEXT=" << patches_[i].getRawData() << std::endl;
////		std::cout << "---------------------------" << std::endl;
//
//		for (size_t patch_index = patch_start_index, str_end_index = 0; patch_index <= static_cast<size_t>(patch_end_index); ++patch_index) {
//			const auto& src_patch = patches_[patch_index];
//			size_t new_patch_offset = src_patch.getOffset(), new_patch_length = src_patch.getLength();
//
//			if (patch_start_index == patch_end_index) {
//				new_patch_offset = src_patch.getOffset() + (from - (src_patch.getEndIndex() - src_patch.getLength()));
//				new_patch_length = len; // to - (src_patch.getEndIndex() - src_patch.getLength()) - new_patch_offset;
//			} else if (patch_index == static_cast<size_t>(patch_start_index)) {
//				new_patch_offset = src_patch.getOffset() + (from - (src_patch.getEndIndex() - src_patch.getLength()));
//				new_patch_length = src_patch.getEndIndex() - from;
//			} else if (patch_index == static_cast<size_t>(patch_end_index)) {
//				new_patch_offset = src_patch.getOffset();
//				new_patch_length = src_patch.getLength() - (src_patch.getEndIndex() - to);
//			}
//
//			auto new_patch = CPatch(new_patch_offset, new_patch_length,
//										str_end_index += new_patch_length, src_patch.getData());
//
//			substr.appendPatch_(&new_patch);
//		}
//
////		std::cout << "SUBSTR END" << std::endl;
////		for (size_t i = 0; i < substr.patch_count_; ++i)
////			std::cout << "PATCH N" << i << ": Offset=" << substr.patches_[i].getOffset() << ", Length=" << substr.patches_[i].getLength() << ", TEXT=" << substr.patches_[i].getRawData() << std::endl;
////		std::cout << "---------------------------" << std::endl;
//
//		return substr;
//	}
//
//	// ✅
//	char* toStr() const {
//		size_t str_length = (patch_count_ ? patches_[patch_count_ - 1].getEndIndex() : 0);
//		char* str = new char[str_length + 1];
//		str[str_length] = '\0';
//
////		std::cout << "--------------JKL-------------" << std::endl;
////		for (size_t i = 0; i < patch_count_; ++i)
////			std::cout << "PATCH N" << i << ": Offset=" << patches_[i].getOffset() << ", Length=" << patches_[i].getLength() << ", TEXT=" << patches_[i].getRawData() << std::endl;
//
//		for (size_t patch_index = 0, str_index = 0; patch_index < patch_count_;
//				str_index += patches_[patch_index].getLength(), ++patch_index) {
//			const auto& src_patch = patches_[patch_index];
//
//			std::memcpy(str + str_index, src_patch.getRawData() + src_patch.getOffset(),
//						src_patch.getLength());
//		}
//
//		return str;
//	}
//
//	// -----------------------------------------------------------------------------------------------------------------
//
//	// ✅
//    CPatchStr& append(const CPatchStr& src) {
//		size_t resized_patch_capacity = DEFAULT_CAPACITY_;
//
//		while (patch_count_ + src.patch_count_ > resized_patch_capacity)
//			resized_patch_capacity *= RESIZE_FACTOR_;
//
//		auto resized_patches = new CPatch[resized_patch_capacity];
//		size_t patch_index = 0;
//
//		for (; patch_index < patch_count_; ++patch_index)
//			resized_patches[patch_index] = CPatch(patches_[patch_index].getOffset(), patches_[patch_index].getLength(),
//			                                      patches_[patch_index].getEndIndex(), patches_[patch_index].getData());
//
//		for (size_t i = 0, str_end_index = (patch_index ? resized_patches[patch_index - 1].getEndIndex() : 0);
//				i < src.patch_count_; ++i, ++patch_index)
//			resized_patches[patch_index] = CPatch(src.patches_[i].getOffset(), src.patches_[i].getLength(),
//			                                      str_end_index += src.patches_[i].getLength(), src.patches_[i].getData());
//
//		deletePatches_();
//
//		patch_capacity_ = resized_patch_capacity;
//		patch_count_ = patch_index;
//		patches_ = resized_patches;
//
//	    return *this;
//	}
//
//    CPatchStr& insert(size_t pos, const CPatchStr& src) {
//	    size_t str_length = (patch_count_ ? patches_[patch_count_ - 1].getEndIndex() : 0);
//
//		if (pos > str_length)
//		    throw std::out_of_range("insert: index out of range!");
//		else if (pos == str_length) {
//		    append(src);
//			return *this;
//		}
//
////	    for (size_t i = 0; i < patch_count_; ++i)
////		    std::cout << "PATCH N" << i << ": Offset=" << patches_[i].getOffset() << ", Length=" << patches_[i].getLength() << ", TEXT=" << patches_[i].getRawData() << std::endl;
////	    std::cout << "---------------------------" << std::endl;
////	    std::cout << "Src" << std::endl;
////	    for (size_t i = 0; i < src.patch_count_; ++i)
////		    std::cout << "PATCH N" << i << ": Offset=" << src.patches_[i].getOffset() << ", Length=" << src.patches_[i].getLength() << ", TEXT=" << src.patches_[i].getRawData() << std::endl;
////	    std::cout << "---------------------------" << std::endl;
//
//	    int patch_insert_index = findPatchIndex_(pos);
//	    size_t new_patch_capacity = DEFAULT_CAPACITY_, new_patch_index = 0;
//
//	    while (patch_count_ + src.patch_count_ + 1 > new_patch_capacity)
//		    new_patch_capacity *= RESIZE_FACTOR_;
//
//	    auto new_patches = new CPatch[new_patch_capacity];
//
//	    for (size_t i = 0; i < static_cast<size_t>(patch_insert_index); ++i)
//		    new_patches[new_patch_index++] = patches_[i];
//
//	    if (pos != (patches_[patch_insert_index].getEndIndex() - patches_[patch_insert_index].getLength())) {
//		    const auto& divided_patch = patches_[patch_insert_index];
//		    size_t new_patch_length = pos - (divided_patch.getEndIndex() - divided_patch.getLength());
//
//		    if (new_patch_length > 0)
//			    new_patches[new_patch_index++] = CPatch(divided_patch.getOffset(), new_patch_length,
//														pos, divided_patch.getData());
//	    }
//
//	    size_t str_end_index = pos;
//	    for (size_t i = 0; i < src.patch_count_; ++i) {
//		    const CPatch& src_patch = src.patches_[i];
//
//		    new_patches[new_patch_index++] = CPatch(src_patch.getOffset(), src_patch.getLength(),
//													str_end_index += src_patch.getLength(), src_patch.getData());
//	    }
//
//		if (pos != (patches_[patch_insert_index].getEndIndex() - patches_[patch_insert_index].getLength())) {
//		    const auto& divided_patch = patches_[patch_insert_index];
//		    size_t new_patch_offset = divided_patch.getOffset() + (pos - (divided_patch.getEndIndex() - divided_patch.getLength()));
//		    size_t new_patch_length = divided_patch.getEndIndex() - pos;
//
//		    new_patches[new_patch_index++] = CPatch(new_patch_offset, new_patch_length,
//													str_end_index += new_patch_length, divided_patch.getData());
//	    }
//
//	    for (size_t i = ((patches_[patch_insert_index].getEndIndex() - patches_[patch_insert_index].getLength()) - pos == 0 ? patch_insert_index : patch_insert_index + 1);
//				i < patch_count_; ++i)
//		    new_patches[new_patch_index++] = CPatch(patches_[i].getOffset(), patches_[i].getLength(),
//		                                            str_end_index += patches_[i].getLength(), patches_[i].getData());
//
//	    deletePatches_();
//
//	    patches_ = new_patches;
//	    patch_count_ = new_patch_index;
//		patch_capacity_ = new_patch_capacity;
//
////	    std::cout << "READY!" << std::endl;
////	    for (size_t i = 0; i < patch_count_; ++i)
////		    std::cout << "PATCH N" << i << ": Offset=" << patches_[i].getOffset() << ", Length=" << patches_[i].getLength() << ", TEXT=" << patches_[i].getRawData() << std::endl;
//
//		return *this;
//	}
//
//    CPatchStr& remove(size_t from, size_t len) {
//	    size_t to = from + len, str_length = (patch_count_ ? patches_[patch_count_ - 1].getEndIndex() : 0);
//
//	    if (from >= str_length || to > str_length)
//		    throw std::out_of_range("remove: index out of range!");
//
//	    if (len == 0)
//		    return *this;
//
////	    std::cout << "From: " << from << " To: " << to << std::endl;
//
//	    int patch_start_index = findPatchIndex_(from), patch_end_index = findPatchIndex_(to - 1);
//
////	    std::cout << "START: " << patch_start_index << " END: " << patch_end_index << std::endl;
//
//	    size_t new_patch_capacity = patch_count_ - (patch_end_index - patch_start_index + 1) + 2, new_patch_index = 0;
//	    auto new_patches = new CPatch[new_patch_capacity];
//
//	    for (size_t i = 0; i < static_cast<size_t>(patch_start_index); ++i)
//		    new_patches[new_patch_index++] = patches_[i];
//
//	    if (from > (patches_[patch_start_index].getEndIndex() - patches_[patch_start_index].getLength())) {
//		    const CPatch& divided_patch = patches_[patch_start_index];
//		    size_t new_patch_length = from - (divided_patch.getEndIndex() - divided_patch.getLength());
//
//		    new_patches[new_patch_index++] = CPatch(divided_patch.getOffset(), new_patch_length, from, divided_patch.getData());
//	    }
//
//	    if ((to - 1) >= (patches_[patch_end_index].getEndIndex() - patches_[patch_end_index].getLength()) && (to - 1) < patches_[patch_end_index].getEndIndex()) {
//		    const CPatch& divided_patch = patches_[patch_end_index];
//		    size_t new_patch_offset = divided_patch.getOffset() + (to - (divided_patch.getEndIndex() - divided_patch.getLength())),
//			       new_patch_length = divided_patch.getEndIndex() - to;
//
//		    new_patches[new_patch_index++] = CPatch(new_patch_offset, new_patch_length,
//													from + new_patch_length, divided_patch.getData());
//	    }
//
//	    for (size_t i = patch_end_index + 1; i < patch_count_; ++i)
//		    new_patches[new_patch_index++] = CPatch(patches_[i].getOffset(), patches_[i].getLength(),
//													patches_[i].getEndIndex() - len, patches_[i].getData());
//
//	    delete[] patches_;
//	    patches_ = new_patches;
//	    patch_count_ = new_patch_index;
//		patch_capacity_ = new_patch_capacity;
//
//	    return *this;
//	}
//
//	size_t patch_count_, patch_capacity_;
//
//private:
//	static inline CSource* sources_ = nullptr;
//
//	// -----------------------------------------------------------------------------------------------------------------
//
//	CPatch* patches_;
////	size_t patch_count_, patch_capacity_;
//	static constexpr const size_t DEFAULT_CAPACITY_ = 100, RESIZE_FACTOR_ = 2;
//
//	// -----------------------------------------------------------------------------------------------------------------
//
//	static std::shared_ptr<char[]> createData_(const char* str) {
//		std::shared_ptr<char[]> data(new char[std::strlen(str) + 1], std::default_delete<char[]>());
//		std::strcpy(data.get(), str);
//
//		return data;
//	}
//
//	// -----------------------------------------------------------------------------------------------------------------
//
//	[[nodiscard]] int findPatchIndex_(size_t str_index) const { // бдить
//		size_t lower_index = 0, upper_index = patch_count_ - 1;
//		int target_index = -1;
//
////		std::cout << "FIND: " << "Lo=" << lower_index << " Hi=" << upper_index << " StrIDX=" << str_index << std::endl;
////	    for (size_t i = 0; i < patch_count_; ++i)
////		    std::cout << "PATCH N" << i << ": Offset=" << patches_[i].getOffset() << ", Length=" << patches_[i].getLength() << ", EI=" << patches_[i].getEndIndex() << ", TEXT=" << patches_[i].getRawData() << std::endl;
////		std::cout << std::endl;
//
//		while (lower_index <= upper_index) {
//			size_t mid_index = lower_index + (upper_index - lower_index) / 2;
//			const CPatch& patch = patches_[mid_index];
//
////			std::cout << "HM: " << "Lo=" << lower_index << " Hi=" << upper_index << " Mid=" << mid_index << std::endl;
//
//			if (str_index < (patch.getEndIndex() - patch.getLength()))
//				upper_index = mid_index - 1;
//			else if (str_index >= patch.getEndIndex())
//				lower_index = mid_index + 1;
//			else {
//				target_index = static_cast<int>(mid_index);
//				break;
//			}
//		}
//
//		return target_index;
//	}
//
//	// -----------------------------------------------------------------------------------------------------------------
//
//	// ✅
//	void copyPatches_(const CPatch* src_patches, size_t src_patch_count, size_t src_patch_capacity) {
//		deletePatches_();
//
//		patch_capacity_ = src_patch_capacity;
//		patches_ = new CPatch[patch_capacity_];
//
//		appendPatches_(src_patches, src_patch_count);
//	}
//
//	// -----------------------------------------------------------------------------------------------------------------
//
//	// ✅
//	static void appendSource_(CSource* src_source) {
//		if (!sources_)
//			sources_ = src_source;
//		else
//			for (auto current_source = sources_; current_source; current_source = current_source->getNext()) {
//				if (strcmp(current_source->getRawData(), src_source->getRawData()) == 0)
//					break;
//
//				if (!current_source->getNext()) {
//					current_source->setNext(src_source);
//					break;
//				}
//			}
//	}
//
//	// ✅
//	void appendPatch_(CPatch* src_patch) {
//		resizePatches_(patch_count_ + 1);
//
//		patches_[patch_count_++] = *src_patch;
//	}
//
//	// ✅
//	void appendPatches_(const CPatch* src_patches, size_t src_patch_count) {
//		for (size_t patch_index = 0, str_end_index = (patch_count_ ? patches_[patch_count_ - 1].getEndIndex() : 0);
//				patch_index < src_patch_count; ++patch_index) {
//			const auto& src_patch = src_patches[patch_index];
//			auto new_patch = CPatch(src_patch.getOffset(), src_patch.getLength(),
//										str_end_index += src_patch.getLength(), src_patch.getData());
//
//			appendPatch_(&new_patch);
//		}
//	}
//
//	// -----------------------------------------------------------------------------------------------------------------
//
//	// ✅
//	void resizePatches_(size_t new_patch_count) {
//		if (!patches_) {
//			patch_capacity_ = DEFAULT_CAPACITY_;
//			patches_ = new CPatch[patch_capacity_];
//		} else {
//			size_t resized_patch_capacity = patch_capacity_;
//
//			while (patch_count_ + new_patch_count > resized_patch_capacity)
//				resized_patch_capacity *= RESIZE_FACTOR_;
//
//			if (resized_patch_capacity > patch_capacity_) {
//				auto resized_patches = new CPatch[resized_patch_capacity];
//
//				size_t patch_index = 0;
//
//				for (; patch_index < patch_count_; ++patch_index)
//					resized_patches[patch_index] = CPatch(patches_[patch_index].getOffset(), patches_[patch_index].getLength(),
//					                                      patches_[patch_index].getEndIndex(), patches_[patch_index].getData());
//
//				deletePatches_();
//
//				patch_capacity_ = resized_patch_capacity;
//				patch_count_ = patch_index;
//				patches_ = resized_patches;
//			}
//		}
//	}
//
//	// -----------------------------------------------------------------------------------------------------------------
//
//	// ✅
//	static void deleteSources_() {
//		for (auto current_source = sources_; current_source; ) {
//			auto next_source = current_source->getNext();
//			delete current_source;
//			current_source = next_source;
//		}
//
//		sources_ = nullptr;
//	}
//
//	// ✅
//	void deletePatches_() {
//		patch_count_ = patch_capacity_ = 0;
//		delete[] patches_;
//		patches_ = nullptr;
//	}
//};

// ---------------------------------------------------------------------------------------------------------------------

#ifndef __PROGTEST__

bool stringMatch(char* str, const char* expected) {
  bool res = std::strcmp(str, expected) == 0;
  delete[] str;

  return res;
}

int main() {
//  char tmpStr[100];
//
//  CPatchStr a ( "test" );
//  assert ( stringMatch ( a . toStr (), "test" ) );
//
//  std::strncpy ( tmpStr, " da", sizeof ( tmpStr ) - 1 );
//  a . append ( tmpStr );
//  assert ( stringMatch ( a . toStr (), "test da" ) );
//
//  std::strncpy ( tmpStr, "ta", sizeof ( tmpStr ) - 1 );
//  a . append ( tmpStr );
//  assert ( stringMatch ( a . toStr (), "test data" ) );
//
//  std::strncpy ( tmpStr, "foo text", sizeof ( tmpStr ) - 1 );
//  CPatchStr b ( tmpStr );
//  assert ( stringMatch ( b . toStr (), "foo text" ) );
//
//  CPatchStr c ( a );
//  assert ( stringMatch ( c . toStr (), "test data" ) );
//
//  CPatchStr d ( a . subStr ( 3, 5 ) );
//  assert ( stringMatch ( d . toStr (), "t dat" ) );
//
//  d . append ( b );
//  assert ( stringMatch ( d . toStr (), "t datfoo text" ) );
//
//  d . append ( b . subStr ( 3, 4 ) );
//  assert ( stringMatch ( d . toStr (), "t datfoo text tex" ) );
//
//  c . append ( d );
//  assert ( stringMatch ( c . toStr (), "test datat datfoo text tex" ) );
//
//  c . append ( c );
//  assert ( stringMatch ( c . toStr (), "test datat datfoo text textest datat datfoo text tex" ) );
//
//  d . insert ( 2, c . subStr ( 6, 9 ) );
//  assert ( stringMatch ( d . toStr (), "t atat datfdatfoo text tex" ) );
//
//  b = "abcdefgh";
//  assert ( stringMatch ( b . toStr (), "abcdefgh" ) );
//  assert ( stringMatch ( d . toStr (), "t atat datfdatfoo text tex" ) );
//  assert ( stringMatch ( d . subStr ( 4, 8 ) . toStr (), "at datfd" ) );
//  assert ( stringMatch ( b . subStr ( 2, 6 ) . toStr (), "cdefgh" ) );
//
//  try {
//    b . subStr ( 2, 7 ) . toStr ();
//    assert ( "Exception not thrown" == nullptr );
//  } catch ( const std::out_of_range & e ) {
//  } catch ( ... ) {
//    assert ( "Invalid exception thrown" == nullptr );
//  }
//
//  a . remove ( 3, 5 );
//  assert ( stringMatch ( a . toStr (), "tesa" ) );

	return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
