module regfile(input  logic        clk, 
               input  logic        we3, 
               input  logic [4:0]  ra1, ra2, wa3, 
               input  logic [31:0] wd3, 
               output logic [31:0] rd1, rd2);

  logic [31:0]     rf[31:0];

	always_comb
	begin
		if (ra1 == 0) 
			rd1 <= 0;
		else	
			rd1 <= rf[ra1];
		if (ra2 == 0) 
			rd2 <= 0;	
		else	
			rd2 <= rf[ra2];
	
	end

	always @ (posedge clk)
	 begin
	if (we3 == 1)
	     rf[wa3] <= wd3;
	 end 
	
	
	
	
	
  // three ported register file

  // read two ports combinationally
  // write third port on rising edge of clock
  // register 0 hardwired to 0


endmodule // regfile
