// MSP External: radians.c// conversion to and from radians and degrees// T.Place (4/07/2001)// 	  	v1.1 (07/20/01) - update for Max/MSP 4//		v1.2 (12/14/01) - minor revisions#include "ext.h"			// Required for all Max External Objects#include "z_dsp.h"		// Required for all MSP External Objects#include <math.h>	#include "ext_strings.h"	// Used for the assistance strings		void *radians_class;		// Required. Global pointing to this classtypedef struct _radians	// Data Structure for this object{	t_pxobject x_obj;	void *radians_out;	// Floating-point outlet	int	radians_sr;	// 1/2 the SR	radians_mode;		// Current mode} t_radians;// Prototypes for methods: need a method for each incoming message type:t_int *radians_perform(t_int *w);							// An MSP Perform (signal) Methodvoid radians_dsp(t_radians *x, t_signal **sp, short *count);		// DSP Methodvoid radians_assist(t_radians *x, void *b, long m, long a, char *s);	// Assistance Methodvoid *radians_new(long myArg);							// New Object Creation Methodvoid radians_float(t_radians *x, double value);					// Float methodvoid radians_mode(t_radians *x, int value);					// "mode" methodvoid radians_free(t_radians *x);/************************************************************************************/// Main() Functionvoid main(void)				// main recieves a copy of the Max function macros table{	setup((t_messlist **)&radians_class, (method)radians_new, (method)radians_free, (short)sizeof(t_radians), 0L, A_DEFLONG, 0);	    						addmess((method)radians_dsp, "dsp", A_CANT, 0);				// Bind method "radians_dsp" to the DSP call from MSP	addmess((method)radians_assist,"assist", A_CANT, 0);			// Bind method "radians_assist" to assistance calls	addfloat((method)radians_float);							// Bind coef method to float input	addmess((method)radians_mode,"mode", A_LONG, 0);			// Bind mode method	addmess((method)inspector_open, "info", A_CANT, 0);			// bind method for opening the license window		dsp_initclass();											// Setup object's class to work with MSP	post("tap.radians~ v1.2 - Copyright � 2001 by Timothy A. Place");	// Print to the Max Window...	post("     Impact Center @ University of Missouri - Kansas City / Silicon Prairie Intermedia");}/************************************************************************************/// Object Creation Methodvoid *radians_new(long myArg){	t_radians *x = (t_radians *)newobject(radians_class);	dsp_setup((t_pxobject *)x,1);	x->radians_out = floatout(x);					// Create a floating-point Outlet	outlet_new((t_object *)x, "signal");	x->radians_mode = myArg;					// default mode	x->radians_sr = sys_getsr();	return (x);}/************************************************************************************/// Methods bound to input/inlets// Method for Assistance Messagesvoid radians_assist(t_radians *x, void *b, long msg, long arg, char *dst){	if(msg==1) 	// Inlets		strcpy(dst, "(signal/float) Input");	else if(msg==2){ // Outlets		switch(arg){			case 0: strcpy(dst, "(signal) Output"); break;			case 1: strcpy(dst, "(float) Output"); break;		}	}}// mode methodvoid radians_mode(t_radians *x, int value){	x->radians_mode = value;}// method for float inputvoid radians_float(t_radians *x, double value){		switch (x->radians_mode) 	{		case 0:			value = value * (3.141593 / (x->radians_sr * 0.5));	// hz to radians			break;		case 1:			value = (value * x->radians_sr) / 6.283186; // radians to hz			break;		case 2:			value = (value * 3.141593) / 180.;			break;		case 3:			value = (value * 180.) / 3.141593;			break;	}	outlet_float(x->radians_out, value);		// output the result}// Memory Deallocationvoid radians_free(t_radians *x){	notify_free((t_object *)x);	dsp_free((t_pxobject *)x);}// Perform (signal) Method - delay is a constant (not a signal)t_int *radians_perform(t_int *w){	t_float *in, value, *out;	int n;	t_radians *x = (t_radians *)(w[1]);			in = (t_float *)(w[2]);	out = (t_float *)(w[3]);	n = (int)(w[4]);		if (x->x_obj.z_disabled) goto out;			while (--n) 	{		value = *++in;										// Input		switch (x->radians_mode) 		{			case 0:				value = value * (3.141593 / (x->radians_sr * 0.5));	// hz to radians				break;			case 1:				value = (value * x->radians_sr) / 6.283186; 		// radians to hz				break;			case 2:				value = (value * 3.141593) / 180.;				break;			case 3:				value = (value * 180.) / 3.141593;				break;		}		*++out = value;	}out:	return (w+5);}// DSP Methodvoid radians_dsp(t_radians *x, t_signal **sp, short *count){	int val = sp[0]->s_sr;	x->radians_sr = val;	if (count[0])		dsp_add(radians_perform, 4, x, sp[0]->s_vec-1, sp[1]->s_vec-1, sp[0]->s_n + 1);   	else   		val = 0;}