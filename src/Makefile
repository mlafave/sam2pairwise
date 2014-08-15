CXX = g++
CC = g++

sam2pairwise:		sam2pairwise.o isolate_md.o shift_cigar.o shift_md.o translate_cigar.o translate_md.o 

isolate_md.o:		isolate_md.cc isolate_md.hh

shift_cigar.o:		shift_cigar.cc shift_cigar.hh

shift_md.o:		shift_md.cc shift_md.hh

translate_cigar.o:	translate_cigar.cc translate_cigar.hh

translate_md.o:		translate_md.cc translate_md.hh

sam2pairwise.o:		sam2pairwise.cc isolate_md.hh shift_cigar.hh shift_md.hh translate_cigar.hh translate_md.hh

clobber:
	rm -f *.o core sam2pairwise

