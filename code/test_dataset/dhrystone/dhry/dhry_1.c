/*
 ****************************************************************************
 *
 *                   "DHRYSTONE" Benchmark Program
 *                   -----------------------------
 *                                                                            
 *  Version:    C, Version 2.1.2
 *                                                                            
 *  File:       dhry_1.c (part 2 of 3)
 *
 *  Date:       May 17, 1988
 *
 *  Author:     Reinhold P. Weicker
 *
 *  Ported to POSIX time measurement and clang compile by Falk
 *  (falk.richter@yandex.com) in June, 2020.
 *
 ****************************************************************************
 */

/* Patch of the NetBSD ports system */
#include <stdlib.h>
#include <string.h>
/* end of patch */
#include "dhry.h"
#include <stdio.h>
#include "../include/utils.h"
#include "../include/uart.h"
#include "../include/xprintf.h"


static float t0, t1;

void start_time(void)
{
  t0 = get_cycle_value();
}

void stop_time(void)
{
  t1 = get_cycle_value();
}

float get_time(void)
{
  return t1 - t0;
}


/* Global Variables: */ 
Rec_Pointer     Ptr_Glob,
				Next_Ptr_Glob;
int             Int_Glob;
Boolean         Bool_Glob;
char            Ch_1_Glob,
				Ch_2_Glob;
int             Arr_1_Glob [50];
int             Arr_2_Glob [50] [50];
/* Patch of the NetBSD ports system */
/*
   extern char     *malloc ();
   */
/* end of patch */ 

#ifndef REG
Boolean Reg = false;
#define REG
/* REG becomes defined as empty */
/* i.e. no register variables   */
#else
Boolean Reg = true;
#endif

/* variables for time measurement: */
// #include <time.h>
/* time structure needed by clock_gettime */
// struct timespec mg_strTs;
/* CLOCK_MONOTONIC is prefered, if it doesn't exist on your 
 * system you could try CLOCK_REALTIME */
// static clockid_t mg_clk_id = CLOCK_MONOTONIC; 
/* 2s in us */
#define Too_Small_Time 2000000u

// long            Begin_Time,
// 				End_Time,
// 				User_Time;
float           Microseconds,
				Dhrystones_Per_Second; 
/* end of variables for time measurement */

/* functions declaration */ 
void Proc_1 (REG Rec_Pointer Ptr_Val_Par);
void Proc_2 (One_Fifty *Int_Par_Ref);
void Proc_3 (Rec_Pointer *Ptr_Ref_Par);
void Proc_4 (void); 
void Proc_5 (void);
/* this functions are defined in dhry_2.c */
extern void Proc_6 (Enumeration  Enum_Val_Par, Enumeration *Enum_Ref_Par);
extern void Proc_7 (One_Fifty Int_1_Par_Val, One_Fifty Int_2_Par_Val, 
	One_Fifty *Int_Par_Ref);
extern void Proc_8 (Arr_1_Dim Arr_1_Par_Ref, Arr_2_Dim Arr_2_Par_Ref,
	int Int_1_Par_Val, int Int_2_Par_Val);
extern Enumeration Func_1 (Capital_Letter Ch_1_Par_Val, 
	Capital_Letter Ch_2_Par_Val);
extern Boolean Func_2 (Str_30  Str_1_Par_Ref, Str_30  Str_2_Par_Ref);
extern Boolean Func_3 (Enumeration Enum_Par_Val);


