/*
 * Top level module for the arm_pipelined processor simulation.
 *
 * Modeled to be like the version for the FPGA so the arm module can be drop-in
 * swapped into the Vivado project and things should "just werk".
 */

module top (input  logic        clk, reset, 
            output logic [31:0] WriteData, DataAdr, 
            output logic        MemWrite);

   logic [31:0] PC, Instr, ReadData;
   logic [31:0] MAddress, MDataOut, MDataIn;
   logic        Ready, PStrobe;
   logic        MRW, MStrobe;
   
   // instantiate processor and memories
   arm arm (.clk(clk),
            .reset(reset),
            .PCF(PC),
            .InstrF(Instr),
            .MemWriteM(MemWrite),
            .ALUOutM(DataAdr), 
            .WriteDataM(WriteData),
            .ReadDataM(ReadData),
            .StrobeM(PStrobe),
            .Ready(Ready));

   cache cache(.PStrobe(PStrobe),
               .PRW(MemWrite),
               .PAddress(DataAdr),
               .PDataIn(WriteData),
               .PReady(Ready),
               .PDataOut(ReadData),
               .MStrobe(MStrobe),
               .MRW(MRW),
               .MAddress(MAddress),
               .MDataOut(MDataOut),
               .MDataIn(MDataIn),
               .Reset(reset),
               .clk(clk));
   imem imem (.mem_addr(PC),
              .mem_out(Instr));
   dmem_wait dmem (.mem_out(MDataOut),
                   .r_w(MRW),
                   .clk(~clk),
                   .mem_addr(MAddress),
                   .mem_data(MDataIn),
                   .mstrobe(MStrobe));
   
endmodule // top
