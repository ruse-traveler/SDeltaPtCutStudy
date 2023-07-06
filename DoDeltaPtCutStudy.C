// ----------------------------------------------------------------------------
// 'DoTrackCutStudy.C'
// Derek Anderson
// 06.29.2023
//
// Runs the 'SDeltaPtCutStudy' class.
// ----------------------------------------------------------------------------

#ifndef DODELTAPTCUTSTUDY_C
#define DODELTAPTCUTSTUDY_C

// standard c includes
#include <cstdlib>
#include <utility>
// root includes
#include "TROOT.h"
#include "TString.h"
// user includes
#include </sphenix/user/danderson/install/include/sdeltaptcutstudy/SDeltaPtCutStudy.h>

using namespace std;

// load libraries
R__LOAD_LIBRARY(/sphenix/user/danderson/install/lib/libsdeltaptcutstudy.so)

// global constants
static const Bool_t DefBatch = false;



void DoDeltaPtCutStudy(const Bool_t inBatchMode = DefBatch) {

  // lower verbosity
  gErrorIgnoreLevel = kWarning;

  // io parameters
  const TString sInFile("../TrackCutStudy/input/embed_only/final_merge/sPhenixG4_forPtCheck_embedScanOn_embedOnly.pt2040n100pim.d8m5y2023.root");
  const TString sOutFile("test.root");
  const TString sInTrack("ntp_track");
  const TString sInTruth("ntp_gtrack");

  // run track cut study
  SDeltaPtCutStudy *study = new SDeltaPtCutStudy();
  study -> SetInputOutputFiles(sInFile, sOutFile);
  study -> SetInputTuples(sInTrack, sInTruth);
  study -> Init();
  study -> Analyze();
  study -> End();

}  // end 'DoDeltaPtCutStudy()'

#endif

// end ------------------------------------------------------------------------
