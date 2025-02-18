# Shuffled Frog Leaping Algorithm (SFLA)

The purpose of this project is to familiarize with the practical implementation of hardware-software co-design processes and to gain a deeper understanding of metaheuristic optimization algorithms. To achieve this, the design and implementation of the **Shuffled Frog Leaping Algorithm (SFLA)** have been considered. The goal of this design is to effectively utilize this algorithm in embedded systems while improving its optimization efficiency, considering cost and implementation space constraints. For this purpose, the main components of this algorithm have been structured as functional blocks, and their implementation in either hardware or software is discussed.

Optimization problems aim to find the best possible solutions to real-world and practical issues. Among various optimization approaches, **metaheuristic algorithms** stand out as one of the most effective methods for solving such problems with cost considerations while maintaining efficiency. Among these, **nature-inspired algorithms**, which draw inspiration from the social behavior of organisms in nature, are highly popular. One such algorithm is the **Shuffled Frog Leaping Algorithm (SFLA)**, which is a **population-based** algorithm inspired by the way frogs leap to search for food. It is widely applied in engineering applications, including **parameter optimization, sensor network design, and machine learning**. This algorithm combines two existing methods: **Shuffled Complex Evolution (SCE) and Particle Swarm Optimization (PSO)**. The optimization problem it tackles can be visualized as finding a stone in a pond that has the maximum amount of accessible food.

This algorithm is mainly used for solving discrete optimization problems, with its key advantage being its **high computational speed while maintaining reasonable accuracy** for general optimization tasks. However, it suffers from drawbacks such as **getting trapped in local optima and requiring a long convergence time**, which have been addressed in subsequent improvements.

A **modular design** for this algorithm, along with segmentation and distribution between **hardware and software processing units**, is one of the most effective methods for improving performance and integrating it into systems. The decision on whether to implement each module in hardware or software depends on the problem and system requirements. The aim of this project is to implement a **co-design process** for this algorithm in a **hardware-software integrated system** to ensure optimal performance and cost-effectiveness.

 In this algorithm, the **initial population** is divided into several **subgroups**, referred to as **memeplexes**. Each frog in a **memeplex** is represented by a vector indicating its position. A memeplex consists of a set of frogs, with each frog being represented by a decision parameter vector:

\[
U_i = \{U_i^1, U_i^2, U_i^3, ..., U_i^d\}
\]

where **d** denotes the number of decision parameters. A **local search** is conducted within each **memeplex**, and after several iterations, information is exchanged between memeplexes. This process continues until the **termination criteria** are met.

### **Algorithm Steps:**
1. **Determine the number of memeplexes** (\(M\)) and assign a size to each (\(N\)).
2. **Generate an initial population** of frogs, given by: \( P = M \times N \).
3. **Evaluate each frog's fitness** using the **Fitness Function**.
4. **Sort frogs in descending order** based on their fitness and store them in an array \( X \), with the best solution labeled as \( P_B \).
5. **Divide solutions among memeplexes** sequentially (e.g., \(U_1\) to the first memeplex, \(U_2\) to the second, etc.).
6. **Perform local search:**
   - Select \( n \) solutions randomly using the **weighted probability equation**:

   \[
   p_j = \frac{2(n + 1 - j)}{n(n + 1)}, j \in \{1,2, … n\}
   \]

   - Sort these solutions in descending order and identify the **best** (\(P_B\)) and **worst** (\(P_W\)).
   - **Step size (S)** for updating frog positions is calculated using:

   \[
   S = \min [r(U_B - U_W), S_{max}], \quad \text{for positive steps}
   \]

   \[
   S = \max [r(U_W - U_B), -S_{max}], \quad \text{for negative steps}
   \]

   where **r** is a random number between **0 and 1**.

   - The **new position** of the worst frog is updated as:

   \[
   U_W' = U_W + S
   \]

   - If the new position is better than the previous, it replaces the old position; otherwise, a new step size is calculated using the **best global solution** (\(U_G\)). If no improvement is observed, a **random solution** is generated.
   - This **local search process repeats** for a predefined number of iterations (\(L_m\)).
7. **Update the best solution \(U_B\)** and reassign solutions among memeplexes.
8. **Repeat the process until termination conditions** (maximum iterations \(G_m\) or minimal improvement threshold \( \epsilon \)) are met.

A **flowchart** illustrating the overall algorithm process is provided in the next section.




- **Parallelization strategies** will be employed to distribute operations between **hardware and software modules**.


For system implementation, the **SystemC language** should be used to describe hardware components, and proper connectivity between hardware and software modules should be established to finalize the design.



### **Evaluation and Benchmarking:**
To **validate** the implementation, the **Knapsack Problem** will be used as a test case:

**Given items with the following properties:**
| Item  | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  |
|-------|----|----|----|----|----|----|----|----|----|
| Price | 6  | 5  | 8  | 9  | 6  | 7  | 3  | 6  | 8  |
| Weight| 2  | 3  | 6  | 7  | 5  | 9  | 3  | 4  | 5  |

The system must determine **the maximum possible profit**, ensuring that fractional selection of items is not allowed.

**Expected Output:**
- The **results** obtained from evaluating the designed system.
- **Code implementations** for both hardware and software components.

**Parameter Setup:**
- The **termination condition** should be **no change in the optimal solution for 10 consecutive iterations** (not just the first 10 iterations).
- The **initial population** must be generated **randomly**.
- The software component can be implemented in **Matlab**.
- If a correct connection between **simulated SystemC hardware** and the **software module** is established, the design will be considered **complete**.

