#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

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
//=================================================================================================
// date_format manipulator - a dummy implementation. Keep this code unless you implement your
// own working manipulator.
ios_base & ( * date_format ( const char * fmt ) ) ( ios_base & x )
{
    return [] ( ios_base & ios ) -> ios_base & { return ios; };
}
//=================================================================================================
class CDate
{
public:

    /**
     * @brief Constructor
     * @param year Year
     * @param month Month
     * @param day Day
     */
    CDate ( int year, int month, int day );

    /**
     * @brief Overloaded + operator
     * @param rhs Number of days to be added to current date
     * @return Updated date
     */
    CDate operator + ( int rhs ) const;

    /**
     * @brief Overloaded - operator
     * @param rhs Number of days to be deducted
     * @return Updated date
     */
    CDate operator - ( int rhs ) const;

    /**
     * @brief Overloaded - operator
     * @param other Second date
     * @return Number of days between the two dates
     */
    int operator - ( CDate other ) const;

    /**
     * @brief Overloaded prefix ++ operator
     * @return Reference to updated date
     */
    CDate& operator++();

    /**
     * @brief Overloaded postfix ++ operator
     * @return Copy of original date before update
     */
    CDate operator++ ( int );

    /**
      * @brief Overloaded prefix -- operator
      * @return Reference to updated date
      */
    CDate& operator--();

    /**
     * @brief Overloaded postfix -- operator
     * @return Copy of original date before update
     */
    CDate operator--(int);


    /**
     * @brief Overloaded == operator
     * @param other Second date
     * @return True if both dates are same
     */
    bool operator == ( CDate other ) const;

    /**
     * @brief Overloaded != operator
     * @param other Second date
     * @return True if both dates are different
     */
    bool operator != ( CDate other ) const;

    /**
     * @brief Overloaded > operator
     * @param other Second date
     * @return True if the first date is later than second
     */
    bool operator > ( CDate other ) const;

    /**
     * @brief Overloaded < operator
     * @param other Second date
     * @return True if the first date is earlier than second
     */
    bool operator < ( CDate other ) const;

    /**
     * @brief Overloaded <= operator
     * @param other Second date
     * @return True if the first date is earlier or equal to second
     */
    bool operator <= ( CDate other ) const;

    /**
     * @brief Overloaded >= operator
     * @param other Second date
     * @return True if the first date is later or equal to second
     */
    bool operator >= ( CDate other ) const;

    /**
     * @brief Overloaded left bitwise operator
     * @param os Output stream
     * @param self Date
     * @return Reference to output stream with the printed date
     */
    friend ostream & operator << ( ostream & os, CDate self );

    /**
     * @brief Overloaded right bitwise operator
     * @param iss Input stream
     * @param self Date to be loaded to
     * @return Reference to the input stream
     */

    friend istream & operator >> ( istream & iss, CDate & self );

private:

    int year,
        month,
        day;

    /**
     * @brief Checks whether the year is leap
     * @param year Year to be checked
     * @return True if the year is leap
     */
    bool isLeapYear ( int year ) const;

    /**
     * @brief Checks how many days are in the chosen month
     * @param month Month of the date
     * @param year Year of the date
     * @return Number of days in the chosen month
     */
    int daysInMonth ( int month, int year ) const;

    /**
     * @brief Checks whether the date is valid
     * @param date Date to be checked
     * @return True if the date is valid
     */
    bool validDate ( CDate date ) const;

    /**
     * @brief Calculates number of days since start of year 2000
     * @param date Date to be used
     * @return Number of days since start of 2000
     */
    int daysFromStart ( CDate date ) const;

};



ostream & operator << ( ostream & os, CDate self )
{
    os << self.year;
    os << "-";
    ( self.month <= 9 )? os << "0" << self.month : os << self.month;
    os << "-";
    ( self.day   <= 9 )? os << "0" << self.day : os << self.day;
    return os;
}

