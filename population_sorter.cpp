




#include <systemc.h>
#include <iostream>
#include <algorithm>

SC_MODULE(PopulationSorter) {
    sc_in<bool> clk;
    sc_in<bool> reset;
    sc_in<sc_int<32>> fitness_values[32];
    sc_out<sc_uint<5>> sorted_indices[32];

    void sort_frogs() {
        if (reset.read()) return;

        int fitness_copy[32], index_copy[32];
        for (int i = 0; i < 32; i++) {
            fitness_copy[i] = fitness_values[i].read();
            index_copy[i] = i;
        }

        std::sort(index_copy, index_copy + 32, [&](int a, int b) {
            return fitness_copy[a] > fitness_copy[b];
            });

        for (int i = 0; i < 32; i++) {
            sorted_indices[i].write(index_copy[i]);
        }
    }

    SC_CTOR(PopulationSorter) {
        SC_METHOD(sort_frogs);
        sensitive << clk.pos();
    }
};



//#include <systemc.h>
//#include <iostream>
//#include <algorithm>
//
//SC_MODULE(PopulationSorter) {
//    // Ports
//    sc_in<bool> clk;                     // Clock signal
//    sc_in<bool> reset;                   // Reset signal
//    sc_in<sc_int<32>> fitness_values[32]; // Input fitness values
//    sc_out<sc_uint<5>> sorted_indices[32]; // Sorted indices (max 32 frogs)
//
//    // Internal signals
//    sc_signal<sc_uint<5>> temp_indices[32]; // Temporary indices for sorting
//    sc_signal<sc_int<32>> temp_fitness[32]; // Temporary fitness values for sorting
//
//    // Processes
//    void sort_frogs() {
//        if (reset.read()) {
//            // Reset sorted indices to an initial state (0, 1, 2, ..., 31)
//            for (int i = 0; i < 32; i++) {
//                sorted_indices[i].write(i);
//                temp_indices[i].write(i);
//                temp_fitness[i].write(0);
//            }
//            std::cout << "[DEBUG] PopulationSorter Reset." << std::endl;
//            return;  // EXIT FUNCTION IMMEDIATELY
//        }
//
//        // Copy fitness values and indices to temporary signals
//        int fitness_copy[32];
//        int index_copy[32];
//
//        for (int i = 0; i < 32; i++) {
//            fitness_copy[i] = fitness_values[i].read();
//            index_copy[i] = i;
//        }
//
//        // Sort in descending order (Bubble Sort for simplicity)
//        for (int i = 0; i < 32 - 1; i++) {
//            bool swapped = false;
//            for (int j = 0; j < 32 - i - 1; j++) {
//                if (fitness_copy[j] < fitness_copy[j + 1]) { // Descending order
//                    swapped = true;
//
//                    // Swap fitness values
//                    std::swap(fitness_copy[j], fitness_copy[j + 1]);
//
//                    // Swap corresponding indices
//                    std::swap(index_copy[j], index_copy[j + 1]);
//                }
//            }
//            if (!swapped) break; // Stop early if sorted
//        }
//
//        // Write sorted indices to the output
//        for (int i = 0; i < 32; i++) {
//            sorted_indices[i].write(index_copy[i]);
//        }
//
//        // Debug: Print sorted fitness values
//        std::cout << "[DEBUG] Sorted Fitness Values: ";
//        for (int i = 0; i < 32; i++) {
//            std::cout << "[" << sorted_indices[i].read() << ":" << fitness_copy[i] << "] ";
//        }
//        std::cout << std::endl;
//    }
//
//    // Constructor
//    SC_CTOR(PopulationSorter) {
//        SC_METHOD(sort_frogs);
//        sensitive << clk.pos(); // Triggered on positive clock edge
//        dont_initialize();
//    }
//};






