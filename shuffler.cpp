





#include <systemc.h>
#include <iostream>
#include <vector>
#include <random>

SC_MODULE(Shuffler) {
    static const int MAX_MEMEPLEXES = 8;
    static const int MAX_FROGS_PER_MEMEPLEX = 4;

    sc_in<bool> clk;
    sc_in<bool> reset;
    sc_in<sc_uint<5>> memeplex_indices[MAX_MEMEPLEXES][MAX_FROGS_PER_MEMEPLEX];
    sc_out<sc_uint<5>> shuffled_indices[MAX_MEMEPLEXES * MAX_FROGS_PER_MEMEPLEX];

    void shuffle_population() {
        if (reset.read()) {
            for (int i = 0; i < MAX_MEMEPLEXES * MAX_FROGS_PER_MEMEPLEX; i++) {
                shuffled_indices[i].write(0);
            }
            std::cout << "[DEBUG] Reset Applied. Memeplex Indices Cleared." << std::endl;
            return;
        }

        // Combine memeplex indices into a single vector
        std::vector<sc_uint<5>> combined_population;
        for (int m = 0; m < MAX_MEMEPLEXES; m++) {
            for (int f = 0; f < MAX_FROGS_PER_MEMEPLEX; f++) {
                combined_population.push_back(memeplex_indices[m][f].read());
            }
        }

        // Shuffle memeplexes randomly
        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(combined_population.begin(), combined_population.end(), gen);

        // Write shuffled indices to output
        for (int i = 0; i < combined_population.size(); i++) {
            shuffled_indices[i].write(combined_population[i]);
        }

        std::cout << "[DEBUG] Memeplex Shuffling Completed." << std::endl;
    }

    SC_CTOR(Shuffler) {
        SC_METHOD(shuffle_population);
        sensitive << clk.pos();
        dont_initialize();
    }
};


//
//
//
//
////#include <systemc.h>
////#include <iostream>
////#include <algorithm>
////#include <vector>
////#include <random>
////
////SC_MODULE(Shuffler) {
////    // Constants
////    static const int MAX_MEMEPLEXES = 8;
////    static const int MAX_FROGS_PER_MEMEPLEX = 4;
////
////    // Ports
////    sc_in<bool> clk;                                    // Clock signal
////    sc_in<bool> reset;                                  // Reset signal
////    sc_in<sc_uint<5>> memeplex_indices[MAX_MEMEPLEXES][MAX_FROGS_PER_MEMEPLEX]; // Memeplex indices
////    sc_out<sc_uint<5>> shuffled_indices[MAX_MEMEPLEXES * MAX_FROGS_PER_MEMEPLEX]; // Shuffled population
////
////    // Processes
////    void shuffle_population() {
////        if (reset.read()) {
////            // Reset all outputs
////            for (int i = 0; i < MAX_MEMEPLEXES * MAX_FROGS_PER_MEMEPLEX; i++) {
////                shuffled_indices[i].write(0);
////            }
////            std::cout << "[DEBUG] Reset applied. Shuffled indices cleared." << std::endl;
////        }
////        else {
////            // Combine memeplex indices into a single vector
////            std::vector<sc_uint<5>> combined_population;
////            for (int m = 0; m < MAX_MEMEPLEXES; m++) {
////                for (int f = 0; f < MAX_FROGS_PER_MEMEPLEX; f++) {
////                    combined_population.push_back(memeplex_indices[m][f].read());
////                }
////            }
////
////            // Shuffle the combined population using std::shuffle
////            std::random_device rd;                            // Random number generator
////            std::mt19937 gen(rd());                          // Mersenne Twister engine
////            std::shuffle(combined_population.begin(), combined_population.end(), gen);
////
////            // Write shuffled indices to output
////            for (int i = 0; i < combined_population.size(); i++) {
////                shuffled_indices[i].write(combined_population[i]);
////            }
////
////            // Debug: Print shuffled indices
////            std::cout << "[DEBUG] Shuffled Indices: ";
////            for (int i = 0; i < MAX_MEMEPLEXES * MAX_FROGS_PER_MEMEPLEX; i++) {
////                std::cout << shuffled_indices[i].read() << " ";
////            }
////            std::cout << std::endl;
////
////            std::cout << "[DEBUG] Shuffling completed." << std::endl;
////        }
////    }
////
////    // Constructor
////    SC_CTOR(Shuffler) {
////        SC_METHOD(shuffle_population);
////        sensitive << clk.pos(); // Triggered on positive clock edge
////        dont_initialize();
////    }
////};
