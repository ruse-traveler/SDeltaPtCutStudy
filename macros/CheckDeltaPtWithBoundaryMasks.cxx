// ----------------------------------------------------------------------------
// 'CheckDeltaPtWithBoundaryMasks.cxx
// Derek Anderson
// 10.30.2023
//
// Use to quickly check what the delta-pt/pt distribution
// looks like if we mask the TPC sector boundaries.
// ----------------------------------------------------------------------------

// standard c libraries
#include <array>
#include <string>
#include <vector>
#include <utility>
#include <iostream>
// ROOT libraries
#include <TH1.h>
#include <TH2.h>
#include <TF1.h>
#include <TFile.h>
#include <TError.h>
#include <TNtuple.h>
#include <TDirectory.h>
#include <ROOT/RDataFrame.hxx>
#include <ROOT/RDF/HistoModels.hxx>

// make common namespaces implicit
using namespace std;
using namespace ROOT;

// set up aliases
using TH1Mod = ROOT::RDF::TH1DModel;
using TH2Mod = ROOT::RDF::TH2DModel;
using THBins = tuple<size_t, double, double>;
using TH1Def = tuple<string, string, THBins>;
using TH2Def = tuple<string, string, THBins, THBins>;
using Leaves = pair<string, string>;

// global constants
static const size_t NSectors = 12;



// check delta-pt/pt after masking sector boundaries --------------------------

