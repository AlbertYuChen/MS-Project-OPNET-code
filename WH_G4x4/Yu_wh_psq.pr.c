/* Process model C form file: Yu_wh_psq.pr.c */
/* Portions of this file copyright 1986-2011 by OPNET Technologies, Inc. */


/*
 =========================== NOTE ==========================
 This file is automatically generated from Yu_wh_psq.pr.m
 during a process model compilation.

 Do NOT manually edit this file.
 Manual edits will be lost during the next compilation.
 =========================== NOTE ==========================
*/



/* This variable carries the header into the object file */
const char Yu_wh_psq_pr_c [] = "MIL_3_Tfile_Hdr_ 171A 30A op_runsim_dev 7 551D74A8 551D74A8 1 ECE-PHO309-01 chenyua 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 2b1a 1                                                                                                                                                                                                                                                                                                                                                                                              ";
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

// define arrival signal here
#define ARRIVAL (op_intrpt_type() == OPC_INTRPT_STRM)

// define flit receiving function here
#define RECEIVE_FLIT receive_flit_in_squeue();


// define flit type here
#define Flit_Type_Data_Flit 0
#define Flit_Type_Dest_Addr 1
#define Flit_Type_Src_Addr 2
#define Flit_Type_Worm_Length 6
#define Flit_Type_Tail 7


// definition of statistics parameters
#define DIFFERENCE_THRESHOULD 0.1

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
	Stathandle	             		ete_gsh                                         ;
	int	                    		This_Node_Number                                ;
	int	                    		Total_Node_Num                                  ;
	Stathandle	             		flit_stat_handler                               ;
	double	                 		flit_counter                                    ;
	Stathandle	             		PGQ_router_handshaking                          ;
	int	                    		Comming_Node_Number                             ;
	double	                 		Worm_Create_Time                                ;
	} Yu_wh_psq_state;

