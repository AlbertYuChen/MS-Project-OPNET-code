/* Process model C form file: Yu_wh_router_proc.pr.c */
/* Portions of this file copyright 1986-2011 by OPNET Technologies, Inc. */


/*
 =========================== NOTE ==========================
 This file is automatically generated from Yu_wh_router_proc.pr.m
 during a process model compilation.

 Do NOT manually edit this file.
 Manual edits will be lost during the next compilation.
 =========================== NOTE ==========================
*/



/* This variable carries the header into the object file */
const char Yu_wh_router_proc_pr_c [] = "MIL_3_Tfile_Hdr_ 171A 30A modeler 7 551D749C 551D749C 1 ECE-PHO309-01 chenyua 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 2b1a 1                                                                                                                                                                                                                                                                                                                                                                                                    ";
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

/*
define the interrupt codes here
*/
#define SA_FLIT_INTRPT_CODE 1000
#define ID_DONE_INTRPT_CODE 1001

// define FLIT_ARRIVAL, stream interrupt within the node
#define FLIT_ARRIVAL (op_intrpt_type () == OPC_INTRPT_STRM)

// check the adjacent nodes
#define SA_FLIT (op_intrpt_type () == OPC_INTRPT_STRM)

// ACK remote intrpt from adjacent node
#define ACK (op_intrpt_type () == OPC_INTRPT_REMOTE)

// PSQUEUE_READY signal
#define PSQUEUE_READY (op_intrpt_type () == OPC_INTRPT_STAT)

// define the time out handler
#define TIME_OUT (op_intrpt_type () == OPC_INTRPT_SELF)

// define the time out handler
#define ID_DONE (op_intrpt_type () == OPC_INTRPT_SELF)

// define flit type here
#define Flit_Type_Data_Flit 0
#define Flit_Type_Dest_Addr 1
#define Flit_Type_Src_Addr 2
#define Flit_Type_Worm_Length 6
#define Flit_Type_Tail 7

// read the node specified routing table here
#define ID_STREAM get_id_stream();

// flit handler when a flit is arriving
#define HANDLE_FLIT flit_handler();

// ACK handler
#define HANDLE_ACK ACK_handler();

// handler swire when PSQueue is ready
#define HANDLE_SWIRE SWIRE_handler();

// 
#define HANDLE_TIMER router_handle_time_out();


/*
routing table
*/
int ** Routing_Table;









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
	int	                    		port_number                                     ;
	int	                    		This_Node_Number                                ;
	int	                    		In_Node_Number                                  ;
	int	                    		Out_Node_Number                                 ;
	int	                    		My_Total_Node_Num                               ;
	int	                    		Total_Node_Num                                  ;
	int	                    		Max_Deg                                         ;
	} Yu_wh_router_proc_state;

