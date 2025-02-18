#include <systemc.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

SC_MODULE(PositionUpdater) {
    static const int MAX_DIMENSION = 32;
    static const int L_m = 10; // Number of local search iterations

    sc_in<bool> clk;  // 🔹 **ADDED: Clock signal**
    sc_in<bool> reset;
    sc_in<sc_int<32>> frog_position[MAX_DIMENSION];
    sc_in<sc_int<32>> best_position[MAX_DIMENSION];
    sc_in<sc_int<32>> global_best_position[MAX_DIMENSION];
    sc_in<sc_int<32>> step_size;
    sc_out<sc_int<32>> new_position[MAX_DIMENSION];

    sc_signal<int> local_iteration;

    std::default_random_engine generator;

    void update_position() {
        if (reset.read()) {
            for (int i = 0; i < MAX_DIMENSION; i++) {
                new_position[i].write(0);
            }
            local_iteration.write(0);
            std::cout << "[DEBUG] Reset applied. Positions cleared." << std::endl;
            return;
        }

        if (local_iteration.read() < L_m) {
            bool improvement = false;

            // Compute weighted probabilities for selecting solutions
            std::vector<double> probabilities(MAX_DIMENSION);
            double total_prob = 0.0;
            for (int j = 0; j < MAX_DIMENSION; j++) {
                probabilities[j] = 2.0 * (MAX_DIMENSION + 1 - (j + 1)) / (MAX_DIMENSION * (MAX_DIMENSION + 1));
                total_prob += probabilities[j];
            }

            // Normalize probabilities
            for (int j = 0; j < MAX_DIMENSION; j++) {
                probabilities[j] /= total_prob;
            }

            // Select a solution based on probability distribution
            std::discrete_distribution<int> distribution(probabilities.begin(), probabilities.end());
            int worst_frog_idx = distribution(generator);

            // Identify best and worst solutions
            sc_int<32> worst_frog = frog_position[worst_frog_idx].read();
            sc_int<32> best_frog = best_position[worst_frog_idx].read();

            // Compute step size
            double r = (rand() % 100) / 100.0;
            sc_int<32> step = std::min(r * (best_frog - worst_frog), (double)step_size.read());

            // Update position
            int new_pos = (frog_position[worst_frog_idx].read() + step > 0) ? 1 : 0;
            new_position[worst_frog_idx].write(new_pos);

            // Check improvement
            if (new_pos != worst_frog) {
                improvement = true;
            }

            // If no improvement, update using global best
            if (!improvement) {
                sc_int<32> global_best = global_best_position[worst_frog_idx].read();
                step = std::min(r * (global_best - worst_frog), (double)step_size.read());
                new_pos = (frog_position[worst_frog_idx].read() + step > 0) ? 1 : 0;
                new_position[worst_frog_idx].write(new_pos);
                std::cout << "[DEBUG] No improvement, moving toward global best." << std::endl;
            }

            // If still no improvement, generate a random solution
            if (!improvement) {
                for (int i = 0; i < MAX_DIMENSION; i++) {
                    int rand_pos = rand() % 2;
                    new_position[i].write(rand_pos);
                }
                std::cout << "[DEBUG] No improvement, generating random solution." << std::endl;
            }

            // Increment local iteration count
            local_iteration.write(local_iteration.read() + 1);
        }
    }

    SC_CTOR(PositionUpdater) {
        SC_METHOD(update_position);
        sensitive << clk.pos(); // ✅ **Fixed: Now 'clk' is properly declared**
        dont_initialize();
    }
};




//int sc_main(int argc, char* argv[]) {
//    PositionUpdater updater("PositionUpdater");
//    sc_start();
//    return 0;
//}




