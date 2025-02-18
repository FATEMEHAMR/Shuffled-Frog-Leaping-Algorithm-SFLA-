#include <systemc.h>
#include <iostream>
#include <fstream>

SC_MODULE(FitnessEvaluation) {
    sc_in<bool> clk;
    sc_in<bool> reset;
    sc_in<sc_int<32>> frog_position[32];
    sc_in<sc_int<32>> item_weights[32];
    sc_in<sc_int<32>> item_values[32];
    sc_in<sc_int<32>> capacity;
    sc_out<sc_int<32>> fitness_value;

    void compute_fitness() {
        if (reset.read()) {
            fitness_value.write(0);
            return;
        }

        int total_value = 0, total_weight = 0;
        for (int i = 0; i < 32; i++) {
            if (frog_position[i].read() == 1) {
                total_value += item_values[i].read();
                total_weight += item_weights[i].read();
            }
        }

        int penalty = (total_weight > capacity.read()) ? (total_weight - capacity.read()) / 2 : 0;
        fitness_value.write(total_value - penalty);
    }

    SC_CTOR(FitnessEvaluation) {
        SC_METHOD(compute_fitness);
        sensitive << clk.pos();
    }
};


//int sc_main(int argc, char* argv[]) {
//    FitnessEvaluation fitness_evaluator("FitnessEvaluation");
//    sc_start();
//    return 0;
//}



//#include <systemc.h>
//#include <iostream>
//#include <algorithm>
//
//SC_MODULE(FitnessEvaluation) {
//    // Ports
//    sc_in<bool> clk;
//    sc_in<bool> reset;
//    sc_in<sc_int<32>> frog_position[32];
//    sc_in<sc_int<32>> item_weights[32];
//    sc_in<sc_int<32>> item_values[32];
//    sc_in<sc_int<32>> capacity;
//    sc_out<sc_int<32>> fitness_value;
//
//    // Internal variables
//    sc_signal<sc_int<32>> total_value;
//    sc_signal<sc_int<32>> total_weight;
//    sc_signal<sc_int<32>> penalty;
//
//    // Processes
//    void compute_fitness() {
//        if (reset.read()) {
//            total_value.write(0);
//            total_weight.write(0);
//            penalty.write(0);
//            fitness_value.write(0);
//            std::cout << "[DEBUG] Reset applied. Internal signals set to 0." << std::endl;
//            return;
//        }
//
//        // Initialize accumulators
//        sc_int<32> val_sum = 0;
//        sc_int<32> wt_sum = 0;
//
//        // Compute total value and weight
//        for (int i = 0; i < 32; i++) {
//            if (frog_position[i].read() == 1) {
//                val_sum += item_values[i].read();
//                wt_sum += item_weights[i].read();
//            }
//        }
//
//        // Store computed values
//        total_value.write(val_sum);
//        total_weight.write(wt_sum);
//
//        // Compute penalty if weight exceeds capacity
//        sc_int<32> capacity_val = capacity.read();
//        sc_int<32> excess_weight = wt_sum - capacity_val;
//        sc_int<32> applied_penalty = (excess_weight > 0) ? excess_weight / 2 : 0;
//        penalty.write(applied_penalty);
//
//        // Compute fitness
//        sc_int<32> new_fitness = (val_sum > applied_penalty) ? (val_sum - applied_penalty) : 1;
//        fitness_value.write(new_fitness);
//
//        // Debugging output
//        std::cout << "[DEBUG] Frog Fitness Computed: ";
//        std::cout << "Total Weight: " << wt_sum
//            << ", Total Value: " << val_sum
//            << ", Capacity: " << capacity_val
//            << ", Excess Weight: " << excess_weight
//            << ", Penalty: " << applied_penalty
//            << ", Fitness: " << new_fitness << std::endl;
//    }
//
//    // Constructor
//    SC_CTOR(FitnessEvaluation) {
//        SC_METHOD(compute_fitness);
//        sensitive << clk.pos();
//        dont_initialize();
//    }
//};




