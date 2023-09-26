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
#include <array>
#include <cmath>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <utility>
#include <iostream>
// root includes
#include <TH1.h>
#include <TH2.h>
#include <TF1.h>
#include <TPad.h>
#include <TFile.h>
#include <TLine.h>
#include <TGraph.h>
#include <TError.h>
#include <TString.h>
#include <TNtuple.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TVector.h>
#include <TPaveText.h>
#include <TDirectory.h>

using namespace std;



// SDeltaPtCutStudy definition ------------------------------------------------

class SDeltaPtCutStudy {

  // constants
  enum Const {
    NTxt     = 3,
    NPad     = 2,
    NPar     = 3,
    NVtx     = 4,
    NRange   = 2,
    NTypes   = 3,
    NTrkCuts = 6,
    NDPtCuts = 7,
    NSigCuts = 5
  };

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
    void SetGeneralTrackCuts(const uint32_t nInttCut, const uint32_t nMvtxCut, const uint32_t nTpcCut, const double qualCut, const double vzCut, const double ptCut);
    void SetPlotRanges(const pair<float, float> ptRange, const pair<float, float> fracRange, const pair<float, float> deltaRange);
    void SetGeneralStyleParameters(const array<uint32_t, Const::NTypes> arrCol, const array<uint32_t, Const::NTypes> arrMar);
    void SetGeneralHistParameters(const uint32_t fill, const uint32_t line, const uint32_t width, const uint32_t font, const uint32_t align, const uint32_t center);
    void SetEffRebinParameters(const bool doRebin, const size_t nRebin = 2);
    void SetProjectionParameters(const vector<tuple<double, TString, uint32_t, uint32_t, uint32_t>> projParams);

  private:

    // io methods [*.io.h]
    void OpenFiles();
    void GetTuples();
    void SaveOutput();
    void CloseFiles();

    // system methods [*.sys.h]
    void InitVectors();
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

    // general cut parameters
    uint32_t nInttTrkMin = 1;
    uint32_t nMVtxTrkMin = 2;
    uint32_t nTpcTrkMin  = 35;
    double   qualTrkMax  = 10.;
    double   vzTrkMax    = 10.;
    double   ptTrkMin    = 0.1;

    // delta-pt/pt cut parameters
    double ptDeltaMax[Const::NDPtCuts] = {0.5, 0.25, 0.1, 0.05, 0.03, 0.02, 0.01};
    double ptDeltaSig[Const::NSigCuts] = {1.,  1.5,  2.,  2.5,  3.};
    double normRange[Const::NRange]    = {0.2, 1.2};

    // plot parameters [FIXME these should be user configurable]
    size_t iCutToDraw = Const::NDPtCuts - 3;
    size_t iSigToDraw = Const::NSigCuts - 3;

    // sigma calculation parameters [FIXME these should be user configurable]
    double sigHiGuess[Const::NPar]      = {1., -1., 1.};
    double sigLoGuess[Const::NPar]      = {1., -1., 1.};
    double deltaFitRange[Const::NRange] = {0.,  0.1};
    double ptFitRange[Const::NRange]    = {0.5, 40.};

    // projection parameters
    size_t           nProj       = 0;
    TString          sPtProjBase = "DeltaPtProj";
    vector<double>   ptProj;
    vector<TString>  sProjSuffix;
    vector<uint32_t> fColProj;
    vector<uint32_t> fMarProj;
    vector<uint32_t> fColFit;

    // histogram parameters [FIXME these shoud be user configurable]
    TString sDPtSuffix[Const::NDPtCuts] = {
      "_dPt50",
      "_dPt25",
      "_dPt10",
      "_dPt05",
      "_dPt03",
      "_dPt02",
      "_dPt01"
    };
    TString sSigSuffix[Const::NSigCuts] = {
      "_sigDPt1",
      "_sidDpt15",
      "_sigDPt2",
      "_sigDPt25",
      "_sigDPt3"
    };

    // plot range parameters
    float rPtRange[Const::NRange]    = {0., 60.};
    float rFracRange[Const::NRange]  = {0., 4.};
    float rDeltaRange[Const::NRange] = {0., 0.1};

