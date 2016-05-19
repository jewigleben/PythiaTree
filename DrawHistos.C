void DrawHistos(TString infile = "histos.root"){
    set_plot_style();
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    
    TFile *_file0 = TFile::Open(infile);
    TH1* hHardPartonspT = (TH1*)gROOT->FindObject("hHardPartonspT");
    TH1* hHardPartonsName = (TH1*)gROOT->FindObject("hHardPartonsName");
    TH1* hParticlepT = (TH1*)gROOT->FindObject("hParticlepT");
    TH2* hParticleEtaPhi = (TH2*)gROOT->FindObject("hParticleEtaPhi");

    TCanvas* can1 = new TCanvas("can1","",600,600);
    gPad->SetLeftMargin(0.15);
    gPad->SetLogy();
    SetHist(hParticlepT,"p_{T} (GeV/c)","Counts",2);
    SetHist(hHardPartonspT,"p_{T} (GeV/c)","Counts",1);
    hParticlepT->Draw();
    hHardPartonspT->Draw("same");
    TLegend* leg1 = new TLegend(0.6,0.7,0.85,0.85);
    SetLeg(leg1);
    leg1->AddEntry(hHardPartonspT,"Hard Parton","L");
    leg1->AddEntry(hParticlepT,"Final Particles","L");
    leg1->Draw();
    can1->SaveAs("pTgraph.png");
    
    TCanvas* can2 = new TCanvas("can2","",600,600);
    gPad->SetLeftMargin(0.15);
    SetHist(hHardPartonsName,"parton ID","Counts",1);
    hHardPartonsName->Draw();
    
    TCanvas* can3 = new TCanvas("can3","",600,600);
    gPad->SetLeftMargin(0.15);
    SetHist(hParticleEtaPhi,"p_{T} (GeV/c)","Counts",1);
    hParticleEtaPhi->Draw("COLZ");
   
    
  }

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
void SetHist(TH1* h, char* xt ="", char* yt ="",int color = 1, int marker = 20,int width = 3, float size = 1.0)
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
