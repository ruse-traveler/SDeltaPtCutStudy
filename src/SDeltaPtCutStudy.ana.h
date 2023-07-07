// ----------------------------------------------------------------------------
// 'SDeltaPtCutStudy.ana.h'
// Derek Anderson
// 07.07.2023
//
// Reads in the 'ntp_track' Ntuple
// generated by the SVtxEvaluator
// class and studies how deltapt/pt
// varies with quality cuts.
// ----------------------------------------------------------------------------

#pragma once

using namespace std;



// analysis methods -----------------------------------------------------------

void SDeltaPtCutStudy::ApplyFlatDeltaPtCuts() {

  // announce start of track loop
  cout << "      First loop over reco. tracks:" << endl;

  // 1st track loop
  Long64_t nBytesTrk = 0;
  for (Long64_t iTrk = 0; iTrk < nTrks; iTrk++) {

    // grab entry
    const Long64_t bytesTrk = ntTrack -> GetEntry(iTrk);
    if (bytesTrk < 0.) {
      cerr << "WARNING: something wrong with track #" << iTrk << "! Aborting loop!" << endl;
      break;
    }
    nBytesTrk += bytesTrk;

    // announce progress
    const Long64_t iProgTrk = iTrk + 1;
    if (iProgTrk == nTrks) {
      cout << "          Processing track " << iProgTrk << "/" << nTrks << "..." << endl;
    } else {
      cout << "          Processing track " << iProgTrk << "/" << nTrks << "...\r" << flush;
    }

    // do calculations
    const Double_t ptFrac  = trk_pt / trk_gpt;
    const Double_t ptDelta = trk_deltapt / trk_pt;

    // apply trk cuts
    const Bool_t isInZVtxCut = (abs(trk_vz) <  vzTrkMax);
    const Bool_t isInInttCut = (trk_nintt   >= nInttTrkMin);
    const Bool_t isInMVtxCut = (trk_nlmaps  >  nMVtxTrkMin);
    const Bool_t isInTpcCut  = (trk_ntpc    >  nTpcTrkMin);
    const Bool_t isInPtCut   = (trk_pt      >  ptTrkMin);
    const Bool_t isInQualCut = (trk_quality <  qualTrkMax);
    const Bool_t isGoodTrk   = (isInZVtxCut && isInInttCut && isInMVtxCut && isInTpcCut && isInPtCut && isInQualCut);
    if (!isGoodTrk) continue;

    // fill histograms
    hPtDelta        -> Fill(ptDelta);
    hPtTrack        -> Fill(trk_pt);
    hPtFrac         -> Fill(ptFrac);
    hPtTrkTru       -> Fill(trk_gpt);
    hPtDeltaVsFrac  -> Fill(ptFrac,  ptDelta);
    hPtDeltaVsTrue  -> Fill(trk_gpt, ptDelta);
    hPtDeltaVsTrack -> Fill(trk_pt,  ptDelta);
    hPtTrueVsTrack  -> Fill(trk_pt,  trk_gpt);

    // apply delta-pt cuts
    const Bool_t isNormalTrk = ((ptFrac > normRange[0]) && (ptFrac < normRange[1]));
    for (Ssiz_t iCut = 0; iCut < CONSTANTS::NDPtCuts; iCut++) {
      const Bool_t isInDeltaPtCut = (ptDelta < ptDeltaMax[iCut]);
      if (isInDeltaPtCut) {

        // fill histograms
        hPtDeltaCut[iCut]        -> Fill(ptDelta);
        hPtTrackCut[iCut]        -> Fill(trk_pt);
        hPtFracCut[iCut]         -> Fill(ptFrac);
        hPtTrkTruCut[iCut]       -> Fill(trk_gpt);
        hPtDeltaVsFracCut[iCut]  -> Fill(ptFrac,  ptDelta);
        hPtDeltaVsTrueCut[iCut]  -> Fill(trk_gpt, ptDelta);
        hPtDeltaVsTrackCut[iCut] -> Fill(trk_pt,  ptDelta);
        hPtTrueVsTrackCut[iCut]  -> Fill(trk_pt,  trk_gpt);

        // increment counters
        if (isNormalTrk) {
          ++nNormCut[iCut];
        } else {
          ++nWeirdCut[iCut];
        }
      }
    }  // end delta-pt cut
  }  // end 1st track loop

  cout << "      First loop over reco. tracks finished!" << endl;
  return;

}  // end 'ApplyFlatDeltaPtCuts()'



