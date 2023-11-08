#include <iostream>
#include <TFile.h>
#include <TSystem.h>
#include <TString.h>


struct Data {
    int run_number;
    int energy;
    float eta;
};

std::string get_run(TString ss1){
        std::string ss(ss1.Data());
        std:: string substring1 = "tiletb_";
        std:: string substring2 = ".root";
        std::size_t ind = ss.find(substring1);
        ss.erase(ind,substring1.length());
        std::size_t ind1 = ss.find(substring2);
        ss.erase(ind1,substring2.length());

        return ss;
}



int tb() {

// open the file that contains run numbers and eta values

    std::ifstream inputFile("run_numbers.csv");
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    // Define vectors to store the data
    std::vector<int> run_number, energy;
    std::vector<float> eta;

    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream lineStream(line);
        std::string cell;
        std::vector<std::string> row;

        while (std::getline(lineStream, cell, ',')) {
            row.push_back(cell);
        }

        if (row.size() >= 6) {
            // Convert the run numbers to an integer
            int intValue;
            if (std::stringstream(row[0]) >> intValue) {
                run_number.push_back(intValue);
            } else {
                std::cerr << "Error: Failed to convert the run numbers to an integer." << std::endl;
                return 1;
            }

            // Convert the eta values to a float
            float floatValue;
            if (std::stringstream(row[5]) >> floatValue) {
                eta.push_back(floatValue);
            } else {
                std::cerr << "Error: Failed to convert the eta values to a float." << std::endl;
                return 1;
            }
            // Convert the energy values to an integer
            int intValue1;
            if (std::stringstream(row[2]) >> intValue1) {
                energy.push_back(intValue1);
            } else {
                std::cerr << "Error: Failed to convert the energy values to an integer." << std::endl;
                return 1;
            }

        } else {
            std::cerr << "Error: The file does not have at least 6 columns in each row." << std::endl;
            return 1;
        }
    }

    // Now you can work with the run_number and eta vectors as needed.

    // Print some sample data from these columns
    // std::cout << "Run numbers:" << std::endl;
    // for (int value : run_number) {
    //     std::cout << value << std::endl;
    // }

    // std::cout << "Eta:" << std::endl;
    // for (float value : eta) {
    //     std::cout << value << std::endl;
    // }

    // std::cout << "Energy:" << std::endl;
    // for (int value : energy) {
    //     std::cout << value << std::endl;
    // }

    std::vector<Data> syncData;

    size_t size = run_number.size();

    syncData.reserve(size);

    for (size_t i = 0; i < size; ++i) {
        syncData.push_back({run_number[i], energy[i], eta[i]});
    }

    // for (const Data& item : syncData) {
    //         std::cout << "Run Number: " << item.run_number << ", Energy: " << item.energy << ", Eta: " << item.eta << std::endl;
    //     }



    int num_of_files = 49; // Change to the number of files in the folder



    for (int rn = 0; rn < num_of_files; ++rn) {

                TString filePath = TString("root_files/tiletb_") + TString(Form("%d", syncData[rn].run_number)) + TString(".root");
                TString fileName = TString("tiletb_") + TString(Form("%d", syncData[rn].run_number)) + TString(".root");



                std::cout << "\n\nProcessing file N " << rn << ": " << filePath << std::endl;
                std::cout << "      run_number: " << syncData[rn].run_number << std::endl;
                std::cout << "      energy: " << syncData[rn].energy << std::endl;
                std::cout << "      eta: " << syncData[rn].eta << std::endl;
                
                

// Process the ROOT file here

                TFile* rootFile = TFile::Open(filePath, "READ");
               

                // Do your file-specific operations here                    
                
                TTree *tree = (TTree*)rootFile->Get("h1000");
                std::cout << "      Total event number: " << tree->GetEntries() << std::endl;

                Float_t  EfitC02[48],EfitC01[48],EfitE03[48], Xcha1_0, Ycha1_0;
                Short_t S1cou, S2cou, Cher1, Cher2, Cher3;

                tree->SetBranchAddress("EfitC02", EfitC02);
                tree->SetBranchAddress("EfitC01", EfitC01);
                tree->SetBranchAddress("EfitE03", EfitE03);
                tree->SetBranchAddress("S1cou", &S1cou);
                tree->SetBranchAddress("S2cou", &S2cou);
                tree->SetBranchAddress("Cher1", &Cher1);
                tree->SetBranchAddress("Cher2", &Cher2);
                tree->SetBranchAddress("Cher3", &Cher3);
                tree->SetBranchAddress("Xcha1_0", &Xcha1_0);
                tree->SetBranchAddress("Ycha1_0", &Ycha1_0);

                std::vector<std::vector<Float_t>> C02(48), C01(48), E03(48);
                std::vector<Short_t> Ch1, Ch2, Ch3, S1, S2;
                std::vector<Float_t> Xcha1, Ycha1;

                TH1F *energy_tot_h = new TH1F("Energy for all Modules","Energy for all Modules",100,0,syncData[rn].energy*1000*1.5);
                TH1F *S1_hist = new TH1F("S1","S1",100,0,10000);
                TH1F *S2_hist = new TH1F("S2","S2",100,0,10000);
                TH1F *Ch1_hist = new TH1F("Ch1","Ch1",100,0,4000);
                TH1F *Ch2_hist = new TH1F("Ch2","Ch2",100,0,4000);
                TH1F *Ch3_hist = new TH1F("Ch3","Ch3",100,0,4000);
                TH1F *Xcha1_hist = new TH1F("Xcha1","Xcha1",100,0,1000);
                TH1F *Ycha1_hist = new TH1F("Ycha1","Ycha1",100,0,6000);

                TH2F *Ch1_Ch2_h = new TH2F("Ch1_Ch2","Ch1_Ch2",100,0,4000,100,0,4000);
                TH2F *Ch1_Ch3_h = new TH2F("Ch1_Ch3","Ch1_Ch3",100,0,4000,100,0,4000);
                TH2F *Ch2_Ch3_h = new TH2F("Ch2_Ch3","Ch2_Ch3",100,0,4000,100,0,4000);
                TH2F *S1_Etot_h = new TH2F("S1_Etot","S1_Etot",100,0,10000,100,0,syncData[rn].energy*1000*1.5);
                TH2F *S2_Etot_h = new TH2F("S2_Etot","S2_Etot",100,0,10000,100,0,syncData[rn].energy*1000*1.5);

                TH1F *Clong_hist = new TH1F("clong","clong",100,0,1.6);
                TH1F *Ctot_hist = new TH1F("ctot","ctot",100,0,0.2);
                TH2F *Clong_Ctot_h = new TH2F("Clong_vs_Ctot", "C_{long} vs C_{tot}", 300,0,0.2, 300,0,1.6);

                const int nHistograms = 48;
                TH1F *C01_h[nHistograms];
                TH1F *C02_h[nHistograms];
                TH1F *E03_h[nHistograms];
                for (int i = 0; i < nHistograms; ++i) {
                    C01_h[i] = new TH1F(Form("C01_h_%d", i), Form("EfitC01 %d", i), 100, 0, syncData[rn].energy*1000*1.5);
                    C02_h[i] = new TH1F(Form("C02_h_%d", i), Form("EfitC02 %d", i), 100, 0, syncData[rn].energy*1000*1.5);
                    E03_h[i] = new TH1F(Form("E03_h_%d", i), Form("EfitE03 %d", i), 100, 0, syncData[rn].energy*1000*1.5);
                }
                
// EVENT LOOP !!!

                for (int i = 0; i < tree->GetEntries(); ++i) {
                    tree->GetEntry(i);
                    
                    if (S1cou < 1000 || S2cou < 1000) continue;
                    
                    if (i%10000==0){
                        std::cout << "Event " << i << std::endl;
                    }
                    for(int j=0;j< 48; ++j){

                        C02[j].push_back(EfitC02[j]);
                        C01[j].push_back(EfitC01[j]);
                        E03[j].push_back(EfitE03[j]);
                        
                        C01_h[j]->Fill(EfitC01[j]);
                        C02_h[j]->Fill(EfitC02[j]);
                        E03_h[j]->Fill(EfitE03[j]);
                    }

                    S1.push_back(S1cou);
                    S2.push_back(S2cou);
                    Ch1.push_back(Cher1);
                    Ch2.push_back(Cher2);
                    Ch3.push_back(Cher3);
                    Xcha1.push_back(Xcha1_0);
                    Ycha1.push_back(Ycha1_0);

                }  

// END OF EVENT LOOP !!!

                std::vector<Float_t> E_Sum_C02(C02[1].size()), E_Sum(C02[1].size());
                // Check the vectores

                // for (int i = 0; i < 48; ++i) {
                //     //std::cout << "Vector " << i << " size: " << C02[i].size() << std::endl;

                //     for (int j=0; j<C02[i].size(); ++j) {
                //         E_Sum_C02[j]+=C02[i][j];
                //     }
                // }


                for (int i = 0; i < 48; ++i) {
                    //std::cout << "Vector " << i << " size: " << C02[i].size() << std::endl;

                    for (int j=0; j<C02[i].size(); ++j) {
                        E_Sum[j]+=C02[i][j] + C01[i][j] + E03[i][j];
                    }
                }
                int size = 0;
                for (float val : E_Sum) {size++;}

                for (int i=0; i < S1.size(); ++i) {
                    energy_tot_h->Fill(E_Sum[i]);
                    S1_hist->Fill(S1[i]);
                    S2_hist->Fill(S2[i]);
                    Ch1_hist->Fill(Ch1[i]);
                    Ch2_hist->Fill(Ch2[i]);
                    Ch3_hist->Fill(Ch3[i]);
                    Xcha1_hist->Fill(Xcha1[i]);
                    Ycha1_hist->Fill(Ycha1[i]);
                    Ch1_Ch2_h->Fill(Ch1[i],Ch2[i]);
                    Ch1_Ch3_h->Fill(Ch1[i],Ch3[i]);
                    Ch2_Ch3_h->Fill(Ch2[i],Ch3[i]);
                    S1_Etot_h->Fill(S1[i],E_Sum[i]);
                    S2_Etot_h->Fill(S2[i],E_Sum[i]);
                }

                // Define the stuff for the C_long and C_tot

                std::vector<Float_t> C_long(size), C_tot(size), C_tot_cells(size), C_tot_avg(size), C_tot_sum(size);


                std::vector<Int_t> C01_Clong, C02_Clong, E03_Clong, C01_Ctot, C02_Ctot, E03_Ctot;

                // 2 layers for C_long and 3 layers for C_tot
                if (std::abs(syncData[rn].eta * 100) == 5) {

                    std::cout << "in Eta = 0.05" << std::endl;    

                    // A and BC layer

                    C01_Clong = {1, 4, 5, 8, 2, 3, 6, 7};
                    C02_Clong = {1, 4, 5, 8, 2, 3, 6, 7};
                    E03_Clong = {4, 5, 6, 7, 8, 9, 2, 3};

                    // A layer only

                    // C01_Clong = {1, 4, 5, 8};
                    // C02_Clong = {1, 4, 5, 8};
                    // E03_Clong = {4, 5, 6, 7};

                    C01_Ctot = {1, 4, 5, 8, 2, 3, 6, 7, 0, 13, 14};
                    C02_Ctot = {1, 4, 5, 8, 2, 3, 6, 7, 0, 13, 14};
                    E03_Ctot = {4, 5, 6, 7, 8, 9, 2, 3, 16, 17, 30};
                }
               // std::cout << "syncData[rn].eta * 100 = " << std::abs(syncData[rn].eta * 100) << std::endl;
                int tiv = std::abs(syncData[rn].eta * 100);
                if (tiv == 15) {

                    std::cout << "in Eta = 0.15" << std::endl;
                    
                    // A and BC layer

                    C01_Clong = {5, 8, 1, 4, 9, 10, 2, 3, 6, 7, 11, 12};
                    C02_Clong = {5, 8, 1, 4, 9, 10, 2, 3, 6, 7, 11, 12};
                    E03_Clong = {6, 7, 10, 11, 4, 5, 8, 9, 14, 15, 2, 3};

                    // A layer only
                    
                    // C01_Clong = {5, 8, 1, 4, 9, 10};
                    // C02_Clong = {5, 8, 1, 4, 9, 10};
                    // E03_Clong = {6, 7, 10, 11, 4, 5};

                    C01_Ctot = {5, 8, 1, 4, 9, 10, 2, 3, 6, 7, 11, 12, 0, 13, 14};
                    C02_Ctot = {5, 8, 1, 4, 9, 10, 2, 3, 6, 7, 11, 12, 0, 13, 14};
                    E03_Ctot = {6, 7, 10, 11, 4, 5, 8, 9, 14, 15, 2, 3, 16, 17, 30};

                }

                if (std::abs(syncData[rn].eta * 100) == 25) {

                    std::cout << "in Eta = 0.25" << std::endl;

                    // A and BC layer

                    C01_Clong = {9, 10, 5, 8, 15, 18, 6, 7, 11, 12, 16, 17};
                    C02_Clong = {9, 10, 5, 8, 15, 18, 6, 7, 11, 12, 16, 17};
                    E03_Clong = {10, 11, 6, 7, 20, 21, 8, 9, 14, 15, 22, 23};

                    // A layer only

                    // C01_Clong = {9, 10, 5, 8, 15, 18};
                    // C02_Clong = {9, 10, 5, 8, 15, 18};
                    // E03_Clong = {10, 11, 6, 7, 20, 21};

                    C01_Ctot = {9, 10, 5, 8, 15, 18, 6, 7, 11, 12, 16, 17, 13, 14, 25, 26};
                    C02_Ctot = {9, 10, 5, 8, 15, 18, 6, 7, 11, 12, 16, 17, 13, 14, 25, 26};
                    E03_Ctot = {10, 11, 6, 7, 20, 21, 8, 9, 14, 15, 22, 23, 16, 17, 30, 31};
                }

                if (std::abs(syncData[rn].eta * 100) == 35) {

                    std::cout << "in Eta = 0.35" << std::endl;

                    // A and BC layer

                    C01_Clong = {9, 10, 15, 18, 19, 20, 11, 12, 16, 17, 21, 22};
                    C02_Clong = {9, 10, 15, 18, 19, 20, 11, 12, 16, 17, 21, 22};
                    E03_Clong = {10, 11, 6, 7, 20, 21, 8, 9, 14, 15, 22, 23};

                    // A layer only

                    // C01_Clong = {9, 10, 15, 18, 19, 20};
                    // C02_Clong = {9, 10, 15, 18, 19, 20};
                    // E03_Clong = {10, 11, 6, 7, 20, 21};

                    C01_Ctot = {9, 10, 15, 18, 19, 20, 11, 12, 16, 17, 21, 22, 13, 14, 25, 26};
                    C02_Ctot = {9, 10, 15, 18, 19, 20, 11, 12, 16, 17, 21, 22, 13, 14, 25, 26};
                    E03_Ctot = {10, 11, 6, 7, 20, 21, 8, 9, 14, 15, 22, 23, 16, 17, 30, 31};
                }

                if (std::abs(syncData[rn].eta * 100) == 45) {

                    std::cout << "in Eta = 0.45" << std::endl;

                    // A and BC layer

                    C01_Clong = {15, 18, 19, 20, 23, 24, 16, 17, 21, 22, 28, 29};
                    C02_Clong = {15, 18, 19, 20, 23, 24, 16, 17, 21, 22, 28, 29};
                    E03_Clong = {10, 11, 24, 25, 20, 21, 14, 15, 22, 23, 26, 27};

                    // A layer only

                    // C01_Clong = {15, 18, 19, 20, 23, 24};
                    // C02_Clong = {15, 18, 19, 20, 23, 24};
                    // E03_Clong = {10, 11, 24, 25, 20, 21};

                    C01_Ctot = {15, 18, 19, 20, 23, 24, 16, 17, 21, 22, 28, 29, 13, 14, 25, 26};
                    C02_Ctot = {15, 18, 19, 20, 23, 24, 16, 17, 21, 22, 28, 29, 13, 14, 25, 26};
                    E03_Ctot = {10, 11, 24, 25, 20, 21, 14, 15, 22, 23, 26, 27, 16, 17, 30, 31};
                }

                if (std::abs(syncData[rn].eta * 100) == 55) {

                    std::cout << "in Eta = 0.55" << std::endl;

                    // A and BC layer

                    C01_Clong = {19, 20, 23, 24, 27, 30, 21, 22, 28, 29, 34, 35};
                    C02_Clong = {19, 20, 23, 24, 27, 30, 21, 22, 28, 29, 34, 35};
                    E03_Clong = {32, 33, 24, 25, 20, 21, 22, 23, 26, 27, 34, 35};

                    // A layer only

                    // C01_Clong = {19, 20, 23, 24, 27, 30};
                    // C02_Clong = {19, 20, 23, 24, 27, 30};
                    // E03_Clong = {32, 33, 24, 25, 20, 21};

                    C01_Ctot = {19, 20, 23, 24, 27, 30, 21, 22, 28, 29, 34, 35, 25, 26, 39, 42};
                    C02_Ctot = {19, 20, 23, 24, 27, 30, 21, 22, 28, 29, 34, 35, 25, 26, 39, 42};
                    E03_Ctot = {32, 33, 24, 25, 20, 21, 22, 23, 26, 27, 34, 35, 16, 17, 30, 31};
                }

                if (std::abs(syncData[rn].eta * 100) == 65) {

                    std::cout << "in Eta = 0.65" << std::endl;

                    // A and BC layer

                    C01_Clong = {33, 36, 23, 24, 27, 30, 28, 29, 34, 35, 40, 41};
                    C02_Clong = {33, 36, 23, 24, 27, 30, 28, 29, 34, 35, 40, 41};
                    E03_Clong = {32, 33, 24, 25, 20, 21, 22, 23, 26, 27, 34, 35};

                    // A layer only

                    // C01_Clong = {33, 36, 23, 24, 27, 30};
                    // C02_Clong = {33, 36, 23, 24, 27, 30};
                    // E03_Clong = {32, 33, 24, 25, 20, 21};

                    C01_Ctot = {33, 36, 23, 24, 27, 30, 28, 29, 34, 35, 40, 41, 25, 26, 39, 42};
                    C02_Ctot = {33, 36, 23, 24, 27, 30, 28, 29, 34, 35, 40, 41, 25, 26, 39, 42};
                    E03_Ctot = {32, 33, 24, 25, 20, 21, 22, 23, 26, 27, 34, 35, 16, 17, 30, 31};
                }


                int Ncell = (C01_Ctot.size() + C02_Ctot.size() + E03_Ctot.size())/2;

                Float_t alpha;
                alpha = 1;
        
                
                for (int i = 0; i < C02_Clong.size(); i++) {
                    int index1 = C01_Clong[i];
                    int index2 = C02_Clong[i];
                    int index3 = E03_Clong[i];

                    
                    for (int j=0; j < C02[index2].size(); ++j) {
                        C_long[j] += C01[index1][j] + C02[index2][j] + E03[index3][j];
                    }
                }


                // Check C_long
                //std::cout<<"Clong size = "<<C_long.size()<<std::endl;

                for (Float_t val : C_long) {
                        val/=syncData[rn].energy*1000;
                        Clong_hist->Fill(val);
                }

                for (int i = 0; i < C02_Ctot.size(); i++) {
                    int index1 = C01_Ctot[i];
                    int index2 = C02_Ctot[i];
                    int index3 = E03_Ctot[i];

                        for (int j=0; j < C02[index2].size(); ++j) {
                            C_tot_cells[j] += pow(C01[index1][j], alpha) + pow(C02[index2][j], alpha) + pow(E03[index3][j], alpha);
                    }
                }
                for (Float_t val : C_tot_cells) {
                        val/=Ncell;
                        C_tot_avg.push_back(val);
                }

                for (int i = 0; i < C02_Ctot.size(); i++) {
                    int index1 = C01_Ctot[i];
                        for (int j=0; j < C02[index1].size(); ++j) {
                            C_tot_sum[j] += pow((pow(C01[index1][j], alpha) - C_tot_avg[j]), 2);
                    }
                }
                for (int i = 0; i < C02_Ctot.size(); i++) {
                    int index2 = C02_Ctot[i];
                        for (int j=0; j < C02[index2].size(); ++j) {
                            C_tot_sum[j] += pow((pow(C02[index2][j], alpha) - C_tot_avg[j]), 2);
                    }
                }
                for (int i = 0; i < C02_Ctot.size(); i++) {
                    int index3 = E03_Ctot[i];
                        for (int j=0; j < E03[index3].size(); ++j) {
                            C_tot_sum[j] += pow((pow(E03[index3][j], alpha) - C_tot_avg[j]), 2);
                    }
                }

                for(int i=0; i< C_tot_sum.size(); ++i) {
                    C_tot[i] = (sqrt(C_tot_sum[i]/Ncell))/C_tot_cells[i];

                }


                // Check C_tot

                for (Float_t val : C_tot) {
                        Ctot_hist->Fill(val);
                }


                for(int i=0; i< C_tot.size(); ++i) {
                    C_long[i]/=syncData[rn].energy*1000;
                    Clong_Ctot_h->Fill(C_tot[i], C_long[i]);
                   
                }


                // PLOTTING THE HISTOGRAMS !!!

                TCanvas* canvas[16];
                for (int i = 0; i < 16; i++) {
                    canvas[i] = new TCanvas(Form("canvas%d", i), Form("Canvas %d", i), 800, 600);
                }

                // Create a TLatex object to add text

                TString text1 = Form("Run: %lld", static_cast<long long>(syncData[rn].run_number));
                TString text2 = Form("Entries: %lld", static_cast<long long>(tree->GetEntries()));
                TString text3 = Form("Energy: %lld GeV", static_cast<long long>(syncData[rn].energy));
                TString text4 = Form("Eta: %g", syncData[rn].eta);


                TLatex latex;
                latex.SetNDC();
                latex.SetTextFont(42); // Font type
                latex.SetTextSize(0.03); // Text size

                

                canvas[0]->cd();
                energy_tot_h->GetXaxis()->SetTitle("Energy [MeV]");
                energy_tot_h->GetYaxis()->SetTitle("Counts");
                energy_tot_h->SetStats(0);
                energy_tot_h->Draw();
                canvas[0]->Update();
                latex.DrawLatex(0.73, 0.83, text1.Data()); // Position and text
                latex.DrawLatex(0.73, 0.80, text2.Data());
                latex.DrawLatex(0.73, 0.77, text3.Data());
                latex.DrawLatex(0.73, 0.74, text4.Data());
                canvas[0]->SaveAs(Form("plots/energy_tot_h_%s.png", get_run(fileName).c_str()));
                
                canvas[1]->cd();
                S1_hist->GetXaxis()->SetTitle("S1");
                S1_hist->GetYaxis()->SetTitle("Counts");
                S1_hist->SetStats(0);
                S1_hist->Draw();
                canvas[1]->Update();
                latex.DrawLatex(0.73, 0.83, text1.Data()); // Position and text
                latex.DrawLatex(0.73, 0.80, text2.Data());
                latex.DrawLatex(0.73, 0.77, text3.Data());
                latex.DrawLatex(0.73, 0.74, text4.Data());
                canvas[1]->SaveAs(Form("plots/S1_hist_%s.png", get_run(fileName).c_str()));

                canvas[2]->cd();
                S2_hist->GetXaxis()->SetTitle("S2");
                S2_hist->GetYaxis()->SetTitle("Counts");
                S2_hist->SetStats(0);
                S2_hist->Draw();
                canvas[2]->Update();
                latex.DrawLatex(0.73, 0.83, text1.Data()); // Position and text
                latex.DrawLatex(0.73, 0.80, text2.Data());
                latex.DrawLatex(0.73, 0.77, text3.Data());
                latex.DrawLatex(0.73, 0.74, text4.Data());
                canvas[2]->SaveAs(Form("plots/S2_hist_%s.png", get_run(fileName).c_str()));

                canvas[3]->cd();
                Ch1_hist->GetXaxis()->SetTitle("Ch1");
                Ch1_hist->GetYaxis()->SetTitle("Counts");
                Ch1_hist->SetStats(0);
                Ch1_hist->Draw();
                canvas[3]->Update();
                latex.DrawLatex(0.73, 0.83, text1.Data()); // Position and text
                latex.DrawLatex(0.73, 0.80, text2.Data());
                latex.DrawLatex(0.73, 0.77, text3.Data());
                latex.DrawLatex(0.73, 0.74, text4.Data());
                canvas[3]->SaveAs(Form("plots/Ch1_hist_%s.png", get_run(fileName).c_str()));

                canvas[4]->cd();
                Ch2_hist->GetXaxis()->SetTitle("Ch2");
                Ch2_hist->GetYaxis()->SetTitle("Counts");
                Ch2_hist->SetStats(0);
                Ch2_hist->Draw();
                canvas[4]->Update();
                latex.DrawLatex(0.73, 0.83, text1.Data()); // Position and text
                latex.DrawLatex(0.73, 0.80, text2.Data());
                latex.DrawLatex(0.73, 0.77, text3.Data());
                latex.DrawLatex(0.73, 0.74, text4.Data());
                canvas[4]->SaveAs(Form("plots/Ch2_hist_%s.png", get_run(fileName).c_str()));

                canvas[5]->cd();
                Ch3_hist->GetXaxis()->SetTitle("Ch3");
                Ch3_hist->GetYaxis()->SetTitle("Counts");
                Ch3_hist->SetStats(0);
                Ch3_hist->Draw();
                canvas[5]->Update();
                latex.DrawLatex(0.73, 0.83, text1.Data()); // Position and text
                latex.DrawLatex(0.73, 0.80, text2.Data());
                latex.DrawLatex(0.73, 0.77, text3.Data());
                latex.DrawLatex(0.73, 0.74, text4.Data());
                canvas[5]->SaveAs(Form("plots/Ch3_hist_%s.png", get_run(fileName).c_str()));

                canvas[6]->cd();
                Xcha1_hist->GetXaxis()->SetTitle("Xcha1");
                Xcha1_hist->GetYaxis()->SetTitle("Counts");
                Xcha1_hist->SetStats(0);
                Xcha1_hist->Draw();
                canvas[6]->Update();
                latex.DrawLatex(0.73, 0.83, text1.Data()); // Position and text
                latex.DrawLatex(0.73, 0.80, text2.Data());
                latex.DrawLatex(0.73, 0.77, text3.Data());
                latex.DrawLatex(0.73, 0.74, text4.Data());
                canvas[6]->SaveAs(Form("plots/Xcha1_hist_%s.png", get_run(fileName).c_str()));

                canvas[7]->cd();
                Ycha1_hist->GetXaxis()->SetTitle("Ycha1");
                Ycha1_hist->GetYaxis()->SetTitle("Counts");
                Ycha1_hist->SetStats(0);
                Ycha1_hist->Draw();
                canvas[7]->Update();
                latex.DrawLatex(0.73, 0.83, text1.Data()); // Position and text
                latex.DrawLatex(0.73, 0.80, text2.Data());
                latex.DrawLatex(0.73, 0.77, text3.Data());
                latex.DrawLatex(0.73, 0.74, text4.Data());
                canvas[7]->SaveAs(Form("plots/Ycha1_hist_%s.png", get_run(fileName).c_str()));

                canvas[8]->cd();
                Ch1_Ch2_h->GetXaxis()->SetTitle("Ch1");
                Ch1_Ch2_h->GetYaxis()->SetTitle("Ch2");
                Ch1_Ch2_h->SetStats(0);
                Ch1_Ch2_h->Draw("colz");
                canvas[8]->Update();
                latex.DrawLatex(0.73, 0.83, text1.Data()); // Position and text
                latex.DrawLatex(0.73, 0.80, text2.Data());
                latex.DrawLatex(0.73, 0.77, text3.Data());
                latex.DrawLatex(0.73, 0.74, text4.Data());
                canvas[8]->SaveAs(Form("plots/Ch1_Ch2_h_%s.png", get_run(fileName).c_str()));

                canvas[9]->cd();
                Ch1_Ch3_h->GetXaxis()->SetTitle("Ch1");
                Ch1_Ch3_h->GetYaxis()->SetTitle("Ch3");
                Ch1_Ch3_h->SetStats(0);
                Ch1_Ch3_h->Draw("colz");
                canvas[9]->Update();
                latex.DrawLatex(0.73, 0.83, text1.Data()); // Position and text
                latex.DrawLatex(0.73, 0.80, text2.Data());
                latex.DrawLatex(0.73, 0.77, text3.Data());
                latex.DrawLatex(0.73, 0.74, text4.Data());
                canvas[9]->SaveAs(Form("plots/Ch1_Ch3_h_%s.png", get_run(fileName).c_str()));

                canvas[10]->cd();
                Ch2_Ch3_h->GetXaxis()->SetTitle("Ch2");
                Ch2_Ch3_h->GetYaxis()->SetTitle("Ch3");
                Ch2_Ch3_h->SetStats(0);
                Ch2_Ch3_h->Draw("colz");
                canvas[10]->Update();
                latex.DrawLatex(0.73, 0.83, text1.Data()); // Position and text
                latex.DrawLatex(0.73, 0.80, text2.Data());
                latex.DrawLatex(0.73, 0.77, text3.Data());
                latex.DrawLatex(0.73, 0.74, text4.Data());
                canvas[10]->SaveAs(Form("plots/Ch2_Ch3_h_%s.png", get_run(fileName).c_str()));

                canvas[11]->cd();
                S1_Etot_h->GetXaxis()->SetTitle("S1");
                S1_Etot_h->GetYaxis()->SetTitle("Energy [MeV]");
                S1_Etot_h->SetStats(0);
                S1_Etot_h->Draw("colz");
                canvas[11]->Update();
                latex.DrawLatex(0.73, 0.83, text1.Data()); // Position and text
                latex.DrawLatex(0.73, 0.80, text2.Data());
                latex.DrawLatex(0.73, 0.77, text3.Data());
                latex.DrawLatex(0.73, 0.74, text4.Data());
                canvas[11]->SaveAs(Form("plots/S1_Etot_h_%s.png", get_run(fileName).c_str()));

                canvas[12]->cd();
                S2_Etot_h->GetXaxis()->SetTitle("S2");
                S2_Etot_h->GetYaxis()->SetTitle("Energy [MeV]");
                S2_Etot_h->SetStats(0);
                S2_Etot_h->Draw("colz");
                canvas[12]->Update();
                latex.DrawLatex(0.73, 0.83, text1.Data()); // Position and text
                latex.DrawLatex(0.73, 0.80, text2.Data());
                latex.DrawLatex(0.73, 0.77, text3.Data());
                latex.DrawLatex(0.73, 0.74, text4.Data());
                canvas[12]->SaveAs(Form("plots/S2_Etot_h_%s.png", get_run(fileName).c_str()));

                canvas[13]->cd();
                Clong_hist->GetXaxis()->SetTitle("C_{long}");
                Clong_hist->GetYaxis()->SetTitle("Counts");
                Clong_hist->SetStats(0);
                Clong_hist->Draw();
                canvas[13]->Update();
                latex.DrawLatex(0.73, 0.83, text1.Data()); // Position and text
                latex.DrawLatex(0.73, 0.80, text2.Data());
                latex.DrawLatex(0.73, 0.77, text3.Data());
                latex.DrawLatex(0.73, 0.74, text4.Data());
                canvas[13]->SaveAs(Form("clong_ctot/Clong_hist_%s.png", get_run(fileName).c_str()));

                canvas[14]->cd();
                Ctot_hist->GetXaxis()->SetTitle("C_{tot}");
                Ctot_hist->GetYaxis()->SetTitle("Counts");
                Ctot_hist->SetStats(0);
                Ctot_hist->Draw();
                canvas[14]->Update();
                latex.DrawLatex(0.73, 0.83, text1.Data()); // Position and text
                latex.DrawLatex(0.73, 0.80, text2.Data());
                latex.DrawLatex(0.73, 0.77, text3.Data());
                latex.DrawLatex(0.73, 0.74, text4.Data());
                canvas[14]->SaveAs(Form("clong_ctot/Ctot_hist_%s.png", get_run(fileName).c_str()));

                canvas[15]->cd();
                Clong_Ctot_h->GetXaxis()->SetTitle("C_{tot}");
                Clong_Ctot_h->GetYaxis()->SetTitle("C_{long}");
                Clong_Ctot_h->SetStats(0);
                Clong_Ctot_h->Draw("colz");
                canvas[15]->Update();
                latex.DrawLatex(0.73, 0.83, text1.Data()); // Position and text
                latex.DrawLatex(0.73, 0.80, text2.Data());
                latex.DrawLatex(0.73, 0.77, text3.Data());
                latex.DrawLatex(0.73, 0.74, text4.Data());
                canvas[15]->SaveAs(Form("clong_ctot/Clong_Ctot_h_%s.png", get_run(fileName).c_str()));

                                
            // Energy for each cell

                // const int nCanvases = 48;
                // TCanvas *c[nCanvases] ;
                // for (int i = 0; i < nCanvases; ++i) {
                //     c[i] = new TCanvas(Form("c%d", i), Form("Canvas %d", i));
                // }

                // for (int i = 0; i < nCanvases; ++i) {
                //     c[i]->cd();
                //     C02_h[i]->GetXaxis()->SetTitle("E[MeV]");
                //     C02_h[i]->GetYaxis()->SetTitle("counts");
                //     C02_h[i]->SetStats(0);
                //     C02_h[i]->Draw();
                //     c[i]->Update();
                //     latex.DrawLatex(0.73, 0.83, text1.Data()); // Position and text
                //     latex.DrawLatex(0.73, 0.80, text2.Data());
                //     latex.DrawLatex(0.73, 0.77, text3.Data());
                //     latex.DrawLatex(0.73, 0.74, text4.Data());
                //     c[i]->SaveAs(Form("C02/C02_h_%d_%s.png", i, get_run(fileName).c_str()));
                //     delete c[i];
                // }

                // // Close the current file
                // for (int i = 0; i < 16; i++) {
                //     delete canvas[i];
                // }


                energy_tot_h->Delete();
                S1_hist->Delete();
                S2_hist->Delete();
                Ch1_hist->Delete();
                Ch2_hist->Delete();
                Ch3_hist->Delete();
                Xcha1_hist->Delete();
                Ycha1_hist->Delete();
                Ch1_Ch2_h->Delete();
                Ch1_Ch3_h->Delete();
                Ch2_Ch3_h->Delete();
                S1_Etot_h->Delete();
                S2_Etot_h->Delete();
                Clong_hist->Delete();
                Ctot_hist->Delete();
                Clong_Ctot_h->Delete();


                rootFile->Close();
            

                std::cout << "Finished processing file: " << fileName << std::endl;
            

    }
        
    



    return 0;
}
