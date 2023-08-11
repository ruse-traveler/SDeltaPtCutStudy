// ----------------------------------------------------------------------------
// 'SDeltaPtCutStudy.h'
// Derek Anderson
// 06.29.2023
//
// Reads in the 'ntp_track' Ntuple
// generated by the SVtxEvaluator
// class and studies how deltapt/pt
// varies with quality cuts.
// ----------------------------------------------------------------------------

#ifndef SDELTAPTCUTSTUDY_H
#define SDELTAPTCUTSTUDY_H

// standard c includes
#include <cmath>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <utility>
#include <iostream>
// root includes
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TPad.h"
#include "TFile.h"
#include "TLine.h"
#include "TGraph.h"
#include "TError.h"
#include "TString.h"
#include "TNtuple.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TPaveText.h"
#include "TDirectory.h"

using namespace std;



// SDeltaPtCutStudy definition ------------------------------------------------

class SDeltaPtCutStudy {

  public:

    // ctor/dtor [*.cc]
    SDeltaPtCutStudy();
    ~SDeltaPtCutStudy();

    // main methods [*.cc]
    void Init();
    void Analyze();
    void End();

    // setters [*.io.h]
    void SetInputOutputFiles(const TString sInput, const TString sOutput);
    void SetInputTuples(const TString sTrack, const TString sTruth);

  private:

    // constants
    enum CONSTANTS {
      NTxt     = 3,
      NPad     = 2,
      NPar     = 3,
      NVtx     = 4,
      NRange   = 2,
      NProj    = 8,
      NTrkCuts = 6,
      NDPtCuts = 7,
      NSigCuts = 5
    };

    // io methods [*.io.h]
    void OpenFiles();
    void GetTuples();
    void SaveOutput();
    void CloseFiles();

    // system methods [*.sys.h]
    void InitTuples();
    void InitHists();

    // analysis methods [*.ana.h]
    void ApplyFlatDeltaPtCuts();
    void ApplyPtDependentDeltaPtCuts();
    void FillTruthHistograms();
    void CreateSigmaGraphs();
    void CalculateRejectionFactors();
    void CalculateEfficiencies();

    // plot methods [*.plot.h]
    void SetStyles();
    void MakePlots();

    // io parameters
    TFile*   fInput;
    TFile*   fOutput;
    TNtuple* ntTrack;
    TNtuple* ntTruth;
    TString  sInFile;
    TString  sOutFile;
    TString  sInTrack;
    TString  sInTruth;

    // cut parameters [FIXME these should be user configurable]
    size_t nInttTrkMin                     = 1;
    size_t nMVtxTrkMin                     = 2;
    size_t nTpcTrkMin                      = 35;
    double qualTrkMax                      = 10.;
    double vzTrkMax                        = 10.;
    double ptTrkMin                        = 0.1;
    double ptDeltaMax[CONSTANTS::NDPtCuts] = {0.5, 0.25, 0.1, 0.05, 0.03, 0.02, 0.01};
    double ptDeltaSig[CONSTANTS::NSigCuts] = {1.,  1.5,  2.,  2.5,  3.};
    double normRange[CONSTANTS::NRange]    = {0.2, 1.2};

    // plot parameters [FIXME these should be user configurable]
    size_t iCutToDraw = CONSTANTS::NDPtCuts - 3;
    size_t iSigToDraw = CONSTANTS::NSigCuts - 3;
    size_t nEffRebin  = 5;
    bool   doEffRebin = true;

    // sigma calculation parameters [FIXME these should be user configurable]
    double ptProj[CONSTANTS::NProj]         = {0.5, 1., 2., 5., 10., 20., 30., 40.};
    double sigHiGuess[CONSTANTS::NPar]      = {1., -1., 1.};
    double sigLoGuess[CONSTANTS::NPar]      = {1., -1., 1.};
    double deltaFitRange[CONSTANTS::NRange] = {0.,  0.1};
    double ptFitRange[CONSTANTS::NRange]    = {0.5, 40.};

