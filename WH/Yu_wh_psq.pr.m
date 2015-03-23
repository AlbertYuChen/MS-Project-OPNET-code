MIL_3_Tfile_Hdr_ 171A 171A modeler 9 5508581A 550F5B6C 77 ECE-PHO305-01 chenyua 0 0 none none 0 0 none 9F071FC2 1C05 0 0 0 0 0 0 2b1a 1                                                                                                                                                                                                                                                                                                                                                                                         ��g�      D   �   �  �  �  �  �  �  �  �  �  �    s        ACK   �������   ����               ����              ����              ����           �Z                 	   begsim intrpt         
   ����   
   doc file            	nd_module      endsim intrpt             ����      failure intrpts            disabled      intrpt interval         ԲI�%��}����      priority              ����      recovery intrpts            disabled      subqueue                     count    ���   
   ����   
      list   	���   
          
      super priority             ����             Stathandle	\ete_gsh;       int	\This_Node_Number;       int	\Total_Node_Num;       Stathandle	\flit_stat_handler;       double	\flit_counter;       #Stathandle	\PGQ_router_handshaking;       int	\Comming_Node_Number;              // include files   #include <stdio.h>   #include <stdlib.h>   #include <math.h>   #include <string.h>   #include <direct.h>       // define arrival signal here   5#define ARRIVAL (op_intrpt_type() == OPC_INTRPT_STRM)       &// define flit receiving function here   .#define RECEIVE_FLIT receive_flit_in_squeue();           // define flit type here   #define Flit_Type_Data_Flit 0   #define Flit_Type_Dest_Addr 1   #define Flit_Type_Src_Addr 2   #define Flit_Type_Worm_Length 6   #define Flit_Type_Tail 7           &// definition of statistics parameters   !#define DIFFERENCE_THRESHOULD 0.1   �       void initialize_PSQ(void) {   	Objid myObjid, parentObjid;   	char parentname[64];   	char RName[128];   	FILE * Rinfile;   	char lbuf[128];   	char * nToken;   	   	FIN(initialize_PSQ());   	//   	flit_counter = 0;   	   	// get the local node number   	myObjid = op_id_self();   (	parentObjid = op_topo_parent (myObjid);   8	op_ima_obj_attr_get (parentObjid, "name", &parentname);   )	This_Node_Number = atoi(&parentname[5]);   	   #	// get the routing table file path   Z	sprintf(RName, "C:\\Users\\chenyua\\OPNET_Project\\WH\\ARnode_%d.txt", This_Node_Number);   &	if (!(Rinfile = fopen(RName, "r"))) {   0		printf("initialize_gen: could not find file");   		exit(-2);   	}   	   	fgets(lbuf, 127, Rinfile);    	nToken = strtok(lbuf, " \t\n");   	Total_Node_Num = atoi(nToken);   	fclose(Rinfile);   	   7	if (op_prg_odb_ltrace_active ("PSQFILE") == OPC_TRUE){   N		printf("PSQ: node %d  total nodes: %d\n", This_Node_Number, Total_Node_Num);   	}   	   	   	FOUT;   }       void discard_rcv_queue(void){   	   	Packet * pkptr;   	int flit_type;   	FIN(discard_rcv_queue());   	   	do {   		if (!op_subq_empty (0)){   /			pkptr = op_subq_pk_remove(0, OPC_QPOS_HEAD);   8			flit_type = op_pk_nfd_get(pkptr, "type", &flit_type);   			op_pk_destroy(pkptr);   			} else{   				break;   		}   '	} while (flit_type != Flit_Type_Tail);   		   	FOUT;   }       *void deal_with_statistics(Packet * pkptr){   	double ete_delay;   	   	FIN(deal_with_statistics());   >	ete_delay = op_sim_time () - op_pk_creation_time_get (pkptr);   '    op_stat_write (ete_gsh, ete_delay);   	   g	// accumulate the cumulative averrage, if the difference is within threshould, then end the simulation       	FOUT;   }       "void receive_flit_in_squeue(void){   	Packet * pkptr;   
	int type;   
	int data;       //	Objid rem_node_objid;   //	Objid rem_queue_objid;   //	char source_node_name[64];   	   	FIN(receive_flit_in_squeue());   	   %	pkptr = op_pk_get(op_intrpt_strm());   	   #	// insert into the receiving queue   /	// op_subq_pk_insert(0, pkptr, OPC_QPOS_TAIL);       %	op_pk_nfd_get(pkptr, "type", &type);   %	op_pk_nfd_get(pkptr, "data", &data);   	   7	if (op_prg_odb_ltrace_active ("PSQ_RCV") == OPC_TRUE){   J		printf("PSQ: node %d type:%d  data:%d\n", This_Node_Number, type, data);   	}   	   	   	switch(type){   S		// if the flit is destination address flit, then wait for the source head flit to   G		// trigger a remote intrrupt to the source node, trigger DEST_ARRIVAL   		case Flit_Type_Dest_Addr:   $		//if( data !=  This_Node_Number) {   5		//	printf("ERROR: PSQ receive_flit_in_squeue()\n");   		//}   		break;   		   Q		// if the flit contains source address, then trigger remote intrrupt to PGQueue   		// of the source node   		case Flit_Type_Src_Addr:   		/*   		//Comming_Node_Number = data;   		Comming_Node_Number = 0;   <		sprintf(source_node_name, "node_%d", Comming_Node_Number);   O		rem_node_objid = op_id_from_name (0, OPC_OBJTYPE_NODE_FIX, source_node_name);   S		rem_queue_objid = op_id_from_name (rem_node_objid, OPC_OBJTYPE_QUEUE, "PGQueue");   N		op_intrpt_schedule_remote(op_sim_time(), This_Node_Number, rem_queue_objid);   7		printf("Worm comes from @%d\n", Comming_Node_Number);   		*/   		break;   		   		case Flit_Type_Worm_Length:   		   		   		break;   		   		// data flit   		case Flit_Type_Data_Flit:   		break;   		   I		// if the flit is a tail, then discard the flits in the receiving queue   		// get teh statistics data   		case Flit_Type_Tail:   !		// deal_with_statistics(pkptr);   		// discard_rcv_queue();   		break;   		   
		default:   I		printf("ERROR@node%d: PSQ receive_flit_in_squeue\n", This_Node_Number);   	}	   	   	flit_counter++;   6	op_stat_write (PGQ_router_handshaking, flit_counter);   	   	printf("op_stat_write\n");   	   	FOUT;   }                                                                                                                             �   �          
   init   
       
      initialize_PSQ();       Jete_gsh = op_stat_reg ("ETE Delay", OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);       RPGQ_router_handshaking = op_stat_reg ("ACK", OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);   
                     
   ����   
          pr_state        �   �          
   idle   
                     
       
           ����             pr_state                       �      �   �     �          
   tr_3   
       ����          ����          
    ����   
          ����                       pr_transition              �   U     s   �  3   o  �   p  �   �          
   tr_4   
       
   default   
       ����          
    ����   
          ����                       pr_transition              �  c     �   �  �  ?  >  D  w   �          
   tr_5   
       
   ARRIVAL   
       
   RECEIVE_FLIT   
       
    ����   
          ����                       pr_transition                   ACK        ������������        ԲI�%��}      	ETE Delay        ������������        ԲI�%��}                        