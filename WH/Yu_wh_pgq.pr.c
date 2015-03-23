/* Process model C form file: Yu_wh_pgq.pr.c */
/* Portions of this file copyright 1986-2011 by OPNET Technologies, Inc. */


/*
 =========================== NOTE ==========================
 This file is automatically generated from Yu_wh_pgq.pr.m
 during a process model compilation.

 Do NOT manually edit this file.
 Manual edits will be lost during the next compilation.
 =========================== NOTE ==========================
*/



/* This variable carries the header into the object file */
const char Yu_wh_pgq_pr_c [] = "MIL_3_Tfile_Hdr_ 171A 30A modeler 7 550F5B49 550F5B49 1 ECE-PHO305-01 chenyua 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 2b1a 1                                                                                                                                                                                                                                                                                                                                                                                                    ";
#include <string.h>



/* OPNET system definitions */
#include <opnet.h>



/* Header Block */

// include files
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <direct.h>

// define packet arrival signal here
#define PK_ARRVL (op_intrpt_type () == OPC_INTRPT_STRM)

// define packet receives at the destination here
#define DEST_REACHED (op_intrpt_type () == OPC_INTRPT_REMOTE)

// get the handshake signale here
#define ROUTER_READY (op_intrpt_type () == OPC_INTRPT_STAT)

// define the time out handler
#define TIME_OUT (op_intrpt_type () == OPC_INTRPT_SELF)

// define of transition functions
#define SEND_NEXT_FLIT send_next_flit();
#define RECEIVE_FLIT receive_flit();
#define HANDLE_TIME_OUT PGQ_handle_time_out();
#define SCHEDULE_TF schedule_tf();

// define flit type here
#define Flit_Type_Data_Flit 0
#define Flit_Type_Dest_Addr 1
#define Flit_Type_Src_Addr 2
#define Flit_Type_Worm_Length 6
#define Flit_Type_Tail 7

// define node number range
#define MIN_NODE_NUMBER 0
#define MAX_NODE_NUMBER 63

// define interrupt code here, self intrrupt start from 100
#define GEN_MSG_INTRPT_CODE 100
#define TAIL_FLIT_INTRPT_CODE 101

// PGQ_Channel_0 0
#define PGQ_to_Router_Channel 0

// Status of Sent_TF_or_not
#define Did_Send_TF 1
#define Didnt_Send_TF 0

// status of Being_Trans_or_Not
#define Being_Trans 1
#define Not_Being_Trans 0

// define FLIT_TRANSTER_TIME
#define FLIT_TRANSTER_TIME 0.00083333333

// define status of TF_Sch_TimeOut
#define TF_Sch_TimeOut_TRUE 1
#define TF_Sch_TimeOut_FALSE 0



/* End of Header Block */

#if !defined (VOSD_NO_FIN)
#undef	BIN
#undef	BOUT
#define	BIN		FIN_LOCAL_FIELD(_op_last_line_passed) = __LINE__ - _op_block_origin;
#define	BOUT	BIN
#define	BINIT	FIN_LOCAL_FIELD(_op_last_line_passed) = 0; _op_block_origin = __LINE__;
#else
#define	BINIT
#endif /* #if !defined (VOSD_NO_FIN) */



/* State variable definitions */
typedef struct
	{
	/* Internal state tracking for FSM */
	FSM_SYS_STATE
	/* State Variables */
	char	                   		Being_Trans_or_Not                              ;
	char	                   		Router_Ready_Statewire                          ;
	char	                   		Sent_TF_or_not                                  ;
	char	                   		TF_Sch_TimeOut                                  ;
	Distribution *	         		Dest_Node_Number_Distr                          ;
	Distribution *	         		Inter_Arrival_Distr                             ;
	Distribution *	         		Worm_Len_Distr                                  ;
	double	                 		Avr_Inter_Arrival_time                          ;
	double	                 		Inter_Arrival_Time                              ;
	double	                 		Worm_Gen_Rate                                   ;
	int	                    		Ave_Worm_Gen_Len                                ;
	int	                    		Dest_Node_Number                                ;
	int	                    		In_Port                                         ;
	int	                    		Num_Flits_Left_to_Send                          ;
	int	                    		Out_Port                                        ;
	int	                    		This_Node_Number                                ;
	int	                    		Total_Node_Num                                  ;
	int	                    		Worm_Counter                                    ;
	int	                    		Worm_Gen_Len                                    ;
	} Yu_wh_pgq_state;

