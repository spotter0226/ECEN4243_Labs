module CacheControl(input Strobe,
                    input DRW,
                    input M,
                    input V,
                    input clk,
                    input reset,
                    output DReady,
                    output W,
                    output MStrobe,
                    output MRW,
                    output RSel,
                    output WSel);

   logic [7:0] WSCLoadVal;   
   logic       CtrSig;
   logic       ReadyEn;
   logic       LdCtr;
   logic       Ready;   
   logic [7:0]  OutputLogic;
  
   assign DReady = (ReadyEn & M && V && ~DRW) || Ready;
   assign {LdCtr, ReadyEn, Ready, W, MStrobe, MRW, RSel, WSel} = OutputLogic;

   parameter [3:0] Idle      = 4'b0000,
                   Read      = 4'b0001,
                   ReadMiss  = 4'b0010,
                   ReadMem   = 4'b0011,
                   ReadData  = 4'b0100,
                   Write     = 4'b0101,
                   WriteHit  = 4'b0110,
                   WriteMiss = 4'b0111,
                   WriteMem  = 4'b1000,
                   WriteData = 4'b1001; 

   logic [3:0] CURRENT_STATE;
   logic [3:0] NEXT_STATE;

   // wait state = 100 cycles
   assign WSCLoadVal = 8'h4;   
   wait_state WaitStateCtr (LdCtr, WSCLoadVal, CtrSig, clk);

   // Insert FSM Here
   /*
     always @(posedge clk)
     begin
	if (reset == 1'b1)	
	  CURRENT_STATE <=  Idle;
	else
	  CURRENT_STATE <=  NEXT_STATE;
     end
   
   always @(CURRENT_STATE or X)
     begin
 	case(CURRENT_STATE)
      Idle:
         if (Strobe == 0) 
         begin
            LdCtr   = 1;
            ReadyEn = 0; 
            Ready   = 0;
            W       = 0;
            MStrobe = 0;
            MRW     = 0;
            RSel    = 0;
            WSel    = 0;
            NEXT_STATE <= Idle;
         end else
         begin
            if (DRW == 1)
            begin
               LdCtr   = 1;
               ReadyEn = 0; 
               Ready   = 0;
               W       = 0;
               MStrobe = 0;
               MRW     = 0;
               RSel    = 0;
               WSel    = 0;
               NEXT_STATE <= Write;
            end
            else begin
               LdCtr   = 1;
               ReadyEn = 0; 
               Ready   = 0;
               W       = 0;
               MStrobe = 0;
               MRW     = 0;
               RSel    = 0;
               WSel    = 0;
               NEXT_STATE <= Read;
            end
         end
      Read:
         


   */


endmodule /* Control */


