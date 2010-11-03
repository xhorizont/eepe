\Qt\2010.05\qt\bin\lupdate.exe eepe.pro
\Qt\2010.05\qt\bin\lconvert.exe -i eepe_.ts -o eepe_.po
\Qt\2010.05\qt\bin\lconvert.exe -i eepe_he.ts -o eepe_he.po
\Qt\2010.05\qt\bin\lconvert.exe -i eepe_fr.ts -o eepe_fr.po
\Qt\2010.05\qt\bin\lrelease.exe eepe.pro
del eepe_.qm
move *.qm "..\eepe-build-desktop\release\"