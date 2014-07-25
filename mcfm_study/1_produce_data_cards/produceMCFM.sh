#!/bin/sh

if [ -z $9 ]; then
    echo "Usage: $0 [PROC] [STRING] [PDF] [MINZ34] [MINZ56] [SCALE] [FSCALE] [DYN][CME]"
    echo "DYN=0: no\tDYN=1: HT\tDYN=2: sqrt(M^2+pt345^2)"
    exit 1
fi

export mw=80.398
export mz=91.1867
export mt=172.5
export no=0

if [ $1 -ge 61 ] && [ $1 -le 69 ]; then
    echo "This is a WW production"
    SCALE=`echo "$6 * $mw" | bc`
    FSCALE=`echo "$7 * $mw" | bc`
    MIN34=$no
    MIN56=$no
    ITE=40

elif [ $1 -ge 71 ] && [ $1 -le 80 ]; then
    echo "This is a WZ production"
    SCALE=`echo "$6 * 0.50 * ($mw + $mz)" | bc`
    FSCALE=`echo "$7 * 0.50 * ($mw + $mz)" | bc`
    MIN34=$4
    MIN56=$5
    ITE=40

elif [ $1 -ge 81 ] && [ $1 -le 90 ]; then
    echo "This is a ZZ production"
    SCALE=`echo "$6 * $mz" | bc`
    FSCALE=`echo "$7 * $mz" | bc`
    MIN34=$4
    MIN56=$5
    ITE=40

elif [ $1 -ge 141 ] && [ $1 -le 151 ]; then
    echo "This is a ttbar production"
    if [ $8 -eq 1 ] || [ $8 -eq 2 ]; then
	SCALE=$6
	FSCALE=$7
    fi
    MIN34=$4
    MIN56=$5
    ITE=20

else
    echo "Not valid option!"
    SCALE=-1
    FSCALE=-1
    MIN34=$no
    MIN56=$no
    ITE=20

fi

if [ $(echo "$6 == 0"|bc) -eq 1 ] && [ $(echo "$7 == 0"|bc) -eq 1 ]; then
    SCALE=-1
    FSCALE=-1
fi

export DYN0="no"
export DYN1="HT"
export DYN2="sqrt(M^2+pt345^2)"

if [ $8 -eq 0 ]; then
    DSCALE=$DYN0
    echo "DYNAMIC SCALE:" $DSCALE
elif [ $8 -eq 1 ]; then
    DSCALE=$DYN1
    echo "DYNAMIC SCALE:" $DSCALE
elif [ $8 -eq 2 ]; then
    DSCALE=$DYN2
    echo "DYNAMIC SCALE:" $DSCALE
else
    echo "Not valid option!"
    DSCALE=$DYN0
    echo "DYNAMIC SCALE:" $DSCALE
fi 

PROC=$1
STRING=$2
PDF=$3
MINZ34=$4
MINZ56=$5
DYN=$8
CME=$9
INCLUSIVE=true
SHORT1=`echo "$3" | cut -c1-5`
SHORT2=`echo "$DSCALE" | cut -c1-2`
SHORT3=`echo "$2" | cut -c1-2`
FILENAME=${SHORT1}_${PROC}_${6}_${7}_${SHORT2}_cut${MIN34}_cut${MIN56}_${SHORT3}.DAT
echo "Output file name: " $FILENAME

cat > ${FILENAME} << EOF
'6.6'		 [file version number]

[Flags to specify the mode in which MCFM is run]
.false.		 [evtgen]
.false.		 [creatent]
.false.		 [skipnt]
.false.		 [dswhisto]
.false.		 [writetop]
.false.		 [writedat]
.false.		 [writegnu]
.true.		 [writeroot]
.false.		 [writepwg] 

