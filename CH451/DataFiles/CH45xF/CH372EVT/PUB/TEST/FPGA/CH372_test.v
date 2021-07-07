///////////////////////////////////////////////////////////
//����ƽ̨��ALTERA Cyclone EP1C12Q240C6
//����Ƶ�ʣ�12MHz
//����оƬ��CH372B
//������ڣ�2013/10/01
//ʵ�ֹ��ܣ�
//��1��	ʹ��FPGA��Ӳ����������ʵ��CH372��TEST����
//��2��	ʹ��оƬ�Ĳ��нӿڣ��ڲ��̼�ģʽ��ʵ��оƬ�ĳ�ʼ���Ͷ˵�2�����շ�
//��3��	���������ֽڳ���Ϊ64��FPGA�������ݽ��ղ������ݰ�λȡ���󷵻ء�
///////////////////////////////////////////////////////////
module CH372_test(
			pin_data,			//����CH372_D0-D7
			pin_int_n,		//����CH372_INT#
			pin_cs_n,			//����CH372_PCS#����ֻ�ǵ�һоƬ����ֱ�ӽӵأ�����Ƭѡ�ɸ���ʵ�������������
			pin_a0,				//����CH372_A0
			pin_rd,				//����CH372_RD#
			pin_wr,				//����CH372_WR#
			pin_clk,			//12M����Ϊ���������ֲ�������������ݼ��ʱ��ȷ��reg_delay_cnt��reg_clk_cnt_init��reg_clk_cnt_int����
			pin_rst,			//FPGA��λ��
			pin_err				//FPGAָʾ�ƣ���Ӳ������ʱΪ1����FPGA��led�Ƶ͵�ƽ�����õ��޴�ʱ����
			);

//-------------port------------------------
input			pin_clk;									
input			pin_rst;
input			pin_int_n;
inout	[7:0]	pin_data;
output			pin_cs_n;
output			pin_a0;
output			pin_rd;
output			pin_wr;
output			pin_err;

//------------output type------------------

reg				reg_err;
reg		[7:0]	buffer;
reg				a0;
reg				rd;
reg				wr;

reg				dat_oe;
reg				reg_rd_oe;
reg				reg_wr_oe;

assign pin_data	=	dat_oe?buffer:8'hzz;
assign pin_cs_n	=	1'b0;									//����Ĭ����Ч���ɸ���ʵ������޸�
assign pin_a0	=	a0;										//Ĭ�� a0  ���Ϊ��
assign pin_rd	=	reg_rd_oe?rd:1'b1;						//Ĭ�� rd# ���Ϊ��
assign pin_wr	=	reg_wr_oe?wr:1'b1;						//Ĭ�� wr# ���Ϊ��
assign pin_err	= 	reg_err;								

//-----------internal reg------------------
reg		[1:0]	reg_state_main;								//��ӦC��main����
reg		[1:0]	next_state_main;

reg		[2:0]	reg_state_init;								//��ӦC�г�ʼ��init����
reg		[2:0]	next_state_init;

reg		[2:0]	reg_state_int;								//��ӦC���ж�int����
reg		[2:0]	next_state_int;

reg		[19:0]	reg_delay_cnt;
reg				reg_init_end;
reg				reg_int_end;
reg				reg_int_flag;
reg				reg_rst;
reg		[7:0]	reg_q;
reg				reg_init_start;
reg				reg_int_start;

reg		[7:0]	reg_buffer [0:63];
reg		[7:0]	reg_dat_len;
reg		[6:0]	reg_dat_cnt;
reg		[8:0]	reg_clk_cnt_init;
reg		[5:0]	reg_clk_cnt_int;
reg				reg_usb_int_ep2_out;
reg				reg_get_len;
reg				reg_init_state_end;
reg				reg_int_state_end;
reg				reg_delay_1clk;


