<CsoundSynthesizer><CsInstruments>;===========; toot01.orc ;===========sr		=		44100kr		=		2205ksmps 	=		20nchnls	=		2		instr 1		kgain  	invalue 1ktrnsp  invalue 2kpitch	=		p4+ktrnsp*1000		kenv	linen	1,.1,p3,.1ipan	linrand 	1ileft	= ipaniright  = 	1-ipana1		oscil	kenv*kgain*4000, kpitch, 1		outs		a1*ileft, a1*iright				endin</CsInstruments><CsScore>;===========; toot01.sco;===========f1	0	4096	10 1		; use GEN10 to compute a sine wave;ins 	dur	pitchi1	0	1  500f0 600				; null function to keep running for realtime eventse					; indicates the end of the score</CsScore></CsoundSynthesizer>