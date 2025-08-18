`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 18.08.2025 22:18:58
// Design Name: 
// Module Name: mux
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

module mux #(parameter int rows,int width, int struct_array [0:rows-1][0:width-1], int splits [0:rows-1], int layer, int output_width, int input_width)(
    input [input_width-1:0] inputs,
    input [output_width-1:0] outs [0:splits[layer]-1],
    output reg [output_width-1:0] results
    );
    
    int i;
    

    
    always @(*) begin
        for(i=0; i<splits[layer]; i = i+1) begin
            if(struct_array[layer][4 + i*2] == inputs[struct_array[layer][2]:struct_array[layer][3]]) begin
                results = outs[i];
            end
        end
    end
    
endmodule
