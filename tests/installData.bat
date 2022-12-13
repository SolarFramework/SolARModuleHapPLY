@echo off
:: Download maps
echo Download and install maps
curl https://repository.solarframework.org/generic/maps/hololens/bcomLab/mapLabA_win_0_10_0.zip -L -o mapA.zip
powershell Expand-Archive mapA.zip -DestinationPath .\data\maps -F
del mapA.zip

