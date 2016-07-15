#include "Pythia.h"

#include <iostream>
#include <string>
#include <sstream>

#include "TROOT.h"
#include "TMath.h"
#include "TString.h"
#include "TObjString.h"
#include "TFile.h"
#include "TH1.h"
#include "TTree.h"
#include "TF1.h"
#include "TLorentzVector.h"
#include "TClonesArray.h"
#include "TParameter.h"

using namespace std;
using namespace Pythia8;


int ReturnPdgValue(string name){
    //Function gives convenient particle names
    int particlenumber = -1000;
    if (name == "d")
        particlenumber = 1;
    if (name == "u")
        particlenumber = 2;
    if (name == "s")
        particlenumber = 3;
    if (name == "c")
        particlenumber = 4;
    if (name == "b")
        particlenumber = 5;
    if (name == "dbar")
        particlenumber = -1;
    if (name == "ubar")
        particlenumber = -2;
    if (name == "sbar")
        particlenumber = -3;
    if (name == "cbar")
        particlenumber = -4;
    if (name == "bbar")
        particlenumber = -5;
    if (name == "g")
        particlenumber = 21;
    if (name == "gamma")
        particlenumber = 22;
    return particlenumber;
}

Int_t main(int argc, char **argv) {

  // Set up output file
  TFile * fout = new TFile("pytest15.root","recreate");
    
    // Generator. Shorthand for event (use FastJet3 and Pythia8 interface!!!)
    //------------------------------------------------------------------------
    
    //Choose collision energies
    Float_t ptHatMin=15.0;
    Float_t ptHatMax=100.0;
    
    //Points to location of Pythia 8
    Pythia pythia("/home/justin/pythia8219/share/Pythia8/xmldoc");
    Event& event = pythia.event;

    // Process selection.
    pythia.readString("HardQCD:all = on");
    pythia.readString("HadronLevel:Decay = on");
    pythia.readString("PartonLevel:ISR = on");
    pythia.readString("PartonLevel:FSR = on");
    //pythia.readString("PromptPhoton:qg2qgamma = on"); // turn on prompt photons
    pythia.readString("PromptPhoton:all = on");
    
    stringstream numbf(stringstream::app|stringstream::in|stringstream::out);
    numbf.setf(ios::fixed, ios::floatfield);
    numbf.setf(ios::showpoint);
    numbf.precision(1);
    numbf.str("PhaseSpace:pTHatMin = "); numbf << ptHatMin;
    string pmin=numbf.str();
    numbf.str("PhaseSpace:pTHatMax = "); numbf << ptHatMax;
    string pmax=numbf.str();
    
    cout<<pmin<<endl;
    cout<<pmax<<endl;
    
    pythia.readString(pmin);
    pythia.readString(pmax);
    
    // No event record printout.
    pythia.readString("Next:numberShowInfo = 1");
    pythia.readString("Next:numberShowProcess = 1");
    pythia.readString("Next:numberShowEvent = 1");
    
    // LHC initialization.
    //pythia.init(2212, 2212,5020.);
    // pythia.init(2212, 2212,2760.);
    
    // RHIC initialization.
    pythia.settings.mode("Beams:idA", 2212);
    pythia.settings.mode("Beams:idB", 2212);
    pythia.settings.parm("Beams:eCM", 200.);
    pythia.init();
    
    // Set up tree
    // -----------
    static const Int_t kmaxT=5000; // Max # of particles per event
    
    TTree* tree  = new TTree("tree","Some pythia jets");
    TClonesArray Particles("TLorentzVector",kmaxT);
    tree->Branch("Particles", &Particles);
    
    // The original outgoing hard partons, status -23
    TClonesArray HardPartons("TLorentzVector",kmaxT);
    tree->Branch("HardPartons", &HardPartons);
    TClonesArray HardPartonNames("TParameter<int>",kmaxT);
    tree->Branch("HardPartonNames", &HardPartonNames);
    
    // Generate events
    //We use a do-while loop rather than a for loop because we may wish to
    //select a certain type of event.  So we want to be able to set the number
    //of selected events (nEvent)
    int nEv = 0;
    Int_t nEvent = 5000;
    
    do {
        if (!pythia.next()) continue;
        
        Particles.Clear();
        HardPartons.Clear();
        HardPartonNames.Clear();
        
        // Grab particles
        int fkTrack=0;
        int fkHard=0;
        int fkPhoton = 0;
        
        for (int nP = 0; nP<event.size() ; ++nP ){
            
            if ( nP<3 )continue;      // 0, 1, 2: total event and beams
            
            Particle& particle = event[nP];
            // Save hardest partons (initial partons)
            // --------------------
            
            if ( particle.status()==-23 ){
                Vec4 pv = particle.p();
                TParameter<int> namenumber;
                namenumber.SetVal(ReturnPdgValue(particle.name().data()));
                new ( HardPartons[fkHard]) TLorentzVector ( pv.px(), pv.py(), pv.pz(), pv.e() ) ;
                new ( HardPartonNames[fkHard]) TParameter<int> (namenumber);
                fkHard++;
            }
            
            //Next save final state particles
            if ( !particle.isFinal() ) continue;
            
            // skip neutrinos
            if ( !particle.name().compare(0, 3, "nu_") ) continue;
            
            // skip neutral particles we can't see
            if ( particle.name() == "K_L0"  ) continue;
            if ( particle.name() == "n0"    ) continue;
            if ( particle.name() == "nbar0" ) continue;
            
            /*if (
                particle.name() != "e+"    &&
                particle.name() != "e-"    &&
                particle.name() != "mu+"   &&
                particle.name() != "mu-"   &&
                particle.name() != "pi+"   &&
                particle.name() != "pi-"   &&
                particle.name() != "K+"    &&
                particle.name() != "K-"    &&
                particle.name() != "p+"    &&
                particle.name() != "pbar-" &&
                particle.name() != "gamma"
                )
                cout << particle.name() << endl;*/
                
                Vec4 pv = particle.p();
            
            
            // some preselections to cut down size and simulate STAR
            if ( pv.pT() < 0.2 ) continue;
            if ( fabs(pv.eta()) > 1.0 ) continue;
            
            new ( Particles[fkTrack] ) TLorentzVector ( pv.px(), pv.py(), pv.pz(), pv.e() ) ;
            
            ++fkTrack;
            
        }
        
        tree->Fill();
        
        ++nEv;
    } while (nEv<nEvent);
    pythia.stat();
    
    fout->Write();
    fout->Close();
    
    return 0;
}