//-------------Internal Constant-------------
parameter	DELAY			=  2'b00;
parameter	INIT			=  2'b01;						//��ʼ��
parameter	WAIT_INT		=  2'b11;						//�ȴ��ж�
parameter	INT				=  2'b10;						//ִ���ж�
//
parameter	INIT_IDLE		=  3'b000;
parameter	TX_C15			=  3'b001;						//��������15H
parameter	TX_C02			=  3'b011;						//��������02H
parameter	WAIT_20u		=  3'b010;						//�ȴ�20us
parameter	RX_C51			=  3'b110;						//�����Ƿ����óɹ�
//
parameter	INT_IDLE		=  3'b110;
parameter	TX_C22			=  3'b010;						//��������22H
parameter	RX_INT_S		=  3'b011;						//�����ж�״̬
parameter	TX_C28			=  3'b001;						//��������28H
parameter	RX_DAT			=  3'b000;						//�������ݳ��Ⱥ�ֵ
parameter	TX_C2B			=  3'b100;						//��������2BH
parameter	TX_DAT			=  3'b101;						//�������ݳ��Ⱥ�ֵ
parameter	TX_C23			=  3'b111;						//��������23H

//----------------syn-----------
always @ (posedge pin_clk or negedge pin_rst)
begin
	if(!pin_rst) begin
		reg_rst <=1'b0;
		reg_q <= 8'h01;
	end
	else begin
		reg_rst <= reg_q[7];
		if (reg_q[6:0] == 7'b0000000)
			reg_q <= 8'h80;
		else	
			reg_q <= reg_q +8'h01;
	end
end

//---------------main FSM------------
always @ (reg_state_main or reg_delay_cnt or reg_init_end or reg_int_end or pin_int_n or reg_err or reg_int_flag or reg_delay_1clk)
begin
	next_state_main  = 2'b00;   
	case(reg_state_main)
	
	DELAY: begin
		if ((reg_delay_cnt == 20'hfffff) & (!reg_err)) 
			next_state_main = INIT;
		else 
			next_state_main = DELAY;
	end
  	INIT: begin
    	if(reg_err & (reg_init_end))
			next_state_main = DELAY;
    	else if (reg_init_end) 
			next_state_main = WAIT_INT;
		else
			next_state_main = INIT;
  	end

  	WAIT_INT: begin
		if (!pin_int_n & reg_delay_1clk)
			next_state_main = INT;  
    	else 
			next_state_main = WAIT_INT;
  	end
  	
  	INT: begin
		if (reg_int_end == reg_int_flag)
			next_state_main = WAIT_INT;  
    	else 
			next_state_main = INT;
  	end
  	
	default: next_state_main = DELAY;  
	endcase
end

//--------------main Seq logic------------
always @ (posedge pin_clk or negedge reg_rst)
begin
	if (!reg_rst) begin
    	reg_state_main  <=  DELAY;
 	end 
 	else begin
    	reg_state_main  <=  next_state_main;
	end
end

//---------------main FSM_OUT----------------
always @ (posedge pin_clk or negedge reg_rst)
begin 
	if (!reg_rst) begin 
		reg_delay_cnt	<= 20'h0_0000;
		reg_init_start	<= 1'b0;
		reg_int_flag	<= 1'b0;
		reg_int_start	<= 1'b0;
		reg_delay_1clk <= 1'b0;
	end 
	else begin
		case(reg_state_main)
 		
 		DELAY: begin
    		reg_delay_cnt <= reg_delay_cnt + 20'h0_0001; 						//��ʱ50ms
		end
 		
 		INIT:  begin
 			reg_delay_cnt	<= 20'h0_0000;   
     		reg_init_start	<= 1'b1; 
 		end   
 
 		WAIT_INT: begin   
			if(!reg_delay_1clk)begin											//only do once when waiting int
				reg_init_start	<= 1'b0;
				reg_int_flag	<= ~reg_int_flag;								//�жϽ�����־λ
				reg_delay_1clk <= 1'b1;
			end
     	end 
     	
     	INT: begin   
    		if(reg_delay_1clk) begin
				reg_int_start	<=1'b1;
				reg_delay_1clk <= 1'b0;
			end
			else
				reg_int_start <= 1'b0;
     	end

		endcase
	end
end

//---------------init FSM------------
always @ (reg_state_init or reg_init_start or reg_init_state_end or reg_clk_cnt_init or reg_init_end or reg_state_main)
begin
	next_state_init  = 3'b000;   
	case(reg_state_init)
	
	INIT_IDLE: begin
		if (reg_init_start & (reg_state_main == INIT)) 
   			next_state_init = TX_C15;
		else 
			next_state_init = INIT_IDLE;
    end
  	TX_C15: begin
    	if (reg_init_state_end) 
       		next_state_init = TX_C02;
    	else 
       		next_state_init = TX_C15;
  	end

  	TX_C02: begin
      	if (!reg_init_state_end)
       		next_state_init = WAIT_20u;  
    	else 
       		next_state_init = TX_C02;
  	end
  	
  	WAIT_20u: begin
      	if (reg_clk_cnt_init == 9'h1ff)
       		next_state_init = RX_C51;  
    	else 
       		next_state_init = WAIT_20u;
  	end
  	
  	RX_C51: begin
		if(reg_init_end) 
       		next_state_init = INIT_IDLE;
		else
			next_state_init = RX_C51;  
  	end
  	
    default: next_state_init = INIT_IDLE;  
	endcase
end

//--------------init Seq logic------------
always @ (posedge pin_clk or negedge reg_rst)
begin
	if (!reg_rst) begin
    	reg_state_init  <=  INIT_IDLE;
 	end 
 	else begin
    	reg_state_init  <=  next_state_init;
	end
end

//---------------init FSM_OUT----------------
always @ (posedge pin_clk or negedge reg_rst)
begin 
	if (!reg_rst) begin 
		reg_clk_cnt_init	<= 9'h000;
		reg_err			<= 1'b0;
		reg_init_end	<= 1'b0;
		reg_init_state_end <=1'b0;
	end 
	else begin
		case(reg_state_init)
 		
 		INIT_IDLE: begin
    		reg_clk_cnt_init	<= 9'h000;
			reg_init_end	<= 1'b0;
			reg_init_state_end <=1'b0;
		end
 		
 		TX_C15:  begin
			if(reg_clk_cnt_init == 9'h01f)begin									//�������>1.5us
				reg_init_state_end <= 1'b1;										//TX_C15 end������reg_init_state_end�ķ�תȷ����״̬����
				reg_clk_cnt_init <= 9'h000;
			end
			else
				reg_clk_cnt_init <= reg_clk_cnt_init + 9'h001;
 		end   
 
 		TX_C02: begin   
			if(reg_clk_cnt_init == 9'h01f)begin
				reg_init_state_end <= 1'b0;										//TX_C02 end
				reg_clk_cnt_init <= 9'h000;
			end
			else
				reg_clk_cnt_init <= reg_clk_cnt_init + 9'h001;
     	end 
     	
     	WAIT_20u: begin   
			reg_clk_cnt_init <= reg_clk_cnt_init + 9'h001;						//�ȴ�20us
     	end
     	
     	RX_C51: begin
     		if(reg_clk_cnt_init == 9'h01f)begin
				reg_init_end	<= 1'b1;										//init state end��init����������־
			end
			else
				reg_clk_cnt_init <= reg_clk_cnt_init + 9'h001;

			if(reg_dat_len == 8'h51)											//reg_dat_len work as buffer here
				reg_err <= 1'b0;
			else
				reg_err <= 1'b1;			
     	end

		endcase
	end
end

//---------------int FSM------------
always @ (reg_state_int or reg_int_start or reg_int_state_end or reg_usb_int_ep2_out or reg_clk_cnt_int or reg_int_end)
begin
	next_state_int  = 3'b000;   
	case(reg_state_int)
	
	INT_IDLE: begin
		if (reg_int_start) 
   			next_state_int = TX_C22;
		else 
			next_state_int = INT_IDLE;
    end
  	TX_C22: begin
    	if (reg_int_state_end) 
       		next_state_int = RX_INT_S;
    	else 
       		next_state_int = TX_C22;
  	end

  	RX_INT_S: begin
		if (reg_usb_int_ep2_out & (!reg_int_state_end))
       		next_state_int = TX_C28;  
    	else if((!reg_usb_int_ep2_out) & (!reg_int_state_end))
       		next_state_int = TX_C23;
		else
       		next_state_int = RX_INT_S;
  	end
  	
  	TX_C28: begin
      	if (reg_int_state_end)
       		next_state_int = RX_DAT;  
    	else 
       		next_state_int = TX_C28;
  	end
  	
  	RX_DAT: begin
		if (!reg_int_state_end)
       		next_state_int = TX_C2B;  
    	else 
       		next_state_int = RX_DAT;
  	end
  	
  	TX_C2B: begin
		if (reg_int_state_end)
       		next_state_int = TX_DAT;  
    	else 
       		next_state_int = TX_C2B;
  	end
  	
  	TX_DAT: begin
		if (!reg_int_state_end)
       		next_state_int = INT_IDLE;  
    	else 
       		next_state_int = TX_DAT;
  	end
  	
  	TX_C23: begin
		if (reg_int_end == reg_int_flag)
       		next_state_int = INT_IDLE;  
    	else 
       		next_state_int = TX_C23;
  	end
  	
    default: next_state_int = INT_IDLE;  
	endcase
end

//--------------int Seq logic------------
always @ (posedge pin_clk or negedge reg_rst)
begin
	if (!reg_rst) begin
    	reg_state_int  <=  INT_IDLE;
 	end 
 	else begin
    	reg_state_int  <=  next_state_int;
	end
end

//---------------int FSM_OUT----------------
always @ (posedge pin_clk or negedge reg_rst)
begin 
	if (!reg_rst) begin 
		reg_int_end		<= 1'b0;
		reg_usb_int_ep2_out	<= 1'b0;
		reg_clk_cnt_int <= 6'h00;
		reg_int_state_end <= 1'b0;
		reg_get_len <= 1'b0;
	end 
	else begin
		case(reg_state_int)
 		
 		INT_IDLE: begin
			reg_usb_int_ep2_out	<= 1'b0;
			reg_clk_cnt_int <= 6'h00;
			reg_int_state_end <= 1'b0;
			reg_get_len <= 1'b0;
		end
 		
 		TX_C22: begin
     		if(reg_clk_cnt_int == 6'h1f)begin
				reg_int_state_end <= 1'b1;										//TX_C22 end
				reg_clk_cnt_int <= 6'h00;
			end
			else
				reg_clk_cnt_int <= reg_clk_cnt_int + 6'h01; 
 		end   
 
 		RX_INT_S: begin   
     		if(reg_clk_cnt_int == 6'h1f)begin
				reg_int_state_end <= 1'b0;										//RX_INT_S end
				reg_clk_cnt_int <= 6'h00;
			end
			else
				reg_clk_cnt_int <= reg_clk_cnt_int + 6'h01;
			
			if(reg_dat_len == 8'h02)
				reg_usb_int_ep2_out <= 1'b1;
			else
				reg_usb_int_ep2_out <= 1'b0;
     	end 
     	
     	TX_C28: begin   
     		if(reg_clk_cnt_int == 6'h1f)begin
				reg_int_state_end <= 1'b1;										//TX_C28 end
				reg_clk_cnt_int <= 6'h00;
			end
			else
				reg_clk_cnt_int <= reg_clk_cnt_int + 6'h01;
     	end
     	
     	RX_DAT: begin
     		if(reg_clk_cnt_int == 6'h1f)begin
				reg_int_state_end <= 1'b0;										//RX_DAT end
				reg_clk_cnt_int <= 6'h00;
				reg_get_len <= 1'b0;
			end
			else if((reg_clk_cnt_int == 6'h0f) & (reg_dat_cnt != reg_dat_len[6:0]))begin	//���������ݼ��� >60ns
				reg_clk_cnt_int <= 6'h00;
				reg_get_len <= 1'b1;
			end
			else begin
				reg_clk_cnt_int <= reg_clk_cnt_int + 6'h01;
			end
     	end
     	
		TX_C2B: begin   
			if(reg_clk_cnt_int == 6'h1f)begin
				reg_int_state_end <= 1'b1;										//TX_C2B end
				reg_clk_cnt_int <= 6'h00;
			end
			else
				reg_clk_cnt_int <= reg_clk_cnt_int + 6'h01;
     	end
     	
     	TX_DAT: begin   
			if(reg_clk_cnt_int == 6'h1f)begin
				reg_int_state_end <= 1'b0;										//TX_DAT end
				reg_int_end <= ~reg_int_end; 									//int state end, int����������־
				reg_clk_cnt_int <= 6'h00;
			end
			else if((reg_clk_cnt_int == 6'h0f) & (reg_dat_cnt != reg_dat_len[6:0]))begin
				reg_clk_cnt_int <= 6'h00;
			end
			else begin
				reg_clk_cnt_int <= reg_clk_cnt_int + 6'h01;
			end	 
     	end
     	
     	TX_C23: begin   
			if(reg_clk_cnt_int == 6'h1f)begin
				reg_clk_cnt_int <= 6'h00;
				reg_int_end <= ~reg_int_end;
			end
			else
				reg_clk_cnt_int <= reg_clk_cnt_int + 6'h01;
     	end

		endcase
	end
end

//------------------control & cmd & TX dat-----------------	//
always @(reg_state_init or reg_state_int or reg_init_start or reg_dat_cnt)
begin
	if(reg_init_start) begin
		case(reg_state_init)

		TX_C15:  begin
 			rd = 1'b1;
     		wr = 1'b0;
     		a0 = 1'b1;
     		dat_oe = 1'b1;
     		buffer = 8'h15;														//CMD11_SET_USB_MODE	0x15	/* ����USB����ģʽ */
 		end   
 
		TX_C02: begin   
			rd = 1'b1;
     		wr = 1'b0;
     		a0 = 1'b0;
     		dat_oe = 1'b1;
     		buffer = 8'h02; 													//00H=δ���õ��豸��ʽ, 01H=�����õ��豸��ʽ����ʹ���ⲿ�̼�ģʽ(���ڲ�֧��), 02H=�����õ��豸��ʽ����ʹ�����ù̼�ģʽ
     																			//03H=SD������ģʽ/δ���õ�����ģʽ,���ڹ���ʹ�ȡSD���е��ļ�
     																			//04H=δ���õ�������ʽ, 05H=�����õ�������ʽ, 06H=�����õ�������ʽ�����Զ�����SOF��, 07H=�����õ�������ʽ���Ҹ�λUSB����
     	end 
     	
		RX_C51: begin
     		rd = 1'b0;
     		wr = 1'b1;
     		a0 = 1'b0;
     		dat_oe = 1'b0;
     		buffer = 8'h00;
     	end
     	
     	default: begin
     		rd = 1'b1;
     		wr = 1'b1;
     		a0 = 1'b0;
     		dat_oe = 1'b0;
     		buffer = 8'h00;
     	end
     	
     	endcase
	end
	else begin
		case(reg_state_int)
 		
 		TX_C22: begin
 			rd = 1'b1;
     		wr = 1'b0;
     		a0 = 1'b1;
     		dat_oe = 1'b1;
     		buffer = 8'h22;														//CMD01_GET_STATUS		0x22	/* ��ȡ�ж�״̬��ȡ���ж����� */
 		end   
 
 		RX_INT_S: begin   
			rd = 1'b0;
     		wr = 1'b1;
     		a0 = 1'b0;
     		dat_oe = 1'b0;
     		buffer = 8'h00;
     	end 
     	
     	TX_C28: begin   
    		rd = 1'b1;
     		wr = 1'b0;
     		a0 = 1'b1;
     		dat_oe = 1'b1;
     		buffer = 8'h28;														//CMD01_RD_USB_DATA		0x28	/* �豸��ʽ: �ӵ�ǰUSB�жϵĶ˵㻺������ȡ���ݿ�, ���ͷŻ�����, �൱�� CMD01_RD_USB_DATA0 + CMD00_UNLOCK_USB */
     	end
     	
     	RX_DAT: begin
     		rd = 1'b0;
     		wr = 1'b1;
     		a0 = 1'b0;
     		dat_oe = 1'b0;
     		buffer = 8'h00;
     	end
     	
		TX_C2B: begin   
    		rd = 1'b1;
     		wr = 1'b0;
     		a0 = 1'b1;
     		dat_oe = 1'b1;
     		buffer = 8'h2B; 													//CMD10_WR_USB_DATA7	0x2B	/* �豸��ʽ: ��USB�˵�2�ķ��ͻ�����д�����ݿ� */
     	end
     	
     	TX_DAT: begin   
    		rd = 1'b1;
     		wr = 1'b0;
     		a0 = 1'b0;
     		dat_oe = 1'b1;
     		case(reg_dat_cnt)
				7'h00: buffer = reg_dat_len;    
				7'h01: buffer = ~reg_buffer[0];      
				7'h02: buffer = ~reg_buffer[1];      
				7'h03: buffer = ~reg_buffer[2];      
				7'h04: buffer = ~reg_buffer[3];      
				7'h05: buffer = ~reg_buffer[4];      
				7'h06: buffer = ~reg_buffer[5];      
				7'h07: buffer = ~reg_buffer[6];      
				7'h08: buffer = ~reg_buffer[7];      
				7'h09: buffer = ~reg_buffer[8];      
				7'h0a: buffer = ~reg_buffer[9];      
				7'h0b: buffer = ~reg_buffer[10];     
				7'h0c: buffer = ~reg_buffer[11];     
				7'h0d: buffer = ~reg_buffer[12];     
				7'h0e: buffer = ~reg_buffer[13];     
				7'h0f: buffer = ~reg_buffer[14];     
				7'h10: buffer = ~reg_buffer[15];     
				7'h11: buffer = ~reg_buffer[16];     
				7'h12: buffer = ~reg_buffer[17];     
				7'h13: buffer = ~reg_buffer[18];     
				7'h14: buffer = ~reg_buffer[19];     
				7'h15: buffer = ~reg_buffer[20];     
				7'h16: buffer = ~reg_buffer[21];     
				7'h17: buffer = ~reg_buffer[22];     
				7'h18: buffer = ~reg_buffer[23];     
				7'h19: buffer = ~reg_buffer[24];     
				7'h1a: buffer = ~reg_buffer[25];     
				7'h1b: buffer = ~reg_buffer[26];     
				7'h1c: buffer = ~reg_buffer[27];     
				7'h1d: buffer = ~reg_buffer[28];     
				7'h1e: buffer = ~reg_buffer[29];     
				7'h1f: buffer = ~reg_buffer[30];     
				7'h20: buffer = ~reg_buffer[31];     
				7'h21: buffer = ~reg_buffer[32];     
				7'h22: buffer = ~reg_buffer[33];     
				7'h23: buffer = ~reg_buffer[34];     
				7'h24: buffer = ~reg_buffer[35];     
				7'h25: buffer = ~reg_buffer[36];     
				7'h26: buffer = ~reg_buffer[37];     
				7'h27: buffer = ~reg_buffer[38];     
				7'h28: buffer = ~reg_buffer[39];     
				7'h29: buffer = ~reg_buffer[40];     
				7'h2a: buffer = ~reg_buffer[41];     
				7'h2b: buffer = ~reg_buffer[42];     
				7'h2c: buffer = ~reg_buffer[43];     
				7'h2d: buffer = ~reg_buffer[44];     
				7'h2e: buffer = ~reg_buffer[45];     
				7'h2f: buffer = ~reg_buffer[46];     
				7'h30: buffer = ~reg_buffer[47];     
				7'h31: buffer = ~reg_buffer[48];     
				7'h32: buffer = ~reg_buffer[49];     
				7'h33: buffer = ~reg_buffer[50];     
				7'h34: buffer = ~reg_buffer[51];     
				7'h35: buffer = ~reg_buffer[52];     
				7'h36: buffer = ~reg_buffer[53];     
				7'h37: buffer = ~reg_buffer[54];     
				7'h38: buffer = ~reg_buffer[55];     
				7'h39: buffer = ~reg_buffer[56];     
				7'h3a: buffer = ~reg_buffer[57];     
				7'h3b: buffer = ~reg_buffer[58];     
				7'h3c: buffer = ~reg_buffer[59];     
				7'h3d: buffer = ~reg_buffer[60];     
				7'h3e: buffer = ~reg_buffer[61];     
				7'h3f: buffer = ~reg_buffer[62]; 
				7'h40: buffer = ~reg_buffer[63]; 
				default: buffer = 8'h00;
				endcase 
     	end
     	
     	TX_C23: begin   
    		rd = 1'b1;
     		wr = 1'b0;
     		a0 = 1'b1;
     		dat_oe = 1'b1;
     		buffer = 8'h23;														//CMD00_UNLOCK_USB		0x23	/* �豸��ʽ: �ͷŵ�ǰUSB������ */
     	end
		
		default:begin
			rd = 1'b1;
     		wr = 1'b1;
     		a0 = 1'b0;
     		dat_oe = 1'b0;
     		buffer = 8'h00;
		end
		
		endcase
	end
end

//-----------------(RX & TX) <---> (dat & cmd)------------
always @ (posedge pin_clk or negedge reg_rst)
begin
	if(!reg_rst) begin
		reg_wr_oe	<= 1'b0;
		reg_rd_oe	<= 1'b0;
		reg_dat_cnt	<= 7'h00;
		reg_dat_len	<= 8'h00;
	end
	else if((((reg_state_init == TX_C15) | (reg_state_init == TX_C02)) & (reg_clk_cnt_init == 9'h001)) | (((reg_state_int == TX_C22) | (reg_state_int == TX_C28) | (reg_state_int == TX_C2B)  | (reg_state_int == TX_C23)) & (reg_clk_cnt_int == 6'h01)))begin	//TX cmd or control
		reg_wr_oe	<= 1'b1;
		reg_rd_oe	<= 1'b1;
	end
	else if(((reg_state_init == RX_C51) & ((reg_clk_cnt_init == 9'h001) | (reg_clk_cnt_init == 9'h002))) | (((reg_state_int == RX_INT_S) | (reg_state_int == RX_DAT)) & ((reg_clk_cnt_int == 6'h01) | (reg_clk_cnt_int == 6'h02)))) begin			//RX dat
		reg_wr_oe	<= 1'b1;
		reg_rd_oe	<= 1'b1;
		if((!reg_get_len) & ((reg_clk_cnt_init == 9'h02) | (reg_clk_cnt_int == 6'h02))) begin
			reg_dat_len   <= pin_data;
		end
		else if(reg_get_len & (reg_clk_cnt_int == 6'h02)) begin
			if(reg_dat_cnt != reg_dat_len[6:0])begin
				reg_dat_cnt <= reg_dat_cnt + 7'h01;
				
				case(reg_dat_cnt)
				7'h00: reg_buffer[0] <= pin_data; 
				7'h01: reg_buffer[1] <= pin_data;  
				7'h02: reg_buffer[2] <= pin_data;   
				7'h03: reg_buffer[3] <= pin_data;   
				7'h04: reg_buffer[4] <= pin_data;   
				7'h05: reg_buffer[5] <= pin_data;   
				7'h06: reg_buffer[6] <= pin_data;   
				7'h07: reg_buffer[7] <= pin_data;   
				7'h08: reg_buffer[8] <= pin_data;   
				7'h09: reg_buffer[9] <= pin_data;   
				7'h0a: reg_buffer[10] <= pin_data;  
				7'h0b: reg_buffer[11] <= pin_data;  
				7'h0c: reg_buffer[12] <= pin_data;  
				7'h0d: reg_buffer[13] <= pin_data;  
				7'h0e: reg_buffer[14] <= pin_data;  
				7'h0f: reg_buffer[15] <= pin_data;  
				7'h10: reg_buffer[16] <= pin_data;  
				7'h11: reg_buffer[17] <= pin_data;  
				7'h12: reg_buffer[18] <= pin_data;  
				7'h13: reg_buffer[19] <= pin_data;  
				7'h14: reg_buffer[20] <= pin_data;  
				7'h15: reg_buffer[21] <= pin_data;  
				7'h16: reg_buffer[22] <= pin_data;  
				7'h17: reg_buffer[23] <= pin_data;  
				7'h18: reg_buffer[24] <= pin_data;  
				7'h19: reg_buffer[25] <= pin_data;  
				7'h1a: reg_buffer[26] <= pin_data;  
				7'h1b: reg_buffer[27] <= pin_data;  
				7'h1c: reg_buffer[28] <= pin_data;  
				7'h1d: reg_buffer[29] <= pin_data;  
				7'h1e: reg_buffer[30] <= pin_data;  
				7'h1f: reg_buffer[31] <= pin_data;  
				7'h20: reg_buffer[32] <= pin_data;  
				7'h21: reg_buffer[33] <= pin_data;  
				7'h22: reg_buffer[34] <= pin_data;  
				7'h23: reg_buffer[35] <= pin_data;  
				7'h24: reg_buffer[36] <= pin_data;  
				7'h25: reg_buffer[37] <= pin_data;  
				7'h26: reg_buffer[38] <= pin_data;  
				7'h27: reg_buffer[39] <= pin_data;  
				7'h28: reg_buffer[40] <= pin_data;  
				7'h29: reg_buffer[41] <= pin_data;  
				7'h2a: reg_buffer[42] <= pin_data;  
				7'h2b: reg_buffer[43] <= pin_data;  
				7'h2c: reg_buffer[44] <= pin_data;  
				7'h2d: reg_buffer[45] <= pin_data;  
				7'h2e: reg_buffer[46] <= pin_data;  
				7'h2f: reg_buffer[47] <= pin_data;  
				7'h30: reg_buffer[48] <= pin_data;  
				7'h31: reg_buffer[49] <= pin_data;  
				7'h32: reg_buffer[50] <= pin_data;  
				7'h33: reg_buffer[51] <= pin_data;  
				7'h34: reg_buffer[52] <= pin_data;  
				7'h35: reg_buffer[53] <= pin_data;  
				7'h36: reg_buffer[54] <= pin_data;  
				7'h37: reg_buffer[55] <= pin_data;  
				7'h38: reg_buffer[56] <= pin_data;  
				7'h39: reg_buffer[57] <= pin_data;  
				7'h3a: reg_buffer[58] <= pin_data;  
				7'h3b: reg_buffer[59] <= pin_data;  
				7'h3c: reg_buffer[60] <= pin_data;  
				7'h3d: reg_buffer[61] <= pin_data;  
				7'h3e: reg_buffer[62] <= pin_data;  
				7'h3f: reg_buffer[63] <= pin_data;   
				endcase
			end
		end		
	end
	else if((reg_state_int == TX_DAT) & ((reg_clk_cnt_int == 6'h01) | (reg_clk_cnt_int == 6'h0f))) begin	//TX DAT
		if(reg_clk_cnt_int == 6'h01)begin
			reg_wr_oe	<= 1'b1;
			reg_rd_oe	<= 1'b1;
		end
		else if(reg_dat_cnt != reg_dat_len[6:0])begin
			reg_dat_cnt <= reg_dat_cnt + 7'h01;			
		end        
	end                
	else begin         
		reg_wr_oe <= 1'b0;
		reg_rd_oe <= 1'b0;
		if(reg_clk_cnt_int == 6'h1f)
			reg_dat_cnt <= 7'h00;
	end                
end                    
                       
endmodule          
                                                         