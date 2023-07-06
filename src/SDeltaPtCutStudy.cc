// ----------------------------------------------------------------------------
// 'SDeltaPtCutStudy.cc'
// Derek Anderson
// 06.29.2023
//
// Reads in the 'ntp_track' Ntuple
// generated by the SVtxEvaluator
// class and studies how deltapt/pt
// varies with quality cuts.
// ----------------------------------------------------------------------------

#define SDELTAPTCUTSTUDY_CC

// header files
#include "SDeltaPtCutStudy.h"

using namespace std;



// ctor/dtor ------------------------------------------------------------------

SDeltaPtCutStudy::SDeltaPtCutStudy() {

  /* stuff will go here */

}  // end ctor




SDeltaPtCutStudy::~SDeltaPtCutStudy() {

  /* stuff will go here */

}  // end dtor



//  main methods --------------------------------------------------------------

void SDeltaPtCutStudy::Init() {

  // announce initialization
  cout << "    Initializing..." << endl;

  // grab input
  OpenFiles();
  GetTuples();

  // initialize input/output
  InitTuples();
  InitHists();
  return;

}  // end Init()



void SDeltaPtCutStudy::Analyze() {

  /* stuff will go here */
  return;

}  // end Analyze()



void SDeltaPtCutStudy::End() {

  // announce completion
  cout << "    Finishing..." << endl;

  SaveOutput();
  CloseFiles();
  return ;

}  // end End()

// end ------------------------------------------------------------------------
