#!/bin/sh

if [ -z $1 ]; then
    echo "Usage: $0 [Center-of-Mass Energy]"
    exit 1
fi

while :
do
    ./produceMCFM.sh 61 WW01 MSTW2008nlo68cl 0 0 1.0 1.0 0 $1
    ./produceMCFM.sh 71 WZ01 MSTW2008nlo68cl 12 12 1.0 1.0 0 $1
    ./produceMCFM.sh 71 WZ03 MSTW2008nlo68cl 40 40 1.0 1.0 0 $1
    ./produceMCFM.sh 76 WZ02 MSTW2008nlo68cl 12 12 1.0 1.0 0 $1
    ./produceMCFM.sh 76 WZ04 MSTW2008nlo68cl 40 40 1.0 1.0 0 $1
    ./produceMCFM.sh 81 ZZ01 MSTW2008nlo68cl 12 12 1.0 1.0 0 $1
    ./produceMCFM.sh 81 ZZ02 MSTW2008nlo68cl 40 40 1.0 1.0 0 $1
    ./produceMCFM.sh 82 ZZ01 MSTW2008nlo68cl 12 0 1.0 1.0 0 $1
    ./produceMCFM.sh 86 ZZ02 MSTW2008nlo68cl 1 1 1.0 1.0 0 $1
    ./produceMCFM.sh 141 tt01 MSTW2008nlo68cl 0 0 1.0 1.0 1 $1
    ./produceMCFM.sh 141 tt02 MSTW2008nlo68cl 0 0 1.0 1.0 2 $1

    ./produceMCFM.sh 61 WW02 NNPDF22nlo100 0 0 1.0 1.0 0 $1
    ./produceMCFM.sh 71 WZ05 NNPDF22nlo100 12 12 1.0 1.0 0 $1
    ./produceMCFM.sh 71 WZ07 NNPDF22nlo100 40 40 1.0 1.0 0 $1
    ./produceMCFM.sh 76 WZ06 NNPDF22nlo100 12 12 1.0 1.0 0 $1
    ./produceMCFM.sh 76 WZ08 NNPDF22nlo100 40 40 1.0 1.0 0 $1
    ./produceMCFM.sh 81 ZZ03 NNPDF22nlo100 12 12 1.0 1.0 0 $1
    ./produceMCFM.sh 81 ZZ04 NNPDF22nlo100 40 40 1.0 1.0 0 $1
    ./produceMCFM.sh 82 ZZ03 NNPDF22nlo100 12 0 1.0 1.0 0 $1
    ./produceMCFM.sh 86 ZZ04 NNPDF22nlo100 1 1 1.0 1.0 0 $1
    ./produceMCFM.sh 141 tt03 NNPDF22nlo100 0 0 1.0 1.0 1 $1
    ./produceMCFM.sh 141 tt04 NNPDF22nlo100 0 0 1.0 1.0 2 $1

    ./produceMCFM.sh 61 WW04 CT10 0 0 0.0 0.0 0 $1
    ./produceMCFM.sh 61 WW05 CT10 0 0 0.5 0.5 0 $1
    ./produceMCFM.sh 61 WW06 CT10 0 0 0.5 1.0 0 $1
    ./produceMCFM.sh 61 WW07 CT10 0 0 1.0 0.5 0 $1
    ./produceMCFM.sh 61 WW08 CT10 0 0 1.0 1.0 0 $1
    ./produceMCFM.sh 61 WW09 CT10 0 0 1.0 2.0 0 $1
    ./produceMCFM.sh 61 WW10 CT10 0 0 2.0 1.0 0 $1
    ./produceMCFM.sh 61 WW11 CT10 0 0 2.0 2.0 0 $1

    ./produceMCFM.sh 71 WZ13 CT10 12 12 0.0 0.0 0 $1
    ./produceMCFM.sh 71 WZ14 CT10 12 12 0.5 0.5 0 $1
    ./produceMCFM.sh 71 WZ15 CT10 12 12 0.5 1.0 0 $1
    ./produceMCFM.sh 71 WZ16 CT10 12 12 1.0 0.5 0 $1
    ./produceMCFM.sh 71 WZ17 CT10 12 12 1.0 1.0 0 $1
    ./produceMCFM.sh 71 WZ18 CT10 12 12 1.0 2.0 0 $1
    ./produceMCFM.sh 71 WZ19 CT10 12 12 2.0 1.0 0 $1
    ./produceMCFM.sh 71 WZ20 CT10 12 12 2.0 2.0 0 $1

    ./produceMCFM.sh 71 WZ21 CT10 40 40 0.0 0.0 0 $1
    ./produceMCFM.sh 71 WZ22 CT10 40 40 0.5 0.5 0 $1
    ./produceMCFM.sh 71 WZ23 CT10 40 40 0.5 1.0 0 $1
    ./produceMCFM.sh 71 WZ24 CT10 40 40 1.0 0.5 0 $1
    ./produceMCFM.sh 71 WZ25 CT10 40 40 1.0 1.0 0 $1
    ./produceMCFM.sh 71 WZ26 CT10 40 40 1.0 2.0 0 $1
    ./produceMCFM.sh 71 WZ27 CT10 40 40 2.0 1.0 0 $1
    ./produceMCFM.sh 71 WZ28 CT10 40 40 2.0 2.0 0 $1

    ./produceMCFM.sh 76 WZ29 CT10 12 12 0.0 0.0 0 $1
    ./produceMCFM.sh 76 WZ30 CT10 12 12 0.5 0.5 0 $1
    ./produceMCFM.sh 76 WZ31 CT10 12 12 0.5 1.0 0 $1
    ./produceMCFM.sh 76 WZ32 CT10 12 12 1.0 0.5 0 $1
    ./produceMCFM.sh 76 WZ33 CT10 12 12 1.0 1.0 0 $1
    ./produceMCFM.sh 76 WZ34 CT10 12 12 1.0 2.0 0 $1
    ./produceMCFM.sh 76 WZ35 CT10 12 12 2.0 1.0 0 $1
    ./produceMCFM.sh 76 WZ36 CT10 12 12 2.0 2.0 0 $1

    ./produceMCFM.sh 76 WZ37 CT10 40 40 0.0 0.0 0 $1
    ./produceMCFM.sh 76 WZ38 CT10 40 40 0.5 0.5 0 $1
    ./produceMCFM.sh 76 WZ39 CT10 40 40 0.5 1.0 0 $1
    ./produceMCFM.sh 76 WZ40 CT10 40 40 1.0 0.5 0 $1
    ./produceMCFM.sh 76 WZ41 CT10 40 40 1.0 1.0 0 $1
    ./produceMCFM.sh 76 WZ42 CT10 40 40 1.0 2.0 0 $1
    ./produceMCFM.sh 76 WZ43 CT10 40 40 2.0 1.0 0 $1
    ./produceMCFM.sh 76 WZ44 CT10 40 40 2.0 2.0 0 $1

    ./produceMCFM.sh 81 ZZ07 CT10 12 12 0.0 0.0 0 $1
    ./produceMCFM.sh 81 ZZ08 CT10 12 12 0.5 0.5 0 $1
    ./produceMCFM.sh 81 ZZ09 CT10 12 12 0.5 1.0 0 $1
    ./produceMCFM.sh 81 ZZ10 CT10 12 12 1.0 0.5 0 $1
    ./produceMCFM.sh 81 ZZ11 CT10 12 12 1.0 1.0 0 $1
    ./produceMCFM.sh 81 ZZ12 CT10 12 12 1.0 2.0 0 $1
    ./produceMCFM.sh 81 ZZ13 CT10 12 12 2.0 1.0 0 $1
    ./produceMCFM.sh 81 ZZ14 CT10 12 12 2.0 2.0 0 $1

    ./produceMCFM.sh 81 ZZ15 CT10 40 40 0.0 0.0 0 $1
    ./produceMCFM.sh 81 ZZ16 CT10 40 40 0.5 0.5 0 $1
    ./produceMCFM.sh 81 ZZ17 CT10 40 40 0.5 1.0 0 $1
    ./produceMCFM.sh 81 ZZ18 CT10 40 40 1.0 0.5 0 $1
    ./produceMCFM.sh 81 ZZ19 CT10 40 40 1.0 1.0 0 $1
    ./produceMCFM.sh 81 ZZ20 CT10 40 40 1.0 2.0 0 $1
    ./produceMCFM.sh 81 ZZ21 CT10 40 40 2.0 1.0 0 $1
    ./produceMCFM.sh 81 ZZ22 CT10 40 40 2.0 2.0 0 $1

    ./produceMCFM.sh 82 ZZ05 CT10 12 0 0.0 0.0 0 $1
    ./produceMCFM.sh 82 ZZ06 CT10 12 0 0.5 0.5 0 $1
    ./produceMCFM.sh 82 ZZ07 CT10 12 0 0.5 1.0 0 $1
    ./produceMCFM.sh 82 ZZ08 CT10 12 0 1.0 0.5 0 $1
    ./produceMCFM.sh 82 ZZ09 CT10 12 0 1.0 1.0 0 $1
    ./produceMCFM.sh 82 ZZ10 CT10 12 0 1.0 2.0 0 $1
    ./produceMCFM.sh 82 ZZ11 CT10 12 0 2.0 1.0 0 $1
    ./produceMCFM.sh 82 ZZ12 CT10 12 0 2.0 2.0 0 $1

    ./produceMCFM.sh 86 ZZ13 CT10 1 1 0.0 0.0 0 $1
    ./produceMCFM.sh 86 ZZ14 CT10 1 1 0.5 0.5 0 $1
    ./produceMCFM.sh 86 ZZ15 CT10 1 1 0.5 1.0 0 $1
    ./produceMCFM.sh 86 ZZ16 CT10 1 1 1.0 0.5 0 $1
    ./produceMCFM.sh 86 ZZ17 CT10 1 1 1.0 1.0 0 $1
    ./produceMCFM.sh 86 ZZ18 CT10 1 1 1.0 2.0 0 $1
    ./produceMCFM.sh 86 ZZ19 CT10 1 1 2.0 1.0 0 $1
    ./produceMCFM.sh 86 ZZ20 CT10 1 1 2.0 2.0 0 $1

    ./produceMCFM.sh 141 tt06 CT10 0 0 0.0 0.0 0 $1
    ./produceMCFM.sh 141 tt07 CT10 0 0 0.5 0.5 1 $1
    ./produceMCFM.sh 141 tt08 CT10 0 0 0.5 1.0 1 $1
    ./produceMCFM.sh 141 tt09 CT10 0 0 1.0 0.5 1 $1
    ./produceMCFM.sh 141 tt10 CT10 0 0 1.0 1.0 1 $1
    ./produceMCFM.sh 141 tt11 CT10 0 0 1.0 2.0 1 $1
    ./produceMCFM.sh 141 tt12 CT10 0 0 2.0 1.0 1 $1
    ./produceMCFM.sh 141 tt13 CT10 0 0 2.0 2.0 1 $1

    ./produceMCFM.sh 141 tt14 CT10 0 0 0.5 0.5 2 $1
    ./produceMCFM.sh 141 tt15 CT10 0 0 0.5 1.0 2 $1
    ./produceMCFM.sh 141 tt16 CT10 0 0 1.0 0.5 2 $1
    ./produceMCFM.sh 141 tt17 CT10 0 0 1.0 1.0 2 $1
    ./produceMCFM.sh 141 tt18 CT10 0 0 1.0 2.0 2 $1
    ./produceMCFM.sh 141 tt19 CT10 0 0 2.0 1.0 2 $1
    ./produceMCFM.sh 141 tt20 CT10 0 0 2.0 2.0 2 $1

    break
done

mkdir JOB
mv *.DAT JOB/

exit
