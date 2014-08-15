/*
 * shift_md.cpp
 *
 *  Created on: Aug 14, 2014
 *      Author: lafavemc
 */

#include <iostream>

using std::istream;

#include "shift_md.hh"

istream& shift_md ( istream& in, int& flag, int& number, char& letter)
{

	if(in){

		if( flag )
		{

			// Take the next element, which should be an interger.
			// Also set mdintnext flag to 0.

			if ( !(in >> number) )
			{
				throw 1;
			}

			flag = 0;

			// But if a 0 is the integer you got, ignore it and pull the
			// next character instead.

			if ( number == 0 && in.rdbuf()->in_avail() )
			{

				if ( !(in >> letter) )
				{
					throw 'e';
				}
				else
				{
					// Assume the thing AFTER this character will be an
					// integer. That's not the case if the character is ^,
					// but that's taken into account below.

					flag = 1;

				}

			}

		}
		else
		{

			// If mdintnext_flag hadn't been set, the next thing should be
			// a character.

			if ( !(in >> letter) )
			{
				throw 'e';
			}

			// After a character should be an integer (unless the character
			// was ^, but that's dealt with below)
			flag = 1;

		}


	}

	return in;

}



