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



module  tree_node #(parameter int rows = 8,int width = 10, int struct_array [0:rows-1][0:width-1] = '{{0,0,15,14,1,1,3,2,-1,-1},{1,0,1,0,0,3,1,4,2,5},{2,0,3,0,8,6,1,7,-1,-1}, {3,1,0,-1,-1,-1,-1,-1,-1,-1}, {4,1,1,-1,-1,-1,-1,-1,-1,-1}, {5,1,2,-1,-1,-1,-1,-1,-1,-1}, {6,1,24,-1,-1,-1,-1,-1,-1,-1}, {7,1,3,-1,-1,-1,-1,-1,-1,-1}}, int splits [0:rows-1] = '{2,3,2,1,1,1,1,1}, int layer = 0, int output_width = 16, int input_width = 16)

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
                tree_node #(.rows(rows), .width(width), .struct_array(struct_array), .splits(splits), .output_width(output_width), .input_width(input_width), .layer(struct_array[layer][5+2*i])) node_inst 
                (
                .inputs(inputs),
                .result(outs[i])
                );
            end                    
        end
    endgenerate
    
endmodule