//#include <systemc.h>
//#include <iostream>
//#include <vector>
//#include <algorithm>
//#include <random>
//
//SC_MODULE(PositionUpdater) {
//    static const int MAX_DIMENSION = 32;
//    static const int L_m = 10; // Number of local search iterations
//    static const int S_max = 2; // Maximum step size
//
//    sc_in<bool> clk;
//    sc_in<bool> reset;
//    sc_in<sc_int<32>> frog_position[MAX_DIMENSION];      // Current frog position
//    sc_in<sc_int<32>> best_position[MAX_DIMENSION];      // Best in memeplex
//    sc_in<sc_int<32>> global_best_position[MAX_DIMENSION]; // Global best solution
//    sc_in<sc_int<32>> step_size;
//    sc_out<sc_int<32>> new_position[MAX_DIMENSION];
//
//    sc_signal<int> local_iteration;
//    std::default_random_engine generator;
//
//    void update_position() {
//        if (reset.read()) {
//            for (int i = 0; i < MAX_DIMENSION; i++) {
//                new_position[i].write(0);
//            }
//            local_iteration.write(0);
//            std::cout << "[DEBUG] Reset applied. Positions cleared." << std::endl;
//            return;
//        }
//
//        if (local_iteration.read() < L_m) {
//            bool improvement = false;
//            std::random_device rd;
//            std::mt19937 gen(rd());
//            std::uniform_real_distribution<double> dist(0.0, 1.0);
//            double r = dist(gen);  // Random number between 0 and 1
//
//            for (int i = 0; i < MAX_DIMENSION; i++) {
//                sc_int<32> worst_frog = frog_position[i].read();
//                sc_int<32> global_best = global_best_position[i].read();
//
//                // **Step size calculation toward global best**
//                sc_int<32> step = 0;
//                if (global_best > worst_frog) {
//                    step = std::min((sc_int<32>)(r * (global_best - worst_frog)), (sc_int<32>)S_max);
//                }
//                else {
//                    step = std::max((sc_int<32>)(r * (worst_frog - global_best)), (sc_int<32>) - S_max);
//                }
//
//                // **Update position ensuring binary values (Knapsack 0/1 constraint)**
//                sc_int<32> new_pos = worst_frog + step;
//                new_pos = (new_pos > 0) ? 1 : 0;
//                new_position[i].write(new_pos);
//
//                // Check if the position improved
//                if (new_pos != worst_frog) {
//                    improvement = true;
//                }
//
//                std::cout << "[DEBUG] Position Update - Dim: " << i
//                    << ", Old: " << worst_frog
//                    << ", Global Best: " << global_best
//                    << ", Step: " << step
//                    << ", New: " << new_pos
//                    << std::endl;
//            }
//
//            // **If still no improvement, generate a completely random solution**
//            if (!improvement) {
//                std::cout << "[DEBUG] No improvement, generating random solution." << std::endl;
//                for (int i = 0; i < MAX_DIMENSION; i++) {
//                    int rand_pos = rand() % 2;
//                    new_position[i].write(rand_pos);
//                }
//            }
//
//            // **Increment local iteration count**
//            local_iteration.write(local_iteration.read() + 1);
//        }
//    }
//
//    SC_CTOR(PositionUpdater) {
//        SC_METHOD(update_position);
//        sensitive << clk.pos();
//        dont_initialize();
//    }
//};





//#include <systemc.h>
//#include <iostream>
//#include <vector>
//#include <algorithm>
//#include <random>
//
//SC_MODULE(PositionUpdater) {
//    static const int MAX_DIMENSION = 32;
//    static const int L_m = 10; // Number of local search iterations
//    static const int S_max = 2; // Maximum step size
//
//    sc_in<bool> clk;
//    sc_in<bool> reset;
//    sc_in<sc_int<32>> frog_position[MAX_DIMENSION];      // Current frog position
//    sc_in<sc_int<32>> best_position[MAX_DIMENSION];      // Best in memeplex
//    sc_in<sc_int<32>> global_best_position[MAX_DIMENSION]; // Global best solution
//    sc_in<sc_int<32>> step_size;
//    sc_out<sc_int<32>> new_position[MAX_DIMENSION];
//
//    sc_signal<int> local_iteration;
//    std::default_random_engine generator;
//
//    void update_position() {
//        if (reset.read()) {
//            for (int i = 0; i < MAX_DIMENSION; i++) {
//                new_position[i].write(0);
//            }
//            local_iteration.write(0);
//            std::cout << "[DEBUG] Reset applied. Positions cleared." << std::endl;
//            return;
//        }
//
//        if (local_iteration.read() < L_m) {
//            bool improvement = false;
//            std::random_device rd;
//            std::mt19937 gen(rd());
//            std::uniform_real_distribution<double> dist(0.0, 1.0);
//            double r = dist(gen);  // Random number between 0 and 1
//
//            for (int i = 0; i < MAX_DIMENSION; i++) {
//                sc_int<32> worst_frog = frog_position[i].read();
//                sc_int<32> global_best = global_best_position[i].read();
//
//                // **Step size calculation toward global best**
//                sc_int<32> step = 0;
//                if (global_best > worst_frog) {
//                    step = std::min((sc_int<32>)(r * (global_best - worst_frog)), (sc_int<32>)S_max);
//                }
//                else {
//                    step = std::max((sc_int<32>)(r * (worst_frog - global_best)), (sc_int<32>) - S_max);
//                }
//
//                // **Update position ensuring binary values (Knapsack 0/1 constraint)**
//                sc_int<32> new_pos = worst_frog + step;
//                new_pos = (new_pos > 0) ? 1 : 0;
//                new_position[i].write(new_pos);
//
//                // Check if the position improved
//                if (new_pos != worst_frog) {
//                    improvement = true;
//                }
//
//                std::cout << "[DEBUG] Position Update - Dim: " << i
//                    << ", Old: " << worst_frog
//                    << ", Global Best: " << global_best
//                    << ", Step: " << step
//                    << ", New: " << new_pos
//                    << std::endl;
//            }
//
//            // **If still no improvement, generate a completely random solution**
//            if (!improvement) {
//                std::cout << "[DEBUG] No improvement, generating random solution." << std::endl;
//                for (int i = 0; i < MAX_DIMENSION; i++) {
//                    int rand_pos = rand() % 2;
//                    new_position[i].write(rand_pos);
//                }
//            }
//
//            // **Increment local iteration count**
//            local_iteration.write(local_iteration.read() + 1);
//        }
//    }
//
//    SC_CTOR(PositionUpdater) {
//        SC_METHOD(update_position);
//        sensitive << clk.pos();
//        dont_initialize();
//    }
//};