/***************************************************************************/
int main (void) { 
  /* main program, corresponds to procedures        */
  /* Main and Proc_0 in the Ada version             */
  uart_init();
  One_Fifty       Int_1_Loc;
  REG   One_Fifty       Int_2_Loc;
  One_Fifty       Int_3_Loc;
  REG   char            Ch_Index;
  Enumeration     Enum_Loc;
  Str_30          Str_1_Loc;
  Str_30          Str_2_Loc;
  REG   int             Run_Index;
  REG   int             Number_Of_Runs;

  /* Initializations */

  Next_Ptr_Glob = (Rec_Pointer) malloc (sizeof (Rec_Type));
  Ptr_Glob = (Rec_Pointer) malloc (sizeof (Rec_Type));

  Ptr_Glob->Ptr_Comp                    = Next_Ptr_Glob;
  Ptr_Glob->Discr                       = Ident_1;
  Ptr_Glob->variant.var_1.Enum_Comp     = Ident_3;
  Ptr_Glob->variant.var_1.Int_Comp      = 40;
  strcpy (Ptr_Glob->variant.var_1.Str_Comp, 
	  "DHRYSTONE PROGRAM, SOME STRING");
  strcpy (Str_1_Loc, "DHRYSTONE PROGRAM, 1'ST STRING");

  Arr_2_Glob [8][7] = 10;
  /* Was missing in published program. Without this statement,    */
  /* Arr_2_Glob [8][7] would have an undefined value.             */
  /* Warning: With 16-Bit processors and Number_Of_Runs > 32000,  */
  /* overflow may occur for this array element.                   */

  xprintf ("\r\n");
  xprintf ("Dhrystone Benchmark, Version 2.1.2 (Language: C)\r\n");
  xprintf ("\r\n");
  if (Reg)
  {
	xprintf ("Program compiled with 'register' attribute\r\n");
	xprintf ("\r\n");
  }
  else
  {
	xprintf ("Program compiled without 'register' attribute\r\n");
	xprintf ("\r\n");
  }
  xprintf ("Please give the number of runs through the benchmark: ");
  {
    int n = 1000000;
    xprintf("%d",n);
	// int n;
	// scanf ("%d", &n);
    Number_Of_Runs = n;
  }
  xprintf ("\r\n");

  xprintf ("Execution starts, %d runs through Dhrystone\r\n", Number_Of_Runs);

  /***************/
  /* Start timer */
  /***************/
  /* try to get the time */
  // if ( clock_gettime(mg_clk_id, &mg_strTs) == 0 ) {
	// Begin_Time = (long) (mg_strTs.tv_sec * 1000000u + mg_strTs.tv_nsec / 1000u);
  // } else {
	// xprintf("Error: could not fetch the time stamp via POSIX clock.");
	// return 1;
  // } 
  start_time();

  for (Run_Index = 1; Run_Index <= Number_Of_Runs; ++Run_Index)
  {

	Proc_5();
	Proc_4();
	/* Ch_1_Glob == 'A', Ch_2_Glob == 'B', Bool_Glob == true */
	Int_1_Loc = 2;
	Int_2_Loc = 3;
	strcpy (Str_2_Loc, "DHRYSTONE PROGRAM, 2'ND STRING");
	Enum_Loc = Ident_2;
	Bool_Glob = ! Func_2 (Str_1_Loc, Str_2_Loc);
	/* Bool_Glob == 1 */
	while (Int_1_Loc < Int_2_Loc)  /* loop body executed once */
	{
	  Int_3_Loc = 5 * Int_1_Loc - Int_2_Loc;
	  /* Int_3_Loc == 7 */
	  Proc_7 (Int_1_Loc, Int_2_Loc, &Int_3_Loc);
	  /* Int_3_Loc == 7 */
	  Int_1_Loc += 1;
	} /* while */
	/* Int_1_Loc == 3, Int_2_Loc == 3, Int_3_Loc == 7 */
	Proc_8 (Arr_1_Glob, Arr_2_Glob, Int_1_Loc, Int_3_Loc);
	/* Int_Glob == 5 */
	Proc_1 (Ptr_Glob);
	for (Ch_Index = 'A'; Ch_Index <= Ch_2_Glob; ++Ch_Index)
	  /* loop body executed twice */
	{
	  if (Enum_Loc == Func_1 (Ch_Index, 'C'))
		/* then, not executed */
	  {
		Proc_6 (Ident_1, &Enum_Loc);
		strcpy (Str_2_Loc, "DHRYSTONE PROGRAM, 3'RD STRING");
		Int_2_Loc = Run_Index;
		Int_Glob = Run_Index;
	  }
	}
	/* Int_1_Loc == 3, Int_2_Loc == 3, Int_3_Loc == 7 */
	Int_2_Loc = Int_2_Loc * Int_1_Loc;
	Int_1_Loc = Int_2_Loc / Int_3_Loc;
	Int_2_Loc = 7 * (Int_2_Loc - Int_3_Loc) - Int_1_Loc;
	/* Int_1_Loc == 1, Int_2_Loc == 13, Int_3_Loc == 7 */
	Proc_2 (&Int_1_Loc);
	/* Int_1_Loc == 5 */

  } /* loop "for Run_Index" */

  /**************/
  /* Stop timer */
  /**************/
  /* try to get the time */
  // if ( clock_gettime(mg_clk_id, &mg_strTs) == 0 ) {
	// End_Time = (long) (mg_strTs.tv_sec * 1000000u + mg_strTs.tv_nsec / 1000u);
  // } else {
	// xprintf("Error: could not fetch the time stamp via POSIX clock.");
	// return 1;
  // } 
  stop_time();

  xprintf ("Execution ends\r\n");
  xprintf ("\r\n");
  xprintf ("Final values of the variables used in the benchmark:\r\n");
  xprintf ("\r\n");
  xprintf ("Int_Glob:            %d\r\n", Int_Glob);
  xprintf ("        should be:   %d\r\n", 5);
  xprintf ("Bool_Glob:           %d\r\n", Bool_Glob);
  xprintf ("        should be:   %d\r\n", 1);
  xprintf ("Ch_1_Glob:           %c\r\n", Ch_1_Glob);
  xprintf ("        should be:   %c\r\n", 'A');
  xprintf ("Ch_2_Glob:           %c\r\n", Ch_2_Glob);
  xprintf ("        should be:   %c\r\n", 'B');
  xprintf ("Arr_1_Glob[8]:       %d\r\n", Arr_1_Glob[8]);
  xprintf ("        should be:   %d\r\n", 7);
  xprintf ("Arr_2_Glob[8][7]:    %d\r\n", Arr_2_Glob[8][7]);
  xprintf ("        should be:   Number_Of_Runs + 10\r\n");
  xprintf ("Ptr_Glob->\r\n");
  /* Patch of NetBSD ports system */
  xprintf ("  Ptr_Comp:          0x%x\r\n", (uint32_t)Ptr_Glob->Ptr_Comp);
  /* End of patch */
  xprintf ("        should be:   (implementation-dependent)\r\n");
  xprintf ("  Discr:             %d\r\n", Ptr_Glob->Discr);
  xprintf ("        should be:   %d\r\n", 0);
  xprintf ("  Enum_Comp:         %d\r\n", Ptr_Glob->variant.var_1.Enum_Comp);
  xprintf ("        should be:   %d\r\n", 2);
  xprintf ("  Int_Comp:          %d\r\n", Ptr_Glob->variant.var_1.Int_Comp);
  xprintf ("        should be:   %d\r\n", 17);
  xprintf ("  Str_Comp:          %s\r\n", Ptr_Glob->variant.var_1.Str_Comp);
  xprintf ("        should be:   DHRYSTONE PROGRAM, SOME STRING\r\n");
  xprintf ("Next_Ptr_Glob->\r\n");
  /* Patch of NetBSD ports system */ 
  xprintf ("  Ptr_Comp:          0x%x\r\n", (uint32_t)Next_Ptr_Glob->Ptr_Comp);
  /* End of patch */ 
  xprintf ("        should be:   (implementation-dependent), same as above\r\n");
  xprintf ("  Discr:             %d\r\n", Next_Ptr_Glob->Discr);
  xprintf ("        should be:   %d\r\n", 0);
  xprintf ("  Enum_Comp:         %d\r\n", Next_Ptr_Glob->variant.var_1.Enum_Comp);
  xprintf ("        should be:   %d\r\n", 1);
  xprintf ("  Int_Comp:          %d\r\n", Next_Ptr_Glob->variant.var_1.Int_Comp);
  xprintf ("        should be:   %d\r\n", 18);
  xprintf ("  Str_Comp:          %s\r\n",
	  Next_Ptr_Glob->variant.var_1.Str_Comp);
  xprintf ("        should be:   DHRYSTONE PROGRAM, SOME STRING\r\n");
  xprintf ("Int_1_Loc:           %d\r\n", Int_1_Loc);
  xprintf ("        should be:   %d\r\n", 5);
  xprintf ("Int_2_Loc:           %d\r\n", Int_2_Loc);
  xprintf ("        should be:   %d\r\n", 13);
  xprintf ("Int_3_Loc:           %d\r\n", Int_3_Loc);
  xprintf ("        should be:   %d\r\n", 7);
  xprintf ("Enum_Loc:            %d\r\n", Enum_Loc);
  xprintf ("        should be:   %d\r\n", 1);
  xprintf ("Str_1_Loc:           %s\r\n", Str_1_Loc);
  xprintf ("        should be:   DHRYSTONE PROGRAM, 1'ST STRING\r\n");
  xprintf ("Str_2_Loc:           %s\r\n", Str_2_Loc);
  xprintf ("        should be:   DHRYSTONE PROGRAM, 2'ND STRING\r\n");
  xprintf ("\r\n");

  // User_Time = End_Time - Begin_Time;
  float User_Time = get_time();
  if (User_Time < (float)(Too_Small_Time/1e6))
  {
	xprintf ("Measured time too small to obtain meaningful results\r\n");
	xprintf ("Please increase number of runs\r\n");
	xprintf ("\r\n");
  }
  else
  {
	/* User_Time is already in us */
	Microseconds = (float) User_Time * (float)1e6 / (float) Number_Of_Runs;
	/* 1e6 converts User_Time in seconds */
	Dhrystones_Per_Second = ((float) Number_Of_Runs)
	  / (float) User_Time; 

	xprintf ("Microseconds for one run through Dhrystone: ");
	// xprintf ("%6.1f \r\n", Microseconds);
  xprintf("%d.%04d\r\n",(uint32_t)Microseconds,(uint32_t)(10000*Microseconds)%10000);
	xprintf ("Dhrystones per Second:                      ");
	// xprintf ("%6.1f \r\n", Dhrystones_Per_Second);
  xprintf("%d.%04d\r\n",(uint32_t)Dhrystones_Per_Second,(uint32_t)(10000*Dhrystones_Per_Second)%10000);
	xprintf ("\r\n");
  }

  return 0;  
}


