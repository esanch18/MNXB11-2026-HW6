#include "MovingObject.h"
#include <TFile.h>
#include <TTree.h>
#include <TRandom.h>

void write()
{
    const Int_t nEvents = 10000;

    // Initialize random seed
    gRandom->SetSeed(12345);
    
    // Initialize your object e.g. as a pointer
    MovingObject *object = new MovingObject();
    
    // Create your root file here
    TFile *file = new TFile("tree_file.root", "RECREATE");
    
    // Create your TTree here
    TTree *tree = new TTree("tree", "Velocity measurements");
    
    // And the associated branches underneath
    // for example a branch with your personal object type
    tree->Branch("object", &object);
    
    // Now we create our loop for filling the tree with some random data
    for (Int_t i{0}; i<nEvents; i++)
    {
        Double_t vx = gRandom->Gaus(0.0, 0.30);
        Double_t vy = gRandom->Gaus(0.0, 0.30);
        Double_t vz = gRandom->Gaus(0.0, 0.30);

        object->SetVelocity(vx, vy, vz);

        tree->Fill();
    }

    tree->Write();
    file->Close();
        
    // Remember to delete it again otherwise you will have memory leak!
    delete object;
    delete file;
}