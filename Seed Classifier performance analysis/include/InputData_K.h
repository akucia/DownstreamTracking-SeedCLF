#ifndef INPUTDATA_H
#define INPUTDATA_H
#include <string>

#include "TTree.h"
#include "TFile.h"

class InputData_K{

	public:
	InputData_K(std::string filepath, std::string treepath){
		file = new TFile(filepath.c_str());
		tree = (TTree*)file->Get(treepath.c_str());
		nrOfEvents = tree->GetEntries(); 
		setBranches();
    }

	~InputData_K(){
		if(tree != nullptr){delete tree; tree = nullptr;}
		if(file != nullptr){delete file; file = nullptr;}
	}
	
	void reset(){
		eventNr = 0;
	}
	
	void setBranches();

	bool nextEvent(){
		if(eventNr < nrOfEvents){
			tree->GetEntry(eventNr);
			++eventNr;
			return 1;
		}
		else
			return 0;
	}

	
	TFile* file = nullptr;
	TTree* tree = nullptr;

	unsigned long eventNr = 0;
	unsigned long nrOfEvents = 0;

// Fixed size dimensions of array or collections stored in the TTree if any.
    const Int_t kMaxKS0_ENDVERTEX_COV = 1;
    const Int_t kMaxKS0_OWNPV_COV = 1;
    const Int_t kMaxpiplus_OWNPV_COV = 1;
    const Int_t kMaxpiplus_ORIVX_COV = 1;
    const Int_t kMaxpiminus_OWNPV_COV = 1;
    const Int_t kMaxpiminus_ORIVX_COV = 1;
    
    // Declaration of leaf types
    Double_t        KS0_LV01;
    Double_t        KS0_eta;
    Double_t        KS0_phi;
    Double_t        KS0_ENDVERTEX_X;
    Double_t        KS0_ENDVERTEX_Y;
    Double_t        KS0_ENDVERTEX_Z;
    Double_t        KS0_ENDVERTEX_XERR;
    Double_t        KS0_ENDVERTEX_YERR;
    Double_t        KS0_ENDVERTEX_ZERR;
    Double_t        KS0_ENDVERTEX_CHI2;
    Int_t           KS0_ENDVERTEX_NDOF;
    Float_t         KS0_ENDVERTEX_COV_[3][3];
    Double_t        KS0_OWNPV_X;
    Double_t        KS0_OWNPV_Y;
    Double_t        KS0_OWNPV_Z;
    Double_t        KS0_OWNPV_XERR;
    Double_t        KS0_OWNPV_YERR;
    Double_t        KS0_OWNPV_ZERR;
    Double_t        KS0_OWNPV_CHI2;
    Int_t           KS0_OWNPV_NDOF;
    Float_t         KS0_OWNPV_COV_[3][3];
    Double_t        KS0_IP_OWNPV;
    Double_t        KS0_IPCHI2_OWNPV;
    Double_t        KS0_P;
    Double_t        KS0_PT;
    Double_t        KS0_PE;
    Double_t        KS0_PX;
    Double_t        KS0_PY;
    Double_t        KS0_PZ;
    Double_t        KS0_MM;
    Double_t        KS0_MMERR;
    Double_t        KS0_M;
    Int_t           KS0_ID;
    Double_t        piplus_LV01;
    Double_t        piplus_eta;
    Double_t        piplus_phi;
    Double_t        piplus_MC12TuneV2_ProbNNe;
    Double_t        piplus_MC12TuneV2_ProbNNmu;
    Double_t        piplus_MC12TuneV2_ProbNNpi;
    Double_t        piplus_MC12TuneV2_ProbNNk;
    Double_t        piplus_MC12TuneV2_ProbNNp;
    Double_t        piplus_MC12TuneV2_ProbNNghost;
    Double_t        piplus_MC12TuneV3_ProbNNe;
    Double_t        piplus_MC12TuneV3_ProbNNmu;
    Double_t        piplus_MC12TuneV3_ProbNNpi;
    Double_t        piplus_MC12TuneV3_ProbNNk;
    Double_t        piplus_MC12TuneV3_ProbNNp;
    Double_t        piplus_MC12TuneV3_ProbNNghost;
    Double_t        piplus_MC12TuneV4_ProbNNe;
    Double_t        piplus_MC12TuneV4_ProbNNmu;
    Double_t        piplus_MC12TuneV4_ProbNNpi;
    Double_t        piplus_MC12TuneV4_ProbNNk;
    Double_t        piplus_MC12TuneV4_ProbNNp;
    Double_t        piplus_MC12TuneV4_ProbNNghost;
    Double_t        piplus_MC15TuneV1_ProbNNe;
    Double_t        piplus_MC15TuneV1_ProbNNmu;
    Double_t        piplus_MC15TuneV1_ProbNNpi;
    Double_t        piplus_MC15TuneV1_ProbNNk;
    Double_t        piplus_MC15TuneV1_ProbNNp;
    Double_t        piplus_MC15TuneV1_ProbNNghost;
    Double_t        piplus_OWNPV_X;
    Double_t        piplus_OWNPV_Y;
    Double_t        piplus_OWNPV_Z;
    Double_t        piplus_OWNPV_XERR;
    Double_t        piplus_OWNPV_YERR;
    Double_t        piplus_OWNPV_ZERR;
    Double_t        piplus_OWNPV_CHI2;
    Int_t           piplus_OWNPV_NDOF;
    Float_t         piplus_OWNPV_COV_[3][3];
    Double_t        piplus_IP_OWNPV;
    Double_t        piplus_IPCHI2_OWNPV;
    Double_t        piplus_ORIVX_X;
    Double_t        piplus_ORIVX_Y;
    Double_t        piplus_ORIVX_Z;
    Double_t        piplus_ORIVX_XERR;
    Double_t        piplus_ORIVX_YERR;
    Double_t        piplus_ORIVX_ZERR;
    Double_t        piplus_ORIVX_CHI2;
    Int_t           piplus_ORIVX_NDOF;
    Float_t         piplus_ORIVX_COV_[3][3];
    Double_t        piplus_P;
    Double_t        piplus_PT;
    Double_t        piplus_PE;
    Double_t        piplus_PX;
    Double_t        piplus_PY;
    Double_t        piplus_PZ;
    Double_t        piplus_M;
    Int_t           piplus_ID;
    Double_t        piplus_PIDe;
    Double_t        piplus_PIDmu;
    Double_t        piplus_PIDK;
    Double_t        piplus_PIDp;
    Double_t        piplus_ProbNNe;
    Double_t        piplus_ProbNNk;
    Double_t        piplus_ProbNNp;
    Double_t        piplus_ProbNNpi;
    Double_t        piplus_ProbNNmu;
    Double_t        piplus_ProbNNghost;
    Bool_t          piplus_hasMuon;
    Bool_t          piplus_isMuon;
    Bool_t          piplus_hasRich;
    Bool_t          piplus_hasCalo;
    Int_t           piplus_TRACK_Type;
    Int_t           piplus_TRACK_Key;
    Double_t        piplus_TRACK_CHI2NDOF;
    Double_t        piplus_TRACK_PCHI2;
    Double_t        piplus_TRACK_MatchCHI2;
    Double_t        piplus_TRACK_GhostProb;
    Double_t        piplus_TRACK_CloneDist;
    Double_t        piplus_TRACK_Likelihood;
    Double_t        piminus_LV01;
    Double_t        piminus_eta;
    Double_t        piminus_phi;
    Double_t        piminus_MC12TuneV2_ProbNNe;
    Double_t        piminus_MC12TuneV2_ProbNNmu;
    Double_t        piminus_MC12TuneV2_ProbNNpi;
    Double_t        piminus_MC12TuneV2_ProbNNk;
    Double_t        piminus_MC12TuneV2_ProbNNp;
    Double_t        piminus_MC12TuneV2_ProbNNghost;
    Double_t        piminus_MC12TuneV3_ProbNNe;
    Double_t        piminus_MC12TuneV3_ProbNNmu;
    Double_t        piminus_MC12TuneV3_ProbNNpi;
    Double_t        piminus_MC12TuneV3_ProbNNk;
    Double_t        piminus_MC12TuneV3_ProbNNp;
    Double_t        piminus_MC12TuneV3_ProbNNghost;
    Double_t        piminus_MC12TuneV4_ProbNNe;
    Double_t        piminus_MC12TuneV4_ProbNNmu;
    Double_t        piminus_MC12TuneV4_ProbNNpi;
    Double_t        piminus_MC12TuneV4_ProbNNk;
    Double_t        piminus_MC12TuneV4_ProbNNp;
    Double_t        piminus_MC12TuneV4_ProbNNghost;
    Double_t        piminus_MC15TuneV1_ProbNNe;
    Double_t        piminus_MC15TuneV1_ProbNNmu;
    Double_t        piminus_MC15TuneV1_ProbNNpi;
    Double_t        piminus_MC15TuneV1_ProbNNk;
    Double_t        piminus_MC15TuneV1_ProbNNp;
    Double_t        piminus_MC15TuneV1_ProbNNghost;
    Double_t        piminus_OWNPV_X;
    Double_t        piminus_OWNPV_Y;
    Double_t        piminus_OWNPV_Z;
    Double_t        piminus_OWNPV_XERR;
    Double_t        piminus_OWNPV_YERR;
    Double_t        piminus_OWNPV_ZERR;
    Double_t        piminus_OWNPV_CHI2;
    Int_t           piminus_OWNPV_NDOF;
    Float_t         piminus_OWNPV_COV_[3][3];
    Double_t        piminus_IP_OWNPV;
    Double_t        piminus_IPCHI2_OWNPV;
    Double_t        piminus_ORIVX_X;
    Double_t        piminus_ORIVX_Y;
    Double_t        piminus_ORIVX_Z;
    Double_t        piminus_ORIVX_XERR;
    Double_t        piminus_ORIVX_YERR;
    Double_t        piminus_ORIVX_ZERR;
    Double_t        piminus_ORIVX_CHI2;
    Int_t           piminus_ORIVX_NDOF;
    Float_t         piminus_ORIVX_COV_[3][3];
    Double_t        piminus_P;
    Double_t        piminus_PT;
    Double_t        piminus_PE;
    Double_t        piminus_PX;
    Double_t        piminus_PY;
    Double_t        piminus_PZ;
    Double_t        piminus_M;
    Int_t           piminus_ID;
    Double_t        piminus_PIDe;
    Double_t        piminus_PIDmu;
    Double_t        piminus_PIDK;
    Double_t        piminus_PIDp;
    Double_t        piminus_ProbNNe;
    Double_t        piminus_ProbNNk;
    Double_t        piminus_ProbNNp;
    Double_t        piminus_ProbNNpi;
    Double_t        piminus_ProbNNmu;
    Double_t        piminus_ProbNNghost;
    Bool_t          piminus_hasMuon;
    Bool_t          piminus_isMuon;
    Bool_t          piminus_hasRich;
    Bool_t          piminus_hasCalo;
    Int_t           piminus_TRACK_Type;
    Int_t           piminus_TRACK_Key;
    Double_t        piminus_TRACK_CHI2NDOF;
    Double_t        piminus_TRACK_PCHI2;
    Double_t        piminus_TRACK_MatchCHI2;
    Double_t        piminus_TRACK_GhostProb;
    Double_t        piminus_TRACK_CloneDist;
    Double_t        piminus_TRACK_Likelihood;
    UInt_t          nCandidate;
    ULong64_t       totCandidates;
    ULong64_t       EventInSequence;
    Int_t           nPV;
    Float_t         PVX[100];   //[nPV]
    Float_t         PVY[100];   //[nPV]
    Float_t         PVZ[100];   //[nPV]
    Float_t         PVXERR[100];   //[nPV]
    Float_t         PVYERR[100];   //[nPV]
    Float_t         PVZERR[100];   //[nPV]
    Float_t         PVCHI2[100];   //[nPV]
    Float_t         PVNDOF[100];   //[nPV]
    Float_t         PVNTRACKS[100];   //[nPV]
    Int_t           nPVs;
    Int_t           nTracks;
    Int_t           nLongTracks;
    Int_t           nDownstreamTracks;
    Int_t           nUpstreamTracks;
    Int_t           nVeloTracks;
    Int_t           nTTracks;
    Int_t           nBackTracks;
    Int_t           nRich1Hits;
    Int_t           nRich2Hits;
    Int_t           nVeloClusters;
    Int_t           nITClusters;
    Int_t           nTTClusters;
    Int_t           nOTClusters;
    Int_t           nSPDHits;
    Int_t           nMuonCoordsS0;
    Int_t           nMuonCoordsS1;
    Int_t           nMuonCoordsS2;
    Int_t           nMuonCoordsS3;
    Int_t           nMuonCoordsS4;
    Int_t           nMuonTracks;
    Int_t           L0Global;
    UInt_t          Hlt1Global;
    UInt_t          Hlt2Global;
    
