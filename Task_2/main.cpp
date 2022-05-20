#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

class CVATRegister
{
public:
    /**
     * @brief Default constructor
     */
     CVATRegister   ( void );

     /**
      * @brief Default destructor
      */
    ~CVATRegister   ( void );

    /**
     * @brief Inserts a new company to register
     * @param name Company name
     * @param addr Company address
     * @param taxID Company ID
     * @return True if the company was inserted successfully, otherwise False ( Company with same ID / Name + Address already exists )
     */
    bool          newCompany     ( const string    & name,
                                   const string    & addr,
                                   const string    & taxID );

    /**
     * @brief Deletes a company with given name + address from register
     * @param name Company name
     * @param addr Company address
     * @return True if the deletion was successful, otherwise False ( Company with given name + address doesn't exist )
     */
    bool          cancelCompany  ( const string    & name,
                                   const string    & addr );

    /**
     * @brief Deletes a company with given ID from register
     * @param taxID Company ID
     * @return True if the deletion was successful, otherwise False ( Company with given ID doesn't exist )
     */
    bool          cancelCompany  ( const string    & taxID );

    /**
     * @brief Records an income of a company with given ID
     * @param taxID Company ID
     * @param amount Income amount
     * @return True if the record was successful, otherwise False ( Company with given ID doesn't exist )
     */

    bool          invoice        ( const string    & taxID,
                                   unsigned int      amount );

    /**
     * @brief Records an income of a company with given name + address
     * @param name Company Name
     * @param addr Company address
     * @param amount Income amount
     * @return True if the record was successful, otherwise False ( Company with given name + address doesn't exist )
     */

    bool          invoice        ( const string    & name,
                                   const string    & addr,
                                   unsigned int      amount );

    /**
     * @brief Counts a sum of all recorded incomes of a company with given name + address
     * @param name Company name
     * @param addr Company address
     * @param sumIncome Sum of incomes
     * @return True if the Company with given name + address exists, otherwise False
     */

    bool          audit          ( const string    & name,
                                   const string    & addr,
                                   unsigned int    & sumIncome ) const;

    /**
     * @brief Counts a sum of all recorded incomes of a company with given ID
     * @param taxID Company ID
     * @param sumIncome Sum of incomes
     * @return True if the Company with given ID exists, otherwise False
     */
    bool          audit          ( const string    & taxID,
                                   unsigned int    & sumIncome ) const;
    /**
     * @brief Finds the first company in register ( Sorted in alphabetical order by name + address )
     * @param name Name of the found company
     * @param addr Address of the founc company
     * @return True if the company was found, otherwise False ( Register is empty )
     */

    bool          firstCompany   ( string          & name,
                                   string          & addr ) const;
    /**
     * @brief Finds the next company in register ( In alphabetical order by name + address ), after the company with given name + address
     * @param name Name of the given company ( Will be overwritten, if the next company is found )
     * @param addr Address of the given company ( Will be overwritten, if the next company is found )
     * @return True if next company was found, otherwise False ( There is no company after the company given in params )
     */
    bool          nextCompany    ( string          & name,
                                   string          & addr ) const;

    /**
     * @brief Finds the median of all invoices of all companies ( Even deleted ones )
     * @return Median value, if the number of invoices is even, function returns the greater value from 2 values in the middle ( default is 0 )
     */
    unsigned int  medianInvoice  ( void ) const;


private:

    /**
     * @brief Struct representing an instance of company in register
     */

    struct Company {

        Company ( const string & name, const string & address, const string & id )
        {
            this->name = name;
            this->address = address;
            this->id = id;
        }

        string name ;
        string address ;
        string id ;
        unsigned int income = 0;
    };

    /**
     * @brief All companies sorted by their IDs
     */
    vector<Company> sortedById;

    /**
     * @brief All companies sorted by their names + addresses
     */
    vector<Company> sortedByName;

    /**
     * @brief All invoices of all companies in register history
     */
    vector<unsigned int> invoices;

    /**
     * @brief Converts a string to lowercase
     * @param text String to be converted
     */

    static void toLowerCase ( string & text );

    /**
     * @brief Compare function for companies. Compares names, eventually addresses, if the names are the same ( both case insensitive )
     * @param company1 First company
     * @param company2 Second company
     * @return True if the 1st company name < 2nd company name or == and 1st company address < 2nd company address, otherwise false
     */

