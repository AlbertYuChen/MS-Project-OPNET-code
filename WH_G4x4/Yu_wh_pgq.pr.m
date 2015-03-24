MIL_3_Tfile_Hdr_ 171A 171A modeler 9 55086144 5510CA68 E5 ECE-PHO309-01 chenyua 0 0 none none 0 0 none EB7AB168 3AEA 0 0 0 0 0 0 2b1a 1                                                                                                                                                                                                                                                                                                                                                                                         ��g�      D  @  D      3�  8�  8�  8�  8�  8�  8�  8�  3�  3�      Worm_Gen_Rate   �������      worms/sec/node       ?�z�G�{����              ����              ����           �Z             Worm_Gen_Len    �������       Flits          
����          ����          ����           �Z                 	   begsim intrpt         
   ����   
   doc file            	nd_module      endsim intrpt             ����      failure intrpts            disabled      intrpt interval         ԲI�%��}����      priority              ����      recovery intrpts            disabled      subqueue                     count    ���   
   ����   
      list   	���   
          
      super priority             ����          &   char	\Being_Trans_or_Not;       char	\Router_Ready_Statewire;       char	\Sent_TF_or_not;       char	\TF_Sch_TimeOut;       'Distribution *	\Dest_Node_Number_Distr;       $Distribution *	\Inter_Arrival_Distr;       Distribution *	\Worm_Len_Distr;       double	\Avr_Inter_Arrival_time;       double	\Inter_Arrival_Time;       double	\Worm_Gen_Rate;       int	\Ave_Worm_Gen_Len;       int	\Dest_Node_Number;       int	\In_Port;       int	\Num_Flits_Left_to_Send;       int	\Out_Port;       int	\This_Node_Number;       int	\Total_Node_Num;       int	\Worm_Counter;       int	\Worm_Gen_Len;           ;   // include files   #include <stdio.h>   #include <stdlib.h>   #include <math.h>   #include <string.h>   #include <direct.h>       $// define packet arrival signal here   7#define PK_ARRVL (op_intrpt_type () == OPC_INTRPT_STRM)       1// define packet receives at the destination here   =#define DEST_REACHED (op_intrpt_type () == OPC_INTRPT_REMOTE)       !// get the handshake signale here   ;#define ROUTER_READY (op_intrpt_type () == OPC_INTRPT_STAT)       // define the time out handler   7#define TIME_OUT (op_intrpt_type () == OPC_INTRPT_SELF)       !// define of transition functions   (#define SEND_NEXT_FLIT send_next_flit();   $#define RECEIVE_FLIT receive_flit();   .#define HANDLE_TIME_OUT PGQ_handle_time_out();   "#define SCHEDULE_TF schedule_tf();       // define flit type here   #define Flit_Type_Data_Flit 0   #define Flit_Type_Dest_Addr 1   #define Flit_Type_Src_Addr 2   #define Flit_Type_Worm_Length 6   #define Flit_Type_Tail 7       // define node number range   #define MIN_NODE_NUMBER 0   #define MAX_NODE_NUMBER 63       ;// define interrupt code here, self intrrupt start from 100   #define GEN_MSG_INTRPT_CODE 100   !#define TAIL_FLIT_INTRPT_CODE 101       // PGQ_Channel_0 0   #define PGQ_to_Router_Channel 0       // Status of Sent_TF_or_not   #define Did_Send_TF 1   #define Didnt_Send_TF 0       // status of Being_Trans_or_Not   #define Being_Trans 1   #define Not_Being_Trans 0       // define FLIT_TRANSTER_TIME   .#define FLIT_TRANSTER_TIME 0.00083333333333333       "// define status of TF_Sch_TimeOut   #define TF_Sch_TimeOut_TRUE 1   #define TF_Sch_TimeOut_FALSE 0          X   +Packet * generate_flit(int type, int data){   	Packet * pkptr;   	   	FIN(generate_flit());   	   ,	pkptr = op_pk_create_fmt("Yu_wh_pkformat");   $	op_pk_nfd_set(pkptr, "type", type);   $	op_pk_nfd_set(pkptr, "data", data);   	   	FRET(pkptr);   }        Packet * get_pk_in_queue(void) {       	Packet * pkptr;   	   	FIN(get_pk_in_queue());   $	if (op_subq_empty(0) == OPC_FALSE){   .		pkptr = op_subq_pk_remove(0, OPC_QPOS_HEAD);   	}else {   		pkptr = NULL;   	}   	   	FRET (pkptr);   }       void schedule_tf(void){       	int rmt_tf_intpt_code;   	double remaining_time;       	FIN(schedule_tf());   	   &	rmt_tf_intpt_code = op_intrpt_code();   	   )	// when the head arrives the destination   K	if (Num_Flits_Left_to_Send > 0 && Dest_Node_Number == rmt_tf_intpt_code) {   @		printf("Num_Flits_Left_to_Send:%d\n", Num_Flits_Left_to_Send);   ?		remaining_time = Num_Flits_Left_to_Send * FLIT_TRANSTER_TIME;   (		TF_Sch_TimeOut = TF_Sch_TimeOut_FALSE;   Q		op_intrpt_schedule_self(op_sim_time() + remaining_time, TAIL_FLIT_INTRPT_CODE);   Q	} else if(Num_Flits_Left_to_Send > 0 && Dest_Node_Number != rmt_tf_intpt_code) {   O		printf("receive unexpected remote intrpt from node:%d\n", rmt_tf_intpt_code);   	}   	   	FOUT;   }       // generate and enqueue message   void generate_message(void){   	Packet * pkptr;   	// int data_flits_length;   	int gen_Dest_Node_Number;   	int tmp_type;   	int tmp_data;   	   	FIN(generate_message());   	   	do {   F		gen_Dest_Node_Number = (int)op_dist_outcome(Dest_Node_Number_Distr);   2	}while(gen_Dest_Node_Number == This_Node_Number);   	   #	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!   	// gen_Dest_Node_Number = 0;   	   2	if (op_prg_odb_ltrace_active ("DN") == OPC_TRUE){   L		printf("node%d: dest node: %d\n", This_Node_Number, gen_Dest_Node_Number);   	}   	   	// generate destination flit   B	pkptr = generate_flit(Flit_Type_Dest_Addr, gen_Dest_Node_Number);   ,	op_subq_pk_insert(0, pkptr, OPC_QPOS_TAIL);   	   	// source node flit   =	pkptr = generate_flit(Flit_Type_Src_Addr, This_Node_Number);   ,	op_subq_pk_insert(0, pkptr, OPC_QPOS_TAIL);   	   	// worm length flit   5	Worm_Gen_Len = (int)op_dist_outcome(Worm_Len_Distr);   P	// data_flits_length = Worm_Gen_Len - 4; // length including 3 heads and 1 tail   <	pkptr = generate_flit(Flit_Type_Worm_Length, Worm_Gen_Len);   ,	op_subq_pk_insert(0, pkptr, OPC_QPOS_TAIL);   	   	/*   =	data flits, for faster simulation, data flits and tail flit    =	are generated in real time rather than putting them in queue   	*/   	/*   3	for(;data_flits_length > 0; data_flits_length--) {   @		pkptr = generate_flit(Flit_Type_Data_Flit, data_flits_length);   -		op_subq_pk_insert(0, pkptr, OPC_QPOS_TAIL);   	}   	   	// Tail Flit   9	pkptr = generate_flit(Flit_Type_Tail, This_Node_Number);   ,	op_subq_pk_insert(0, pkptr, OPC_QPOS_TAIL);   	*/   	   	Worm_Counter++;   	   9	if (op_prg_odb_ltrace_active ("SelfIntr3") == OPC_TRUE){   =		printf("node%d: new worm  Qsize: %f  Worm_Counter: %d \n",    G			This_Node_Number, op_subq_stat (0, OPC_QSTAT_PKSIZE), Worm_Counter);   	}   	   1	if (op_prg_odb_ltrace_active ("Q") == OPC_TRUE){   		while(pkptr != NULL) {   +			op_pk_nfd_get(pkptr, "type", &tmp_type);   +			op_pk_nfd_get(pkptr, "data", &tmp_data);   R			printf("pk@node%d: type:%d  data: %d\n", This_Node_Number, tmp_type, tmp_data);   			pkptr = get_pk_in_queue();   		}   	}   	   	/*    	check whether to send to router   F	when finish generating a message, get ready to send the message head.   G	only send the head to the router as other ports. If the port is free,    $	then starting sending whole message   	*/       ,	if(Being_Trans_or_Not == Not_Being_Trans) {   		pkptr = get_pk_in_queue();   		if(pkptr != NULL) {   $			Being_Trans_or_Not = Being_Trans;   +			Dest_Node_Number = gen_Dest_Node_Number;   ,			op_pk_send(pkptr, PGQ_to_Router_Channel);   -			Num_Flits_Left_to_Send = Worm_Gen_Len - 1;   			   K			// if the dest node head flit is sent, the tail can't be sent right now.   "			Sent_TF_or_not = Didnt_Send_TF;   		}   	}   	   	// schedule the next message   <	Inter_Arrival_Time = op_dist_outcome(Inter_Arrival_Distr);	   T	op_intrpt_schedule_self (op_sim_time () + Inter_Arrival_Time, GEN_MSG_INTRPT_CODE);       	   	FOUT;   }       void send_next_flit(){   	Packet * pkptr;   	   	int tmp_data;   	int tmp_type;   	   	FIN(send_next_flit());       U	// when the tail flit has been sent, need to start sending a new message if possible   #	if(Sent_TF_or_not == Did_Send_TF){   !		Sent_TF_or_not = Didnt_Send_TF;   '		Being_Trans_or_Not = Not_Being_Trans;   		   H		// if there are other worms left in the queue, then start sending them   		if(Worm_Counter > 0){   			pkptr = get_pk_in_queue();   			if (!pkptr) {   ,				op_pk_nfd_get(pkptr, "type", &tmp_type);   ,				op_pk_nfd_get(pkptr, "data", &tmp_data);   -				op_pk_send(pkptr, PGQ_to_Router_Channel);    				Dest_Node_Number = tmp_data;   				   .				Num_Flits_Left_to_Send = Worm_Gen_Len - 1;   %				Being_Trans_or_Not = Being_Trans;   			}   		}   	}    	   /	// if the worm is in processing, then continue   	else if (Worm_Counter > 0){   Z		// if the next flit is source head flit or worm length flit, then extract from the queue   _		if(Num_Flits_Left_to_Send == Worm_Gen_Len - 1 || Num_Flits_Left_to_Send == Worm_Gen_Len - 2){   			pkptr = get_pk_in_queue();   ,			op_pk_send(pkptr, PGQ_to_Router_Channel);   			Num_Flits_Left_to_Send--;   		}   	   C		//when the scheduled interrupt times out, send the tail flit here   :		//Once receive remote intrpt of schedul_TF, send TF ASAP   9		else if(Num_Flits_Left_to_Send == TF_Sch_TimeOut_TRUE){   O			// only left the tail, and only triggered when received the remote interrupt   /			if (TF_Sch_TimeOut == TF_Sch_TimeOut_TRUE) {   				Worm_Counter--;   E				// after sending the tail flit, the transmission shall be hang up   )				Being_Trans_or_Not = Not_Being_Trans;   				Num_Flits_Left_to_Send = 0;   !				Sent_TF_or_not = Did_Send_TF;   			   1				// currently, there's no node to send worm to   				Dest_Node_Number = -1;       R				// the tail flit is generated here, not in the queue, in order to save the RAM   <				pkptr = generate_flit(Flit_Type_Tail, This_Node_Number);   -				op_pk_send(pkptr, PGQ_to_Router_Channel);   			}   		}    		   H		//generate the data flit here, this is a heuristic way for simulation,   H		//actually data flits are supposed to be read from the queue, however,   P		//in order to improve the performance of simulator, we generate the data flits   6		//here. and that's the same thing for the tail flit.   		else{   			Num_Flits_Left_to_Send--;   F			pkptr = generate_flit(Flit_Type_Data_Flit, Num_Flits_Left_to_Send);   ,			op_pk_send(pkptr, PGQ_to_Router_Channel);   		}   	}   	   /*   0	pkptr = generate_flit(Flit_Type_Data_Flit, 22);   *	op_pk_send(pkptr, PGQ_to_Router_Channel);   	*/   	FOUT;   }       void read_Rnode_file(void){       	Objid myObjid, parentObjid;   	char parentname[64];   	char RName[128];   	FILE * Rinfile;   	char lbuf[128];   	char * nToken;   	   	FIN(read_Rnode_file());   	   	// get the local node number   	myObjid = op_id_self();   (	parentObjid = op_topo_parent (myObjid);   8	op_ima_obj_attr_get (parentObjid, "name", &parentname);   )	This_Node_Number = atoi(&parentname[5]);   	   #	// get the routing table file path   ^	sprintf(RName, "C:\\Users\\chenyua\\OPNET_Project\\WH_G4x4\\Rnode_%d.txt", This_Node_Number);   &	if (!(Rinfile = fopen(RName, "r"))) {   0		printf("initialize_gen: could not find file");   		exit(-2);   	}   	   	fgets(lbuf, 127, Rinfile);   	nToken = strtok(lbuf, " ");   	Total_Node_Num = atoi(nToken);   	fclose(Rinfile);   	   7	if (op_prg_odb_ltrace_active ("PGQFILE") == OPC_TRUE){   &		printf("FILE %d\n", Total_Node_Num);   :		printf("node: %s, %d\n", parentname , This_Node_Number);   	}   	   	FOUT;   }       void initialize_PGQ(void){       	Objid myObjid;   	   	FIN(initialize_PGQ());   	// initialize self parameters   	read_Rnode_file();   	   	myObjid = op_id_self();   	   )	// clean number of messages in the queue   	Worm_Counter = 0;   	   5	// generate the destination node number distribution   M	Dest_Node_Number_Distr = op_dist_load("uniform_int", 0, Total_Node_Num - 1);       '	// get the model attribution variable    @	op_ima_obj_attr_get (myObjid, "Worm_Gen_Rate", &Worm_Gen_Rate);   .	Avr_Inter_Arrival_time = 1.0 / Worm_Gen_Rate;   Q	Inter_Arrival_Distr = op_dist_load ("exponential", Avr_Inter_Arrival_time, 0.0);   ;	Inter_Arrival_Time = op_dist_outcome(Inter_Arrival_Distr);   	   	// worm or message length   B	op_ima_obj_attr_get (myObjid, "Worm_Gen_Len", &Ave_Worm_Gen_Len);   R	Worm_Len_Distr = op_dist_load("uniform_int", Ave_Worm_Gen_Len, Ave_Worm_Gen_Len);   	   3	if (op_prg_odb_ltrace_active ("iat") == OPC_TRUE){   9		printf("Inter_Arrival_Time: %f\n", Inter_Arrival_Time);   	}   	   	//if (This_Node_Number == 0)   U	op_intrpt_schedule_self (op_sim_time () +  Inter_Arrival_Time, GEN_MSG_INTRPT_CODE);   		   	FOUT;   }       void PGQ_handle_time_out(void){       	int intrpt_code;   	// Packet * pkptr;   	   	FIN(PGQ_handle_time_out());   	    	intrpt_code = op_intrpt_code();   	   	switch(intrpt_code) {   		//tail flit is transmitted   		case TAIL_FLIT_INTRPT_CODE:    :		if (op_prg_odb_ltrace_active ("SelfIntr1") == OPC_TRUE){   K			printf("node%d: self intrpt TAIL_FLIT_INTRPT_CODE\n", This_Node_Number);   		}   ;		// Once receive remote intrpt of schedul_TF, send TF ASAP   '		TF_Sch_TimeOut = TF_Sch_TimeOut_TRUE;   		Num_Flits_Left_to_Send = 1;   		send_next_flit();   		break;   		   		// generating another message   		case GEN_MSG_INTRPT_CODE:    :		if (op_prg_odb_ltrace_active ("SelfIntr2") == OPC_TRUE){   I			printf("node%d: self intrpt GEN_MSG_INTRPT_CODE\n", This_Node_Number);   		}   		generate_message();   		break;   		   
		default:   		break;   	}   	   	FOUT;   }                                                                                                                          �  �          
   init   
       
          initialize_PGQ();   
                     
   ����   
          pr_state        �  �          
   idle   
                     
          
           ����             pr_state                       0  �      �  �  �  �          
   tr_0   
       ����          ����          
    ����   
          ����                       pr_transition              J   �     �  t  �   �  �  K  �  �          
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
          ����                       pr_transition              5  .     �  �  �  '    �  �  �          
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