// arm_single.sv
// David_Harris@hmc.edu and Sarah_Harris@hmc.edu 25 June 2013
// Single-cycle implementation of a subset of ARMv4
//
// This version has been modified by:
//     Dr. James Stine (james.stine@okstate.edu)
//     Alex Underwood  (alexander.underwood@okstate.edu)
// for formatting and additional functionality.

// 16 32-bit registers
// Data-processing instructions
//   ADD, SUB, AND, ORR
//   INSTR<cond><S> rd, rn, #immediate
//   INSTR<cond><S> rd, rn, rm
//    rd <- rn INSTR rm       if (S) Update Status Flags
//    rd <- rn INSTR immediate  if (S) Update Status Flags
//   Instr[31:28] = cond
//   Instr[27:26] = op = 00
//   Instr[25:20] = funct
//                  [25]:    1 for immediate, 0 for register
//                  [24:21]: 0100 (ADD) / 0010 (SUB) /
//                           0000 (AND) / 1100 (ORR)
//                  [20]:    S (1 = update CPSR status Flags)
//   Instr[19:16] = rn
//   Instr[15:12] = rd
//   Instr[11:8]  = 0000
//   Instr[7:0]   = imm8      (for #immediate type) / 
//                  {0000,rm} (for register type)
//   
// Load/Store instructions
//   LDR, STR
//   INSTR rd, [rn, #offset]
//    LDR: rd <- Mem[rn+offset]
//    STR: Mem[rn+offset] <- rd
//   Instr[31:28] = cond
//   Instr[27:26] = op = 01 
//   Instr[25:20] = funct
//                  [25]:    0 (A)
//                  [24:21]: 1100 (P/U/B/W)
//                  [20]:    L (1 for LDR, 0 for STR)
//   Instr[19:16] = rn
//   Instr[15:12] = rd
//   Instr[11:0]  = imm12 (zero extended)
//
// Branch instruction (PC <= PC + offset, PC holds 8 bytes past Branch Instr)
//   B
//   B target
//    PC <- PC + 8 + imm24 << 2
//   Instr[31:28] = cond
//   Instr[27:25] = op = 10
//   Instr[25:24] = funct
//                  [25]: 1 (Branch)
//                  [24]: 0 (link)
//   Instr[23:0]  = imm24 (sign extend, shift left 2)
//   Note: no Branch delay slot on ARM
//
// Other:
//   R15 reads as PC+8
//   Conditional Encoding
//    cond  Meaning                       Flag
//    0000  Equal                         Z = 1
//    0001  Not Equal                     Z = 0
//    0010  Carry Set                     C = 1
//    0011  Carry Clear                   C = 0
//    0100  Minus                         N = 1
//    0101  Plus                          N = 0
//    0110  Overflow                      V = 1
//    0111  No Overflow                   V = 0
//    1000  Unsigned Higher               C = 1 & Z = 0
//    1001  Unsigned Lower/Same           C = 0 | Z = 1
//    1010  Signed greater/equal          N = V
//    1011  Signed less                   N != V
//    1100  Signed greater                N = V & Z = 0
//    1101  Signed less/equal             N != V | Z = 1
//    1110  Always                        any

module arm (input  logic        clk, reset,
            output logic [31:0] PC,
            input  logic [31:0] Instr,
            output logic        MemWrite,
            output logic [31:0] ALUResult, WriteData,
            input  logic [31:0] ReadData,
            output logic        MemStrobe,
            input  logic        PCReady);
   
   logic [3:0] ALUFlags;
   logic       RegWrite, ALUSrc, MemtoReg, PCSrc;
   logic [2:0] RegSrc;   
   logic [1:0] ImmSrc;
   logic [4:0] ALUControl;
   
   controller c (.clk(clk),
                 .reset(reset),
                 .Instr(Instr[31:12]),
                 .ALUFlags(ALUFlags),
                 .RegSrc(RegSrc),
                 .RegWrite(RegWrite),
                 .ImmSrc(ImmSrc),
                 .ALUSrc(ALUSrc),
                 .ALUControl(ALUControl),
                 .MemWrite(MemWrite),
                 .MemtoReg(MemtoReg),
                 .PCSrc(PCSrc),
                 .MemStrobe(MemStrobe));
   datapath dp (.clk(clk),
                .reset(reset),
                .RegSrc(RegSrc),
                .RegWrite(RegWrite),
                .ImmSrc(ImmSrc),
                .ALUSrc(ALUSrc),
                .ALUControl(ALUControl),
                .MemtoReg(MemtoReg),
                .PCSrc(PCSrc),
                .ALUFlags(ALUFlags),
                .PC(PC),
                .Instr(Instr),
                .ALUResult(ALUResult),
                .WriteData(WriteData),
                .ReadData(ReadData),
                .PCReady(PCReady),
                .I(Instr[25]), //immediate
                .sh(Instr[6:5])); //shift instructions
   