#define Being_Trans_or_Not      		op_sv_ptr->Being_Trans_or_Not
#define Router_Ready_Statewire  		op_sv_ptr->Router_Ready_Statewire
#define Sent_TF_or_not          		op_sv_ptr->Sent_TF_or_not
#define TF_Sch_TimeOut          		op_sv_ptr->TF_Sch_TimeOut
#define Dest_Node_Number_Distr  		op_sv_ptr->Dest_Node_Number_Distr
#define Inter_Arrival_Distr     		op_sv_ptr->Inter_Arrival_Distr
#define Worm_Len_Distr          		op_sv_ptr->Worm_Len_Distr
#define Avr_Inter_Arrival_time  		op_sv_ptr->Avr_Inter_Arrival_time
#define Inter_Arrival_Time      		op_sv_ptr->Inter_Arrival_Time
#define Worm_Gen_Rate           		op_sv_ptr->Worm_Gen_Rate
#define Ave_Worm_Gen_Len        		op_sv_ptr->Ave_Worm_Gen_Len
#define Dest_Node_Number        		op_sv_ptr->Dest_Node_Number
#define In_Port                 		op_sv_ptr->In_Port
#define Num_Flits_Left_to_Send  		op_sv_ptr->Num_Flits_Left_to_Send
#define Out_Port                		op_sv_ptr->Out_Port
#define This_Node_Number        		op_sv_ptr->This_Node_Number
#define Total_Node_Num          		op_sv_ptr->Total_Node_Num
#define Worm_Counter            		op_sv_ptr->Worm_Counter
#define Worm_Gen_Len            		op_sv_ptr->Worm_Gen_Len

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	Yu_wh_pgq_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((Yu_wh_pgq_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


/* Function Block */

#if !defined (VOSD_NO_FIN)
enum { _op_block_origin = __LINE__ + 2};
#endif

Packet * generate_flit(int type, int data){
	Packet * pkptr;
	
	FIN(generate_flit());
	
	pkptr = op_pk_create_fmt("Yu_wh_pkformat");
	op_pk_nfd_set(pkptr, "type", type);
	op_pk_nfd_set(pkptr, "data", data);
	
	FRET(pkptr);
}

Packet * get_pk_in_queue(void) {

	Packet * pkptr;
	
	FIN(get_pk_in_queue());
	if (op_subq_empty(0) == OPC_FALSE){
		pkptr = op_subq_pk_remove(0, OPC_QPOS_HEAD);
	}else {
		pkptr = NULL;
	}
	
	FRET (pkptr);
}

void schedule_tf(void){

	int rmt_tf_intpt_code;
	double remaining_time;

	FIN(schedule_tf());
	
	rmt_tf_intpt_code = op_intrpt_code();
	
	// when the head arrives the destination
	if (Num_Flits_Left_to_Send > 0 && Dest_Node_Number == rmt_tf_intpt_code) {
		remaining_time = Num_Flits_Left_to_Send * FLIT_TRANSTER_TIME;
		TF_Sch_TimeOut = TF_Sch_TimeOut_FALSE;
		op_intrpt_schedule_self(op_sim_time() + remaining_time, TAIL_FLIT_INTRPT_CODE);
	} else if(Num_Flits_Left_to_Send > 0 && Dest_Node_Number != rmt_tf_intpt_code) {
		printf("receive unexpected remote intrpt from node:%d\n", rmt_tf_intpt_code);
	}
	
	FOUT;
}

// generate and enqueue message
void generate_message(void){
	Packet * pkptr;
	int data_flits_length;
	int gen_Dest_Node_Number;
	int tmp_type;
	int tmp_data;
	
	FIN(generate_message());
	
	do {
		gen_Dest_Node_Number = (int)op_dist_outcome(Dest_Node_Number_Distr);
	}while(gen_Dest_Node_Number == This_Node_Number);
	
	if (op_prg_odb_ltrace_active ("DN") == OPC_TRUE){
		printf("node%d: dest node: %d\n", This_Node_Number, gen_Dest_Node_Number);
	}
	
	// generate destination flit
	pkptr = generate_flit(Flit_Type_Dest_Addr, gen_Dest_Node_Number);
	op_subq_pk_insert(0, pkptr, OPC_QPOS_TAIL);
	
	// source node flit
	pkptr = generate_flit(Flit_Type_Src_Addr, This_Node_Number);
	op_subq_pk_insert(0, pkptr, OPC_QPOS_TAIL);
	
	// worm length flit
	Worm_Gen_Len = (int)op_dist_outcome(Worm_Len_Distr);
	data_flits_length = Worm_Gen_Len - 4; // length including 3 heads and 1 tail
	pkptr = generate_flit(Flit_Type_Worm_Length, Worm_Gen_Len);
	op_subq_pk_insert(0, pkptr, OPC_QPOS_TAIL);
	
	/*
	data flits, for faster simulation, data flits and tail flit 
	are generated in real time rather than putting them in queue
	*/
	/*
	for(;data_flits_length > 0; data_flits_length--) {
		pkptr = generate_flit(Flit_Type_Data_Flit, data_flits_length);
		op_subq_pk_insert(0, pkptr, OPC_QPOS_TAIL);
	}
	
	// Tail Flit
	pkptr = generate_flit(Flit_Type_Tail, This_Node_Number);
	op_subq_pk_insert(0, pkptr, OPC_QPOS_TAIL);
	*/
	
	Worm_Counter++;
	
	if (op_prg_odb_ltrace_active ("SelfIntr3") == OPC_TRUE){
		printf("node%d: new worm  Qsize: %f  Worm_Counter: %d \n", 
			This_Node_Number, op_subq_stat (0, OPC_QSTAT_PKSIZE), Worm_Counter);
	}
	
	if (op_prg_odb_ltrace_active ("Q") == OPC_TRUE){
		while(pkptr != NULL) {
			op_pk_nfd_get(pkptr, "type", &tmp_type);
			op_pk_nfd_get(pkptr, "data", &tmp_data);
			printf("pk@node%d: type:%d  data: %d\n", This_Node_Number, tmp_type, tmp_data);
			pkptr = get_pk_in_queue();
		}
	}
	
	/*
	check whether to send to router
	when finish generating a message, get ready to send the message head.
	only send the head to the router as other ports. If the port is free, 
	then starting sending whole message
	*/

	if(Being_Trans_or_Not == Not_Being_Trans) {
		pkptr = get_pk_in_queue();
		if(pkptr != NULL) {
			Being_Trans_or_Not = Being_Trans;
			Dest_Node_Number = gen_Dest_Node_Number;
			op_pk_send(pkptr, PGQ_to_Router_Channel);
			Num_Flits_Left_to_Send = Worm_Gen_Len - 1;
			
			// if the dest node head flit is sent, the tail can't be sent right now.
			Sent_TF_or_not = Didnt_Send_TF;
		}
	}
	
	// schedule the next message
	Inter_Arrival_Time = op_dist_outcome(Inter_Arrival_Distr);	
	op_intrpt_schedule_self (op_sim_time () + Inter_Arrival_Time, GEN_MSG_INTRPT_CODE);

	
	FOUT;
}

void send_next_flit(){
	Packet * pkptr;
	
	int tmp_data;
	int tmp_type;
	
	FIN(send_next_flit());

	// when the tail flit has been sent, need to start sending a new message if possible
	if(Sent_TF_or_not == Did_Send_TF){
		Sent_TF_or_not = Didnt_Send_TF;
		Being_Trans_or_Not = Not_Being_Trans;
		
		// if there are other worms left in the queue, then start sending them
		if(Worm_Counter > 0){
			pkptr = get_pk_in_queue();
			if (!pkptr) {
				op_pk_nfd_get(pkptr, "type", &tmp_type);
				op_pk_nfd_get(pkptr, "data", &tmp_data);
				op_pk_send(pkptr, PGQ_to_Router_Channel);
				Dest_Node_Number = tmp_data;
				
				Num_Flits_Left_to_Send = Worm_Gen_Len - 1;
				Being_Trans_or_Not = Being_Trans;
			}
		}
	} 
	
	// if the worm is in processing, then continue
	else if (Worm_Counter > 0){
		// if the next flit is source head flit or worm length flit, then extract from the queue
		if(Num_Flits_Left_to_Send == Worm_Gen_Len - 1 || Num_Flits_Left_to_Send == Worm_Gen_Len - 2){
			pkptr = get_pk_in_queue();
			op_pk_send(pkptr, PGQ_to_Router_Channel);
			Num_Flits_Left_to_Send--;
		}
	

		//when the scheduled interrupt times out, send the tail flit here
		//Once receive remote intrpt of schedul_TF, send TF ASAP


		else if(Num_Flits_Left_to_Send == TF_Sch_TimeOut_TRUE){
			// only left the tail, and only triggered when received the remote interrupt
			if (TF_Sch_TimeOut == TF_Sch_TimeOut_TRUE) {
				Worm_Counter--;
				// after sending the tail flit, the transmission shall be hang up
				Being_Trans_or_Not = Not_Being_Trans;
				Num_Flits_Left_to_Send = 0;
				Sent_TF_or_not = Did_Send_TF;
			
				// currently, there's no node to send worm to
				Dest_Node_Number = -1;

				// the tail flit is generated here, not in the queue, in order to save the RAM
				pkptr = generate_flit(Flit_Type_Tail, This_Node_Number);
				op_pk_send(pkptr, PGQ_to_Router_Channel);
			}
		} 

	
		
		//generate the data flit here, this is a heuristic way for simulation,
		//actually data flits are supposed to be read from the queue, however,
		//in order to improve the performance of simulator, we generate the data flits
		//here. and that's the same thing for the tail flit.
	

		else{
			Num_Flits_Left_to_Send--;
			pkptr = generate_flit(Flit_Type_Data_Flit, Num_Flits_Left_to_Send);
			op_pk_send(pkptr, PGQ_to_Router_Channel);
		}
	}
	
/*
	pkptr = generate_flit(Flit_Type_Data_Flit, 22);
	op_pk_send(pkptr, PGQ_to_Router_Channel);
	*/
	FOUT;
}

void initialize_PGQ(void){

	Objid myObjid, parentObjid;
	char parentname[64];
	char RName[128];
	FILE * Rinfile;
	char lbuf[128];
	char * nToken;

	// int i;
	double TEST_ave = 0.0;
	
	FIN(initialize_PGQ());
	// clean number of messages in the queue
	Worm_Counter = 0;
	
	// get the local node number
	myObjid = op_id_self();
	parentObjid = op_topo_parent (myObjid);
	op_ima_obj_attr_get (parentObjid, "name", &parentname);
	This_Node_Number = atoi(&parentname[5]);
	
	// get the routing table file path
	sprintf(RName, "C:\\Users\\chenyua\\OPNET_Project\\WH\\ARnode_%d.txt", This_Node_Number);
	if (!(Rinfile = fopen(RName, "r"))) {
		printf("initialize_gen: could not find file");
		exit(-2);
	}
	
	fgets(lbuf, 127, Rinfile);
	nToken = strtok(lbuf, " \t\n");
	Total_Node_Num = atoi(nToken);
	fclose(Rinfile);
	
	if (op_prg_odb_ltrace_active ("PGQFILE") == OPC_TRUE){
		printf("FILE %d\n", Total_Node_Num);
		printf("node: %s, %d\n", parentname , This_Node_Number);
	}
	
	// generate the destination node number distribution
	Dest_Node_Number_Distr = op_dist_load("uniform_int", 0, Total_Node_Num - 1);

	// get the model attribution variable 
	op_ima_obj_attr_get (myObjid, "Worm_Gen_Rate", &Worm_Gen_Rate);
	Avr_Inter_Arrival_time = 1.0 / Worm_Gen_Rate;
	Inter_Arrival_Distr = op_dist_load ("exponential", Avr_Inter_Arrival_time, 0.0);
	Inter_Arrival_Time = op_dist_outcome(Inter_Arrival_Distr);
	
	// worm or message length
	op_ima_obj_attr_get (myObjid, "Worm_Gen_Len", &Ave_Worm_Gen_Len);
	Worm_Len_Distr = op_dist_load("uniform_int", Ave_Worm_Gen_Len, Ave_Worm_Gen_Len);
	
	if (op_prg_odb_ltrace_active ("iat") == OPC_TRUE){
		printf("Inter_Arrival_Time: %f\n", Inter_Arrival_Time);
	}
	
	//if (This_Node_Number == 0)
	op_intrpt_schedule_self (op_sim_time () +  Inter_Arrival_Time, GEN_MSG_INTRPT_CODE);
		
	FOUT;
}

void PGQ_handle_time_out(void){

	int intrpt_code;
	// Packet * pkptr;
	
	FIN(PGQ_handle_time_out());
	
	intrpt_code = op_intrpt_code();
	
	switch(intrpt_code) {
		//tail flit is transmitted
		case TAIL_FLIT_INTRPT_CODE: 
		if (op_prg_odb_ltrace_active ("SelfIntr1") == OPC_TRUE){
			printf("node%d: self intrpt TAIL_FLIT_INTRPT_CODE\n", This_Node_Number);
		}
		// Once receive remote intrpt of schedul_TF, send TF ASAP
		TF_Sch_TimeOut = TF_Sch_TimeOut_TRUE;
		Num_Flits_Left_to_Send = 1;
		send_next_flit();
		break;
		
		// generating another message
		case GEN_MSG_INTRPT_CODE: 
		if (op_prg_odb_ltrace_active ("SelfIntr2") == OPC_TRUE){
			printf("node%d: self intrpt GEN_MSG_INTRPT_CODE\n", This_Node_Number);
		}
		generate_message();
		break;
		
		default:
		break;
	}
	
	FOUT;
}




















/* End of Function Block */

/* Undefine optional tracing in FIN/FOUT/FRET */
/* The FSM has its own tracing code and the other */
/* functions should not have any tracing.		  */
#undef FIN_TRACING
#define FIN_TRACING

#undef FOUTRET_TRACING
#define FOUTRET_TRACING

#if defined (__cplusplus)
extern "C" {
#endif
	void Yu_wh_pgq (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_Yu_wh_pgq_init (int * init_block_ptr);
	void _op_Yu_wh_pgq_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_Yu_wh_pgq_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_Yu_wh_pgq_alloc (VosT_Obtype, int);
	void _op_Yu_wh_pgq_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
Yu_wh_pgq (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (Yu_wh_pgq ());

		{


		FSM_ENTER ("Yu_wh_pgq")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (init) enter executives **/
			FSM_STATE_ENTER_FORCED_NOLABEL (0, "init", "Yu_wh_pgq [init enter execs]")
				FSM_PROFILE_SECTION_IN ("Yu_wh_pgq [init enter execs]", state0_enter_exec)
				{
				
				initialize_PGQ();
				}
				FSM_PROFILE_SECTION_OUT (state0_enter_exec)

			/** state (init) exit executives **/
			FSM_STATE_EXIT_FORCED (0, "init", "Yu_wh_pgq [init exit execs]")


			/** state (init) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "init", "idle", "tr_0", "Yu_wh_pgq [init -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (idle) enter executives **/
			FSM_STATE_ENTER_UNFORCED (1, "idle", state1_enter_exec, "Yu_wh_pgq [idle enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (3,"Yu_wh_pgq")


			/** state (idle) exit executives **/
			FSM_STATE_EXIT_UNFORCED (1, "idle", "Yu_wh_pgq [idle exit execs]")
				FSM_PROFILE_SECTION_IN ("Yu_wh_pgq [idle exit execs]", state1_exit_exec)
				{
				
				}
				FSM_PROFILE_SECTION_OUT (state1_exit_exec)


			/** state (idle) transition processing **/
			FSM_PROFILE_SECTION_IN ("Yu_wh_pgq [idle trans conditions]", state1_trans_conds)
			FSM_INIT_COND (ROUTER_READY)
			FSM_TEST_COND (DEST_REACHED)
			FSM_TEST_COND (TIME_OUT)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("idle")
			FSM_PROFILE_SECTION_OUT (state1_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 1, state1_enter_exec, SEND_NEXT_FLIT;, "ROUTER_READY", "SEND_NEXT_FLIT", "idle", "idle", "tr_2", "Yu_wh_pgq [idle -> idle : ROUTER_READY / SEND_NEXT_FLIT]")
				FSM_CASE_TRANSIT (1, 1, state1_enter_exec, SCHEDULE_TF;, "DEST_REACHED", "SCHEDULE_TF", "idle", "idle", "tr_3", "Yu_wh_pgq [idle -> idle : DEST_REACHED / SCHEDULE_TF]")
				FSM_CASE_TRANSIT (2, 1, state1_enter_exec, HANDLE_TIME_OUT;, "TIME_OUT", "HANDLE_TIME_OUT", "idle", "idle", "tr_4", "Yu_wh_pgq [idle -> idle : TIME_OUT / HANDLE_TIME_OUT]")
				FSM_CASE_TRANSIT (3, 1, state1_enter_exec, ;, "default", "", "idle", "idle", "tr_5", "Yu_wh_pgq [idle -> idle : default / ]")
				}
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (0,"Yu_wh_pgq")
		}
	}




void
_op_Yu_wh_pgq_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_Yu_wh_pgq_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_Yu_wh_pgq_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_Yu_wh_pgq_svar function. */
#undef Being_Trans_or_Not
#undef Router_Ready_Statewire
#undef Sent_TF_or_not
#undef TF_Sch_TimeOut
#undef Dest_Node_Number_Distr
#undef Inter_Arrival_Distr
#undef Worm_Len_Distr
#undef Avr_Inter_Arrival_time
#undef Inter_Arrival_Time
#undef Worm_Gen_Rate
#undef Ave_Worm_Gen_Len
#undef Dest_Node_Number
#undef In_Port
#undef Num_Flits_Left_to_Send
#undef Out_Port
#undef This_Node_Number
#undef Total_Node_Num
#undef Worm_Counter
#undef Worm_Gen_Len

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_Yu_wh_pgq_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_Yu_wh_pgq_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (Yu_wh_pgq)",
		sizeof (Yu_wh_pgq_state));
	*init_block_ptr = 0;

	FRET (obtype)
	}

VosT_Address
_op_Yu_wh_pgq_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	Yu_wh_pgq_state * ptr;
	FIN_MT (_op_Yu_wh_pgq_alloc (obtype))

	ptr = (Yu_wh_pgq_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "Yu_wh_pgq [init enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_Yu_wh_pgq_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	Yu_wh_pgq_state		*prs_ptr;

	FIN_MT (_op_Yu_wh_pgq_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (Yu_wh_pgq_state *)gen_ptr;

	if (strcmp ("Being_Trans_or_Not" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Being_Trans_or_Not);
		FOUT
		}
	if (strcmp ("Router_Ready_Statewire" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Router_Ready_Statewire);
		FOUT
		}
	if (strcmp ("Sent_TF_or_not" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Sent_TF_or_not);
		FOUT
		}
	if (strcmp ("TF_Sch_TimeOut" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->TF_Sch_TimeOut);
		FOUT
		}
	if (strcmp ("Dest_Node_Number_Distr" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Dest_Node_Number_Distr);
		FOUT
		}
	if (strcmp ("Inter_Arrival_Distr" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Inter_Arrival_Distr);
		FOUT
		}
	if (strcmp ("Worm_Len_Distr" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Worm_Len_Distr);
		FOUT
		}
	if (strcmp ("Avr_Inter_Arrival_time" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Avr_Inter_Arrival_time);
		FOUT
		}
	if (strcmp ("Inter_Arrival_Time" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Inter_Arrival_Time);
		FOUT
		}
	if (strcmp ("Worm_Gen_Rate" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Worm_Gen_Rate);
		FOUT
		}
	if (strcmp ("Ave_Worm_Gen_Len" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Ave_Worm_Gen_Len);
		FOUT
		}
	if (strcmp ("Dest_Node_Number" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Dest_Node_Number);
		FOUT
		}
	if (strcmp ("In_Port" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->In_Port);
		FOUT
		}
	if (strcmp ("Num_Flits_Left_to_Send" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Num_Flits_Left_to_Send);
		FOUT
		}
	if (strcmp ("Out_Port" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Out_Port);
		FOUT
		}
	if (strcmp ("This_Node_Number" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->This_Node_Number);
		FOUT
		}
	if (strcmp ("Total_Node_Num" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Total_Node_Num);
		FOUT
		}
	if (strcmp ("Worm_Counter" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Worm_Counter);
		FOUT
		}
	if (strcmp ("Worm_Gen_Len" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Worm_Gen_Len);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}

