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
 always @(posedge clk)
     begin
         if (reset == 1'b1)	
            CURRENT_STATE <=  Idle;
         else
            CURRENT_STATE <=  NEXT_STATE;
     end

   always @(CtrSig or Strobe or M or V or DRW or CURRENT_STATE)
   begin
      case(CURRENT_STATE)
         Idle:	
            if (Strobe == 1'b0) // idle
               begin
                  OutputLogic = 8'b10000000;
                  NEXT_STATE <=  Idle;
               end 
            else if (Strobe == 1'b1 && DRW == 1'b1) // write
               begin
                  OutputLogic = 8'b10000000;
                  NEXT_STATE <=  Write;
               end
            else // read
               begin
                  OutputLogic = 8'b10000000;
                  NEXT_STATE <=  Read;
               end

         Read:	
            if (M == 1'b0 && V == 1'b0)
               begin
                  OutputLogic = 8'b11000000;
                  NEXT_STATE <=  ReadMiss;
               end 
            else if ( M == 1'b0 && V == 1'b1)
               begin
                  OutputLogic = 8'b11000000;
                  NEXT_STATE <=  ReadMiss;
               end
            else if ( M == 1'b1 && V == 1'b0)
               begin
                  OutputLogic = 8'b11000000;
                  NEXT_STATE <=  ReadMiss;
               end
            else
               begin
                  OutputLogic = 8'b00110110;
                  NEXT_STATE <= Idle;
               end		

         ReadMiss:	
            if (CtrSig == 1'bx)
               begin
                  OutputLogic = 8'b10001000;
                  NEXT_STATE <=  ReadMem;
               end 
            else if ( CtrSig == 1'b1)
               begin
                  OutputLogic = 8'b00000000;
                  NEXT_STATE <=  ReadData;
               end
            else 
               begin
                  OutputLogic = 8'b00000000;
                  NEXT_STATE <=  ReadMem;
               end	

         ReadData:	
            begin
               OutputLogic = 8'b00110110;
               NEXT_STATE = Idle;
            end	

         Write:	
            if (M == 1'b0 && V == 1'b0)
               begin
                  OutputLogic = 8'b10000000;
                  NEXT_STATE <=  WriteMiss;
               end 
            else if ( M == 1'b0 && V == 1'b1)
               begin
                  OutputLogic = 8'b10000000;
                  NEXT_STATE <=  WriteMiss;
               end
            else if ( M == 1'b0 && V == 1'b1)
               begin
                  OutputLogic = 8'b10000000;
                  NEXT_STATE <=  WriteMiss;
               end
            else 
               begin
                  OutputLogic = 8'b10000000;
                  NEXT_STATE <=  WriteHit;
               end	

         WriteMiss:	
            begin
               OutputLogic = 8'b10001100;
               NEXT_STATE <= WriteMem;
            end

         WriteHit:	
            begin
               OutputLogic = 8'b10001100;
               NEXT_STATE <= WriteMem;
            end	

         WriteMem:	
            if (CtrSig == 1'b1)
               begin
                  OutputLogic = 8'b00000100;
                  NEXT_STATE <=  WriteData;
               end 
            else 
               begin
                  OutputLogic = 8'b00000100;
                  NEXT_STATE <=  WriteMem;
               end
         
         WriteData:	
            begin
               OutputLogic = 8'b00110101;
               NEXT_STATE <= Idle;
            end
      
         default: 
            begin
               NEXT_STATE <=  Idle;
               OutputLogic = 8'b10000000;	     
            end
	  
	   endcase // case (CURRENT_STATE)	
   end // always @ (CURRENT_STATE or X)   


endmodule /* Control */
