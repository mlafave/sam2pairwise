/*
 * shift_cigar.cpp
 *
 *  Created on: Aug 14, 2014
 *      Author: lafavemc
 */

#include <iostream>

using namespace std;

using std::istream;

#include "shift_cigar.hh"

istream& shift_cigar( istream& in, int& number, char& letter )
{
	if (in)
	{

		if ( !(in >> number >> letter) )
		{
			throw 1;
		}

	}

	return in;

}