void SDeltaPtCutStudy::ApplyPtDependentDeltaPtCuts() {

  // announce start of track loop
  cout << "      Second loop over reco. tracks:" << endl;

  // 2nd track loop
  Long64_t nBytesTrk = 0;
  for (Long64_t iTrk = 0; iTrk < nTrks; iTrk++) {

    // grab entry
    const Long64_t bytesTrk = ntTrack -> GetEntry(iTrk);
    if (bytesTrk < 0.) {
      cerr << "WARNING: something wrong with track #" << iTrk << "! Aborting loop!" << endl;
      break;
    }
    nBytesTrk += bytesTrk;

    // announce progress
    const Long64_t iProgTrk = iTrk + 1;
    if (iProgTrk == nTrks) {
      cout << "          Processing track " << iProgTrk << "/" << nTrks << "..." << endl;
    } else {
      cout << "          Processing track " << iProgTrk << "/" << nTrks << "...\r" << flush;
    }

    // do calculations
    const Double_t ptFrac  = trk_pt / trk_gpt;
    const Double_t ptDelta = trk_deltapt / trk_pt;

    // apply trk cuts
    const Bool_t isInZVtxCut = (abs(trk_vz) <  vzTrkMax);
    const Bool_t isInInttCut = (trk_nintt   >= nInttTrkMin);
    const Bool_t isInMVtxCut = (trk_nlmaps  >  nMVtxTrkMin);
    const Bool_t isInTpcCut  = (trk_ntpc    >  nTpcTrkMin);
    const Bool_t isInPtCut   = (trk_pt      >  ptTrkMin);
    const Bool_t isInQualCut = (trk_quality <  qualTrkMax);
    const Bool_t isGoodTrk   = (isInZVtxCut && isInInttCut && isInMVtxCut && isInTpcCut && isInPtCut && isInQualCut);
    if (!isGoodTrk) continue;

    // apply delta-pt cuts
    const Bool_t isNormalTrk = ((ptFrac > normRange[0]) && (ptFrac < normRange[1]));
    for (Ssiz_t iSig = 0; iSig < NSigCuts; iSig++) {

      // get bounds
      const Float_t ptDeltaMin = fMuLoProj[iSig] -> Eval(trk_pt);
      const Float_t ptDeltaMax = fMuHiProj[iSig] -> Eval(trk_pt);

      const Bool_t isInDeltaPtSigma = ((ptDelta >= ptDeltaMin) && (ptDelta <= ptDeltaMax));
      if (isInDeltaPtSigma) {

        // fill histograms
        hPtDeltaSig[iSig]        -> Fill(ptDelta);
        hPtTrackSig[iSig]        -> Fill(trk_pt);
        hPtFracSig[iSig]         -> Fill(ptFrac);
        hPtTrkTruSig[iSig]       -> Fill(trk_gpt);
        hPtDeltaVsFracSig[iSig]  -> Fill(ptFrac,  ptDelta);
        hPtDeltaVsTrueSig[iSig]  -> Fill(trk_gpt, ptDelta);
        hPtDeltaVsTrackSig[iSig] -> Fill(trk_pt,  ptDelta);
        hPtTrueVsTrackSig[iSig]  -> Fill(trk_pt,  trk_gpt);

        // increment counters
        if (isNormalTrk) {
          ++nNormSig[iSig];
        } else {
          ++nWeirdSig[iSig];
        }
      }
    }  // end delta-pt cut
  }  // end 1st track loop

  cout << "      Second loop over reco. tracks finished!" << endl;
  return;

}  // end 'ApplyPtDependentDeltaptCuts()'



void SDeltaPtCutStudy::FillTruthHistograms() {

  // announce start of truth loop
  cout << "      Loop over particles:" << endl;

  // truth loop
  Long64_t nBytesTru = 0;
  for (Long64_t iTru = 0; iTru < nTrus; iTru++) {

    // grab entry
    const Long64_t bytesTru = ntTruth -> GetEntry(iTru);
    if (bytesTru < 0.) {
      cerr << "WARNING: something wrong with particle #" << iTru << "! Aborting loop!" << endl;
      break;
    }
    nBytesTru += bytesTru;

    // announce progress
    const Long64_t iProgTru = iTru + 1;
    if (iProgTru == nTrus) {
      cout << "          Processing particle " << iProgTru << "/" << nTrus << "..." << endl;
    } else {
      cout << "          Processing particle" << iProgTru << "/" << nTrus << "...\r" << flush;
    }

    // fill truth histogram
    const Bool_t isPrimary = (tru_gprimary == 1);
    if (isPrimary) {
      hPtTruth -> Fill(tru_gpt);
    }
  }  // end track loop
  cout << "      Loop over particles finished!" << endl;

}  // end 'FillTruthHistograms()'