    static bool compareFunction ( Company company1, Company company2 );

    /**
     * @brief Checks, whether the company with given id exists
     * @param id Company ID
     * @return true if the company exists, otherwise false
     */

    bool isIncluded ( const string & id ) const;

    /**
     * @brief @brief Checks, whether the company with given name + address exists
     * @param name Company name
     * @param address Company address
     * @return true if the company exists, otherwise false
     */

    bool isIncluded ( const string & name, const string & address ) const;

};

bool CVATRegister::newCompany( const string &name, const string &addr, const string &taxID )
{
    Company newCompanyToInsert ( name, addr, taxID );

    if ( isIncluded( taxID ) || isIncluded( name, addr ))
    {
        return false ;
    }

    // Find the positions, where should be the new company inserted

    auto positionByName = lower_bound( sortedByName.begin(), sortedByName.end(), newCompanyToInsert, compareFunction );

    auto positionById = lower_bound ( sortedById.begin(), sortedById.end(), newCompanyToInsert,  [] ( const Company & company1, const Company & company2) {
        return company1.id < company2.id;
    } );

    // Inserting the companies to register
    sortedById.insert  (positionById,   newCompanyToInsert );
    sortedByName.insert(positionByName, newCompanyToInsert );

    return true;
}

bool CVATRegister::cancelCompany( const string &name, const string &addr )
{
    if ( !isIncluded ( name, addr ) )
    {
        return false ;
    }

    Company tmpByName ( name, addr, "" );

    // Find position of company in vector sorted by names + addresses
    auto posByName = lower_bound(sortedByName.begin(), sortedByName.end(), tmpByName, compareFunction );

    // Find position of company in vector sorted by ID's. Using the previously found company
    auto posById = lower_bound( sortedById.begin(), sortedById.end(), *posByName, [] ( const Company & company1, const Company & company2) {
        return company1.id < company2.id;
    } );

    // Deleting the company
    sortedByName.erase( posByName );
    sortedById.  erase(posById );

    return true;
}

bool CVATRegister::cancelCompany( const string &taxID )
{
    if ( !isIncluded ( taxID ))
    {
        return false ;
    }

    Company tmpById ( "", "", taxID );

    // Find position of company in vector sorted by ID's.
    auto posById = lower_bound( sortedById.begin(), sortedById.end(), tmpById, [] ( const Company & company1, const Company & company2) {
        return company1.id < company2.id;
    } );

    // Find position of company in vector sorted by names + addresses. Using the previously found company.
    auto posByName = lower_bound(sortedByName.begin(), sortedByName.end(), *posById, compareFunction);


    // Deleting the company
    sortedById.erase(posById);
    sortedByName.erase(posByName);

    return true;

}

bool CVATRegister::invoice( const string &taxID, unsigned int amount )
{

    if ( !isIncluded ( taxID ))
    {
        return false ;
    }

    Company tmpById ( "", "", taxID );

    // Find position of company in vector sorted by ID's.
    auto posById = lower_bound( sortedById.begin(), sortedById.end(), tmpById, [] ( const Company & company1, const Company & company2) {
        return company1.id < company2.id;
    } );

    // Find position of company in vector sorted by names + addresses. Using the previously found company.
    auto posByName = lower_bound(sortedByName.begin(), sortedByName.end(), *posById, compareFunction );

    // Increase a total income of company
    posById->income += amount;
    posByName->income += amount;

    // Insert the invoice to the vector of all invoices
    invoices.push_back( amount );

    return true;
}

bool CVATRegister::invoice(const string &name, const string &addr, unsigned int amount) {

    if ( !isIncluded ( name, addr ) )
    {
        return false ;
    }

    Company tmpByName ( name, addr, "" );

    // Find position of company in vector sorted by names + addresses
    auto posByName = lower_bound(sortedByName.begin(), sortedByName.end(), tmpByName, compareFunction );

    // Find position of company in vector sorted by ID's. Using the previously found company
    auto posById = lower_bound( sortedById.begin(), sortedById.end(), *posByName, [] ( const Company & company1, const Company & company2) {
        return company1.id < company2.id;
    } );

    // Increase a total income of company
    posByName->income += amount;
    posById->income += amount;

    // Insert the invoice to the vector of all invoices
    invoices.push_back( amount );

    return true;
}

