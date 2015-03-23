MIL_3_Tfile_Hdr_ 171A 171A modeler 9 550777F5 550ECF79 9B ECE-PHO305-01 chenyua 0 0 none none 0 0 none 909D266E 2EE4 0 0 0 0 0 0 2b1a 1                                                                                                                                                                                                                                                                                                                                                                                         ��g�      D  \  `  (  ,  #�  ,�  ,�  ,�  ,�  ,�  ,�  ,�  #�  #�      R_Pt_Num    �������    ����          ����          ����          ����           �Z                this       is       the       largest       port       number,       which       is       the       total       port       number       minus       one       	   begsim intrpt         
   ����   
   doc file            	nd_module      endsim intrpt             ����      failure intrpts            disabled      intrpt interval         ԲI�%��}����      priority              ����      recovery intrpts            disabled      subqueue                     count    ���   
   ����   
      list   	���   
          
      super priority             ����             int	\port_number;       int	\This_Node_Number;       int	\In_Node_Number;       int	\Out_Node_Number;       int	\My_Total_Node_Num;       int	\Total_Node_Num;       int	\Max_Deg;              B   // include files   #include <stdio.h>   #include <stdlib.h>   #include <math.h>   #include <string.h>   #include <direct.h>       /*   define the interrupt codes here   */    #define SA_FLIT_INTRPT_CODE 1000    #define ID_DONE_INTRPT_CODE 1001       8// define FLIT_ARRIVAL, stream interrupt within the node   ;#define FLIT_ARRIVAL (op_intrpt_type () == OPC_INTRPT_STRM)       // check the adjacent nodes   6#define SA_FLIT (op_intrpt_type () == OPC_INTRPT_STRM)       '// ACK remote intrpt from adjacent node   4#define ACK (op_intrpt_type () == OPC_INTRPT_REMOTE)       // PSQUEUE_READY signal   <#define PSQUEUE_READY (op_intrpt_type () == OPC_INTRPT_STAT)       // define the time out handler   7#define TIME_OUT (op_intrpt_type () == OPC_INTRPT_SELF)       // define the time out handler   6#define ID_DONE (op_intrpt_type () == OPC_INTRPT_SELF)       // define flit type here   #define Flit_Type_Data_Flit 0   #define Flit_Type_Dest_Addr 1   #define Flit_Type_Src_Addr 2   #define Flit_Type_Worm_Length 6   #define Flit_Type_Tail 7       -// read the node specified routing table here   "#define ID_STREAM get_id_stream();       '// flit handler when a flit is arriving   ##define HANDLE_FLIT flit_handler();       // ACK handler   !#define HANDLE_ACK ACK_handler();       &// handler swire when PSQueue is ready   %#define HANDLE_SWIRE SWIRE_handler();       //    .#define HANDLE_TIMER router_handle_time_out();           /*   routing table   */   int ** Routing_Table;                                     -Packet * r_generate_flit(int type, int data){   	Packet * pkptr;   	   	FIN(generate_flit());   	   ,	pkptr = op_pk_create_fmt("Yu_wh_pkformat");   $	op_pk_nfd_set(pkptr, "type", type);   $	op_pk_nfd_set(pkptr, "data", data);   	   	FRET(pkptr);   }       &void load_ARnode_routing_table(void) {   	int i;   	char RName[128];   	char lbuf[2048];   	char * nToken;   	FILE * Rinfile;   		int row;   		int col;   	int dest_port;   	   "	FIN(load_ARnode_routing_table());   	   Z	sprintf(RName, "C:\\Users\\chenyua\\OPNET_Project\\WH\\ARnode_%d.txt", This_Node_Number);   &	if (!(Rinfile = fopen(RName, "r"))) {   4		printf("load_routing_table: could not find file");   		exit(-2);   	}   	   7	Routing_Table = (int **)malloc(Max_Deg * sizeof(int));   [	// the reason for adding one here is that the port includes the PSQueue port, i.e. port #0   $	for (i = 0; i < Max_Deg + 1; i++) {   A		Routing_Table[i] = (int *)malloc(Total_Node_Num * sizeof(int));   	}   	   	// start from the second line   	fgets(lbuf, 2048, Rinfile);   		row = 0;   #	while(fgets(lbuf, 2048, Rinfile)){   		//printf("%s", lbuf);   !		nToken = strtok(lbuf, " ] [ ");   		   .		for (col = 0; col < Total_Node_Num; col++) {   			   			dest_port = atoi(nToken);   '			Routing_Table[row][col] = dest_port;   			   4			if (op_prg_odb_ltrace_active ("RT") == OPC_TRUE){   +				printf("%3d", Routing_Table[row][col]);   				if(col == 63){   ,					printf("\nrow:%d  col:%d\n", row, col);   				}   			}   #			nToken = strtok(NULL, " ] [ ");	   		}   		row++;   	}   	FOUT;   }       void initialize_router(void){   	   	Packet * pkptr;   	Objid myObjid, parentObjid;   	char parentname[64];   	char RName[128];   	FILE * Rinfile;   	char lbuf[128];   	char * nToken;   	int Max_Pt_Num;   	int strm_pt;   	   	FIN(initialize_router());   		   	// get the local node number   	myObjid = op_id_self();   (	parentObjid = op_topo_parent (myObjid);   8	op_ima_obj_attr_get (parentObjid, "name", &parentname);   )	This_Node_Number = atoi(&parentname[5]);   	   #	// get the routing table file path   Z	sprintf(RName, "C:\\Users\\chenyua\\OPNET_Project\\WH\\ARnode_%d.txt", This_Node_Number);   &	if (!(Rinfile = fopen(RName, "r"))) {   0		printf("initialize_gen: could not find file");   		exit(-2);   	}   	   	fgets(lbuf, 128, Rinfile);    	nToken = strtok(lbuf, " \t\n");   	Total_Node_Num = atoi(nToken);    	nToken = strtok(NULL, " \t\n");   	Max_Deg = atoi(nToken);   	fclose(Rinfile);   	   H	// printf("Total_Node_Num: %d  Max_Deg: %d\n",Total_Node_Num, Max_Deg);   	   T	// send the source head flit to adjacent nodes, they will get the connection status   4	// the max port number is defined in the attributes   8	op_ima_obj_attr_get (myObjid, "R_Pt_Num", &Max_Pt_Num);   	   7	for (strm_pt = 2; strm_pt <= Max_Deg + 1; strm_pt++) {   /	//for (strm_pt = 2; strm_pt <= 2; strm_pt++) {   D		pkptr = r_generate_flit(Flit_Type_Src_Addr, This_Node_Number);				   		op_pk_send(pkptr, strm_pt);   	}   	   +	// call the routing table initializer here   	// if(This_Node_Number == 0)   	load_ARnode_routing_table();   	   	FOUT;   }       )// get node attribution in Identify state   void get_id_stream(void) {       	Packet * pkptr;   
	int type;   
	int data;   	int strm_port_number;       	FIN(get_id_stream());   	   &	strm_port_number = op_intrpt_strm ();   &	pkptr = op_pk_get (strm_port_number);   %	op_pk_nfd_get(pkptr, "type", &type);   %	op_pk_nfd_get(pkptr, "data", &data);   	   2	if (op_prg_odb_ltrace_active ("SA") == OPC_TRUE){   `		printf("R_Node%d strm@%d: type:%d data:%d\n", This_Node_Number, strm_port_number, type, data);   	}       	// get the neighbors ID   H	// send source address flit to all neighbours and wait for the response   <	// pkptr = generate_flit(Flit_Type_Src_Addr, node_ID_self);   	   <	// how to get active ports? and how to hear back from them?       	   9	// schedule a self interrupt to move to Listening status   -	op_intrpt_schedule_self (op_sim_time (), 0);       	FOUT;   }       %void send_stat_wire_to_PGQueue(void){   	   "	FIN(send_stat_wire_to_PGQueue());   	   	FOUT;   }       /void send_remote_intrpt_to_adjacent_node(void){       ,	FIN(send_remote_intrpt_to_adjacent_node());   	   	FOUT;   }       (void remote_node_intrpt_listerner(void){       %	FIN(remote_node_intrpt_listerner());   	   	FOUT;   }       // functions for Listerning   void flit_handler(void){   	Packet * pkptr;   	int flit_type;   	int tmp_type;   	int tmp_data;   	   	FIN(flit_handler());   	   %	pkptr = op_pk_get(op_intrpt_strm());   6	flit_type = op_pk_nfd_get(pkptr, "type", &flit_type);   )	op_pk_nfd_get(pkptr, "type", &tmp_type);   )	op_pk_nfd_get(pkptr, "data", &tmp_data);   	   2	if (op_prg_odb_ltrace_active ("LS") == OPC_TRUE){   a		printf("Listern: node: %d got flit type:%d data:%d\n\n", This_Node_Number, tmp_type, tmp_data);   	}   	   ;	// call other procedures that handle individual flit types   '	if(flit_type == Flit_Type_Dest_Addr) {   ;	// look up the routing table and determine the output port   	   ,	// identify whether the port is busy or not   	   9	// else if it's busy, keep the header flit in a register   	   S	// else if it's not busy, then the input port it came from is associated with the    '	// output port and binding takes place   	   	   ,	}else if(flit_type == Flit_Type_Src_Addr) {   	   	   /	}else if(flit_type == Flit_Type_Worm_Length) {   	   	   (	}else if(flit_type == Flit_Type_Tail) {   '	// check the list of waiting head flit   2	// free the input port and associated output port   	   	}   	   	send_stat_wire_to_PGQueue();   	   	FOUT;   }       void ACK_handler(void) {   	   	FIN(ACK_handler());   	   	FOUT;   }       void SWIRE_handler(void){       	FIN(SWIRE_handler());   	   	   	FOUT;   }           "void router_handle_time_out(void){   	int intrpt_code;   	   	FIN(router_handle_time_out());   	    	intrpt_code = op_intrpt_code();   	   	//for debug usage   	   	FOUT;   }       void route_pk(void){              Packet * pkptr;   	  int type;   	  int dest_address;   	           FIN(route_pk());   	     	  // get packet (flit)   (	  pkptr = op_pk_get(op_intrpt_strm ());   	     	  // get type   1      op_pk_nfd_get_int32 (pkptr, "type", &type);   	     	  // get data   1      op_pk_nfd_get_int32 (pkptr, "data", &type);   	     	  dest_address = 0;   '      op_pk_send (pkptr, dest_address);   	           FOUT;   }                                                         �            
   Init   
       
          initialize_router();   
                     
   ����   
          pr_state        J            
   Identify   
       
      // printf("ENTER IDentify!\n");   
                         ����             pr_state        v            
   	Listening   
       
       // printf("ENTER Listening!\n");   
       
       
           ����             pr_state         	            �        �    L            
   tr_4   
       ����          ����          
    ����   
          ����                       pr_transition              H   �     9     �   �  �   �  Z             
   tr_5   
       
   SA_FLIT   
       
   	ID_STREAM   
       
    ����   
          ����                       pr_transition              J  �     @     �  �  �  �  X            
   tr_7   
       
   default   
       ����          
    ����   
          ����                       pr_transition              �       S    p            
   tr_8   
       
   ID_DONE   
       ����          
    ����   
          ����                       pr_transition      	        �   �     e    �   �     �  o   �          
   tr_9   
       
   default   
       ����          
    ����   
          ����                       pr_transition      
        �   u     y   �  P   �  �   �  �             
   tr_10   
       
   FLIT_ARRIVAL   
       
   HANDLE_FLIT   
       
    ����   
          ����                       pr_transition              �  �     �    �  C  �  `  �  �  {            
   tr_12   
       
   PSQUEUE_READY   
       
   HANDLE_SWIRE   
       
    ����   
          ����                       pr_transition              #  �     t  "  e  �  �  K  k            
   tr_13   
       
   TIME_OUT   
       
   HANDLE_TIMER   
       
    ����   
          ����                       pr_transition              �       �   �  �   �     �    A  �            
   tr_14   
       
   ACK   
       
   
HANDLE_ACK   
       
    ����   
          ����                       pr_transition                                             