    // histogram parameters [FIXME these shoud be user configurable]
    TString sPtProjBase = "DeltaPtProj";
    TString sProjSuffix[CONSTANTS::NProj] = {
      "_pt05",
      "_pt1",
      "_pt2",
      "_pt5",
      "_pt10",
      "_pt20",
      "_pt30",
      "_pt40"
    }; 
    TString sDPtSuffix[CONSTANTS::NDPtCuts] = {
      "_dPt50",
      "_dPt25",
      "_dPt10",
      "_dPt05",
      "_dPt03",
      "_dPt02",
      "_dPt01"
    };
    TString sSigSuffix[CONSTANTS::NSigCuts] = {
      "_sigDPt1",
      "_sidDpt15",
      "_sigDPt2",
      "_sigDPt25",
      "_sigDPt3"
    };

    // generic histogram style parameters [FIXME these should be user configurable]
    uint32_t fFil     = 0;
    uint32_t fLin     = 1;
    uint32_t fWid     = 1;
    uint32_t fTxt     = 42;
    uint32_t fAln     = 12;
    uint32_t fCnt     = 1;
    uint32_t fColTrue = 923;
    uint32_t fColPure = 923;
    uint32_t fColTrk  = 809;
    uint32_t fMarTrue = 20;
    uint32_t fMarPure = 20;
    uint32_t fMarTrk  = 46;

    // cut-dependent histogram style paramters [FIXME these should be user configurable]
    uint32_t fColProj[CONSTANTS::NProj]   = {799, 633, 899, 617, 879, 859, 839, 819};
    uint32_t fMarProj[CONSTANTS::NProj]   = {20,  22,  23,  21,  33,  34,  47,  20};
    uint32_t fColCut[CONSTANTS::NDPtCuts] = {899, 909, 879, 889, 859, 869, 839};
    uint32_t fMarCut[CONSTANTS::NDPtCuts] = {24,  26,  32,  25,  27,  28,  30};

    // plot range parameters [FIXME these should be user configurable]
    float    rPtRange[CONSTANTS::NRange]    = {0., 60.};
    float    rFracRange[CONSTANTS::NRange]  = {0., 4.};
    float    rDeltaRange[CONSTANTS::NRange] = {0., 0.1};

    // graph/fit style parameters [FIXME these should be user configurable]
    uint32_t fColFit[CONSTANTS::NProj]       = {803, 636, 893, 620, 883, 863, 843, 813};
    uint32_t fColSigFit[CONSTANTS::NSigCuts] = {893, 903, 873, 883, 863};
    uint32_t fColSig[CONSTANTS::NSigCuts]    = {899, 909, 879, 889, 859};
    uint32_t fMarSig[CONSTANTS::NSigCuts]    = {24,  26,  32,  25,  27};