void SDeltaPtCutStudy::CreateSigmaGraphs() {

  // for graph names
  const TString sMuHiBase = "MeanPlusSigma";
  const TString sMuLoBase = "MeanMinusSigma";
  const TString sSigBase  = "ProjectionSigma";
  const TString sMuBase   = "ProjectionMean";

  // projection fit names
  TString sFitProj[CONSTANTS::NProj];
  for (Ssiz_t iProj = 0; iProj < CONSTANTS::NProj; iProj++) {
    sFitProj[iProj] = "f";
    sFitProj[iProj].Append(sPtProjBase.Data());
    sFitProj[iProj].Append(sProjSuffix[iProj].Data());
  }

  // project slices of delta-pt and get sigmas
  const UInt_t fWidFit = 2;
  const UInt_t fLinFit = 1;
  for (Ssiz_t iProj = 0; iProj < CONSTANTS::NProj; iProj++) {

    // do projection
    const UInt_t iBinProj = hPtDeltaVsTrack -> GetXaxis() -> FindBin(ptProj[iProj]);
    hPtDeltaProj[iProj]   = hPtDeltaVsTrack -> ProjectionY(sPtProj[iProj], iBinProj, iBinProj, "");

    // get initial values for fit
    const Float_t ampGuess = hPtDeltaProj[iProj] -> GetMaximum();
    const Float_t muGuess  = hPtDeltaProj[iProj] -> GetMean();
    const Float_t sigGuess = hPtDeltaProj[iProj] -> GetRMS();

    // fit with gaussian
    fPtDeltaProj[iProj] = new TF1(sFitProj[iProj].Data(), "gaus", deltaFitRange[0], deltaFitRange[1]);
    fPtDeltaProj[iProj] -> SetLineColor(fColFit[iProj]);
    fPtDeltaProj[iProj] -> SetLineStyle(fLinFit);
    fPtDeltaProj[iProj] -> SetLineWidth(fWidFit);
    fPtDeltaProj[iProj] -> SetParameter(0, ampGuess);
    fPtDeltaProj[iProj] -> SetParameter(1, muGuess);
    fPtDeltaProj[iProj] -> SetParameter(2, sigGuess);
    hPtDeltaProj[iProj] -> Fit(sFitProj[iProj].Data(), "R");

    // add values to arrays
    muProj[iProj]  = fPtDeltaProj[iProj] -> GetParameter(1);
    sigProj[iProj] = fPtDeltaProj[iProj] -> GetParameter(2);
    for (Ssiz_t iSig = 0; iSig < NSigCuts; iSig++) {
      muHiProj[iSig][iProj] = muProj[iProj] + (ptDeltaSig[iSig] * sigProj[iProj]);
      muLoProj[iSig][iProj] = muProj[iProj] - (ptDeltaSig[iSig] * sigProj[iProj]);
    }
  }  // end projection loop
  cout << "      Obtained delta-pt projections, fits, and sigmas." << endl;

  // sigma graph names
  TString sMuProj("gr");
  TString sSigProj("gr");
  sMuProj.Append(sMuBase.Data());
  sSigProj.Append(sSigBase.Data());

  TString sGrMuHiProj[CONSTANTS::NSigCuts];
  TString sGrMuLoProj[CONSTANTS::NSigCuts];
  TString sFnMuHiProj[CONSTANTS::NSigCuts];
  TString sFnMuLoProj[CONSTANTS::NSigCuts];
  for (Ssiz_t iSig = 0; iSig < CONSTANTS::NSigCuts; iSig++) {
    sGrMuHiProj[iSig] = "gr";
    sGrMuLoProj[iSig] = "gr";
    sFnMuHiProj[iSig] = "f";
    sFnMuLoProj[iSig] = "f";
    sGrMuHiProj[iSig].Append(sMuHiBase.Data());
    sGrMuLoProj[iSig].Append(sMuLoBase.Data());
    sFnMuHiProj[iSig].Append(sMuHiBase.Data());
    sFnMuLoProj[iSig].Append(sMuLoBase.Data());
    sGrMuHiProj[iSig].Append(sSigSuffix[iSig].Data());
    sGrMuLoProj[iSig].Append(sSigSuffix[iSig].Data());
    sFnMuHiProj[iSig].Append(sSigSuffix[iSig].Data());
    sFnMuLoProj[iSig].Append(sSigSuffix[iSig].Data());
  }

  // construct sigma graphs
  grMuProj  = new TGraph(CONSTANTS::NProj, ptProj, muProj);
  grSigProj = new TGraph(CONSTANTS::NProj, ptProj, sigProj);
  grMuProj  -> SetName(sMuProj);
  grSigProj -> SetName(sSigProj);

  fMuHiProj[CONSTANTS::NSigCuts];
  fMuLoProj[CONSTANTS::NSigCuts];
  grMuHiProj[CONSTANTS::NSigCuts];
  grMuLoProj[CONSTANTS::NSigCuts];
  for (Ssiz_t iSig = 0; iSig < NSigCuts; iSig++) {

    // create graphs
    grMuHiProj[iSig] = new TGraph(NProj, ptProj, muHiProj[iSig]);
    grMuLoProj[iSig] = new TGraph(NProj, ptProj, muLoProj[iSig]);
    grMuHiProj[iSig] -> SetName(sGrMuHiProj[iSig].Data());
    grMuLoProj[iSig] -> SetName(sGrMuLoProj[iSig].Data());

    // create fit functions
    fMuHiProj[iSig] = new TF1(sFnMuHiProj[iSig].Data(), "pol2", rPtRange[0], rPtRange[1]);
    fMuLoProj[iSig] = new TF1(sFnMuLoProj[iSig].Data(), "pol2", rPtRange[0], rPtRange[1]);
    fMuHiProj[iSig] -> SetLineColor(fColSigFit[iSig]);
    fMuLoProj[iSig] -> SetLineColor(fColSigFit[iSig]);
    fMuHiProj[iSig] -> SetLineStyle(fLinFit);
    fMuLoProj[iSig] -> SetLineStyle(fLinFit);
    fMuHiProj[iSig] -> SetLineWidth(fWidFit);
    fMuLoProj[iSig] -> SetLineWidth(fWidFit);
    fMuHiProj[iSig] -> SetParameter(0, sigHiGuess[0]);
    fMuLoProj[iSig] -> SetParameter(0, sigLoGuess[0]);
    fMuHiProj[iSig] -> SetParameter(1, sigHiGuess[1]);
    fMuLoProj[iSig] -> SetParameter(1, sigLoGuess[1]);
    fMuHiProj[iSig] -> SetParameter(2, sigHiGuess[2]);
    fMuLoProj[iSig] -> SetParameter(2, sigLoGuess[2]);

    // fit graphs
    grMuHiProj[iSig] -> Fit(sFnMuHiProj[iSig].Data(), "", "", ptFitRange[0], ptFitRange[1]);
    grMuLoProj[iSig] -> Fit(sFnMuLoProj[iSig].Data(), "", "", ptFitRange[0], ptFitRange[1]);
  }

  cout << "      Created and fit sigma graphs."  << endl;
  return;

}  // end 'CreateSigmaGraphs()'



