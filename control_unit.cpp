#include <systemc.h>
#include <iostream>

SC_MODULE(ControlUnit) {
    static const int MAX_ITERATIONS = 100;
    static const int STAGNATION_LIMIT = 10;

    sc_in<bool> clk;
    sc_in<bool> reset;
    sc_out<sc_uint<8>> iteration_count;
    sc_out<bool> termination_flag;

    sc_uint<8> current_iteration;
    int stagnation_count = 0;
    int prev_best_fitness = -1;
    sc_signal<sc_int<32>> best_fitness;

    void monitor() {
        if (reset.read()) {
            current_iteration = 0;
            iteration_count.write(0);
            termination_flag.write(false);
            stagnation_count = 0;
            std::cout << "[DEBUG] Reset Applied." << std::endl;
            return;
        }

        if (!termination_flag.read()) {
            current_iteration++;
            iteration_count.write(current_iteration);

            if (current_iteration >= MAX_ITERATIONS || stagnation_count >= STAGNATION_LIMIT) {
                termination_flag.write(true);
                std::cout << "[DEBUG] Termination Condition Met." << std::endl;
            }
            else {
                std::cout << "[DEBUG] Iteration: " << current_iteration << std::endl;
            }
        }
    }

    SC_CTOR(ControlUnit) {
        SC_METHOD(monitor);
        sensitive << clk.pos();
        dont_initialize();
    }
};



//#include <systemc.h>
//#include <iostream>
//
//SC_MODULE(ControlUnit) {
//    static const int MAX_ITERATIONS = 100;
//    static const int STAGNATION_LIMIT = 10; // Termination after 10 unchanged solutions
//
//    sc_in<bool> clk;
//    sc_in<bool> reset;
//    sc_in<sc_uint<8>> max_iterations;
//    sc_in<bool> termination_flag;
//    sc_out<sc_uint<8>> iteration_count;
//    sc_out<bool> done;
//
//    sc_uint<8> current_iteration;
//    int stagnation_count = 0;
//    sc_uint<8> last_best_iteration;
//
//    void control_flow() {
//        if (reset.read()) {
//            current_iteration = 0;
//            iteration_count.write(0);
//            done.write(false);
//            stagnation_count = 0;
//            last_best_iteration = 0;
//            std::cout << "[DEBUG] Reset applied." << std::endl;
//            return;
//        }
//
//        if (!done.read()) {
//            if (termination_flag.read() || stagnation_count >= STAGNATION_LIMIT) {
//                done.write(true);
//                std::cout << "[DEBUG] Termination condition met." << std::endl;
//            }
//            else {
//                current_iteration++;
//                iteration_count.write(current_iteration);
//                stagnation_count++;  // Increase if no improvement
//                std::cout << "[DEBUG] Iteration: " << current_iteration << std::endl;
//            }
//        }
//    }
//
//    SC_CTOR(ControlUnit) {
//        SC_METHOD(control_flow);
//        sensitive << clk.pos();
//        dont_initialize();
//    }
//};



//#include <systemc.h>
//#include <iostream>
//
//SC_MODULE(ControlUnit) {
//    // Constants
//    static const int MAX_ITERATIONS = 100;
//
//    // Ports
//    sc_in<bool> clk;                      // Clock signal
//    sc_in<bool> reset;                    // Reset signal
//    sc_in<sc_uint<8>> max_iterations;     // Maximum number of iterations
//    sc_in<bool> termination_flag;         // Optimization termination condition
//    sc_out<sc_uint<8>> iteration_count;   // Current iteration count
//    sc_out<bool> done;                    // Algorithm completion flag
//
//    // Internal variables
//    sc_uint<8> current_iteration;         // Tracks the current iteration
//
//    // Processes
//    void control_flow() {
//        if (reset.read()) {
//            current_iteration = 0;
//            iteration_count.write(0);
//            done.write(false);
//            std::cout << "[DEBUG] ControlUnit RESET: Iteration reset to 0." << std::endl;
//            return;  // EXIT to prevent further execution during reset
//        }
//
//        //if (reset.read()) {
//        //    // Reset all internal states and outputs
//        //    current_iteration = 0;
//        //    iteration_count.write(0);
//        //    done.write(false);
//        //    std::cout << "[DEBUG] ControlUnit reset." << std::endl;
//        //}
//
//        else {
//            if (!done.read() && !reset.read()) { // Ensure reset is not active
//                // Check termination conditions
//                std::cout << "[DEBUG] Checking termination: Iteration = " << current_iteration
//                    << ", Max Iterations = " << max_iterations.read()
//                    << ", Termination Flag = " << termination_flag.read() << std::endl;
//
//                if (max_iterations.read() == 0) {
//                    std::cerr << "[ERROR] max_iterations is 0! This may cause an infinite loop." << std::endl;
//                }
//
//                if (termination_flag.read() || current_iteration >= max_iterations.read()) {
//                    done.write(true);
//                    std::cout << "[DEBUG] ControlUnit marked done." << std::endl;
//                }
//
//                /*if (termination_flag.read() || current_iteration >= max_iterations.read()) {
//                    done.write(true);
//                    std::cout << "[DEBUG] ControlUnit finished at iteration: " << current_iteration << std::endl;
//                }*/
//
//                else {
//                    // Increment iteration count
//                    current_iteration++;
//                    iteration_count.write(current_iteration);
//                    std::cout << "[DEBUG] ControlUnit iteration: " << current_iteration << std::endl;
//                }
//            }
//        }
//    }
//
//    // Constructor
//    SC_CTOR(ControlUnit) {
//        SC_METHOD(control_flow);
//        sensitive << clk.pos();
//        dont_initialize();
//    }
//};

