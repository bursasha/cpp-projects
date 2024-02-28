# The task is to develop class `CDate` to represent a date. 

The dates will follow Gregorian calendar rules. It is required that at least years 2000 to 2030 are handled by the class.

The class shall have the following interface:
- `constructor with parameters (y,m,d)` initializes an instance of the class with the date given. The constructor shall test the date. If the date is invalid, InvalidDateException shall be thrown,
- `operator +` can be used to add an integer to an instance of `CDate`. The operation returns a date which is shifted to the future by the given number of days (if the integer is negative, it shifts to the past),
- `operator -` can be used to subtract an integer from `CDate`. The result is a date shifted by the given number of days to the past (to the future if the integer is negative),
- `operator -` can be used to subtract two instances of `CDate`, the result is an integer equal to the number of days between the two dates,
- `operator ++ and --` in both prefix and postfix notation can be used to increase/decrease the date by one day. The operators shall follow the usual behavior,
- `operators ==, !=, <, <=, >, and >=` can be used to compare two instances of CDate. Dates in the future are considered greater than dates in the past.
- `operator <<` can be used to display the date in an output stream. The conversion shall use the ISO date format (`%Y-%m-%d`, e.g. 2000-01-31). The mandatory tests will use the ISO format only. The format may be modified by `date_format` manipulator, the manipulator must be implemented to pass the bonus test (it is not used in the mandatory tests), 
- `operator >>` can be used to read date from an input stream. The mandatory tests assume default ISO date format. If the reading fails (invalid format, invalid date, ...), the operator sets fail bit in the corresponding stream and leaves the original contents of the CDate instance unmodified. Similarly to the output operator, bonus tests require an implementation of `date_format` manipulator to handle arbitrary input format.

Submit a source file with your `CDate` implementation. The class must follow the interface described above. If there is a mismatch in the interface, the compilation will fail. You may extend the interface and add you auxiliary methods and member variables (both `public` and `private`, although `private` are preferred). The submitted file must include both declarations as well as implementation of the classes (the methods may be implemented `inline` but do not have to). The submitted file shall not contain main, your tests, or any `#include` definitions. If present, please, keep them in a conditional compile block. Use the template as a basis for your implementation. If the preprocessor definitions are preserved, the file may be submitted to Progtest.

This homework does not aim at speed. Moreover, the range of required years is very limited. Thus standard library date functions may be used to convert/compare dates (however, the standard functions must be used with certain caution).

There are mandatory and bonus tests in the homework. The mandatory tests do not use the `date_format` manipulator, the default ISO format `%Y-%m-%d` is used for all conversions. The bonus test requires an implementation of the manipulator such that the date format may be set arbitrarily. If you decide not to implement the bonus, please, keep the dummy implementation provided in the sample. If the manipulator is not present at all, the program will not compile.

`date_format` manipulator uses the following formats:
- characters except percent character: if present in the input format, it means that the input shall contain that character. The character is to be skipped in the input stream. If present in the output format, it means that the character is to be copied to the output. An example is the dash character in the ISO format `%Y-%m-%d`,
- percent character is used to start day (`%d`), month (`%m`), or year (`%Y`) conversion. If present in the input format then the corresponding number (day, month, or year) is to be parsed from the input stream. If present in the output format, the corresponding day/month/year value shall be appended to the output stream (left padded by zero, 2 digits day+month/4 digits year),
- percent character followed by a character other than Y, m, and d (e.g. `%x`, `%%`, ...) represents the second character (i.e. x and % in the example). The "escaped" character is processed as in the first case.

The `date_format` manipulator is just like other manipulators (hex, oct, ... ) in that it sets the conversion of all subsequent `CDate` processing in the stream until the format is modified by another `date_format` use. The manipulator does apply to the stream where it was used, it must not modify the behavior of other streams.

There are not any restrictions on output format (an extreme case is the "hello kitty" format below). The input format is restricted to guarantee unambiguous processing: each of `%Y`, `%m` and `%d` must be used exactly once in the format. The example shows invalid input formats (some conversion is missing/duplicate), these input conversions lead to an error in the subsequent reading.
### Advice:
- A source with runs are available in the attached archive. The source contains mandatory tests as well as examples of the bonus test (with the manipulator).
- It may be surprising for you, however, some days are not 24 hours. This fact may be important for certain implementations.
- If implementing the bonus part, read C++ manual, methods `xalloc`, `register_callback`, `pword`, and `iword` in `ios_base` class.
- Your date reading operator should check the width of months and dates. Both values should be 2 digits wide, padded with an extra zero if needed. This check may be omitted in the mandatory tests (the values generated for these tests are correctly padded). On the other hand, the check must be thorough to pass the bonus test (if the check is not present, the reading with arbitrary date format may be ambiguous). There are some extra test cases included in the attached bonus tests.
- Based on your implementation, you may need to explicitly handle leap years. The basic rule is: a year divisible by 4 is a leap year (e.g., 2020, 2024, ...). There is an exception to this rule: if a year is divisible by 100, then it is a leap year (e.g., years 1900 and 2100 were not/will not be leap years). Moreover, there is an exception of the exception: if a year is divisible by 400, then it is a leap year (e.g., year 2000 was a leap year). The homework is, however, simplified since the years are limited to the range 2000 - 2030.
- The solution of this homework is quite simple and straightforward. Therefore, solutions of this homework be used for code review. On the other hand, a solution that correctly passes all tests (i.e., including bonus tests) is not that trivial and therefore may be used for code review.