void SDeltaPtCutStudy::CalculateRejectionFactors() {

  // for graph names
  const TString sRejCutBase = "Reject_flatDPtCut";
  const TString sRejSigBase = "Reject_sigmaCut";

  // calculate flat delta-pt rejection factors
  for (Ssiz_t iCut = 0; iCut < CONSTANTS::NDPtCuts; iCut++) {
    rejCut[iCut] = (Double_t) nNormCut[iCut] / (Double_t) nWeirdCut[iCut];
  }
  cout << "      Calculated flat delta-pt rejection factors." << endl;

  // calculate pt-dependent delta-pt rejection factors
  for (Ssiz_t iSig = 0; iSig < CONSTANTS::NSigCuts; iSig++) {
    rejSig[iSig] = (Double_t) nNormSig[iSig] / (Double_t) nWeirdSig[iSig];
  }
  cout << "      Calculated pt-depdendent delta-pt rejection factors\n"
       << "      Rejection factors:\n"
       << "        Flat delta-pt cuts"
       << endl;

  // announce flat delta-pt rejection factors
  for (Ssiz_t iCut = 0; iCut < CONSTANTS::NDPtCuts; iCut++) {
    cout << "          n(Norm, Weird) = (" << nNormCut[iCut] << ", " << nWeirdCut[iCut] << "), rejection = " << rejCut[iCut] << endl;
  }

  // announce pt-dependent delta-pt rejection factors
  cout << "        Pt-dependent delta-pt cuts" << endl;
  for (Ssiz_t iSig = 0; iSig < CONSTANTS::NSigCuts; iSig++) {
    cout << "          n(Norm, Weird) = (" << nNormSig[iSig] << ", " << nWeirdSig[iSig] << "), rejection = " << rejSig[iSig] << endl;
  }

  // make rejection graphs
  TString sRejCut("gr");
  TString sRejSig("gr");
  sRejCut.Append(sRejCutBase.Data());
  sRejSig.Append(sRejSigBase.Data());

  grRejCut = new TGraph(CONSTANTS::NDPtCuts, ptDeltaMax, rejCut);
  grRejSig = new TGraph(CONSTANTS::NSigCuts, ptDeltaSig, rejSig);
  grRejCut -> SetName(sRejCut.Data());
  grRejSig -> SetName(sRejSig.Data());

  cout << "      Made rejection factor graph." << endl; 
  return;

}  // end 'CalculateRejectionFactors()'



