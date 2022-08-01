#include "ROOT/RDataFrame.hxx"
#include <iostream>
#include "vector"
#include "string"
#include "TCanvas.h"
#include "TInterpreter.h"
#include "TH1D.h"
#include "FCCAnalyses/ReconstructedParticle.h"


#define nCPUS 4
#define outputDir "outputs/FCCee/higgs/mH-recoil/mumu/stage1"
#define inputDir "/tmp/fccperf/"

using namespace ROOT;


// class RDFanalysis {

// public:
//         auto analysers(ROOT::RDataFrame df) {
//             auto df2 = df
//             // define an alias for muon index collection
//             .Alias("Muon0", "Muon#0.index")

//             // define the muon collection
//             .Define("muons", "ReconstructedParticle::get(Muon0, ReconstructedParticles)")

//             // select muons on pT
//             .Define("selected_muons", "ReconstructedParticle::sel_pt(10.)(muons)")

//             // create branch with muon transverse momentum
//             .Define("selected_muons_pt", "ReconstructedParticle::get_pt(selected_muons)")

//             // create branch with muon rapidity
//             .Define("selected_muons_y", "ReconstructedParticle::get_y(selected_muons)")

//             // create branch with muon total momentum
//             .Define("selected_muons_p", "ReconstructedParticle::get_p(selected_muons)")

//             // create branch with muon energy
//             .Define("selected_muons_e", "ReconstructedParticle::get_e(selected_muons)")

//             // find zed candidates from  di-muon resonances
//             .Define("zed_leptonic", "ReconstructedParticle::resonanceBuilder(91)(selected_muons)")

//             // create branch with zed mass
//             .Define("zed_leptonic_m", "ReconstructedParticle::get_mass(zed_leptonic)")

//             // create branch with zed transverse momentum
//             .Define("zed_leptonic_pt", "ReconstructedParticle::get_pt(zed_leptonic)")

//             // calculate recoil of zed_leptonic
//             .Define("zed_leptonic_recoil", "ReconstructedParticle::recoilBuilder(240)(zed_leptonic)")

//             // create branch with recoil mass
//             .Define("zed_leptonic_recoil_m", "ReconstructedParticle::get_mass(zed_leptonic_recoil)")

//             // create branch with leptonic charge
//             .Define("zed_leptonic_charge", "ReconstructedParticle::get_charge(zed_leptonic)")

//             // Filter at least one candidate
//             .Filter("zed_leptonic_recoil_m.size() > 0");

//             return df2;

//     }
// };


// std::vector<std::string> output() {
//     std::vector<std::string> branchList = {
//         "selected_muons_pt",
//         "selected_muons_y",
//         "selected_muons_p",
//         "selected_muons_e",
//         "zed_leptonic_pt",
//         "zed_leptonic_m",
//         "zed_leptonic_charge",
//         "zed_leptonic_recoil_m"
//     };
//     return branchList;
// }
 
//  ROOT::gInterpreter.Declare("""                                                                                                                                                                                  
//  float MyMinEnergy(ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in) {                                                                                                                                 
//     float min=999999.;                                                                                                                                                                                          
//     for (auto & p: in) {                                                                                                                                                                                        
//       if (p.energy<min && p.energy>0) min=p.energy;                                                                                                                                                              
//     }                                                                                                                                                                                                            
//     return min;                                                                                                                                                                                                  
//  }                                                                                                                                                                                                              
//  """) 

// TInterpreter* interpreter = new TInterpreter


auto isOne = [] (int a) { return a == 1; };

int main(int args, char* argv[]) {

    auto reconstructedParticle = [] (ROOT::VecOps::RVec<int> index, ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in) { return FCCAnalyses::ReconstructedParticle::get(index, in); };

    std::string testFile = "root://eospublic.cern.ch//eos/experiment/fcc/ee/generation/DelphesEvents/spring2021/IDEA/p8_ee_ZH_ecm240/events_101027117.root";

    ROOT::EnableImplicitMT();

    ROOT::RDataFrame df("events", testFile);
    auto testHisto_ptr = df
    .Alias("Muon0", "Muon#0.index")
    .Define("muons", FCCAnalyses::ReconstructedParticle::get, {"Muon0", "ReconstructedParticles"})
    .Define("selected_muons", FCCAnalyses::ReconstructedParticle::sel_pt(10.), {"muons"})
    .Define("selected_muons_pt", FCCAnalyses::ReconstructedParticle::get_pt, {"selected_muons"})
    .Histo1D("selected_muons_pt");

    TCanvas* c = new TCanvas();
    testHisto_ptr->Print();
    testHisto_ptr->Draw();
    testHisto_ptr->SaveAs("testHisto.pdf");
    c->Draw();

    delete c;


    return 0;
}
