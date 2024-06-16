#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <stdexcept>

// ---------------------------------------------------------------------------------------------------------------------

using namespace std;

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

//=================================================================================================
// a dummy exception class, keep this implementation
class InvalidDateException : public invalid_argument
{
  public:
    InvalidDateException ( )
      : invalid_argument ( "invalid date or format" )
    {
    }
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

//=================================================================================================
// date_format manipulator - a dummy implementation. Keep this code unless you implement your
// own working manipulator.
ios_base & ( * date_format ( const char * fmt ) ) ( ios_base & x )
{
  return [] ( ios_base & ios ) -> ios_base & { return ios; };
}

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Class representing a date with year, month, and day.
 */
class CDate
{
public:
	/**
	 * @brief Constructor for CDate.
	 *
	 * @param year The year of the date.
	 * @param month The month of the date.
	 * @param day The day of the date.
	 */
	CDate (int year, int month, int day)
	{
		bool valid = isValid(year, month, day);
		
		if (!valid)
			throw InvalidDateException();
		else
		{
			c_Year = year;
			c_Month = month;
			c_Day = day;
		}
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Output stream operator for printing the date.
	 *
	 * @param os The output stream.
	 * @param object The CDate object to print.
	 * @return A reference to the output stream.
	 */
	friend ostream &operator << (ostream &os, CDate &object)
	{
		os << object.c_Year;
		os << '-' << setfill('0') << setw(2) << object.c_Month;
		os << '-' << setfill('0') << setw(2) << object.c_Day;
		return os;
	}

	/**
	 * @brief Input stream operator for reading a date.
	 *
	 * @param is The input stream.
	 * @param object The CDate object to read into.
	 * @return A reference to the input stream.
	 */
	friend istream &operator >> (istream &is, CDate &object)
	{
		char dash_first, dash_second;
		int set_day, set_month, set_year;

		if ((is >> set_year >> dash_first >> set_month >> dash_second >> set_day) && dash_first == '-' && dash_second == '-')
		{
			if (isValid(set_year, set_month, set_day))
			{
				tm new_date = tm();

				new_date.tm_mday = set_day;
				new_date.tm_mon = set_month - 1;
				new_date.tm_year = set_year - 1900;

				mktime(&new_date);

				object.c_Day = new_date.tm_mday;
				object.c_Month = new_date.tm_mon + 1;
				object.c_Year = new_date.tm_year + 1900;

				return is;
			}

			else
			{
				is.setstate(ios::failbit);
				return is;
			}
		}

		else
		{
			is.setstate(ios::failbit);
			return is;
		}
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Addition operator for adding days to a date.
	 *
	 * @param days The number of days to add.
	 * @return The new CDate object with the added days.
	 */
	CDate operator + (int days)
	{
		tm new_date = tm();
		new_date.tm_mday = c_Day + days;
		new_date.tm_mon = c_Month - 1;
		new_date.tm_year = c_Year - 1900;
		mktime(&new_date);

		return CDate (new_date.tm_year + 1900, new_date.tm_mon + 1, new_date.tm_mday);
	}

	/**
	 * @brief Subtraction operator for subtracting days from a date.
	 *
	 * @param days The number of days to subtract.
	 * @return The new CDate object with the subtracted days.
	 */
	CDate operator - (int days)
	{
		return (days >= 0) ? operator + (-days) : operator + (days);
	}

	/**
	 * @brief Subtraction operator for calculating the difference in days between two dates.
	 *
	 * @param other The other CDate object to subtract.
	 * @return The difference in days between the two dates.
	 */
	int operator - (CDate &other)
	{
		tm end_time = tm();
		end_time.tm_mday = c_Day;
		end_time.tm_mon = c_Month - 1;
		end_time.tm_year = c_Year - 1900;

		tm beginning_time = tm();
		beginning_time.tm_mday = other.c_Day;
		beginning_time.tm_mon = other.c_Month - 1;
		beginning_time.tm_year = other.c_Year - 1900;

		return (int) difftime(mktime(&end_time), mktime(&beginning_time)) / seconds_in_day;
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Prefix increment operator for advancing the date by one day.
	 *
	 * @return The new CDate object with the incremented day.
	 */
	CDate operator ++ ()
	{
		return *this = operator + (1);
	}

	/**
	 * @brief Postfix increment operator for advancing the date by one day.
	 *
	 * @return The original CDate object before the increment.
	 */
	CDate operator ++ (int)
	{
		CDate date = *this;

		tm new_date = tm();
		new_date.tm_mday = c_Day + 1;
		new_date.tm_mon = c_Month - 1;
		new_date.tm_year = c_Year - 1900;
		mktime(&new_date);

		c_Day = new_date.tm_mday;
		c_Month = new_date.tm_mon + 1;
		c_Year = new_date.tm_year + 1900;
		
		return date;
	}

	/**
	 * @brief Prefix decrement operator for moving the date back by one day.
	 *
	 * @return The new CDate object with the decremented day.
	 */
	CDate operator -- ()
	{
		return *this = operator - (1);
	}

	/**
	 * @brief Postfix decrement operator for moving the date back by one day.
	 *
	 * @return The original CDate object before the decrement.
	 */
	CDate operator -- (int)
	{
		CDate date = *this;

		tm new_date = tm();
		new_date.tm_mday = c_Day - 1;
		new_date.tm_mon = c_Month - 1;
		new_date.tm_year = c_Year - 1900;
		mktime(&new_date);

		c_Day = new_date.tm_mday;
		c_Month = new_date.tm_mon + 1;
		c_Year = new_date.tm_year + 1900;
		
		return date;
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Equality operator for comparing two dates.
	 *
	 * @param other The other CDate object to compare with.
	 * @return True if the dates are equal, false otherwise.
	 */
	bool operator == (CDate &other)
	{
		return (c_Day == other.c_Day && c_Month == other.c_Month && c_Year == other.c_Year);
	}

	/**
	 * @brief Inequality operator for comparing two dates.
	 *
	 * @param other The other CDate object to compare with.
	 * @return True if the dates are not equal, false otherwise.
	 */
	bool operator != (CDate &other)
	{
		return !(operator == (other));
	}

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Less than or equal operator for comparing two dates.
	 *
	 * @param other The other CDate object to compare with.
	 * @return True if the date is less than or equal to the other date, false otherwise.
	 */
	bool operator <= (CDate &other)
	{
		return (c_Year < other.c_Year || (c_Year == other.c_Year && c_Month < other.c_Month) 
							|| (c_Year == other.c_Year && c_Month == other.c_Month && c_Day <= other.c_Day));
	}

	/**
	 * @brief Less than operator for comparing two dates.
	 *
	 * @param other The other CDate object to compare with.
	 * @return True if the date is less than the other date, false otherwise.
	 */
	bool operator < (CDate &other)
	{
		return (c_Year < other.c_Year || (c_Year == other.c_Year && c_Month < other.c_Month) 
							|| (c_Year == other.c_Year && c_Month == other.c_Month && c_Day < other.c_Day));
	}

	/**
	 * @brief Greater than or equal operator for comparing two dates.
	 *
	 * @param other The other CDate object to compare with.
	 * @return True if the date is greater than or equal to the other date, false otherwise.
	 */
	bool operator >= (CDate &other)
	{
		return (c_Year > other.c_Year || (c_Year == other.c_Year && c_Month > other.c_Month) 
							|| (c_Year == other.c_Year && c_Month == other.c_Month && c_Day >= other.c_Day));
	}

	/**
	 * @brief Greater than operator for comparing two dates.
	 *
	 * @param other The other CDate object to compare with.
	 * @return True if the date is greater than the other date, false otherwise.
	 */
	bool operator > (CDate &other)
	{
		return (c_Year > other.c_Year || (c_Year == other.c_Year && c_Month > other.c_Month) 
							|| (c_Year == other.c_Year && c_Month == other.c_Month && c_Day > other.c_Day));
	}

private:
	int c_Year, c_Month, c_Day;
	static const int seconds_in_day = 86400;

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Validate the given date.
	 *
	 * @param year The year to validate.
	 * @param month The month to validate.
	 * @param day The day to validate.
	 * @return True if the date is valid, false otherwise.
	 */
	static bool isValid (int year, int month, int day)
	{
		if (month == 2)
		{
			if (((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) && (day >= 1 && day <= 29))
				return true;
			else if (day >= 1 && day <= 28)
				return true;
			else
				return false;
		}
		else if ((month == 4 || month == 6 || month == 9 || month == 11) && (day >= 1 && day <= 30))
			return true;
		else if ((month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) && (day >= 1 && day <= 31))
			return true;
		else
			return false;
	}
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

int main ( void )
{
  ostringstream oss;
  istringstream iss;

  CDate a ( 2000, 1, 2 );
  CDate b ( 2010, 2, 3 );
  CDate c ( 2004, 2, 10 );
  oss . str ("");
  oss << a;
  assert ( oss . str () == "2000-01-02" );
  oss . str ("");
  oss << b;
  assert ( oss . str () == "2010-02-03" );
  oss . str ("");
  oss << c;
  assert ( oss . str () == "2004-02-10" );
  a = a + 1500;
  oss . str ("");
  oss << a;
  assert ( oss . str () == "2004-02-10" );
  b = b - 2000;
  oss . str ("");
  oss << b;
  assert ( oss . str () == "2004-08-13" );
  assert ( b - a == 185 );
  assert ( ( b == a ) == false );
  assert ( ( b != a ) == true );
  assert ( ( b <= a ) == false );
  assert ( ( b < a ) == false );
  assert ( ( b >= a ) == true );
  assert ( ( b > a ) == true );
  assert ( ( c == a ) == true );
  assert ( ( c != a ) == false );
  assert ( ( c <= a ) == true );
  assert ( ( c < a ) == false );
  assert ( ( c >= a ) == true );
  assert ( ( c > a ) == false );
  a = ++c;
  oss . str ( "" );
  oss << a << " " << c;
  assert ( oss . str () == "2004-02-11 2004-02-11" );
  a = --c;
  oss . str ( "" );
  oss << a << " " << c;
  assert ( oss . str () == "2004-02-10 2004-02-10" );
  a = c++;
  oss . str ( "" );
  oss << a << " " << c;
  assert ( oss . str () == "2004-02-10 2004-02-11" );
  a = c--;
  oss . str ( "" );
  oss << a << " " << c;
  assert ( oss . str () == "2004-02-11 2004-02-10" );
  iss . clear ();
  iss . str ( "2015-09-03" );
  assert ( ( iss >> a ) );
  oss . str ("");
  oss << a;
  assert ( oss . str () == "2015-09-03" );
  a = a + 70;
  oss . str ("");
  oss << a;
  assert ( oss . str () == "2015-11-12" );

  CDate d ( 2000, 1, 1 );
  try
  {
    CDate e ( 2000, 32, 1 );
    assert ( "No exception thrown!" == nullptr );
  }
  catch ( ... )
  {
  }
  iss . clear ();
  iss . str ( "2000-12-33" );
  assert ( ! ( iss >> d ) );
  oss . str ("");
  oss << d;
  assert ( oss . str () == "2000-01-01" );
  iss . clear ();
  iss . str ( "2000-11-31" );
  assert ( ! ( iss >> d ) );
  oss . str ("");
  oss << d;
  assert ( oss . str () == "2000-01-01" );
  iss . clear ();
  iss . str ( "2000-02-29" );
  assert ( ( iss >> d ) );
  oss . str ("");
  oss << d;
  assert ( oss . str () == "2000-02-29" );
  iss . clear ();
  iss . str ( "2001-02-29" );
  assert ( ! ( iss >> d ) );
  oss . str ("");
  oss << d;
  assert ( oss . str () == "2000-02-29" );

  //-----------------------------------------------------------------------------
  // bonus test examples
  //-----------------------------------------------------------------------------
  CDate f ( 2000, 5, 12 );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2000-05-12" );
  oss . str ("");
  oss << date_format ( "%Y/%m/%d" ) << f;
  assert ( oss . str () == "2000/05/12" );
  oss . str ("");
  oss << date_format ( "%d.%m.%Y" ) << f;
  assert ( oss . str () == "12.05.2000" );
  oss . str ("");
  oss << date_format ( "%m/%d/%Y" ) << f;
  assert ( oss . str () == "05/12/2000" );
  oss . str ("");
  oss << date_format ( "%Y%m%d" ) << f;
  assert ( oss . str () == "20000512" );
  oss . str ("");
  oss << date_format ( "hello kitty" ) << f;
  assert ( oss . str () == "hello kitty" );
  oss . str ("");
  oss << date_format ( "%d%d%d%d%d%d%m%m%m%Y%Y%Y%%%%%%%%%%" ) << f;
  assert ( oss . str () == "121212121212050505200020002000%%%%%" );
  oss . str ("");
  oss << date_format ( "%Y-%m-%d" ) << f;
  assert ( oss . str () == "2000-05-12" );
  iss . clear ();
  iss . str ( "2001-01-1" );
  assert ( ! ( iss >> f ) );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2000-05-12" );
  iss . clear ();
  iss . str ( "2001-1-01" );
  assert ( ! ( iss >> f ) );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2000-05-12" );
  iss . clear ();
  iss . str ( "2001-001-01" );
  assert ( ! ( iss >> f ) );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2000-05-12" );
  iss . clear ();
  iss . str ( "2001-01-02" );
  assert ( ( iss >> date_format ( "%Y-%m-%d" ) >> f ) );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2001-01-02" );
  iss . clear ();
  iss . str ( "05.06.2003" );
  assert ( ( iss >> date_format ( "%d.%m.%Y" ) >> f ) );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2003-06-05" );
  iss . clear ();
  iss . str ( "07/08/2004" );
  assert ( ( iss >> date_format ( "%m/%d/%Y" ) >> f ) );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2004-07-08" );
  iss . clear ();
  iss . str ( "2002*03*04" );
  assert ( ( iss >> date_format ( "%Y*%m*%d" ) >> f ) );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2002-03-04" );
  iss . clear ();
  iss . str ( "C++09format10PA22006rulez" );
  assert ( ( iss >> date_format ( "C++%mformat%dPA2%Yrulez" ) >> f ) );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2006-09-10" );
  iss . clear ();
  iss . str ( "%12%13%2010%" );
  assert ( ( iss >> date_format ( "%%%m%%%d%%%Y%%" ) >> f ) );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2010-12-13" );

  CDate g ( 2000, 6, 8 );
  iss . clear ();
  iss . str ( "2001-11-33" );
  assert ( ! ( iss >> date_format ( "%Y-%m-%d" ) >> g ) );
  oss . str ("");
  oss << g;
  assert ( oss . str () == "2000-06-08" );
  iss . clear ();
  iss . str ( "29.02.2003" );
  assert ( ! ( iss >> date_format ( "%d.%m.%Y" ) >> g ) );
  oss . str ("");
  oss << g;
  assert ( oss . str () == "2000-06-08" );
  iss . clear ();
  iss . str ( "14/02/2004" );
  assert ( ! ( iss >> date_format ( "%m/%d/%Y" ) >> g ) );
  oss . str ("");
  oss << g;
  assert ( oss . str () == "2000-06-08" );
  iss . clear ();
  iss . str ( "2002-03" );
  assert ( ! ( iss >> date_format ( "%Y-%m" ) >> g ) );
  oss . str ("");
  oss << g;
  assert ( oss . str () == "2000-06-08" );
  iss . clear ();
  iss . str ( "hello kitty" );
  assert ( ! ( iss >> date_format ( "hello kitty" ) >> g ) );
  oss . str ("");
  oss << g;
  assert ( oss . str () == "2000-06-08" );
  iss . clear ();
  iss . str ( "2005-07-12-07" );
  assert ( ! ( iss >> date_format ( "%Y-%m-%d-%m" ) >> g ) );
  oss . str ("");
  oss << g;
  assert ( oss . str () == "2000-06-08" );
  iss . clear ();
  iss . str ( "20000101" );
  assert ( ( iss >> date_format ( "%Y%m%d" ) >> g ) );
  oss . str ("");
  oss << g;
  assert ( oss . str () == "2000-01-01" );

  return EXIT_SUCCESS;
}
