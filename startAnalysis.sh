#!/bin/bash
# run this from the DOI_scan folder

# run this file from the root folder of the doi scanning dataset. In principle you should prepare a folder with this structure

# produceDatasets.sh  
# startAnalysis.sh  
# y0/  
# y1/  
# y2/  
# y3/  
# y4/  
# y5/  
# y6/  
# y7/

# where the yN are folders, one for each row of the scan.
# Each yN folder should have the relative zN subfolders, one per point of the vertical scan. Finally each zN folder should have a Run_... folder with the .root files from the acquisition and the config file for the analysis. 
# You can start this macro by 
# ./startAnalysis.sh zconfig
# zconfig is the name of the config files you are using (without the .cfg extension)

# Before you start it, prepare the zconfig file properly. This means:
# go to the yN/zN/Run_... folder, and prepare the zconfig files in such a way that the right mppc numbers are selected etc. See example of relevant lines here:
# # doi tagging bench
# usingTaggingBench = 1      # if this is a tagging DOI bench measurement, put 1, otherwise 0
# taggingPosition = 13.6     # position of the tagging bench in mm 
# taggingCrystalChannel = 16  
# digiChannelsForDoi = 12,13,14,15
#  doiColumnOffset = 1        # for DOI output, fix the column i by adding this quantity. if not stated, 0 by default
# IMPORTANT - doiColumnOffset has to be chosen between 0 and 1 (or more) according the the real column under investigation in this y. otherwise it's always interpreted as 0 (i.e. the leftmost column of crystals in this mppc)

# Finally, choose the runList and zlist accordingly: write which y and which z you are going to run on.
# After this macro finishes, you can start the produceDatasets.sh macro (see that file for instructions) 

if [ $# -eq 0 ]
  then
    echo "No arguments supplied. You need to supply the name of the .cfg files used by ModuleCalibration. Only the name, without .cfg"
  else
  runList='y0 y1 y2 y3 y4 y5 y6 y7'
  zlist='z0 z1 z2 z3 z4 z5 z6 z7 z8 z9'
  macroName=$1
  mkdir -p all_$macroName
  foo () {
      local run=$1
      cd $run
      for j in $zlist; do cd $j/Run*/; ModuleCalibration -c ${macroName}.cfg `ls TTree_*` ; cd ../../; done
      cd ..
  }
  for run in $runList; do foo "$run" & done
fi

