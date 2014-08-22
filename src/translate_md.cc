/*
 * translate_md.cpp
 *
 *  Created on: Aug 14, 2014
 *      Author: lafavemc
 */

#include <iostream>
#include <string>

using std::istream;
using std::string;

#include "translate_md.hh"


istream& translate_md ( istream& mdstream, const string& read, string& reference, const int& subpos, int& mdpos, const int& md_number, char& md_letter, const int& mdintnext_flag, const int& insert_flag, int& nonmatch_flag, const int& softclip_flag, const int& pad_flag )
{

		if ( insert_flag )
		{

			// If insert flag is set, ref gets a dash added to it.

			reference += '-';

		}
		else if ( softclip_flag )
		{

			// Else if softclip flag is set, ref gets an N added to it.

			reference += 'N';

		}
		else if ( pad_flag )
		{
			reference += '*';
		}
		else if ( !(mdintnext_flag) && mdpos < md_number )
		{

			// Else if the md entry is a digit (indicating a run of identical matches),
			// AND the md position is less than that value, the reference gets whatever
			// the modseq got from the read. Also increment mdposition.

			reference += read.substr(subpos, 1);
			++mdpos;

		}
		else if ( mdintnext_flag && md_letter == '^' )
		{

			// If the mdintnext_flag is set, you should be currently looking at
			// a character. Make sure that character isn't ^, and if it is,
			// grab the following character instead. ^ just indicates a
			// deletion, which is already taken into account by the CIGAR.

			if ( !(mdstream >> md_letter) )
			{
				throw 1;
			}
			else
			{
				reference += md_letter;

				++mdpos;
			}

		}
		else if ( mdintnext_flag )
		{
			// Else if the mdentry is a letter (case-insensitive, but probably caps),
			// put that letter on the reference output, set the mismatch flag, and increment.

			reference += md_letter;

			nonmatch_flag = 1;

			++mdpos;

		}

		return mdstream;

}