    // track tuple addresses
    float trk_event;
    float trk_seed;
    float trk_trackID;
    float trk_crossing;
    float trk_px;
    float trk_py;
    float trk_pz;
    float trk_pt;
    float trk_eta;
    float trk_phi;
    float trk_deltapt;
    float trk_deltaeta;
    float trk_deltaphi;
    float trk_charge;
    float trk_quality;
    float trk_chisq;
    float trk_ndf;
    float trk_nhits;
    float trk_nmaps;
    float trk_nintt;
    float trk_ntpc;
    float trk_nmms;
    float trk_ntpc1;
    float trk_ntpc11;
    float trk_ntpc2;
    float trk_ntpc3;
    float trk_nlmaps;
    float trk_nlintt;
    float trk_nltpc;
    float trk_nlmms;
    float trk_layers;
    float trk_vertexID;
    float trk_vx;
    float trk_vy;
    float trk_vz;
    float trk_dca2d;
    float trk_dca2dsigma;
    float trk_dca3dxy;
    float trk_dca3dxysigma;
    float trk_dca3dz;
    float trk_dca3dzsigma;
    float trk_pcax;
    float trk_pcay;
    float trk_pcaz;
    float trk_gtrackID;
    float trk_gflavor;
    float trk_gnhits;
    float trk_gnmaps;
    float trk_gnintt;
    float trk_gntpc;
    float trk_gnmms;
    float trk_gnlmaps;
    float trk_gnlintt;
    float trk_gnltpc;
    float trk_gnlmms;
    float trk_gpx;
    float trk_gpy;
    float trk_gpz;
    float trk_gpt;
    float trk_geta;
    float trk_gphi;
    float trk_gvx;
    float trk_gvy;
    float trk_gvz;
    float trk_gvt;
    float trk_gfpx;
    float trk_gfpy;
    float trk_gfpz;
    float trk_gfx;
    float trk_gfy;
    float trk_gfz;
    float trk_gembed;
    float trk_gprimary;
    float trk_nfromtruth;
    float trk_nwrong;
    float trk_ntrumaps;
    float trk_ntruintt;
    float trk_ntrutpc;
    float trk_ntrumms;
    float trk_ntrutpc1;
    float trk_ntrutpc11;
    float trk_ntrutpc2;
    float trk_ntrutpc3;
    float trk_layersfromtruth;
    float trk_nhittpcall;
    float trk_nhittpcin;
    float trk_nhittpcmid;
    float trk_nhittpcout;
    float trk_nclusall;
    float trk_nclustpc;
    float trk_nclusintt;
    float trk_nclusmaps;
    float trk_nclusmms;

    // truth tuple addresses
    float tru_event;
    float tru_seed;
    float tru_gntracks;
    float tru_gtrackID;
    float tru_gflavor;
    float tru_gnhits;
    float tru_gnmaps;
    float tru_gnintt;
    float tru_gnmms;
    float tru_gnintt1;
    float tru_gnintt2;
    float tru_gnintt3;
    float tru_gnintt4;
    float tru_gnintt5;
    float tru_gnintt6;
    float tru_gnintt7;
    float tru_gnintt8;
    float tru_gntpc;
    float tru_gnlmaps;
    float tru_gnlintt;
    float tru_gnltpc;
    float tru_gnlmms;
    float tru_gpx;
    float tru_gpy;
    float tru_gpz;
    float tru_gpt;
    float tru_geta;
    float tru_gphi;
    float tru_gvx;
    float tru_gvy;
    float tru_gvz;
    float tru_gvt;
    float tru_gfpx;
    float tru_gfpy;
    float tru_gfpz;
    float tru_gfx;
    float tru_gfy;
    float tru_gfz;
    float tru_gembed;
    float tru_gprimary;
    float tru_trackID;
    float tru_px;
    float tru_py;
    float tru_pz;
    float tru_pt;
    float tru_eta;
    float tru_phi;
    float tru_deltapt;
    float tru_deltaeta;
    float tru_deltaphi;
    float tru_charge;
    float tru_quality;
    float tru_chisq;
    float tru_ndf;
    float tru_nhits;
    float tru_layers;
    float tru_nmaps;
    float tru_nintt;
    float tru_ntpc;
    float tru_nmms;
    float tru_ntpc1;
    float tru_ntpc11;
    float tru_ntpc2;
    float tru_ntpc3;
    float tru_nlmaps;
    float tru_nlintt;
    float tru_nltpc;
    float tru_nlmms;
    float tru_vertexID;
    float tru_vx;
    float tru_vy;
    float tru_vz;
    float tru_dca2d;
    float tru_dca2dsigma;
    float tru_dca3dxy;
    float tru_dca3dxysigma;
    float tru_dca3dz;
    float tru_dca3dzsigma;
    float tru_pcax;
    float tru_pcay;
    float tru_pcaz;
    float tru_nfromtruth;
    float tru_nwrong;
    float tru_ntrumaps;
    float tru_ntruintt;
    float tru_ntrutpc;
    float tru_ntrumms;
    float tru_ntrutpc1;
    float tru_ntrutpc11;
    float tru_ntrutpc2;
    float tru_ntrutpc3;
    float tru_layersfromtruth;
    float tru_nhittpcall;
    float tru_nhittpcin;
    float tru_nhittpcmid;
    float tru_nhittpcout;
    float tru_nclusall;
    float tru_nclustpc;
    float tru_nclusintt;
    float tru_nclusmaps;
    float tru_nclusmms;

