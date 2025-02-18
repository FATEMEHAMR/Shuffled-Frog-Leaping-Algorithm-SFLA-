#include <systemc.h>
#include <iostream>

SC_MODULE(MemeplexDivider) {
    sc_in<bool> clk;
    sc_in<bool> reset;
    sc_in<sc_uint<5>> sorted_indices[32];
    sc_out<sc_uint<5>> memeplex_indices[8][4];

    void divide() {
        if (reset.read()) return;

        for (int i = 0; i < 32; i++) {
            int m = i % 8;
            int k = i / 8;
            memeplex_indices[m][k].write(sorted_indices[i].read());
        }
    }

    SC_CTOR(MemeplexDivider) {
        SC_METHOD(divide);
        sensitive << clk.pos();
    }
};




//#include <systemc.h>
//#include <iostream>
//
//SC_MODULE(MemeplexDivider) {
//    // Constants
//    static const int MAX_FROGS = 32;
//    static const int MAX_MEMEPLEXES = 8;
//
//    // Ports
//    sc_in<bool> clk;
//    sc_in<bool> reset;
//    sc_in<sc_uint<5>> sorted_indices[MAX_FROGS];
//    sc_out<sc_uint<5>> memeplex_indices[MAX_MEMEPLEXES][MAX_FROGS / MAX_MEMEPLEXES];
//
//    // Processes
//    void divide_frogs() {
//        if (reset.read()) {
//            for (int m = 0; m < MAX_MEMEPLEXES; m++) {
//                for (int f = 0; f < MAX_FROGS / MAX_MEMEPLEXES; f++) {
//                    memeplex_indices[m][f].write(0);
//                }
//            }
//            std::cout << "[DEBUG] Reset applied. Memeplex indices cleared." << std::endl;
//            return;
//        }
//
//        // Distribute frogs using 8k + n pattern
//        for (int i = 0; i < MAX_FROGS; i++) {
//            int memeplex = i % MAX_MEMEPLEXES;
//            int position = i / MAX_MEMEPLEXES;
//
//            if (position < MAX_FROGS / MAX_MEMEPLEXES) {
//                memeplex_indices[memeplex][position].write(sorted_indices[i].read());
//            }
//        }
//
//        // Debugging: Print memeplex assignments
//        for (int m = 0; m < MAX_MEMEPLEXES; m++) {
//            std::cout << "[DEBUG] Memeplex " << m << ": ";
//            for (int f = 0; f < MAX_FROGS / MAX_MEMEPLEXES; f++) {
//                std::cout << memeplex_indices[m][f].read() << " ";
//            }
//            std::cout << std::endl;
//        }
//    }
//
//    // Constructor
//    SC_CTOR(MemeplexDivider) {
//        SC_METHOD(divide_frogs);
//        sensitive << clk.pos();
//        dont_initialize();
//    }
//};




//#include <systemc.h>
//#include <iostream>
//
//SC_MODULE(MemeplexDivider) {
//    // Constants
//    static const int MAX_FROGS = 32;
//    static const int MAX_MEMEPLEXES = 8;
//
//    // Ports
//    sc_in<bool> clk;
//    sc_in<bool> reset;
//    sc_in<sc_uint<5>> sorted_indices[MAX_FROGS];
//    sc_out<sc_uint<5>> memeplex_indices[MAX_MEMEPLEXES][MAX_FROGS / MAX_MEMEPLEXES];
//
//    // Processes
//    void divide_frogs() {
//        if (reset.read()) {
//            for (int m = 0; m < MAX_MEMEPLEXES; m++) {
//                for (int f = 0; f < MAX_FROGS / MAX_MEMEPLEXES; f++) {
//                    memeplex_indices[m][f].write(0);
//                }
//            }
//            std::cout << "[DEBUG] Reset applied. Memeplex indices cleared." << std::endl;
//            return;
//        }
//
//        // Distribute frogs using the 8k + n pattern
//        for (int i = 0; i < MAX_FROGS; i++) {
//            int memeplex = i % MAX_MEMEPLEXES;
//            int position = i / MAX_MEMEPLEXES;
//
//            if (position < MAX_FROGS / MAX_MEMEPLEXES) {
//                memeplex_indices[memeplex][position].write(sorted_indices[i].read());
//            }
//        }
//
//        // Debugging: Print memeplex assignments
//        for (int m = 0; m < MAX_MEMEPLEXES; m++) {
//            std::cout << "[DEBUG] Memeplex " << m << ": ";
//            for (int f = 0; f < MAX_FROGS / MAX_MEMEPLEXES; f++) {
//                std::cout << memeplex_indices[m][f].read() << " ";
//            }
//            std::cout << std::endl;
//        }
//        std::cout << "[DEBUG] Memeplex division completed using 8k + n rule." << std::endl;
//    }
//
//    // Constructor
//    SC_CTOR(MemeplexDivider) {
//        SC_METHOD(divide_frogs);
//        sensitive << clk.pos();
//        dont_initialize();
//    }
//};