//#include <systemc.h>
//#include <iostream>
//#include <algorithm>
//
//SC_MODULE(FitnessEvaluation) {
//    // Ports
//    sc_in<bool> clk;
//    sc_in<bool> reset;
//    sc_in<sc_int<32>> frog_position[32];
//    sc_in<sc_int<32>> item_weights[32];
//    sc_in<sc_int<32>> item_values[32];
//    sc_in<sc_int<32>> capacity;
//    sc_out<sc_int<32>> fitness_value;
//
//    // Internal variables
//    sc_signal<sc_int<32>> total_value;
//    sc_signal<sc_int<32>> total_weight;
//    sc_signal<sc_int<32>> penalty;
//
//    // Processes
//    void compute_fitness() {
//        if (reset.read()) {
//            total_value.write(0);
//            total_weight.write(0);
//            penalty.write(0);
//            fitness_value.write(0);
//            std::cout << "[DEBUG] Reset applied. Internal signals set to 0." << std::endl;
//            return;
//        }
//
//        // Initialize accumulators
//        sc_int<32> val_sum = 0;
//        sc_int<32> wt_sum = 0;
//
//        // Compute total value and weight
//        for (int i = 0; i < 32; i++) {
//            if (frog_position[i].read() == 1) {
//                val_sum += item_values[i].read();
//                wt_sum += item_weights[i].read();
//            }
//        }
//
//        // Store computed values
//        total_value.write(val_sum);
//        total_weight.write(wt_sum);
//
//        // Compute penalty if weight exceeds capacity
//        sc_int<32> capacity_val = capacity.read();
//        sc_int<32> excess_weight = wt_sum - capacity_val;
//        sc_int<32> applied_penalty = (excess_weight > 0) ? excess_weight / 2 : 0;
//        penalty.write(applied_penalty);
//
//        // Compute fitness
//        sc_int<32> new_fitness = (val_sum > applied_penalty) ? (val_sum - applied_penalty) : 1;
//        fitness_value.write(new_fitness);
//
//        // Debugging output
//        std::cout << "[DEBUG] Frog Fitness Computed: ";
//        std::cout << "Total Weight: " << wt_sum
//            << ", Total Value: " << val_sum
//            << ", Capacity: " << capacity_val
//            << ", Excess Weight: " << excess_weight
//            << ", Penalty: " << applied_penalty
//            << ", Fitness: " << new_fitness << std::endl;
//    }
//
//    // Constructor
//    SC_CTOR(FitnessEvaluation) {
//        SC_METHOD(compute_fitness);
//        sensitive << clk.pos();
//        dont_initialize();
//    }
//};



//#include <systemc.h>
//#include <iostream>
//#include <algorithm>
//
//SC_MODULE(FitnessEvaluation) {
//    // Ports
//    sc_in<bool> clk;
//    sc_in<bool> reset;
//    sc_in<sc_int<32>> frog_position[32];
//    sc_in<sc_int<32>> item_weights[32];
//    sc_in<sc_int<32>> item_values[32];
//    sc_in<sc_int<32>> capacity;
//    sc_out<sc_int<32>> fitness_value;
//
//    // Internal variables
//    sc_signal<sc_int<32>> total_value;
//    sc_signal<sc_int<32>> total_weight;
//    sc_signal<sc_int<32>> penalty;
//
//    // Processes
//    void compute_fitness() {
//        if (reset.read()) {
//            total_value.write(0);
//            total_weight.write(0);
//            penalty.write(0);
//            fitness_value.write(0);
//            std::cout << "[DEBUG] Reset applied. Internal signals set to 0." << std::endl;
//            return;
//        }
//
//        sc_int<32> val_sum = 0;
//        sc_int<32> wt_sum = 0;
//
//        std::cout << "[DEBUG] Frog Position: ";
//        bool has_selected_items = false; // ✅ Track if at least one item is selected
//
//        for (int i = 0; i < 32; i++) {
//            int pos = frog_position[i].read();
//            std::cout << pos << " ";
//
//            if (pos == 1) {
//                has_selected_items = true; // ✅ At least one item selected
//                val_sum += item_values[i].read();
//                wt_sum += item_weights[i].read();
//            }
//        }
//        std::cout << std::endl;
//
//        total_value.write(val_sum);
//        total_weight.write(wt_sum);
//
//        // ✅ Check if any items were selected
//        if (!has_selected_items) {
//            std::cerr << "[WARNING] Frog selected no items. Fitness will be low." << std::endl;
//        }
//
//        // ✅ Compute penalty correctly
//        sc_int<32> capacity_val = capacity.read();
//        sc_int<32> excess_weight = wt_sum - capacity_val;
//        sc_int<32> applied_penalty = (excess_weight > 0) ? excess_weight / 2 : 0; // Reduce penalty severity
//        penalty.write(applied_penalty);
//
//        // ✅ Ensure fitness is never negative
//        sc_int<32> new_fitness = (val_sum > applied_penalty) ? (val_sum - applied_penalty) : 1;
//        fitness_value.write(new_fitness);
//
//        // ✅ Debug output
//        std::cout << "[DEBUG] Total Weight: " << wt_sum
//            << ", Total Value: " << val_sum
//            << ", Capacity: " << capacity_val
//            << ", Excess Weight: " << excess_weight
//            << ", Penalty: " << applied_penalty
//            << ", Fitness: " << new_fitness << std::endl;
//    }
//
//
//    // Constructor
//    SC_CTOR(FitnessEvaluation) {
//        SC_METHOD(compute_fitness);
//        sensitive << clk.pos();
//        dont_initialize();
//    }
//};