[General options to specify the process and execution]
${PROC}		 [nproc]
'tota'		 [part 'lord','real' or 'virt','tota']
'${STRING}'    	 ['runstring']
${CME}000d0		 [sqrts in GeV]
+1		 [ih1 =1 for proton and -1 for antiproton]
+1		 [ih2 =1 for proton and -1 for antiproton]
126d0		 [hmass]
${SCALE}d0     	 [scale:QCD scale choice]
${FSCALE}d0    	 [facscale:QCD fac_scale choice]
'${DSCALE}'    	 [dynamicscale of the four momentum sum of outgoing parton]
.false.		 [zerowidth]
.false.		 [removebr]
${ITE}		 [itmx1, number of iterations for pre-conditioning]
100000		 [ncall1]
${ITE}		 [itmx2, number of iterations for final run]
100000		 [ncall2]
1089		 [ij]
.false.		 [dryrun]
.true.		 [Qflag]
.true.		 [Gflag]

[Heavy quark masses]
173.2d0		 [top mass]
4.75d0		 [bottom mass]
1.5d0		 [charm mass]

[Pdf selection]
'cteq66m'		 [pdlabel]
4			 [NGROUP, see PDFLIB]
46			 [NSET - see PDFLIB]
${PDF}.LHgrid  		 [LHAPDF group]
-1			 [LHAPDF set]

[Jet definition and event cuts]
${MIN34}d0     	 [m34min]
14000d0		 [m34max]
${MIN56}d0     	 [m56min]
14000d0		 [m56max]
.true.		 [inclusive]
'ankt'		 [algorithm]
15d0		 [ptjet_min]
0d0		 [|etajet|_min]
3d0		 [|etajet|_max]
0.5d0		 [Rcut_jet]  
.false.		 [makecuts]
0d0		 [ptlepton_min]
99d0		 [|etalepton|_max]
0d0		 [ptmin_missing]
0d0		 [ptlepton(2nd+)_min]
99d0		 [|etalepton(2nd+)|_max]
0.0d0		 [minimum (3,4) transverse mass] 
0.0d0		 [R(jet,lept)_min]
0.0d0		 [R(lept,lept)_min]
0d0		 [Delta_eta(jet,jet)_min]
.false.		 [jets_opphem]
0		 [lepbtwnjets_scheme]
0d0		 [ptmin_bjet]
99d0		 [etamax_bjet]

[Settings for photon processes]
.false.		 [fragmentation included]
'BFGsetII'	 [fragmentation set]
80d0		 [fragmentation scale]
20d0		 [ptmin_photon]
2.5d0		 [etamax_photon]
10d0		 [ptmin_photon2]
0.7d0		 [R(photon,lept)_min]
0.4d0		 [R(photon,photon)_min]
0.7d0		 [cone size for isolation]
0.4d0		 [epsilon_h, energy fraction for isolation]

[Anomalous couplings of the W and Z]
0.0d0		 [Delta_g1(Z)]
0.0d0		 [Delta_K(Z)]
0.0d0		 [Delta_K(gamma)]
0.0d0		 [Lambda(Z)]
0.0d0		 [Lambda(gamma)]
0.0d0		 [h1(Z)]
0.0d0		 [h1(gamma)]
0.0d0		 [h2(Z)]
0.0d0		 [h2(gamma)]
0.0d0		 [h3(Z)]
0.0d0		 [h3(gamma)]
0.0d0		 [h4(Z)]
0.0d0		 [h4(gamma)]
2.0d0		 [Form-factor scale, in TeV]

[How to resume/save a run]
.false.		 [readin]
.false.		 [writeout]
''		 [ingridfile]
''		 [outgridfile]

[Technical parameters that should not normally be changed]
.false.		 [debug]
.true.		 [verbose]
.false.		 [new_pspace]
.false.		 [virtonly]
.false.		 [realonly]
.true.		 [spira]
.false.		 [noglue]
.false.		 [ggonly]
.false.		 [gqonly]
.false.		 [omitgg]
.false.		 [vanillafiles]
1		 [nmin]
2		 [nmax]
.true.		 [clustering]
.false.		 [realwt]
0		 [colourchoice]
1d-2		 [rtsmin]
1d-4		 [cutoff]
1d0		 [aii]
1d0		 [aif]
1d0		 [afi]
1d0		 [aff]
1d0		 [bfi]
1d0		 [bff]
EOF

exit