    // for sigma calculation
    double muProj[CONSTANTS::NProj];
    double sigProj[CONSTANTS::NProj];
    double muHiProj[CONSTANTS::NSigCuts][CONSTANTS::NProj];
    double muLoProj[CONSTANTS::NSigCuts][CONSTANTS::NProj];

    // for reject calculation
    uint64_t nNormCut[CONSTANTS::NDPtCuts];
    uint64_t nNormSig[CONSTANTS::NSigCuts];
    uint64_t nWeirdCut[CONSTANTS::NDPtCuts];
    uint64_t nWeirdSig[CONSTANTS::NSigCuts];
    double   rejCut[CONSTANTS::NDPtCuts];
    double   rejSig[CONSTANTS::NSigCuts];

    // for tuple loops
    uint64_t nTrks;
    uint64_t nTrus;

    // 1d histograms
    TH1D* hEff;
    TH1D* hPtTruth;
    TH1D* hPtDelta;
    TH1D* hPtTrack;
    TH1D* hPtFrac;
    TH1D* hPtTrkTru;
    TH1D* hPtDeltaProj[CONSTANTS::NProj];
    TH1D* hPtDeltaCut[CONSTANTS::NDPtCuts];
    TH1D* hPtDeltaSig[CONSTANTS::NSigCuts];
    TH1D* hPtTrackCut[CONSTANTS::NDPtCuts];
    TH1D* hPtTrackSig[CONSTANTS::NSigCuts];
    TH1D* hPtFracCut[CONSTANTS::NDPtCuts];
    TH1D* hPtFracSig[CONSTANTS::NSigCuts];
    TH1D* hPtTrkTruCut[CONSTANTS::NDPtCuts];
    TH1D* hPtTrkTruSig[CONSTANTS::NSigCuts];
    TH1D* hEffCut[CONSTANTS::NDPtCuts];
    TH1D* hEffSig[CONSTANTS::NSigCuts];

    // 2d histograms
    TH2D* hPtDeltaVsFrac;
    TH2D* hPtDeltaVsTrue;
    TH2D* hPtDeltaVsTrack;
    TH2D* hPtTrueVsTrack;
    TH2D* hPtDeltaVsFracCut[CONSTANTS::NDPtCuts];
    TH2D* hPtDeltaVsFracSig[CONSTANTS::NSigCuts];
    TH2D* hPtDeltaVsTrueCut[CONSTANTS::NDPtCuts];
    TH2D* hPtDeltaVsTrueSig[CONSTANTS::NSigCuts];
    TH2D* hPtDeltaVsTrackCut[CONSTANTS::NDPtCuts];
    TH2D* hPtDeltaVsTrackSig[CONSTANTS::NSigCuts];
    TH2D* hPtTrueVsTrackCut[CONSTANTS::NDPtCuts];
    TH2D* hPtTrueVsTrackSig[CONSTANTS::NSigCuts];

    // projection names
    TString sPtProj[CONSTANTS::NProj];

    // functions
    TF1* fMuHiProj[CONSTANTS::NSigCuts];
    TF1* fMuLoProj[CONSTANTS::NSigCuts];
    TF1 *fPtDeltaProj[CONSTANTS::NProj];

    // graphs
    TGraph* grMuProj;
    TGraph* grSigProj;
    TGraph* grRejCut;
    TGraph* grRejSig;
    TGraph* grMuHiProj[CONSTANTS::NSigCuts];
    TGraph* grMuLoProj[CONSTANTS::NSigCuts];

};  // end SDeltaPtCutStudy definition

#endif

// end ------------------------------------------------------------------------