//#include <systemc.h>
//#include <iostream>
//#include <vector>
//#include <algorithm>
//#include <random>
//
//SC_MODULE(PositionUpdater) {
//    static const int MAX_DIMENSION = 32;
//    static const int L_m = 10; // Number of local search iterations
//    static const int S_max = 2; // Maximum step size
//
//    sc_in<bool> clk;
//    sc_in<bool> reset;
//    sc_in<sc_int<32>> frog_position[MAX_DIMENSION];      // Current frog position
//    sc_in<sc_int<32>> best_position[MAX_DIMENSION];      // Best in memeplex
//    sc_in<sc_int<32>> global_best_position[MAX_DIMENSION]; // Global best solution
//    sc_in<sc_int<32>> step_size;
//    sc_out<sc_int<32>> new_position[MAX_DIMENSION];
//
//    sc_signal<int> local_iteration;
//
//    std::default_random_engine generator;
//
//    void update_position() {
//        if (reset.read()) {
//            for (int i = 0; i < MAX_DIMENSION; i++) {
//                new_position[i].write(0);
//            }
//            local_iteration.write(0);
//            std::cout << "[DEBUG] Reset applied. Positions cleared." << std::endl;
//            return;
//        }
//
//        if (local_iteration.read() < L_m) {
//            bool improvement = false;
//            std::random_device rd;
//            std::mt19937 gen(rd());
//            std::uniform_real_distribution<double> dist(0.0, 1.0);
//            double r = dist(gen);  // Random number between 0 and 1
//
//            for (int i = 0; i < MAX_DIMENSION; i++) {
//                sc_int<32> worst_frog = frog_position[i].read();
//                sc_int<32> best_frog = best_position[i].read();
//                sc_int<32> global_best = global_best_position[i].read();
//
//                sc_int<32> step = 0;
//
//                // **Step size calculation using proper min/max**
//                if (best_frog > worst_frog) {
//                    step = std::min((sc_int<32>)(r * (best_frog - worst_frog)), (sc_int<32>)S_max);
//                }
//                else {
//                    step = std::max((sc_int<32>)(r * (worst_frog - best_frog)), (sc_int<32>) - S_max);
//                }
//
//                // **Update position ensuring binary values (Knapsack 0/1 constraint)**
//                sc_int<32> new_pos = worst_frog + step;
//                new_pos = (new_pos > 0) ? 1 : 0;
//                new_position[i].write(new_pos);
//
//                // Check if the position improved
//                if (new_pos != worst_frog) {
//                    improvement = true;
//                }
//
//                std::cout << "[DEBUG] Position Update - Dim: " << i
//                    << ", Old: " << worst_frog
//                    << ", Best: " << best_frog
//                    << ", Step: " << step
//                    << ", New: " << new_pos
//                    << std::endl;
//            }
//
//            // **If no improvement, move toward global best**
//            if (!improvement) {
//                for (int i = 0; i < MAX_DIMENSION; i++) {
//                    sc_int<32> worst_frog = frog_position[i].read();
//                    sc_int<32> global_best = global_best_position[i].read();
//
//                    sc_int<32> step = 0;
//                    if (global_best > worst_frog) {
//                        step = std::min((sc_int<32>)(r * (global_best - worst_frog)), (sc_int<32>)S_max);
//                    }
//                    else {
//                        step = std::max((sc_int<32>)(r * (worst_frog - global_best)), (sc_int<32>) - S_max);
//                    }
//
//                    sc_int<32> new_pos = worst_frog + step;
//                    new_pos = (new_pos > 0) ? 1 : 0;
//                    new_position[i].write(new_pos);
//                }
//                std::cout << "[DEBUG] No improvement, moving toward global best." << std::endl;
//            }
//
//            // **If still no improvement, generate a completely random solution**
//            if (!improvement) {
//                std::cout << "[DEBUG] No improvement, generating random solution." << std::endl;
//                for (int i = 0; i < MAX_DIMENSION; i++) {
//                    int rand_pos = rand() % 2;
//                    new_position[i].write(rand_pos);
//                }
//            }
//
//            // **Increment local iteration count**
//            local_iteration.write(local_iteration.read() + 1);
//        }
//    }
//
//    SC_CTOR(PositionUpdater) {
//        SC_METHOD(update_position);
//        sensitive << clk.pos();
//        dont_initialize();
//    }
//};





