#!/usr/bin/bash

cd build
cmake .. && make -j
if [ $? -eq 0 ]; then
	echo -e "\n--------------- Build Success, Running... ---------------\n"
	cd ../bin
	./ymc-wallpaper-generator
else
	echo -e "\n--------------- Build Failed, Terminating... ---------------\n"
fi
cd ..
