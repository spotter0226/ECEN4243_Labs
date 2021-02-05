module regfile__tb();

    logic        clk; 
    logic        we3; 
    logic [4:0]  ra1, ra2, wa3;
    logic [31:0] wd3; 
    logic [31:0] rd1, rd2;

    integer handle3;
    integer desc3;


regfile dut (.clk (clk), 
             .we3(we3), 
             .ra1(ra1), .ra2(ra2), .wa3(wa3), 
             .wd3(wd3), 
             .rd1(rd1), .rd2(rd2));


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
        #5 $fdisplay(desc3, "%b %b || %b", ra1, ra2, rd1);
      end

initial
     begin
	#0 we3 = 1'b0;

	#10 we3 = 1'b1;

	#5 ra1 = 1'b1;
	#0 wa3 = 1'b1;
	#0 wd3 = 2'b10;

	#10 ra1 = 1'b0;

	#5 ra1 = 1'b1;
	#0 ra2 = 1'b1;
	#0 wd3 = 3'b111;
	
	#20 ra2 = 1'b0;
	#0 ra1 = 1'b0;

	#10 we3 = 1'b0;

     end
endmodule
