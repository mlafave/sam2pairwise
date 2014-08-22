#!/usr/bin/env bash

prog='../src/sam2pairwise'

test()
{
	if diff $1 $2; then
    	echo ok
	else
    	echo fail
	fi
}

###########################################################
#  Test a perfect match
###########################################################
echo "    t01..."
echo \
"MS2007487-600V3:1:1101:06791:15000	163	cecr1a_T1_25_17051437-17051625	1	70	189M	=	1	188
GTTTCAGTGGATTGGCTGGTGAAGAACGTGACCTACAGAGAGAACTGCTATGTTTGCTTCACAGATAAGCAGACCGTGCAGTTCATCTTCTCCTCTGGACCACCAGCATCCAGTTCACACTGCTCCTCATGGACTCTGCTCAGATCCCTCAGGGAAAAAGTTAAAAACACCACTGAACTGGACAACAGG
|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
GTTTCAGTGGATTGGCTGGTGAAGAACGTGACCTACAGAGAGAACTGCTATGTTTGCTTCACAGATAAGCAGACCGTGCAGTTCATCTTCTCCTCTGGACCACCAGCATCCAGTTCACACTGCTCCTCATGGACTCTGCTCAGATCCCTCAGGGAAAAAGTTAAAAACACCACTGAACTGGACAACAGG" > expected
$prog <test_M_noMD > observed
test observed expected
rm observed expected


###########################################################
#  Test a deletion & soft clip
###########################################################
echo "    t02..."
echo \
"MS2007487-600V3:1:1103:08049:16258	163	man2b1_T2_11_32098232-32098503	1	70	141M3D128M7S	=	1	271
CTCAAGAAAATGCAGGTTTGCATAAAAGTACCTGTGAAGAGATCGGCAATAGGTGGAGTCAGGCTCTCACTGGCCCTCCAGAGCATTTCCATCTCTTTGGTCTTCATCCTGCGGTCCTTGTCCTGGTAGTCCAAGCGGCCA---AAGAAGCCGTCATAGCCCATCTATAAACAAATCAGAGCAGAGCCATCAGCAACTTTAGATATTAAAATACAATTCATGATGCCAGATAACATGCCATGGCTACCTGAGCAAAGATGGAGGCGTGTTCTAAACCAA
|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||   ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||       
CTCAAGAAAATGCAGGTTTGCATAAAAGTACCTGTGAAGAGATCGGCAATAGGTGGAGTCAGGCTCTCACTGGCCCTCCAGAGCATTTCCATCTCTTTGGTCTTCATCCTGCGGTCCTTGTCCTGGTAGTCCAAGCGGCCAAAGAAGAAGCCGTCATAGCCCATCTATAAACAAATCAGAGCAGAGCCATCAGCAACTTTAGATATTAAAATACAATTCATGATGCCAGATAACATGCCATGGCTACCTGAGCAAAGATGGAGGCGTGTTCTNNNNNNN" > expected
$prog <test_DS > observed
test observed expected
rm observed expected


###########################################################
#  Test a soft clip & insertion
###########################################################
echo "    t03..."
echo \
"MS2007487-600V3:1:2104:28968:11326	147	cecr1b_T2_4_5106979-5107305	39	70	1S110M3I179M	=	1	-326
AAAGGCTCTTTCAAACCCTGGTAGATGTGATCCTTCAGGACAGGAGCATATGTAATCAGCCCAGACAGAGCAATGAAAATCTTCTCAAACCGCTTCCATATATAGTCCTATATAATAGGATAAGACGTCTCCGGGTCTTCCGTAAAAAGCGTTAGCTTACGCATCAAGCTGAAAGATAATGGCTGAGTTAGTCTACAGTAGTCAACGTTTGGATTTGGTTAATTAAATGAGAGCTATTATGCCAAACTCACTCTTTAACTCACTTGTTGTTTAAAAAAGGTTGTGTGGCAACA
 || ||||||||||||||||||||||||||||||||||||||||||||||||| ||||||||||||||||||||||||||||||||||||||||||||||||||||||| |    | |||||||||||||||||||||||||||||||||||||| |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| ||||||||||||||||||||||||||||||||||||||||||||||||||
NAAAGCTCTTTCAAACCCTGGTAGATGTGATCCTTCAGGACAGGAGCATATGTGATCAGCCCAGACAGAGCAATGAAAATCTTCTCAAACCGCTTCCATATATAGTCCTGT---GTGGGATAAGACGTCTCCGGGTCTTCCGTAAAAAGCGTTAGATTACGCATCAAGCTGAAAGATAATGGCTGAGTTAGTCTACAGTAGTCAACGTTTGGATTTGGTTAATTAAATGAGAGCTATTATGCAAAACTCACTCTTTAACTCACTTGTTGTTTAAAAAAGGTTGTGTGGCAACA" > expected
$prog <test_SI > observed
test observed expected
rm observed expected


