MIL_3_Tfile_Hdr_ 171A 171A modeler 9 550777F5 550B2E97 2A ECE-PHO305-01 chenyua 0 0 none none 0 0 none 5E3470CB 1E70 0 0 0 0 0 0 2b1a 1                                                                                                                                                                                                                                                                                                                                                                                         ��g�      D   H   L      p  L  P  T  X  d  h  l  `  l           	   begsim intrpt         
   ����   
   doc file            	nd_module      endsim intrpt             ����      failure intrpts            disabled      intrpt interval         ԲI�%��}����      priority              ����      recovery intrpts            disabled      subqueue                     count    ���   
   ����   
      list   	���   
          
      super priority             ����             int	\port_number;       int	\node_ID_self;              6       ;#define FLIT_ARRIVAL (op_intrpt_type () == OPC_INTRPT_STRM)       6#define SA_FLIT (op_intrpt_type () == OPC_INTRPT_STRM)       '// ACK remote intrpt from adjacent node   4#define ACK (op_intrpt_type () == OPC_INTRPT_REMOTE)       // PSQUEUE_READY signal   <#define PSQUEUE_READY (op_intrpt_type () == OPC_INTRPT_STAT)       // define the time out handler   7#define TIME_OUT (op_intrpt_type () == OPC_INTRPT_SELF)       // define the time out handler   6#define ID_DONE (op_intrpt_type () == OPC_INTRPT_SELF)       // define flit type here   #define Flit_Type_Data_Flit 0   #define Flit_Type_Dest_Addr 1   #define Flit_Type_Src_Addr 2   #define Flit_Type_Worm_Length 6   #define Flit_Type_Tail 7       -// read the node specified routing table here   "#define ID_STREAM get_id_stream();       '// flit handler when a flit is arriving   ##define HANDLE_FLIT flit_handler();       // ACK handler   !#define HANDLE_ACK ACK_handler();       &// handler swire when PSQueue is ready   %#define HANDLE_SWIRE SWIRE_handler();       .#define HANDLE_TIMER router_handle_time_out();           /* packet stream definitions */   #define RCV_IN_STRM 0   #define SRC_IN_STRM 1   #define XMT_OUT_STRM 0       /* transition macros */   H#define SRC_ARRVL (ev_type == OPC_INTRPT_STRM && ev_strm == SRC_IN_STRM)   H#define RCV_ARRVL (ev_type == OPC_INTRPT_STRM && ev_strm == RCV_IN_STRM)                               �           )// get node attribution in Identify state   void get_id_stream(void) {       	FIN(get_id_stream());   		   	// get the neighbors ID   H	// send source address flit to all neighbours and wait for the response   <	// pkptr = generate_flit(Flit_Type_Src_Addr, node_ID_self);   	   <	// how to get active ports? and how to hear back from them?       	   9	// schedule a self interrupt to move to Listening status   -	op_intrpt_schedule_self (op_sim_time (), 0);           	FOUT;   }       %void send_stat_wire_to_PGQueue(void){   	   "	FIN(send_stat_wire_to_PGQueue());   	   	FOUT;   }       /void send_remote_intrpt_to_adjacent_node(void){       ,	FIN(send_remote_intrpt_to_adjacent_node());   	   	FOUT;   }       (void remote_node_intrpt_listerner(void){       %	FIN(remote_node_intrpt_listerner());   	   	FOUT;   }       // functions for Listerning   void flit_handler(void){   	Packet * pkptr;   	int flit_type;   	int tmp_type;   	int tmp_data;   	   	FIN(flit_handler());   	   %	pkptr = op_pk_get(op_intrpt_strm());   6	flit_type = op_pk_nfd_get(pkptr, "type", &flit_type);   )	op_pk_nfd_get(pkptr, "type", &tmp_type);   )	op_pk_nfd_get(pkptr, "data", &tmp_data);   	   W	printf("node: %d got flit type:%d data:%d\n\n", This_Node_Number, tmp_type, tmp_data);   	   ;	// call other procedures that handle individual flit types   '	if(flit_type == Flit_Type_Dest_Addr) {   ;	// look up the routing table and determine the output port   	   ,	// identify whether the port is busy or not   	   9	// else if it's busy, keep the header flit in a register   	   S	// else if it's not busy, then the input port it came from is associated with the    '	// output port and binding takes place   	   	   ,	}else if(flit_type == Flit_Type_Src_Addr) {   	   	   /	}else if(flit_type == Flit_Type_Worm_Length) {   	   	   (	}else if(flit_type == Flit_Type_Tail) {   '	// check the list of waiting head flit   2	// free the input port and associated output port   	   	}   	   	send_stat_wire_to_PGQueue();   	   	FOUT;   }       void ACK_handler(void) {   	   	FIN(ACK_handler());   	   	FOUT;   }       void SWIRE_handler(void){       	FIN(SWIRE_handler());   	   	   	FOUT;   }           "void router_handle_time_out(void){   	int intrpt_code;   	   	FIN(router_handle_time_out());   	    	intrpt_code = op_intrpt_code();   	   	//for debug usage   	   	FOUT;   }       void route_pk(void){              Packet * pkptr;   	  int type;   	  int dest_address;   	           FIN(route_pk());   	     	  // get packet (flit)   (	  pkptr = op_pk_get(op_intrpt_strm ());   	     	  // get type   1      op_pk_nfd_get_int32 (pkptr, "type", &type);   	     	  // get data   1      op_pk_nfd_get_int32 (pkptr, "data", &type);   	     	  dest_address = 0;   '      op_pk_send (pkptr, dest_address);   	           FOUT;   }                                                         �            
   Init   
       
      // get the self node ID   node_ID_self = op_id_self ();   
                     
   ����   
          pr_state        J            
   Identify   
                                       ����             pr_state        v            
   	Listening   
                     
       
           ����             pr_state         	            �        �    L            
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