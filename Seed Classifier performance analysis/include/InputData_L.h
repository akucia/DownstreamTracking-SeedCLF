#ifndef INPUTDATA_L_H
#define INPUTDATA_L_H
#include <string>

#include "TTree.h"
#include "TFile.h"

class InputData_L{

	public:
	InputData_L(std::string filepath, std::string treepath){
		file = new TFile(filepath.c_str());
		tree = (TTree*)file->Get(treepath.c_str());
		nrOfEvents = tree->GetEntries(); 
		setBranches();
    }

	~InputData_L(){
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
    static constexpr Int_t kMaxLambda0_ENDVERTEX_COV = 1;
    static constexpr Int_t kMaxLambda0_OWNPV_COV = 1;
    static constexpr Int_t kMaxpplus_OWNPV_COV = 1;
    static constexpr Int_t kMaxpplus_ORIVX_COV = 1;
    static constexpr Int_t kMaxpiminus_OWNPV_COV = 1;
    static constexpr Int_t kMaxpiminus_ORIVX_COV = 1;
    
    // Declaration of leaf types
    Double_t        Lambda0_LV01;
    Double_t        Lambda0_eta;
    Double_t        Lambda0_phi;
    Double_t        Lambda0_ENDVERTEX_X;
    Double_t        Lambda0_ENDVERTEX_Y;
    Double_t        Lambda0_ENDVERTEX_Z;
    Double_t        Lambda0_ENDVERTEX_XERR;
    Double_t        Lambda0_ENDVERTEX_YERR;
    Double_t        Lambda0_ENDVERTEX_ZERR;
    Double_t        Lambda0_ENDVERTEX_CHI2;
    Int_t           Lambda0_ENDVERTEX_NDOF;
    Float_t         Lambda0_ENDVERTEX_COV_[3][3];
    Double_t        Lambda0_OWNPV_X;
    Double_t        Lambda0_OWNPV_Y;
    Double_t        Lambda0_OWNPV_Z;
    Double_t        Lambda0_OWNPV_XERR;
    Double_t        Lambda0_OWNPV_YERR;
    Double_t        Lambda0_OWNPV_ZERR;
    Double_t        Lambda0_OWNPV_CHI2;
    Int_t           Lambda0_OWNPV_NDOF;
    Float_t         Lambda0_OWNPV_COV_[3][3];
    Double_t        Lambda0_IP_OWNPV;
    Double_t        Lambda0_IPCHI2_OWNPV;
    Double_t        Lambda0_P;
    Double_t        Lambda0_PT;
    Double_t        Lambda0_PE;
    Double_t        Lambda0_PX;
    Double_t        Lambda0_PY;
    Double_t        Lambda0_PZ;
    Double_t        Lambda0_MM;
    Double_t        Lambda0_MMERR;
    Double_t        Lambda0_M;
    Int_t           Lambda0_ID;
    Double_t        pplus_LV01;
    Double_t        pplus_eta;
    Double_t        pplus_phi;
    Double_t        pplus_MC12TuneV2_ProbNNe;
    Double_t        pplus_MC12TuneV2_ProbNNmu;
    Double_t        pplus_MC12TuneV2_ProbNNpi;
    Double_t        pplus_MC12TuneV2_ProbNNk;
    Double_t        pplus_MC12TuneV2_ProbNNp;
    Double_t        pplus_MC12TuneV2_ProbNNghost;
    Double_t        pplus_MC12TuneV3_ProbNNe;
    Double_t        pplus_MC12TuneV3_ProbNNmu;
    Double_t        pplus_MC12TuneV3_ProbNNpi;
    Double_t        pplus_MC12TuneV3_ProbNNk;
    Double_t        pplus_MC12TuneV3_ProbNNp;
    Double_t        pplus_MC12TuneV3_ProbNNghost;
    Double_t        pplus_MC12TuneV4_ProbNNe;
    Double_t        pplus_MC12TuneV4_ProbNNmu;
    Double_t        pplus_MC12TuneV4_ProbNNpi;
    Double_t        pplus_MC12TuneV4_ProbNNk;
    Double_t        pplus_MC12TuneV4_ProbNNp;
    Double_t        pplus_MC12TuneV4_ProbNNghost;
    Double_t        pplus_MC15TuneV1_ProbNNe;
    Double_t        pplus_MC15TuneV1_ProbNNmu;
    Double_t        pplus_MC15TuneV1_ProbNNpi;
    Double_t        pplus_MC15TuneV1_ProbNNk;
    Double_t        pplus_MC15TuneV1_ProbNNp;
    Double_t        pplus_MC15TuneV1_ProbNNghost;
    Double_t        pplus_OWNPV_X;
    Double_t        pplus_OWNPV_Y;
    Double_t        pplus_OWNPV_Z;
    Double_t        pplus_OWNPV_XERR;
    Double_t        pplus_OWNPV_YERR;
    Double_t        pplus_OWNPV_ZERR;
    Double_t        pplus_OWNPV_CHI2;
    Int_t           pplus_OWNPV_NDOF;
    Float_t         pplus_OWNPV_COV_[3][3];
    Double_t        pplus_IP_OWNPV;
    Double_t        pplus_IPCHI2_OWNPV;
    Double_t        pplus_ORIVX_X;
    Double_t        pplus_ORIVX_Y;
    Double_t        pplus_ORIVX_Z;
    Double_t        pplus_ORIVX_XERR;
    Double_t        pplus_ORIVX_YERR;
    Double_t        pplus_ORIVX_ZERR;
    Double_t        pplus_ORIVX_CHI2;
    Int_t           pplus_ORIVX_NDOF;
    Float_t         pplus_ORIVX_COV_[3][3];
    Double_t        pplus_P;
    Double_t        pplus_PT;
    Double_t        pplus_PE;
    Double_t        pplus_PX;
    Double_t        pplus_PY;
    Double_t        pplus_PZ;
    Double_t        pplus_M;
    Int_t           pplus_ID;
    Double_t        pplus_PIDe;
    Double_t        pplus_PIDmu;
    Double_t        pplus_PIDK;
    Double_t        pplus_PIDp;
    Double_t        pplus_ProbNNe;
    Double_t        pplus_ProbNNk;
    Double_t        pplus_ProbNNp;
    Double_t        pplus_ProbNNpi;
    Double_t        pplus_ProbNNmu;
    Double_t        pplus_ProbNNghost;
    Bool_t          pplus_hasMuon;
    Bool_t          pplus_isMuon;
    Bool_t          pplus_hasRich;
    Bool_t          pplus_hasCalo;
    Int_t           pplus_TRACK_Type;
    Int_t           pplus_TRACK_Key;
    Double_t        pplus_TRACK_CHI2NDOF;
    Double_t        pplus_TRACK_PCHI2;
    Double_t        pplus_TRACK_MatchCHI2;
    Double_t        pplus_TRACK_GhostProb;
    Double_t        pplus_TRACK_CloneDist;
    Double_t        pplus_TRACK_Likelihood;
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
    TBranch        *b_Lambda0_LV01;   //!
    TBranch        *b_Lambda0_eta;   //!
    TBranch        *b_Lambda0_phi;   //!
    TBranch        *b_Lambda0_ENDVERTEX_X;   //!
    TBranch        *b_Lambda0_ENDVERTEX_Y;   //!
    TBranch        *b_Lambda0_ENDVERTEX_Z;   //!
    TBranch        *b_Lambda0_ENDVERTEX_XERR;   //!
    TBranch        *b_Lambda0_ENDVERTEX_YERR;   //!
    TBranch        *b_Lambda0_ENDVERTEX_ZERR;   //!
    TBranch        *b_Lambda0_ENDVERTEX_CHI2;   //!
    TBranch        *b_Lambda0_ENDVERTEX_NDOF;   //!
    TBranch        *b_Lambda0_ENDVERTEX_COV_;   //!
    TBranch        *b_Lambda0_OWNPV_X;   //!
    TBranch        *b_Lambda0_OWNPV_Y;   //!
    TBranch        *b_Lambda0_OWNPV_Z;   //!
    TBranch        *b_Lambda0_OWNPV_XERR;   //!
    TBranch        *b_Lambda0_OWNPV_YERR;   //!
    TBranch        *b_Lambda0_OWNPV_ZERR;   //!
    TBranch        *b_Lambda0_OWNPV_CHI2;   //!
    TBranch        *b_Lambda0_OWNPV_NDOF;   //!
    TBranch        *b_Lambda0_OWNPV_COV_;   //!
    TBranch        *b_Lambda0_IP_OWNPV;   //!
    TBranch        *b_Lambda0_IPCHI2_OWNPV;   //!
    TBranch        *b_Lambda0_P;   //!
    TBranch        *b_Lambda0_PT;   //!
    TBranch        *b_Lambda0_PE;   //!
    TBranch        *b_Lambda0_PX;   //!
    TBranch        *b_Lambda0_PY;   //!
    TBranch        *b_Lambda0_PZ;   //!
    TBranch        *b_Lambda0_MM;   //!
    TBranch        *b_Lambda0_MMERR;   //!
    TBranch        *b_Lambda0_M;   //!
    TBranch        *b_Lambda0_ID;   //!
    TBranch        *b_pplus_LV01;   //!
    TBranch        *b_pplus_eta;   //!
    TBranch        *b_pplus_phi;   //!
    TBranch        *b_pplus_MC12TuneV2_ProbNNe;   //!
    TBranch        *b_pplus_MC12TuneV2_ProbNNmu;   //!
    TBranch        *b_pplus_MC12TuneV2_ProbNNpi;   //!
    TBranch        *b_pplus_MC12TuneV2_ProbNNk;   //!
    TBranch        *b_pplus_MC12TuneV2_ProbNNp;   //!
    TBranch        *b_pplus_MC12TuneV2_ProbNNghost;   //!
    TBranch        *b_pplus_MC12TuneV3_ProbNNe;   //!
    TBranch        *b_pplus_MC12TuneV3_ProbNNmu;   //!
    TBranch        *b_pplus_MC12TuneV3_ProbNNpi;   //!
    TBranch        *b_pplus_MC12TuneV3_ProbNNk;   //!
    TBranch        *b_pplus_MC12TuneV3_ProbNNp;   //!
    TBranch        *b_pplus_MC12TuneV3_ProbNNghost;   //!
    TBranch        *b_pplus_MC12TuneV4_ProbNNe;   //!
    TBranch        *b_pplus_MC12TuneV4_ProbNNmu;   //!
    TBranch        *b_pplus_MC12TuneV4_ProbNNpi;   //!
    TBranch        *b_pplus_MC12TuneV4_ProbNNk;   //!
    TBranch        *b_pplus_MC12TuneV4_ProbNNp;   //!
    TBranch        *b_pplus_MC12TuneV4_ProbNNghost;   //!
    TBranch        *b_pplus_MC15TuneV1_ProbNNe;   //!
    TBranch        *b_pplus_MC15TuneV1_ProbNNmu;   //!
    TBranch        *b_pplus_MC15TuneV1_ProbNNpi;   //!
    TBranch        *b_pplus_MC15TuneV1_ProbNNk;   //!
    TBranch        *b_pplus_MC15TuneV1_ProbNNp;   //!
    TBranch        *b_pplus_MC15TuneV1_ProbNNghost;   //!
    TBranch        *b_pplus_OWNPV_X;   //!
    TBranch        *b_pplus_OWNPV_Y;   //!
    TBranch        *b_pplus_OWNPV_Z;   //!
    TBranch        *b_pplus_OWNPV_XERR;   //!
    TBranch        *b_pplus_OWNPV_YERR;   //!
    TBranch        *b_pplus_OWNPV_ZERR;   //!
    TBranch        *b_pplus_OWNPV_CHI2;   //!
    TBranch        *b_pplus_OWNPV_NDOF;   //!
    TBranch        *b_pplus_OWNPV_COV_;   //!
    TBranch        *b_pplus_IP_OWNPV;   //!
    TBranch        *b_pplus_IPCHI2_OWNPV;   //!
    TBranch        *b_pplus_ORIVX_X;   //!
    TBranch        *b_pplus_ORIVX_Y;   //!
    TBranch        *b_pplus_ORIVX_Z;   //!
    TBranch        *b_pplus_ORIVX_XERR;   //!
    TBranch        *b_pplus_ORIVX_YERR;   //!
    TBranch        *b_pplus_ORIVX_ZERR;   //!
    TBranch        *b_pplus_ORIVX_CHI2;   //!
    TBranch        *b_pplus_ORIVX_NDOF;   //!
    TBranch        *b_pplus_ORIVX_COV_;   //!
    TBranch        *b_pplus_P;   //!
    TBranch        *b_pplus_PT;   //!
    TBranch        *b_pplus_PE;   //!
    TBranch        *b_pplus_PX;   //!
    TBranch        *b_pplus_PY;   //!
    TBranch        *b_pplus_PZ;   //!
    TBranch        *b_pplus_M;   //!
    TBranch        *b_pplus_ID;   //!
    TBranch        *b_pplus_PIDe;   //!
    TBranch        *b_pplus_PIDmu;   //!
    TBranch        *b_pplus_PIDK;   //!
    TBranch        *b_pplus_PIDp;   //!
    TBranch        *b_pplus_ProbNNe;   //!
    TBranch        *b_pplus_ProbNNk;   //!
    TBranch        *b_pplus_ProbNNp;   //!
    TBranch        *b_pplus_ProbNNpi;   //!
    TBranch        *b_pplus_ProbNNmu;   //!
    TBranch        *b_pplus_ProbNNghost;   //!
    TBranch        *b_pplus_hasMuon;   //!
    TBranch        *b_pplus_isMuon;   //!
    TBranch        *b_pplus_hasRich;   //!
    TBranch        *b_pplus_hasCalo;   //!
    TBranch        *b_pplus_TRACK_Type;   //!
    TBranch        *b_pplus_TRACK_Key;   //!
    TBranch        *b_pplus_TRACK_CHI2NDOF;   //!
    TBranch        *b_pplus_TRACK_PCHI2;   //!
    TBranch        *b_pplus_TRACK_MatchCHI2;   //!
    TBranch        *b_pplus_TRACK_GhostProb;   //!
    TBranch        *b_pplus_TRACK_CloneDist;   //!
    TBranch        *b_pplus_TRACK_Likelihood;   //!
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

void InputData_L::setBranches(){
    
    tree->SetBranchAddress("Lambda0_LV01", &Lambda0_LV01, &b_Lambda0_LV01);
    tree->SetBranchAddress("Lambda0_eta", &Lambda0_eta, &b_Lambda0_eta);
    tree->SetBranchAddress("Lambda0_phi", &Lambda0_phi, &b_Lambda0_phi);
    tree->SetBranchAddress("Lambda0_ENDVERTEX_X", &Lambda0_ENDVERTEX_X, &b_Lambda0_ENDVERTEX_X);
    tree->SetBranchAddress("Lambda0_ENDVERTEX_Y", &Lambda0_ENDVERTEX_Y, &b_Lambda0_ENDVERTEX_Y);
    tree->SetBranchAddress("Lambda0_ENDVERTEX_Z", &Lambda0_ENDVERTEX_Z, &b_Lambda0_ENDVERTEX_Z);
    tree->SetBranchAddress("Lambda0_ENDVERTEX_XERR", &Lambda0_ENDVERTEX_XERR, &b_Lambda0_ENDVERTEX_XERR);
    tree->SetBranchAddress("Lambda0_ENDVERTEX_YERR", &Lambda0_ENDVERTEX_YERR, &b_Lambda0_ENDVERTEX_YERR);
    tree->SetBranchAddress("Lambda0_ENDVERTEX_ZERR", &Lambda0_ENDVERTEX_ZERR, &b_Lambda0_ENDVERTEX_ZERR);
    tree->SetBranchAddress("Lambda0_ENDVERTEX_CHI2", &Lambda0_ENDVERTEX_CHI2, &b_Lambda0_ENDVERTEX_CHI2);
    tree->SetBranchAddress("Lambda0_ENDVERTEX_NDOF", &Lambda0_ENDVERTEX_NDOF, &b_Lambda0_ENDVERTEX_NDOF);
    tree->SetBranchAddress("Lambda0_ENDVERTEX_COV_", Lambda0_ENDVERTEX_COV_, &b_Lambda0_ENDVERTEX_COV_);
    tree->SetBranchAddress("Lambda0_OWNPV_X", &Lambda0_OWNPV_X, &b_Lambda0_OWNPV_X);
    tree->SetBranchAddress("Lambda0_OWNPV_Y", &Lambda0_OWNPV_Y, &b_Lambda0_OWNPV_Y);
    tree->SetBranchAddress("Lambda0_OWNPV_Z", &Lambda0_OWNPV_Z, &b_Lambda0_OWNPV_Z);
    tree->SetBranchAddress("Lambda0_OWNPV_XERR", &Lambda0_OWNPV_XERR, &b_Lambda0_OWNPV_XERR);
    tree->SetBranchAddress("Lambda0_OWNPV_YERR", &Lambda0_OWNPV_YERR, &b_Lambda0_OWNPV_YERR);
    tree->SetBranchAddress("Lambda0_OWNPV_ZERR", &Lambda0_OWNPV_ZERR, &b_Lambda0_OWNPV_ZERR);
    tree->SetBranchAddress("Lambda0_OWNPV_CHI2", &Lambda0_OWNPV_CHI2, &b_Lambda0_OWNPV_CHI2);
    tree->SetBranchAddress("Lambda0_OWNPV_NDOF", &Lambda0_OWNPV_NDOF, &b_Lambda0_OWNPV_NDOF);
    tree->SetBranchAddress("Lambda0_OWNPV_COV_", Lambda0_OWNPV_COV_, &b_Lambda0_OWNPV_COV_);
    tree->SetBranchAddress("Lambda0_IP_OWNPV", &Lambda0_IP_OWNPV, &b_Lambda0_IP_OWNPV);
    tree->SetBranchAddress("Lambda0_IPCHI2_OWNPV", &Lambda0_IPCHI2_OWNPV, &b_Lambda0_IPCHI2_OWNPV);
    tree->SetBranchAddress("Lambda0_P", &Lambda0_P, &b_Lambda0_P);
    tree->SetBranchAddress("Lambda0_PT", &Lambda0_PT, &b_Lambda0_PT);
    tree->SetBranchAddress("Lambda0_PE", &Lambda0_PE, &b_Lambda0_PE);
    tree->SetBranchAddress("Lambda0_PX", &Lambda0_PX, &b_Lambda0_PX);
    tree->SetBranchAddress("Lambda0_PY", &Lambda0_PY, &b_Lambda0_PY);
    tree->SetBranchAddress("Lambda0_PZ", &Lambda0_PZ, &b_Lambda0_PZ);
    tree->SetBranchAddress("Lambda0_MM", &Lambda0_MM, &b_Lambda0_MM);
    tree->SetBranchAddress("Lambda0_MMERR", &Lambda0_MMERR, &b_Lambda0_MMERR);
    tree->SetBranchAddress("Lambda0_M", &Lambda0_M, &b_Lambda0_M);
    tree->SetBranchAddress("Lambda0_ID", &Lambda0_ID, &b_Lambda0_ID);
    tree->SetBranchAddress("pplus_LV01", &pplus_LV01, &b_pplus_LV01);
    tree->SetBranchAddress("pplus_eta", &pplus_eta, &b_pplus_eta);
    tree->SetBranchAddress("pplus_phi", &pplus_phi, &b_pplus_phi);
    tree->SetBranchAddress("pplus_MC12TuneV2_ProbNNe", &pplus_MC12TuneV2_ProbNNe, &b_pplus_MC12TuneV2_ProbNNe);
    tree->SetBranchAddress("pplus_MC12TuneV2_ProbNNmu", &pplus_MC12TuneV2_ProbNNmu, &b_pplus_MC12TuneV2_ProbNNmu);
    tree->SetBranchAddress("pplus_MC12TuneV2_ProbNNpi", &pplus_MC12TuneV2_ProbNNpi, &b_pplus_MC12TuneV2_ProbNNpi);
    tree->SetBranchAddress("pplus_MC12TuneV2_ProbNNk", &pplus_MC12TuneV2_ProbNNk, &b_pplus_MC12TuneV2_ProbNNk);
    tree->SetBranchAddress("pplus_MC12TuneV2_ProbNNp", &pplus_MC12TuneV2_ProbNNp, &b_pplus_MC12TuneV2_ProbNNp);
    tree->SetBranchAddress("pplus_MC12TuneV2_ProbNNghost", &pplus_MC12TuneV2_ProbNNghost, &b_pplus_MC12TuneV2_ProbNNghost);
    tree->SetBranchAddress("pplus_MC12TuneV3_ProbNNe", &pplus_MC12TuneV3_ProbNNe, &b_pplus_MC12TuneV3_ProbNNe);
    tree->SetBranchAddress("pplus_MC12TuneV3_ProbNNmu", &pplus_MC12TuneV3_ProbNNmu, &b_pplus_MC12TuneV3_ProbNNmu);
    tree->SetBranchAddress("pplus_MC12TuneV3_ProbNNpi", &pplus_MC12TuneV3_ProbNNpi, &b_pplus_MC12TuneV3_ProbNNpi);
    tree->SetBranchAddress("pplus_MC12TuneV3_ProbNNk", &pplus_MC12TuneV3_ProbNNk, &b_pplus_MC12TuneV3_ProbNNk);
    tree->SetBranchAddress("pplus_MC12TuneV3_ProbNNp", &pplus_MC12TuneV3_ProbNNp, &b_pplus_MC12TuneV3_ProbNNp);
    tree->SetBranchAddress("pplus_MC12TuneV3_ProbNNghost", &pplus_MC12TuneV3_ProbNNghost, &b_pplus_MC12TuneV3_ProbNNghost);
    tree->SetBranchAddress("pplus_MC12TuneV4_ProbNNe", &pplus_MC12TuneV4_ProbNNe, &b_pplus_MC12TuneV4_ProbNNe);
    tree->SetBranchAddress("pplus_MC12TuneV4_ProbNNmu", &pplus_MC12TuneV4_ProbNNmu, &b_pplus_MC12TuneV4_ProbNNmu);
    tree->SetBranchAddress("pplus_MC12TuneV4_ProbNNpi", &pplus_MC12TuneV4_ProbNNpi, &b_pplus_MC12TuneV4_ProbNNpi);
    tree->SetBranchAddress("pplus_MC12TuneV4_ProbNNk", &pplus_MC12TuneV4_ProbNNk, &b_pplus_MC12TuneV4_ProbNNk);
    tree->SetBranchAddress("pplus_MC12TuneV4_ProbNNp", &pplus_MC12TuneV4_ProbNNp, &b_pplus_MC12TuneV4_ProbNNp);
    tree->SetBranchAddress("pplus_MC12TuneV4_ProbNNghost", &pplus_MC12TuneV4_ProbNNghost, &b_pplus_MC12TuneV4_ProbNNghost);
    tree->SetBranchAddress("pplus_MC15TuneV1_ProbNNe", &pplus_MC15TuneV1_ProbNNe, &b_pplus_MC15TuneV1_ProbNNe);
    tree->SetBranchAddress("pplus_MC15TuneV1_ProbNNmu", &pplus_MC15TuneV1_ProbNNmu, &b_pplus_MC15TuneV1_ProbNNmu);
    tree->SetBranchAddress("pplus_MC15TuneV1_ProbNNpi", &pplus_MC15TuneV1_ProbNNpi, &b_pplus_MC15TuneV1_ProbNNpi);
    tree->SetBranchAddress("pplus_MC15TuneV1_ProbNNk", &pplus_MC15TuneV1_ProbNNk, &b_pplus_MC15TuneV1_ProbNNk);
    tree->SetBranchAddress("pplus_MC15TuneV1_ProbNNp", &pplus_MC15TuneV1_ProbNNp, &b_pplus_MC15TuneV1_ProbNNp);
    tree->SetBranchAddress("pplus_MC15TuneV1_ProbNNghost", &pplus_MC15TuneV1_ProbNNghost, &b_pplus_MC15TuneV1_ProbNNghost);
    tree->SetBranchAddress("pplus_OWNPV_X", &pplus_OWNPV_X, &b_pplus_OWNPV_X);
    tree->SetBranchAddress("pplus_OWNPV_Y", &pplus_OWNPV_Y, &b_pplus_OWNPV_Y);
    tree->SetBranchAddress("pplus_OWNPV_Z", &pplus_OWNPV_Z, &b_pplus_OWNPV_Z);
    tree->SetBranchAddress("pplus_OWNPV_XERR", &pplus_OWNPV_XERR, &b_pplus_OWNPV_XERR);
    tree->SetBranchAddress("pplus_OWNPV_YERR", &pplus_OWNPV_YERR, &b_pplus_OWNPV_YERR);
    tree->SetBranchAddress("pplus_OWNPV_ZERR", &pplus_OWNPV_ZERR, &b_pplus_OWNPV_ZERR);
    tree->SetBranchAddress("pplus_OWNPV_CHI2", &pplus_OWNPV_CHI2, &b_pplus_OWNPV_CHI2);
    tree->SetBranchAddress("pplus_OWNPV_NDOF", &pplus_OWNPV_NDOF, &b_pplus_OWNPV_NDOF);
    tree->SetBranchAddress("pplus_OWNPV_COV_", pplus_OWNPV_COV_, &b_pplus_OWNPV_COV_);
    tree->SetBranchAddress("pplus_IP_OWNPV", &pplus_IP_OWNPV, &b_pplus_IP_OWNPV);
    tree->SetBranchAddress("pplus_IPCHI2_OWNPV", &pplus_IPCHI2_OWNPV, &b_pplus_IPCHI2_OWNPV);
    tree->SetBranchAddress("pplus_ORIVX_X", &pplus_ORIVX_X, &b_pplus_ORIVX_X);
    tree->SetBranchAddress("pplus_ORIVX_Y", &pplus_ORIVX_Y, &b_pplus_ORIVX_Y);
    tree->SetBranchAddress("pplus_ORIVX_Z", &pplus_ORIVX_Z, &b_pplus_ORIVX_Z);
    tree->SetBranchAddress("pplus_ORIVX_XERR", &pplus_ORIVX_XERR, &b_pplus_ORIVX_XERR);
    tree->SetBranchAddress("pplus_ORIVX_YERR", &pplus_ORIVX_YERR, &b_pplus_ORIVX_YERR);
    tree->SetBranchAddress("pplus_ORIVX_ZERR", &pplus_ORIVX_ZERR, &b_pplus_ORIVX_ZERR);
    tree->SetBranchAddress("pplus_ORIVX_CHI2", &pplus_ORIVX_CHI2, &b_pplus_ORIVX_CHI2);
    tree->SetBranchAddress("pplus_ORIVX_NDOF", &pplus_ORIVX_NDOF, &b_pplus_ORIVX_NDOF);
    tree->SetBranchAddress("pplus_ORIVX_COV_", pplus_ORIVX_COV_, &b_pplus_ORIVX_COV_);
    tree->SetBranchAddress("pplus_P", &pplus_P, &b_pplus_P);
    tree->SetBranchAddress("pplus_PT", &pplus_PT, &b_pplus_PT);
    tree->SetBranchAddress("pplus_PE", &pplus_PE, &b_pplus_PE);
    tree->SetBranchAddress("pplus_PX", &pplus_PX, &b_pplus_PX);
    tree->SetBranchAddress("pplus_PY", &pplus_PY, &b_pplus_PY);
    tree->SetBranchAddress("pplus_PZ", &pplus_PZ, &b_pplus_PZ);
    tree->SetBranchAddress("pplus_M", &pplus_M, &b_pplus_M);
    tree->SetBranchAddress("pplus_ID", &pplus_ID, &b_pplus_ID);
    tree->SetBranchAddress("pplus_PIDe", &pplus_PIDe, &b_pplus_PIDe);
    tree->SetBranchAddress("pplus_PIDmu", &pplus_PIDmu, &b_pplus_PIDmu);
    tree->SetBranchAddress("pplus_PIDK", &pplus_PIDK, &b_pplus_PIDK);
    tree->SetBranchAddress("pplus_PIDp", &pplus_PIDp, &b_pplus_PIDp);
    tree->SetBranchAddress("pplus_ProbNNe", &pplus_ProbNNe, &b_pplus_ProbNNe);
    tree->SetBranchAddress("pplus_ProbNNk", &pplus_ProbNNk, &b_pplus_ProbNNk);
    tree->SetBranchAddress("pplus_ProbNNp", &pplus_ProbNNp, &b_pplus_ProbNNp);
    tree->SetBranchAddress("pplus_ProbNNpi", &pplus_ProbNNpi, &b_pplus_ProbNNpi);
    tree->SetBranchAddress("pplus_ProbNNmu", &pplus_ProbNNmu, &b_pplus_ProbNNmu);
    tree->SetBranchAddress("pplus_ProbNNghost", &pplus_ProbNNghost, &b_pplus_ProbNNghost);
    tree->SetBranchAddress("pplus_hasMuon", &pplus_hasMuon, &b_pplus_hasMuon);
    tree->SetBranchAddress("pplus_isMuon", &pplus_isMuon, &b_pplus_isMuon);
    tree->SetBranchAddress("pplus_hasRich", &pplus_hasRich, &b_pplus_hasRich);
    tree->SetBranchAddress("pplus_hasCalo", &pplus_hasCalo, &b_pplus_hasCalo);
    tree->SetBranchAddress("pplus_TRACK_Type", &pplus_TRACK_Type, &b_pplus_TRACK_Type);
    tree->SetBranchAddress("pplus_TRACK_Key", &pplus_TRACK_Key, &b_pplus_TRACK_Key);
    tree->SetBranchAddress("pplus_TRACK_CHI2NDOF", &pplus_TRACK_CHI2NDOF, &b_pplus_TRACK_CHI2NDOF);
    tree->SetBranchAddress("pplus_TRACK_PCHI2", &pplus_TRACK_PCHI2, &b_pplus_TRACK_PCHI2);
    tree->SetBranchAddress("pplus_TRACK_MatchCHI2", &pplus_TRACK_MatchCHI2, &b_pplus_TRACK_MatchCHI2);
    tree->SetBranchAddress("pplus_TRACK_GhostProb", &pplus_TRACK_GhostProb, &b_pplus_TRACK_GhostProb);
    tree->SetBranchAddress("pplus_TRACK_CloneDist", &pplus_TRACK_CloneDist, &b_pplus_TRACK_CloneDist);
    tree->SetBranchAddress("pplus_TRACK_Likelihood", &pplus_TRACK_Likelihood, &b_pplus_TRACK_Likelihood);
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