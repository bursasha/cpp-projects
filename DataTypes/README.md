# The task is to develop classes that implement data types in a C/C++ compiler.

In a C/C++ compiler, it's essential to manage various data types used in the program. This homework involves implementing classes to describe and perform basic operations with four data types: `int`, `double`, enumeration, and structure.


The program consists of the following classes:

## CDataTypeInt
The class implements the built-in type `int`. The interface includes:
- `default constructor`: initializes the instance,
- `getSize()`: returns the size of the type (4 for `int`),
- `operator ==`: compares two types and returns true if they are identical,
- `operator !=`: compares two types and returns true if they are not identical,
- `operator <<`: displays the type declaration in the given stream.

## CDataTypeDouble
The class implements the built-in type `double`. The interface includes:
- `default constructor`: initializes the instance,
- `getSize()`: returns the size of the type (8 for `double`),
- `operator ==`: compares two types and returns true if they are identical,
- `operator !=`: compares two types and returns true if they are not identical,
- `operator <<`: displays the type declaration in the given stream.

## CDataTypeEnum
The class represents an enumeration. The interface includes:
- `default constructor`: initializes the instance,
- `getSize()`: returns the size of the type (4 for enums),
- `add()`: adds a new enumeration value to the list. An exception is thrown if the same value is already included in the list (see sample runs),
- `operator ==`: compares two types and returns true if they are identical (both are enumerations, moreover, the lists of values are identical, the values are listed in the same order),
- `operator !=`: compares two types and returns true if they are not identical,
- `operator <<`: displays the type declaration in the given stream. Caution, enum values must be printed in the order they were added.

## CDataTypeStruct
The class represents a structure. The interface includes:
- `default constructor`: initializes the instance,
- `getSize()`: returns the size of the type (based on the size of the fields),
- `addField(name, type)`: adds a new field to the end of the field list. A field is described by its name and type (int/double/enum). If the name collides with another existing name in the field list, the method shall throw an exception (see sample runs),
- `field(name)`: the method is used to access the type of the given field. If the name does not refer to an existing field, the method throws an exception (see sample runs). The field is accessed in read-only mode,
- `operator ==`: compares two types and returns true if they are identical (both are structures, both have the same number of fields, the datatypes of corresponding fields are the same, HOWEVER, field names do not have to match),
- `operator !=`: compares two types and returns true if they are not identical,
- `operator <<`: displays the type declaration in the given stream. The fields are listed in the order they were added via `addField`.

Submit a source file with the implementation of `CDataTypeInt`, `CDataTypeDouble`, `CDataTypeEnum`, and `CDataTypeStruct`. The classes must implement the above interface, moreover, you may add your own method to simplify the code.

### Notes:
- Both STL and C++ string is available. However, the problem needs a good analysis before the coding. Use polymorphism when designing the classes. Indeed, the classes will not be accepted if they are not polymorphic.
- Type declarations produced by `operator <<` are not tested for an exact match. Instead, whitespace is ignored in the comparison. The attached example uses function whitespaceMatch for the comparison, you are expected to implement the function.
- Method `getSize` should return the size using `size_t` data type.
- Code sample is available in the attached archive.
- A solution of this (simplified) problem be used for code review. However, a correct solution of the extended problem (Data types II) is eligible for code review.
- Update 2022-04-09: several methods accept strings as parameters. Implement your interface such that it accepts both C-strings (const char *) and C++ strings (std::string).