endmodule // arm

module controller (input  logic         clk, reset,
                   input  logic [31:12] Instr,
                   input  logic [ 3:0]  ALUFlags,
                   output logic [ 2:0]  RegSrc,
                   output logic         RegWrite,
                   output logic [ 1:0]  ImmSrc,
                   output logic         ALUSrc, 
                   output logic [ 4:0]  ALUControl,
                   output logic         MemWrite, MemtoReg,
                   output logic         PCSrc,
                   output logic         MemStrobe);
   
   logic [1:0] FlagW;
   logic       PCS, RegW, MemW;
   
   decoder dec (.Op(Instr[27:26]),
                .Funct(Instr[25:20]),
                .Rd(Instr[15:12]),
                .FlagW(FlagW),
                .PCS(PCS),
                .RegW(RegW),
                .MemW(MemW),
                .MemtoReg(MemtoReg),
                .ALUSrc(ALUSrc),
                .ImmSrc(ImmSrc),
                .RegSrc(RegSrc),
                .ALUControl(ALUControl),
                .MemStrobe(MemStrobe));
   condlogic cl (.clk(clk),
                 .reset(reset),
                 .Cond(Instr[31:28]),
                 .ALUFlags(ALUFlags),
                 .FlagW(FlagW),
                 .PCS(PCS),
                 .RegW(RegW),
                 .MemW(MemW),
                 .PCSrc(PCSrc),
                 .RegWrite(RegWrite),
                 .MemWrite(MemWrite));
endmodule