//#include <systemc.h>
//#include <iostream>
//#include <algorithm>
//
//SC_MODULE(PopulationSorter) {
//    // Ports
//    sc_in<bool> clk;                     // Clock signal
//    sc_in<bool> reset;                   // Reset signal
//    sc_in<sc_int<32>> fitness_values[32]; // Input fitness values
//    sc_out<sc_uint<5>> sorted_indices[32]; // Sorted indices (max 32 frogs)
//
//    // Internal signals
//    sc_signal<sc_uint<5>> temp_indices[32]; // Temporary indices for sorting
//    sc_signal<sc_int<32>> temp_fitness[32]; // Temporary fitness values for sorting
//
//    // Processes
//    void sort_frogs() {
//        if (reset.read()) {
//            // Reset sorted indices to an initial state (e.g., 0, 1, 2, ..., 31)
//            for (int i = 0; i < 32; i++) {
//                sorted_indices[i].write(i);
//                temp_indices[i].write(i);
//                temp_fitness[i].write(0);
//            }
//            std::cout << "[DEBUG] PopulationSorter reset." << std::endl;
//            return;  // EXIT FUNCTION IMMEDIATELY
//        }
//
//        // Copy fitness values and indices to temporary signals
//        for (int i = 0; i < 32; i++) {
//            temp_fitness[i].write(fitness_values[i].read());
//            temp_indices[i].write(i);
//        }
//
//        // Bubble sort based on fitness values (descending order)
//        for (int i = 0; i < 32 - 1; i++) {
//            bool swapped = false;  // Check if swaps happened
//            for (int j = 0; j < 32 - i - 1; j++) {
//                if (temp_fitness[j].read() < temp_fitness[j + 1].read()) {
//                    swapped = true;
//
//                    // Swap fitness values
//                    sc_int<32> temp = temp_fitness[j].read();
//                    temp_fitness[j].write(temp_fitness[j + 1].read());
//                    temp_fitness[j + 1].write(temp);
//
//                    // Swap corresponding indices
//                    sc_uint<5> temp_idx = temp_indices[j].read();
//                    temp_indices[j].write(temp_indices[j + 1].read());
//                    temp_indices[j + 1].write(temp_idx);
//                }
//            }
//            if (!swapped) break; // Exit early if no swaps
//        }
//
//        // Write sorted indices to the output
//        bool changed = false;
//        for (int i = 0; i < 32; i++) {
//            if (sorted_indices[i].read() != temp_indices[i].read()) changed = true;
//            sorted_indices[i].write(temp_indices[i].read());
//        }
//
//        if (!changed) {
//            std::cerr << "[WARNING] PopulationSorter: No change in sorted indices! Check if fitness values are updating correctly." << std::endl;
//        }
//
//        // Debug: Print sorted indices
//        for (int i = 0; i < 32; i++) {
//            std::cout << "[DEBUG] Sorted Index " << i << ": " << sorted_indices[i].read() << std::endl;
//        }
//    }
//
//    // Constructor
//    SC_CTOR(PopulationSorter) {
//        SC_METHOD(sort_frogs);
//        sensitive << clk.pos(); // Triggered on positive clock edge
//        dont_initialize();
//    }
//};
//
//
////#include "population_sorter.h"
////#include <algorithm>
////
////void PopulationSorter::sort_frogs() {
////    if (reset.read()) {
////        // Reset sorted indices to an initial state (e.g., 0, 1, 2, ..., 31)
////        for (int i = 0; i < 32; i++) {
////            sorted_indices[i].write(i);
////            temp_indices[i].write(i);
////            temp_fitness[i].write(0);
////        }
////        std::cout << "[DEBUG] PopulationSorter reset." << std::endl;
////    }
////    else {
////        // Copy fitness values and indices to temporary signals
////        for (int i = 0; i < 32; i++) {
////            temp_fitness[i].write(fitness_values[i].read());
////            temp_indices[i].write(i);
////        }
////
////        // Bubble sort based on fitness values (descending order)
////        for (int i = 0; i < 32 - 1; i++) {
////            for (int j = 0; j < 32 - i - 1; j++) {
////                if (temp_fitness[j].read() < temp_fitness[j + 1].read()) {
////                    // Swap fitness values
////                    sc_int<32> temp = temp_fitness[j].read();
////                    temp_fitness[j].write(temp_fitness[j + 1].read());
////                    temp_fitness[j + 1].write(temp);
////
////                    // Swap corresponding indices
////                    sc_uint<5> temp_idx = temp_indices[j].read();
////                    temp_indices[j].write(temp_indices[j + 1].read());
////                    temp_indices[j + 1].write(temp_idx);
////                }
////            }
////        }
////
////        // Write sorted indices to the output
////        for (int i = 0; i < 32; i++) {
////            sorted_indices[i].write(temp_indices[i].read());
////            // Debug: Print sorted indices
////            std::cout << "[DEBUG] Sorted Index " << i << ": " << sorted_indices[i].read() << std::endl;
////        }
////    }
////}