bool CVATRegister::audit(const string &name, const string &addr, unsigned int &sumIncome) const
{
    if (! isIncluded( name, addr ))
    {
        return false ;
    }

    Company tmpByName ( name, addr, "" );

    // Find position of company in vector sorted by names + addresses
    auto posByName = lower_bound(sortedByName.begin(), sortedByName.end(), tmpByName, compareFunction );

    sumIncome = posByName->income;
    return true;

}

bool CVATRegister::audit(const string &taxID, unsigned int &sumIncome) const
{
    if ( !isIncluded ( taxID ))
    {
        return false ;
    }

    Company tmpById ( "", "", taxID );

    // Find position of company in vector sorted by ID's
    auto posById = lower_bound( sortedById.begin(), sortedById.end(), tmpById, [] ( const Company & company1, const Company & company2) {
        return company1.id < company2.id;
    } );

    sumIncome = posById->income;
    return true;
}

bool CVATRegister::firstCompany(string &name, string &addr) const
{
    // Check if there are any companies
    if ( sortedByName.empty())
    {
        return false;
    }

    // Return first of them in vector sorted by names + addresses
    name = sortedByName[0].name;
    addr = sortedByName[0].address;
    return true;
}

bool CVATRegister::nextCompany(string &name, string &addr) const
{
    // Check if there are any companies
    if ( sortedByName.empty())
    {
        return false;
    }

    Company tmpCompany ( name, addr, "" );

    // Find the first company, after the company with given name and address
    auto pos = upper_bound(sortedByName.begin(), sortedByName.end(), tmpCompany, compareFunction );

    // No company found
    if ( pos == sortedByName.end() )
    {
        return false;
    }

    name = pos->name;
    addr = pos->address;
    return true;
}

unsigned int CVATRegister::medianInvoice(void) const
{
    // Default return value is 0
    if ( invoices.empty() )
    {
        return 0;
    }

    // Sort all of the invoices
    vector<unsigned int> tmpInvoices = invoices;
    sort (tmpInvoices.begin(), tmpInvoices.end());

    // If the number of invoices is odd, return the middle one
    if ( invoices.size() % 2 )
    {
        return tmpInvoices[floor(invoices.size()/2)];
    }

    // Else return the greater value from the 2 values in the middle ( Vector is sorted in ascending order -> the greater index )
    else
    {
        return tmpInvoices[(invoices.size()/2)];
    }
}

void CVATRegister::toLowerCase( string & text )
{
    for_each( text.begin(), text.end(), []( char & c )
    {
        c = ::tolower(c);
    } );
}

bool CVATRegister::compareFunction(CVATRegister::Company company1, CVATRegister::Company company2)
{
    // We want to compare case-insensitive
    toLowerCase( company1.name );
    toLowerCase( company2.name );
    toLowerCase( company1.address );
    toLowerCase( company2.address );

    return company1.name != company2.name ? company1.name < company2.name : company1.address < company2.address;
}

bool CVATRegister::isIncluded(const string &id) const
{
    Company toSearch ( "", "", id );

    return binary_search(sortedById.begin(), sortedById.end(), toSearch, [] ( Company company1, Company company2 ) {
        return ( company1.id < company2.id ) ;
    } );
}

bool CVATRegister::isIncluded(const string &name, const string &address) const
{
    Company toSearch ( name, address, "" );
    return binary_search(sortedByName.begin(), sortedByName.end(), toSearch, compareFunction );
}


CVATRegister::CVATRegister(void) = default;

CVATRegister::~CVATRegister(void) = default;





