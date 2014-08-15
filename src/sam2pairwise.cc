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

using std::istream;
using std::stringstream;
using std::string;
using std::getline;
using std::cin;
using std::cout;
using std::endl;
using std::vector;

#include "isolate_md.hh"
#include "shift_cigar.hh"
#include "shift_md.hh"
#include "translate_cigar.hh"
#include "translate_md.hh"



int main()
{

	string input_string;
	int pastheader = 0;

	while ( getline( cin, input_string ) )
	{

		// Verify that you aren't in the header
		if( !pastheader )
		{
			if(input_string.substr(0,1) == "@")
			{
				continue;
			}
			else
			{
				pastheader = 1;
			}
		}

		// Split the line into a vector

		string buf; // a buffer
		stringstream ss(input_string);

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
