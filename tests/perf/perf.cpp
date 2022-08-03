#include "ROOT/RDataFrame.hxx"
#include <iostream>
#include <chrono>
#include "stdlib.h"
#include "vector"
#include "string"
#include "TCanvas.h"
#include "TInterpreter.h"
#include "TH1D.h"
#include "FCCAnalyses/ReconstructedParticle.h"


//#define nCPUS 4
#define outputDir "/home/imelnyk/FCCAnalyses/build/tests/perf/"
#define outputFile "outputTest.root"
#define inputDir "/home/imelnyk/FCCAnalyses/test_rootdir/"

using namespace ROOT;



void perf(int nThreads) {

    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

    auto reconstructedParticle = [] (ROOT::VecOps::RVec<int> index, ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in) {
        return FCCAnalyses::ReconstructedParticle::get(index, in); };

    std::string testFile = "root://eospublic.cern.ch//eos/experiment/fcc/ee/generation/DelphesEvents/spring2021/IDEA/p8_ee_ZH_ecm240/events_101027117.root";

    ROOT::EnableImplicitMT(nThreads);

    std::string rootFiles = inputDir + std::string("*");

    ROOT::RDataFrame df("events", rootFiles);
    auto df2 = df
    .Alias("Muon0", "Muon#0.index")
    .Define("muons", FCCAnalyses::ReconstructedParticle::get, {"Muon0", "ReconstructedParticles"})
    .Define("selected_muons", FCCAnalyses::ReconstructedParticle::sel_pt(10.), {"muons"})
    .Define("selected_muons_pt", FCCAnalyses::ReconstructedParticle::get_pt, {"selected_muons"})

    .Define("selected_muons_y", FCCAnalyses::ReconstructedParticle::get_y, {"selected_muons"})


    .Define("selected_muons_e", FCCAnalyses::ReconstructedParticle::get_e, {"selected_muons"})
    .Define("zed_leptonic", FCCAnalyses::ReconstructedParticle::resonanceBuilder(91.), {"selected_muons"})
    .Define("zed_leptonic_m", FCCAnalyses::ReconstructedParticle::get_mass, {"zed_leptonic"})
    .Define("zed_leptonic_pt", FCCAnalyses::ReconstructedParticle::get_pt, {"zed_leptonic"})
    .Define("zed_leptonic_recoil", FCCAnalyses::ReconstructedParticle::recoilBuilder(240.), {"zed_leptonic"})
    .Define("zed_leptonic_recoil_m", FCCAnalyses::ReconstructedParticle::get_mass, {"zed_leptonic_recoil"})
    .Define("zed_leptonic_charge", FCCAnalyses::ReconstructedParticle::get_charge, {"zed_leptonic"})
    .Filter("zed_leptonic_recoil_m.size() > 0", "zed_leptonic_recoil_m")
    .Snapshot("events", std::string(outputDir) + std::string(outputFile), {
    "muons",
    "selected_muons",
    "selected_muons_pt",
    "selected_muons_y",
    "selected_muons_e",
    "zed_leptonic",
    "zed_leptonic_m",
    "zed_leptonic_pt",
    "zed_leptonic_recoil",
    "zed_leptonic_recoil_m",
    "zed_leptonic_charge"});

    auto df3 = df
    .Alias("Muon0", "Muon#0.index")
    .Define("muons", FCCAnalyses::ReconstructedParticle::get, {"Muon0", "ReconstructedParticles"})
    .Define("selected_muons", FCCAnalyses::ReconstructedParticle::sel_pt(10.), {"muons"})
    .Define("selected_muons_p", FCCAnalyses::ReconstructedParticle::get_e, {"selected_muons"});
//    .Histo1D("selected_muons_pt");

//    TCanvas* c = new TCanvas();
//    df2->Print();
//    df2->Draw();
//    c->Print(outputDir + std::string("testHisto.pdf"));

//    delete c;

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << std::endl;

}

int main(int args, char* argv[]) {

    int nCPUS = atoi(argv[1]);

    std::cout << "nThreads:" << nCPUS << std::endl;

    perf(nCPUS);

    return 0;
}