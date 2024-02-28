# The task is to implement class template, the template will implement a generic index.

The index is implemented as generic class `CIndex`. The instance is initialized with a parameter - a sequence of elements. The sequence is to be indexed. The sequence may be of the form:
- `string` - a C or C++ string consisting of characters, 
- `vector <T>` - a vector of values (elements of type T), 
- `list <T>` - a list if values (elements of type T).

Once the instance is initialized and the input sequence is indexed, the instance may be used for searching. The search is given some sequence of elements (the data type matches the stored sequence). The result is a set of indices where the searched sequence was found in the indexed sequence.

To develop a flexible class, the matching of elements may be parameterized. The class will accept two generic parameters - the type of the indexed sequence and a comparator (second optional generic parameter). The searching does not have to compare the elements for an exact match. If a custom comparator is provided, the matching shall follow that comparator. For instance, the customized comparator may be used to implement case insensitive searching. The comparator will follow the guidelines used in STL: it is of the form of a function, functor, or a lambda function. The comparator accepts two parameters of type `T` (elements to compare), the result is true if the first element is smaller than the second element in the desired ordering. If no custom comparator is provided, `operator <` will be used.

The elements in the sequences are virtually unlimited. Examples are characters (`char`), integers (`int`), or strings (C++ `string`). In general, the element type `T` guarantees:
- copying (`operator =` and `copy constructor`),
- comparison by `operator <` ("less than"), or by the custom comparator,
- freeing (`destructor`),
- there may be further operations available in type `T`, however, your implementation cannot rely on them. Caution: `default constructor`, `operator ==`, `operator !=`, ... are not guaranteed.

Submit a source file with your `CIndex` template implementation. Moreover, add any further class (or classes) your implementation requires. The class must follow the public interface below. If there is a mismatch in the interface, the compilation will fail. You may extend the interface and add you auxiliary methods and member variables (both `public` and `private`, although `private` are preferred). The submitted file must include both declarations as well as implementation of the class (the methods may be implemented inline but do not have to). The submitted file shall not contain anything unnecessary. In particular, if the file contains main, your tests, or any `#include` definitions, please, keep them in a conditional compile block. Use the template below as a basis for your implementation. If the preprocessor definitions are preserved, the file maybe submitted to Progtest.

Your implementation must use STL, the compiler supports almost all C++17 extensions.

There are mandatory and bonus tests in the homework. An implementation using naive searching algorithm passes mandatory tests, however, it does not pass speed tests. To pass the bonus tests, an improved algorithm must be used. You may assume:
- Once indexed, the sequence is searched many times. Therefore, it make sense to spend some time preprocessing the sequence, the searching may be faster. The searching is called approx 100 times per instance.
- The fully generic implementation (generic type, generic comparator) is required in the mandatory tests. The speed tests use string type and default comparator. If a specialized template with these generic parameters is provided, the speed may be several orders of magnitude higher.
- KMP algorithm may be used to speed up string searching, suffix array or DAWG may be used to improve the indexing. Examples and basic tests are included in the attached file.

The solution of this homework cannot be used for code review.
