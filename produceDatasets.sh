#!/bin/bash

# run this from the DOI_scan folder
# run it after the startAnalysis.sh macro

# Use the same runList and zlist you just used in startAnalysis.sh.
# run it with
# ./produceDatasets.sh zconfig
# zconfig is the name of the config files you are using (without the .cfg extension)

# the final dataset will be in the all_zconfig folder

if [ $# -eq 0 ]
  then
    echo "No arguments supplied. You need to supply the name of the .cfg files used by ModuleCalibration. Only the name, without .cfg"
  else
  runList='y0 y1 y2 y3 y4 y5 y6 y7'
  zlist='z0 z1 z2 z3 z4 z5 z6 z7 z8 z9'
  macroName=$1
  for i in $runList ; do for j in $zlist; do cp $i/$j/Run*/doiData.txt $i/$j/doiData.txt ; done; done
  for i in $runList 
  do
    cd $i
    doifit `echo $zlist`
    cd ..
  done
  for i in $runList ; do cp $i/calibration_params.txt ./all_$macroName/${i}calibration_params.txt ; cp $i/doi.txt ./all_$macroName/${i}doi.txt; done
  #delete the old global files, create the new ones
  cd all_$macroName
  rm calibration_params.txt
  rm doi.txt
  for i in $runList ; do  cat ${i}calibration_params.txt >> calibration_params.txt; done
  for i in $runList ; do  cat ${i}doi.txt >> doi.txt; done
  cd ..
fi
