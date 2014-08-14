/*
 * sam2pairwise.cpp
 *
 *  Created on: Aug 8, 2014
 *      Author: lafavemc
 */

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

string isolate_md( const vector<string>& samline )
{
	string md;

	for(int n = 11; n != samline.size(); ++n)
	{

		if(samline[n].substr(0,5) == "MD:Z:")
		{
			md = samline[n].substr(5, samline[n].size() - 5);
		}

	}

	return md;

}

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

			if ( number == 0 )
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

int translate_cigar( string& seq, const string& read, const char& letter, const int& seqposition, int& cigarposition, int& nonmatch_flag, int& insert_flag, int& softclip_flag )
{

	if( letter == 'M')
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
	else
	{
		// If the letter was N, H, P, =, or X, print that it currently
		// isn't supported.

		throw 1;

	}

	return 0;

}

istream& translate_md ( istream& mdstream, const string& read, string& reference, const int& subpos, int& mdpos, const int& md_number, char& md_letter, const int& mdintnext_flag, const int& insert_flag, int& nonmatch_flag, const int& softclip_flag )
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



int main()
{

	string test_string;

	while ( getline( cin, test_string ) )
	{

		// Split the line into a vector

		string buf; // a buffer
		stringstream ss(test_string);

		vector<string> line;

		while (ss >> buf)
		{
			line.push_back(buf);
		}



		// Isolate MD tag (quit if no MD tag)
		// Already know it can't be the first 11 columns, due to SAM spec.

		string md = isolate_md(line);

		if(md.empty())
		{
			cout << line[0] << "\t" << line[1] << "\t"<< line[2] << "\t"<< line[3]
			     << "\t"<< line[4] << "\t"<< line[5] << "\t"<< line[6] << "\t"
			     << line[7] << "\t"<< line[8] << endl << "MD tag not found"
			     << endl << endl << endl;
			continue;
		}



		// Initialize variables for the three substring positions, the content of the
		// CIGAR and MD, and the sequences for the new read, the reference, and the matches

		string read = line[9];

		int subpos = 0;
		int cigarpos = 0;
		int mdpos = 0;

		stringstream cigarstream(line[5]);
		int cigar_number;
		char cigar_letter;

		stringstream mdstream(md);
		int md_number;
		char md_letter;

		// Flags to tell if an integer or character should be next in the MD tag
		int mdintnext_flag = 1;

		string modseq;
		string matches;
		string reference;



		// While the substring counter is less than the length of the read
		while ( subpos < line[9].size() )
		{

			// If cigar position is 0, shift the first element(s) of the CIGAR vector
			// to get the distance and the letter

			if( cigarpos == 0 )
			{

				try {
					shift_cigar( cigarstream, cigar_number, cigar_letter );
				} catch (int e) {
					cout << "shift_cigar failed. Exiting." << endl;
					return 1;
				}

			}



			// If there's still something in the mdstream, mdposition is 0, and
			// CIGAR is not S (meaning you're not in the softclip region), shift
			// the elements of the MD until it's something other than 0.

			if ( (mdstream.rdbuf()->in_avail() != 0) && mdpos == 0 && cigar_letter != 'S')
			{

				try{

					shift_md( mdstream, mdintnext_flag, md_number, md_letter);

				} catch (int e){

					cout << "shift_md failed; didn't grab integer. Exiting." << endl;
					return 1;

				} catch (char e){

					cout << "shift_md failed; didn't grab character. Exiting." << endl;
					return 1;

				}

			}



			// Initialize flags for insertions, mismatches, and softclips

			int insert_flag = 0;
			int nonmatch_flag = 0;
			int softclip_flag = 0;

			// If the cigar position is less than the current cigar distance number:

			if( cigarpos < cigar_number )
			{

				try{

					translate_cigar( modseq, read, cigar_letter, subpos, cigarpos, nonmatch_flag, insert_flag, softclip_flag );

				} catch (int e){

					cout << line[0] << "\t" << line[1] << "\t" << line[2] << "\t"
					<< line[3] << "\t" << line[4] << "\t" << line[5] << "\t"
					<< line[6] << "\t" << line[7] << "\t" << line[8] << endl
					<< "Currently unsupported CIGAR character encountered" << endl
					<< endl << endl;

					break;

				}

			}



			// Now deal with the reference.

			try {

				translate_md( mdstream,  read, reference, subpos, mdpos, md_number, md_letter, mdintnext_flag, insert_flag, nonmatch_flag, softclip_flag );

			} catch (int e){

				cout << "Character not detected after ^ in MD tag. Exiting" << endl;
				return 1;

			}



			// If the mismatch flag is set, that line gets a space. If not, a |.

			if ( nonmatch_flag )
			{
				matches += ' ';
			}
			else
			{
				matches += '|';
			}



			// Check if either the cigarpos or mdpos need to be reset to 0.
			// If cigar position equals the cigar number, reset to 0.

			if ( cigarpos == cigar_number )
			{
				cigarpos = 0;
			}



			// Check the mdpos:

			if ( cigar_letter != 'S' && !(mdintnext_flag) && mdpos == md_number )
			{

				// If the CIGAR isn't S (and therefore it's reasonable to assume
				// that you're not going to examine md_number before it's actually
				// been defined by the contents of the MD tag), you're currently
				// looking at an integer, and the mdpos position is equal to that
				// integer, reset mdpos to 0.

				mdpos = 0;

			}
			else if ( mdintnext_flag )
			{
				// If you're currently looking at a character, and for some reason
				// mdpos isn't already 0, reset it to 0 now.

				mdpos = 0;

			}



			// If the cigar letter was anything except a D, increment the "subpos"
			// counter that tells you where you are in the read.

			if ( cigar_letter != 'D' )
			{
				++subpos;
			}

		}



		// Print the alignment

		if (subpos == line[9].size())
		{
			cout << line[0] << "\t" << line[1] << "\t"<< line[2] << "\t"<< line[3]
				 << "\t"<< line[4] << "\t"<< line[5] << "\t"<< line[6] << "\t"
				 << line[7] << "\t"<< line[8] << endl << modseq << endl << matches
				 << endl << reference << endl;
		}

	}

	return 0;

}
