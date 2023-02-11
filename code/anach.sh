#/bin/bash

filename="daq-17772-NORM-"
filename1="daq-17772-NORM-"
raw="/media/psf/Expansion/lxx/2022-Yb/Empty"
path="/home/lixinxiang/lxx/CSNS/2022/data/Empty"
start=1
end=20
for(( i=${start}; i<=${end}; i++ ))

  do 

InputDatName="${filename1}0000${i}.raw"
OutputDatName="${filename}0000${i}.root"
if [ $i -ge 10 ] 
then
InputDatName="${filename1}000${i}.raw"
OutputDatName="${filename}000${i}.root"
fi
if [ $i -ge 100 ] 
then
InputDatName="${filename1}00${i}.raw"
OutputDatName="${filename}00${i}.root"
fi

./decode ${raw}/$InputDatName ${path}/$OutputDatName

done
