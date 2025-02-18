#include <systemc.h>
#include <iostream>
#include <fstream>
#include <cstdlib>

SC_MODULE(SFLA_SystemC) {
    void execute() {
        std::ifstream checkFile1("fitness_evaluation.exe");
        std::ifstream checkFile2("position_updater.exe");

        if (!checkFile1) {
            std::cerr << "[ERROR] fitness_evaluation.exe not found!" << std::endl;
        }
        else {
            std::cout << "[INFO] Running Fitness Evaluation...\n";
            system("fitness_evaluation.exe");
        }

        if (!checkFile2) {
            std::cerr << "[ERROR] position_updater.exe not found!" << std::endl;
        }
        else {
            std::cout << "[INFO] Running Position Updater...\n";
            system("position_updater.exe");
        }

        // Call MATLAB for software optimization
        std::cout << "[INFO] Running MATLAB Optimization...\n";
        system("matlab -batch sfla_matlab");
    }

    SC_CTOR(SFLA_SystemC) {
        SC_THREAD(execute);
    }
};

int sc_main(int argc, char* argv[]) {
    SFLA_SystemC sfla_system("SFLA_SystemC");
    sc_start();
    return 0;
}


//#include <systemc.h>
//#include "fitness_evaluation.cpp"
//#include "population_sorter.cpp"
//#include "memeplex_divider.cpp"
//#include "control_unit.cpp"
//#include "shuffler.cpp"
//#include "position_updater.cpp"
//
//// Define constants for memeplex configuration
//#define MAX_MEMEPLEXES 8
//#define MAX_FROGS 32
//
//// Top-level module to integrate all SFLA components
//SC_MODULE(SFLA_System) {
//public:
//    // Sub-modules
//    FitnessEvaluation* fitness_evaluations[MAX_FROGS];
//    PopulationSorter* population_sorter;
//    MemeplexDivider* memeplex_divider;
//    ControlUnit* control_unit;
//    Shuffler* shuffler;
//    PositionUpdater* position_updaters[MAX_FROGS];
//
//    // Signals for interconnections
//    sc_signal<sc_int<32>> fitness_values[MAX_FROGS];
//    sc_signal<sc_uint<5>> sorted_indices[MAX_FROGS];
//    sc_signal<sc_uint<5>> memeplex_indices[MAX_MEMEPLEXES][MAX_FROGS / MAX_MEMEPLEXES];
//    sc_signal<sc_uint<8>> iteration_count;
//    sc_signal<bool> termination_flag, done;
//    sc_signal<sc_uint<5>> shuffled_indices[MAX_MEMEPLEXES * (MAX_FROGS / MAX_MEMEPLEXES)];
//    sc_signal<sc_int<32>> frog_positions[MAX_FROGS][32];
//    sc_signal<sc_int<32>> best_positions[MAX_FROGS][32];
//    sc_signal<sc_int<32>> global_best_positions[32];
//    sc_signal<sc_int<32>> step_size;
//    sc_signal<sc_int<32>> updated_positions[MAX_FROGS][32];
//    sc_signal<sc_uint<8>> max_iterations_signal;
//
//    // ✅ Signals for item weights, values, and capacity
//    sc_signal<sc_int<32>> item_weights[32];
//    sc_signal<sc_int<32>> item_values[32];
//    sc_signal<sc_int<32>> knapsack_capacity;
//
//    // Constructor
//    SC_CTOR(SFLA_System) {
//        // Instantiate FitnessEvaluation modules
//        for (int i = 0; i < MAX_FROGS; i++) {
//            fitness_evaluations[i] = new FitnessEvaluation(("FitnessEvaluation_" + std::to_string(i)).c_str());
//            fitness_evaluations[i]->clk(clk);
//            fitness_evaluations[i]->reset(reset);
//
//            for (int j = 0; j < 32; j++) {
//                fitness_evaluations[i]->frog_position[j](frog_positions[i][j]);
//            }
//
//            // ✅ Connect item weights and values
//            for (int j = 0; j < 32; j++) {
//                fitness_evaluations[i]->item_weights[j](item_weights[j]);
//                fitness_evaluations[i]->item_values[j](item_values[j]);
//            }
//
//            fitness_evaluations[i]->capacity(knapsack_capacity);
//            fitness_evaluations[i]->fitness_value(fitness_values[i]);
//        }
//
//        // Instantiate other modules
//        population_sorter = new PopulationSorter("PopulationSorter");
//        memeplex_divider = new MemeplexDivider("MemeplexDivider");
//        control_unit = new ControlUnit("ControlUnit");
//        shuffler = new Shuffler("Shuffler");
//
//        // Connect PopulationSorter
//        population_sorter->clk(clk);
//        population_sorter->reset(reset);
//        for (int i = 0; i < MAX_FROGS; i++) {
//            population_sorter->fitness_values[i](fitness_values[i]);
//            population_sorter->sorted_indices[i](sorted_indices[i]);
//        }
//
//        // Connect MemeplexDivider
//        memeplex_divider->clk(clk);
//        memeplex_divider->reset(reset);
//        for (int i = 0; i < MAX_FROGS; i++) {
//            memeplex_divider->sorted_indices[i](sorted_indices[i]);
//        }
//        for (int m = 0; m < MAX_MEMEPLEXES; m++) {
//            for (int f = 0; f < MAX_FROGS / MAX_MEMEPLEXES; f++) {
//                memeplex_divider->memeplex_indices[m][f](memeplex_indices[m][f]);
//            }
//        }
//
//        // Connect ControlUnit
//        control_unit->clk(clk);
//        control_unit->reset(reset);
//        control_unit->max_iterations(max_iterations_signal);
//        control_unit->termination_flag(termination_flag);
//        control_unit->iteration_count(iteration_count);
//        control_unit->done(done);
//
//        // Connect Shuffler
//        shuffler->clk(clk);
//        shuffler->reset(reset);
//        for (int i = 0; i < MAX_MEMEPLEXES; i++) {
//            for (int j = 0; j < MAX_FROGS / MAX_MEMEPLEXES; j++) {
//                shuffler->memeplex_indices[i][j](memeplex_indices[i][j]);
//            }
//        }
//        for (int i = 0; i < MAX_MEMEPLEXES * (MAX_FROGS / MAX_MEMEPLEXES); i++) {
//            shuffler->shuffled_indices[i](shuffled_indices[i]);
//        }
//
//        // Instantiate PositionUpdater modules
//        for (int i = 0; i < MAX_FROGS; i++) {
//            position_updaters[i] = new PositionUpdater(("PositionUpdater_" + std::to_string(i)).c_str());
//            position_updaters[i]->clk(clk);
//            position_updaters[i]->reset(reset);
//            position_updaters[i]->step_size(step_size);
//            for (int d = 0; d < 32; d++) {
//                position_updaters[i]->frog_position[d](frog_positions[i][d]);
//                position_updaters[i]->best_position[d](best_positions[i][d]);
//                position_updaters[i]->global_best_position[d](global_best_positions[d]);
//                position_updaters[i]->new_position[d](updated_positions[i][d]);
//            }
//        }
//
//        // Assign max_iterations dynamically
//        SC_METHOD(assign_max_iterations);
//        sensitive << frog_positions[0][0]; // ✅ Sensitivity changed to avoid errors
//        dont_initialize();
//    }
//
//    void assign_max_iterations() {
//        max_iterations_signal.write(static_cast<sc_uint<8>>(100)); // Default 100 iterations
//        std::cout << "[DEBUG] Max Iterations Signal: " << max_iterations_signal.read() << std::endl;
//    }
//
//    // Ports for external clock and reset
//    sc_in<bool> clk;
//    sc_in<bool> reset;
//};
//
//int sc_main(int argc, char* argv[]) {
//    // Instantiate top-level module
//    SFLA_System sfla_system("SFLA_System");
//
//    // Clock and reset signals
//    sc_clock clk("clk", 10, SC_NS);
//    sc_signal<bool> reset;
//
//    sfla_system.clk(clk);
//    sfla_system.reset(reset);
//
//    // ✅ Initialize item weights and values
//    int weights[32] = { 2, 3, 6, 7, 5, 9, 3, 4, 5, 1, 8, 6, 2, 7, 3, 5, 4, 3, 6, 8, 2, 3, 7, 9, 5, 4, 6, 7, 3, 8, 5, 2 };
//#include <systemc.h>
//#include <iostream>
//#include <fstream>
//#include <cstdlib>
//
//SC_MODULE(SFLA_SystemC) {
//    void execute() {
//        std::cout << "[INFO] Running Fitness Evaluation...\n";
//        system("./fitness_evaluation.exe");
//
//        std::cout << "[INFO] Running Position Updater...\n";
//        system("./position_updater.exe");
//    }
//
//    SC_CTOR(SFLA_SystemC) {
//        SC_THREAD(execute);
//    }
//};
//
//int sc_main(int argc, char* argv[]) {
//    SFLA_SystemC sfla_system("SFLA_SystemC");
//    sc_start();
//    return 0;
//}
//    int values[32] = { 6, 5, 8, 9, 6, 7, 3, 6, 8, 2, 7, 4, 5, 9, 6, 3, 8, 5, 4, 6, 9, 3, 8, 6, 7, 5, 4, 8, 9, 6, 7, 5 };
//
//    for (int i = 0; i < 32; i++) {
//        sfla_system.item_weights[i].write(weights[i]);
//        sfla_system.item_values[i].write(values[i]);
//    }
//
//    // ✅ Set knapsack capacity
//    sfla_system.knapsack_capacity.write(50);
//
//    // ✅ Initialize frog_positions randomly (Ensuring at least one selected item)
//    for (int i = 0; i < MAX_FROGS; i++) {
//        bool hasSelectedItems = false;
//        for (int j = 0; j < 32; j++) {
//            int random_bit = rand() % 2;
//            sfla_system.frog_positions[i][j].write(random_bit);
//            if (random_bit == 1) hasSelectedItems = true;
//        }
//        if (!hasSelectedItems) {
//            int forceSelectionIndex = rand() % 32;
//            sfla_system.frog_positions[i][forceSelectionIndex].write(1);
//        }
//    }
//
//    // Run simulation
//    sc_start(1000, SC_NS);
//    std::cout << "[DEBUG] SFLA Algorithm Completed." << std::endl;
//
//    return 0;
//}