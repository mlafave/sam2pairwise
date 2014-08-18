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

		if ( !(in >> number) )
		{
			// If you're unable to take an integer, it must be a character.
			// Clear the error and take the character, and set the flag indicating
			// that the next element has the possibility of being an integer.

			in.clear();
			if (!(in >> letter))
			{
				throw 'e';
			}
			flag = 1;

		}
		else
		{
			//  If you're able to take an integer, you know the next element
			// cannot be an integer, so lower the flag.

			flag = 0;

			if ( number == 0 && in.rdbuf()->in_avail() )
			{

				// If the integer you took was 0, and if there are elements
				// remaining, then take the next element (which you know must be a
				// character).
				if ( !(in >> letter) )
				{
					throw 'e';
				}
				else
				{
					flag = 1;
				}

			}

		}

	}

	return in;

}