//#include <systemc.h>
//#include <iostream>
//#include <vector>
//#include <algorithm>
//#include <random>
//
//SC_MODULE(PositionUpdater) {
//    static const int MAX_DIMENSION = 32;
//    static const int L_m = 10; // Number of local search iterations
//
//    sc_in<bool> clk;
//    sc_in<bool> reset;
//    sc_in<sc_int<32>> frog_position[MAX_DIMENSION];
//    sc_in<sc_int<32>> best_position[MAX_DIMENSION];
//    sc_in<sc_int<32>> global_best_position[MAX_DIMENSION];
//    sc_in<sc_int<32>> step_size;
//    sc_out<sc_int<32>> new_position[MAX_DIMENSION];
//
//    sc_signal<int> local_iteration;
//
//    std::default_random_engine generator;
//
//    void update_position() {
//        if (reset.read()) {
//            for (int i = 0; i < MAX_DIMENSION; i++) {
//                new_position[i].write(0);
//            }
//            local_iteration.write(0);
//            std::cout << "[DEBUG] Reset applied. Positions cleared." << std::endl;
//            return;
//        }
//
//        if (local_iteration.read() < L_m) {
//            bool improvement = false;
//
//            // Compute weighted probabilities for selecting solutions
//            std::vector<double> probabilities(MAX_DIMENSION);
//            double total_prob = 0.0;
//            for (int j = 0; j < MAX_DIMENSION; j++) {
//                probabilities[j] = 2.0 * (MAX_DIMENSION + 1 - (j + 1)) / (MAX_DIMENSION * (MAX_DIMENSION + 1));
//                total_prob += probabilities[j];
//            }
//
//            // Normalize probabilities
//            for (int j = 0; j < MAX_DIMENSION; j++) {
//                probabilities[j] /= total_prob;
//            }
//
//            // Select a solution based on probability distribution
//            std::discrete_distribution<int> distribution(probabilities.begin(), probabilities.end());
//            int worst_frog_idx = distribution(generator);
//
//            // Identify best and worst solutions
//            sc_int<32> worst_frog = frog_position[worst_frog_idx].read();
//            sc_int<32> best_frog = best_position[worst_frog_idx].read();
//
//            // Compute step size
//            double r = (rand() % 100) / 100.0;
//            sc_int<32> step = std::min(r * (best_frog - worst_frog), (double)step_size.read());
//
//            // Update position
//            int new_pos = (frog_position[worst_frog_idx].read() + step > 0) ? 1 : 0;
//            new_position[worst_frog_idx].write(new_pos);
//
//            // Check improvement
//            if (new_pos != worst_frog) {
//                improvement = true;
//            }
//
//            // If no improvement, update using global best
//            if (!improvement) {
//                sc_int<32> global_best = global_best_position[worst_frog_idx].read();
//                step = std::min(r * (global_best - worst_frog), (double)step_size.read());
//                new_pos = (frog_position[worst_frog_idx].read() + step > 0) ? 1 : 0;
//                new_position[worst_frog_idx].write(new_pos);
//                std::cout << "[DEBUG] No improvement, moving toward global best." << std::endl;
//            }
//
//            // If still no improvement, generate a random solution
//            if (!improvement) {
//                for (int i = 0; i < MAX_DIMENSION; i++) {
//                    int rand_pos = rand() % 2;
//                    new_position[i].write(rand_pos);
//                }
//                std::cout << "[DEBUG] No improvement, generating random solution." << std::endl;
//            }
//
//            // Increment local iteration count
//            local_iteration.write(local_iteration.read() + 1);
//        }
//    }
//
//    SC_CTOR(PositionUpdater) {
//        SC_METHOD(update_position);
//        sensitive << clk.pos();
//        dont_initialize();
//    }
//};



