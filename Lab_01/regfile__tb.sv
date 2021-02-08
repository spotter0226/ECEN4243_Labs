module regfile__tb();

    logic        clk; 
    logic        we3; 
    logic [4:0]  ra1, ra2, wa3;
    logic [31:0] wd3; 
    logic [31:0] rd1, rd2;

    integer handle3;
    integer desc3;

// Section to init register variables
regfile dut (.clk (clk), 
             .we3(we3), 
             .ra1(ra1), .ra2(ra2), .wa3(wa3), 
             .wd3(wd3), 
             .rd1(rd1), .rd2(rd2));

    // Section to init the clk
    initial 
      begin  
        clk = 1'b1;
        forever #5 clk = ~clk;
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
        #5 $fdisplay(desc3, "%b %b || %b", rd1, rd2, wd3);
      end

initial
     begin
	#0 we3 = 1'b0; // Sets that the write enable is zero

	#10 we3 = 1'b1; // Sets that the write enable is one

	#5 ra1 = 1'b1; // Sets that the source read is one
	#0 wa3 = 1'b1; // Sets that the write source is one 
	#0 wd3 = 2'b10; // Sets the write data register to a 2 bit binary value for "2"

	#10 ra1 = 1'b0; // Sets the read source register to 0

	#5 ra1 = 1'b1; // Sets the read source #1 register to 1
	#0 ra2 = 1'b1; // Sets the read source #2 register to 1
	#0 wd3 = 3'b111; // Sets the write data register to 3-bit binary value for "7"
	
	#20 ra2 = 1'b0; // Sets the read source #1 register to 0
	#0 ra1 = 1'b0; // Sets the read source #1 register to 0

	#10 we3 = 1'b0; // Sets that the write enable is zero

     end
endmodule