istream &operator >> ( istream & iss, CDate & self )
{
    char delim1, delim2;
    CDate tmp = self;

    if ( ( ! ( iss >> tmp.year >> delim1 >> tmp.month >> delim2 >> tmp.day )) || delim1 != '-' || delim2 != '-' || ! self.validDate( tmp ))
    {
        iss.setstate(ios::failbit);
        return iss;
    }

    self = tmp;
    return iss;
}

CDate::CDate( int year, int month, int day )
{
    if ( ! ( month >= 1 && month   <= 12                           &&
             day   >= 1 && day     <= daysInMonth( month, year )   &&
             year  >= 2000 && year <= 2030 ) )
    {
        throw InvalidDateException();
    }

    this->year = year;
    this->month = month;
    this->day = day;
}

CDate CDate::operator + ( int rhs ) const
{
    CDate newDate ( this->year, this->month, this->day );
    newDate.day += rhs;

    while ( !( validDate( newDate )) )
    {
        if ( newDate.day < 1 )
        {
            newDate.month -= 1;
            if ( newDate.month == 0 )
            {
                newDate.month = 12;
                newDate.year -= 1;
            }
            newDate.day += daysInMonth( newDate.month, newDate.year );
        }

        if ( newDate.day > daysInMonth( newDate.month, newDate.year ))
        {
            newDate.day -= daysInMonth( newDate.month, newDate.year );
            newDate.month += 1;
            if ( newDate.month == 13 )
            {
                newDate.month = 1;
                newDate.year += 1;
            }
        }
    }

    return newDate;
}

CDate CDate::operator-(int rhs) const
{
    return ((*this) + ( - rhs ));
}

int CDate::operator-(CDate other) const
{
    int order1 = daysFromStart( *this );
    int order2 = daysFromStart( other );
    return abs( order1 - order2 );
}

CDate &CDate::operator ++ ()
{
    *this = ( *this + 1 );
    return *this;
}

CDate CDate::operator++(int)
{
    CDate old = *this;
    *this = ( *this + 1 );
    return old;
}

CDate &CDate::operator--()
{
    *this = ( *this - 1 );
    return *this;
}

CDate CDate::operator--(int)
{
    CDate old = *this;
    *this = ( *this - 1 );
    return old;
}

bool CDate::operator==(CDate other) const
{
    return ( year == other.year && month == other.month && day == other.day );
}

bool CDate::operator!=(CDate other) const
{
    return !( *this == other );
}

bool CDate::operator>(CDate other) const
{
    return ( year  > other.year                           ||
           ( year == other.year && month > other.month )  ||
           ( year == other.year && month == other.month && day > other.day )
    );
}

bool CDate::operator<(CDate other) const
{
    return ( !( *this > other ) && !( *this == other ) );
}

bool CDate::operator<=(CDate other) const
{
    return ( !( *this > other ) );
}

bool CDate::operator>=(CDate other) const
{
    return ( !( *this < other ) );
}

bool CDate::isLeapYear(int year) const
{
    if ( year % 4   != 0 ) { return false; }
    if ( year % 100 != 0 ) { return true;  }
    if ( year % 400 != 0 ) { return false; }
    return true;
}

int CDate::daysInMonth(int month, int year) const
{
    switch ( month )
    {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12: return 31;
        case 4:
        case 6:
        case 9:
        case 11: return 30;
        case 2: if (isLeapYear( year )) { return 29; } else { return 28; }
        default: return 0;
    }
}

bool CDate::validDate(CDate date) const
{
    return ( date.month >= 1 && date.month <= 12 && date.day >= 1 && date.day <= daysInMonth( date.month, date.year ) );
}

int CDate::daysFromStart(CDate date) const
{
    int result = 0;
    for ( int i = 2000; i < date.year ; ++i ) {
        if ( isLeapYear( i ) ) { result += 366 ;}
        else { result += 365 ;}
    }

    for ( int i = 1; i < date.month ; ++i ) {
        result += daysInMonth( i, date.year );
    }

    result += date.day;
    return result;
}


#ifndef __PROGTEST__
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
    return EXIT_SUCCESS;

}

#endif /* __PROGTEST__ */
