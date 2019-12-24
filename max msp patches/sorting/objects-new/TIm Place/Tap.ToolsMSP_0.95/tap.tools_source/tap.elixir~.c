// MSP External: tap.elixir~.c// an advanced signal selector/elixir// T.Place - 05.04.2001//		v1.1 (08/28/2001) - update for Max/MSP 4, added interpolation option//		v1.2 (11/30/2001) - minor revisions// Things to think about adding: a switch for the automatic gain cut#include "ext.h"				// Required for all Max External Objects#include "z_dsp.h"			// Required for all MSP External Objects#include <math.h>			// Required for sqrt function#include <stdlib.h>#include "ext_strings.h"		// Used for the assistance stringsvoid *elixir_class;			// Required. Global pointing to this classtypedef struct _elixir		// Data Structure for this object{	t_pxobject x_obj;	int elixir_toggle[10];		// The 10 switches for the 10 inlets (array)	float elixir_time[10];		// The 10 slew times for the 10 inlets (array)	float elixir_mult[10];		// Amount to scale the output by	int elixir_numInlets;		// Number of inlets created by the _new method	float elixir_gtime;	int elixir_sr;	int line_toggle[10];	double line_result[10];	double line_destination[10];	double line_step[10];	long line_samplenum[10];	long line_samplesDone[10]; 	long line_slewTime[10];} t_elixir;// Prototypes for methods: need a method for each incoming message typevoid *elixir_new(long numInlets, float slewtimes);						// New Object Creation Methodt_int *elixir_perform2(t_int *w);									// An MSP Perform (signal) Methodt_int *elixir_perform3(t_int *w);									// An MSP Perform (signal) Methodt_int *elixir_perform4(t_int *w);									// An MSP Perform (signal) Methodt_int *elixir_perform5(t_int *w);									// An MSP Perform (signal) Methodt_int *elixir_perform6(t_int *w);									// An MSP Perform (signal) Methodt_int *elixir_perform7(t_int *w);									// An MSP Perform (signal) Methodt_int *elixir_perform8(t_int *w);									// An MSP Perform (signal) Methodt_int *elixir_perform9(t_int *w);									// An MSP Perform (signal) Methodt_int *elixir_perform10(t_int *w);									// An MSP Perform (signal) Methodvoid elixir_dsp(t_elixir *x, t_signal **sp, short *count);					// DSP Methodvoid elixir_assist(t_elixir *x, void *b, long m, long a, char *s);				// Assistance Methodvoid elixir_int(t_elixir *x, long toggle);								// Int Methodvoid elixir_free(t_elixir *x);void elixir_doit(t_elixir *x, long val, short inlet, short typeofmess);void elixir_gtime(t_elixir *x, float slewtime);void elixir_tellmeeverything(t_elixir *x);void elixir_list(t_elixir *x, t_symbol *s, short argc, t_atom *argv);void interp_calc(t_elixir *x, short inletNumber, float oldmult);float perform_interpolation(t_elixir *x, short inletNumber);/************************************************************************************/// Main() Functionvoid main(void)				// main recieves a copy of the Max function macros table{	setup((t_messlist **)&elixir_class, (method)elixir_new, (method)elixir_free, (short)sizeof(t_elixir), 0L, A_DEFLONG, A_DEFFLOAT, 0);	addmess((method)elixir_dsp, "dsp", A_CANT, 0);						// Bind method "elixir_dsp" to the DSP call from MSP	addmess((method)elixir_assist, "assist", A_CANT, 0);					// Bind method "elixir_assist" to assistance calls	addint((method)elixir_int);		addmess((method)elixir_list, "list", A_GIMME, 0);			addmess((method)elixir_gtime, "gtime", A_FLOAT, 0);	addmess((method)inspector_open, "info", A_CANT, 0);					// bind method for opening the license window	addmess((method)elixir_tellmeeverything, "tellmeeverything", 0);		dsp_initclass();													// Setup object's class to work with MSP	post("tap.elixir~ v1.2 - Copyright � 2001 by Timothy A. Place");			// Print to the Max Window...	post("     Impact Center @ University of Missouri - Kansas City / Silicon Prairie Intermedia");}/************************************************************************************/// Object Creation Methodvoid *elixir_new(long numInlets, float slewtime){	int	i;	t_elixir *x = (t_elixir *)newobject(elixir_class);		if (numInlets < 2) numInlets = 2;	else if (numInlets > 10) numInlets = 10; 	x->elixir_numInlets = numInlets;        dsp_setup((t_pxobject *)x,numInlets);		// Create Object and Inlets	outlet_new((t_pxobject *)x, "signal");		// Create a signal Outlet       	for (i=0; i<10; i++){	    	if (slewtime > 0)    			x->elixir_time[i] = slewtime;    		else    			x->elixir_time[i] = 0;  		x->elixir_toggle[i] = 0; 		  		x->line_toggle[i] = 0;		x->line_result[i] = 0;		x->line_destination[i] = 0;		x->line_step[i] = 0;		x->line_samplenum[i] = 0;		x->line_samplesDone[i] = 0;     	}    	x->elixir_gtime = slewtime;	return (x);							// Return the pointer}/************************************************************************************/// Methods bound to input/inlets// Method for Assistance Messagesvoid elixir_assist(t_elixir *x, void *b, long msg, long arg, char *dst){	if(msg==1){ 	// Inlets		switch(arg){			case 0: strcpy(dst, "(signal) Input1, (toggle) gate control, etc."); break;			case 1: strcpy(dst, "(signal) Input2, (toggle) gate control, etc."); break;			case 2: strcpy(dst, "(signal) Input3, (toggle) gate control, etc."); break;			case 3: strcpy(dst, "(signal) Input4, (toggle) gate control, etc."); break;			case 4: strcpy(dst, "(signal) Input5, (toggle) gate control, etc."); break;			case 5: strcpy(dst, "(signal) Input6, (toggle) gate control, etc."); break;			case 6: strcpy(dst, "(signal) Input7, (toggle) gate control, etc."); break;			case 7: strcpy(dst, "(signal) Input8, (toggle) gate control, etc."); break;			case 8: strcpy(dst, "(signal) Input9, (toggle) gate control, etc."); break;			case 9: strcpy(dst, "(signal) Input10, (toggle) gate control, etc."); break;		}	}	else if(msg==2) // Outlets		strcpy(dst, "(signal) Mixed output");}// Tell Me Everythingvoid elixir_tellmeeverything(t_elixir *x){	post("Tell Me Everything...");	post("     tap.elixir version 1.2, Copyright � 2001 by Timothy Place");	post("     Number of Inlets: %i", x->elixir_numInlets);	post("     Toggle Position of Inlets: %i %i %i %i %i %i %i %i %i %i", x->elixir_toggle[0], x->elixir_toggle[1], x->elixir_toggle[2], x->elixir_toggle[3], x->elixir_toggle[4], x->elixir_toggle[5], x->elixir_toggle[6], x->elixir_toggle[7], x->elixir_toggle[8], x->elixir_toggle[9]);	post("     Interpolation Time of Inlets: %f %f %f %f %f %f %f %f %f %f", x->elixir_time[0], x->elixir_time[1], x->elixir_time[2], x->elixir_time[3], x->elixir_time[4], x->elixir_time[5], x->elixir_time[6], x->elixir_time[7], x->elixir_time[8], x->elixir_time[9]);	post("     Current Scaling Factors: %f %f %f %f %f %f %f %f %f %f", x->elixir_mult[0], x->elixir_mult[1], x->elixir_mult[2], x->elixir_mult[3], x->elixir_mult[4], x->elixir_mult[5], x->elixir_mult[6], x->elixir_mult[7], x->elixir_mult[8], x->elixir_mult[9]);	post("     Inlets using interpolation: %i %i %i %i %i %i %i %i %i %i", x->line_toggle[0], x->line_toggle[1], x->line_toggle[2], x->line_toggle[3], x->line_toggle[4], x->line_toggle[5], x->line_toggle[6], x->line_toggle[7], x->line_toggle[8], x->line_toggle[9]);}// Set a global slew timevoid elixir_gtime(t_elixir *x, float slewtime){	short i;	x->elixir_gtime = slewtime;    	if (slewtime > 0){    		for (i=0; i<10; i++)    			x->elixir_time[i] = slewtime;    	}    	else{    		for (i=0; i<10; i++)    			x->elixir_time[i] = 0;    	}		}// List Methodvoid elixir_list(t_elixir *x, t_symbol *s, short argc, t_atom *argv){		short inlet = x->x_obj.z_in;	x->elixir_time[inlet] = atom_getfloatarg(1,argc,argv);		elixir_doit(x, atom_getfloatarg(0,argc,argv), inlet, 1);}// Int (toggle) Methodvoid elixir_int(t_elixir *x, long val){	elixir_doit(x, val, x->x_obj.z_in ,0);}// Do itvoid elixir_doit(t_elixir *x, long val, short inlet, short typeofmess){	int i;	float n, oldmult = x->elixir_mult[inlet];	x->elixir_toggle[inlet] = val;		if (typeofmess == 0)							// If input is a single int, supply the gtime				x->elixir_time[inlet] = x->elixir_gtime;				val = 0;									// Count the number of channels that are on	for (i = 0; i < 10; i = i + 1)	{		if (x->elixir_toggle[i] == 1)			val = val + 1;				}	n = val;	for (i=0; i<10; i++)							// Set the multiply amount for this channel	{				if (n != 0)		{			if (x->elixir_toggle[i] != 0)				x->elixir_mult[i] = 1.0 / n;			else				x->elixir_mult[i] = 0;			}		else 			x->elixir_mult[i] = 0.;			}	for(i=0; i<10; i++)							// Recalculate and trigger the mixer channels		interp_calc(x, i, oldmult);}void interp_calc(t_elixir *x, short inletNumber, float oldmult){	float f, ff, diff, slew, step;		if ( x->elixir_time[inletNumber] != 0 )			// If interpolation is desired...	{			slew = x->line_slewTime[inletNumber] = x->elixir_time[inletNumber] * 0.001 * x->elixir_sr;	// ms -> sec * sr = slewtime_in_samples		x->line_toggle[inletNumber] = 1;									ff = x->line_result[inletNumber]; 							// most recent output value		diff = x->elixir_mult[inletNumber] - ff;						// difference between it, and new value		step = diff / slew;										// increment per sample to interpolate linearly		x->line_destination[inletNumber] = x->elixir_mult[inletNumber];		// store final destination		x->line_step[inletNumber] = step;							// store step size of the interpolation		x->line_samplenum[inletNumber] = x->line_slewTime[inletNumber];	// needed for triggering the interpolation		x->line_samplesDone[inletNumber] = 0;						// Reset the position in the interpolation process	}	else 		x->line_toggle[inletNumber] = 0;							}// Memory Deallocationvoid elixir_free(t_elixir *x){	notify_free((t_object *)x);	dsp_free((t_pxobject *)x);}// Perform (signal) Method2t_int *elixir_perform2(t_int *w){    t_float *in1 = (t_float *)(w[1]);				// Input1    t_float *in2 = (t_float *)(w[2]);				// Input2    t_float *out = (t_float *)(w[3]);				// Output   	t_elixir *x = (t_elixir *)(w[4]);				// Pointer	int n = (int)(w[5]);							// Vector Size			double input[2], val;	short i;							if (x->x_obj.z_disabled) return (w+6);	while (--n) 	{		input[0] = *++in1; 		input[1] = *++in2; 		for(i=0; i<2; i++){			if(x->line_toggle[i] == 0)				input[i] = input[i] * x->elixir_mult[i];			else				input[i] = input[i] * perform_interpolation(x, i);		}		val = 0;		for(i=0; i<2; i++)			val = val + input[i];					*++out = val * 0.95;	}	    return (w + 6);								// Return a pointer to the NEXT object in the DSP call chain}// Perform (signal) Method3t_int *elixir_perform3(t_int *w){	t_float *in1 = (t_float *)(w[1]);	// Input1	t_float *in2 = (t_float *)(w[2]);	// Input2	t_float *in3 = (t_float *)(w[3]);	// Input3	t_float *out = (t_float *)(w[4]);	// Output   	t_elixir *x = (t_elixir *)(w[5]);		// Pointer	int n = (int)(w[6]);				// Vector Size			double input[3], val;	short i;							if (x->x_obj.z_disabled) return (w+7);	while (--n) 	{		input[0] = *++in1; 		input[1] = *++in2; 		input[2] = *++in3;		for(i=0; i<3; i++){			if(x->line_toggle[i] == 0)				input[i] = input[i] * x->elixir_mult[i];			else				input[i] = input[i] * perform_interpolation(x, i);		}		val = 0;		for(i=0; i<3; i++)			val = val + input[i];					*++out = val * 0.95;	}	    return (w + 7);								// Return a pointer to the NEXT object in the DSP call chain}// Perform (signal) Methodt_int *elixir_perform4(t_int *w){    t_float *in1 = (t_float *)(w[1]);	// Input1    t_float *in2 = (t_float *)(w[2]);	// Input2	t_float *in3 = (t_float *)(w[3]);	// Input3	t_float *in4 = (t_float *)(w[4]);    t_float *out = (t_float *)(w[5]);	// Output   	t_elixir *x = (t_elixir *)(w[6]);	// Pointer	int n = (int)(w[7]);				// Vector Size			double input[4], val;	short i;							if (x->x_obj.z_disabled) return (w+8);	while (--n) 	{		input[0] = *++in1; 		input[1] = *++in2; 		input[2] = *++in3;		input[3] = *++in4;		for(i=0; i<4; i++){			if(x->line_toggle[i] == 0)				input[i] = input[i] * x->elixir_mult[i];			else				input[i] = input[i] * perform_interpolation(x, i);		}		val = 0;		for(i=0; i<4; i++)			val = val + input[i];					*++out = val * 0.95;	}	    return (w + 8);								// Return a pointer to the NEXT object in the DSP call chain}// Perform (signal) Methodt_int *elixir_perform5(t_int *w){    t_float *in1 = (t_float *)(w[1]);	// Input1    t_float *in2 = (t_float *)(w[2]);	// Input2	t_float *in3 = (t_float *)(w[3]);	// Input3	t_float *in4 = (t_float *)(w[4]);	t_float *in5 = (t_float *)(w[5]);	    t_float *out = (t_float *)(w[6]);	// Output   	t_elixir *x = (t_elixir *)(w[7]);	// Pointer	int n = (int)(w[8]);				// Vector Size			double input[5], val;	short i;							if (x->x_obj.z_disabled) return (w+9);	while (--n) 	{		input[0] = *++in1; 		input[1] = *++in2; 		input[2] = *++in3;		input[3] = *++in4;		input[4] = *++in5;		for(i=0; i<5; i++){			if(x->line_toggle[i] == 0)				input[i] = input[i] * x->elixir_mult[i];			else				input[i] = input[i] * perform_interpolation(x, i);		}		val = 0;		for(i=0; i<5; i++)			val = val + input[i];					*++out = val * 0.95;	}	    return (w + 9);								// Return a pointer to the NEXT object in the DSP call chain}// Perform (signal) Methodt_int *elixir_perform6(t_int *w){    t_float *in1 = (t_float *)(w[1]);	// Input1    t_float *in2 = (t_float *)(w[2]);	// Input2	t_float *in3 = (t_float *)(w[3]);	// Input3	t_float *in4 = (t_float *)(w[4]);	t_float *in5 = (t_float *)(w[5]);		t_float *in6 = (t_float *)(w[6]);		    t_float *out = (t_float *)(w[7]);	// Output   	t_elixir *x = (t_elixir *)(w[8]);	// Pointer	int n = (int)(w[9]);				// Vector Size			double input[6], val;	short i;							if (x->x_obj.z_disabled) return (w+10);	while (--n) 	{		input[0] = *++in1; 		input[1] = *++in2; 		input[2] = *++in3;		input[3] = *++in4;		input[4] = *++in5;		input[5] = *++in6;				for(i=0; i<6; i++){			if(x->line_toggle[i] == 0)				input[i] = input[i] * x->elixir_mult[i];			else				input[i] = input[i] * perform_interpolation(x, i);		}		val = 0;		for(i=0; i<6; i++)			val = val + input[i];					*++out = val * 0.95;	}	    return (w + 10);								// Return a pointer to the NEXT object in the DSP call chain}// Perform (signal) Methodt_int *elixir_perform7(t_int *w){    t_float *in1 = (t_float *)(w[1]);	// Input1    t_float *in2 = (t_float *)(w[2]);	// Input2	t_float *in3 = (t_float *)(w[3]);	// Input3	t_float *in4 = (t_float *)(w[4]);	t_float *in5 = (t_float *)(w[5]);		t_float *in6 = (t_float *)(w[6]);		t_float *in7 = (t_float *)(w[7]);			    t_float *out = (t_float *)(w[8]);	// Output   	t_elixir *x = (t_elixir *)(w[9]);	// Pointer	int n = (int)(w[10]);				// Vector Size			double input[7], val;	short i;							if (x->x_obj.z_disabled) return (w+11);	while (--n) 	{		input[0] = *++in1; 		input[1] = *++in2; 		input[2] = *++in3;		input[3] = *++in4;		input[4] = *++in5;		input[5] = *++in6;		input[6] = *++in7;				for(i=0; i<7; i++){			if(x->line_toggle[i] == 0)				input[i] = input[i] * x->elixir_mult[i];			else				input[i] = input[i] * perform_interpolation(x, i);		}		val = 0;		for(i=0; i<7; i++)			val = val + input[i];					*++out = val * 0.95;	}		    return (w + 11);								// Return a pointer to the NEXT object in the DSP call chain}// Perform (signal) Methodt_int *elixir_perform8(t_int *w){    t_float *in1 = (t_float *)(w[1]);	// Input1    t_float *in2 = (t_float *)(w[2]);	// Input2	t_float *in3 = (t_float *)(w[3]);	// Input3	t_float *in4 = (t_float *)(w[4]);	t_float *in5 = (t_float *)(w[5]);		t_float *in6 = (t_float *)(w[6]);		t_float *in7 = (t_float *)(w[7]);		t_float *in8 = (t_float *)(w[8]);				    t_float *out = (t_float *)(w[9]);	// Output   	t_elixir *x = (t_elixir *)(w[10]);	// Pointer	int n = (int)(w[11]);				// Vector Size			double input[8], val;	short i;							if (x->x_obj.z_disabled) return (w+12);	while (--n) 	{		input[0] = *++in1; 		input[1] = *++in2; 		input[2] = *++in3;		input[3] = *++in4;		input[4] = *++in5;		input[5] = *++in6;		input[6] = *++in7;		input[7] = *++in8;		for(i=0; i<8; i++){			if(x->line_toggle[i] == 0)				input[i] = input[i] * x->elixir_mult[i];			else				input[i] = input[i] * perform_interpolation(x, i);		}		val = 0;		for(i=0; i<8; i++)			val = val + input[i];					*++out = val * 0.95;	}	    return (w + 12);								// Return a pointer to the NEXT object in the DSP call chain}// Perform (signal) Methodt_int *elixir_perform9(t_int *w){    t_float *in1 = (t_float *)(w[1]);	// Input1    t_float *in2 = (t_float *)(w[2]);	// Input2	t_float *in3 = (t_float *)(w[3]);	// Input3	t_float *in4 = (t_float *)(w[4]);	t_float *in5 = (t_float *)(w[5]);		t_float *in6 = (t_float *)(w[6]);		t_float *in7 = (t_float *)(w[7]);		t_float *in8 = (t_float *)(w[8]);		t_float *in9 = (t_float *)(w[9]);					    t_float *out = (t_float *)(w[10]);	// Output   	t_elixir *x = (t_elixir *)(w[11]);	// Pointer	int n = (int)(w[12]);				// Vector Size			double input[9], val;	short i;							if (x->x_obj.z_disabled) return (w+13);	while (--n) 	{		input[0] = *++in1; 		input[1] = *++in2; 		input[2] = *++in3;		input[3] = *++in4;		input[4] = *++in5;		input[5] = *++in6;		input[6] = *++in7;		input[7] = *++in8;		input[8] = *++in9;				for(i=0; i<9; i++){			if(x->line_toggle[i] == 0)				input[i] = input[i] * x->elixir_mult[i];			else				input[i] = input[i] * perform_interpolation(x, i);		}		val = 0;		for(i=0; i<9; i++)			val = val + input[i];					*++out = val * 0.95;	}	    return (w + 13);								// Return a pointer to the NEXT object in the DSP call chain}// Perform (signal) Methodt_int *elixir_perform10(t_int *w){    t_float *in1 = (t_float *)(w[1]);	// Input1    t_float *in2 = (t_float *)(w[2]);	// Input2	t_float *in3 = (t_float *)(w[3]);	// Input3	t_float *in4 = (t_float *)(w[4]);	t_float *in5 = (t_float *)(w[5]);		t_float *in6 = (t_float *)(w[6]);		t_float *in7 = (t_float *)(w[7]);		t_float *in8 = (t_float *)(w[8]);		t_float *in9 = (t_float *)(w[9]);		t_float *in10 = (t_float *)(w[10]);						    t_float *out = (t_float *)(w[11]);	// Output   	t_elixir *x = (t_elixir *)(w[12]);	// Pointer	int n = (int)(w[13]);				// Vector Size			double input[10], val;	short i;							if (x->x_obj.z_disabled) return (w+14);	while (--n) 	{		input[0] = *++in1; 		input[1] = *++in2; 		input[2] = *++in3;		input[3] = *++in4;		input[4] = *++in5;		input[5] = *++in6;		input[6] = *++in7;		input[7] = *++in8;		input[8] = *++in9;		input[9] = *++in10;				for(i=0; i<10; i++){			if(x->line_toggle[i] == 0)				input[i] = input[i] * x->elixir_mult[i];			else				input[i] = input[i] * perform_interpolation(x, i);		}		val = 0;		for(i=0; i<10; i++)			val = val + input[i];					*++out = val * 0.95;	}	    return (w + 14);								// Return a pointer to the NEXT object in the DSP call chain}// Interpolator...float perform_interpolation(t_elixir *x, short inletNumber){	double xx;	if (x->line_samplesDone[inletNumber] != x->line_samplenum[inletNumber])	{		xx = x->line_result[inletNumber] + x->line_step[inletNumber];		x->line_samplesDone[inletNumber]= x->line_samplesDone[inletNumber] + 1;		x->line_result[inletNumber] = xx;				// store the result									return(xx);	}	return(x->line_result[inletNumber]);}// DSP Methodvoid elixir_dsp(t_elixir *x, t_signal **sp, short *count){	short i;	x->elixir_sr = sp[0]->s_sr;		for(i=0; i<10; i++){		x->line_samplenum[i] = 0;		x->line_samplesDone[i] = 0; 	}		switch (x->elixir_numInlets) {		case 2:			dsp_add(elixir_perform2, 5, sp[0]->s_vec-1, sp[1]->s_vec-1, 				sp[2]->s_vec-1, x, sp[0]->s_n+1);			// Add Perform Method to the DSP Call Chain			break;		case 3:			dsp_add(elixir_perform3, 6, sp[0]->s_vec-1, sp[1]->s_vec-1, sp[2]->s_vec-1,				sp[3]->s_vec-1, x, sp[0]->s_n+1);			// Add Perform Method to the DSP Call Chain			break;		case 4:			dsp_add(elixir_perform4, 7, sp[0]->s_vec-1, sp[1]->s_vec-1, sp[2]->s_vec-1, sp[3]->s_vec-1,				sp[4]->s_vec-1, x, sp[0]->s_n+1);			// Add Perform Method to the DSP Call Chain			break;		case 5:			dsp_add(elixir_perform5, 8, sp[0]->s_vec-1, sp[1]->s_vec-1, sp[2]->s_vec-1, sp[3]->s_vec-1, sp[4]->s_vec-1,				sp[5]->s_vec-1, x, sp[0]->s_n+1);			// Add Perform Method to the DSP Call Chain			break;		case 6:			dsp_add(elixir_perform6, 9, sp[0]->s_vec-1, sp[1]->s_vec-1, sp[2]->s_vec-1, sp[3]->s_vec-1, sp[4]->s_vec-1, sp[5]->s_vec-1,				sp[6]->s_vec-1, x, sp[0]->s_n+1);			// Add Perform Method to the DSP Call Chain			break;		case 7:			dsp_add(elixir_perform7, 10, sp[0]->s_vec-1, sp[1]->s_vec-1, sp[2]->s_vec-1, sp[3]->s_vec-1, sp[4]->s_vec-1, sp[5]->s_vec-1, sp[6]->s_vec-1,				sp[7]->s_vec-1, x, sp[0]->s_n+1);			// Add Perform Method to the DSP Call Chain			break;		case 8:			dsp_add(elixir_perform8, 11, sp[0]->s_vec-1, sp[1]->s_vec-1, sp[2]->s_vec-1, sp[3]->s_vec-1, sp[4]->s_vec-1, sp[5]->s_vec-1, sp[6]->s_vec-1, sp[7]->s_vec-1,				sp[8]->s_vec-1, x, sp[0]->s_n+1);			// Add Perform Method to the DSP Call Chain			break;		case 9:			dsp_add(elixir_perform9, 12, sp[0]->s_vec-1, sp[1]->s_vec-1, sp[2]->s_vec-1, sp[3]->s_vec-1, sp[4]->s_vec-1, sp[5]->s_vec-1, sp[6]->s_vec-1, sp[7]->s_vec-1, sp[8]->s_vec-1,				sp[9]->s_vec-1, x, sp[0]->s_n+1);			// Add Perform Method to the DSP Call Chain			break;		case 10:			dsp_add(elixir_perform10, 13, sp[0]->s_vec-1, sp[1]->s_vec-1, sp[2]->s_vec-1, sp[3]->s_vec-1, sp[4]->s_vec-1, sp[5]->s_vec-1, sp[6]->s_vec-1, sp[7]->s_vec-1, sp[8]->s_vec-1, sp[9]->s_vec-1,				sp[10]->s_vec-1, x, sp[0]->s_n+1);			// Add Perform Method to the DSP Call Chain			break;	}}