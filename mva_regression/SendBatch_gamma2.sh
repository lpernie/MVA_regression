#!/bin/bash
cd /afs/cern.ch/work/l/lpernie/ECALpro/CMSSW_5_3_0/src/
export SCRAM_ARCH=slc5_amd64_gcc462
eval `scramv1 runtime -sh`
cd mva_regression
root -l -q -b trainPi0.C+'( "MVA_ETA_training.root","weights_Eta_2/weight_redPi0_EB.root",100,2,"EB", false )'
