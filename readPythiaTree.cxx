#include "TSystem.h"
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TClonesArray.h"
#include "TBranch.h"
#include "TLorentzVector.h"
#include "TParameter.h"
#include <iostream>
#include <cmath>
#include <exception>
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/ClusterSequenceArea.hh"
#include "fastjet/Selector.hh"
#include "fastjet/tools/JetMedianBackgroundEstimator.hh"
#include "fastjet/tools/Subtractor.hh"

using namespace fastjet;
using namespace std;


int main(int argc, const char** argv)
{
    const char *defaults[6] = {"readPythiaTree","test.root","in","2","150","jets"};
    if ( argc==1 ) {
        argv=defaults;
        argc=8;
    }

    TString InFileName = argv[1];
    cout<<"In file is "<<InFileName.Data()<<endl;
    
    TString OutFileName = argv[2];
    if (OutFileName == "in"){
        OutFileName = InFileName;
        OutFileName.Append(".jets.root");
    }
    cout<<"Out file is "<<OutFileName.Data()<<endl;
    
    float ptconsmin = atof(argv[4])/1000.; //constiuent cut
    cout<<"min pt cut = "<<ptconsmin<<endl;
    
    TString jetname = argv[5];
    jetname.Append("_R0");
    jetname.Append(argv[3]);
    jetname.Append("0_ptmin");
    jetname.Append(argv[4]);
    
    float radius = atof(argv[3])/10.;
    cout<<"R = "<<radius<<endl;
    cout<<"Jet name = "<<jetname.Data()<<endl;
    
    TFile *_file0 = TFile::Open(InFileName);
    
    
    //read tree
    TTree* tree = (TTree*)gROOT->FindObject("tree");
    //tree->Print();
    //read collections
    TClonesArray *particles = new TClonesArray("TLorentzVector");
    TBranch* bparticles = tree->GetBranch("Particles");
    bparticles->SetAddress(&particles);
    
    //open output file
    TFile *file1 = TFile::Open(OutFileName.Data(),"RECREATE");
    Int_t entries = tree->GetEntries();
    cout<<"Entries = "<<entries<<endl;
    
    //copy existing tree
    TTree* newtree = tree->CloneTree(0);
    //Make new branch for jet collection
    TClonesArray *jetsclone = new TClonesArray("TLorentzVector");
    newtree->Branch(jetname.Data(),&jetsclone);
    
    
    //Create the collection of pseudo jets to be used in jet finding
    vector<PseudoJet> particles_jets;
    //Define background and signal jets
    JetDefinition jet_def(antikt_algorithm, radius);
    
    //Some specifications for ghost particles
    double ghost_maxrap = 1.5;
    GhostedAreaSpec area_spec(ghost_maxrap);
    AreaDefinition area_def(active_area, area_spec);
    AreaDefinition area_def_bkgd(active_area_explicit_ghosts, GhostedAreaSpec(ghost_maxrap));
    
    for (Int_t iev = 0;iev<entries;iev++){
        if (iev % 1000 == 0)
            cout<<"Event "<<iev<<endl;
        //Loop over events;
        //First clear all the collections for nice memory management
        particles_jets.clear();
        jetsclone->Clear();
        
        //Load current event
        tree->GetEntry(iev);
        bparticles->GetEntry(iev);
        
        Int_t ntrk = particles->GetEntriesFast();
        for (int itrk = 0;itrk<ntrk;itrk++){
            //Loop over tracks and push them into the vector of pseudo vectors
            TLorentzVector* t = (TLorentzVector*)particles->At(itrk);
            if (t->Pt()< ptconsmin)
                continue;
            particles_jets.push_back( PseudoJet(t->X(),t->Y(),t->Z(),t->E()));
        }
        //run jet finder
        ClusterSequenceArea cs(particles_jets, jet_def, area_def);
        vector<PseudoJet> jets = sorted_by_pt(cs.inclusive_jets());
        int accJets = 0;
        for (unsigned i = 0; i < jets.size(); i++) {
            //if (fabs(jets[i].eta())>1)
              //  continue;
            TLorentzVector* j = new TLorentzVector(jets[i].px(),jets[i].py(),jets[i].pz(),jets[i].E());
            new ((*jetsclone)[accJets]) TLorentzVector(*j);
            accJets++;
            delete j;
        }
        newtree->Fill();
    }
    newtree->Write();
    file1->Close();
    return 0;
}