//#include <systemc.h>
//#include <iostream>
//#include <algorithm>
//
//SC_MODULE(FitnessEvaluation) {
//    // Ports
//    sc_in<bool> clk;
//    sc_in<bool> reset;
//    sc_in<sc_int<32>> frog_position[32];
//    sc_in<sc_int<32>> item_weights[32];
//    sc_in<sc_int<32>> item_values[32];
//    sc_in<sc_int<32>> capacity;
//    sc_out<sc_int<32>> fitness_value;
//
//    // Internal variables
//    sc_signal<sc_int<32>> total_value;
//    sc_signal<sc_int<32>> total_weight;
//    sc_signal<sc_int<32>> penalty;
//
//    // Processes
//    void compute_fitness() {
//        if (reset.read()) {
//            total_value.write(0);
//            total_weight.write(0);
//            penalty.write(0);
//            fitness_value.write(0);
//            std::cout << "[DEBUG] Reset applied. Internal signals set to 0." << std::endl;
//            return;
//        }
//
//        // Initialize accumulators
//        sc_int<32> val_sum = 0;
//        sc_int<32> wt_sum = 0;
//
//        // Debug: Print frog position
//        std::cout << "[DEBUG] Frog Position: ";
//        for (int i = 0; i < 32; i++) {
//            int pos = frog_position[i].read();
//            std::cout << pos << " ";
//            if (pos != 0 && pos != 1) {
//                std::cerr << "[ERROR] Invalid frog_position[" << i << "] = " << pos << "! Expected 0 or 1." << std::endl;
//            }
//        }
//        std::cout << std::endl;
//
//        // Compute total value and weight
//        for (int i = 0; i < 32; i++) {
//            if (frog_position[i].read() == 1) {
//                val_sum += item_values[i].read();
//                wt_sum += item_weights[i].read();
//            }
//        }
//
//        // Store computed values
//        total_value.write(val_sum);
//        total_weight.write(wt_sum);
//
//        // Read capacity
//        sc_int<32> capacity_val = capacity.read();
//        if (capacity_val <= 0) {
//            std::cerr << "[ERROR] Capacity is non-positive: " << capacity_val << "! This may cause all frogs to be penalized." << std::endl;
//        }
//
//        // Compute penalty if weight exceeds capacity
//        sc_int<32> excess_weight = wt_sum - capacity_val;
//        sc_int<32> applied_penalty = (excess_weight > 0) ? excess_weight / 2 : 0;  // Reduced penalty to prevent fitness from always being zero
//        penalty.write(applied_penalty);
//
//        // Compute fitness
//        sc_int<32> new_fitness = (val_sum > applied_penalty) ? (val_sum - applied_penalty) : 1;  // Ensure fitness is always at least 1
//        fitness_value.write(new_fitness);
//
//        // Debugging output
//        std::cout << "[DEBUG] Total Weight: " << wt_sum
//            << ", Total Value: " << val_sum
//            << ", Capacity: " << capacity_val
//            << ", Excess Weight: " << excess_weight
//            << ", Penalty: " << applied_penalty
//            << ", Fitness: " << new_fitness << std::endl;
//
//        if (new_fitness <= 0) {
//            std::cerr << "[WARNING] Fitness value is zero or negative!" << std::endl;
//        }
//    }
//
//    // Constructor
//    SC_CTOR(FitnessEvaluation) {
//        SC_METHOD(compute_fitness);
//        sensitive << clk.pos();
//        dont_initialize();
//    }
//};





