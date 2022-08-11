#include "ROOT/RDataFrame.hxx"
#include <iostream>
#include <chrono>
#include "stdlib.h"
#include "vector"
#include "string"
#include "TCanvas.h"
#include "TChain.h"
#include "TInterpreter.h"
#include "TH1D.h"
#include "FCCAnalyses/ReconstructedParticle.h"
#include <filesystem>
//#include "TSeqCollection.h"
#include "TROOT.h"
//#include "TIter.h"


//#define nCPUS 4
#define outputDir "/home/imelnyk/FCCAnalyses/build/tests/perf/"
#define outputFile "outputTest.root"
//#define inputDir "/home/imelnyk/FCCAnalyses/p8_ee_ZH_ecm240/"
#define inputDir "/tmp/fccperf/p8_ee_ZH_ecm240/"
#define sampleSize 100
#define isTest false

using namespace ROOT;

TChain* retrieveData(int breakPoint) {

    std::string testFile = "root://eospublic.cern.ch//eos/experiment/fcc/ee/generation/DelphesEvents/spring2021/IDEA/p8_ee_ZH_ecm240/events_101027117.root"; // use testfile
    std::vector<std::string> files;
    TChain* rootChain = new TChain("events");


// Example of event
// 0, 10_000
// rand = 5_001
// end = sqrt(rand)
// start = 2
//
// for i in range(start, end + 1, 1):
//     result = rand % i
//     if (result == 0):
//         print("not prime")

    if (isTest) {

        rootChain->Add(testFile.c_str());
        std::cout << "#############################################################################################################" << std::endl;
        std::cout << "#############################################################################################################" << std::endl;
        std::cout << "  Test file " << testFile.c_str() << " was added to the ROOT::TChain object " << std::endl;
        std::cout << "#############################################################################################################" << std::endl;
        std::cout << "#############################################################################################################" << std::endl;
    }
    else {

        std::cout << "#############################################################################################################" << std::endl;
        std::cout << "#############################################################################################################" << std::endl;
        std::cout << "                       Files retrieving starts: " << breakPoint << " files are going to be processed         " << std::endl;
        std::cout << "#############################################################################################################" << std::endl;
        std::cout << "#############################################################################################################" << std::endl;
        std::cout << std::endl;

        int currentPoint = 0;
        for (const auto & entry : std::filesystem::directory_iterator(inputDir)) {
            if (currentPoint == breakPoint) break;
            std::string file = entry.path().string();
            rootChain->Add(file.c_str());
            std::cout << "  File " << file.c_str() << " was added to the ROOT::TChain object " << std::endl;
            currentPoint++;
        }

        std::cout << std::endl;
        std::cout << "#############################################################################################################" << std::endl;
        std::cout << "#############################################################################################################" << std::endl;
        std::cout << "                       Files retrieving was finished: " << breakPoint << " files had been added          " << std::endl;
        std::cout << "#############################################################################################################" << std::endl;
        std::cout << "#############################################################################################################" << std::endl;
        std::cout << std::endl;

    }

    return rootChain;

}



