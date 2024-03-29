#include <iostream>
#include "NikiControllerX.C"
#include "TLine.h"
#include "TCut.h"
#include "TCanvas.h"
#include "TStyle.h"

void CheckKPBoundary(Int_t kpmin, Int_t kpmax) {
  gStyle->SetOptStat("irmen");
  const TString rootfile_num    = "20210714000204";
  const TString gatenetlog_name = "../gatenetlog/gatenetlog_scan20210713_x_m2_scan_1.txt";
  const TString rootfile_name   = Form("%s_list.root", rootfile_num.Data());
  TCut cut_rpmt = "f==4";
  TCut cut_bm   = "ch==4";
  TCut cut_tof  = "tof>1.0e3";
  const Double_t xmax      = 128.0;
  const Double_t ymax      = 128.0;
  const Double_t tmax      = 40.0;
  // const Int_t kp_lag       = 10;
  // const Int_t kp_kill_head = 10;
  // const Int_t kp_kill_tail = 10;
  const Int_t kp_lag       = 5;
  const Int_t kp_kill_head = 3;
  const Int_t kp_kill_tail = 2;
  std::vector<Int_t> kp_head_v, kp_tail_v;
  // TTree *tree_whole = NikiControllerX::GetWholeTree(rootfile_name, "Six");
  TTree *tree_whole = NikiControllerX::GetWholeTree(rootfile_name, "T");
  std::cout << "GetWholeTree done" << std::endl;
  NikiControllerX::GetAdjustedKPVectors(gatenetlog_name,
					kp_kill_head, kp_kill_tail, kp_lag,
					kp_head_v, kp_tail_v);
  TObjArray *arr_line_kp = new TObjArray();
  for (size_t i = 0; i < kp_head_v.size(); i++) {
    if (kp_head_v.at(i) < kpmin) continue;
    if (kp_tail_v.at(i) > kpmax) continue;
    std::cout << i << ":" << kp_head_v.at(i) << " " << kp_tail_v.at(i) << std::endl;
    // TLine *line_kp_head = new TLine(kp_head_v.at(i), 0.,
    // 				    kp_head_v.at(i), tmax);
    // TLine *line_kp_tail = new TLine(kp_tail_v.at(i), 0.,
    // 				    kp_tail_v.at(i), tmax);
    TLine *line_kp_head = new TLine(kp_head_v.at(i), 0.,
				    kp_head_v.at(i), 100);
    TLine *line_kp_tail = new TLine(kp_tail_v.at(i), 0.,
				    kp_tail_v.at(i), 100);
    line_kp_head->SetLineColor(kBlack);
    line_kp_tail->SetLineColor(kRed);
    arr_line_kp->Add(line_kp_head);
    arr_line_kp->Add(line_kp_tail);
  }


  TCanvas *c = new TCanvas("c", "Canvas", 800, 1200);
  c->Divide(1,2);
  c->cd(1);
  tree_whole->Draw(Form("x*%f:kp>>h1(%d, %d, %d, 128,0,%f)",
    		xmax, (kpmax-kpmin)/10, kpmin, kpmax, xmax),
    	   cut_rpmt && cut_tof && Form("kp>=%d", kpmin) &&Form("kp<=%d", kpmax),
    	   "colz");
  // tree_whole->Draw(Form("tof*1e-3:kp>>h1(%d, %d, %d, 200,0,40)",
  // 			(kpmax-kpmin)/10, kpmin, kpmax),
  // 			// (kpmax-kpmin)/100, kpmin, kpmax),
  //   	   cut_bm && cut_tof && Form("kp>=%d", kpmin) &&Form("kp<=%d", kpmax),
  //   	   "colz");
  for (Int_t i = 0; i < arr_line_kp->GetEntries(); i++) {
    TLine *line = (TLine*)arr_line_kp->At(i);
    line->Draw();
  }
  c->cd(2);
  tree_whole->Draw(Form("kp-%d>>h2(%d, %d, %d)",
                        kpmin, (kpmax-kpmin)/10, 0, kpmax-kpmin),
                   cut_rpmt && cut_tof && Form("kp>=%d", kpmin) &&Form("kp<=%d", kpmax),
                   "eh");
  // tree_whole->Draw(Form("kp-%d>>h2(%d, %d, %d)",
  //                       kpmin, (kpmax-kpmin)/10, 0, kpmax-kpmin),
  //                  cut_bm && cut_tof && Form("kp>=%d", kpmin) &&Form("kp<=%d", kpmax),
  //                  "eh");
  c->cd(2)->Update();
  // for (Int_t i = 0; i < arr_line_kp->GetEntries(); i++) {
  //   TLine *line_kp_head = new TLine(kp_head_v.at(i)-kpmin, gPad->GetUymin(),
  // 				    kp_head_v.at(i)-kpmin, gPad->GetUymax());
  //   TLine *line_kp_tail = new TLine(kp_tail_v.at(i)-kpmin, gPad->GetUymin(),
  // 				    kp_tail_v.at(i)-kpmin, gPad->GetUymax());
  //   line_kp_head->SetLineColor(kBlack);
  //   line_kp_tail->SetLineColor(kRed);
  //   line_kp_head->Draw();
  //   line_kp_tail->Draw();
  // }
  c->Update();
}

void CheckKPBoundary() {
  // CheckKPBoundary(    0.0,  1.00E4);
  CheckKPBoundary( 1e4, 1.5e4);
}
