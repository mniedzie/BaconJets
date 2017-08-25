#include "../include/parameters.h"
#include "../include/useful_functions.h"
#include "../include/CorrectionObject.h"
#include "../include/tdrstyle_mod15.h"

#include <TStyle.h>
#include <TF1.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TLine.h>
#include <TCanvas.h>
#include <TMatrixDSym.h>
#include <TPaveStats.h>
#include <TFitResult.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TVirtualFitter.h>
#include <TMath.h>
#include <TFile.h>


using namespace std;

void CorrectionObject::GenResponsePlots(){
  cout << "--------------- Starting GenResponsePlots() ---------------" << endl << endl;
  gStyle->SetOptStat(0);
  TString flavorLabel = "";
  //Table with number of events in each pT- and eta-bin
  
  //Set up histos for ratios of responses
  // TH1D *hmc_response_probejet[n_pt-1][n_eta-1];   // <pTreco>/<pTgen> for tag&probe jet matched to GEN jets
  // TH1D *hmc_response_probetagjet[n_pt-1][n_eta-1];// <pT,probe,gen>/<pT,tag,gen> for tag&probe jet matched to GEN jets
  // TH1D *hmc_response_probetagreco[n_pt-1][n_eta-1];// <pT,probe,RECO>/<pT,tag,RECO> for tag&probe jet matched to GEN jets
  // TH1D *hmc_response_probetagparton[n_pt-1][n_eta-1];// <pT,probe,parton>/<pT,tag,parton> for tag&probe jet matched to PARTONS
  // TH1D *hmc_response_tagtagjet[n_pt-1][n_eta-1];// <pT,tag,RECO>/<pT,tag,gen> for tag&probe jet matched to GEN jets

  TH1D *hmc_A[n_pt-1][n_eta-1];   // Assymetry_RECO tag&probe jet matched to GEN jets
  TH1D *hmc_A_GEN[n_pt-1][n_eta-1];   // Assymetry_GEN tag&probe jet matched to GEN jets
  TH1D *hmc_A_PARTON[n_pt-1][n_eta-1];   // Assymetry_PARTON tag&probe jet matched to GEN jets

  TH1I *hmc_jet1_genID[n_pt-1][n_eta-1];// genID for the 1st jet
  TH1I *hmc_jet2_genID[n_pt-1][n_eta-1];// genID for the 1st jet
  TH1I *hmc_jet3_genID[n_pt-1][n_eta-1];// genID for the 1st jet

  TH1D *hmc_probejetpt_flavor[3][n_eta-1];// probe jet pt separated by flavor, 0 = not matched, 1 = quark, 2 = gluon
  TH1D *hmc_tagjetpt_flavor[3][n_eta-1];// tag jet pt separated by flavor, 0 = not matched, 1 = quark, 2 = gluon
  TH1I *hmc_QQevents[n_pt-1][n_eta-1];// number of events with QQ(tag-probe) 
  TH1I *hmc_GGevents[n_pt-1][n_eta-1];// number of events with GG(tag-probe) 
  TH1I *hmc_QGevents[n_pt-1][n_eta-1];// number of events with QG(tag-probe) 
  TH1I *hmc_GQevents[n_pt-1][n_eta-1];// number of events with GQ(tag-probe) 
  TH1D *hmc_normjetpt[n_pt-1][n_eta-1];//  binning variable used for normalisation
  TH1D *hmc_probejetpt[n_pt-1][n_eta-1];// RECO probe jet pt devided to binning variable, e.g <pT,probe,RECO> = <pT,probe,RECO/pT,ave,RECO> * <pT,ave,RECO>
  TH1D *hmc_probegenjetpt[n_pt-1][n_eta-1];// GEN probe jet pt devided to binning variable, e.g <pT,probe,GEN> = <pT,probe,GEN/pT,ave,RECO> * <pT,ave,RECO>
  TH1D *hmc_tagjetpt[n_pt-1][n_eta-1];// RECO tag jet pt devided to binning variable, e.g <pT,tag,RECO> = <pT,tag,RECO/pT,ave,RECO> * <pT,ave,RECO>
  TH1D *hmc_taggenjetpt[n_pt-1][n_eta-1];// GEN tag jet pt devided to binning variable, e.g <pT,tag,GEN> = <pT,tag,GEN/pT,ave,RECO> * <pT,ave,RECO>
  TH1D *hmc_tagpartonjetpt[n_pt-1][n_eta-1];// PARTON tag jet pt devided to binning variable, e.g <pT,tag,PARTON> = <pT,tag,PARTON/pT,ave,RECO> * <pT,ave,RECO>
  TH1D *hmc_probepartonjetpt[n_pt-1][n_eta-1];// PARTON probe jet pt devided to binning variable, e.g <pT,probe,PARTON> = <pT,probe,PARTON/pT,ave,RECO> * <pT,ave,RECO>
  int count = 0;
 
  /*  TString name3 = "hist_mc_genresponse_probe_";
  TString name4 = "hist_mc_genresponse_probetag_";
  TString name10 = "hist_mc_genresponse_tagtag_";
  TString name5 = "hist_mc_genresponse_probetag_parton_";
  TString name2 = "hist_mc_genresponse_probetag_reco_";*/

  TString name11 = "hist_mc_A_";
  TString name12 = "hist_mc_A_GEN_";
  TString name13 = "hist_mc_A_PARTON_";

  TString name6 = "hist_mc_jet1_genID";
  TString name7 = "hist_mc_jet2_genID";
  TString name8 = "hist_mc_jet3_genID";

  TString name9 = "hist_mc_probejetpt_flavor";
  TString name91 = "hist_mc_tagjetpt_flavor";

  TString name99 = "hist_mc_normpt_";
  TString name100 = "hist_mc_probejetrecopt_";
  TString name101 = "hist_mc_probejetgenpt_";
  TString name102 = "hist_mc_tagjetrecopt_";
  TString name103 = "hist_mc_tagjetgenpt_";

  TString name104 = "hist_mc_probejetpartonpt_";
  TString name105 = "hist_mc_tagjetpartonpt_";

  TString name106 = "hist_mc_QQevents_";
  TString name107 = "hist_mc_GGevents_";
  TString name108 = "hist_mc_QGevents_";
  TString name109 = "hist_mc_GQevents_";

  for(int j=0; j<n_eta-1; j++){
      TString eta_name = "eta_"+eta_range2[j]+"_"+eta_range2[j+1];
    for(int k=0; k<n_pt-1; k++){
      TString pt_name = "pt_"+pt_range[k]+"_"+pt_range[k+1];
      TString name;
      /*      TString name = name3 + eta_name + "_" + pt_name;
      hmc_response_probejet[k][j] = new TH1D(name,"",nResponseBins, -10.0, 10.0);
      name = name4 + eta_name + "_" + pt_name;
      hmc_response_probetagjet[k][j] = new TH1D(name,"",nResponseBins, -10.0, 10.0);
      name = name10 + eta_name + "_" + pt_name;
      hmc_response_tagtagjet[k][j] = new TH1D(name,"",nResponseBins, -10.0, 10.0);
      name = name5 + eta_name + "_" + pt_name;
      hmc_response_probetagparton[k][j] = new TH1D(name,"",nResponseBins, -10.0, 10.0);
      name = name2 + eta_name + "_" + pt_name;
      hmc_response_probetagreco[k][j] = new TH1D(name,"",nResponseBins, -10.0, 10.0);*/

      name = name11 + eta_name + "_" + pt_name;
      hmc_A[k][j] = new TH1D(name,"",nResponseBins, -2.0, 2.0);
      name = name12 + eta_name + "_" + pt_name;
      hmc_A_GEN[k][j] = new TH1D(name,"",nResponseBins, -2.0, 2.0);
      name = name13 + eta_name + "_" + pt_name;
      hmc_A_PARTON[k][j] = new TH1D(name,"",nResponseBins, -2.0, 2.0);

      name = name99 + eta_name + "_" + pt_name;
      hmc_normjetpt[k][j] = new TH1D(name,"",250, 0, 1000);
      name = name100 + eta_name + "_" + pt_name;
      hmc_probejetpt[k][j] = new TH1D(name,"",nResponseBins, 0.0, 2.0);
      name = name101 + eta_name + "_" + pt_name;
      hmc_probegenjetpt[k][j] = new TH1D(name,"",nResponseBins, 0.0, 2.0);
      name = name102 + eta_name + "_" + pt_name;
      hmc_tagjetpt[k][j] = new TH1D(name,"",nResponseBins, 0.0, 2.0);
      name = name103 + eta_name + "_" + pt_name;
      hmc_taggenjetpt[k][j] = new TH1D(name,"",nResponseBins, 0.0, 2.0);
      name = name105 + eta_name + "_" + pt_name;
      hmc_tagpartonjetpt[k][j] = new TH1D(name,"",nResponseBins, 0.0, 2.0);
      name = name104 + eta_name + "_" + pt_name;
      hmc_probepartonjetpt[k][j] = new TH1D(name,"",nResponseBins, 0.0, 2.0);

      name = name6 + eta_name + "_" + pt_name;
      hmc_jet1_genID[k][j] = new TH1I(name,"",15,-5,10);
      name = name7 + eta_name + "_" + pt_name;
      hmc_jet2_genID[k][j] = new TH1I(name,"",15,-5,10);
      name = name8 + eta_name + "_" + pt_name;
      hmc_jet3_genID[k][j] = new TH1I(name,"",15,-5,10);
      name = name106 + eta_name + "_" + pt_name;    
      hmc_QQevents[k][j] = new TH1I(name,"",10, 0,10);
      name = name107 + eta_name + "_" + pt_name;    
      hmc_GGevents[k][j] = new TH1I(name,"",10, 0,10);
      name = name108 + eta_name + "_" + pt_name;    
      hmc_QGevents[k][j] = new TH1I(name,"",10, 0,10);
      name = name109 + eta_name + "_" + pt_name;    
      hmc_GQevents[k][j] = new TH1I(name,"",10, 0,10);
      //      count++;
    }

    TString name = name9 + eta_name;
    for(int ifl=0;ifl<3;ifl++){
      if(ifl==0) name +="_notmatched";
      if(ifl==1) name +="_quark";
      if(ifl==2) name +="_gluon";
      hmc_probejetpt_flavor[ifl][j] = new TH1D(name,"",100,0,1000);
    }

    name = name91 + eta_name;
    for(int ifl=0;ifl<3;ifl++){
      if(ifl==0) name +="_notmatched";
      if(ifl==1) name +="_quark";
      if(ifl==2) name +="_gluon";
      hmc_tagjetpt_flavor[ifl][j] = new TH1D(name,"",100,0,1000);
    }

  }



  //Get relevant information from MC, loop over MC events 
  TTreeReader myReader_MC("AnalysisTree", CorrectionObject::_MCFile);
  TTreeReaderValue<Float_t> pt_ave_mc(myReader_MC, "pt_ave");
  TTreeReaderValue<Float_t> probejet_eta_mc(myReader_MC, "probejet_eta");
  TTreeReaderValue<Float_t> probejet_pt_mc(myReader_MC, "probejet_pt");
  TTreeReaderValue<Float_t> barreljet_pt_mc(myReader_MC, "barreljet_pt");
  TTreeReaderValue<Float_t> alpha_mc(myReader_MC, "alpha");
  TTreeReaderValue<Float_t> weight_mc(myReader_MC, "weight");

  TTreeReaderValue<Float_t> probejet_ptgen_mc(myReader_MC, "probejet_ptgen");
  TTreeReaderValue<Float_t> barreljet_ptgen_mc(myReader_MC, "barreljet_ptgen");

  TTreeReaderValue<Float_t> probejet_ptparton_mc(myReader_MC, "probejet_ptptcl");
  TTreeReaderValue<Float_t> barreljet_ptparton_mc(myReader_MC, "barreljet_ptptcl");

  TTreeReaderValue<Int_t> jet1_genID_mc(myReader_MC, "jet1_genID");
  TTreeReaderValue<Int_t> jet2_genID_mc(myReader_MC, "jet2_genID");
  TTreeReaderValue<Int_t> jet3_genID_mc(myReader_MC, "jet3_genID");

  TTreeReaderValue<Int_t> flavorProbejet_mc(myReader_MC, "flavorProbejet");
  TTreeReaderValue<Int_t> flavorTagjet_mc(myReader_MC, "flavorBarreljet");

  int icount=0;

  TString pt_binning_var_str = "#bar{p}^{RECO}_{T} [GeV]";//bin in pt_ave, RECO
  TString pt_binning_var_name = "__pT_ave_RECO__";//bin in pt_ave, RECO

  // TString pt_binning_var_str = "p^{tag,GEN}_{T} [GeV]";//bin in pt_tag, GEN
  // TString pt_binning_var_name = "__pT_tag_GEN__";//bin in pt_tag, GEN

  // TString pt_binning_var_str = "p^{probe,GEN}_{T} [GeV]";//bin in pt_probe, GEN
  // TString pt_binning_var_name = "__pT_probe_GEN__";//bin in pt_probe, GEN

  // TString pt_binning_var_str = "#bar{p}^{GEN}_{T} [GeV]";//bin in pt_ave, GEN
  // TString pt_binning_var_name = "__pT_ave_GEN__";//bin in pt_ave, GEN


  while (myReader_MC.Next()) {
  //  while (myReader_MC.Next() && icount<1e6) {
        double pt_binning_var = *pt_ave_mc;//bin in pt_ave, RECO
    //    double pt_binning_var = *barreljet_ptgen_mc;//bin in pt_tag, GEN
    //    double pt_binning_var = *probejet_ptgen_mc;//bin in pt_probe, GEN
    //    double pt_binning_var = 0.5*(*barreljet_ptgen_mc+*probejet_ptgen_mc);//bin in pt_ave, GEN
    if(*alpha_mc>alpha_cut) continue;
    //fill histos in bins of pt and eta
    for(int k=0; k<n_pt-1; k++){
      if(pt_binning_var<pt_bins[k] || pt_binning_var>pt_bins[k+1]) continue;
      for(int j=0; j<n_eta-1; j++){
	if(fabs(*probejet_eta_mc)>eta_bins[j+1] || fabs(*probejet_eta_mc)<eta_bins[j]) continue;
	else{
	  //	  bool matched;
	  if(*probejet_ptgen_mc<0 || *barreljet_ptgen_mc<0){ //not matched
	    //	    matched = false;
	  }
	  else{ //matched

	    // ///[BEGIN] Selection according to flavor of tag&probe jets---------------
	    bool flavor_sel=false;
	    if(*flavorProbejet_mc>0 && *flavorProbejet_mc<6 && *flavorTagjet_mc>0 && *flavorTagjet_mc<6){
	      hmc_QQevents[k][j]->Fill(1,*weight_mc);
	      // flavorLabel = "QQ";
	      // flavor_sel=true;//QQ
	    }
	    if(*flavorTagjet_mc==21 && *flavorProbejet_mc>0 && *flavorProbejet_mc<6 ){
	      hmc_GQevents[k][j]->Fill(1,*weight_mc);
	      flavorLabel = "GQ";
	      flavor_sel=true;//GQ
	    }
	    if(*flavorTagjet_mc==21 && *flavorProbejet_mc==21){
	      hmc_GGevents[k][j]->Fill(1,*weight_mc);
	      // flavorLabel = "GG";
	      // flavor_sel=true;//GG
	    }
	    if(*flavorTagjet_mc>0 && *flavorTagjet_mc<6 && *flavorProbejet_mc==21){
	      hmc_QGevents[k][j]->Fill(1,*weight_mc);
	      // flavorLabel = "QG";
	      // flavor_sel=true;//QG
	    }

	    if(!flavor_sel) continue;
	    // ///[END] Selection according to flavor of tag&probe jets---------------

	    ///// flavor ////
	  if(*flavorProbejet_mc<0){//not matched
	    hmc_probejetpt_flavor[0][j]->Fill(*probejet_pt_mc,*weight_mc);
	  }
	  else{
	    if(*flavorProbejet_mc>0 && *flavorProbejet_mc<6){
	      hmc_probejetpt_flavor[1][j]->Fill(*probejet_pt_mc,*weight_mc);
	    }
	    else{
	      hmc_probejetpt_flavor[2][j]->Fill(*probejet_pt_mc,*weight_mc);
	    }
	  }
	  if(*flavorTagjet_mc<0){
	    hmc_tagjetpt_flavor[0][j]->Fill(*barreljet_pt_mc,*weight_mc);
	  }
	  else{
	    if(*flavorTagjet_mc>0 && *flavorTagjet_mc<6){
	      hmc_tagjetpt_flavor[1][j]->Fill(*barreljet_pt_mc,*weight_mc);
	    }
	    else{
	      hmc_tagjetpt_flavor[2][j]->Fill(*barreljet_pt_mc,*weight_mc);
	    }
	  }
	  ///// [END] flavor /////
	  hmc_normjetpt[k][j]->Fill(pt_binning_var,*weight_mc);

	  double probejetpt_norm = (*probejet_pt_mc)/pt_binning_var;
	  hmc_probejetpt[k][j]->Fill(probejetpt_norm,*weight_mc);
	  double probegenjetpt_norm = (*probejet_ptgen_mc)/pt_binning_var;
	  hmc_probegenjetpt[k][j]->Fill(probegenjetpt_norm,*weight_mc);
	  double probepartonjetpt_norm = (*probejet_ptparton_mc)/pt_binning_var;
	  hmc_probepartonjetpt[k][j]->Fill(probepartonjetpt_norm,*weight_mc);

	  double tagjetpt_norm = (*barreljet_pt_mc)/pt_binning_var;
	  hmc_tagjetpt[k][j]->Fill(tagjetpt_norm,*weight_mc);
	  double taggenjetpt_norm = (*barreljet_ptgen_mc)/pt_binning_var;
	  hmc_taggenjetpt[k][j]->Fill(taggenjetpt_norm,*weight_mc);
	  double tagpartonjetpt_norm = (*barreljet_ptparton_mc)/pt_binning_var;
	  hmc_tagpartonjetpt[k][j]->Fill(tagpartonjetpt_norm,*weight_mc);



	  /*	  double genresponse = (*probejet_pt_mc)/(*probejet_ptgen_mc);
	  hmc_response_probejet[k][j]->Fill(genresponse,*weight_mc);
	  double gentagproberesponse = (*probejet_ptgen_mc)/(*barreljet_ptgen_mc);
	  hmc_response_probetagjet[k][j]->Fill(gentagproberesponse,*weight_mc);
	  double gentagprobepartonresponse = (*probejet_ptparton_mc)/(*barreljet_ptparton_mc);
	  hmc_response_probetagparton[k][j]->Fill(gentagprobepartonresponse,*weight_mc);
	  double recoresponse = (*probejet_pt_mc)/(*barreljet_pt_mc);
	  hmc_response_probetagreco[k][j]->Fill(recoresponse,*weight_mc);
	  double tagtagresponse = (*barreljet_pt_mc)/(*barreljet_ptgen_mc);
	  //	    cout<<"barreljet_pt = "<<*barreljet_pt_mc<<" barreljet_ptgen = "<<*barreljet_ptgen_mc<<" tagtagresponse = "<<tagtagresponse<<endl;
	  hmc_response_tagtagjet[k][j]->Fill(tagtagresponse,*weight_mc);*/
	  double assymetry = ((*probejet_pt_mc)-(*barreljet_pt_mc))/((*probejet_pt_mc)+(*barreljet_pt_mc));
	  hmc_A[k][j]->Fill(assymetry,*weight_mc);

	  double assymetry_GEN = ((*probejet_ptgen_mc)-(*barreljet_ptgen_mc))/((*probejet_ptgen_mc)+(*barreljet_ptgen_mc));
	  hmc_A_GEN[k][j]->Fill(assymetry_GEN,*weight_mc);
	  double assymetry_PARTON = ((*probejet_ptparton_mc)-(*barreljet_ptparton_mc))/((*probejet_ptparton_mc)+(*barreljet_ptparton_mc));
	  hmc_A_PARTON[k][j]->Fill(assymetry_PARTON,*weight_mc);

	  //	    cout<<"probe_gen = "<<(*probejet_ptgen_mc)<<" barrel_gen = "<<(*barreljet_ptgen_mc)<<endl;
	  //	    cout<<"genresponse = "<<genresponse<<" gentagproberesponse = "<<gentagproberesponse<<endl;
	  }
	  int jet1_genID_mc_val = *jet1_genID_mc;
	  if(jet1_genID_mc_val>-1){
	    jet1_genID_mc_val++;
	    hmc_jet1_genID[k][j]->Fill(jet1_genID_mc_val-1,*weight_mc);
	  }
	  int jet2_genID_mc_val = *jet2_genID_mc;
	  if(jet2_genID_mc_val>-1){
	    jet2_genID_mc_val++;
	    hmc_jet2_genID[k][j]->Fill(jet2_genID_mc_val-2,*weight_mc);
	  }

	  int jet3_genID_mc_val = *jet3_genID_mc;
	  if(jet3_genID_mc_val>-1){
	    jet3_genID_mc_val++;
	    hmc_jet3_genID[k][j]->Fill(jet3_genID_mc_val-3,*weight_mc);
	  }
	  //	  cout<<""<<jet1_genID_mc_val<<" "<<jet2_genID_mc_val<<" "<<jet3_genID_mc_val<<endl;
	 
	}
      }
    }
    icount++;
  } 

  /*  ofstream output;
  output.open(CorrectionObject::_outpath+"plots/control/GenResponse_Number_Events_Pt_Eta_bins_"+CorrectionObject::_generator_tag+"_"+CorrectionObject::_jettag+".txt");
    

  output << "Number of matched events in each bin for MC" << endl;
  output << "|Eta|:          ";
  double n_tot_MC = 0;
  for(int i=0; i<n_eta; i++) {
    if(i != n_eta-1) output << eta_range[i] << " -- ";
    else output << eta_range[i] << endl;
  }
  for(int i=0; i<n_pt-1; i++){
    if(i==0) output << "pT = ["  << fixed << setprecision(0) << pt_bins[i] << "," << setprecision(0) << pt_bins[i+1] << "]  :    ";
    else if(i==1) output << "pT = ["  << fixed << setprecision(0) << pt_bins[i] << "," << setprecision(0) << pt_bins[i+1] << "] :    ";
    else output << "pT = ["  << fixed << setprecision(0) << pt_bins[i] << "," << setprecision(0) << pt_bins[i+1] << "]:    ";

    for(int j=0; j<n_eta-1; j++){
      if(j!=n_eta-2){
	if(hmc_response_probejet[i][j]->GetEntries()/1000 < 0.01)     output << hmc_response_probejet[i][j]->GetEntries() << "      - "; //<1000
	else if(hmc_response_probejet[i][j]->GetEntries()/1000 < 0.1) output << hmc_response_probejet[i][j]->GetEntries() << "     - "; //<1000
	else if(hmc_response_probejet[i][j]->GetEntries()/1000 < 1)   output << hmc_response_probejet[i][j]->GetEntries() << "    - "; //<1000
	else if(hmc_response_probejet[i][j]->GetEntries()/1000 <10)   output << hmc_response_probejet[i][j]->GetEntries() << "   - "; //<10000
	else if(hmc_response_probejet[i][j]->GetEntries()/1000 <100)  output << hmc_response_probejet[i][j]->GetEntries() << "  - ";
	else                                              output << hmc_response_probejet[i][j]->GetEntries() << " - ";
      }
      else output << hmc_response_probejet[i][j]->GetEntries() << endl;
      n_tot_MC+= hmc_response_probejet[i][j]->GetEntries();
    }

  }
  output << endl << endl << "Total number of matched events in MC: " << n_tot_MC << endl;
  */
 
 

  // Dump 1-d distributions of A and B in bins of pT, eta

  TFile* test_out_mc_B = new TFile(CorrectionObject::_outpath+"plots/control/GenResponse_1d_mc_matched.root","RECREATE");
  for(int j=0; j<n_eta-1; j++){
    for(int k=0; k<n_pt-1; k++){     ///k=0 n_pt-1 
      /*      hmc_response_probejet[k][j]->Write();
      hmc_response_probetagjet[k][j]->Write();
      hmc_response_probetagparton[k][j]->Write();
      hmc_response_probetagreco[k][j]->Write();
      hmc_response_tagtagjet[k][j]->Write();*/
      hmc_jet1_genID[k][j]->Write();
      hmc_jet2_genID[k][j]->Write();
      hmc_jet3_genID[k][j]->Write();
      hmc_normjetpt[k][j]->Write();
      hmc_probejetpt[k][j]->Write();
      hmc_probegenjetpt[k][j]->Write();
      hmc_probepartonjetpt[k][j]->Write();
      hmc_tagjetpt[k][j]->Write();
      hmc_taggenjetpt[k][j]->Write();
      hmc_tagpartonjetpt[k][j]->Write();
      hmc_A[k][j]->Write();
      hmc_A_GEN[k][j]->Write();
      hmc_A_PARTON[k][j]->Write();
      hmc_QQevents[k][j]->Write();
      hmc_GGevents[k][j]->Write();
      hmc_QGevents[k][j]->Write();
      hmc_GQevents[k][j]->Write();
      for(int ifl=0;ifl<3;ifl++){
	hmc_probejetpt_flavor[ifl][j]->Write();
	hmc_tagjetpt_flavor[ifl][j]->Write();
      }
    }
  }
  test_out_mc_B->Close();
  delete test_out_mc_B;


  //R_MC as a function of pT, in bins of |eta|
  /* double val_rel_mc[n_eta-1][n_pt-1]; //value at pt,eta
  double err_rel_mc[n_eta-1][n_pt-1]; //error of ratio at pt,eta
  double val_tagprobe_mc[n_eta-1][n_pt-1]; //value at pt,eta
  double err_tagprobe_mc[n_eta-1][n_pt-1]; //error of ratio at pt,eta
  double val_tagprobe_parton_mc[n_eta-1][n_pt-1]; //value at pt,eta
  double err_tagprobe_parton_mc[n_eta-1][n_pt-1]; //error of ratio at pt,eta
  double val_tagprobe_reco_mc[n_eta-1][n_pt-1]; //value at pt,eta
  double err_tagprobe_reco_mc[n_eta-1][n_pt-1]; //error of ratio at pt,eta
  double val_tagtag_mc[n_eta-1][n_pt-1]; //value at pt,eta
  double err_tagtag_mc[n_eta-1][n_pt-1]; //error of ratio at pt,eta

  double val_rel_A_mc[n_eta-1][n_pt-1]; //value at pt,eta
  double err_rel_A_mc[n_eta-1][n_pt-1]; //error of ratio at pt,eta*/

  double val_rel_A_mc[n_eta-1][n_pt-1]; //value at pt,eta
  double err_rel_A_mc[n_eta-1][n_pt-1]; //error of ratio at pt,eta

  double val_probejet_pt[n_eta-1][n_pt-1]; //value at pt,eta
  double err_probejet_pt[n_eta-1][n_pt-1]; //value at pt,eta
  double val_probegenjet_pt[n_eta-1][n_pt-1]; //value at pt,eta
  double err_probegenjet_pt[n_eta-1][n_pt-1]; //value at pt,eta
  double val_probepartonjet_pt[n_eta-1][n_pt-1]; //value at pt,eta
  double err_probepartonjet_pt[n_eta-1][n_pt-1]; //value at pt,eta
  double val_tagjet_pt[n_eta-1][n_pt-1]; //value at pt,eta
  double err_tagjet_pt[n_eta-1][n_pt-1]; //value at pt,eta
  double val_taggenjet_pt[n_eta-1][n_pt-1]; //value at pt,eta
  double err_taggenjet_pt[n_eta-1][n_pt-1]; //value at pt,eta
  double val_tagpartonjet_pt[n_eta-1][n_pt-1]; //value at pt,eta
  double err_tagpartonjet_pt[n_eta-1][n_pt-1]; //value at pt,eta

  double val_probeRECO_probeGEN[n_eta-1][n_pt-1];
  double err_probeRECO_probeGEN[n_eta-1][n_pt-1];
  double val_tagRECO_tagGEN[n_eta-1][n_pt-1];
  double err_tagRECO_tagGEN[n_eta-1][n_pt-1];

  double val_probeRECO_tagRECO[n_eta-1][n_pt-1];
  double err_probeRECO_tagRECO[n_eta-1][n_pt-1];

  double val_rel_A_GEN_mc[n_eta-1][n_pt-1]; //value at pt,eta
  double err_rel_A_GEN_mc[n_eta-1][n_pt-1]; //error of ratio at pt,eta
  double val_rel_A_PARTON_mc[n_eta-1][n_pt-1]; //value at pt,eta
  double err_rel_A_PARTON_mc[n_eta-1][n_pt-1]; //error of ratio at pt,eta

  double val_probeGEN_probePARTON[n_eta-1][n_pt-1];
  double err_probeGEN_probePARTON[n_eta-1][n_pt-1];
  double val_tagGEN_tagPARTON[n_eta-1][n_pt-1];
  double err_tagGEN_tagPARTON[n_eta-1][n_pt-1];
  double val_probePARTON_tagPARTON[n_eta-1][n_pt-1];
  double err_probePARTON_tagPARTON[n_eta-1][n_pt-1];
  double val_probeGEN_tagGEN[n_eta-1][n_pt-1];
  double err_probeGEN_tagGEN[n_eta-1][n_pt-1];

  double val_QQ[n_eta-1][n_pt-1];
  double err_QQ[n_eta-1][n_pt-1];
  double val_GG[n_eta-1][n_pt-1];
  double err_GG[n_eta-1][n_pt-1];
  double val_QG[n_eta-1][n_pt-1];
  double err_QG[n_eta-1][n_pt-1];
  double val_GQ[n_eta-1][n_pt-1];
  double err_GQ[n_eta-1][n_pt-1];
  double val_SUM[n_eta-1][n_pt-1];



  for(int i=0; i<n_eta-1; i++){
    for(int j=0; j<n_pt-1; j++){
      hmc_QQevents[j][i]->Print();
      val_QQ[i][j] = hmc_QQevents[j][i]->Integral();
      err_QQ[i][j] = 1e-3;
      val_GG[i][j] = hmc_GGevents[j][i]->Integral();
      err_GG[i][j] = 1e-3;
      val_QG[i][j] = hmc_QGevents[j][i]->Integral();
      err_QG[i][j] = 1e-3;
      val_GQ[i][j] = hmc_GQevents[j][i]->Integral();
      err_GQ[i][j] = 1e-3;

      val_SUM[i][j] = val_QQ[i][j]+val_GG[i][j]+val_QG[i][j]+val_GQ[i][j];
      if(val_SUM[i][j]>1e3){
      val_QQ[i][j] = val_QQ[i][j]/val_SUM[i][j];
      val_GG[i][j] = val_GG[i][j]/val_SUM[i][j];
      val_QG[i][j] = val_QG[i][j]/val_SUM[i][j];
      val_GQ[i][j] = val_GQ[i][j]/val_SUM[i][j];
      if(j>8)
	cout<<"J= "<<j<<" val_QQ[i][j] = "<<val_QQ[i][j]<<"  val_GG[i][j] = "<< val_GG[i][j]<<" val_QG[i][j] = "<<val_QG[i][j]<<" val_GQ[i][j] = "<<val_GQ[i][j]<<endl; 
      }
         //Get <response> and error on <response>
      pair <double,double> A_mc = GetValueAndError(hmc_A[j][i]);
      pair<double,double> res_mc_rel_r;
      res_mc_rel_r.first = (1+A_mc.first)/(1-A_mc.first);
      res_mc_rel_r.second = 2/(pow((1-A_mc.first),2)) * A_mc.second;
      val_rel_A_mc[i][j] = res_mc_rel_r.first;
      err_rel_A_mc[i][j] = res_mc_rel_r.second;

      pair <double,double> A_GEN_mc = GetValueAndError(hmc_A_GEN[j][i]);
      pair<double,double> res_mc_rel_r_GEN;
      res_mc_rel_r_GEN.first = (1+A_GEN_mc.first)/(1-A_GEN_mc.first);
      res_mc_rel_r_GEN.second = 2/(pow((1-A_GEN_mc.first),2)) * A_GEN_mc.second;
      val_rel_A_GEN_mc[i][j] = res_mc_rel_r_GEN.first;
      err_rel_A_GEN_mc[i][j] = res_mc_rel_r_GEN.second;
      pair <double,double> A_PARTON_mc = GetValueAndError(hmc_A_PARTON[j][i]);
      if(hmc_A_PARTON[j][i]->GetEntries()>100)
	A_PARTON_mc.second = 1e-4;
      //       cout<<"A_PARTON = "<<A_PARTON_mc.first<<" +/- "<<A_PARTON_mc.second<<endl;
      pair<double,double> res_mc_rel_r_PARTON;
      res_mc_rel_r_PARTON.first = (1+A_PARTON_mc.first)/(1-A_PARTON_mc.first);
      res_mc_rel_r_PARTON.second = 2/(pow((1-A_PARTON_mc.first),2)) * A_PARTON_mc.second;
      val_rel_A_PARTON_mc[i][j] = res_mc_rel_r_PARTON.first;
      err_rel_A_PARTON_mc[i][j] = res_mc_rel_r_PARTON.second;


      pair <double,double> normpt_mc = GetValueAndError(hmc_normjetpt[j][i]); //<pt_bin> value used for normalisation
      pair <double,double> probejetpt_mc = GetValueAndError(hmc_probejetpt[j][i]);
      val_probejet_pt[i][j] = probejetpt_mc.first*normpt_mc.first;
      err_probejet_pt[i][j] =  ErrorPropagation_AB(probejetpt_mc,normpt_mc);
      pair <double,double> probegenjetpt_mc = GetValueAndError(hmc_probegenjetpt[j][i]);
      val_probegenjet_pt[i][j] = probegenjetpt_mc.first*normpt_mc.first;
      err_probegenjet_pt[i][j] = ErrorPropagation_AB(probegenjetpt_mc,normpt_mc);
      pair <double,double> probepartonjetpt_mc = GetValueAndError(hmc_probepartonjetpt[j][i]);
      val_probepartonjet_pt[i][j] = probepartonjetpt_mc.first*normpt_mc.first;
      err_probepartonjet_pt[i][j] = ErrorPropagation_AB(probepartonjetpt_mc,normpt_mc);

      pair <double,double> tagjetpt_mc = GetValueAndError(hmc_tagjetpt[j][i]);
      val_tagjet_pt[i][j] = tagjetpt_mc.first*normpt_mc.first;
      err_tagjet_pt[i][j] = ErrorPropagation_AB(tagjetpt_mc,normpt_mc);
      pair <double,double> taggenjetpt_mc = GetValueAndError(hmc_taggenjetpt[j][i]);
      val_taggenjet_pt[i][j] = taggenjetpt_mc.first*normpt_mc.first;
      err_taggenjet_pt[i][j] = ErrorPropagation_AB(taggenjetpt_mc,normpt_mc);
      pair <double,double> tagpartonjetpt_mc = GetValueAndError(hmc_tagpartonjetpt[j][i]);
      val_tagpartonjet_pt[i][j] = tagpartonjetpt_mc.first*normpt_mc.first;
      err_tagpartonjet_pt[i][j] = ErrorPropagation_AB(tagpartonjetpt_mc,normpt_mc);

      if(val_probegenjet_pt[i][j]>0){
      val_probeRECO_probeGEN[i][j] = val_probejet_pt[i][j]/val_probegenjet_pt[i][j];
      pair<double,double> tmp1; tmp1.first = val_probejet_pt[i][j]; tmp1.second = err_probejet_pt[i][j];
      pair<double,double> tmp2; tmp2.first = val_probegenjet_pt[i][j]; tmp2.second = err_probegenjet_pt[i][j];
      err_probeRECO_probeGEN[i][j] = ErrorPropagation_AoverB(tmp1,tmp2);
      }
      else{
	val_probeRECO_probeGEN[i][j] = 0;  err_probeRECO_probeGEN[i][j] =0;
      }
      if(val_taggenjet_pt[i][j]>0){
	val_tagRECO_tagGEN[i][j] = val_tagjet_pt[i][j]/val_taggenjet_pt[i][j];
	pair<double,double> tmp1; tmp1.first = val_tagjet_pt[i][j]; tmp1.second = err_tagjet_pt[i][j];
	pair<double,double> tmp2; tmp2.first = val_taggenjet_pt[i][j]; tmp2.second = err_taggenjet_pt[i][j];
	err_tagRECO_tagGEN[i][j] = ErrorPropagation_AoverB(tmp1,tmp2);

      }
      else{
	val_tagRECO_tagGEN[i][j] = 0;  err_tagRECO_tagGEN[i][j] =0;
      }
      if(val_tagjet_pt[i][j]>0){
	val_probeRECO_tagRECO[i][j] = val_probejet_pt[i][j]/val_tagjet_pt[i][j];
	pair<double,double> tmp1; tmp1.first = val_probejet_pt[i][j]; tmp1.second = err_probejet_pt[i][j];
	pair<double,double> tmp2; tmp2.first = val_tagjet_pt[i][j]; tmp2.second = err_tagjet_pt[i][j];
	err_probeRECO_tagRECO[i][j] = ErrorPropagation_AoverB(tmp1,tmp2);

      }
      else{
	val_probeRECO_tagRECO[i][j] = 0;  err_probeRECO_tagRECO[i][j] =0;
      }
      if(val_probepartonjet_pt[i][j]>0){
      val_probeGEN_probePARTON[i][j] = val_probegenjet_pt[i][j]/val_probepartonjet_pt[i][j];
      pair<double,double> tmp1; tmp1.first = val_probegenjet_pt[i][j]; tmp1.second = err_probegenjet_pt[i][j];
      pair<double,double> tmp2; tmp2.first = val_probepartonjet_pt[i][j]; tmp2.second = err_probepartonjet_pt[i][j];
      err_probeGEN_probePARTON[i][j] = ErrorPropagation_AoverB(tmp1,tmp2);
      }
      else{
	val_probeGEN_probePARTON[i][j] = 0;  err_probeGEN_probePARTON[i][j] =0;
      }
      if(val_tagpartonjet_pt[i][j]>0){
      val_tagGEN_tagPARTON[i][j] = val_taggenjet_pt[i][j]/val_tagpartonjet_pt[i][j];
      pair<double,double> tmp1; tmp1.first = val_taggenjet_pt[i][j]; tmp1.second = err_taggenjet_pt[i][j];
      pair<double,double> tmp2; tmp2.first = val_tagpartonjet_pt[i][j]; tmp2.second = err_tagpartonjet_pt[i][j];
      err_tagGEN_tagPARTON[i][j] = ErrorPropagation_AoverB(tmp1,tmp2);
      }
      else{
	val_tagGEN_tagPARTON[i][j] = 0;  err_tagGEN_tagPARTON[i][j] =0;
      }
      if(val_taggenjet_pt[i][j]>0){
	val_probeGEN_tagGEN[i][j] = val_probegenjet_pt[i][j]/val_taggenjet_pt[i][j];
	pair<double,double> tmp1; tmp1.first = val_probegenjet_pt[i][j]; tmp1.second = err_probegenjet_pt[i][j];
	pair<double,double> tmp2; tmp2.first = val_taggenjet_pt[i][j]; tmp2.second = err_taggenjet_pt[i][j];
	err_probeGEN_tagGEN[i][j] = ErrorPropagation_AoverB(tmp1,tmp2);

      }
      else{
	val_probeGEN_tagGEN[i][j] = 0;  err_probeGEN_tagGEN[i][j] =0;
      }
      

      // cout<<" val_probeRECO_probeGEN[i][j] = "<<val_probeRECO_probeGEN[i][j]<<" "<<probejetpt_mc.first<<" "<<normpt_mc.first<<endl;
      // cout<<" err_probeRECO_probeGEN[i][j] = "<<err_probeRECO_probeGEN[i][j]<<" "<<probejetpt_mc.second<<" "<<normpt_mc.second<<endl;
      // cout<<""<<endl;
      //      cout<<" val_tagRECO_tagGEN[i][j] = "<<val_tagRECO_tagGEN[i][j]<<" "<<tagjetpt_mc.first<<" "<<normpt_mc.first<<endl;
      
      // //get <response> and error on <response>
      // pair <double,double> ResGEN_mc = GetValueAndError(hmc_response_probejet[i][j]);
      // val_rel_mc[i][j] = ResGEN_mc.first;
      // err_rel_mc[i][j] = ResGEN_mc.second;

      // //get <response> and error on <response>
      // pair <double,double> Res_mc = GetValueAndError(hmc_response_probetagjet[i][j]);
      // //      cout<<" Res_mc.first = "<<Res_mc.first<<" Res_mc.second = "<<Res_mc.second<<endl;
      // val_tagprobe_mc[i][j] = Res_mc.first;
      // err_tagprobe_mc[i][j] = Res_mc.second;

      // //get <response> and error on <response>
      // pair <double,double> ResPARTON_mc = GetValueAndError(hmc_response_probetagparton[i][j]);
      // val_tagprobe_parton_mc[i][j] = ResPARTON_mc.first;
      // err_tagprobe_parton_mc[i][j] = ResPARTON_mc.second;

      // pair <double,double> ResRECO_mc = GetValueAndError(hmc_response_probetagreco[i][j]);
      // val_tagprobe_reco_mc[i][j] = ResRECO_mc.first;
      // err_tagprobe_reco_mc[i][j] = ResRECO_mc.second;

      // pair <double,double> ResTAG_mc = GetValueAndError(hmc_response_tagtagjet[i][j]);
      // //      cout<<"ResTAG_mc.first = "<<ResTAG_mc.first<<" ResTAG_mc.second = "<<ResTAG_mc.second<<endl;
      // val_tagtag_mc[i][j] = ResTAG_mc.first;
      // err_tagtag_mc[i][j] = ResTAG_mc.second;
      // //      cout<<"val_tagtag_A_mc[i][j] = "<<val_tagtag_A_mc[i][j]<<" err_tagtag_A_mc[i][j] = "<<err_tagtag_A_mc[i][j]<<endl; 
    }
  }

  //dummy for tdrCanvas
  TH1D *h = new TH1D("h",";dummy;",41,0,5.191);
  h->SetMaximum(1.2);
  h->SetMinimum(0.8);

  TH1D *hEF = new TH1D("hEF",";dummy;",1000,0,5.191);

  TCanvas* c_0 = new TCanvas();
  tdrCanvas(c_0,"c_0",h,4,10,true,CorrectionObject::_lumitag);

  
  for(int i=0; i<n_eta-1; i++){
    //Create and fill TGraphErrors
    double xbin_tgraph[n_pt-1];
    double zero[n_pt-1];
    for(int i=0;i<n_pt-1;i++){
      xbin_tgraph[i]=(pt_bins[i]+pt_bins[i+1])/2;
      zero[i]=(pt_bins[i+1]-pt_bins[i])/2 ;
    }

    TString alVal;
    alVal.Form("%0.2f\n",alpha_cut);
    TString altitle = "{#alpha<"+alVal+"}";

    TGraphErrors *graph_rel_A_mc   = new TGraphErrors(n_pt-1, xbin_tgraph, val_rel_A_mc[i], zero, err_rel_A_mc[i]);
    graph_rel_A_mc   = (TGraphErrors*)CleanEmptyPoints(graph_rel_A_mc);
    //    cout<<"graph_rel_A_mc"<<endl;
    //    graph_rel_A_mc->Print();
    graph_rel_A_mc->SetTitle("");
    // graph_rel_A_mc->GetXaxis()->SetTitle(pt_binning_var_str);
    // graph_rel_A_mc->GetXaxis()->SetTitleSize(0.05);
    // graph_rel_A_mc->GetXaxis()->SetTitleOffset(0.80);
    // graph_rel_A_mc->GetXaxis()->SetLimits(30,pt_bins[n_pt-1]+100);
    // graph_rel_A_mc->GetYaxis()->SetRangeUser(0.70,1.30);
    graph_rel_A_mc->SetMarkerColor(kOrange+7);
    graph_rel_A_mc->SetMarkerStyle(29);
    graph_rel_A_mc->SetMarkerSize(1.7);
    graph_rel_A_mc->SetLineColor(kOrange+7);
    TString axistitle_A_mc = "(1+<A_{RECO}>)/(1-<A_{RECO}>)";

    TGraphErrors *graph_probeRECO_probeGEN   = new TGraphErrors(n_pt-1, xbin_tgraph, val_probeRECO_probeGEN[i], zero, err_probeRECO_probeGEN[i]);
    graph_probeRECO_probeGEN   = (TGraphErrors*)CleanEmptyPoints(graph_probeRECO_probeGEN);
    //    cout<<"graph_probeRECO_probeGEN"<<endl;
    //    graph_probeRECO_probeGEN->Print();
    graph_probeRECO_probeGEN->SetTitle("");
    graph_probeRECO_probeGEN->SetMarkerColor(kRed);
    graph_probeRECO_probeGEN->SetMarkerStyle(20);
    graph_probeRECO_probeGEN->SetLineColor(kRed);
    TString axistitle_mc_probeprobe = "<p^{probe,RECO}_{T}>/<p^{probe,GEN}_{T}>";

    TGraphErrors *graph_probeRECO_tagRECO   = new TGraphErrors(n_pt-1, xbin_tgraph, val_probeRECO_tagRECO[i], zero, err_probeRECO_tagRECO[i]);
    graph_probeRECO_tagRECO   = (TGraphErrors*)CleanEmptyPoints(graph_probeRECO_tagRECO);
    //    cout<<"graph_probeRECO_tagRECO"<<endl;
    //    graph_probeRECO_tagRECO->Print();
    graph_probeRECO_tagRECO->SetTitle("");
    graph_probeRECO_tagRECO->SetMarkerColor(kGreen);
    graph_probeRECO_tagRECO->SetMarkerStyle(20);
    graph_probeRECO_tagRECO->SetMarkerSize(0.6);
    graph_probeRECO_tagRECO->SetLineColor(kGreen);
    TString axistitle_mc_probetagRECO = "<p^{probe,RECO}_{T}>/<p^{tag,RECO}_{T}>";

    TGraphErrors *graph_tagRECO_tagGEN   = new TGraphErrors(n_pt-1, xbin_tgraph, val_tagRECO_tagGEN[i], zero, err_tagRECO_tagGEN[i]);
    graph_tagRECO_tagGEN   = (TGraphErrors*)CleanEmptyPoints(graph_tagRECO_tagGEN);
    //    cout<<"graph_tagRECO_tagGEN"<<endl;
    //    graph_tagRECO_tagGEN->Print();
    graph_tagRECO_tagGEN->SetTitle("");
    graph_tagRECO_tagGEN->SetMarkerColor(kBlue);
    graph_tagRECO_tagGEN->SetMarkerStyle(20);
    //    graph_tagRECO_tagGEN->SetMarkerSize(1.0);
    graph_tagRECO_tagGEN->SetLineColor(kBlue);
    TString axistitle_mc_tagtag = "<p^{tag,RECO}_{T}>/<p^{tag,GEN}_{T}>";

    TLatex *tex = new TLatex();
    tex->SetNDC();
    tex->SetTextSize(0.045); 
    TString text = eta_range[i] + " < |#eta| < " + eta_range[i+1];

    TCanvas* c_rel = new TCanvas();
    tdrCanvas(c_rel,"c_rel",h,4,10,true,CorrectionObject::_lumitag);
    h->GetXaxis()->SetTitle(pt_binning_var_str);
    h->GetXaxis()->SetTitleSize(0.05);
    h->GetXaxis()->SetLimits(30,pt_bins[n_pt-1]+100);
    h->GetYaxis()->SetRangeUser(0.70,1.30);
    graph_rel_A_mc->Draw("P SAME");
    graph_probeRECO_probeGEN->Draw("P SAME");
    graph_tagRECO_tagGEN->Draw("P SAME");
    graph_probeRECO_tagRECO->Draw("P SAME");

    gPad->SetLogx();
    TLegend *leg_rel;
    leg_rel = new TLegend(0.45,0.15,0.91,0.49,"","brNDC");//x+0.1
    leg_rel->SetBorderSize(0);
    leg_rel->SetTextSize(0.030);
    leg_rel->SetFillColor(10);
    leg_rel->SetFillStyle(0);
    leg_rel->SetLineColor(1);
    leg_rel->SetTextFont(42);
    leg_rel->SetHeader("R^{MC}_"+altitle+", "+eta_range[i]+"#leq|#eta|<"+eta_range[i+1]+" "+flavorLabel); 
    leg_rel->AddEntry(graph_rel_A_mc, axistitle_A_mc,"P");
    leg_rel->AddEntry(graph_probeRECO_probeGEN, axistitle_mc_probeprobe,"P");
    leg_rel->AddEntry(graph_tagRECO_tagGEN, axistitle_mc_tagtag,"P");
    leg_rel->AddEntry(graph_probeRECO_tagRECO, axistitle_mc_probetagRECO,"P");
    leg_rel->Draw();
    c_rel->SaveAs(CorrectionObject::_outpath+"plots/control/GenResponse_RatioOfAverages_RECOvsGEN_"+pt_binning_var_name+ CorrectionObject::_generator_tag + "_eta_" + eta_range2[i] + "_" + eta_range2[i+1] + ".pdf");

    delete graph_rel_A_mc;
    delete graph_probeRECO_probeGEN;
    delete graph_tagRECO_tagGEN;
    delete graph_probeRECO_tagRECO;
  }

  for(int i=0; i<n_eta-1; i++){
    //Create and fill TGraphErrors
    double xbin_tgraph[n_pt-1];
    double zero[n_pt-1];
    for(int i=0;i<n_pt-1;i++){
      xbin_tgraph[i]=(pt_bins[i]+pt_bins[i+1])/2;
      zero[i]=(pt_bins[i+1]-pt_bins[i])/2 ;
    }

    TString alVal;
    alVal.Form("%0.2f\n",alpha_cut);
    TString altitle = "{#alpha<"+alVal+"}";

    TGraphErrors *graph_QQ   = new TGraphErrors(n_pt-1, xbin_tgraph, val_QQ[i], zero, err_QQ[i]);
    graph_QQ   = (TGraphErrors*)CleanEmptyPoints(graph_QQ);
    graph_QQ->SetTitle("");
    graph_QQ->SetMarkerColor(kOrange+7);
    graph_QQ->SetMarkerStyle(23);
    //    graph_QQ->SetMarkerSize(1.7);
    graph_QQ->SetLineColor(kOrange+7);
    TString axistitle_QQ = "QQ";
    TGraphErrors *graph_GG   = new TGraphErrors(n_pt-1, xbin_tgraph, val_GG[i], zero, err_GG[i]);
    graph_GG   = (TGraphErrors*)CleanEmptyPoints(graph_GG);
    graph_GG->SetTitle("");
    graph_GG->SetMarkerColor(kGreen+2);
    graph_GG->SetMarkerStyle(22);
    //    graph_GG->SetMarkerSize(1.7);
    graph_GG->SetLineColor(kGreen+2);
    TString axistitle_GG = "GG";
    TGraphErrors *graph_QG   = new TGraphErrors(n_pt-1, xbin_tgraph, val_QG[i], zero, err_QG[i]);
    graph_QG   = (TGraphErrors*)CleanEmptyPoints(graph_QG);
    graph_QG->SetTitle("");
    graph_QG->SetMarkerColor(kRed);
    graph_QG->SetMarkerStyle(20);
    //    graph_QG->SetMarkerSize(1.7);
    graph_QG->SetLineColor(kRed);
    TString axistitle_QG = "QG";
    TGraphErrors *graph_GQ   = new TGraphErrors(n_pt-1, xbin_tgraph, val_GQ[i], zero, err_GQ[i]);
    graph_GQ   = (TGraphErrors*)CleanEmptyPoints(graph_GQ);
    graph_GQ->SetTitle("");
    graph_GQ->SetMarkerColor(kBlue+3);
    graph_GQ->SetMarkerStyle(20);
    //    graph_GQ->SetMarkerSize(1.7);
    graph_GQ->SetLineColor(kBlue+3);
    TString axistitle_GQ = "GQ";

    TLatex *tex = new TLatex();
    tex->SetNDC();
    tex->SetTextSize(0.045); 
    TString text = eta_range[i] + " < |#eta| < " + eta_range[i+1];

    TCanvas* c_rel = new TCanvas();
    tdrCanvas(c_rel,"c_rel",h,4,10,true,CorrectionObject::_lumitag);
    h->GetXaxis()->SetTitle(pt_binning_var_str);
    h->GetXaxis()->SetTitleSize(0.05);
    h->GetXaxis()->SetLimits(30,pt_bins[n_pt-1]+100);
    h->GetYaxis()->SetTitle("Fraction");
    h->GetYaxis()->SetRangeUser(0.0,1.00);
    graph_QQ->Draw("P SAME");
    graph_GG->Draw("P SAME");
    graph_QG->Draw("P SAME");
    graph_GQ->Draw("P SAME");


    gPad->SetLogx();
    TLegend *leg_rel;
    leg_rel = new TLegend(0.55,0.65,0.91,0.89,"","brNDC");//x+0.1
    leg_rel->SetBorderSize(0);
    leg_rel->SetTextSize(0.030);
    leg_rel->SetFillColor(10);
    leg_rel->SetFillStyle(0);
    leg_rel->SetLineColor(1);
    leg_rel->SetTextFont(42);
    leg_rel->SetHeader("R^{MC, fraction}_"+altitle+", "+eta_range[i]+"#leq|#eta|<"+eta_range[i+1]); 
    leg_rel->AddEntry(graph_QQ, axistitle_QQ,"P");
    leg_rel->AddEntry(graph_GG, axistitle_GG,"P");
    leg_rel->AddEntry(graph_QG, axistitle_QG,"P");
    leg_rel->AddEntry(graph_GQ, axistitle_GQ,"P");
    leg_rel->Draw();
    c_rel->SaveAs(CorrectionObject::_outpath+"plots/control/GenResponse_Fractions_"+pt_binning_var_name+ CorrectionObject::_generator_tag + "_eta_" + eta_range2[i] + "_" + eta_range2[i+1] + ".pdf");

    delete graph_QQ;
    delete graph_GG;
    delete graph_QG;
    delete graph_GQ;
  }



  for(int i=0; i<n_eta-1; i++){
    //Create and fill TGraphErrors
    double xbin_tgraph[n_pt-1];
    double zero[n_pt-1];
    for(int i=0;i<n_pt-1;i++){
      xbin_tgraph[i]=(pt_bins[i]+pt_bins[i+1])/2;
      zero[i]=(pt_bins[i+1]-pt_bins[i])/2 ;
    }

    TString alVal;
    alVal.Form("%0.2f\n",alpha_cut);
    TString altitle = "{#alpha<"+alVal+"}";

    TGraphErrors *graph_rel_A_GEN_mc   = new TGraphErrors(n_pt-1, xbin_tgraph, val_rel_A_GEN_mc[i], zero, err_rel_A_GEN_mc[i]);
    graph_rel_A_GEN_mc   = (TGraphErrors*)CleanEmptyPoints(graph_rel_A_GEN_mc);
    //    cout<<"graph_rel_A_GEN_mc"<<endl;
    //    graph_rel_A_GEN_mc->Print();
    graph_rel_A_GEN_mc->SetTitle("");
    // graph_rel_A_GEN_mc->GetXaxis()->SetTitle(pt_binning_var_str);
    // graph_rel_A_GEN_mc->GetXaxis()->SetTitleSize(0.05);
    // graph_rel_A_GEN_mc->GetXaxis()->SetTitleOffset(0.80);
    // graph_rel_A_GEN_mc->GetXaxis()->SetLimits(30,pt_bins[n_pt-1]+100);
    // graph_rel_A_GEN_mc->GetYaxis()->SetRangeUser(0.70,1.30);
    graph_rel_A_GEN_mc->SetMarkerColor(kOrange+7);
    graph_rel_A_GEN_mc->SetMarkerStyle(29);
    graph_rel_A_GEN_mc->SetMarkerSize(1.7);
    graph_rel_A_GEN_mc->SetLineColor(kOrange+7);
    TString axistitle_A_GEN_mc = "(1+<A_{GEN}>)/(1-<A_{GEN}>)";

    TGraphErrors *graph_rel_A_PARTON_mc   = new TGraphErrors(n_pt-1, xbin_tgraph, val_rel_A_PARTON_mc[i], zero, err_rel_A_PARTON_mc[i]);
    graph_rel_A_PARTON_mc   = (TGraphErrors*)CleanEmptyPoints(graph_rel_A_PARTON_mc);
    //    cout<<"graph_rel_A_PARTON_mc"<<endl;     
    //    graph_rel_A_PARTON_mc->Print();
    graph_rel_A_PARTON_mc->SetTitle("");
    graph_rel_A_PARTON_mc->SetMarkerColor(kMagenta);
    graph_rel_A_PARTON_mc->SetMarkerStyle(29);
    graph_rel_A_PARTON_mc->SetMarkerSize(1.7);
    graph_rel_A_PARTON_mc->SetLineColor(kMagenta);
    TString axistitle_A_PARTON_mc = "(1+<A_{PARTON}>)/(1-<A_{PARTON}>)";

    TGraphErrors *graph_probeGEN_probePARTON   = new TGraphErrors(n_pt-1, xbin_tgraph, val_probeGEN_probePARTON[i], zero, err_probeGEN_probePARTON[i]);
    graph_probeGEN_probePARTON   = (TGraphErrors*)CleanEmptyPoints(graph_probeGEN_probePARTON);
    //    cout<<"graph_probeGEN_probePARTON"<<endl;
    //    graph_probeGEN_probePARTON->Print();
    graph_probeGEN_probePARTON->SetTitle("");
    graph_probeGEN_probePARTON->SetMarkerColor(kRed);
    graph_probeGEN_probePARTON->SetMarkerStyle(20);
    graph_probeGEN_probePARTON->SetLineColor(kRed);
    TString axistitle_mc_probeprobe = "<p^{probe,GEN}_{T}>/<p^{probe,PARTON}_{T}>";

    TGraphErrors *graph_probeGEN_tagGEN   = new TGraphErrors(n_pt-1, xbin_tgraph, val_probeGEN_tagGEN[i], zero, err_probeGEN_tagGEN[i]);
    graph_probeGEN_tagGEN   = (TGraphErrors*)CleanEmptyPoints(graph_probeGEN_tagGEN);
    //    cout<<"graph_probeGEN_tagGEN"<<endl;
    //    graph_probeGEN_tagGEN->Print();
    graph_probeGEN_tagGEN->SetTitle("");
    graph_probeGEN_tagGEN->SetMarkerColor(kGreen);
    graph_probeGEN_tagGEN->SetMarkerStyle(20);
    graph_probeGEN_tagGEN->SetMarkerSize(0.8);
    graph_probeGEN_tagGEN->SetLineColor(kGreen);
    TString axistitle_mc_probetagGEN = "<p^{probe,GEN}_{T}>/<p^{tag,GEN}_{T}>";

    TGraphErrors *graph_tagGEN_tagPARTON   = new TGraphErrors(n_pt-1, xbin_tgraph, val_tagGEN_tagPARTON[i], zero, err_tagGEN_tagPARTON[i]);
    graph_tagGEN_tagPARTON   = (TGraphErrors*)CleanEmptyPoints(graph_tagGEN_tagPARTON);
    //    cout<<"graph_tagGEN_tagPARTON"<<endl;
    //    graph_tagGEN_tagPARTON->Print();
    graph_tagGEN_tagPARTON->SetTitle("");
    graph_tagGEN_tagPARTON->SetMarkerColor(kBlue);
    graph_tagGEN_tagPARTON->SetMarkerStyle(20);
    //    graph_tagGEN_tagPARTON->SetMarkerSize(1.7);
    graph_tagGEN_tagPARTON->SetLineColor(kBlue);
    TString axistitle_mc_tagtag = "<p^{tag,GEN}_{T}>/<p^{tag,PARTON}_{T}>";

    TLatex *tex = new TLatex();
    tex->SetNDC();
    tex->SetTextSize(0.045); 
    TString text = eta_range[i] + " < |#eta| < " + eta_range[i+1];

    TCanvas* c_rel = new TCanvas();
    tdrCanvas(c_rel,"c_rel",h,4,10,true,CorrectionObject::_lumitag);
    h->GetXaxis()->SetTitle(pt_binning_var_str);
    h->GetXaxis()->SetTitleSize(0.05);
    h->GetXaxis()->SetLimits(30,pt_bins[n_pt-1]+100);
    h->GetYaxis()->SetRangeUser(0.70,1.30);
    h->GetYaxis()->SetTitle("");
    graph_rel_A_GEN_mc->Draw("P SAME");
    graph_rel_A_PARTON_mc->Draw("P SAME");
    graph_probeGEN_probePARTON->Draw("P SAME");
    graph_tagGEN_tagPARTON->Draw("P SAME");
    graph_probeGEN_tagGEN->Draw("P SAME");

    gPad->SetLogx();
    TLegend *leg_rel;
    leg_rel = new TLegend(0.45,0.15,0.91,0.49,"","brNDC");//x+0.1
    leg_rel->SetBorderSize(0);
    leg_rel->SetTextSize(0.030);
    leg_rel->SetFillColor(10);
    leg_rel->SetFillStyle(0);
    leg_rel->SetLineColor(1);
    leg_rel->SetTextFont(42);
    leg_rel->SetHeader("R^{MC}_"+altitle+", "+eta_range[i]+"#leq|#eta|<"+eta_range[i+1]+" "+flavorLabel); 
    leg_rel->AddEntry(graph_rel_A_GEN_mc, axistitle_A_GEN_mc,"P");
    leg_rel->AddEntry(graph_rel_A_PARTON_mc, axistitle_A_PARTON_mc,"P");
    leg_rel->AddEntry(graph_probeGEN_probePARTON, axistitle_mc_probeprobe,"P");
    leg_rel->AddEntry(graph_tagGEN_tagPARTON, axistitle_mc_tagtag,"P");
    leg_rel->AddEntry(graph_probeGEN_tagGEN, axistitle_mc_probetagGEN,"P");
    leg_rel->Draw();
    c_rel->SaveAs(CorrectionObject::_outpath+"plots/control/GenResponse_RatioOfAverages_GENvsPARTON_"+pt_binning_var_name+ CorrectionObject::_generator_tag + "_eta_" + eta_range2[i] + "_" + eta_range2[i+1] + ".pdf");

    delete graph_rel_A_GEN_mc;
    delete graph_rel_A_PARTON_mc;
    delete graph_probeGEN_probePARTON;
    delete graph_tagGEN_tagPARTON;
    delete graph_probeGEN_tagGEN;

  }
  
  
    /* TGraphErrors *graph_rel_A_mc   = new TGraphErrors(n_pt-1, xbin_tgraph, val_rel_A_mc[i], zero, err_rel_A_mc[i]);
    graph_rel_A_mc   = (TGraphErrors*)CleanEmptyPoints(graph_rel_A_mc);

    graph_rel_A_mc->SetTitle("");
    // graph_rel_A_mc->GetXaxis()->SetTitle("#bar{p}_{T} [GeV]");
    graph_rel_A_mc->GetXaxis()->SetTitle(pt_binning_var_str);
    graph_rel_A_mc->GetXaxis()->SetTitleSize(0.05);
    graph_rel_A_mc->GetXaxis()->SetTitleOffset(0.80);
    graph_rel_A_mc->GetXaxis()->SetLimits(30,pt_bins[n_pt-1]+100);
    graph_rel_A_mc->GetYaxis()->SetRangeUser(0.70,1.30);
    graph_rel_A_mc->SetMarkerColor(kOrange+7);
    graph_rel_A_mc->SetMarkerStyle(29);
    graph_rel_A_mc->SetMarkerSize(1.7);
    graph_rel_A_mc->SetLineColor(kOrange+7);

    TString alVal;
    alVal.Form("%0.2f\n",alpha_cut);
    TString altitle = "{#alpha<"+alVal+"}";
    // TString axistitle_A_mc = "R^{MC}_"; 
    // axistitle_A_mc += altitle;
    // axistitle_A_mc += "((1+<A>)/(1-<A>))";
    TString axistitle_A_mc = "(1+<A>)/(1-<A>)";

    TGraphErrors *graph_rel_mc   = new TGraphErrors(n_pt-1, xbin_tgraph, val_rel_mc[i], zero, err_rel_mc[i]);
    graph_rel_mc   = (TGraphErrors*)CleanEmptyPoints(graph_rel_mc);

    graph_rel_mc->SetTitle("");
    // //    graph_rel_mc->GetXaxis()->SetTitle("#bar{p}_{T} [GeV]");
    // graph_rel_mc->GetXaxis()->SetTitle(pt_binning_var_str);
    // graph_rel_mc->GetXaxis()->SetTitleSize(0.05);
    // graph_rel_mc->GetXaxis()->SetTitleOffset(0.80);
    // graph_rel_mc->GetXaxis()->SetLimits(30,pt_bins[n_pt-1]+100);
    // graph_rel_mc->GetYaxis()->SetRangeUser(0.70,1.30);
    graph_rel_mc->SetMarkerColor(kRed);
    graph_rel_mc->SetMarkerStyle(20);
    graph_rel_mc->SetLineColor(kRed);

    // TString axistitle_mc = "R^{MC}_"; 
    // axistitle_mc   += altitle;
    // axistitle_mc += "(<p^{probe,RECO}_{T}/p^{probe,GEN}_{T}>)";
    TString axistitle_mc = "<p^{probe,RECO}_{T}/p^{probe,GEN}_{T}>";

    TGraphErrors *graph_tagprobe_mc   = new TGraphErrors(n_pt-1, xbin_tgraph, val_tagprobe_mc[i], zero, err_tagprobe_mc[i]);
    graph_tagprobe_mc   = (TGraphErrors*)CleanEmptyPoints(graph_tagprobe_mc);

    graph_tagprobe_mc->SetTitle("");
    //    graph_tagprobe_mc->GetXaxis()->SetTitle("#bar{p}_{T} [GeV]");
    // graph_tagprobe_mc->GetXaxis()->SetTitle(pt_binning_var_str);
    // graph_tagprobe_mc->GetXaxis()->SetTitleSize(0.05);
    // graph_tagprobe_mc->GetXaxis()->SetTitleOffset(0.80);
    // graph_tagprobe_mc->GetXaxis()->SetLimits(30,pt_bins[n_pt-1]+100);
    //    graph_tagprobe_mc->GetYaxis()->SetRangeUser(0.70,1.30);
    graph_tagprobe_mc->SetMarkerColor(kBlack);
    graph_tagprobe_mc->SetMarkerSize(1.1);
    graph_tagprobe_mc->SetMarkerStyle(20);
    graph_tagprobe_mc->SetLineColor(kBlack);

    // TString axistitle_tagprobe = "R^{MC}_"; 
    // axistitle_tagprobe   += altitle;
    // axistitle_tagprobe += "(<p^{probe,GEN}_{T}/p^{tag,GEN}_{T}>)";
    TString axistitle_tagprobe = "<p^{probe,GEN}_{T}/p^{tag,GEN}_{T}>";
    TGraphErrors *graph_tagprobe_parton_mc   = new TGraphErrors(n_pt-1, xbin_tgraph, val_tagprobe_parton_mc[i], zero, err_tagprobe_parton_mc[i]);
    //    graph_tagprobe_parton_mc   = (TGraphErrors*)CleanEmptyPoints(graph_tagprobe_parton_mc);

    graph_tagprobe_parton_mc->SetTitle("");
    //    graph_tagprobe_parton_mc->GetXaxis()->SetTitle("#bar{p}_{T} [GeV]");
    // graph_tagprobe_parton_mc->GetXaxis()->SetTitle(pt_binning_var_str);
    // graph_tagprobe_parton_mc->GetXaxis()->SetTitleSize(0.05);
    // graph_tagprobe_parton_mc->GetXaxis()->SetTitleOffset(0.80);
    // graph_tagprobe_parton_mc->GetXaxis()->SetLimits(30,pt_bins[n_pt-1]+100);
    // graph_tagprobe_parton_mc->GetYaxis()->SetRangeUser(0.70,1.30);
    graph_tagprobe_parton_mc->SetMarkerColor(kGreen);
    graph_tagprobe_parton_mc->SetMarkerStyle(20);
    graph_tagprobe_parton_mc->SetLineColor(kGreen);

    // TString axistitle_tagprobe_parton = "R^{MC}_"; 
    // axistitle_tagprobe_parton   += altitle;
    // axistitle_tagprobe_parton += "(<p^{probe,PAR}_{T}/p^{tag,PAR}_{T}>)";
    TString axistitle_tagprobe_parton = "<p^{probe,PAR}_{T}/p^{tag,PAR}_{T}>";
    TGraphErrors *graph_tagprobe_reco_mc   = new TGraphErrors(n_pt-1, xbin_tgraph, val_tagprobe_reco_mc[i], zero, err_tagprobe_reco_mc[i]);
    graph_tagprobe_reco_mc   = (TGraphErrors*)CleanEmptyPoints(graph_tagprobe_reco_mc);

    graph_tagprobe_reco_mc->SetTitle("");
    //    graph_tagprobe_reco_mc->GetXaxis()->SetTitle("#bar{p}_{T} [GeV]");
    // graph_tagprobe_reco_mc->GetXaxis()->SetTitle(pt_binning_var_str);
    // graph_tagprobe_reco_mc->GetXaxis()->SetTitleSize(0.05);
    // graph_tagprobe_reco_mc->GetXaxis()->SetTitleOffset(0.80);
    // graph_tagprobe_reco_mc->GetXaxis()->SetLimits(30,pt_bins[n_pt-1]+100);
    // graph_tagprobe_reco_mc->GetYaxis()->SetRangeUser(0.70,1.30);
    graph_tagprobe_reco_mc->SetMarkerColor(kMagenta);
    graph_tagprobe_reco_mc->SetMarkerStyle(20);
    graph_tagprobe_reco_mc->SetLineColor(kMagenta);

    // TString axistitle_tagprobe_reco = "R^{MC}_"; 
    // axistitle_tagprobe_reco   += altitle;
    // axistitle_tagprobe_reco += "(<p^{probe,RECO}_{T}/p^{tag,RECO}_{T}>)";
    TString axistitle_tagprobe_reco = "<p^{probe,RECO}_{T}/p^{tag,RECO}_{T}>";

    TGraphErrors *graph_tagtag_mc   = new TGraphErrors(n_pt-1, xbin_tgraph, val_tagtag_mc[i], zero, err_tagtag_mc[i]);
    graph_tagtag_mc   = (TGraphErrors*)CleanEmptyPoints(graph_tagtag_mc);
    // cout<<"TAGTAG graph"<<endl;
    // graph_tagtag_mc->Print();
    graph_tagtag_mc->SetTitle("");

    //    graph_tagtag_mc->GetXaxis()->SetTitle("#bar{p}_{T} [GeV]");
    // graph_tagtag_mc->GetXaxis()->SetTitle(pt_binning_var_str);
    // graph_tagtag_mc->GetXaxis()->SetTitleSize(0.05);
    // graph_tagtag_mc->GetXaxis()->SetTitleOffset(0.80);
    // graph_tagtag_mc->GetXaxis()->SetLimits(30,pt_bins[n_pt-1]+100);
    // graph_tagtag_mc->GetYaxis()->SetRangeUser(0.70,1.30);
    graph_tagtag_mc->SetMarkerColor(kBlue);
    graph_tagtag_mc->SetMarkerStyle(29);
    graph_tagtag_mc->SetMarkerSize(1.7);
    graph_tagtag_mc->SetLineColor(kBlue);

    // TString axistitle_tagtag = "R^{MC}_"; 
    // axistitle_tagtag   += altitle;
    // axistitle_tagtag += "(<p^{tag,RECO}_{T}/p^{tag,GEN}_{T}>)";
    TString axistitle_tagtag = "<p^{tag,RECO}_{T}/p^{tag,GEN}_{T}>";

    TLatex *tex = new TLatex();
    tex->SetNDC();
    tex->SetTextSize(0.045); 
    TString text = eta_range[i] + " < |#eta| < " + eta_range[i+1];

    TCanvas* c_rel = new TCanvas();
    tdrCanvas(c_rel,"c_rel",h,4,10,true,CorrectionObject::_lumitag);
    //    h->GetXaxis()->SetTitle("#bar{p}_{T} [GeV]");
    h->GetXaxis()->SetTitle(pt_binning_var_str);
    h->GetXaxis()->SetTitleSize(0.05);
    //    h->GetXaxis()->SetTitleOffset(0.90);
    h->GetXaxis()->SetLimits(30,pt_bins[n_pt-1]+100);
    h->GetYaxis()->SetRangeUser(0.70,1.30);
    graph_rel_A_mc->Draw("P SAME");
    graph_tagtag_mc->Draw("P SAME");
    graph_rel_mc->Draw("P SAME");
    graph_tagprobe_mc->Draw("P SAME");
    graph_tagprobe_parton_mc->Draw("P SAME");
    graph_tagprobe_reco_mc->Draw("P SAME");


    // graph_tagtag_mc->Print();
    // graph_rel_A_mc->Print();

    gPad->SetLogx();

    TLegend *leg_rel;
    //    leg_rel = new TLegend(0.35,0.50,0.91,0.89,"","brNDC");//x+0.1
    leg_rel = new TLegend(0.35,0.15,0.91,0.49,"","brNDC");//x+0.1
    //    leg_rel->SetNColumns(2);
    leg_rel->SetBorderSize(0);
    leg_rel->SetTextSize(0.030);
    leg_rel->SetFillColor(10);
    leg_rel->SetFillStyle(0);
    leg_rel->SetLineColor(1);
    leg_rel->SetTextFont(42);
    leg_rel->SetHeader("R^{MC}_"+altitle+", "+eta_range[i]+"#leq|#eta|<"+eta_range[i+1]); 
    leg_rel->AddEntry(graph_rel_A_mc, axistitle_A_mc,"P");
    leg_rel->AddEntry(graph_rel_mc, axistitle_mc,"P");
    leg_rel->AddEntry(graph_tagprobe_mc, axistitle_tagprobe,"P");
    leg_rel->AddEntry(graph_tagprobe_parton_mc, axistitle_tagprobe_parton,"P");
    leg_rel->AddEntry(graph_tagprobe_reco_mc, axistitle_tagprobe_reco,"P");
    leg_rel->AddEntry(graph_tagtag_mc, axistitle_tagtag,"P");
    leg_rel->Draw();
    //tex->DrawLatex(0.53,0.91,CorrectionObject::_lumitag+"(13TeV)");

    c_rel->SaveAs(CorrectionObject::_outpath+"plots/control/GenResponse_matchedProbeJet_"+pt_binning_var_name+ CorrectionObject::_generator_tag + "_eta_" + eta_range2[i] + "_" + eta_range2[i+1] + ".pdf");


    //delete leg_rel;
    delete c_rel;
    //delete leg_mpf;
    delete tex;
    delete graph_rel_mc;
   
  }
   */
  // //Plot 1d genID->OFFLINE jet distributions in a particular eta-bin for different pt-bins on different canvases
  // for(int i=0; i<n_eta-1; i++){
  //   TString eta_name = "eta_"+eta_range2[i]+"_"+eta_range2[i+1];
    
  //   TLatex *tex = new TLatex();
  //   tex->SetNDC();
  //   tex->SetTextSize(0.045); 
  //   TString text = eta_range[i] + " < |#eta| < " + eta_range[i+1];

  //   TLatex *tex_lumi = new TLatex();
  //   tex_lumi->SetNDC();
  //   tex_lumi->SetTextSize(0.045); 
  //   for(int j=0; j<n_pt-1; j++){   ///j=0 j<pt_n-1
  //     TString pt_name = "pt_"+pt_range[j]+"_"+pt_range[j+1];
  //     TString legname = "p_{T} #in [" + pt_range[j] + "," + pt_range[j+1] + "]";
  //     int n_ev1 = hmc_jet1_genID[i][j]->GetEntries();
  //     int n_ev2 = hmc_jet2_genID[i][j]->GetEntries();
  //     int n_ev3 = hmc_jet3_genID[i][j]->GetEntries();
  //     // cout<<"Scale = "<<1/hmc_jet1_genID[i][j]->Integral()<<endl;
  //     // if(hmc_jet1_genID[i][j]->Integral() > 0) hmc_jet1_genID[i][j]->Scale(1/hmc_jet1_genID[i][j]->Integral());
  //     // if(hmc_jet2_genID[i][j]->Integral() > 0) hmc_jet2_genID[i][j]->Scale(1/hmc_jet2_genID[i][j]->Integral());
  //     // if(hmc_jet3_genID[i][j]->Integral() > 0) hmc_jet3_genID[i][j]->Scale(1/hmc_jet3_genID[i][j]->Integral());

  //     hmc_jet1_genID[i][j]->Print();
  //     //      if(htemp_mpf_mc->Integral() > 0)htemp_mpf_mc->Scale(1/htemp_mpf_mc->Integral());

  //     if(hmc_jet1_genID[i][j]->Integral() > 0)
  //     h->GetXaxis()->SetTitle("genID-recoID");
  //     //      h->GetYaxis()->SetTitle("Normalized entries");
  //     h->GetYaxis()->SetTitleOffset(1.5);
  //     h->GetXaxis()->SetLimits(-5,10);
  //     h->SetMinimum(1e-1);
  //     h->SetMaximum(1e15);
  //     hmc_jet1_genID[i][j]->SetLineColor(kRed);
  //     hmc_jet1_genID[i][j]->SetMarkerColor(kRed);
  //     hmc_jet1_genID[i][j]->SetLineWidth(3);
  //     hmc_jet1_genID[i][j]->SetMarkerStyle(20);
  //     hmc_jet1_genID[i][j]->SetFillColorAlpha(kRed,0.99);

  //     hmc_jet2_genID[i][j]->SetLineColor(kGreen);
  //     hmc_jet2_genID[i][j]->SetMarkerColor(kGreen);
  //     hmc_jet2_genID[i][j]->SetLineWidth(3);
  //     hmc_jet2_genID[i][j]->SetMarkerStyle(20);
  //     hmc_jet2_genID[i][j]->SetFillColorAlpha(kGreen,0.5);
  //     hmc_jet2_genID[i][j]->SetFillStyle(3005);
  //     //      hmc_jet2_genID[i][j]->SetFillColor(3005kGreen);

  //     hmc_jet3_genID[i][j]->SetLineColor(kBlack);
  //     hmc_jet3_genID[i][j]->SetMarkerColor(kBlack);
  //     hmc_jet3_genID[i][j]->SetLineWidth(3);
  //     hmc_jet3_genID[i][j]->SetMarkerStyle(20);
  //     hmc_jet3_genID[i][j]->SetFillColorAlpha(kBlack,0.25);
      
  //     TCanvas* ctmp = new TCanvas();
  //     tdrCanvas(ctmp,"ctmp",h,4,10,kSquare,CorrectionObject::_lumitag);
  //     TLegend leg2 = tdrLeg(0.35,0.6,0.90,0.89);
  //     leg2.SetHeader(""+eta_range[i]+"#leq|#eta|<"+eta_range[i+1]+", "+legname); 
  //     leg2.AddEntry(hmc_jet1_genID[i][j], "RECO jet1", "l");
  //     leg2.AddEntry(hmc_jet2_genID[i][j], "RECO jet2", "l");
  //     leg2.AddEntry(hmc_jet3_genID[i][j], "RECO jet3", "l");
  //     if(n_ev1>0) hmc_jet1_genID[i][j]->Draw("HIST SAME");
  //     if(n_ev2>0) hmc_jet2_genID[i][j]->Draw("HIST SAME");
  //     if(n_ev3>0) hmc_jet3_genID[i][j]->Draw("HIST SAME");
  //     leg2.Draw();
  //     gPad->SetLogy();
  //     //      tex->DrawLatex(0.47,0.85,"MC, " + text);
  //     ctmp->SaveAs(CorrectionObject::_outpath+"plots/control/Matched_OFFLINEJet_genID_" + CorrectionObject::_generator_tag + "_eta_" + eta_range2[i] + "_" + eta_range2[i+1] 
  // 		   +"_" +pt_name +".pdf");
  //   }
  // }
  
  //Plot Probe jet pt for different flavors on one canvas per eta bin
  for(int i=0; i<n_eta-1; i++){

    TString eta_name = "eta_"+eta_range2[i]+"_"+eta_range2[i+1];
    
    TLatex *tex = new TLatex();
    tex->SetNDC();
    tex->SetTextSize(0.045); 
    TString text = eta_range[i] + " < |#eta| < " + eta_range[i+1];

    TLatex *tex_lumi = new TLatex();
    tex_lumi->SetNDC();
    tex_lumi->SetTextSize(0.045); 
    TCanvas* ctmp = new TCanvas();
    tdrCanvas(ctmp,"ctmp",h,4,10,kSquare,CorrectionObject::_lumitag);
    TLegend leg2 = tdrLeg(0.35,0.6,0.90,0.89);
    leg2.SetHeader(""+eta_range[i]+"#leq|#eta|<"+eta_range[i+1]); 
    // TH1D *h_sum = hmc_probejetpt_flavor[0][i];
    // h_sum->Add(hmc_probejetpt_flavor[1][i]);
    // h_sum->Add(hmc_probejetpt_flavor[2][i]);
    for(int j=1; j<3; j++){   //jet flavor
    //    for(int j=2; j>-1; j--){   //jet flavor
      TString pt_name = "pt_"+pt_range[j]+"_"+pt_range[j+1];
      TString legname = "p_{T} #in [" + pt_range[j] + "," + pt_range[j+1] + "]";
      int n_ev = hmc_probejetpt_flavor[j][i]->GetEntries();
      //      if(hmc_probejetpt_flavor[j][i]->Integral() > 0)hmc_probejetpt_flavor[j][i]->Scale(1/hmc_probejetpt_flavor[j][i]->Integral());
      //      double scale = h_sum->Integral()/hmc_probejetpt_flavor[j][i]->Integral();
      //      cout<<"scale = "<<scale<<endl;
      //      if(h_sum->Integral() > 0 && hmc_probejetpt_flavor[j][i]->Integral() > 0) hmc_probejetpt_flavor[j][i]->Scale(h_sum->Integral()/hmc_probejetpt_flavor[j][i]->Integral());
      h->GetXaxis()->SetTitle("RECO probe jet p_{T}, GeV");
      //      h->GetYaxis()->SetTitle("Normalized entries");
      //      h->GetYaxis()->SetTitle("Normalized Fraction");
      h->GetYaxis()->SetTitleOffset(1.5);
      h->GetXaxis()->SetLimits(0,500);
      h->SetMinimum(1e-2);
      //      h->SetMaximum(1.0);
      h->SetMaximum(1e14);

      hmc_probejetpt_flavor[j][i]->SetLineColor(kRed+2*j);
      hmc_probejetpt_flavor[j][i]->SetMarkerColor(kRed+2*j);
      // hmc_probejetpt_flavor[j][i]->SetLineColor(1+2*j);
      // hmc_probejetpt_flavor[j][i]->SetMarkerColor(1+2*j);
      hmc_probejetpt_flavor[j][i]->SetLineWidth(3);
      hmc_probejetpt_flavor[j][i]->SetMarkerStyle(20);
      hmc_probejetpt_flavor[j][i]->SetFillColorAlpha(kRed+2*j,1-(j+1)/3.);
      //      hmc_probejetpt_flavor[j][i]->SetFillColorAlpha(1+2*j,1-0.1*(j+1)/3.);
      if(j==0){
	hmc_probejetpt_flavor[j][i]->SetLineColor(kBlue+2*j);
	hmc_probejetpt_flavor[j][i]->SetMarkerColor(kBlue+2*j);
	hmc_probejetpt_flavor[j][i]->SetFillColorAlpha(kBlue+2*j,1-(j+1)/3.);
      }

      TString legtext;
      if(j==0) legtext = "unmatched";
      if(j==1) legtext = "Quarks";
      if(j==2) legtext = "Gluons";
      leg2.AddEntry(hmc_probejetpt_flavor[j][i], legtext, "fl");
      if(n_ev>0) hmc_probejetpt_flavor[j][i]->Draw("HIST SAME");
    }
      leg2.Draw();
      gPad->SetLogy();
      //      tex->DrawLatex(0.47,0.85,"MC, " + text);
      ctmp->SaveAs(CorrectionObject::_outpath+"plots/control/Matched_ProbeJetpt_flavor_" + CorrectionObject::_generator_tag + "_eta_" + eta_range2[i] + "_" + eta_range2[i+1]+".pdf");
  }

//Plot Tag jet pt for different flavors on one canvas per eta bin
  for(int i=0; i<n_eta-1; i++){

    TString eta_name = "eta_"+eta_range2[i]+"_"+eta_range2[i+1];
    
    TLatex *tex = new TLatex();
    tex->SetNDC();
    tex->SetTextSize(0.045); 
    TString text = eta_range[i] + " < |#eta| < " + eta_range[i+1];

    TLatex *tex_lumi = new TLatex();
    tex_lumi->SetNDC();
    tex_lumi->SetTextSize(0.045); 
    TCanvas* ctmp = new TCanvas();
    tdrCanvas(ctmp,"ctmp",h,4,10,kSquare,CorrectionObject::_lumitag);
    TLegend leg2 = tdrLeg(0.35,0.6,0.90,0.89);
    //    TLegend leg2 = tdrLeg(0.35,0.1,0.90,0.49);
    leg2.SetHeader(""+eta_range[i]+"#leq|#eta|<"+eta_range[i+1]); 
    TH1D *h_sum = hmc_tagjetpt_flavor[0][i];
    h_sum->Add(hmc_tagjetpt_flavor[1][i]);
    h_sum->Add(hmc_tagjetpt_flavor[2][i]);
    for(int j=1; j<3; j++){   //jet flavor
    //    for(int j=2; j>-1; j--){   //jet flavor
      TString pt_name = "pt_"+pt_range[j]+"_"+pt_range[j+1];
      TString legname = "p_{T} #in [" + pt_range[j] + "," + pt_range[j+1] + "]";
      int n_ev = hmc_tagjetpt_flavor[j][i]->GetEntries();
      //      if(h_sum->Integral() > 0) hmc_tagjetpt_flavor[j][i]->Scale(1/h_sum->Integral());
      h->GetXaxis()->SetTitle("RECO tag jet p_{T}, GeV");
      //      h->GetYaxis()->SetTitle("Normalized Fraction");
      h->GetYaxis()->SetTitleOffset(1.5);
      h->GetXaxis()->SetLimits(0,500);
      h->SetMinimum(1e-2);
      //      h->SetMaximum(100.0);
      h->SetMaximum(1e14);

      hmc_tagjetpt_flavor[j][i]->SetLineColor(kRed+2*j);
      hmc_tagjetpt_flavor[j][i]->SetMarkerColor(kRed+2*j);
      // hmc_tagjetpt_flavor[j][i]->SetLineColor(1+2*j);
      // hmc_tagjetpt_flavor[j][i]->SetMarkerColor(1+2*j);
      hmc_tagjetpt_flavor[j][i]->SetLineWidth(3);
      hmc_tagjetpt_flavor[j][i]->SetMarkerStyle(20);
      hmc_tagjetpt_flavor[j][i]->SetFillColorAlpha(kRed+2*j,1-(j+1)/3.);
      if(j==0){
	hmc_tagjetpt_flavor[j][i]->SetLineColor(kBlue+2*j);
	hmc_tagjetpt_flavor[j][i]->SetMarkerColor(kBlue+2*j);
	hmc_tagjetpt_flavor[j][i]->SetFillColorAlpha(kBlue+2*j,1-(j+1)/3.);
      }
      //      hmc_tagjetpt_flavor[j][i]->SetFillColorAlpha(1+2*j,1-0.1*(j+1)/3.);


      TString legtext;
      if(j==0) legtext = "unmatched";
      if(j==1) legtext = "Quarks";
      if(j==2) legtext = "Gluons";
      leg2.AddEntry(hmc_tagjetpt_flavor[j][i], legtext, "fl");
      if(n_ev>0) hmc_tagjetpt_flavor[j][i]->Draw("HIST SAME");
    }
      leg2.Draw();
      gPad->SetLogy();
      //      tex->DrawLatex(0.47,0.85,"MC, " + text);
      ctmp->SaveAs(CorrectionObject::_outpath+"plots/control/Matched_TagJetpt_flavor_" + CorrectionObject::_generator_tag + "_eta_" + eta_range2[i] + "_" + eta_range2[i+1]+".pdf");
  }

  
  
}

//  LocalWords:  tagPARTON GG