/***************************************************************************/
void Proc_1 (REG Rec_Pointer Ptr_Val_Par) {
  /* executed once */

  REG Rec_Pointer Next_Record = Ptr_Val_Par->Ptr_Comp;  
  /* == Ptr_Glob_Next */
  /* Local variable, initialized with Ptr_Val_Par->Ptr_Comp,    */
  /* corresponds to "rename" in Ada, "with" in Pascal           */

  structassign (*Ptr_Val_Par->Ptr_Comp, *Ptr_Glob); 
  Ptr_Val_Par->variant.var_1.Int_Comp = 5;
  Next_Record->variant.var_1.Int_Comp 
	= Ptr_Val_Par->variant.var_1.Int_Comp;
  Next_Record->Ptr_Comp = Ptr_Val_Par->Ptr_Comp;
  Proc_3 (&Next_Record->Ptr_Comp);
  /* Ptr_Val_Par->Ptr_Comp->Ptr_Comp 
	 == Ptr_Glob->Ptr_Comp */
  if (Next_Record->Discr == Ident_1)
	/* then, executed */
  {
	Next_Record->variant.var_1.Int_Comp = 6;
	Proc_6 (Ptr_Val_Par->variant.var_1.Enum_Comp, 
		&Next_Record->variant.var_1.Enum_Comp);
	Next_Record->Ptr_Comp = Ptr_Glob->Ptr_Comp;
	Proc_7 (Next_Record->variant.var_1.Int_Comp, 10, 
		&Next_Record->variant.var_1.Int_Comp);
  }
  else /* not executed */
	structassign (*Ptr_Val_Par, *Ptr_Val_Par->Ptr_Comp);
} /* Proc_1 */


