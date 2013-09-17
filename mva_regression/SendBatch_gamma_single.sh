#!/bin/bash
cd /afs/cern.ch/work/l/lpernie/pi0/MVA/JoshMVA/CMSSW_5_3_0/src/
export SCRAM_ARCH=slc5_amd64_gcc462
eval `scramv1 runtime -sh`
cd UserCode/CITHZZ/LeptonScaleAndResolution/Electrons
echo 'root -l -q -b trainPi0.C+( "LocalPi0Gun_GAMMA_train.root","weights_GAMMA/weight_Mediumtrain.root",100,0 )'
root -l -q -b trainPi0.C+'( "LocalPi0Gun_GAMMA_train.root","weights_GAMMA/weight_Mediumtrain.root",100,0 )'
