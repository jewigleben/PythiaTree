void DrawEfficiencyError(){
    set_plot_style();
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    int fillcol = TColor::kCyan-6;
    int fillstyle = 1001;
    int linecolor = TColor::kCyan+3;
    int markerstyle = 1;
    float legfontsize = 0.04;
    
   
    TH1* num = new TH1F("num","num",10,0,10);
    TH1* den = new TH1F("den","den",10,0,10);
   
    
    for (int i = 1;i<11;i++){
        num->SetBinContent(i,(i-1)*10);
        den->SetBinContent(i,100);
    }
    
    TH1* hDummy = new TH1F("hDummy","",10,0,10);  //dummy histo for axis etc
    hDummy->SetMinimum(0);
    hDummy->SetMaximum(1);
    TCanvas* can1 = new TCanvas("can1","",600,600);
    gPad->SetLeftMargin(0.13);
    gPad->SetTickx();
    gPad->SetTicky();
    TEfficiency* pEff = new TEfficiency(*num,*den);
    SetHist(hDummy,"x axis (units)","y axis (units)");
    SetHist(pEff,linecolor,markerstyle);
    
    pEff->SetFillColor(fillcol);
    pEff->SetFillStyle(fillstyle);
    
    hDummy->Draw("P");
    pEff->Draw("E3Same");
    pEff->Draw("LX0Same");
    
    TLegend* leg1 = new TLegend(0.2,0.65,0.6,0.85);
    SetLeg(leg1,legfontsize);
    leg1->AddEntry(hDummy,"The Plot Title","");
    leg1->AddEntry(hDummy,"Other key info","");
    leg1->Draw();
    
    TLegend* leg2 = new TLegend(0.5,0.25,0.85,0.4);
    SetLeg(leg2,0.03);
    leg2->AddEntry(hDummy,"Even more info","");
    leg2->AddEntry(pEff,"Plot line style","L");
    leg2->Draw();
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
    h->GetXaxis()->SetTitleOffset(1.2);
    h->GetXaxis()->SetTitle(xt);
}

void SetHist(TEfficiency* h, int color = 1, int marker = 20,int width = 3, float size = 1.0)
{
    h->SetLineWidth(width);
    h->SetLineColor(color);
    h->SetMarkerColor(color);
    h->SetMarkerSize(size);
    h->SetMarkerStyle(marker);
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
