#!/usr/bin/bash

cd build || exit
cmake .. && make -j
if [ $? -eq 0 ]; then
	echo -e "\n--------------- Build Success, Running... ---------------\n"
	cd ../bin || exit
	./ymc-wallpaper-generator
	echo -e "\n--------------- Program Returned Exit Status $? ---------------\n"
else
	echo -e "\n--------------- Build Failed, Terminating... ---------------\n"
fi
cd ..
