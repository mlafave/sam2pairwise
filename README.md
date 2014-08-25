sam2pairwise
============

sam2pairwise takes a SAM file and uses the CIGAR and MD tag to reconstruct the pairwise alignment of each read. 


Requirements
------------

A C++ compiler, such as G++.


Installation
------------

To compile sam2pairwise, enter the src/ directory and type:

    make

sam2pairwise can then be moved to /usr/bin/ or wherever is convenient. 
To remove the compiled files, enter the src/ directory and type:

    make clobber


Usage
-------

Input can be read out of a SAM file:

    sam2pairwise < file.sam > file.out

Or from a pipe:

    samtools view file.bam | sam2pairwise > file.out

To display the current version and quit, enter:

	sam2pairwise --version


Output
-------

sam2pairwise currently supports the following CIGAR elements for reconstructing the alignment:

* M: Alignment match. The MD tag is used to determine if this is a sequence match or mismatch.
* I: Insertion to the reference, indicated by '-' on the reference sequence.
* D: Deletion from the reference, indicated '-' on the read sequence.
* N: Skipped region from the reference, indicated by '.' on the read and 'N' on the reference sequence.
* S: Soft clipping. These characters are represented normally on the read sequence, and as 'N's on the reference sequence.
* H: Hard clipping. These characters are no longer present in the read sequence, so this value is skipped.
* P: Padding. Represented as an * in both the read and reference sequences.
* =: Sequence match. Treated the same as 'M'.
* X: Sequence mismatch. Treated the same as 'M'.

Each line of input has a corresponding four lines of output. The first line serves as the header, and reproduces the first nine mandatory fields from the SAM entry. The second line is the read itself, the third indicates sequence matches, and the fourth represents the reference sequence.

So, an input like this:

    CRATOS:145:D1UH5ACXX:2:1308:6211:53528	153	Zv9_scaffold3453	49562	26	1S15M1D65M2I18M	=	49562	0	GCCTGAGAACAAGTGAGAAAGAAACTCATTCCTGTCTTTCAATGAGTGCTTTTGTGCATTTAGGAGAACTAGGCAGCACACATTTAGGGCTGAAAGATGNA	(CCDCCCCAEECCDFFFFFFFHECHFHJIHDJIIIIJJJJJJJJJJJJIHGIFJJIGJJJIIIIIIJIJIJIGIIHGFCCJJJJJIJJHGHHHFFFDA1#C	PG:Z:novoalign	AS:i:206	UQ:i:206	NM:i:7	MD:Z:15^T35A30C7C6G1

Would produce an output like this:

    CRATOS:145:D1UH5ACXX:2:1308:6211:53528	153	Zv9_scaffold3453	49562	26	1S15M1D65M2I18M	=	49562	0
    GCCTGAGAACAAGTGA-GAAAGAAACTCATTCCTGTCTTTCAATGAGTGCTTTTGTGCATTTAGGAGAACTAGGCAGCACACATTTAGGGCTGAAAGATGNA
     ||||||||||||||| ||||||||||||||||||||||||||||||||||| |||||||||||||||||||||||||||||  | ||||||| |||||| |
    NCCTGAGAACAAGTGATGAAAGAAACTCATTCCTGTCTTTCAATGAGTGCTTATGTGCATTTAGGAGAACTAGGCAGCACAC--TCAGGGCTGCAAGATGGA

In the event that the MD tag is not detected & the CIGAR contains characters that require the MD tag to resolve, or if an unsupported character is detected in the CIGAR, the program maintains the four-line periodicity of the output by printing the header output, an error message, and two blank lines.


License
-------

sam2pairwise is licensed under the MIT license. In addition, as a work created at the National Institutes of Health, it carries the following notice:
 
This software/database is either a United States Government Work or was made under contract for the United States Government. In either case the public may use the software/database on a worldwide and royalty-free basis for any purpose and may reproduce and prepare derivative works without limitation. Although all reasonable efforts have been taken to ensure the accuracy and reliability of the software/database and associated data, the National Human Genome Research Institute (NHGRI), National Institutes of Health (NIH) and the U.S. Government do not and cannot warrant the performance or results that may be obtained by using this software/database or data. NHGRI, NIH and the U.S. Government disclaim all warranties as to performance, merchantability or fitness for any particular purpose.


Citation
--------

If you use sam2pairwise in your work, please use the following DOI to cite it:

[![DOI](https://zenodo.org/badge/doi/10.5281/zenodo.11377.png)](http://dx.doi.org/10.5281/zenodo.11377)


Contact
-------

Matthew C. LaFave, Ph.D.
Developmental Genomics Section, Translational and Functional Genomics Branch
NHGRI, NIH

Email: matthew.lafave [at sign] nih.gov


