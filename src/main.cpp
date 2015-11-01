#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <string>
#include <exception>

#include "spirit/include/boost/spirit.hpp"

using namespace std;

class cFileException : public exception {
  public:
    cFileException( string const &_fileName, string const & _reason ) {
        mFileName = _fileName;
    }

    virtual const char *what() const throw() {
        return "file open exception";
    };

  protected:
    string mFileName, mReason;
};

string readFile( string const &_fname ) {

    string ret;
    fstream fs;

    fs.open( _fname, ifstream::in | ifstream::binary );

    if ( fs ) {
        fs.seekg( 0, fs.end );
        size_t length = fs.tellg();
        fs.seekg( 0, fs.beg );
        ret.resize( length );
        fs.read( &ret[ 0 ], length );
    } else {
        throw(cFileException(_fname, "opening"));
    }

    return ret;
}

int main( int argc, char *argv[] ) {

    auto text = readFile( "glisp.clj" );

    return 0;
}