//#include <systemc.h>
//#include <iostream>
//#include <cmath>
//
//SC_MODULE(PositionUpdater) {
//    // Constants
//    static const int MAX_DIMENSION = 32;
//    static const int L_m = 10; // Number of local search iterations
//
//    // Ports
//    sc_in<bool> clk;
//    sc_in<bool> reset;
//    sc_in<sc_int<32>> frog_position[MAX_DIMENSION];
//    sc_in<sc_int<32>> best_position[MAX_DIMENSION];
//    sc_in<sc_int<32>> global_best_position[MAX_DIMENSION];
//    sc_in<sc_int<32>> step_size;
//    sc_out<sc_int<32>> new_position[MAX_DIMENSION];
//
//    // Internal Variables
//    sc_signal<int> local_iteration;
//
//    // Processes
//    void update_position() {
//        if (reset.read()) {
//            for (int i = 0; i < MAX_DIMENSION; i++) {
//                new_position[i].write(0);
//            }
//            local_iteration.write(0);
//            std::cout << "[DEBUG] Reset applied. Positions cleared." << std::endl;
//            return;
//        }
//
//        // Perform local search for L_m iterations
//        if (local_iteration.read() < L_m) {
//            bool improvement = false;
//
//            for (int i = 0; i < MAX_DIMENSION; i++) {
//                sc_int<32> diff = best_position[i].read() - frog_position[i].read();
//                double r = (rand() % 100) / 100.0; // Random value between 0 and 1
//
//                // Compute step size
//                sc_int<32> step = std::min(r * diff, (double)step_size.read());
//
//                // Update position
//                int new_pos = (frog_position[i].read() + step > 0) ? 1 : 0;
//                new_position[i].write(new_pos);
//
//                // Check if position improved
//                if (new_pos != frog_position[i].read()) {
//                    improvement = true;
//                }
//            }
//
//            // If no improvement, move toward global best
//            if (!improvement) {
//                for (int i = 0; i < MAX_DIMENSION; i++) {
//                    sc_int<32> diff = global_best_position[i].read() - frog_position[i].read();
//                    double r = (rand() % 100) / 100.0;
//                    sc_int<32> step = std::min(r * diff, (double)step_size.read());
//
//                    int new_pos = (frog_position[i].read() + step > 0) ? 1 : 0;
//                    new_position[i].write(new_pos);
//                }
//                std::cout << "[DEBUG] No improvement, moving toward global best.\n";
//            }
//
//            // Increment iteration count
//            local_iteration.write(local_iteration.read() + 1);
//        }
//    }
//
//    // Constructor
//    SC_CTOR(PositionUpdater) {
//        SC_METHOD(update_position);
//        sensitive << clk.pos();
//        dont_initialize();
//    }
//};




