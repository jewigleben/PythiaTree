void set_plot_style()
{
    const Int_t NRGBs = 5;
    const Int_t NCont = 255;
    Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
    Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);
}
void SetHist(TH1* h, const char* xt ="", const char* yt ="",int color = 1, int marker = 20,int width = 3, float size = 1.0)
{
    h->SetLineWidth(width);
    h->SetLineColor(color);
    h->SetMarkerColor(color);
    h->SetMarkerSize(size);
    h->SetMarkerStyle(marker);
    h->GetYaxis()->SetTitle(yt);
    h->GetYaxis()->SetTitleOffset(1.6);
    h->GetXaxis()->SetTitle(xt);
}

void SetHist(TH2* h, const char* xt ="", const char* yt ="",int color = 1, int marker = 20,int width = 3, float size = 1.0)
{
    h->SetLineWidth(width);
    h->SetLineColor(color);
    h->SetMarkerColor(color);
    h->SetMarkerSize(size);
    h->SetMarkerStyle(marker);
    h->GetYaxis()->SetTitle(yt);
    h->GetYaxis()->SetTitleOffset(1.6);
    h->GetXaxis()->SetTitle(xt);
}

void SetHist(TH1* h, int color = 1)
{
    h->SetLineWidth(3);
    h->SetLineColor(color);
    h->SetMarkerColor(color);
    h->SetMarkerSize(1);
}

void SetLeg(TLegend* l,float txtsize=0.03){
    l->SetBorderSize(0);
    l->SetFillColor(0);
    l->SetTextSize(txtsize);
}

void ToyBackGround(){
    set_plot_style();
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    
    int Nevents = 1000;
    int Ntracks = 1000;
    float pT0 = 0.500; //GeV
    //f(pT)dpT =Cexp(-pT/pT,0)dpT so C = 1/pT,0
    
    TH1* hpT = new TH1F("hpT","",100,0,20);
    TH1* hPhi = new TH1F("hPhi","",20,0,2*TMath::Pi());
    TH1* hEta = new TH1F("hEta","",20,-1.5,1.5);
    SetHist(hpT,"p_{T,track} (GeV/c)","#frac{d^{2}N}{2#pidp_{T}d#eta}",1);
    hpT->GetYaxis()->SetTitleOffset(1.8);
    SetHist(hPhi,"#phi","#frac{dN}{d#phi}");
    SetHist(hEta,"#eta","#frac{dN}{d#eta}");
    
    TRandom3* random = new TRandom3;
    random->SetSeed(5000);
    
    double rand; double pT; double phi; double eta;
    for (int ie = 0; ie < Nevents; ie++){
    for (int i = 0;i<Ntracks;i++){
        rand = random->Rndm();
        pT = -pT0*log(1 - rand);
        hpT->Fill(pT);
        rand = random->Rndm();
        phi = TMath::Pi()*2.*rand;
        hPhi->Fill(phi);
        rand = random->Rndm();
        eta = 2*(rand-0.5);
        hEta->Fill(eta);
        TLorentzVector* t = new TLorentzVector();
        t->SetPtEtaPhiM(pT,eta,phi,0);
        
        //t->Print();
        //cout<<t<<endl;
    }
    }
    hpT->Sumw2();
    hpT->Scale(1./(2*TMath::Pi()*hpT->GetBinWidth(1)*2.*Nevents));
    for (int i = 1;i<=hpT->GetNbinsX();i++){
        float y = hpT->GetBinContent(i);
        float yerr = hpT->GetBinError(i);
        float pTbin = hpT->GetBinCenter(i);
        hpT->SetBinContent(i,y/pTbin);
        hpT->SetBinError(i,yerr/pTbin);
    }
    hPhi->Sumw2();
    hPhi->Scale(1.0/(Nevents*hPhi->GetBinWidth(1)));
    hEta->Sumw2();
    hEta->Scale(1.0/(Nevents*hEta->GetBinWidth(1)));
    
    TCanvas* can1 = new TCanvas("can1","",1200,400);
    can1->Divide(3,1);
    can1->cd(1);
    gPad->SetLeftMargin(0.15);
    gPad->SetLogy();
    hpT->Draw("ehist");
    can1->cd(2);
    gPad->SetLeftMargin(0.15);
    hPhi->SetMinimum(0);
    hPhi->Draw("ehist");
    can1->cd(3);
    gPad->SetLeftMargin(0.15);
    hEta->Draw("ehist");
    can1->SaveAs("BackgroundOnly.eps");
}