    // List of branches
    TBranch        *b_KS0_LV01;   //!
    TBranch        *b_KS0_eta;   //!
    TBranch        *b_KS0_phi;   //!
    TBranch        *b_KS0_ENDVERTEX_X;   //!
    TBranch        *b_KS0_ENDVERTEX_Y;   //!
    TBranch        *b_KS0_ENDVERTEX_Z;   //!
    TBranch        *b_KS0_ENDVERTEX_XERR;   //!
    TBranch        *b_KS0_ENDVERTEX_YERR;   //!
    TBranch        *b_KS0_ENDVERTEX_ZERR;   //!
    TBranch        *b_KS0_ENDVERTEX_CHI2;   //!
    TBranch        *b_KS0_ENDVERTEX_NDOF;   //!
    TBranch        *b_KS0_ENDVERTEX_COV_;   //!
    TBranch        *b_KS0_OWNPV_X;   //!
    TBranch        *b_KS0_OWNPV_Y;   //!
    TBranch        *b_KS0_OWNPV_Z;   //!
    TBranch        *b_KS0_OWNPV_XERR;   //!
    TBranch        *b_KS0_OWNPV_YERR;   //!
    TBranch        *b_KS0_OWNPV_ZERR;   //!
    TBranch        *b_KS0_OWNPV_CHI2;   //!
    TBranch        *b_KS0_OWNPV_NDOF;   //!
    TBranch        *b_KS0_OWNPV_COV_;   //!
    TBranch        *b_KS0_IP_OWNPV;   //!
    TBranch        *b_KS0_IPCHI2_OWNPV;   //!
    TBranch        *b_KS0_P;   //!
    TBranch        *b_KS0_PT;   //!
    TBranch        *b_KS0_PE;   //!
    TBranch        *b_KS0_PX;   //!
    TBranch        *b_KS0_PY;   //!
    TBranch        *b_KS0_PZ;   //!
    TBranch        *b_KS0_MM;   //!
    TBranch        *b_KS0_MMERR;   //!
    TBranch        *b_KS0_M;   //!
    TBranch        *b_KS0_ID;   //!
    TBranch        *b_piplus_LV01;   //!
    TBranch        *b_piplus_eta;   //!
    TBranch        *b_piplus_phi;   //!
    TBranch        *b_piplus_MC12TuneV2_ProbNNe;   //!
    TBranch        *b_piplus_MC12TuneV2_ProbNNmu;   //!
    TBranch        *b_piplus_MC12TuneV2_ProbNNpi;   //!
    TBranch        *b_piplus_MC12TuneV2_ProbNNk;   //!
    TBranch        *b_piplus_MC12TuneV2_ProbNNp;   //!
    TBranch        *b_piplus_MC12TuneV2_ProbNNghost;   //!
    TBranch        *b_piplus_MC12TuneV3_ProbNNe;   //!
    TBranch        *b_piplus_MC12TuneV3_ProbNNmu;   //!
    TBranch        *b_piplus_MC12TuneV3_ProbNNpi;   //!
    TBranch        *b_piplus_MC12TuneV3_ProbNNk;   //!
    TBranch        *b_piplus_MC12TuneV3_ProbNNp;   //!
    TBranch        *b_piplus_MC12TuneV3_ProbNNghost;   //!
    TBranch        *b_piplus_MC12TuneV4_ProbNNe;   //!
    TBranch        *b_piplus_MC12TuneV4_ProbNNmu;   //!
    TBranch        *b_piplus_MC12TuneV4_ProbNNpi;   //!
    TBranch        *b_piplus_MC12TuneV4_ProbNNk;   //!
    TBranch        *b_piplus_MC12TuneV4_ProbNNp;   //!
    TBranch        *b_piplus_MC12TuneV4_ProbNNghost;   //!
    TBranch        *b_piplus_MC15TuneV1_ProbNNe;   //!
    TBranch        *b_piplus_MC15TuneV1_ProbNNmu;   //!
    TBranch        *b_piplus_MC15TuneV1_ProbNNpi;   //!
    TBranch        *b_piplus_MC15TuneV1_ProbNNk;   //!
    TBranch        *b_piplus_MC15TuneV1_ProbNNp;   //!
    TBranch        *b_piplus_MC15TuneV1_ProbNNghost;   //!
    TBranch        *b_piplus_OWNPV_X;   //!
    TBranch        *b_piplus_OWNPV_Y;   //!
    TBranch        *b_piplus_OWNPV_Z;   //!
    TBranch        *b_piplus_OWNPV_XERR;   //!
    TBranch        *b_piplus_OWNPV_YERR;   //!
    TBranch        *b_piplus_OWNPV_ZERR;   //!
    TBranch        *b_piplus_OWNPV_CHI2;   //!
    TBranch        *b_piplus_OWNPV_NDOF;   //!
    TBranch        *b_piplus_OWNPV_COV_;   //!
    TBranch        *b_piplus_IP_OWNPV;   //!
    TBranch        *b_piplus_IPCHI2_OWNPV;   //!
    TBranch        *b_piplus_ORIVX_X;   //!
    TBranch        *b_piplus_ORIVX_Y;   //!
    TBranch        *b_piplus_ORIVX_Z;   //!
    TBranch        *b_piplus_ORIVX_XERR;   //!
    TBranch        *b_piplus_ORIVX_YERR;   //!
    TBranch        *b_piplus_ORIVX_ZERR;   //!
    TBranch        *b_piplus_ORIVX_CHI2;   //!
    TBranch        *b_piplus_ORIVX_NDOF;   //!
    TBranch        *b_piplus_ORIVX_COV_;   //!
    TBranch        *b_piplus_P;   //!
    TBranch        *b_piplus_PT;   //!
    TBranch        *b_piplus_PE;   //!
    TBranch        *b_piplus_PX;   //!
    TBranch        *b_piplus_PY;   //!
    TBranch        *b_piplus_PZ;   //!
    TBranch        *b_piplus_M;   //!
    TBranch        *b_piplus_ID;   //!
    TBranch        *b_piplus_PIDe;   //!
    TBranch        *b_piplus_PIDmu;   //!
    TBranch        *b_piplus_PIDK;   //!
    TBranch        *b_piplus_PIDp;   //!
    TBranch        *b_piplus_ProbNNe;   //!
    TBranch        *b_piplus_ProbNNk;   //!
    TBranch        *b_piplus_ProbNNp;   //!
    TBranch        *b_piplus_ProbNNpi;   //!
    TBranch        *b_piplus_ProbNNmu;   //!
    TBranch        *b_piplus_ProbNNghost;   //!
    TBranch        *b_piplus_hasMuon;   //!
    TBranch        *b_piplus_isMuon;   //!
    TBranch        *b_piplus_hasRich;   //!
    TBranch        *b_piplus_hasCalo;   //!
    TBranch        *b_piplus_TRACK_Type;   //!
    TBranch        *b_piplus_TRACK_Key;   //!
    TBranch        *b_piplus_TRACK_CHI2NDOF;   //!
    TBranch        *b_piplus_TRACK_PCHI2;   //!
    TBranch        *b_piplus_TRACK_MatchCHI2;   //!
    TBranch        *b_piplus_TRACK_GhostProb;   //!
    TBranch        *b_piplus_TRACK_CloneDist;   //!
    TBranch        *b_piplus_TRACK_Likelihood;   //!
    TBranch        *b_piminus_LV01;   //!
    TBranch        *b_piminus_eta;   //!
    TBranch        *b_piminus_phi;   //!
    TBranch        *b_piminus_MC12TuneV2_ProbNNe;   //!
    TBranch        *b_piminus_MC12TuneV2_ProbNNmu;   //!
    TBranch        *b_piminus_MC12TuneV2_ProbNNpi;   //!
    TBranch        *b_piminus_MC12TuneV2_ProbNNk;   //!
    TBranch        *b_piminus_MC12TuneV2_ProbNNp;   //!
    TBranch        *b_piminus_MC12TuneV2_ProbNNghost;   //!
    TBranch        *b_piminus_MC12TuneV3_ProbNNe;   //!
    TBranch        *b_piminus_MC12TuneV3_ProbNNmu;   //!
    TBranch        *b_piminus_MC12TuneV3_ProbNNpi;   //!
    TBranch        *b_piminus_MC12TuneV3_ProbNNk;   //!
    TBranch        *b_piminus_MC12TuneV3_ProbNNp;   //!
    TBranch        *b_piminus_MC12TuneV3_ProbNNghost;   //!
    TBranch        *b_piminus_MC12TuneV4_ProbNNe;   //!
    TBranch        *b_piminus_MC12TuneV4_ProbNNmu;   //!
    TBranch        *b_piminus_MC12TuneV4_ProbNNpi;   //!
    TBranch        *b_piminus_MC12TuneV4_ProbNNk;   //!
    TBranch        *b_piminus_MC12TuneV4_ProbNNp;   //!
    TBranch        *b_piminus_MC12TuneV4_ProbNNghost;   //!
    TBranch        *b_piminus_MC15TuneV1_ProbNNe;   //!
    TBranch        *b_piminus_MC15TuneV1_ProbNNmu;   //!
    TBranch        *b_piminus_MC15TuneV1_ProbNNpi;   //!
    TBranch        *b_piminus_MC15TuneV1_ProbNNk;   //!
    TBranch        *b_piminus_MC15TuneV1_ProbNNp;   //!
    TBranch        *b_piminus_MC15TuneV1_ProbNNghost;   //!
    TBranch        *b_piminus_OWNPV_X;   //!
    TBranch        *b_piminus_OWNPV_Y;   //!
    TBranch        *b_piminus_OWNPV_Z;   //!
    TBranch        *b_piminus_OWNPV_XERR;   //!
    TBranch        *b_piminus_OWNPV_YERR;   //!
    TBranch        *b_piminus_OWNPV_ZERR;   //!
    TBranch        *b_piminus_OWNPV_CHI2;   //!
    TBranch        *b_piminus_OWNPV_NDOF;   //!
    TBranch        *b_piminus_OWNPV_COV_;   //!
    TBranch        *b_piminus_IP_OWNPV;   //!
    TBranch        *b_piminus_IPCHI2_OWNPV;   //!
    TBranch        *b_piminus_ORIVX_X;   //!
    TBranch        *b_piminus_ORIVX_Y;   //!
    TBranch        *b_piminus_ORIVX_Z;   //!
    TBranch        *b_piminus_ORIVX_XERR;   //!
    TBranch        *b_piminus_ORIVX_YERR;   //!
    TBranch        *b_piminus_ORIVX_ZERR;   //!
    TBranch        *b_piminus_ORIVX_CHI2;   //!
    TBranch        *b_piminus_ORIVX_NDOF;   //!
    TBranch        *b_piminus_ORIVX_COV_;   //!
    TBranch        *b_piminus_P;   //!
    TBranch        *b_piminus_PT;   //!
    TBranch        *b_piminus_PE;   //!
    TBranch        *b_piminus_PX;   //!
    TBranch        *b_piminus_PY;   //!
    TBranch        *b_piminus_PZ;   //!
    TBranch        *b_piminus_M;   //!
    TBranch        *b_piminus_ID;   //!
    TBranch        *b_piminus_PIDe;   //!
    TBranch        *b_piminus_PIDmu;   //!
    TBranch        *b_piminus_PIDK;   //!
    TBranch        *b_piminus_PIDp;   //!
    TBranch        *b_piminus_ProbNNe;   //!
    TBranch        *b_piminus_ProbNNk;   //!
    TBranch        *b_piminus_ProbNNp;   //!
    TBranch        *b_piminus_ProbNNpi;   //!
    TBranch        *b_piminus_ProbNNmu;   //!
    TBranch        *b_piminus_ProbNNghost;   //!
    TBranch        *b_piminus_hasMuon;   //!
    TBranch        *b_piminus_isMuon;   //!
    TBranch        *b_piminus_hasRich;   //!
    TBranch        *b_piminus_hasCalo;   //!
    TBranch        *b_piminus_TRACK_Type;   //!
    TBranch        *b_piminus_TRACK_Key;   //!
    TBranch        *b_piminus_TRACK_CHI2NDOF;   //!
    TBranch        *b_piminus_TRACK_PCHI2;   //!
    TBranch        *b_piminus_TRACK_MatchCHI2;   //!
    TBranch        *b_piminus_TRACK_GhostProb;   //!
    TBranch        *b_piminus_TRACK_CloneDist;   //!
    TBranch        *b_piminus_TRACK_Likelihood;   //!
    TBranch        *b_nCandidate;   //!
    TBranch        *b_totCandidates;   //!
    TBranch        *b_EventInSequence;   //!
    TBranch        *b_nPV;   //!
    TBranch        *b_PVX;   //!
    TBranch        *b_PVY;   //!
    TBranch        *b_PVZ;   //!
    TBranch        *b_PVXERR;   //!
    TBranch        *b_PVYERR;   //!
    TBranch        *b_PVZERR;   //!
    TBranch        *b_PVCHI2;   //!
    TBranch        *b_PVNDOF;   //!
    TBranch        *b_PVNTRACKS;   //!
    TBranch        *b_nPVs;   //!
    TBranch        *b_nTracks;   //!
    TBranch        *b_nLongTracks;   //!
    TBranch        *b_nDownstreamTracks;   //!
    TBranch        *b_nUpstreamTracks;   //!
    TBranch        *b_nVeloTracks;   //!
    TBranch        *b_nTTracks;   //!
    TBranch        *b_nBackTracks;   //!
    TBranch        *b_nRich1Hits;   //!
    TBranch        *b_nRich2Hits;   //!
    TBranch        *b_nVeloClusters;   //!
    TBranch        *b_nITClusters;   //!
    TBranch        *b_nTTClusters;   //!
    TBranch        *b_nOTClusters;   //!
    TBranch        *b_nSPDHits;   //!
    TBranch        *b_nMuonCoordsS0;   //!
    TBranch        *b_nMuonCoordsS1;   //!
    TBranch        *b_nMuonCoordsS2;   //!
    TBranch        *b_nMuonCoordsS3;   //!
    TBranch        *b_nMuonCoordsS4;   //!
    TBranch        *b_nMuonTracks;   //!
    TBranch        *b_L0Global;   //!
    TBranch        *b_Hlt1Global;   //!
    TBranch        *b_Hlt2Global;   //!

};