void SDeltaPtCutStudy::CalculateEfficiencies() {

  // for histogram names
  const TString sEffBase = "Efficiency";

  // rebin histograms if needed
  if (doEffRebin) {
    hPtTruth  -> Rebin(nEffRebin);
    hPtTrkTru -> Rebin(nEffRebin);
    for (Ssiz_t iCut = 0; iCut < CONSTANTS::NDPtCuts; iCut++) {
      hPtTrkTruCut[iCut] -> Rebin(nEffRebin);
    }
    for (Ssiz_t iSig = 0; iSig < NSigCuts; iSig++) {
      hPtTrkTruSig[iSig] -> Rebin(nEffRebin);
    }
    cout << "      Rebinned efficiency histograms." << endl;
  }

  TString sEff("h");
  sEff.Append(sEffBase.Data());

  // create flat delta-pt cut efficiency names
  TString sEffCut[CONSTANTS::NDPtCuts];
  for (Ssiz_t iCut = 0; iCut < CONSTANTS::NDPtCuts; iCut++) {
    sEffCut[iCut] = "h";
    sEffCut[iCut].Append(sEffBase.Data());
    sEffCut[iCut].Append(sDPtSuffix[iCut].Data());
  }

  // create pt-dependent delta-pt cut efficiency names
  TString sEffSig[CONSTANTS::NSigCuts];
  for (Ssiz_t iSig = 0; iSig < CONSTANTS::NSigCuts; iSig++) {
    sEffSig[iSig] = "h";
    sEffSig[iSig].Append(sEffBase.Data());
    sEffSig[iSig].Append(sSigSuffix[iSig].Data());
  }

  hEff = (TH1D*) hPtTruth -> Clone();
  hEff -> SetName(sEff.Data());
  hEff -> Reset("ICES");
  hEff -> Divide(hPtTrkTru, hPtTruth, 1., 1.);

  // calculate flat delta-pt cut efficiencies
  for (Ssiz_t iCut = 0; iCut < CONSTANTS::NDPtCuts; iCut++) {
    hEffCut[iCut] = (TH1D*) hPtTruth -> Clone();
    hEffCut[iCut] -> SetName(sEffCut[iCut].Data());
    hEffCut[iCut] -> Reset("ICES");
    hEffCut[iCut] -> Divide(hPtTrkTruCut[iCut], hPtTruth, 1., 1.);
  }

  // calculate pt-dependent delta-pt cut efficiencies
  for (Ssiz_t iSig = 0; iSig < CONSTANTS::NSigCuts; iSig++) {
    hEffSig[iSig] = (TH1D*) hPtTruth -> Clone();
    hEffSig[iSig] -> SetName(sEffSig[iSig].Data());
    hEffSig[iSig] -> Reset("ICES");
    hEffSig[iSig] -> Divide(hPtTrkTruSig[iSig], hPtTruth, 1., 1.);
  }

  cout << "      Calculated efficiencies." << endl;
  return;

}  // end 'CalculateEfficiencies()'

// end ------------------------------------------------------------------------
