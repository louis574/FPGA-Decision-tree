`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 18.08.2025 17:57:06
// Design Name: 
// Module Name: tree_node
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

//header declarations are global for all instantiations

`include "param_declare.vh"

module  tree_node #(parameter int layer = 0)

(
    input [input_width-1:0] inputs,
    output reg [output_width-1:0] result

    );
    
    genvar i;
    int x;
    
    
    wire [output_width-1:0] outs [0:splits[layer]-1];
    
    generate
            if(struct_array[layer][1]) begin
                assign result = outs[0];
            end
            else begin
                always @(*) begin
                result = {output_width{1'b1}}; //to protect from latches (even in dead nodes that arent expressed but are still used)
                    for(x=0; x<splits[layer]; x = x+1) begin
                        if(struct_array[layer][4 + x*2] == inputs[struct_array[layer][2]:struct_array[layer][3]]) begin
                            result = outs[x];
                        end
                    end
                end
            end
    endgenerate
    
    
    generate
        if(struct_array[layer][1]) begin : leaf
            assign outs[0] = struct_array[layer][2];
        end
        else begin //node
            for(i=0; i < splits[layer]; i = i+1) begin : node
                tree_node #(.layer(struct_array[layer][5+2*i])) node_inst 
                (
                .inputs(inputs),
                .result(outs[i])
                );
            end                    
        end
    endgenerate
    
endmodule