module decoder (input  logic [1:0] Op,
                input  logic [5:0] Funct,
                input  logic [3:0] Rd,
                output logic [1:0] FlagW,
                output logic       PCS, RegW, MemW,
                output logic       MemtoReg, ALUSrc,
                output logic [1:0] ImmSrc, 
                output logic [4:0] ALUControl,
                output logic [2:0] RegSrc,
                output logic       MemStrobe);
   
   logic [11:0] controls;
   logic        Branch, ALUOp;

   // Main Decoder
   always_comb
     case(Op)
       // Data processing immediate
       2'b00: if (Funct[5]) controls = 12'b0000_0101_0010;
       // Data processing register
         else controls = 12'b0000_0001_0010;
       // LDR
       2'b01: if (Funct[0]) controls = 12'b0000_1111_0001;
       // STR
         else controls = 12'b0100_1110_1001;
       // BL
       2'b10: if (Funct[4]) controls = 12'b1011_0101_0100;
       // B
         else controls = 12'b0011_0100_0100;
       // Unimplemented
       default: controls = 12'bx;
     endcase // case (Op)

   assign {RegSrc, ImmSrc, ALUSrc, MemtoReg,
          RegW, MemW, Branch, ALUOp, MemStrobe} = controls;
      
   // ALU Decoder             
   always_comb
     if (ALUOp)
       begin                 // which DP Instr?
         case(Funct[4:1]) 
           4'b0100: ALUControl = 5'b00000; // ADD--
           4'b0010: ALUControl = 5'b00001; // SUB--
           4'b0101: ALUControl = 5'b01100; // ADC--
           4'b1010: ALUControl = 5'b00001; // CMP--
           4'b1011: ALUControl = 5'b00000; // CMN--
           4'b1101: ALUControl = 5'b10000; // MOV, LSL, LSR, ASR, ROR--
           //4'b1101: ALUControl = 5'b10001; // LSL
           //4'b1101: ALUControl = 5'b10010; // LSR
           //4'b1101: ALUControl = 5'b10011; // ASR
           //4'b1101: ALUControl = 5'b10100; // ROR
           4'b1111: ALUControl = 5'b00001; // MVN--

           4'b0000: ALUControl = 5'b00010; // AND--
           4'b1000: ALUControl = 5'b00010; // TST--

           4'b1100: ALUControl = 5'b00011; // ORR--

           4'b0001: ALUControl = 5'b00111; // EOR--
           4'b1001: ALUControl = 5'b00111; // TEQ--
           
           4'b0110: ALUControl = 5'b00101; // SBC--
           4'b0011: ALUControl = 5'b01101; // RSB--
           4'b0111: ALUControl = 5'b01000; // RSC--
           4'b1110: ALUControl = 5'b00110; // BIC--
           
           default: ALUControl = 5'bx;  // unimplemented
         endcase
         // update flags if S bit is set 
         // (C & V only updated for arith instructions)
         FlagW[1]      = Funct[0]; // FlagW[1] = S-bit
         // FlagW[0] = S-bit & (ADD | SUB)
         FlagW[0]      = Funct[0] & (ALUControl == 2'b00 | ALUControl == 2'b01);
       end
     else
       begin
         ALUControl = 5'b00000; // add for non-DP instructions
         FlagW      = 2'b00; // don't update Flags
       end
   
   // PC Logic
   assign PCS  = ((Rd == 4'b1111) & RegW) | Branch;
   
endmodule // decoder

module condlogic (input  logic       clk, reset,
                  input  logic [3:0] Cond,
                  input  logic [3:0] ALUFlags,
                  input  logic [1:0] FlagW,
                  input  logic       PCS, RegW, MemW,
                  output logic       PCSrc, RegWrite, MemWrite);
   
   logic [1:0] FlagWrite;
   logic [3:0] Flags;
   logic       CondEx;

   // Notice hard-coding of FFs to structurally model
   flopenr #(2) flagreg1 (.clk(clk),
                          .reset(reset),
                          .en(FlagWrite[1]),
                          .d(ALUFlags[3:2]),
                          .q(Flags[3:2]));
   flopenr #(2) flagreg0 (.clk(clk),
                          .reset(reset),
                          .en(FlagWrite[0]),
                          .d(ALUFlags[1:0]),
                          .q(Flags[1:0]));
   
   // write controls are conditional
   condcheck cc (.Cond(Cond),
                 .Flags(Flags),
                 .CondEx(CondEx));
   assign FlagWrite = FlagW & {2{CondEx}};
   assign RegWrite  = RegW  & CondEx;
   assign MemWrite  = MemW  & CondEx;
   assign PCSrc     = PCS   & CondEx;
   
endmodule // condlogic

module condcheck (input  logic [3:0] Cond,
                  input  logic [3:0] Flags,
                  output logic       CondEx);
   
   logic neg, zero, carry, overflow, ge;
   
   assign {neg, zero, carry, overflow} = Flags;
   assign ge = (neg == overflow);
   
   always_comb
     case(Cond)
       4'b0000: CondEx = zero;             // EQ
       4'b0001: CondEx = ~zero;            // NE
       4'b0010: CondEx = carry;            // CS
       4'b0011: CondEx = ~carry;           // CC
       4'b0100: CondEx = neg;              // MI
       4'b0101: CondEx = ~neg;             // PL
       4'b0110: CondEx = overflow;         // VS
       4'b0111: CondEx = ~overflow;        // VC
       4'b1000: CondEx = carry & ~zero;    // HI
       4'b1001: CondEx = ~(carry & ~zero); // LS
       4'b1010: CondEx = ge;               // GE
       4'b1011: CondEx = ~ge;              // LT
       4'b1100: CondEx = ~zero & ge;       // GT
       4'b1101: CondEx = ~(~zero & ge);    // LE
       4'b1110: CondEx = 1'b1;             // Always
       default: CondEx = 1'bx;             // undefined
     endcase // case (Cond)
   
endmodule // condcheck

