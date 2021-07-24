#!/bin/bash
length=$(($#-1))
OPTIONS=${@:1:$length}
REPONAME="${!#}"
CWD=$PWD
echo -e "\n\nInstalling commons libraries...\n\n"
COMMONS="so-commons-library"
git clone "https://github.com/sisoputnfrba/${COMMONS}.git" $COMMONS
cd $COMMONS
sudo make uninstall
make all
sudo make install
echo -e "\n\nInstalling libncurses5-dev...\n\n"
sudo apt-get install libncurses5-dev
echo -e "\n\nInstalling so-nivel-gui-library...\n\n"
MAP=so-nivel-gui-library
git clone https://github.com/sisoputnfrba/so-nivel-gui-library/
cd $MAP
sudo make install
cd $CWD
echo -e "\n\nBuilding projects...\n\n"
mkdir ./shared/obj
make clean -C ./Discordiador
mkdir ./Discordiador/obj
make -C ./Discordiador
make clean -C ./i-Mongo-Store
mkdir ./i-Mongo-Store/obj
make -C ./i-Mongo-Store
make clean -C ./Mi-Ram-HQ
mkdir ./Mi-Ram-HQ/obj
make -C ./Mi-Ram-HQ
echo -e "\n\nDeploy done!\n\n"