/***************************************************************************/
void Proc_2 (One_Fifty *Int_Par_Ref) {
  /* executed once */
  /* *Int_Par_Ref == 1, becomes 4 */

  One_Fifty  Int_Loc;  
  Enumeration   Enum_Loc;

  Int_Loc = *Int_Par_Ref + 10;
  do /* executed once */
	if (Ch_1_Glob == 'A')
	  /* then, executed */
	{
	  Int_Loc -= 1;
	  *Int_Par_Ref = Int_Loc - Int_Glob;
	  Enum_Loc = Ident_1;
	} /* if */
  while (Enum_Loc != Ident_1); /* true */
} /* Proc_2 */


/***************************************************************************/
void Proc_3 (Rec_Pointer *Ptr_Ref_Par) {
  /* executed once */
  /* Ptr_Ref_Par becomes Ptr_Glob */

  if (Ptr_Glob != Null)
	/* then, executed */
	*Ptr_Ref_Par = Ptr_Glob->Ptr_Comp;
  Proc_7 (10, Int_Glob, &Ptr_Glob->variant.var_1.Int_Comp);
} /* Proc_3 */


/***************************************************************************/
void Proc_4 (void) {
  /* executed once */
  
  Boolean Bool_Loc;

  Bool_Loc = Ch_1_Glob == 'A';
  Bool_Glob = Bool_Loc | Bool_Glob;
  Ch_2_Glob = 'B';
} /* Proc_4 */


/***************************************************************************/
void Proc_5 (void) {
  /* executed once */
  
  Ch_1_Glob = 'A';
  Bool_Glob = false;
} /* Proc_5 */


/* Procedure for the assignment of structures,          */
/* if the C compiler doesn't support this feature       */
#ifdef  NOSTRUCTASSIGN
memcpy (d, s, l)
  register char   *d;
  register char   *s;
  register int    l;
{
  while (l--) *d++ = *s++;
}
#endif