    // general histogram style parameters
    uint32_t fFil       = 0;
    uint32_t fLin       = 1;
    uint32_t fWid       = 1;
    uint32_t fTxt       = 42;
    uint32_t fAln       = 12;
    uint32_t fCnt       = 1;
    uint32_t fColTrue   = 923;
    uint32_t fColPure   = 923;
    uint32_t fColTrk    = 809;
    uint32_t fMarTrue   = 20;
    uint32_t fMarPure   = 20;
    uint32_t fMarTrk    = 46;
    size_t   nEffRebin  = 5;
    bool     doEffRebin = true;

    // cut-dependent histogram style paramters [FIXME these should be user configurable]
    uint32_t fColCut[Const::NDPtCuts] = {899, 909, 879, 889, 859, 869, 839};
    uint32_t fMarCut[Const::NDPtCuts] = {24,  26,  32,  25,  27,  28,  30};

    // graph/fit style parameters [FIXME these should be user configurable]
    uint32_t fColSigFit[Const::NSigCuts] = {893, 903, 873, 883, 863};
    uint32_t fColSig[Const::NSigCuts]    = {899, 909, 879, 889, 859};
    uint32_t fMarSig[Const::NSigCuts]    = {24,  26,  32,  25,  27};

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
    vector<double> muProj;
    vector<double> sigProj;
    array<vector<double>, Const::NSigCuts> muHiProj;
    array<vector<double>, Const::NSigCuts> muLoProj;

    // for reject calculation
    uint64_t nNormCut[Const::NDPtCuts];
    uint64_t nNormSig[Const::NSigCuts];
    uint64_t nWeirdCut[Const::NDPtCuts];
    uint64_t nWeirdSig[Const::NSigCuts];
    double   rejCut[Const::NDPtCuts];
    double   rejSig[Const::NSigCuts];

    // for tuple loops
    uint64_t nTrks;
    uint64_t nTrus;

    // general 1d histograms
    TH1D* hEff;
    TH1D* hPtTruth;
    TH1D* hPtDelta;
    TH1D* hPtTrack;
    TH1D* hPtFrac;
    TH1D* hPtTrkTru;
    TH1D* hPtDeltaCut[Const::NDPtCuts];
    TH1D* hPtDeltaSig[Const::NSigCuts];
    TH1D* hPtTrackCut[Const::NDPtCuts];
    TH1D* hPtTrackSig[Const::NSigCuts];
    TH1D* hPtFracCut[Const::NDPtCuts];
    TH1D* hPtFracSig[Const::NSigCuts];
    TH1D* hPtTrkTruCut[Const::NDPtCuts];
    TH1D* hPtTrkTruSig[Const::NSigCuts];
    TH1D* hEffCut[Const::NDPtCuts];
    TH1D* hEffSig[Const::NSigCuts];

    // 1d projection & delta-pt-dependent histograms
    vector<TH1D*> hPtDeltaProj;


    // general 2d histograms
    TH2D* hPtDeltaVsFrac;
    TH2D* hPtDeltaVsTrue;
    TH2D* hPtDeltaVsTrack;
    TH2D* hPtTrueVsTrack;
    TH2D* hPtDeltaVsFracCut[Const::NDPtCuts];
    TH2D* hPtDeltaVsFracSig[Const::NSigCuts];
    TH2D* hPtDeltaVsTrueCut[Const::NDPtCuts];
    TH2D* hPtDeltaVsTrueSig[Const::NSigCuts];
    TH2D* hPtDeltaVsTrackCut[Const::NDPtCuts];
    TH2D* hPtDeltaVsTrackSig[Const::NSigCuts];
    TH2D* hPtTrueVsTrackCut[Const::NDPtCuts];
    TH2D* hPtTrueVsTrackSig[Const::NSigCuts];

    // projection names
    vector<TString> sPtProj;

    // functions
    TF1* fMuHiProj[Const::NSigCuts];
    TF1* fMuLoProj[Const::NSigCuts];
    vector<TF1*> fPtDeltaProj;

    // graphs
    TGraph* grMuProj;
    TGraph* grSigProj;
    TGraph* grRejCut;
    TGraph* grRejSig;
    TGraph* grMuHiProj[Const::NSigCuts];
    TGraph* grMuLoProj[Const::NSigCuts];

};  // end SDeltaPtCutStudy definition

#endif

// end ------------------------------------------------------------------------
