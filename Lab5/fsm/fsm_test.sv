module stimulus;

   logic   Clk;     
   
   // Declare variables for stimulating input
   logic   X;
   logic   reset;
   
   // All outputs declared with wire   
   logic   Z;
   
   integer handle3;
   integer desc3;   
   
   fsm dut (Z, X, Clk, reset);
   
   // Setup the clock to toggle every 1 time units 
   initial 
     begin	
	Clk = 1'b1;
	forever #5 Clk = ~Clk;
     end

   initial
     begin
	#200 $finish;		
     end


   // Stimulate the Input Signals
   initial
     begin
	// Add your test vectors here
	#10 reset=1;
	#0 X=0;
	#10 reset=0;
	#0 X=1;
	#10 X=0;

	#10 X=1;
	#10 X=0;
	#10 X=1;
	#10 X=1;
	#10 X=0;
	#10 X=1;
	#10 X=1;
	#10 X=0;
	
	#10 X=1;
	#10 X=1;
	#10 X=0;

	#10 X=1;
	#10 X=1;
	#10 X=0;	
     end

endmodule // stimulus





