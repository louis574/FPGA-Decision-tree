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


module tree_node_tb;


    localparam int rows=8;
    localparam int width=10;
    localparam int struct_array [0:rows-1][0:width-1] = '{{0,0,15,14,1,1,3,2,-1,-1},{1,0,1,0,0,3,1,4,2,5},{2,0,3,0,8,6,1,7,-1,-1}, {3,1,0,-1,-1,-1,-1,-1,-1,-1}, {4,1,1,-1,-1,-1,-1,-1,-1,-1}, {5,1,2,-1,-1,-1,-1,-1,-1,-1}, {6,1,24,-1,-1,-1,-1,-1,-1,-1}, {7,1,3,-1,-1,-1,-1,-1,-1,-1}}; //curly for declaring arrays
    localparam int splits [0:rows-1] = '{2,3,2,1,1,1,1,1};
    localparam int layer = 0;
    localparam int output_width = 16;
    localparam int input_width = 16;
    
    reg [input_width-1:0] inputs;
    wire [output_width-1:0] result;
    
    tree_node #(.rows(rows), .width(width), .struct_array(struct_array), .splits(splits), .layer(layer), .output_width(output_width), .input_width(input_width)) t
    (
        .inputs(inputs),
        .result(result)
            
    );
    
    initial begin
    
        inputs = 16'b0111111111111100;
        //should output 0
        #50;
        inputs = 16'b0111111111111110;
        //should output 2
        #50;
        inputs = 16'b0111111111111101;
        //should output 1
        #50;
        inputs = 16'b0100000000000001;
        //should output 1
        #50;
        inputs = 16'b1111111111111000;
        //should output 24
        #50;
        inputs = 16'b1111111111110001;
        //should output 3
        #50;
        inputs = 16'b1111111111111110;
        //should output XX
        #50;
        inputs = 16'b0111111111111110;
        //should output 2
        #50;
        
        $finish;
    
    end    
    
endmodule