void perf(int nThreads) {

    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();



    auto reconstructedParticle = [] (ROOT::VecOps::RVec<int> index, ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in) {
        return FCCAnalyses::ReconstructedParticle::get(index, in); };



    TChain* chainZH = retrieveData(sampleSize);
//    Long64_t filesSize = TFile::GetFileBytesRead();
    int eventsTotal = chainZH->GetEntries();

    ROOT::EnableImplicitMT(nThreads);

//    std::string rootFiles = inputDir + std::string("*");

    ROOT::RDataFrame df(*chainZH);

    auto df2 = df
    .Alias("Muon0", "Muon#0.index")
    .Define("muons", FCCAnalyses::ReconstructedParticle::get, {"Muon0", "ReconstructedParticles"})
    .Define("selected_muons", FCCAnalyses::ReconstructedParticle::sel_pt(10.), {"muons"});
//    .Define("selected_muons_pt", FCCAnalyses::ReconstructedParticle::get_pt, {"selected_muons"}) +++
//
//    .Define("selected_muons_y", FCCAnalyses::ReconstructedParticle::get_y, {"selected_muons"}) +++
//
//
//    .Define("selected_muons_e", FCCAnalyses::ReconstructedParticle::get_e, {"selected_muons"}) +++
//    .Define("zed_leptonic", FCCAnalyses::ReconstructedParticle::resonanceBuilder(91.), {"selected_muons"}) +++
//    .Define("zed_leptonic_m", FCCAnalyses::ReconstructedParticle::get_mass, {"zed_leptonic"}) +++
//    .Define("zed_leptonic_pt", FCCAnalyses::ReconstructedParticle::get_pt, {"zed_leptonic"}) +++
//    .Define("zed_leptonic_recoil", FCCAnalyses::ReconstructedParticle::recoilBuilder(240.), {"zed_leptonic"}) +++
//    .Define("zed_leptonic_recoil_m", FCCAnalyses::ReconstructedParticle::get_mass, {"zed_leptonic_recoil"}) +++
//    .Define("zed_leptonic_charge", FCCAnalyses::ReconstructedParticle::get_charge, {"zed_leptonic"}) +++
//    .Filter("zed_leptonic_recoil_m.size() > 0", "zed_leptonic_recoil_m") +++
//    .Snapshot("events", std::string(outputDir) + std::string(outputFile), { +++
//    "muons",
//    "selected_muons",
//    "selected_muons_pt",
//    "selected_muons_y",
//    "selected_muons_e",
//    "zed_leptonic",
//    "zed_leptonic_m",
//    "zed_leptonic_pt",
//    "zed_leptonic_recoil",
//    "zed_leptonic_recoil_m",
//    "zed_leptonic_charge"});

    auto df3 = df2
    .Define("selected_muons_pt", FCCAnalyses::ReconstructedParticle::get_pt, {"selected_muons"});

    auto df4 = df3
    .Define("selected_muons_y", FCCAnalyses::ReconstructedParticle::get_y, {"selected_muons"});

    auto df5 = df4
    .Define("selected_muons_p", FCCAnalyses::ReconstructedParticle::get_p, {"selected_muons"});

    auto df6 = df5
    .Define("selected_muons_e", FCCAnalyses::ReconstructedParticle::get_e, {"selected_muons"});

    auto df7 = df6
    .Define("zed_leptonic", FCCAnalyses::ReconstructedParticle::resonanceBuilder(91.), {"selected_muons"});

    auto df8 = df7
    .Define("zed_leptonic_m", FCCAnalyses::ReconstructedParticle::get_mass, {"zed_leptonic"});

    auto df9 = df8
    .Define("zed_leptonic_pt", FCCAnalyses::ReconstructedParticle::get_pt, {"zed_leptonic"});

    auto df10 = df9
    .Define("zed_leptonic_recoil", FCCAnalyses::ReconstructedParticle::recoilBuilder(240.), {"zed_leptonic"});

    auto df11 = df10
    .Define("zed_leptonic_recoil_m", FCCAnalyses::ReconstructedParticle::get_mass, {"zed_leptonic_recoil"});

    auto df12 = df11
    .Define("zed_leptonic_charge", FCCAnalyses::ReconstructedParticle::get_charge, {"zed_leptonic"});

    auto df13 = df12
    .Filter("zed_leptonic_recoil_m.size() > 0", "zed_leptonic_recoil_m");

    auto df14 = df13
    .Snapshot("events", std::string(outputDir) + std::string(outputFile), {
    "muons",
    "selected_muons",
    "selected_muons_pt",
    "selected_muons_y",
    "selected_muons_p",
    "selected_muons_e",
    "zed_leptonic",
    "zed_leptonic_m",
    "zed_leptonic_pt",
    "zed_leptonic_recoil",
    "zed_leptonic_recoil_m",
    "zed_leptonic_charge"});

//    TObjArray* filesArray = chainZH->GetListOfFiles();
//    Int_t nFiles = filesArray->GetEntries();
//    std::cout << "N of files is " << nFiles << std::endl;

//    TSeqCollection* listOfFiles = gROOT->GetListOfFiles();
//    TIter next(listOfFiles);
//    TGlobal* global;
//    int indexLast = listOfFiles->GetLast();
//    TFile* file;
//    while (file = (TFile*)next()) {

//        TObject* obj =  listOfFiles->At(0);
//        TFile* file = dynamic_cast<TFile*>(obj);
//        std::cout << "Number of opened files: " << indexLast << std::endl;
//        std::cout << "Class name: " << file->ClassName() << "    " << "Title: " << file->GetTitle() << "    " << "Name: " << file->GetName() << std::endl;
//    }



//    long filesSize = 0.0;
//    gROOT->

//    for (Int_t i = 0; i < nFiles; i++) {
////        auto file = filesArray[i];
//
////        TFile* rootFile = static_cast<TFile*>(filesArray->At(i));
//        std::cout << "Name = " << filesArray->At(i)->GetName() << std::endl;
//        std::cout << "Class = " << filesArray->At(i)->ClassName() << std::endl;
//        std::cout << "Title = " << filesArray->At(i)->GetTitle() << std::endl;
//
//        TFile* rootFile = new TFile(filesArray->At(i)->GetTitle());
//
//        std::cout << "Bytes = " << rootFile->GetSize() << std::endl;
//        filesSize += rootFile->GetSize();
//        std::cout << "ROOT TFile* " << rootFile << std::endl;
//    }
//
//    //    std::cout << "gROOT: " << std::endl;
//
//    std::cout << "Size of all the files is " << filesSize << std::endl;


    delete chainZH;

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << std::endl;
    std::cout << "Total events: " << eventsTotal << std::endl;

}

int main(int args, char* argv[]) {

    std::cout << "Programme start" << std::endl;

    int nCPUS = atoi(argv[1]);

    std::cout << "nThreads:" << nCPUS << std::endl;

    perf(nCPUS);

    std::cout << "Programme end" << std::endl;
    return 0;
}