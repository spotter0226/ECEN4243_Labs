//This is to test the code before putting it into control.sv without losing the fsm example
//Will need work, esp before putting into control.sv
module fsmcontrol(Strobe, DRW, M, V, clk, reset, DReady, W, MStrobe, MRW, RSel, WSel);
	input Strobe;
    input DRW;
    input M;
    input V;
    input clk;
    input reset;

    output DReady;
    output W;
    output MStrobe;
    output MRW;
    output RSel;
    output WSel;

   parameter [3:0] S0 = 4'd0,
     S1 = 4'd1,
     S2 = 4'd2,
     S3 = 4'd3,
     S4 = 4'd4,
     S5 = 4'd5,
     S6 = 4'd6,
     S7 = 4'd7,
     Idle = 4'd8;

   logic [3:0] CURRENT_STATE;
   logic [3:0] NEXT_STATE;

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
	    if (X == 1'b0)
	      begin
		 Z = 1'b0;
		 NEXT_STATE <=  Idle;
	      end else begin
		 Z = 1'b0;
		 NEXT_STATE <=  S0;
	      end
	  
	  S0:	
	    if (X == 1'b0)
	      begin
		 Z = 1'b0;
		 NEXT_STATE <=  S1;
	      end else begin
		 Z = 1'b0;
		 NEXT_STATE <=  S0;
	      end	
	  
	  S1:	
	    if (X == 1'b0)
	      begin
		 Z = 1'b0;
		 NEXT_STATE <=  Idle;
	      end else begin
		 Z = 1'b0;
		 NEXT_STATE <=  S2;
	      end	

	  S2:	
	    if (X == 1'b0)
	      begin
		 Z = 1'b0;
		 NEXT_STATE <=  S1;
	      end else begin
		 Z = 1'b0;
		 NEXT_STATE <=  S3;
	      end	

	  S3:	
	    if (X == 1'b0)
	      begin
		 Z = 1'b0;
		 NEXT_STATE <=  S4;
	      end else begin
		 Z = 1'b0;
		 NEXT_STATE <=  S0;
	      end	

	  S4:	
	    if (X == 1'b0)
	      begin
		 Z = 1'b0;
		 NEXT_STATE <=  Idle;
	      end else begin
		 Z = 1'b0;
		 NEXT_STATE <=  S5;
	      end	

	  S5:	
	    if (X == 1'b0)
	      begin
		 Z = 1'b0;
		 NEXT_STATE <=  S1;
	      end else begin
		 Z = 1'b0;
		 NEXT_STATE <=  S6;
	      end	

	  S6:	
	    if (X == 1'b0)
	      begin
		 Z = 1'b0;
		 NEXT_STATE <=  S7;
	      end else begin
		 Z = 1'b0;
		 NEXT_STATE <=  S0;
	      end	

	  S7:	
	    if (X == 1'b0)
	      begin
		 Z = 1'b1;
		 NEXT_STATE <=  Idle;
	      end else begin
		 Z = 1'b1;
		 NEXT_STATE <=  S5;
	      end
	  
	  default: 
	    begin
	       NEXT_STATE <=  S0;
	       Z = 1'b0;	     
	    end
	  
	endcase // case (CURRENT_STATE)	
     end // always @ (CURRENT_STATE or X)   

endmodule // fsm
