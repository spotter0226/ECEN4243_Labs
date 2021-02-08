module regfile(input  logic        clk, 
               input  logic        we3, 
               input  logic [4:0]  ra1, ra2, wa3, 
               input  logic [31:0] wd3, 
               output logic [31:0] rd1, rd2);

  logic [31:0]     rf[31:0];

	always_comb // Combinational statement 
	begin
		if (ra1 == 0) // Assigning ports
			rd1 <= 0;
		else	
			rd1 <= rf[ra1];
		if (ra2 == 0) 
			rd2 <= 0;	
		else	
			rd2 <= rf[ra2];
	
	end

	always @ (posedge clk) // Always do on the positive edge of the clock
	 begin
	if (we3 == 1) // If we3 is enabled then allow write
	     rf[wa3] <= wd3;
	 end 
	if (ra0 != 0) // register 0 hardwired to 0
		rf[ra0] <= 0;
	end
  	


endmodule // regfile
