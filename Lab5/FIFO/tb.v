//
// File name : tb.v
// Title     : stimulus
// project   : ECEN3233
// Library   : test
// Author(s) : James E. Stine, Jr.
// Purpose   : definition of modules for testbench 
// notes :   
//
// Copyright Oklahoma State University
//

// Top level stimulus module

module stimulus;

   reg Clk;  // Always declared so can simulate based on clock

   // Declare variables for stimulating input
   wire [31:0] rdata;   
   wire        wfull;
   wire        rempty;
   
   reg [31:0]  wdata;   
   reg 	       winc, wclk, wrst_n;   
   reg 	       rinc, rclk, rrst_n;
   	
   integer     handle3;
   integer     desc3;   
   
   // Instantiate the design block counter
   fifo1 #(32,4) (rdata, wfull, rempty,
		  wdata, winc, Clk, wrst_n,
		  rinc, Clk, rrst_n);

   // Setup the clock to toggle every 1 time units 
   initial 
     begin	
	Clk = 1'b1;
	forever #5 Clk = ~Clk;
     end

   initial
     begin
	// Gives output file name
	handle3 = $fopen("test.out");
	// Tells when to finish simulation
	#500 $finish;		
     end

   always 
     begin
	desc3 = handle3;
	#5 $fdisplay(desc3, "%h %b %b || %h %b %b", 
		     wdata, winc, rinc, rdata, wfull, rempty);
     end

   // Stimulate the Input Signals
   initial
     begin
	// Add your test vectors here	
	#0  rrst_n = 1'b0;
	#43 rrstn_n = 1'b1;
	
     end

endmodule // stimulus





