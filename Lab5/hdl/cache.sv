//------------------------------------------------
// cache.v
// James E. Stine
// April 1, 2018 (no joke)
// Oklahoma State University
// ECEN 4243
// Direct-mapped cache
//------------------------------------------------

module cache (input  logic        PStrobe,
              input  logic        PRW,
              input  logic [31:0] PAddress,
              input  logic [31:0] PDataIn,
              output logic        PReady,
              output logic [31:0] PDataOut,
              output logic        MStrobe,
              output logic        MRW,
              output logic [31:0] MAddress,
              input  logic [31:0] MDataOut,
              output logic [31:0] MDataIn,
              input  logic        Reset,
              input  logic        clk);

   logic [31:0] DDataOut;
   logic [17:0] TagRamTag;
   logic [31:0] DataRamDataOut;

   // Output Address/Data for Main Memory
   assign MAddress = PAddress;
   assign MDataIn = PDataIn;   

   // Tag RAM
   tag_ram TagRam (.mem_out(TagRamTag),
                   .r_w(Write),
                   .clk(~clk),
                   .mem_addr(PAddress[13:2]),
                   .mem_data(PAddress[31:14]));
   // Valid (V) RAM
   valid_ram ValidRam (.mem_out(Valid),
                       .r_w(Write),
                       .clk(~clk),
                       .mem_addr(PAddress[13:2]),
                       .mem_data(1'b1));
   mux2 #(32) m1 (.d0(DataRamDataOut),
                  .d1(MDataOut),
                  .s(RSel),
                  .y(PDataOut));   
   mux2 #(32) m2 (.d0(PDataIn),
                  .d1(MDataOut),
                  .s(WSel),
                  .y(DDataOut));
   // Data RAM for direct-mapped cache (4KB)
   data_ram DataRam (.mem_out(DataRamDataOut),
                     .r_w(Write),
                     .clk(~clk),
                     .mem_addr(PAddress[13:2]),
                     .mem_data(DDataOut));
   // check cache for Match
   Comparator Comparator (.Tag1(PAddress[31:14]),
                          .Tag2(TagRamTag),
                          .Match(Match));
   // brains of cache (FSM)
   CacheControl CacheControl (.Strobe(PStrobe),
                              .DRW(PRW),
                              .M(Match),
                              .V(Valid),
                              .DReady(PReady),
                              .W(Write),
                              .MStrobe(MStrobe),
                              .MRW(MRW),
                              .RSel(RSel),
                              .WSel(WSel),
                              .clk(clk),
                              .reset(Reset));
   

endmodule // Cache