void InputData_K::setBranches(){
    tree->SetBranchAddress("KS0_LV01", &KS0_LV01, &b_KS0_LV01);
    tree->SetBranchAddress("KS0_eta", &KS0_eta, &b_KS0_eta);
    tree->SetBranchAddress("KS0_phi", &KS0_phi, &b_KS0_phi);
    tree->SetBranchAddress("KS0_ENDVERTEX_X", &KS0_ENDVERTEX_X, &b_KS0_ENDVERTEX_X);
    tree->SetBranchAddress("KS0_ENDVERTEX_Y", &KS0_ENDVERTEX_Y, &b_KS0_ENDVERTEX_Y);
    tree->SetBranchAddress("KS0_ENDVERTEX_Z", &KS0_ENDVERTEX_Z, &b_KS0_ENDVERTEX_Z);
    tree->SetBranchAddress("KS0_ENDVERTEX_XERR", &KS0_ENDVERTEX_XERR, &b_KS0_ENDVERTEX_XERR);
    tree->SetBranchAddress("KS0_ENDVERTEX_YERR", &KS0_ENDVERTEX_YERR, &b_KS0_ENDVERTEX_YERR);
    tree->SetBranchAddress("KS0_ENDVERTEX_ZERR", &KS0_ENDVERTEX_ZERR, &b_KS0_ENDVERTEX_ZERR);
    tree->SetBranchAddress("KS0_ENDVERTEX_CHI2", &KS0_ENDVERTEX_CHI2, &b_KS0_ENDVERTEX_CHI2);
    tree->SetBranchAddress("KS0_ENDVERTEX_NDOF", &KS0_ENDVERTEX_NDOF, &b_KS0_ENDVERTEX_NDOF);
    tree->SetBranchAddress("KS0_ENDVERTEX_COV_", KS0_ENDVERTEX_COV_, &b_KS0_ENDVERTEX_COV_);
    tree->SetBranchAddress("KS0_OWNPV_X", &KS0_OWNPV_X, &b_KS0_OWNPV_X);
    tree->SetBranchAddress("KS0_OWNPV_Y", &KS0_OWNPV_Y, &b_KS0_OWNPV_Y);
    tree->SetBranchAddress("KS0_OWNPV_Z", &KS0_OWNPV_Z, &b_KS0_OWNPV_Z);
    tree->SetBranchAddress("KS0_OWNPV_XERR", &KS0_OWNPV_XERR, &b_KS0_OWNPV_XERR);
    tree->SetBranchAddress("KS0_OWNPV_YERR", &KS0_OWNPV_YERR, &b_KS0_OWNPV_YERR);
    tree->SetBranchAddress("KS0_OWNPV_ZERR", &KS0_OWNPV_ZERR, &b_KS0_OWNPV_ZERR);
    tree->SetBranchAddress("KS0_OWNPV_CHI2", &KS0_OWNPV_CHI2, &b_KS0_OWNPV_CHI2);
    tree->SetBranchAddress("KS0_OWNPV_NDOF", &KS0_OWNPV_NDOF, &b_KS0_OWNPV_NDOF);
    tree->SetBranchAddress("KS0_OWNPV_COV_", KS0_OWNPV_COV_, &b_KS0_OWNPV_COV_);
    tree->SetBranchAddress("KS0_IP_OWNPV", &KS0_IP_OWNPV, &b_KS0_IP_OWNPV);
    tree->SetBranchAddress("KS0_IPCHI2_OWNPV", &KS0_IPCHI2_OWNPV, &b_KS0_IPCHI2_OWNPV);
    tree->SetBranchAddress("KS0_P", &KS0_P, &b_KS0_P);
    tree->SetBranchAddress("KS0_PT", &KS0_PT, &b_KS0_PT);
    tree->SetBranchAddress("KS0_PE", &KS0_PE, &b_KS0_PE);
    tree->SetBranchAddress("KS0_PX", &KS0_PX, &b_KS0_PX);
    tree->SetBranchAddress("KS0_PY", &KS0_PY, &b_KS0_PY);
    tree->SetBranchAddress("KS0_PZ", &KS0_PZ, &b_KS0_PZ);
    tree->SetBranchAddress("KS0_MM", &KS0_MM, &b_KS0_MM);
    tree->SetBranchAddress("KS0_MMERR", &KS0_MMERR, &b_KS0_MMERR);
    tree->SetBranchAddress("KS0_M", &KS0_M, &b_KS0_M);
    tree->SetBranchAddress("KS0_ID", &KS0_ID, &b_KS0_ID);
    tree->SetBranchAddress("piplus_LV01", &piplus_LV01, &b_piplus_LV01);
    tree->SetBranchAddress("piplus_eta", &piplus_eta, &b_piplus_eta);
    tree->SetBranchAddress("piplus_phi", &piplus_phi, &b_piplus_phi);
    tree->SetBranchAddress("piplus_MC12TuneV2_ProbNNe", &piplus_MC12TuneV2_ProbNNe, &b_piplus_MC12TuneV2_ProbNNe);
    tree->SetBranchAddress("piplus_MC12TuneV2_ProbNNmu", &piplus_MC12TuneV2_ProbNNmu, &b_piplus_MC12TuneV2_ProbNNmu);
    tree->SetBranchAddress("piplus_MC12TuneV2_ProbNNpi", &piplus_MC12TuneV2_ProbNNpi, &b_piplus_MC12TuneV2_ProbNNpi);
    tree->SetBranchAddress("piplus_MC12TuneV2_ProbNNk", &piplus_MC12TuneV2_ProbNNk, &b_piplus_MC12TuneV2_ProbNNk);
    tree->SetBranchAddress("piplus_MC12TuneV2_ProbNNp", &piplus_MC12TuneV2_ProbNNp, &b_piplus_MC12TuneV2_ProbNNp);
    tree->SetBranchAddress("piplus_MC12TuneV2_ProbNNghost", &piplus_MC12TuneV2_ProbNNghost, &b_piplus_MC12TuneV2_ProbNNghost);
    tree->SetBranchAddress("piplus_MC12TuneV3_ProbNNe", &piplus_MC12TuneV3_ProbNNe, &b_piplus_MC12TuneV3_ProbNNe);
    tree->SetBranchAddress("piplus_MC12TuneV3_ProbNNmu", &piplus_MC12TuneV3_ProbNNmu, &b_piplus_MC12TuneV3_ProbNNmu);
    tree->SetBranchAddress("piplus_MC12TuneV3_ProbNNpi", &piplus_MC12TuneV3_ProbNNpi, &b_piplus_MC12TuneV3_ProbNNpi);
    tree->SetBranchAddress("piplus_MC12TuneV3_ProbNNk", &piplus_MC12TuneV3_ProbNNk, &b_piplus_MC12TuneV3_ProbNNk);
    tree->SetBranchAddress("piplus_MC12TuneV3_ProbNNp", &piplus_MC12TuneV3_ProbNNp, &b_piplus_MC12TuneV3_ProbNNp);
    tree->SetBranchAddress("piplus_MC12TuneV3_ProbNNghost", &piplus_MC12TuneV3_ProbNNghost, &b_piplus_MC12TuneV3_ProbNNghost);
    tree->SetBranchAddress("piplus_MC12TuneV4_ProbNNe", &piplus_MC12TuneV4_ProbNNe, &b_piplus_MC12TuneV4_ProbNNe);
    tree->SetBranchAddress("piplus_MC12TuneV4_ProbNNmu", &piplus_MC12TuneV4_ProbNNmu, &b_piplus_MC12TuneV4_ProbNNmu);
    tree->SetBranchAddress("piplus_MC12TuneV4_ProbNNpi", &piplus_MC12TuneV4_ProbNNpi, &b_piplus_MC12TuneV4_ProbNNpi);
    tree->SetBranchAddress("piplus_MC12TuneV4_ProbNNk", &piplus_MC12TuneV4_ProbNNk, &b_piplus_MC12TuneV4_ProbNNk);
    tree->SetBranchAddress("piplus_MC12TuneV4_ProbNNp", &piplus_MC12TuneV4_ProbNNp, &b_piplus_MC12TuneV4_ProbNNp);
    tree->SetBranchAddress("piplus_MC12TuneV4_ProbNNghost", &piplus_MC12TuneV4_ProbNNghost, &b_piplus_MC12TuneV4_ProbNNghost);
    tree->SetBranchAddress("piplus_MC15TuneV1_ProbNNe", &piplus_MC15TuneV1_ProbNNe, &b_piplus_MC15TuneV1_ProbNNe);
    tree->SetBranchAddress("piplus_MC15TuneV1_ProbNNmu", &piplus_MC15TuneV1_ProbNNmu, &b_piplus_MC15TuneV1_ProbNNmu);
    tree->SetBranchAddress("piplus_MC15TuneV1_ProbNNpi", &piplus_MC15TuneV1_ProbNNpi, &b_piplus_MC15TuneV1_ProbNNpi);
    tree->SetBranchAddress("piplus_MC15TuneV1_ProbNNk", &piplus_MC15TuneV1_ProbNNk, &b_piplus_MC15TuneV1_ProbNNk);
    tree->SetBranchAddress("piplus_MC15TuneV1_ProbNNp", &piplus_MC15TuneV1_ProbNNp, &b_piplus_MC15TuneV1_ProbNNp);
    tree->SetBranchAddress("piplus_MC15TuneV1_ProbNNghost", &piplus_MC15TuneV1_ProbNNghost, &b_piplus_MC15TuneV1_ProbNNghost);
    tree->SetBranchAddress("piplus_OWNPV_X", &piplus_OWNPV_X, &b_piplus_OWNPV_X);
    tree->SetBranchAddress("piplus_OWNPV_Y", &piplus_OWNPV_Y, &b_piplus_OWNPV_Y);
    tree->SetBranchAddress("piplus_OWNPV_Z", &piplus_OWNPV_Z, &b_piplus_OWNPV_Z);
    tree->SetBranchAddress("piplus_OWNPV_XERR", &piplus_OWNPV_XERR, &b_piplus_OWNPV_XERR);
    tree->SetBranchAddress("piplus_OWNPV_YERR", &piplus_OWNPV_YERR, &b_piplus_OWNPV_YERR);
    tree->SetBranchAddress("piplus_OWNPV_ZERR", &piplus_OWNPV_ZERR, &b_piplus_OWNPV_ZERR);
    tree->SetBranchAddress("piplus_OWNPV_CHI2", &piplus_OWNPV_CHI2, &b_piplus_OWNPV_CHI2);
    tree->SetBranchAddress("piplus_OWNPV_NDOF", &piplus_OWNPV_NDOF, &b_piplus_OWNPV_NDOF);
    tree->SetBranchAddress("piplus_OWNPV_COV_", piplus_OWNPV_COV_, &b_piplus_OWNPV_COV_);
    tree->SetBranchAddress("piplus_IP_OWNPV", &piplus_IP_OWNPV, &b_piplus_IP_OWNPV);
    tree->SetBranchAddress("piplus_IPCHI2_OWNPV", &piplus_IPCHI2_OWNPV, &b_piplus_IPCHI2_OWNPV);
    tree->SetBranchAddress("piplus_ORIVX_X", &piplus_ORIVX_X, &b_piplus_ORIVX_X);
    tree->SetBranchAddress("piplus_ORIVX_Y", &piplus_ORIVX_Y, &b_piplus_ORIVX_Y);
    tree->SetBranchAddress("piplus_ORIVX_Z", &piplus_ORIVX_Z, &b_piplus_ORIVX_Z);
    tree->SetBranchAddress("piplus_ORIVX_XERR", &piplus_ORIVX_XERR, &b_piplus_ORIVX_XERR);
    tree->SetBranchAddress("piplus_ORIVX_YERR", &piplus_ORIVX_YERR, &b_piplus_ORIVX_YERR);
    tree->SetBranchAddress("piplus_ORIVX_ZERR", &piplus_ORIVX_ZERR, &b_piplus_ORIVX_ZERR);
    tree->SetBranchAddress("piplus_ORIVX_CHI2", &piplus_ORIVX_CHI2, &b_piplus_ORIVX_CHI2);
    tree->SetBranchAddress("piplus_ORIVX_NDOF", &piplus_ORIVX_NDOF, &b_piplus_ORIVX_NDOF);
    tree->SetBranchAddress("piplus_ORIVX_COV_", piplus_ORIVX_COV_, &b_piplus_ORIVX_COV_);
    tree->SetBranchAddress("piplus_P", &piplus_P, &b_piplus_P);
    tree->SetBranchAddress("piplus_PT", &piplus_PT, &b_piplus_PT);
    tree->SetBranchAddress("piplus_PE", &piplus_PE, &b_piplus_PE);
    tree->SetBranchAddress("piplus_PX", &piplus_PX, &b_piplus_PX);
    tree->SetBranchAddress("piplus_PY", &piplus_PY, &b_piplus_PY);
    tree->SetBranchAddress("piplus_PZ", &piplus_PZ, &b_piplus_PZ);
    tree->SetBranchAddress("piplus_M", &piplus_M, &b_piplus_M);
    tree->SetBranchAddress("piplus_ID", &piplus_ID, &b_piplus_ID);
    tree->SetBranchAddress("piplus_PIDe", &piplus_PIDe, &b_piplus_PIDe);
    tree->SetBranchAddress("piplus_PIDmu", &piplus_PIDmu, &b_piplus_PIDmu);
    tree->SetBranchAddress("piplus_PIDK", &piplus_PIDK, &b_piplus_PIDK);
    tree->SetBranchAddress("piplus_PIDp", &piplus_PIDp, &b_piplus_PIDp);
    tree->SetBranchAddress("piplus_ProbNNe", &piplus_ProbNNe, &b_piplus_ProbNNe);
    tree->SetBranchAddress("piplus_ProbNNk", &piplus_ProbNNk, &b_piplus_ProbNNk);
    tree->SetBranchAddress("piplus_ProbNNp", &piplus_ProbNNp, &b_piplus_ProbNNp);
    tree->SetBranchAddress("piplus_ProbNNpi", &piplus_ProbNNpi, &b_piplus_ProbNNpi);
    tree->SetBranchAddress("piplus_ProbNNmu", &piplus_ProbNNmu, &b_piplus_ProbNNmu);
    tree->SetBranchAddress("piplus_ProbNNghost", &piplus_ProbNNghost, &b_piplus_ProbNNghost);
    tree->SetBranchAddress("piplus_hasMuon", &piplus_hasMuon, &b_piplus_hasMuon);
    tree->SetBranchAddress("piplus_isMuon", &piplus_isMuon, &b_piplus_isMuon);
    tree->SetBranchAddress("piplus_hasRich", &piplus_hasRich, &b_piplus_hasRich);
    tree->SetBranchAddress("piplus_hasCalo", &piplus_hasCalo, &b_piplus_hasCalo);
    tree->SetBranchAddress("piplus_TRACK_Type", &piplus_TRACK_Type, &b_piplus_TRACK_Type);
    tree->SetBranchAddress("piplus_TRACK_Key", &piplus_TRACK_Key, &b_piplus_TRACK_Key);
    tree->SetBranchAddress("piplus_TRACK_CHI2NDOF", &piplus_TRACK_CHI2NDOF, &b_piplus_TRACK_CHI2NDOF);
    tree->SetBranchAddress("piplus_TRACK_PCHI2", &piplus_TRACK_PCHI2, &b_piplus_TRACK_PCHI2);
    tree->SetBranchAddress("piplus_TRACK_MatchCHI2", &piplus_TRACK_MatchCHI2, &b_piplus_TRACK_MatchCHI2);
    tree->SetBranchAddress("piplus_TRACK_GhostProb", &piplus_TRACK_GhostProb, &b_piplus_TRACK_GhostProb);
    tree->SetBranchAddress("piplus_TRACK_CloneDist", &piplus_TRACK_CloneDist, &b_piplus_TRACK_CloneDist);
    tree->SetBranchAddress("piplus_TRACK_Likelihood", &piplus_TRACK_Likelihood, &b_piplus_TRACK_Likelihood);
    tree->SetBranchAddress("piminus_LV01", &piminus_LV01, &b_piminus_LV01);
    tree->SetBranchAddress("piminus_eta", &piminus_eta, &b_piminus_eta);
    tree->SetBranchAddress("piminus_phi", &piminus_phi, &b_piminus_phi);
    tree->SetBranchAddress("piminus_MC12TuneV2_ProbNNe", &piminus_MC12TuneV2_ProbNNe, &b_piminus_MC12TuneV2_ProbNNe);
    tree->SetBranchAddress("piminus_MC12TuneV2_ProbNNmu", &piminus_MC12TuneV2_ProbNNmu, &b_piminus_MC12TuneV2_ProbNNmu);
    tree->SetBranchAddress("piminus_MC12TuneV2_ProbNNpi", &piminus_MC12TuneV2_ProbNNpi, &b_piminus_MC12TuneV2_ProbNNpi);
    tree->SetBranchAddress("piminus_MC12TuneV2_ProbNNk", &piminus_MC12TuneV2_ProbNNk, &b_piminus_MC12TuneV2_ProbNNk);
    tree->SetBranchAddress("piminus_MC12TuneV2_ProbNNp", &piminus_MC12TuneV2_ProbNNp, &b_piminus_MC12TuneV2_ProbNNp);
    tree->SetBranchAddress("piminus_MC12TuneV2_ProbNNghost", &piminus_MC12TuneV2_ProbNNghost, &b_piminus_MC12TuneV2_ProbNNghost);
    tree->SetBranchAddress("piminus_MC12TuneV3_ProbNNe", &piminus_MC12TuneV3_ProbNNe, &b_piminus_MC12TuneV3_ProbNNe);
    tree->SetBranchAddress("piminus_MC12TuneV3_ProbNNmu", &piminus_MC12TuneV3_ProbNNmu, &b_piminus_MC12TuneV3_ProbNNmu);
    tree->SetBranchAddress("piminus_MC12TuneV3_ProbNNpi", &piminus_MC12TuneV3_ProbNNpi, &b_piminus_MC12TuneV3_ProbNNpi);
    tree->SetBranchAddress("piminus_MC12TuneV3_ProbNNk", &piminus_MC12TuneV3_ProbNNk, &b_piminus_MC12TuneV3_ProbNNk);
    tree->SetBranchAddress("piminus_MC12TuneV3_ProbNNp", &piminus_MC12TuneV3_ProbNNp, &b_piminus_MC12TuneV3_ProbNNp);
    tree->SetBranchAddress("piminus_MC12TuneV3_ProbNNghost", &piminus_MC12TuneV3_ProbNNghost, &b_piminus_MC12TuneV3_ProbNNghost);
    tree->SetBranchAddress("piminus_MC12TuneV4_ProbNNe", &piminus_MC12TuneV4_ProbNNe, &b_piminus_MC12TuneV4_ProbNNe);
    tree->SetBranchAddress("piminus_MC12TuneV4_ProbNNmu", &piminus_MC12TuneV4_ProbNNmu, &b_piminus_MC12TuneV4_ProbNNmu);
    tree->SetBranchAddress("piminus_MC12TuneV4_ProbNNpi", &piminus_MC12TuneV4_ProbNNpi, &b_piminus_MC12TuneV4_ProbNNpi);
    tree->SetBranchAddress("piminus_MC12TuneV4_ProbNNk", &piminus_MC12TuneV4_ProbNNk, &b_piminus_MC12TuneV4_ProbNNk);
    tree->SetBranchAddress("piminus_MC12TuneV4_ProbNNp", &piminus_MC12TuneV4_ProbNNp, &b_piminus_MC12TuneV4_ProbNNp);
    tree->SetBranchAddress("piminus_MC12TuneV4_ProbNNghost", &piminus_MC12TuneV4_ProbNNghost, &b_piminus_MC12TuneV4_ProbNNghost);
    tree->SetBranchAddress("piminus_MC15TuneV1_ProbNNe", &piminus_MC15TuneV1_ProbNNe, &b_piminus_MC15TuneV1_ProbNNe);
    tree->SetBranchAddress("piminus_MC15TuneV1_ProbNNmu", &piminus_MC15TuneV1_ProbNNmu, &b_piminus_MC15TuneV1_ProbNNmu);
    tree->SetBranchAddress("piminus_MC15TuneV1_ProbNNpi", &piminus_MC15TuneV1_ProbNNpi, &b_piminus_MC15TuneV1_ProbNNpi);
    tree->SetBranchAddress("piminus_MC15TuneV1_ProbNNk", &piminus_MC15TuneV1_ProbNNk, &b_piminus_MC15TuneV1_ProbNNk);
    tree->SetBranchAddress("piminus_MC15TuneV1_ProbNNp", &piminus_MC15TuneV1_ProbNNp, &b_piminus_MC15TuneV1_ProbNNp);
    tree->SetBranchAddress("piminus_MC15TuneV1_ProbNNghost", &piminus_MC15TuneV1_ProbNNghost, &b_piminus_MC15TuneV1_ProbNNghost);
    tree->SetBranchAddress("piminus_OWNPV_X", &piminus_OWNPV_X, &b_piminus_OWNPV_X);
    tree->SetBranchAddress("piminus_OWNPV_Y", &piminus_OWNPV_Y, &b_piminus_OWNPV_Y);
    tree->SetBranchAddress("piminus_OWNPV_Z", &piminus_OWNPV_Z, &b_piminus_OWNPV_Z);
    tree->SetBranchAddress("piminus_OWNPV_XERR", &piminus_OWNPV_XERR, &b_piminus_OWNPV_XERR);
    tree->SetBranchAddress("piminus_OWNPV_YERR", &piminus_OWNPV_YERR, &b_piminus_OWNPV_YERR);
    tree->SetBranchAddress("piminus_OWNPV_ZERR", &piminus_OWNPV_ZERR, &b_piminus_OWNPV_ZERR);
    tree->SetBranchAddress("piminus_OWNPV_CHI2", &piminus_OWNPV_CHI2, &b_piminus_OWNPV_CHI2);
    tree->SetBranchAddress("piminus_OWNPV_NDOF", &piminus_OWNPV_NDOF, &b_piminus_OWNPV_NDOF);
    tree->SetBranchAddress("piminus_OWNPV_COV_", piminus_OWNPV_COV_, &b_piminus_OWNPV_COV_);
    tree->SetBranchAddress("piminus_IP_OWNPV", &piminus_IP_OWNPV, &b_piminus_IP_OWNPV);
    tree->SetBranchAddress("piminus_IPCHI2_OWNPV", &piminus_IPCHI2_OWNPV, &b_piminus_IPCHI2_OWNPV);
    tree->SetBranchAddress("piminus_ORIVX_X", &piminus_ORIVX_X, &b_piminus_ORIVX_X);
    tree->SetBranchAddress("piminus_ORIVX_Y", &piminus_ORIVX_Y, &b_piminus_ORIVX_Y);
    tree->SetBranchAddress("piminus_ORIVX_Z", &piminus_ORIVX_Z, &b_piminus_ORIVX_Z);
    tree->SetBranchAddress("piminus_ORIVX_XERR", &piminus_ORIVX_XERR, &b_piminus_ORIVX_XERR);
    tree->SetBranchAddress("piminus_ORIVX_YERR", &piminus_ORIVX_YERR, &b_piminus_ORIVX_YERR);
    tree->SetBranchAddress("piminus_ORIVX_ZERR", &piminus_ORIVX_ZERR, &b_piminus_ORIVX_ZERR);
    tree->SetBranchAddress("piminus_ORIVX_CHI2", &piminus_ORIVX_CHI2, &b_piminus_ORIVX_CHI2);
    tree->SetBranchAddress("piminus_ORIVX_NDOF", &piminus_ORIVX_NDOF, &b_piminus_ORIVX_NDOF);
    tree->SetBranchAddress("piminus_ORIVX_COV_", piminus_ORIVX_COV_, &b_piminus_ORIVX_COV_);
    tree->SetBranchAddress("piminus_P", &piminus_P, &b_piminus_P);
    tree->SetBranchAddress("piminus_PT", &piminus_PT, &b_piminus_PT);
    tree->SetBranchAddress("piminus_PE", &piminus_PE, &b_piminus_PE);
    tree->SetBranchAddress("piminus_PX", &piminus_PX, &b_piminus_PX);
    tree->SetBranchAddress("piminus_PY", &piminus_PY, &b_piminus_PY);
    tree->SetBranchAddress("piminus_PZ", &piminus_PZ, &b_piminus_PZ);
    tree->SetBranchAddress("piminus_M", &piminus_M, &b_piminus_M);
    tree->SetBranchAddress("piminus_ID", &piminus_ID, &b_piminus_ID);
    tree->SetBranchAddress("piminus_PIDe", &piminus_PIDe, &b_piminus_PIDe);
    tree->SetBranchAddress("piminus_PIDmu", &piminus_PIDmu, &b_piminus_PIDmu);
    tree->SetBranchAddress("piminus_PIDK", &piminus_PIDK, &b_piminus_PIDK);
    tree->SetBranchAddress("piminus_PIDp", &piminus_PIDp, &b_piminus_PIDp);
    tree->SetBranchAddress("piminus_ProbNNe", &piminus_ProbNNe, &b_piminus_ProbNNe);
    tree->SetBranchAddress("piminus_ProbNNk", &piminus_ProbNNk, &b_piminus_ProbNNk);
    tree->SetBranchAddress("piminus_ProbNNp", &piminus_ProbNNp, &b_piminus_ProbNNp);
    tree->SetBranchAddress("piminus_ProbNNpi", &piminus_ProbNNpi, &b_piminus_ProbNNpi);
    tree->SetBranchAddress("piminus_ProbNNmu", &piminus_ProbNNmu, &b_piminus_ProbNNmu);
    tree->SetBranchAddress("piminus_ProbNNghost", &piminus_ProbNNghost, &b_piminus_ProbNNghost);
    tree->SetBranchAddress("piminus_hasMuon", &piminus_hasMuon, &b_piminus_hasMuon);
    tree->SetBranchAddress("piminus_isMuon", &piminus_isMuon, &b_piminus_isMuon);
    tree->SetBranchAddress("piminus_hasRich", &piminus_hasRich, &b_piminus_hasRich);
    tree->SetBranchAddress("piminus_hasCalo", &piminus_hasCalo, &b_piminus_hasCalo);
    tree->SetBranchAddress("piminus_TRACK_Type", &piminus_TRACK_Type, &b_piminus_TRACK_Type);
    tree->SetBranchAddress("piminus_TRACK_Key", &piminus_TRACK_Key, &b_piminus_TRACK_Key);
    tree->SetBranchAddress("piminus_TRACK_CHI2NDOF", &piminus_TRACK_CHI2NDOF, &b_piminus_TRACK_CHI2NDOF);
    tree->SetBranchAddress("piminus_TRACK_PCHI2", &piminus_TRACK_PCHI2, &b_piminus_TRACK_PCHI2);
    tree->SetBranchAddress("piminus_TRACK_MatchCHI2", &piminus_TRACK_MatchCHI2, &b_piminus_TRACK_MatchCHI2);
    tree->SetBranchAddress("piminus_TRACK_GhostProb", &piminus_TRACK_GhostProb, &b_piminus_TRACK_GhostProb);
    tree->SetBranchAddress("piminus_TRACK_CloneDist", &piminus_TRACK_CloneDist, &b_piminus_TRACK_CloneDist);
    tree->SetBranchAddress("piminus_TRACK_Likelihood", &piminus_TRACK_Likelihood, &b_piminus_TRACK_Likelihood);
    tree->SetBranchAddress("nCandidate", &nCandidate, &b_nCandidate);
    tree->SetBranchAddress("totCandidates", &totCandidates, &b_totCandidates);
    tree->SetBranchAddress("EventInSequence", &EventInSequence, &b_EventInSequence);
    tree->SetBranchAddress("nPV", &nPV, &b_nPV);
    tree->SetBranchAddress("PVX", PVX, &b_PVX);
    tree->SetBranchAddress("PVY", PVY, &b_PVY);
    tree->SetBranchAddress("PVZ", PVZ, &b_PVZ);
    tree->SetBranchAddress("PVXERR", PVXERR, &b_PVXERR);
    tree->SetBranchAddress("PVYERR", PVYERR, &b_PVYERR);
    tree->SetBranchAddress("PVZERR", PVZERR, &b_PVZERR);
    tree->SetBranchAddress("PVCHI2", PVCHI2, &b_PVCHI2);
    tree->SetBranchAddress("PVNDOF", PVNDOF, &b_PVNDOF);
    tree->SetBranchAddress("PVNTRACKS", PVNTRACKS, &b_PVNTRACKS);
    tree->SetBranchAddress("nPVs", &nPVs, &b_nPVs);
    tree->SetBranchAddress("nTracks", &nTracks, &b_nTracks);
    tree->SetBranchAddress("nLongTracks", &nLongTracks, &b_nLongTracks);
    tree->SetBranchAddress("nDownstreamTracks", &nDownstreamTracks, &b_nDownstreamTracks);
    tree->SetBranchAddress("nUpstreamTracks", &nUpstreamTracks, &b_nUpstreamTracks);
    tree->SetBranchAddress("nVeloTracks", &nVeloTracks, &b_nVeloTracks);
    tree->SetBranchAddress("nTTracks", &nTTracks, &b_nTTracks);
    tree->SetBranchAddress("nBackTracks", &nBackTracks, &b_nBackTracks);
    tree->SetBranchAddress("nRich1Hits", &nRich1Hits, &b_nRich1Hits);
    tree->SetBranchAddress("nRich2Hits", &nRich2Hits, &b_nRich2Hits);
    tree->SetBranchAddress("nVeloClusters", &nVeloClusters, &b_nVeloClusters);
    tree->SetBranchAddress("nITClusters", &nITClusters, &b_nITClusters);
    tree->SetBranchAddress("nTTClusters", &nTTClusters, &b_nTTClusters);
    tree->SetBranchAddress("nOTClusters", &nOTClusters, &b_nOTClusters);
    tree->SetBranchAddress("nSPDHits", &nSPDHits, &b_nSPDHits);
    tree->SetBranchAddress("nMuonCoordsS0", &nMuonCoordsS0, &b_nMuonCoordsS0);
    tree->SetBranchAddress("nMuonCoordsS1", &nMuonCoordsS1, &b_nMuonCoordsS1);
    tree->SetBranchAddress("nMuonCoordsS2", &nMuonCoordsS2, &b_nMuonCoordsS2);
    tree->SetBranchAddress("nMuonCoordsS3", &nMuonCoordsS3, &b_nMuonCoordsS3);
    tree->SetBranchAddress("nMuonCoordsS4", &nMuonCoordsS4, &b_nMuonCoordsS4);
    tree->SetBranchAddress("nMuonTracks", &nMuonTracks, &b_nMuonTracks);
    tree->SetBranchAddress("L0Global", &L0Global, &b_L0Global);
    tree->SetBranchAddress("Hlt1Global", &Hlt1Global, &b_Hlt1Global);
    tree->SetBranchAddress("Hlt2Global", &Hlt2Global, &b_Hlt2Global);

}


#endif