//#include <systemc.h>
//#include <iostream>
//
//SC_MODULE(MemeplexDivider) {
//    // Constants
//    static const int MAX_FROGS = 32;
//    static const int MAX_MEMEPLEXES = 8;
//
//    // Ports
//    sc_in<bool> clk;                                  // Clock signal
//    sc_in<bool> reset;                                // Reset signal
//    sc_in<sc_uint<5>> sorted_indices[MAX_FROGS];      // Input sorted indices
//    sc_out<sc_uint<5>> memeplex_indices[MAX_MEMEPLEXES][MAX_FROGS / MAX_MEMEPLEXES]; // Output memeplex indices
//
//    // Processes
//    void divide_frogs() {
//        if (reset.read()) {
//            // Reset memeplex indices
//            for (int m = 0; m < MAX_MEMEPLEXES; m++) {
//                for (int f = 0; f < MAX_FROGS / MAX_MEMEPLEXES; f++) {
//                    memeplex_indices[m][f].write(0); // Reset to 0
//                }
//            }
//            std::cout << "[DEBUG] Reset applied. Memeplex indices cleared." << std::endl;
//            return; // Exit function after reset
//        }
//
//        // Divide frogs into memeplexes using the 8k + n pattern
//        for (int i = 0; i < MAX_FROGS; i++) {
//            int memeplex = i % MAX_MEMEPLEXES;  // Compute memeplex index (n)
//            int position = i / MAX_MEMEPLEXES;  // Compute position inside memeplex (k)
//
//            // Ensure we do not exceed the array bounds
//            if (position < MAX_FROGS / MAX_MEMEPLEXES) {
//                memeplex_indices[memeplex][position].write(sorted_indices[i].read());
//            }
//        }
//
//        // Debugging: Print memeplex assignments
//        for (int m = 0; m < MAX_MEMEPLEXES; m++) {
//            std::cout << "[DEBUG] Memeplex " << m << ": ";
//            for (int f = 0; f < MAX_FROGS / MAX_MEMEPLEXES; f++) {
//                std::cout << memeplex_indices[m][f].read() << " ";
//            }
//            std::cout << std::endl;
//        }
//
//        std::cout << "[DEBUG] Memeplex division completed using 8k + n rule." << std::endl;
//    }
//
//    // Constructor
//    SC_CTOR(MemeplexDivider) {
//        SC_METHOD(divide_frogs);
//        sensitive << clk.pos(); // Triggered on positive clock edge
//        dont_initialize();
//    }
//};
//
//
//
////#include <systemc.h>
////#include <iostream>
////
////SC_MODULE(MemeplexDivider) {
////    // Constants
////    static const int MAX_FROGS = 32;
////    static const int MAX_MEMEPLEXES = 8;
////
////    // Ports
////    sc_in<bool> clk;                                  // Clock signal
////    sc_in<bool> reset;                                // Reset signal
////    sc_in<sc_uint<5>> sorted_indices[MAX_FROGS];      // Input sorted indices
////    sc_out<sc_uint<5>> memeplex_indices[MAX_MEMEPLEXES][MAX_FROGS / MAX_MEMEPLEXES]; // Output memeplex indices
////
////    // Processes
////    void divide_frogs() {
////        if (reset.read()) {
////            // Reset memeplex indices
////            for (int m = 0; m < MAX_MEMEPLEXES; m++) {
////                for (int f = 0; f < MAX_FROGS / MAX_MEMEPLEXES; f++) {
////                    memeplex_indices[m][f].write(0); // Reset to 0
////                }
////            }
////            std::cout << "[DEBUG] Reset applied. Memeplex indices cleared." << std::endl;
////            return;  // EXIT FUNCTION IMMEDIATELY
////        }
////
////        //if (reset.read()) {
////        //    // Reset memeplex indices
////        //    for (int m = 0; m < MAX_MEMEPLEXES; m++) {
////        //        for (int f = 0; f < MAX_FROGS / MAX_MEMEPLEXES; f++) {
////        //            memeplex_indices[m][f].write(0); // Reset to 0
////        //        }
////        //    }
////        //    std::cout << "[DEBUG] Reset applied. Memeplex indices cleared." << std::endl;
////        //}
////        else {
////            // Divide frogs into memeplexes
////            bool sorted_changed = false;
////            for (int m = 0; m < MAX_MEMEPLEXES; m++) {
////                for (int f = 0; f < MAX_FROGS / MAX_MEMEPLEXES; f++) {
////                    sc_uint<5> new_val = sorted_indices[m * (MAX_FROGS / MAX_MEMEPLEXES) + f].read();
////                    if (memeplex_indices[m][f].read() != new_val) sorted_changed = true;
////                    memeplex_indices[m][f].write(new_val);
////                }
////            }
////
////            if (!sorted_changed) {
////                std::cerr << "[WARNING] Memeplex indices did not change! Sorting may not be updating correctly." << std::endl;
////            }
////
////            /*for (int m = 0; m < MAX_MEMEPLEXES; m++) {
////                for (int f = 0; f < MAX_FROGS / MAX_MEMEPLEXES; f++) {
////                    memeplex_indices[m][f].write(sorted_indices[m * (MAX_FROGS / MAX_MEMEPLEXES) + f].read());
////                }
////            }*/
////
////            // Debug: Print memeplex assignments
////            for (int m = 0; m < MAX_MEMEPLEXES; m++) {
////                for (int f = 0; f < MAX_FROGS / MAX_MEMEPLEXES; f++) {
////                    std::cout << "[DEBUG] Memeplex " << m << ", Frog " << f << ": Index "
////                        << memeplex_indices[m][f].read() << std::endl;
////                }
////            }
////
////            std::cout << "[DEBUG] Memeplex division completed." << std::endl;
////        }
////    }
////
////    // Constructor
////    SC_CTOR(MemeplexDivider) {
////        SC_METHOD(divide_frogs);
////        sensitive << clk.pos(); // Triggered on positive clock edge
////        dont_initialize();
////    }
////};