void CheckDeltaPtWithBoundaryMasks() {

  // lower verbosity
  gErrorIgnoreLevel = kError;
  cout << "\n  Beginning sector boundary-masking check..." << endl;

  // options ------------------------------------------------------------------

  // i/o parameters
ldOutput
  const string sOutput  = "test.root";
  const string sInput   = "../TruthMatching/input/merged/sPhenixG4_oneMatchPerParticle_oldEvaluator.pt020num5evt500pim.d19m10y2023.root";
  const string sInTuple = "ntp_track";

  // mask parameters
  const double maskSize = 0.02;

  // fit guesses
  const array<float, NSectors> arrMeanGuess = {
    -2.877,
    -2.354,
    -1.831,
    -1.308,
    -0.785,
    -0.262,
    0.262,
    0.785,
    1.308,
    1.831,
    2.354,
    2.877
  };
  const float sigmaGuess = maskSize;
  const float ampGuess   = 10.;
  const float fitRange   = 2. * maskSize;

  // histogram definitions ----------------------------------------------------

  // binning accessor
  enum class Var {Ene, Phi, DPt, Frac};

  // other variable binning
  //   <0> = no. of bins
  //   <1> = 1st bin lower edge
  //   <2> = last bin upper edge
  const vector<THBins> vecBins = {
    make_tuple(200,    0.,   100.),
    make_tuple(360,   -3.15, 3.15),
    make_tuple(5000,   0.,   5.),
    make_tuple(5000,   0.,   5.)
  };

  // histogram accessors
  enum class Cut  {Before, Left, Cut};
  enum class Hist {PtReco, PtTrue, PtFrac, Phi, DPt, DPtVsPhi};

  // before vs. after labels
  const vector<string> vecCutLabels = {
    "_beforeMask",
    "_afterMask_leftIn",
    "_afterMask_cutOut"
  };

  // axis titles
  const vector<string> vecAxisTitles = {
    ";p_{T}^{reco} [GeV/c];counts",
    ";p_{T}^{true} [GeV/c];counts",
    ";p_{T}^{reco}/p_{T}^{true};counts",
    ";#varphi^{trk};counts",
    ";#deltap_{T}^{reco}/p_{T}^{reco}",
    ";#varphi^{trk};#deltap_{T}^{reco}/p_{T}^{reco}" 
  };

  // leaves to draw
  const vector<Leaves> vecLeaves = {
    make_pair("pt",     ""),
    make_pair("gpt",    ""),
    make_pair("ptFrac", ""),
    make_pair("phi",    ""),
    make_pair("ptErr",  ""),
    make_pair("phi",    "ptErr")
  };

  // 1d histogram definitions
  //   first = leaves to draw
  //   second = hist definition
  //     <0> = histogram name
  //     <1> = axis titles
  //     <2> = x-axis binning
  const vector<pair<Leaves, TH1Def>> vecHistDef1D = {
    make_pair(vecLeaves[static_cast<uint8_t>(Hist::PtReco)], make_tuple("hPtReco",  vecTitles[static_cast<uint8_t>(Hist::PtReco)].data(), vecBins[static_cast<uint8_t>(Var::Ene)])),
    make_pair(vecLeaves[static_cast<uint8_t>(Hist::PtTrue)], make_tuple("hPtTrue",  vecTitles[static_cast<uint8_t>(Hist::PtTrue)].data(), vecBins[static_cast<uint8_t>(Var::Ene)])),
    make_pair(vecLeaves[static_cast<uint8_t>(Hist::PtFrac)], make_tuple("hPtFrac",  vecTitles[static_cast<uint8_t>(Hist::PtFrac)].data(), vecBins[static_cast<uint8_t>(Var::Frac)])),
    make_pair(vecLeaves[static_cast<uint8_t>(Hist::Phi)], make_tuple("hPhi",     vecTitles[static_cast<uint8_t>(Hist::Phi)].data(),    vecBins[static_cast<uint8_t>(Var::Phi)])),
    make_pair(vecLeaves[static_cast<uint8_t>(Hist::DPt)], make_tuple("hDeltaPt", vecTitles[static_cast<uint8_t>(Hist::DPt)].data(),    vecBins[static_cast<uint8_t>(Var::DPt)]))
  };

  // 2d histogram definitions
  //   first = leaves to draw
  //   second = hist definition
  //     <0> = histogram name
  //     <1> = axis titles
  //     <2> = x-axis binning
  //     <3> = y-axis binning
  const vector<pair<Leaves, TH2Def>> vecHistDef2D = {
    make_pair(vecLeaves[static_cast<uint8_t>(Hist::DPtVsPhi)], make_tuple("hDPtVsPhi", vecTitles[static_cast<uint8_t>(Hist::DPtVsPhi)].data(), vecBins[static_cast<uint8_t>(Var::Phi)], vecBins[static_cast<uint8_t>(Var::DPt)]))
  }
  cout << "    Defined histograms." << endl;

  // create histograms
  const size_t nCuts = vecCutLabels.size();

  // for storing vectors
  vector<pair<Leaves, TH1Mod>>         vecArgAndHistRow1D;
  vector<pair<Leaves, TH2Mod>>         vecArgAndHistRow2D;
  vector<vector<pair<Leaves, TH1Mod>>> vecArgAndHist1D(nCuts);
  vector<vector<pair<Leaves, TH2Mod>>> vecArgAndHist2D(nCust);

  // instantiate histograms & load into vectors
  for (const string cut : vecCutLabels) {

    // make 1d hists
    vecArgAndHistRow1D.clear();
    for (const auto histDef1D : vecHistDef1D) {

      // make name
      string name = get<0>(histDef1D.second);
      name += cut;

      // instantiate hist
      THBins bins = get<2>(histDef1D.second);
      TH1Mod hist = TH1Mod(name, get<1>(histDef1D.second), get<0>(bins), get<1>(bins), get <2>(bins));
      vecArgAndHistRow1D.push_back(make_pair(histDef1D.first, hist));
    }
    vecArgAndHist1D.push_back(vecArgAndHistRow1D);

    // make 2d hists
    vecArgAndHistRow2D.clear();
    for (const auto histDef2D : vecHistDef2D) {

      // make name
      string name = get<0>(histDef2D.second);
      name += cut;

      // instantiate hist
      THBins xBins = get<2>(histDef2D.second);
      THBins yBins = get<3>(histDef2D.second);
      TH2Mod hist  = TH2Mod(name, get<1>(histDef2D.second), get<0>(xBins), get<1>(xBins), get <2>(xBins), get<0>(yBins), get<1>(yBins), get<2>(yBins));
      vecArgAndHistRow2D.push_back(make_pair(histDef2D.first, hist));
    }
    vecArgAndHist2D.push_back(vecArgAndHistRow1D);
  }  // end cut loop
  cout << "    Created histograms." << endl;

  // open files and make frames -----------------------------------------------

  // open output file
  TFile* fOutput = new TFile(sOutput.data(), "recreate");
  if (!fOutput) {
    cerr << "PANIC: couldn't open a file!\n"
         << "       fOutput = " << fOutput <<  "\n"
         << endl;
    return;
  }
  cout << "    Opened output file." << endl;

  // create data frame
  RDataFrame frame(sInTuple.data(), sInput.data());

  // make sure file isn't empty
  auto tracks = frame.Count();
  if (tracks == 0) {
    cerr << "Error: No tracks found!" << endl;
    return;
  }
  cout << "    Set up data frame." << endl;

  // helper lambdas -----------------------------------------------------------

  auto getFrac = [](const float a, const float b) { 
    return a / b;
  };  // end 'getFrac(float, float)'

  // for masking sector boundaries
  array<float, NSectors> arrSectors;
  arrSectors.fill(0.);

  auto isInMask = [](const float phi) {
    bool  inMask   = false;
    float halfMask = maskSize / 2.;
    for (const float sector : arrSectors) {
      if ((phi > (sector - halfMask)) && (phi < (sector + halfMask))) {
        inMask = true;
        break;
      }
    }
    return inMask;
  };  // end 'isInMask(float)'

  // run analyses -------------------------------------------------------------

  // get histograms before masking
  auto before = frame.Define("ptFrac", getFrac, {"pt", "gpt"})
                     .Define("ptErr",  getFrac, {"deltapt", "pt"};

  // get initial 1d histograms
  vector<vector<TH1D*>> vecOutHist1D;
  vector<vector<TH2D*>> vecOutHist2D;
  
  // save & close -------------------------------------------------------------

  // save histograms
  fOutput -> cd();
  for (auto histRow1D : vecOutHist1D) {
    for (auto hist1D : histRow1D) {
      hist1D -> Write();
    }
  }
  for (auto histRow2D : vecOutHist2D) {
    for (auto hist2D : histRow2D) {
      hist2D -> Write();
    }
  }
  cout << "    Saved histograms." << endl;

  // close files
  fOutput -> cd();
  fOutput -> Close();
  cout << "  Finished sector boundary-masking check!" << endl;

}

// end ------------------------------------------------------------------------
