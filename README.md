# FPGA-Decision-tree

This is a personal project I built to explore how classic machine learning algorithms can be translated into hardware.  
I was particularly interested in decision trees because they’re one of the forms of machine learning that can really excel on FPGAs:  
they can be encoded into minimal bit-widths, and their logic boils down to just multiplexers and comparators.  
That makes them naturally **quick, low-power, and low-resource** — exactly the kind of workload where hardware shines.  
The project ended up splitting into two halves: a C++ program that constructs and flattens the tree, and a recursive SystemVerilog design that rebuilds it in hardware.  
Along the way, I focused on keeping the design efficient and FPGA-friendly, with compact input encodings and tiny LUT usage.  


---

## 🔧 Features  

### Tree Constructor (C++)  
- Implements an **ID3 classifier** for **discrete/categorical input data**.  
- Uses **entropy-based splitting** to minimise node count.  
- Supports **multi-branch splits** (not restricted to binary) to minimise node count.  
- Built **recursively**, then flattened into a **custom array format**.  


#### Input Encoding & Hardware Efficiency  
- C++ **encodes categories into compact bitfields**, minimising input width.  
- Example: categories `{red, green, blue}` → encodings `{00, 01, 10}`.  
- A mapping file `encodings.txt` is generated to show which bit fields in the compact input word represent which catgories and the different encodings for each feature.  
- This reduces the number of FPGA input bits required, massively cutting **LUT/FF utilisation**.  

#### Verilog Export  
- Exports the tree as a **Verilog header file** (`param_declare.vh`).  
- This header defines the complete array representation of the trained tree for the systemverilog tree constructor to build in hardware.  

---

### SystemVerilog Implementation  
- Single file: **`tree_node.sv`**.  
- Imports the auto-generated **header file** (`param_declare.vh`).  
- Implements the tree using a **recursive module design**:  
  - Each node is a module instance.  
  - If the node is internal, the module **instantiates child modules** for each branch, passing down the appropriate parameters.  
  - If the node is a leaf, the module directly outputs the classification.  
- This recursive instantiation naturally builds the entire tree structure in hardware.  
- Includes `tree_node_tb.sv` as a verification testbench with example inputs (which can be found in the c++ code).

## 🏁 Results  

As hoped, decision trees turned out to be a great match for FPGA hardware.  
Since the C++ constructor encodes categories into minimal bitfields, the hardware only ever sees compact inputs.  
The SystemVerilog implementation then resolves each decision using just comparators and multiplexers, which keeps the logic depth shallow and the utilisation tiny.  

- **Critical path delay**: ~**+1.3 ns per tree layer**  
- **Latency**: **1 clock tick** for output  
- **Resource usage**: extremely small — a **7-node tree optimised to just 1 LUT**  