module datapath (input  logic        clk, reset,
                 input  logic [ 2:0]  RegSrc,
                 input  logic        RegWrite,
                 input  logic [ 1:0]  ImmSrc,
                 input  logic        ALUSrc,
                 input  logic [ 4:0]  ALUControl,
                 input  logic        MemtoReg,
                 input  logic        PCSrc,
                 output logic [ 3:0]  ALUFlags,
                 output logic [31:0] PC,
                 input  logic [31:0] Instr,
                 output logic [31:0] ALUResult, WriteData,
                 input  logic [31:0] ReadData,
                 input  logic        PCReady,
                 input  logic        I, //immediate
                 input  logic [1:0]  sh); //shift instructions
   
   logic [31:0] PCNext, PCPlus4, PCPlus8;
   logic [31:0] ExtImm, SrcA, SrcB, Result;
   logic [ 3:0]  RA1, RA2, RA3;
   logic [31:0] RA4;   
   
   // next PC logic
   mux2 #(32)  pcmux (.d0(PCPlus4),
                      .d1(Result),
                      .s(PCSrc),
                      .y(PCNext));
   flopenr #(32) pcreg (.clk(clk),
                        .reset(reset),
                        .en(PCReady),
                        .d(PCNext),
                        .q(PC));
   adder #(32) pcadd1 (.a(PC),
                       .b(32'b100),
                       .y(PCPlus4));
   adder #(32) pcadd2 (.a(PCPlus4),
                       .b(32'b100),
                       .y(PCPlus8));

   // register file logic
   mux2 #(4)   ra1mux (.d0(Instr[19:16]),
                       .d1(4'b1111),
                       .s(RegSrc[0]),
                       .y(RA1));
   mux2 #(4)   ra2mux (.d0(Instr[3:0]),
                       .d1(Instr[15:12]),
                       .s(RegSrc[1]),
                       .y(RA2));
   mux2 #(4)   ra3mux (.d0(Instr[15:12]),
                       .d1(4'hE),
                       .s(RegSrc[2]),
                       .y(RA3));
   mux2 #(32)  ra4mux (.d0(Result),
                       .d1(PCPlus4),
                       .s(RegSrc[2]),
                       .y(RA4));
   
   regfile     rf (.clk(clk),
                   .we3(RegWrite),
                   .ra1(RA1),
                   .ra2(RA2),
                   .wa3(RA3),
                   .wd3(RA4),
                   .r15(PCPlus8),
                   .rd1(SrcA),
                   .rd2(WriteData)); 
   mux2 #(32)  resmux (.d0(ALUResult),
                       .d1(ReadData),
                       .s(MemtoReg),
                       .y(Result));
   extend      ext (.Instr(Instr[23:0]),
                    .ImmSrc(ImmSrc),
                    .ExtImm(ExtImm));

   // ALU logic
   mux2 #(32)  srcbmux (.d0(WriteData),
                        .d1(ExtImm),
                        .s(ALUSrc),
                        .y(SrcB));
   alu         alu (.a(SrcA),
                    .b(SrcB),
                    .ALUControl(ALUControl),
                    .I(Instr[25]), //immediate
                    .sh(Instr[6:5]), //shift
                    .Result(ALUResult),
                    .ALUFlags(ALUFlags));
endmodule // datapath

module regfile (input  logic        clk, 
                input  logic        we3, 
                input  logic [ 3:0] ra1, ra2, wa3, 
                input  logic [31:0] wd3, r15,
                output logic [31:0] rd1, rd2);
   
   logic [31:0] rf[14:0];
   
   // three ported register file
   // read two ports combinationally
   // write third port on rising edge of clock
   // register 15 reads PC+8 instead
   
   always_ff @(posedge clk)
     if (we3) rf[wa3] <= wd3;   

   assign rd1 = (ra1 == 4'b1111) ? r15 : rf[ra1];
   assign rd2 = (ra2 == 4'b1111) ? r15 : rf[ra2];
   
endmodule // regfile

module extend (input  logic [23:0] Instr,
               input  logic [ 1:0] ImmSrc,
               output logic [31:0] ExtImm);
   
   always_comb
     case(ImmSrc) 
       // 8-bit unsigned immediate
       2'b00:   ExtImm = {24'b0, Instr[7:0]};  
       // 12-bit unsigned immediate 
       2'b01:   ExtImm = {20'b0, Instr[11:0]}; 
       // 24-bit two's complement shifted branch 
       2'b10:   ExtImm = {{6{Instr[23]}}, Instr[23:0], 2'b00}; 
       default: ExtImm = 32'bx; // undefined
     endcase // case (ImmSrc)
   
endmodule // extend

module adder #(parameter WIDTH=8)
   (input  logic [WIDTH-1:0] a, b,
    output logic [WIDTH-1:0] y);
   
   assign y = a + b;
   
endmodule // adder