#ifndef __PROGTEST__
int               main           ( void )
{
    string name, addr;
    unsigned int sumIncome;

    CVATRegister b1;
    assert ( b1 . newCompany ( "ACME", "Thakurova", "666/666" ) );
    assert ( b1 . newCompany ( "ACME", "Kolejni", "666/666/666" ) );
    assert ( b1 . newCompany ( "Dummy", "Thakurova", "123456" ) );
    assert ( b1 . invoice ( "666/666", 2000 ) );
    assert ( b1 . medianInvoice () == 2000 );
    assert ( b1 . invoice ( "666/666/666", 3000 ) );
    assert ( b1 . medianInvoice () == 3000 );
    assert ( b1 . invoice ( "123456", 4000 ) );
    assert ( b1 . medianInvoice () == 3000 );
    assert ( b1 . invoice ( "aCmE", "Kolejni", 5000 ) );
    assert ( b1 . medianInvoice () == 4000 );
    assert ( b1 . audit ( "ACME", "Kolejni", sumIncome ) && sumIncome == 8000 );
    assert ( b1 . audit ( "123456", sumIncome ) && sumIncome == 4000 );
    assert ( b1 . firstCompany ( name, addr ) && name == "ACME" && addr == "Kolejni" );
    assert ( b1 . nextCompany ( name, addr ) && name == "ACME" && addr == "Thakurova" );
    assert ( b1 . nextCompany ( name, addr ) && name == "Dummy" && addr == "Thakurova" );
    assert ( ! b1 . nextCompany ( name, addr ) );
    assert ( b1 . cancelCompany ( "ACME", "KoLeJnI" ) );
    assert ( b1 . medianInvoice () == 4000 );
    assert ( b1 . cancelCompany ( "666/666" ) );
    assert ( b1 . medianInvoice () == 4000 );
    assert ( b1 . invoice ( "123456", 100 ) );
    assert ( b1 . medianInvoice () == 3000 );
    assert ( b1 . invoice ( "123456", 300 ) );
    assert ( b1 . medianInvoice () == 3000 );
    assert ( b1 . invoice ( "123456", 200 ) );
    assert ( b1 . medianInvoice () == 2000 );
    assert ( b1 . invoice ( "123456", 230 ) );
    assert ( b1 . medianInvoice () == 2000 );
    assert ( b1 . invoice ( "123456", 830 ) );
    assert ( b1 . medianInvoice () == 830 );
    assert ( b1 . invoice ( "123456", 1830 ) );
    assert ( b1 . medianInvoice () == 1830 );
    assert ( b1 . invoice ( "123456", 2830 ) );
    assert ( b1 . medianInvoice () == 1830 );
    assert ( b1 . invoice ( "123456", 2830 ) );
    assert ( b1 . medianInvoice () == 2000 );
    assert ( b1 . invoice ( "123456", 3200 ) );
    assert ( b1 . medianInvoice () == 2000 );
    assert ( b1 . firstCompany ( name, addr ) && name == "Dummy" && addr == "Thakurova" );
    assert ( ! b1 . nextCompany ( name, addr ) );
    assert ( b1 . cancelCompany ( "123456" ) );
    assert ( ! b1 . firstCompany ( name, addr ) );

    CVATRegister b2;
    assert ( b2 . newCompany ( "ACME", "Kolejni", "abcdef" ) );
    assert ( b2 . newCompany ( "Dummy", "Kolejni", "123456" ) );
    assert ( ! b2 . newCompany ( "AcMe", "kOlEjNi", "1234" ) );
    assert ( b2 . newCompany ( "Dummy", "Thakurova", "ABCDEF" ) );
    assert ( b2 . medianInvoice () == 0 );
    assert ( b2 . invoice ( "ABCDEF", 1000 ) );
    assert ( b2 . medianInvoice () == 1000 );
    assert ( b2 . invoice ( "abcdef", 2000 ) );
    assert ( b2 . medianInvoice () == 2000 );
    assert ( b2 . invoice ( "aCMe", "kOlEjNi", 3000 ) );
    assert ( b2 . medianInvoice () == 2000 );
    assert ( ! b2 . invoice ( "1234567", 100 ) );
    assert ( ! b2 . invoice ( "ACE", "Kolejni", 100 ) );
    assert ( ! b2 . invoice ( "ACME", "Thakurova", 100 ) );
    assert ( ! b2 . audit ( "1234567", sumIncome ) );
    assert ( ! b2 . audit ( "ACE", "Kolejni", sumIncome ) );
    assert ( ! b2 . audit ( "ACME", "Thakurova", sumIncome ) );
    assert ( ! b2 . cancelCompany ( "1234567" ) );
    assert ( ! b2 . cancelCompany ( "ACE", "Kolejni" ) );
    assert ( ! b2 . cancelCompany ( "ACME", "Thakurova" ) );
    assert ( b2 . cancelCompany ( "abcdef" ) );
    assert ( b2 . medianInvoice () == 2000 );
    assert ( ! b2 . cancelCompany ( "abcdef" ) );
    assert ( b2 . newCompany ( "ACME", "Kolejni", "abcdef" ) );
    assert ( b2 . cancelCompany ( "ACME", "Kolejni" ) );
    assert ( ! b2 . cancelCompany ( "ACME", "Kolejni" ) );

    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