#define port_number             		op_sv_ptr->port_number
#define This_Node_Number        		op_sv_ptr->This_Node_Number
#define In_Node_Number          		op_sv_ptr->In_Node_Number
#define Out_Node_Number         		op_sv_ptr->Out_Node_Number
#define My_Total_Node_Num       		op_sv_ptr->My_Total_Node_Num
#define Total_Node_Num          		op_sv_ptr->Total_Node_Num
#define Max_Deg                 		op_sv_ptr->Max_Deg

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	Yu_wh_router_proc_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((Yu_wh_router_proc_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


/* Function Block */

#if !defined (VOSD_NO_FIN)
enum { _op_block_origin = __LINE__ + 2};
#endif

Packet * r_generate_flit(int type, int data){
	Packet * pkptr;
	
	FIN(generate_flit());
	
	pkptr = op_pk_create_fmt("Yu_wh_pkformat");
	op_pk_nfd_set(pkptr, "type", type);
	op_pk_nfd_set(pkptr, "data", data);
	
	FRET(pkptr);
}

void load_ARnode_routing_table(void) {
	Packet * pkptr;
	Objid myObjid, parentObjid;
	char parentname[64];
	char RName[128];
	FILE * Rinfile;
	char lbuf[128];
	char * nToken;
	int strm_pt;

	int i;

	int row;
	int col;
	int dest_port;
	
	FIN(load_ARnode_routing_table());
	
	// get the local node number
	myObjid = op_id_self();
	parentObjid = op_topo_parent (myObjid);
	op_ima_obj_attr_get (parentObjid, "name", &parentname);
	This_Node_Number = atoi(&parentname[5]);
	
	// get the routing table file path
	sprintf(RName, "C:\\Users\\chenyua\\OPNET_Project\\WH_G4x4\\Rnode_%d.txt", This_Node_Number);
	if (!(Rinfile = fopen(RName, "r"))) {
		printf("load_ARnode_routing_table@%d: could not find file", This_Node_Number);
		exit(-2);
	}
	
	fgets(lbuf, 128, Rinfile);
	nToken = strtok(lbuf, " \t\n");
	Total_Node_Num = atoi(nToken);
	nToken = strtok(NULL, " \t\n");
	Max_Deg = atoi(nToken);
	
	//>>>>>>>>>>>>>>>>>> this G4x4 Rnode table is different from the G8x8 ARnode table
	Max_Deg--;
	
	printf("Total_Node_Num: %d  Max_Deg: %d\n",Total_Node_Num, Max_Deg);
	// send the source head flit to adjacent nodes, they will get the connection status
	// the max port number is defined in the attributes
	
	for (strm_pt = 2; strm_pt <= Max_Deg + 1; strm_pt++) {
	//for (strm_pt = 2; strm_pt <= 2; strm_pt++) {
		pkptr = r_generate_flit(Flit_Type_Src_Addr, This_Node_Number);				
		op_pk_send(pkptr, strm_pt);
	}
	
	Routing_Table = (int **)malloc(Max_Deg * sizeof(int));
	// the reason for adding one here is that the port includes the PSQueue port, i.e. port #0
	for (i = 0; i < Max_Deg + 1; i++) {
		Routing_Table[i] = (int *)malloc(Total_Node_Num * sizeof(int));
	}
	
	// start from the second line
	row = 0;
	while(fgets(lbuf, 2048, Rinfile)){
		//printf("%s", lbuf);
		nToken = strtok(lbuf, " ");
		
		for (col = 0; col < Total_Node_Num; col++) {
			
			dest_port = atoi(nToken);
			Routing_Table[row][col] = dest_port;
			
			if (op_prg_odb_ltrace_active ("ART") == OPC_TRUE){
				printf("%3d", Routing_Table[row][col]);
				if(col == Total_Node_Num - 1){
					printf("\nrow:%d  col:%d\n", row, col);
				}
			}
			nToken = strtok(NULL, " ");	
		}
		row++;
	}
	FOUT;
}

void load_Rnode_routing_table(void) {
	int i;
	char RName[128];
	char lbuf[2048];
	char * nToken;
	FILE * Rinfile;
	int row;
	int col;
	int dest_port;
	
	FIN(load_ARnode_routing_table());
	
	sprintf(RName, "C:\\Users\\chenyua\\OPNET_Project\\WH\\ARnode_%d.txt", This_Node_Number);
	if (!(Rinfile = fopen(RName, "r"))) {
		printf("load_routing_table: could not find file");
		exit(-2);
	}
	
	Routing_Table = (int **)malloc(Max_Deg * sizeof(int));
	// the reason for adding one here is that the port includes the PSQueue port, i.e. port #0
	for (i = 0; i < Max_Deg + 1; i++) {
		Routing_Table[i] = (int *)malloc(Total_Node_Num * sizeof(int));
	}
	
	// start from the second line
	fgets(lbuf, 2048, Rinfile);
	row = 0;
	while(fgets(lbuf, 2048, Rinfile)){
		//printf("%s", lbuf);
		nToken = strtok(lbuf, " ] [ ");
		
		for (col = 0; col < Total_Node_Num; col++) {
			
			dest_port = atoi(nToken);
			Routing_Table[row][col] = dest_port;
			
			if (op_prg_odb_ltrace_active ("RT") == OPC_TRUE){
				printf("%3d", Routing_Table[row][col]);
				if(col == 63){
					printf("\nrow:%d  col:%d\n", row, col);
				}
			}
			nToken = strtok(NULL, " ] [ ");	
		}
		row++;
	}
	FOUT;
}

void initialize_router(void){
	
	FIN(initialize_router());
	
	// call the routing table initializer here
	// if(This_Node_Number == 0)
	// load_Rnode_routing_table();
	load_ARnode_routing_table();
	
	FOUT;
}

// get node attribution in Identify state
void get_id_stream(void) {

	Packet * pkptr;
	int type;
	int data;
	int strm_port_number;

	FIN(get_id_stream());
	
	strm_port_number = op_intrpt_strm ();
	pkptr = op_pk_get (strm_port_number);
	op_pk_nfd_get(pkptr, "type", &type);
	op_pk_nfd_get(pkptr, "data", &data);
	
	if (op_prg_odb_ltrace_active ("SA") == OPC_TRUE){
		printf("R_Node%d strm@%d: type:%d data:%d\n", This_Node_Number, strm_port_number, type, data);
	}

	// get the neighbors ID
	// send source address flit to all neighbours and wait for the response
	// pkptr = generate_flit(Flit_Type_Src_Addr, node_ID_self);
	
	// how to get active ports? and how to hear back from them?

	
	// schedule a self interrupt to move to Listening status
	op_intrpt_schedule_self (op_sim_time (), 0);

	FOUT;
}

void send_stat_wire_to_PGQueue(void){
	
	FIN(send_stat_wire_to_PGQueue());
	
	FOUT;
}

void send_remote_intrpt_to_adjacent_node(void){

	FIN(send_remote_intrpt_to_adjacent_node());
	
	FOUT;
}

void remote_node_intrpt_listerner(void){

	FIN(remote_node_intrpt_listerner());
	
	FOUT;
}

// functions for Listerning
void flit_handler(void){
	Packet * pkptr;
	int flit_type;
	int tmp_type;
	int tmp_data;
	
	FIN(flit_handler());
	
	pkptr = op_pk_get(op_intrpt_strm());
	flit_type = op_pk_nfd_get(pkptr, "type", &flit_type);
	op_pk_nfd_get(pkptr, "type", &tmp_type);
	op_pk_nfd_get(pkptr, "data", &tmp_data);
	
	if (op_prg_odb_ltrace_active ("LS") == OPC_TRUE){
		printf("Listern: node: %d got flit type:%d data:%d\n\n", This_Node_Number, tmp_type, tmp_data);
	}
	
	// call other procedures that handle individual flit types
	if(flit_type == Flit_Type_Dest_Addr) {
	// look up the routing table and determine the output port
	
	// identify whether the port is busy or not
	
	// else if it's busy, keep the header flit in a register
	
	// else if it's not busy, then the input port it came from is associated with the 
	// output port and binding takes place
	
	
	}else if(flit_type == Flit_Type_Src_Addr) {
	
	
	}else if(flit_type == Flit_Type_Worm_Length) {
	
	
	}else if(flit_type == Flit_Type_Tail) {
	// check the list of waiting head flit
	// free the input port and associated output port
	
	}
	
	send_stat_wire_to_PGQueue();
	
	FOUT;
}

void ACK_handler(void) {
	
	FIN(ACK_handler());
	
	FOUT;
}

void SWIRE_handler(void){

	FIN(SWIRE_handler());
	
	
	FOUT;
}


void router_handle_time_out(void){
	int intrpt_code;
	
	FIN(router_handle_time_out());
	
	intrpt_code = op_intrpt_code();
	
	//for debug usage
	
	FOUT;
}

void route_pk(void){ 

      Packet * pkptr;
	  int type;
	  int dest_address;
	  
      FIN(route_pk());
	  
	  // get packet (flit)
	  pkptr = op_pk_get(op_intrpt_strm ());
	  
	  // get type
      op_pk_nfd_get_int32 (pkptr, "type", &type);
	  
	  // get data
      op_pk_nfd_get_int32 (pkptr, "data", &type);
	  
	  dest_address = 0;
      op_pk_send (pkptr, dest_address);
	  
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
	void Yu_wh_router_proc (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_Yu_wh_router_proc_init (int * init_block_ptr);
	void _op_Yu_wh_router_proc_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_Yu_wh_router_proc_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_Yu_wh_router_proc_alloc (VosT_Obtype, int);
	void _op_Yu_wh_router_proc_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
Yu_wh_router_proc (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (Yu_wh_router_proc ());

		{
		/* Temporary Variables */
		
		/* End of Temporary Variables */


		FSM_ENTER ("Yu_wh_router_proc")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (Init) enter executives **/
			FSM_STATE_ENTER_FORCED_NOLABEL (0, "Init", "Yu_wh_router_proc [Init enter execs]")
				FSM_PROFILE_SECTION_IN ("Yu_wh_router_proc [Init enter execs]", state0_enter_exec)
				{
				
				initialize_router();
				}
				FSM_PROFILE_SECTION_OUT (state0_enter_exec)

			/** state (Init) exit executives **/
			FSM_STATE_EXIT_FORCED (0, "Init", "Yu_wh_router_proc [Init exit execs]")


			/** state (Init) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "Init", "Identify", "tr_4", "Yu_wh_router_proc [Init -> Identify : default / ]")
				/*---------------------------------------------------------*/



			/** state (Identify) enter executives **/
			FSM_STATE_ENTER_UNFORCED (1, "Identify", state1_enter_exec, "Yu_wh_router_proc [Identify enter execs]")
				FSM_PROFILE_SECTION_IN ("Yu_wh_router_proc [Identify enter execs]", state1_enter_exec)
				{
				// printf("ENTER IDentify!\n");
				}
				FSM_PROFILE_SECTION_OUT (state1_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (3,"Yu_wh_router_proc")


			/** state (Identify) exit executives **/
			FSM_STATE_EXIT_UNFORCED (1, "Identify", "Yu_wh_router_proc [Identify exit execs]")


			/** state (Identify) transition processing **/
			FSM_PROFILE_SECTION_IN ("Yu_wh_router_proc [Identify trans conditions]", state1_trans_conds)
			FSM_INIT_COND (SA_FLIT)
			FSM_TEST_COND (ID_DONE)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("Identify")
			FSM_PROFILE_SECTION_OUT (state1_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 1, state1_enter_exec, ID_STREAM;, "SA_FLIT", "ID_STREAM", "Identify", "Identify", "tr_5", "Yu_wh_router_proc [Identify -> Identify : SA_FLIT / ID_STREAM]")
				FSM_CASE_TRANSIT (1, 2, state2_enter_exec, ;, "ID_DONE", "", "Identify", "Listening", "tr_8", "Yu_wh_router_proc [Identify -> Listening : ID_DONE / ]")
				FSM_CASE_TRANSIT (2, 1, state1_enter_exec, ;, "default", "", "Identify", "Identify", "tr_7", "Yu_wh_router_proc [Identify -> Identify : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (Listening) enter executives **/
			FSM_STATE_ENTER_UNFORCED (2, "Listening", state2_enter_exec, "Yu_wh_router_proc [Listening enter execs]")
				FSM_PROFILE_SECTION_IN ("Yu_wh_router_proc [Listening enter execs]", state2_enter_exec)
				{
				// printf("ENTER Listening!\n");
				}
				FSM_PROFILE_SECTION_OUT (state2_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (5,"Yu_wh_router_proc")


			/** state (Listening) exit executives **/
			FSM_STATE_EXIT_UNFORCED (2, "Listening", "Yu_wh_router_proc [Listening exit execs]")


			/** state (Listening) transition processing **/
			FSM_PROFILE_SECTION_IN ("Yu_wh_router_proc [Listening trans conditions]", state2_trans_conds)
			FSM_INIT_COND (FLIT_ARRIVAL)
			FSM_TEST_COND (PSQUEUE_READY)
			FSM_TEST_COND (TIME_OUT)
			FSM_TEST_COND (ACK)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("Listening")
			FSM_PROFILE_SECTION_OUT (state2_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 2, state2_enter_exec, HANDLE_FLIT;, "FLIT_ARRIVAL", "HANDLE_FLIT", "Listening", "Listening", "tr_10", "Yu_wh_router_proc [Listening -> Listening : FLIT_ARRIVAL / HANDLE_FLIT]")
				FSM_CASE_TRANSIT (1, 2, state2_enter_exec, HANDLE_SWIRE;, "PSQUEUE_READY", "HANDLE_SWIRE", "Listening", "Listening", "tr_12", "Yu_wh_router_proc [Listening -> Listening : PSQUEUE_READY / HANDLE_SWIRE]")
				FSM_CASE_TRANSIT (2, 2, state2_enter_exec, HANDLE_TIMER;, "TIME_OUT", "HANDLE_TIMER", "Listening", "Listening", "tr_13", "Yu_wh_router_proc [Listening -> Listening : TIME_OUT / HANDLE_TIMER]")
				FSM_CASE_TRANSIT (3, 2, state2_enter_exec, HANDLE_ACK;, "ACK", "HANDLE_ACK", "Listening", "Listening", "tr_14", "Yu_wh_router_proc [Listening -> Listening : ACK / HANDLE_ACK]")
				FSM_CASE_TRANSIT (4, 2, state2_enter_exec, ;, "default", "", "Listening", "Listening", "tr_9", "Yu_wh_router_proc [Listening -> Listening : default / ]")
				}
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (0,"Yu_wh_router_proc")
		}
	}




void
_op_Yu_wh_router_proc_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_Yu_wh_router_proc_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_Yu_wh_router_proc_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_Yu_wh_router_proc_svar function. */
#undef port_number
#undef This_Node_Number
#undef In_Node_Number
#undef Out_Node_Number
#undef My_Total_Node_Num
#undef Total_Node_Num
#undef Max_Deg

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_Yu_wh_router_proc_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_Yu_wh_router_proc_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (Yu_wh_router_proc)",
		sizeof (Yu_wh_router_proc_state));
	*init_block_ptr = 0;

	FRET (obtype)
	}

VosT_Address
_op_Yu_wh_router_proc_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	Yu_wh_router_proc_state * ptr;
	FIN_MT (_op_Yu_wh_router_proc_alloc (obtype))

	ptr = (Yu_wh_router_proc_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "Yu_wh_router_proc [Init enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_Yu_wh_router_proc_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	Yu_wh_router_proc_state		*prs_ptr;

	FIN_MT (_op_Yu_wh_router_proc_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (Yu_wh_router_proc_state *)gen_ptr;

	if (strcmp ("port_number" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->port_number);
		FOUT
		}
	if (strcmp ("This_Node_Number" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->This_Node_Number);
		FOUT
		}
	if (strcmp ("In_Node_Number" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->In_Node_Number);
		FOUT
		}
	if (strcmp ("Out_Node_Number" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Out_Node_Number);
		FOUT
		}
	if (strcmp ("My_Total_Node_Num" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->My_Total_Node_Num);
		FOUT
		}
	if (strcmp ("Total_Node_Num" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Total_Node_Num);
		FOUT
		}
	if (strcmp ("Max_Deg" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Max_Deg);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}

