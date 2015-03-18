MIL_3_Tfile_Hdr_ 171A 171A modeler 9 55086144 5509BA52 1C ECE-PHO305-01 chenyua 0 0 none none 0 0 none 477750D1 187C 0 0 0 0 0 0 2b1a 1                                                                                                                                                                                                                                                                                                                                                                                         ��g�      D   H   L        X  \  `  d  p  t  x  �             	   begsim intrpt             ����      doc file            	nd_module      endsim intrpt             ����      failure intrpts            disabled      intrpt interval         ԲI�%��}����      priority              ����      recovery intrpts            disabled      subqueue                     count    ���   
   ����   
      list   	���   
          
      super priority             ����             char	\router_ready_statewire;       int	\this_node_address;          int ev_type, ev_strm;       -   *// ? #define a self initialization trigger           $// define packet arrival signal here   7#define PK_ARRVL (op_intrpt_type () == OPC_INTRPT_STRM)       1// define packet receives at the destination here   =#define DEST_REACHED (op_intrpt_type () == OPC_INTRPT_REMOTE)       !// get the handshake signale here   ;#define ROUTER_READY (op_intrpt_type () == OPC_INTRPT_STAT)       // define the time out handler   7#define TIME_OUT (op_intrpt_type () == OPC_INTRPT_SELF)           (#define SEND_NEXT_FLIT send_next_flit();       $#define RECEIVE_FLIT receive_flit();       *#define HANDLE_TIME_OUT handle_time_out();       "#define SCHEDULE_TF schedule_tf();               // define flit type here   #define Flit_Type_Data_Flit 0   #define Flit_Type_Dest_Addr 1   #define Flit_Type_Src_Addr 2   #define Flit_Type_Worm_Length 6   #define Flit_Type_Tail 7       /* packet stream definitions */   #define RCV_IN_STRM 0   #define SRC_IN_STRM 1   #define XMT_OUT_STRM 0       /* transition macros */   H#define SRC_ARRVL (ev_type == OPC_INTRPT_STRM && ev_strm == SRC_IN_STRM)   H#define RCV_ARRVL (ev_type == OPC_INTRPT_STRM && ev_strm == RCV_IN_STRM)       // define node number range   #define MIN_NODE_NUMBER 0   #define MAX_NODE_NUMBER 64   r       void handle_time_out(void){   	int intrpt_code;   	   	FIN(handle_time_out());   	    	intrpt_code = op_intrpt_code();   	   	//tail flit is transmitted   	if (intrpt_code == 1) {   	   	   (	// otherwise generating another message   	} else if (intrpt_code == 2) {   	   	   	}   	   	FOUT;   }       +Packet * generate_flit(int type, int data){   	Packet * pkptr;   	   	FIN(generate_flit());   	   ,	pkptr = op_pk_create_fmt("Yu_wh_pkformat");   $	op_pk_nfd_set(pkptr, "type", type);   $	op_pk_nfd_set(pkptr, "data", data);   	   	FRET(pkptr);   }       void schedule_tf(void){   	Packet * pkptr;       	FIN(schedule_tf());   	   +	pkptr = generate_flit(Flit_Type_Tail, 11);   ,	op_subq_pk_insert(0, pkptr, OPC_QPOS_TAIL);   	   	   	FOUT;   }       $void generate_message_enqueue(void){   	Packet * pkptr;   	int dest_address;   	int message_length;   	   	FIN(generate_message());   	   	do {   e		dest_address = (int)op_dist_outcome(op_dist_load("uniform_int", MIN_NODE_NUMBER, MAX_NODE_NUMBER));   	   +	}while(dest_address == this_node_address);   	   	// generate destination flit   :	pkptr = generate_flit(Flit_Type_Dest_Addr, dest_address);   ,	op_subq_pk_insert(0, pkptr, OPC_QPOS_TAIL);   	   ?	pkptr = generate_flit(Flit_Type_Dest_Addr, this_node_address);   ,	op_subq_pk_insert(0, pkptr, OPC_QPOS_TAIL);   	   	message_length = 200;   >	pkptr = generate_flit(Flit_Type_Worm_Length, message_length);   ,	op_subq_pk_insert(0, pkptr, OPC_QPOS_TAIL);   	   -	for(;message_length >=0; message_length--) {   =		pkptr = generate_flit(Flit_Type_Data_Flit, message_length);   -		op_subq_pk_insert(0, pkptr, OPC_QPOS_TAIL);   	}   	   	   	FOUT;   }       void send_next_flit(){   	Packet * pkptr;   	   	int intrpt_code;   	   	FIN(send_next_flit());   	    	intrpt_code = op_intrpt_code();   	   $	if (op_subq_empty(0) == OPC_FALSE){   .		pkptr = op_subq_pk_remove(0, OPC_QPOS_HEAD);   	}else {   1		pkptr = generate_flit(Flit_Type_Data_Flit, 11);   	}   	   !	op_pk_send(pkptr, XMT_OUT_STRM);   	   	FOUT;   }                                                                                                                      �  �          
   init   
       J      1// read in the values for the run-time attributes           3// generate a self interrupt here to send a message           // get this node address   !this_node_address = op_id_self();   J                     
   ����   
          pr_state        �  �          
   idle   
                     J      0// judge the interrupt type and stream type here       ev_type = op_intrpt_type();   if (ev_type == OPC_INTRPT_STRM)      ev_strm = op_intrpt_strm();   J           ����             pr_state                       0  �      �  �  �  �          
   tr_0   
       ����          ����          
    ����   
          ����                       pr_transition              5   �     �  t  �   �  �  K  �  �          
   tr_2   
       
   ROUTER_READY   
       
   SEND_NEXT_FLIT   
       
    ����   
          ����                       pr_transition              N       �  �  w  �  �  ,  �  �          
   tr_3   
       
   DEST_REACHED   
       
   SCHEDULE_TF   
       
    ����   
          ����                       pr_transition              4       �  �  �  '    �  �  �          
   tr_4   
       
   TIME_OUT   
       
   HANDLE_TIME_OUT   
       
    ����   
          ����                       pr_transition              [  -     �  |    `  �   �  �  t          
   tr_5   
       
   default   
       ����          
    ����   
          ����                       pr_transition                                             