//#include <systemc.h>
//#include <iostream>
//#include <vector>
//#include <algorithm>
//#include <random>
//
//SC_MODULE(PositionUpdater) {
//    // Constants
//    static const int MAX_DIMENSION = 32;
//    static const int MAX_MEMEPLEXES = 8;
//    static const int MAX_FROGS_PER_MEMEPLEX = 4;
//    static const int L_m = 10; // Number of local search iterations
//
//    // Ports
//    sc_in<bool> clk;                                  // Clock signal
//    sc_in<bool> reset;                                // Reset signal
//    sc_in<sc_int<32>> frog_position[MAX_DIMENSION];   // Current position of the frog
//    sc_in<sc_int<32>> best_position[MAX_DIMENSION];   // Best frog's position in the memeplex
//    sc_in<sc_int<32>> global_best_position[MAX_DIMENSION]; // Global best frog position
//    sc_in<sc_int<32>> step_size;                      // Step size
//    sc_out<sc_int<32>> new_position[MAX_DIMENSION];   // Updated position
//
//    // Internal Variables
//    sc_signal<int> local_iteration;  // Counter for L_m iterations
//
//    // Processes
//    void update_position() {
//        if (reset.read()) {
//            // Reset all outputs
//            for (int i = 0; i < MAX_DIMENSION; i++) {
//                new_position[i].write(0); // Reset to 0
//            }
//            local_iteration.write(0);
//            std::cout << "[DEBUG] Reset applied. All positions cleared." << std::endl;
//            return;
//        }
//
//        // Perform local search for L_m iterations
//        if (local_iteration.read() < L_m) {
//            bool improvement = false;
//
//            for (int i = 0; i < MAX_DIMENSION; i++) {
//                sc_int<32> diff = best_position[i].read() - frog_position[i].read();
//                double r = (rand() % 100) / 100.0; // Random number between 0 and 1
//
//                // Compute step using SFLA formula
//                sc_int<32> step = sc_min<sc_int<32>>(r * diff, step_size.read());
//
//                // Update position (ensure binary format)
//                int new_pos = (frog_position[i].read() + step > 0) ? 1 : 0;
//                new_position[i].write(new_pos);
//
//                // Check if position improved
//                if (new_pos != frog_position[i].read()) {
//                    improvement = true;
//                }
//
//                // Debugging output
//                std::cout << "[DEBUG] Position Update - Dim: " << i
//                    << ", Old: " << frog_position[i].read()
//                    << ", Best: " << best_position[i].read()
//                    << ", Diff: " << diff
//                    << ", Step: " << step
//                    << ", New: " << new_position[i].read() << std::endl;
//            }
//
//            // If no improvement, try updating toward the global best
//            if (!improvement) {
//                for (int i = 0; i < MAX_DIMENSION; i++) {
//                    sc_int<32> diff = global_best_position[i].read() - frog_position[i].read();
//                    double r = (rand() % 100) / 100.0;
//
//                    // Compute step using U_G (global best)
//                    sc_int<32> step = sc_min<sc_int<32>>(r * diff, step_size.read());
//
//                    // Update position
//                    int new_pos = (frog_position[i].read() + step > 0) ? 1 : 0;
//                    new_position[i].write(new_pos);
//                }
//
//                std::cout << "[DEBUG] No improvement, moving toward global best.\n";
//            }
//
//            // If still no improvement, generate a random solution
//            if (!improvement) {
//                for (int i = 0; i < MAX_DIMENSION; i++) {
//                    int rand_pos = rand() % 2;
//                    new_position[i].write(rand_pos);
//                }
//
//                std::cout << "[DEBUG] No improvement, generating random solution.\n";
//            }
//
//            // Increment local search iteration
//            local_iteration.write(local_iteration.read() + 1);
//        }
//    }
//
//    // Constructor
//    SC_CTOR(PositionUpdater) {
//        SC_METHOD(update_position);
//        sensitive << clk.pos();
//        dont_initialize();
//    }
//};
//
//
//
//
//
//
////#include <systemc.h>
////#include <iostream>
////
////SC_MODULE(PositionUpdater) {
////    // Constants
////    static const int MAX_DIMENSION = 32;
////
////    // Ports
////    sc_in<bool> clk;                                  // Clock signal
////    sc_in<bool> reset;                                // Reset signal
////    sc_in<sc_int<32>> frog_position[MAX_DIMENSION];   // Current position of the frog
////    sc_in<sc_int<32>> best_position[MAX_DIMENSION];   // Best frog's position
////    sc_in<sc_int<32>> step_size;                      // Step size
////    sc_out<sc_int<32>> new_position[MAX_DIMENSION];   // Updated position
////
////    // Processes
////    void update_position() {
////        if (reset.read()) {
////            // Reset all outputs
////            for (int i = 0; i < MAX_DIMENSION; i++) {
////                new_position[i].write(0); // Reset to 0
////            }
////            std::cout << "[DEBUG] Reset applied. All positions cleared." << std::endl;
////            return;  // EXIT FUNCTION IMMEDIATELY
////        }
////
////        // Update positions based on step size
////        for (int i = 0; i < MAX_DIMENSION; i++) {
////            sc_int<32> diff = best_position[i].read() - frog_position[i].read();
////
////            // Ensure step_size is not zero
////            sc_int<32> step_size_val = step_size.read();
////            if (step_size_val == 0) {
////                std::cerr << "[ERROR] Step size is zero! This will cause stagnation. Setting to 1." << std::endl;
////                step_size_val = 1;  // Ensure movement occurs
////            }
////
////            // Calculate step movement
////            sc_int<32> step;
////            if (diff > 0) {
////                step = sc_min<sc_int<32>>(diff, step_size_val);
////            }
////            else {
////                step = sc_max<sc_int<32>>(diff, -step_size_val);
////            }
////
////            // Debug: Print step_size
////            std::cout << "[DEBUG] step_size: " << step_size.read() << std::endl;
////
////            // Ensure new_position remains binary
////            int new_pos = (frog_position[i].read() + step > 0) ? 1 : 0;
////            if (new_pos != 0 && new_pos != 1) {
////                std::cerr << "[ERROR] new_position[" << i << "] is not binary! Value: " << new_pos << std::endl;
////            }
////            new_position[i].write(new_pos);
////
////            // Debug: Print position update details
////            std::cout << "[DEBUG] Frog Position Update - Dimension: " << i
////                << ", Old Position: " << frog_position[i].read()
////                << ", Best Position: " << best_position[i].read()
////                << ", Diff: " << diff
////                << ", Step: " << step
////                << ", New Position: " << new_position[i].read() << std::endl;
////        }
////    }
////
////    // Constructor
////    SC_CTOR(PositionUpdater) {
////        SC_METHOD(update_position);
////        sensitive << clk.pos(); // Triggered on positive clock edge
////        dont_initialize();
////    }
////};
////
////
////
////
//////#include <systemc.h>
//////#include <iostream>
//////
//////SC_MODULE(PositionUpdater) {
//////    // Constants
//////    static const int MAX_DIMENSION = 32;
//////
//////    // Ports
//////    sc_in<bool> clk;                                  // Clock signal
//////    sc_in<bool> reset;                                // Reset signal
//////    sc_in<sc_int<32>> frog_position[MAX_DIMENSION];   // Current position of the frog
//////    sc_in<sc_int<32>> best_position[MAX_DIMENSION];   // Best frog's position
//////    sc_in<sc_int<32>> step_size;                      // Step size
//////    sc_out<sc_int<32>> new_position[MAX_DIMENSION];   // Updated position
//////
//////    // Processes
//////    void update_position() {
//////        if (reset.read()) {
//////            // Reset all outputs
//////            for (int i = 0; i < MAX_DIMENSION; i++) {
//////                new_position[i].write(0); // Reset to 0
//////            }
//////            std::cout << "[DEBUG] Reset applied. All positions cleared." << std::endl;
//////        }
//////        else {
//////            // Update positions based on step size
//////            for (int i = 0; i < MAX_DIMENSION; i++) {
//////                sc_int<32> diff = best_position[i].read() - frog_position[i].read();
//////                sc_int<32> step;
//////
//////                if (diff > 0) {
//////                    // If diff is positive, use the smaller of diff or step_size
//////                    step = sc_min<sc_int<32>>(diff, step_size.read());
//////                }
//////                else {
//////                    // If diff is negative, use the larger of diff or -step_size
//////                    step = sc_max<sc_int<32>>(diff, -step_size.read());
//////                }
//////
//////                // Debug: Print step_size
//////                std::cout << "[DEBUG] step_size: " << step_size.read() << std::endl;
//////
//////                // Update the frog's position (ENSURE IT REMAINS BINARY)
//////                if (frog_position[i].read() + step > 0) {
//////                    new_position[i].write(1);
//////                }
//////                else {
//////                    new_position[i].write(0);
//////                }
//////
//////                // Debug: Print position update details
//////                std::cout << "[DEBUG] Frog Position Update - Dimension: " << i
//////                    << ", Old Position: " << frog_position[i].read()
//////                    << ", Best Position: " << best_position[i].read()
//////                    << ", Diff: " << diff
//////                    << ", Step: " << step
//////                    << ", New Position: " << new_position[i].read() << std::endl;
//////            }
//////        }
//////    }
//////
//////    // Constructor
//////    SC_CTOR(PositionUpdater) {
//////        SC_METHOD(update_position);
//////        sensitive << clk.pos(); // Triggered on positive clock edge 
//////        dont_initialize();
//////    }
//////};
