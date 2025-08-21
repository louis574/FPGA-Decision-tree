`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 18.08.2025 23:13:22
// Design Name: 
// Module Name: tree_node_tb
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

//Input structure
//node:
//[line_no, node (0), bit_top, bit_end, val_1, pointer_1, ....]
//leaf:
//[line_no, leaf (1), output]

//will all be padded to width of longest one, with -1 as a filler

`include "param_declare.vh"

module tree_node_tb;



    
    reg [input_width-1:0] inputs;
    wire [output_width-1:0] result;
    
    tree_node t
    (
        .inputs(inputs),
        .result(result)
            
    );
    
    initial begin
    
        inputs = 4'b0101;
        //should output 2
        #50;
        
        $finish;
    
    end    
    
endmodule
