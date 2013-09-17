// --> >// $Id: rootlogon.C,v 1.1 2013/02/05 13:09:28 lpernie Exp $
{
 {
  TString libstr(Form("%s/lib/%s/%s",
                      gSystem->Getenv("CMSSW_BASE"),
                      gSystem->Getenv("SCRAM_ARCH"),
                      "libCondFormatsEgammaObjects.so"));

  gSystem->Load(libstr);
 }
 {
  TString libstr(Form("%s/lib/%s/%s",
                      gSystem->Getenv("CMSSW_BASE"),
                      gSystem->Getenv("SCRAM_ARCH"),
                      "libMitEdmGBRTrain.so"));

  gSystem->Load(libstr);
 }
  gSystem->AddIncludePath("-I$ROOFITSYS/include");
  gSystem->AddIncludePath("-I$CMSSW_BASE/src/CondFormats/EgammaObjects/interface");
  gSystem->AddIncludePath("-I$CMSSW_BASE/src/MitEdm/GBRTrain/interface");

  gInterpreter->AddIncludePath((TString(":")+TString(gSystem->Getenv("CMSSW_BASE"))+
				TString("/src/CondFormats/EgammaObjects/interface")).Data());
  gInterpreter->AddIncludePath((TString(":")+TString(gSystem->Getenv("CMSSW_BASE"))+
                                TString("/src/MitEdm/GBRTrain/interface")).Data());                                
  gInterpreter->AddIncludePath((TString(":")+TString(gSystem->Getenv("ROOFITSYS"))+
				TString("/include")).Data());

}


