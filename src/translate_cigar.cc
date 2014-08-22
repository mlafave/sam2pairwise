/*
 * translate_cigar.cpp
 *
 *  Created on: Aug 14, 2014
 *      Author: lafavemc
 */

#include <string>

using std::string;

#include "translate_cigar.hh"


int translate_cigar( string& seq, const string& read, const char& letter, const int& seqposition, int& cigarposition, int& nonmatch_flag, int& insert_flag, int& softclip_flag, int& pad_flag )
{

	if( letter == 'M' || letter == 'X' || letter == '=')
	{
		// If the cigar letter is M, take the next letter of the read and add
		// it to the output "modseq" read. Increment cigar position. (Or I COULD
		// do that after this 'if', since they all increment)

		seq += read.substr(seqposition, 1);
		++cigarposition;

		//cout << modseq << endl;

	}
	else if ( letter == 'I' )
	{

		// if it's I, take the next letter of the read, as above. However, also
		// set the insertion flag (causes ref to be printed as dashes) and the
		// non-match flag, and increment cigar position.

		seq += read.substr(seqposition, 1);

		insert_flag = 1;
		nonmatch_flag = 1;

		++cigarposition;

	}
	else if ( letter == 'D' )
	{

		// If the cigar letter is D, print a dash into the modseq. Set
		//nonmatch flag and increment cigar position.

		seq += '-';

		nonmatch_flag = 1;

		++cigarposition;

	}
	else if ( letter == 'S' )
	{

		// If the letter is S, take the next letter, set the softclip flag
		// (which prints reference as N), increment position

		seq += read.substr(seqposition, 1);

		nonmatch_flag = 1;
		softclip_flag = 1;

		++cigarposition;

	}
	else if ( letter == 'P' )
	{
		// If the letter is P, both the read and reference should be printed as
		// asterisks. This information isn't represented in the MD tag, so
		// set a flag here.

		seq += '*';

		nonmatch_flag = 1;
		pad_flag = 1;

		++cigarposition;

	}
	else
	{
		// If the letter was N, print that it currently
		// isn't supported.

		throw 1;

	}

	return 0;

}