###########################################################
#  Test terminal mismatches & an insertion followed by mismatches
###########################################################
echo "    t04..."
echo \
"MS2007487-600V3:1:2109:08999:01136	163	cecr1a_T2_25_17051701-17051898	1	70	99M5I99M	=	1	197
TTCGAAATCTCACGCTCTTTACTGAAGACCCAGNTAGAGCTTATCCTAATCAGGATACTGTGTGGGANAGNTTCGAGCAGGTATTTTTAGTGNCCTATGCCCCACTATGGTCACTTATGCCCCTGTCTTTAAAGACTACCTCTATGAAGGTCTCCGACAGTTTTATGAGGACAACATCACGTATGTGGAAATCAGAGCACTNN
||||||||||||||||||||||||||||||||| ||||||||||||||||||||||||||||||||| || ||||||||||||||||||||| ||||||        |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| ||||||||| |||||||||||  
TTCGAAATCTCACGCTCTTTACTGAAGACCCAGATAGAGCTTATCCTAATCAGGATACTGTGTGGGAGAGGTTCGAGCAGGTATTTTTAGTGGCCTATG-----GGTTGGTCACTTATGCCCCTGTCTTTAAAGACTACCTCTATGAAGGTCTCCGACAGTTTTATGAGGACAACATCATGTATGTGGAGATCAGAGCACTGC" > expected
$prog <test_MD0end > observed
test observed expected
rm observed expected


###########################################################
#  Test a CIGAR with X and =
###########################################################
echo "    t05..."
echo \
"MS2007487-600V3:1:2109:08999:01136	163	cecr1a_T2_25_17051701-17051898	1	70	3=1X4=2X4=	=	1	197
CTTATATTGGCCTT
||| ||||  ||||
CTTCTATTATCCTT" > expected
$prog <test_X= > observed
test observed expected
rm observed expected


###########################################################
#  Test a CIGAR that starts with hard clipping
###########################################################
echo "    t06..."
echo \
"CON_5	16	EGFR_Exon_19	67	255	66H26M15D76M	*	0	0
AAAGTTAAAATTCCCGTCGCTATCAA---------------AACATCTCCGAAAGCCAACAAGGAAATCCTCGATGTGAGTTTCTGCTTTGCTGTGTGGGGGTCCATGGCTCTGAAC
||||||||||||||||||||||||||               ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
AAAGTTAAAATTCCCGTCGCTATCAAGGAATTAAGAGAAGCAACATCTCCGAAAGCCAACAAGGAAATCCTCGATGTGAGTTTCTGCTTTGCTGTGTGGGGGTCCATGGCTCTGAAC" > expected
$prog <test_Hstart > observed
test observed expected
rm observed expected


###########################################################
#  Test an insertion with no MD tag
###########################################################
echo "    t07..."
echo \
"CRATOS:145:D1UH5ACXX:4:1309:20149:28710	83	Zv9_scaffold3453	7862	70	92M1I8M	=	7609	-352
CATTATAATTTCTTAAAATATTAATGACTGCAGAACATGGGCTGCTCTACAAATACATATTTTAATTATGGCAAGAATAAAAGCAAAAAAAAGAAAAAGTA
|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| ||||||||
CATTATAATTTCTTAAAATATTAATGACTGCAGAACATGGGCTGCTCTACAAATACATATTTTAATTATGGCAAGAATAAAAGCAAAAAAAA-AAAAAGTA" > expected
$prog <test_I_noMD > observed
test observed expected
rm observed expected


###########################################################
#  Test an insertion with no MD tag
###########################################################
echo "    t08..."
echo \
"CRATOS:145:D1UH5ACXX:4:1309:20149:28710	83	Zv9_scaffold3453	7862	70	92M1D8M	=	7609	-352
D encountered in CIGAR without MD tag

" > expected
$prog <test_D_noMD 2> observed
test observed expected
rm observed expected


###########################################################
#  Test a CIGAR that doesn't begin with an integer
###########################################################
echo "    t09..."
echo \
"CRATOS:145:D1UH5ACXX:2:1308:6211:53528	153	Zv9_scaffold3453	49562	26	S15M1D65M2I18M	=	49562	0
shift_cigar failed. Exiting." > expected
$prog <test_charstartCIGAR 2> observed
test observed expected
rm observed expected


###########################################################
#  Test a CIGAR with an unsupported character
###########################################################
echo "    t10..."
echo \
"MS2007487-600V3:1:1101:06791:15000	163	cecr1a_T1_25_17051437-17051625	1	70	189?	=	1	188
Currently unsupported CIGAR character encountered

" > expected
$prog <test_unsupportedCIGAR 2> observed
test observed expected
rm observed expected


###########################################################
#  Test a CIGAR with P
###########################################################
echo "    t11..."
echo \
"r002	0	ref	9	30	1S2I6M1P1I1P1I4M2I	*	0	0
AAAAGATAA*G*GGATAAA
   ||||||    ||||  
N--AGATAA*-*-GATA--" > expected
$prog <test_SPI > observed
test observed expected
rm observed expected


exit;