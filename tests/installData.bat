@echo off
:: Download maps
echo Download and install maps
curl https://artifact.b-com.com/solar-generic-local/maps/hololens/bcomLab/mapLabA_win_0_10_0.zip -L -o mapA.zip
powershell Expand-Archive mapA.zip -DestinationPath .\data\maps -F
del mapA.zip