//#include <systemc.h>
//#include <iostream>
//#include <algorithm>
//
//SC_MODULE(FitnessEvaluation) {
//    // Ports
//    sc_in<bool> clk;
//    sc_in<bool> reset;
//    sc_in<sc_int<32>> frog_position[32];
//    sc_in<sc_int<32>> item_weights[32];
//    sc_in<sc_int<32>> item_values[32];
//    sc_in<sc_int<32>> capacity;
//    sc_out<sc_int<32>> fitness_value;
//
//    // Internal variables
//    sc_signal<sc_int<32>> total_value;
//    sc_signal<sc_int<32>> total_weight;
//    sc_signal<sc_int<32>> penalty;
//
//    // Processes
//    void compute_fitness() {
//        if (reset.read()) {
//            total_value.write(0);
//            total_weight.write(0);
//            penalty.write(0);
//            fitness_value.write(0);
//            std::cout << "[DEBUG] Reset applied. Internal signals set to 0." << std::endl;
//            return;
//        }
//
//        // Initialize accumulators for value and weight
//        sc_int<32> val_sum = 0;
//        sc_int<32> wt_sum = 0;
//
//        // Debugging: Print frog position
//        std::cout << "[DEBUG] Frog Position: ";
//        for (int i = 0; i < 32; i++) {
//            int pos = frog_position[i].read();
//            std::cout << pos << " ";
//            if (pos != 0 && pos != 1) {
//                std::cerr << "[ERROR] Invalid frog_position[" << i << "] = " << pos << "! Expected 0 or 1." << std::endl;
//            }
//        }
//        std::cout << std::endl;
//
//        // Compute total value and weight
//        for (int i = 0; i < 32; i++) {
//            if (frog_position[i].read() == 1) {
//                val_sum += item_values[i].read();
//                wt_sum += item_weights[i].read();
//            }
//        }
//
//        // Store computed values
//        total_value.write(val_sum);
//        total_weight.write(wt_sum);
//
//        // Read capacity
//        sc_int<32> capacity_val = capacity.read();
//        if (capacity_val <= 0) {
//            std::cerr << "[ERROR] Capacity is non-positive: " << capacity_val << "! This may cause all frogs to be penalized." << std::endl;
//        }
//
//        // Compute penalty if weight exceeds capacity
//        sc_int<32> excess_weight = wt_sum - capacity_val;
//        sc_int<32> applied_penalty = (excess_weight > 0) ? excess_weight : sc_int<32>(0);
//        //sc_int<32> applied_penalty = (excess_weight > 0) ? excess_weight : 0;
//        penalty.write(applied_penalty);
//
//        // Compute fitness
//        sc_int<32> new_fitness = (val_sum > applied_penalty) ? (val_sum - applied_penalty) : 1;
//        fitness_value.write(new_fitness);
//
//        // Debugging output
//        std::cout << "[DEBUG] Total Weight: " << wt_sum
//            << ", Total Value: " << val_sum
//            << ", Capacity: " << capacity_val
//            << ", Excess Weight: " << excess_weight
//            << ", Penalty: " << applied_penalty
//            << ", Fitness: " << new_fitness << std::endl;
//
//        if (new_fitness <= 0) {
//            std::cerr << "[WARNING] Fitness value is zero or negative!" << std::endl;
//        }
//    }
//
//    // Constructor
//    SC_CTOR(FitnessEvaluation) {
//        SC_METHOD(compute_fitness);
//        sensitive << clk.pos();
//        dont_initialize();
//    }
//};