#define ete_gsh                 		op_sv_ptr->ete_gsh
#define This_Node_Number        		op_sv_ptr->This_Node_Number
#define Total_Node_Num          		op_sv_ptr->Total_Node_Num
#define flit_stat_handler       		op_sv_ptr->flit_stat_handler
#define flit_counter            		op_sv_ptr->flit_counter
#define PGQ_router_handshaking  		op_sv_ptr->PGQ_router_handshaking
#define Comming_Node_Number     		op_sv_ptr->Comming_Node_Number
#define Worm_Create_Time        		op_sv_ptr->Worm_Create_Time

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	Yu_wh_psq_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((Yu_wh_psq_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


/* Function Block */

#if !defined (VOSD_NO_FIN)
enum { _op_block_origin = __LINE__ + 2};
#endif


void initialize_PSQ(void) {
	Objid myObjid, parentObjid;
	char parentname[64];
	char RName[128];
	FILE * Rinfile;
	char lbuf[128];
	char * nToken;
	
	FIN(initialize_PSQ());
	//
	flit_counter = 0;
	
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
	
	if (op_prg_odb_ltrace_active ("PSQFILE") == OPC_TRUE){
		printf("PSQ: node %d  total nodes: %d\n", This_Node_Number, Total_Node_Num);
	}
	
	
	FOUT;
}

void discard_rcv_queue(void){
	
	Packet * pkptr;
	int flit_type;
	FIN(discard_rcv_queue());
	
	do {
		if (!op_subq_empty (0)){
			pkptr = op_subq_pk_remove(0, OPC_QPOS_HEAD);
			flit_type = op_pk_nfd_get(pkptr, "type", &flit_type);
			op_pk_destroy(pkptr);
		} else{
			break;
		}
	} while (flit_type != Flit_Type_Tail);
		
	FOUT;
}

void deal_with_statistics(Packet * pkptr){
	double ete_delay;
	
	FIN(deal_with_statistics());
	ete_delay = op_sim_time () - op_pk_creation_time_get (pkptr);
    op_stat_write (ete_gsh, ete_delay);
	
	// accumulate the cumulative averrage, if the difference is within threshould, then end the simulation

	FOUT;
}

void receive_flit_in_squeue(void){
	Packet * pkptr;
	int type;
	int data;

	Objid rem_node_objid;
	Objid rem_queue_objid;
	char source_node_name[64];
	
	FIN(receive_flit_in_squeue());
	
	pkptr = op_pk_get(op_intrpt_strm());

	op_pk_nfd_get(pkptr, "type", &type);
	op_pk_nfd_get(pkptr, "data", &data);
	
	if (op_prg_odb_ltrace_active ("PSQ_RCV") == OPC_TRUE){
		printf("PSQ: node %d type:%d  data:%d\n", This_Node_Number, type, data);
	}
	
	switch(type){
		// if the flit is destination address flit, then wait for the source head flit to
		// trigger a remote intrrupt to the source node, trigger DEST_ARRIVAL
		case Flit_Type_Dest_Addr:
		if( data !=  This_Node_Number) {
			printf("ERROR: PSQ receive_flit_in_squeue()\n");
		}
		
		printf("Worm slot: %f\n",  op_pk_creation_time_get (pkptr) - Worm_Create_Time);
		
		Worm_Create_Time = op_pk_creation_time_get (pkptr);
		op_pk_destroy(pkptr);
		break;
		
		
		// if the flit contains source address, then trigger remote intrrupt to PGQueue
		// of the source node
		case Flit_Type_Src_Addr:
	
		Comming_Node_Number = data;
		sprintf(source_node_name, "node_%d", Comming_Node_Number);
		rem_node_objid = op_id_from_name (1, OPC_OBJTYPE_NODE_FIXED, source_node_name);
		printf("comming node:%s  rem_node_objid:%d\n", source_node_name, rem_node_objid);
		
		rem_queue_objid = op_id_from_name (rem_node_objid, OPC_OBJTYPE_QUEUE, "PGQueue");
		op_intrpt_schedule_remote(op_sim_time(), This_Node_Number, rem_queue_objid);
		op_pk_destroy(pkptr);
		break;
		
		case Flit_Type_Worm_Length:
		op_pk_destroy(pkptr);
		break;
		
		
		// data flit
		case Flit_Type_Data_Flit:
		op_pk_destroy(pkptr);
		break;
		
		
		// if the flit is a tail, then discard the flits in the receiving queue
		// get teh statistics data
		case Flit_Type_Tail:
		// deal_with_statistics(pkptr);
		// discard_rcv_queue();
		
		printf("Worm delay @node:%f\n", op_sim_time() - Worm_Create_Time);
		op_stat_write (ete_gsh,	op_sim_time() - Worm_Create_Time);
		op_pk_destroy(pkptr);
		break;
		
		
		default:
		printf("ERROR@node%d: PSQ receive_flit_in_squeue\n", This_Node_Number);
		op_pk_destroy(pkptr);
	}	
	
	flit_counter++;
	op_stat_write (PGQ_router_handshaking, flit_counter);
	
	printf("op_stat_write\n");
	
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
	void Yu_wh_psq (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_Yu_wh_psq_init (int * init_block_ptr);
	void _op_Yu_wh_psq_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_Yu_wh_psq_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_Yu_wh_psq_alloc (VosT_Obtype, int);
	void _op_Yu_wh_psq_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
Yu_wh_psq (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (Yu_wh_psq ());

		{


		FSM_ENTER ("Yu_wh_psq")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (init) enter executives **/
			FSM_STATE_ENTER_FORCED_NOLABEL (0, "init", "Yu_wh_psq [init enter execs]")
				FSM_PROFILE_SECTION_IN ("Yu_wh_psq [init enter execs]", state0_enter_exec)
				{
				initialize_PSQ();
				
				ete_gsh = op_stat_reg ("ETE Delay", OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				
				PGQ_router_handshaking = op_stat_reg ("ACK", OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				}
				FSM_PROFILE_SECTION_OUT (state0_enter_exec)

			/** state (init) exit executives **/
			FSM_STATE_EXIT_FORCED (0, "init", "Yu_wh_psq [init exit execs]")


			/** state (init) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "init", "idle", "tr_3", "Yu_wh_psq [init -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (idle) enter executives **/
			FSM_STATE_ENTER_UNFORCED (1, "idle", state1_enter_exec, "Yu_wh_psq [idle enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (3,"Yu_wh_psq")


			/** state (idle) exit executives **/
			FSM_STATE_EXIT_UNFORCED (1, "idle", "Yu_wh_psq [idle exit execs]")


			/** state (idle) transition processing **/
			FSM_PROFILE_SECTION_IN ("Yu_wh_psq [idle trans conditions]", state1_trans_conds)
			FSM_INIT_COND (ARRIVAL)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("idle")
			FSM_PROFILE_SECTION_OUT (state1_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 1, state1_enter_exec, RECEIVE_FLIT;, "ARRIVAL", "RECEIVE_FLIT", "idle", "idle", "tr_5", "Yu_wh_psq [idle -> idle : ARRIVAL / RECEIVE_FLIT]")
				FSM_CASE_TRANSIT (1, 1, state1_enter_exec, ;, "default", "", "idle", "idle", "tr_4", "Yu_wh_psq [idle -> idle : default / ]")
				}
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (0,"Yu_wh_psq")
		}
	}




void
_op_Yu_wh_psq_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_Yu_wh_psq_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_Yu_wh_psq_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_Yu_wh_psq_svar function. */
#undef ete_gsh
#undef This_Node_Number
#undef Total_Node_Num
#undef flit_stat_handler
#undef flit_counter
#undef PGQ_router_handshaking
#undef Comming_Node_Number
#undef Worm_Create_Time

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_Yu_wh_psq_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_Yu_wh_psq_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (Yu_wh_psq)",
		sizeof (Yu_wh_psq_state));
	*init_block_ptr = 0;

	FRET (obtype)
	}

VosT_Address
_op_Yu_wh_psq_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	Yu_wh_psq_state * ptr;
	FIN_MT (_op_Yu_wh_psq_alloc (obtype))

	ptr = (Yu_wh_psq_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "Yu_wh_psq [init enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_Yu_wh_psq_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	Yu_wh_psq_state		*prs_ptr;

	FIN_MT (_op_Yu_wh_psq_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (Yu_wh_psq_state *)gen_ptr;

	if (strcmp ("ete_gsh" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->ete_gsh);
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
	if (strcmp ("flit_stat_handler" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->flit_stat_handler);
		FOUT
		}
	if (strcmp ("flit_counter" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->flit_counter);
		FOUT
		}
	if (strcmp ("PGQ_router_handshaking" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->PGQ_router_handshaking);
		FOUT
		}
	if (strcmp ("Comming_Node_Number" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Comming_Node_Number);
		FOUT
		}
	if (strcmp ("Worm_Create_Time" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Worm_Create_Time);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}

