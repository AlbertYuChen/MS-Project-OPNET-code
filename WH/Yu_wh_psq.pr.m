MIL_3_Tfile_Hdr_ 171A 171A modeler 9 5508581A 5509C9E1 15 ECE-PHO305-01 chenyua 0 0 none none 0 0 none 34C04BAD 11D7 0 0 0 0 0 0 2b1a 1                                                                                                                                                                                                                                                                                                                                                                                         ��g�      D   H   L      {  �  �  �  �  �  �  �  k  w           	   begsim intrpt             ����      doc file            	nd_module      endsim intrpt             ����      failure intrpts            disabled      intrpt interval         ԲI�%��}����      priority              ����      recovery intrpts            disabled      subqueue                     count    ���   
   ����   
      list   	���   
          
      super priority             ����             Stathandle	\ete_gsh;          int   ev_type, ev_strm;          // define arrival signal here   5#define ARRIVAL (op_intrpt_type() == OPC_INTRPT_STRM)           &// define flit receiving function here   .#define RECEIVE_FLIT receive_flit_in_squeue();           // define flit type here   #define Flit_Type_Data_Flit 0   #define Flit_Type_Dest_Addr 1   #define Flit_Type_Src_Addr 2   #define Flit_Type_Worm_Length 6   #define Flit_Type_Tail 7           /* packet stream definitions */   #define RCV_IN_STRM 0   #define SRC_IN_STRM 1   #define XMT_OUT_STRM 0       /* transition macros */   H#define SRC_ARRVL (ev_type == OPC_INTRPT_STRM && ev_strm == SRC_IN_STRM)   H#define RCV_ARRVL (ev_type == OPC_INTRPT_STRM && ev_strm == RCV_IN_STRM)           U       void discard_rcv_queue(void){   	   	Packet * pkptr;   	int flit_type;   	FIN(discard_rcv_queue());   	   	do {   		if (!op_subq_empty (0)) {   /			pkptr = op_subq_pk_remove(0, OPC_QPOS_HEAD);   8			flit_type = op_pk_nfd_get(pkptr, "type", &flit_type);   			op_pk_destroy(pkptr);   			} else{   				break;   		}   '	} while (flit_type != Flit_Type_Tail);   		   	FOUT;   }           *void deal_with_statistics(Packet * pkptr){   	double ete_delay;   	   	FIN(deal_with_statistics());   >	ete_delay = op_sim_time () - op_pk_creation_time_get (pkptr);   '    op_stat_write (ete_gsh, ete_delay);       	FOUT;   }       "void receive_flit_in_squeue(void){   	Packet * pkptr;   	int flit_type;   	FIN(receive_flit_in_squeue());   	   %	pkptr = op_pk_get(op_intrpt_strm());   	   #	// insert into the receiving queue   ,	op_subq_pk_insert(0, pkptr, OPC_QPOS_TAIL);   	   	// get the receiving flit type   *	op_pk_nfd_get(pkptr, "type", &flit_type);   	   	   '	if(flit_type == Flit_Type_Dest_Addr) {   3		// if the flit is destination address flit, then    	   	   	   .	} else if (flit_type == Flit_Type_Src_Addr) {   Q		// if the flit contains source address, then trigger remote intrrupt to PGQueue   		// of the source node   	   *	} else if (flit_type == Flit_Type_Tail) {   I		// if the flit is a tail, then discard the flits in the receiving queue   		// get teh statistics data   		deal_with_statistics(pkptr);   		discard_rcv_queue();   	}   	   	   	   	FOUT;   }                                                                                                                             �   �          
   st_4   
       J      Jete_gsh = op_stat_reg ("ETE Delay", OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);   J                     
   ����   
          pr_state        �   �          
   st_5   
                     J      ev_type = op_intrpt_type();   if (ev_type == OPC_INTRPT_STRM)      ev_strm = op_intrpt_strm();                       J           ����             pr_state                       �      �   �     �          
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
          ����                       pr_transition                       	ETE Delay        ������������        ԲI�%��}                        