//#include <systemc.h>
//#include <iostream>
//#include <algorithm>
//
//SC_MODULE(FitnessEvaluation) {
//    // Ports
//    sc_in<bool> clk;                     // Clock signal
//    sc_in<bool> reset;                   // Reset signal
//    sc_in<sc_int<32>> frog_position[32]; // Frog's binary representation (up to 32 items)
//    sc_in<sc_int<32>> item_weights[32];  // Weights of the items
//    sc_in<sc_int<32>> item_values[32];   // Values of the items
//    sc_in<sc_int<32>> capacity;          // Knapsack capacity
//    sc_out<sc_int<32>> fitness_value;    // Output fitness value
//
//    // Internal variables
//    sc_signal<sc_int<32>> total_value;
//    sc_signal<sc_int<32>> total_weight;
//    sc_signal<sc_int<32>> penalty;
//
//    // Processes
//    void compute_fitness() {
//        if (reset.read()) {
//            total_value.write(0);
//            total_weight.write(0);
//            penalty.write(0);
//            fitness_value.write(0);
//            std::cout << "[DEBUG] Reset applied. Internal signals set to 0." << std::endl;
//            return;  // EXIT FUNCTION IMMEDIATELY
//        }
//
//        //if (reset.read()) {
//        //    // Reset all internal signals and outputs
//        //    total_value.write(0);
//        //    total_weight.write(0);
//        //    penalty.write(0);
//        //    fitness_value.write(0);
//        //    std::cout << "[DEBUG] Reset applied. Internal signals set to 0." << std::endl;
//        //}
//        else {
//            // Initialize accumulators for value and weight
//            sc_int<32> val_sum = 0;
//            sc_int<32> wt_sum = 0;
//
//            // Debug: Print frog position
//            std::cout << "[DEBUG] Frog Position in compute_fitness: ";
//            for (int i = 0; i < 32; i++) {
//                int pos = frog_position[i].read();
//                std::cout << pos << " ";
//                if (pos != 0 && pos != 1) {
//                    std::cerr << "[ERROR] Invalid frog_position[" << i << "] = " << pos << "! Expected 0 or 1." << std::endl;
//                }
//            }
//            std::cout << std::endl;
//
//            /*std::cout << "[DEBUG] Frog Position in compute_fitness: ";
//            for (int i = 0; i < 32; i++) {
//                std::cout << frog_position[i].read() << " ";
//            }
//            std::cout << std::endl;*/
//
//            // Calculate total value and total weight for the current frog
//            for (int i = 0; i < 32; i++) {
//                if (frog_position[i].read() == 1) {
//                    val_sum += item_values[i].read();
//                    wt_sum += item_weights[i].read();
//                }
//            }
//
//            // Write total value and weight to respective signals
//            total_value.write(val_sum);
//            total_weight.write(wt_sum);
//
//            // Compute penalty if the total weight exceeds the capacity
//            sc_int<32> capacity_val = capacity.read();
//            if (capacity_val <= 0) {
//                std::cerr << "[ERROR] Capacity is non-positive: " << capacity_val << "! This may cause all frogs to be penalized." << std::endl;
//            }
//            sc_int<32> excess_weight = wt_sum - capacity_val;
//            //sc_int<32> excess_weight = wt_sum - capacity.read();
//
//            if (excess_weight > 0) {
//                penalty.write(excess_weight); // Apply penalty proportional to excess weight
//            }
//            else {
//                penalty.write(0); // No penalty
//            }
//
//            // Calculate fitness as total value minus penalty, clamped to zero
//            fitness_value.write(sc_max<sc_int<32>>(val_sum - penalty.read(), 0));
//
//            // Debugging output
//            std::cout << "[DEBUG] Total Weight: " << total_weight.read()
//                << ", Total Value: " << total_value.read()
//                << ", Penalty: " << penalty.read()
//                << ", Fitness: " << fitness_value.read() << std::endl;
//
//            // Detect frogs with zero or negative fitness for debugging purposes
//
//            /*if (fitness_value.read() <= 0) {
//                std::cout << "[DEBUG] Frog Issue Detected!" << std::endl;
//            }*/
//            if (fitness_value.read() <= 0) {
//                std::cerr << "[WARNING] Frog fitness is zero or negative! Value=" << fitness_value.read()
//                    << ", Total Value=" << total_value.read() << ", Penalty=" << penalty.read() << std::endl;
//            }
//
//        }
//    }
//
//    // Constructor
//    SC_CTOR(FitnessEvaluation) {
//        SC_METHOD(compute_fitness);
//        sensitive << clk.pos(); // Triggered on positive clock edge
//        dont_initialize();
//    }
//};
