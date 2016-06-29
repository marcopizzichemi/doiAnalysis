#!/bin/bash
# macro to modify some parameters in ALL the zconfig files listed in yN zN. Can be adapted to fit your needs, assuming you know what you are doing. 
# Just set some string to be searched for and removed (ex. zLine=`grep -n "output" zconfig.cfg | grep -Eo '^[^:]+'`) and
# how to replace them (ex. stringOut="output = zdoiNearW")



for i in y0 y1 y2 y3 y4 y5 y6 y7 
do 
  for j in z0 z1 z2 z3 z4 z5 z6 z7 z8 z9
  do 
    cd $i/$j/Run* 
    zLine=`grep -n "output" zconfig.cfg | grep -Eo '^[^:]+'`
    stringOut="output = zdoiAllW"
    stringW="wAllChannels      = 1"
    sed "${zLine}d" zconfig.cfg | cat - > temp.cfg 
    echo $stringOut   >> temp.cfg
    echo $stringW     >> temp.cfg
    cp temp.cfg zconfigAllW.cfg
    rm temp.cfg
    stringOut="output = zdoiNearW"
    stringW="wAllChannels      = 0"
    sed "${zLine}d" zconfig.cfg | cat - > temp.cfg
    echo $stringOut   >> temp.cfg
    echo $stringW     >> temp.cfg
    cp temp.cfg zconfigNearW.cfg
    rm temp.cfg
    cd ../../.. 
  done 
done