module flopenr #(parameter WIDTH = 8)
   (input  logic             clk, reset, en,
    input  logic [WIDTH-1:0] d, 
    output logic [WIDTH-1:0] q);

   always_ff @(posedge clk, posedge reset)
     if (reset)   q <= 0;
     else if (en) q <= d;
   
endmodule // flopenr

module flopr #(parameter WIDTH = 8)
   (input  logic             clk, reset,
    input  logic [WIDTH-1:0] d, 
    output logic [WIDTH-1:0] q);

   // Reset has start of .text
   always_ff @(posedge clk, posedge reset)
     if (reset) q <= 0;
     else       q <= d;
   
endmodule // flopr

module mux2 #(parameter WIDTH = 8)
   (input  logic [WIDTH-1:0] d0, d1, 
    input  logic             s, 
    output logic [WIDTH-1:0] y);

   assign y = s ? d1 : d0;
   
endmodule // mux2

module alu (input  logic [31:0] a, b,
            input  logic [ 4:0] ALUControl,
            input  logic I, //immediate
            input  logic [1:0] sh, //shift 
            output logic [31:0] Result,
            output logic [ 3:0] ALUFlags);
   
   logic        neg, zero, carry, overflow;
   logic [31:0] condinvb;
   logic [32:0] sum;
   
   assign condinvb = ALUControl[0] ? ~b : b;
   assign sum = a + condinvb + ALUControl[0];

   always_comb
    casex (ALUControl[4:0])
       5'b0000?:  Result = sum; // ADD Rn + Src2, SUB Rn - Src2, CMP, CMN, MOV, MVN
       
      /*LOGIC NOT RIGHT
      //5'b0_0001:  Result = a - b; // SUB Rn - Src2
      //5'b0_0010:  Result = a && b; // AND Rn & Src2
      //5'b0_0011:  Result = a || b; // ORR Rn | Src2
      //5'b0_0100:  Result = a + b + carry; // ADC Rn + Src2 + C
      //5'b0_0101:  Result = a >>> b; // ASR Rm >>> Src2
      //5'b0_0110:  Result = a && ~b; // BIC Rn & ~Src2
      //5'b0_0111:  Result = ; // CMN Rn + Src2 (compare negative and set flags)
      //5'b0_1000:  Result = ; // CMP Rn - Src2 (set flags based on this)
      //5'b0_1001:  Result = a ^ b; // EOR Rn ^ Src2
      //5'b0_1010:  Result = ; // LSL Rm << Src2
      //5'b0_1011:  Result = ; // LSR Rm >> Src2
      //5'b0_1100:  Result = b; // MOV Rd = Src2
      //5'b0_1101:  Result = ~a; // MVN Rd = ~Rn
      //5'b0_1110:  Result = ; // ROR Rn ror Src2
      //5'b0_1111:  Result = a - b - ~carry; // SBC Rn - Src2 - ~C
       //5'b1_0000:  Result = ; // TEQ Rn ^ Src2 (test that, then set flags)
       //5'b1_0001:  Result = ; // TST Rn & Src2 (test, then set flags)*/

       5'b10000:
       begin
         if (I == 1) //MOV
         {
           Result = b;
         }
         else
           begin
             casex (sh)
              2'b00: Result = a << b; //LSL
              2'b01: Result = a >> b; //LSR
              2'b10: Result = a >>> b; //ASR
              2'b11: Result = a <<< 2 * b; //ROR
             endcase
           end
       end
       5'b00010: Result = a & b; // AND, TST
       5'b00011: Result = a | b; // ORR
       5'b00111: Result = a ^ b; // EOR, TEQ

       5'b01100: Result = sum + carry; // ADC
       5'b00101: Result = sum - ~carry; // SBC
       5'b01000: Result = b - a - ~carry; // RSC
       5'b01101: Result = b - a; // RSB
       5'b00110: Result = a & ~b; // BIC

       default: Result = 32'bx;
     endcase

always_comb 
  if(ALUControl[4:0] = 5'b10000)
    begin
      neg      = Result[31];
      zero     = (Result == 32'b0);
      carry    = (ALUControl[1] == 1'b0) & sum[32];
      overflow = (ALUControl[1] == 1'b0) & 
                     ~(a[31] ^ b[31] ^ ALUControl[0]) & 
                     (a[31] ^ sum[31]); 
      ALUFlags = {neg, zero, carry, overflow};
    end
   
endmodule // alu
