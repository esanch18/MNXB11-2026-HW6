#include "MovingObject.h"
#include <TFile.h>
#include <TTree.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <iostream>

void read()
{
    // First plot
    TH2D *h = new TH2D(
        "h",
        "Velocity components;Vx (m/s);Vy (m/s)",
        100, -1.5, 1.5,
        100, -1.5, 1.5
    );
     
    TFile *file = TFile::Open("tree_file.root", "READ");
     
     if (!file || file->IsZombie())
     {
        std::cerr << "Error: could not open tree_file.root" << std::endl;
        return;
    }
    
    TTree *tree = (TTree*)file->Get("tree");
    
    if (!tree)
    {
        std::cerr << "Error: could not find tree" << std::endl;
        file->Close();
        delete file;
        return;
    }
     
    MovingObject *object = nullptr;
    tree->SetBranchAddress("object", &object);

    const Long64_t nEntries = tree->GetEntries();


    for (Long64_t i = 0; i < nEntries; ++i)
    {
        tree->GetEntry(i);

        h->Fill(object->Vx(), object->Vy());
    }

    TCanvas *canvas1 = new TCanvas(
        "canvas1",
        "Velocity components",
        800,
        600
    );

    h->Draw("COLZ");

    canvas1->SaveAs("velocity_components.pdf");
    canvas1->SaveAs("velocity_components.png");

    // Second plot
    TCanvas *canvas2 = new TCanvas(
        "canvas2",
        "Horizontal speed versus Vz",
        800,
        600
    );

    tree->Draw(
        "sqrt(object.fVx*object.fVx + object.fVy*object.fVy):object.fVz",
        "sqrt(object.fVx*object.fVx + object.fVy*object.fVy + object.fVz*object.fVz) > 0.50",
        "p"
    );

    TH1 *h2 = (TH1*)gPad->GetPrimitive("htemp");
    h2->SetTitle("Horizontal speed versus Vz;vz (m/s);vxy (m/s)");

    canvas2->SaveAs("vxy_vs_vz.pdf");
    canvas2->SaveAs("vxy_vs_vz.png");

    file->Close();
    delete file;
}