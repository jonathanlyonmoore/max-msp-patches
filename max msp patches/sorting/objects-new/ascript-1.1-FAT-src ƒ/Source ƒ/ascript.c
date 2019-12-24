/* * ascript.c * Max External, written with CodeWarrior 8 * * Copyright � 1996 by  * G�nter Obiltschnig (g.obiltschnig@jk.uni-linz.ac.at) * and Telecooperations Workgroup, University of Linz * * History: * 960425: first release * */  #include <A4Stuff.h>#include <SetUpA4.h>#include <Memory.h>#include <Files.h>#include <Resources.h>#include <TextEdit.h>#include <OSA.h>#include <AppleScript.h>#include "ext.h"#include "ext_common.h"#include "ext_user.h"#include "edit.h"#pragma d0_pointers on                            /* to be THINK-C compatible *//* * Macros * */ #define kAssistStrs			2112                  /* ID of STR# resource containing assistance info */#define kFirstInlet			1#define kFirstOutlet		2#define kScriptResType		kOSAScriptResourceType    #define kScriptResID		128                   /* so we're compatible with the Script Editor */#define kScriptFileType     'osas'#define kScriptFileCreator  'ToyS'#define kASEnglishDialect	"AppleScript English" /* AppleScript English dialect name */#define kASMinStackSize			8*1024#define kASPreferredStackSize	16*1024#define kASMaxStackSize			32*1024#define kASMinHeapSize			8*1024#define kASPreferredHeapSize	128*1024#define kASMaxHeapSize			kASDefaultMaxHeapSize#ifndef kComponentNotFound#define kComponentNotFound  -1#endif/* * Types * */typedef struct object Object;typedef struct ed     Ed;typedef struct qelem  Qelem;typedef struct binbuf Binbuf;typedef struct aScript{	/* object specific stuff */	Object  obj;	void*   resOut;               /* outlet for script result */		/* osa/applescript stuff */	OSAID             contextID;  /* our own context for executing our script */	OSAID             scriptID;   /* our script's ID */	AEDesc            scriptText; /* the script's source text */	AEDesc            setGetText; /* temporarily used for set/get */		/* more stuff */	Qelem*  compQE;               /* Qelem for compiling the script */	Qelem*  sourceQE;             /* Qelem for de-compiling the script */	Qelem*  execQE;               /* Qelem for executing the script */		Qelem*  setGetQE;             /* Qelem for set/get */	char    fileName[32];         /* name of script's file (C string) */	short   vRef;                 /* volume refnum of script's file */	Symbol* arg;                  /* Symbol given as argument */	Ed*     ed;                   /* the script editor */} AScript;/* * Prototypes * */OSAError InitAppleScript();OSAError SetAppleScriptDialect(char* dialectName);void  OpenEditor(AScript* x);void  SendScriptResult(AScript* x, OSAID resultID);void  CompileScript(AScript* x);void  DecompileScript(AScript* x);void  ExecScript(AScript* x);void  ExecSetGet(AScript* x);void  LoadScript(AScript* x);void  SaveScript(AScript* x);void  ShowScriptErrorInfo(AScript* x);void* AScriptNew(Symbol* name);void  AScriptFree(AScript* x);void  AScriptBang(AScript* x);void  AScriptSave(AScript* x, Binbuf* dest);void  AScriptRestore(AScript* x, Symbol* s, short argc, Atom* argv);void  AScriptInfo(AScript* x, struct patcher* p, struct box* b);void  AScriptEdClose(AScript* x, char** text, long size);void* AScriptEdSave(AScript* x, char** text, long size, char* fName, short vol);void  AScriptAssist(AScript* x, void* box, long msg, long arg, char* str);void  AScriptSet(AScript* x, Symbol *s, short argc, Atom* argv);void  AScriptGet(AScript* x, Symbol *s, short argc, Atom* argv);void  CtoPStr(char* cStr, unsigned char* pStr);/* * Globals * */fptr*   FNS;                      /* Max's function macro table */void*   class;                    /* the class variable */Symbol* bindMe;                   /* needed for save/restore */Symbol* intSym;                   /* to prevent calling gensym() each time we need it */Symbol* floatSym;ComponentInstance gScriptComp;    /* we use one component for all objects */int               gInstanceCount; /* we count all our instances. when the */                                  /* last instance is destroyed, we can   */                                  /* close the scripting component.       */void main(fptr* f)	/* The entry point of the code resource. */	/* Called at launch time.                */{	EnterCodeResource(); /* usual A4 stuff */	PrepareCallback();		FNS = f;             /* set Max's globals */		/* define class & methods */	setup(&class, (method) AScriptNew, (method) AScriptFree, (short) sizeof(AScript), 0L, A_DEFSYM, 0);	addbang((method) AScriptBang);	addmess((method) AScriptSave,    "save",     A_CANT,  0);	addmess((method) AScriptRestore, "restore",  A_GIMME, 0);	addmess((method) AScriptInfo,    "info",     A_CANT,  0);	addmess((method) AScriptAssist,  "assist",   A_CANT,  0);	addmess((method) AScriptEdClose, "edclose",  A_CANT,  0);	addmess((method) AScriptEdSave,  "edsave",   A_CANT,  0);	addmess((method) AScriptSet,     "set",      A_GIMME, 0);	addmess((method) AScriptGet,     "get",      A_GIMME, 0);		rescopy('STR#', kAssistStrs);	finder_addclass("Control", "ascript");	bindMe   = gensym("#X");    /* symbol that we'll use for restore */	intSym   = gensym("int");   /* frequently used symbols */	floatSym = gensym("float");		gInstanceCount = 0;	/* Initialization of AppleScript is done when the first */	/* instance of our object is created. */		/* say hello */#ifdef powerpc	post("ascript Object v1.1 (PPC) Copyright � 1996-97 by G�nter Obiltschnig.");#else	post("ascript Object v1.1 (68K) Copyright � 1996-97 by G�nter Obiltschnig.");#endif	ExitCodeResource();} /* main */OSAError InitAppleScript()	/* Open the AppleScript OSA component */{	ComponentDescription descr;	Component            aComponent;		descr.componentType         = kOSAComponentType;	descr.componentSubType      = kAppleScriptSubtype;	descr.componentManufacturer = (OSType) 0;	descr.componentFlags        = kOSASupportsCompiling + 								  kOSASupportsGetSource + 								  kOSASupportsConvenience + 								  kOSASupportsEventHandling;	descr.componentFlagsMask    = descr.componentFlags;		aComponent = FindNextComponent(NULL, &descr);			if (!aComponent)		return kComponentNotFound;	else	{		gScriptComp = OpenComponent(aComponent);						if (!gScriptComp) return kComponentNotFound;	}		ASInit(gScriptComp, kOSAModeNull,	       kASMinStackSize, kASPreferredStackSize, kASMaxStackSize,	       kASMinHeapSize,  kASPreferredHeapSize,  kASMaxHeapSize);			return noErr;} /* InitAppleScript */OSAError SetAppleScriptDialect(char* theDialectName)	/* Make the specified dialect the current dialect */{	AEDesc    dialectList, dialectCodeDesc, dialectInfoDesc;	OSAError  err;	long      i, j, n;	short     dialectCode;	AEKeyword key;	char      dialectName[256];		err = OSAAvailableDialectCodeList(gScriptComp, &dialectList);	if (err == noErr)	{		err = AECountItems(&dialectList, &n);		/* post("ascript: %d dialects found", (short) n); */		/* search dialect */		for (i = 1; i <= n; i++)		{			err = AEGetNthDesc(&dialectList, i, typeShortInteger, &key, &dialectCodeDesc);			if (err == noErr)			{				dialectCode = **(short**) dialectCodeDesc.dataHandle;				/* post("ascript: dialect code: %d", dialectCode); */				dialectInfoDesc.descriptorType = typeChar;				dialectInfoDesc.dataHandle     = NULL;				err = OSAGetDialectInfo(gScriptComp, dialectCode, keyOSADialectName, &dialectInfoDesc);				if (err == noErr)				{					/* copy dialect name */					for (j = 0; j < GetHandleSize(dialectInfoDesc.dataHandle); j++)						dialectName[j] = (*(char**) dialectInfoDesc.dataHandle)[j];					dialectName[j] = '\0';					/* post("ascript: dialect name: %s", dialectName); */					/* compare dialect names */					for (j = 0; dialectName[j] && dialectName[j] == theDialectName[j]; j++);					if (!dialectName[j])					{						err = OSASetCurrentDialect(gScriptComp, dialectCode);						if (err != noErr)						{							error("ascript: can't set dialect (%d)", (short) err);						}					}				}				else				{					error("ascript: can't get dialect info (%d)", (short) err);				}			}			else			{				error("ascript: can't get dialect code (%d)", (short) err);			}		}	}	else	{		error("ascript: can't get dialect code list (%d)", (short) err);	}	return err;		} /* SetAppleScriptDialect */void OpenEditor(AScript* x){	/* create a new editor window */	if (x->ed)	{		ed_vis(x->ed);	}	else	{		x->ed = ed_new((t_object*) x);		wind_filename(x->ed->e_wind, x->fileName, x->vRef, -1);		if (x->scriptText.dataHandle)		{			ed_settext(x->ed, x->scriptText.dataHandle, GetHandleSize(x->scriptText.dataHandle));		}		else if (x->scriptID != kOSANullScript)		{			qelem_set(x->sourceQE); /* get source from compiled script */		}	}} /* OpenEditor */void SendScriptResult(AScript* x, OSAID resultID)	/* Triy to convert the script's result value into a max message */	/* and send the message through our outlet.                     */{	AEDesc   result;	short    prevLock;	Atom     outVal;	char     resultStr[32];	int      len;	char*    resType;	OSAError err;		/* Convert result value and send it through outlet. */	err = OSACoerceToDesc(gScriptComp, resultID, typeWildCard, kOSAModeNull, &result);	if (err == noErr)	{		prevLock = lockout_set(1); /* entering critical region (outlet_anything) */		switch (result.descriptorType)		{		case typeBoolean:      /* converted to int 0/1 */			SETLONG(&outVal, (long) **((char **) result.dataHandle));			outlet_anything(x->resOut, intSym, 1, &outVal);			break;		case typeTrue:         /* converted to int 1 */			SETLONG(&outVal, 1L);			outlet_anything(x->resOut, intSym, 1, &outVal);		case typeFalse:        /* converted to int 0 */			SETLONG(&outVal, 1L);			outlet_anything(x->resOut, intSym, 1, &outVal);				case typeShortInteger: /* converted to int n */			SETLONG(&outVal, (long) **((short **) result.dataHandle));			outlet_anything(x->resOut, intSym, 1, &outVal);			break;		case typeLongInteger:  /* converted to int n */			SETLONG(&outVal, **((long **) result.dataHandle));			outlet_anything(x->resOut, intSym, 1, &outVal);			break;		case typeFloat:        /* converted to float n */			SETFLOAT(&outVal, (float) **((double **) result.dataHandle));			outlet_anything(x->resOut, floatSym, 1, &outVal);			break;		case typeChar:         /* converted to a symbol */			len = GetHandleSize(result.dataHandle);			if (len > 31) len = 31;			BlockMove(*result.dataHandle, resultStr, len); resultStr[len] = '\0';			outlet_anything(x->resOut, gensym(resultStr), 0, NIL);			break;		case typeNull:         /* nothing is sent */			break;		default:               /* can't convert this */			resType = (char *) &result.descriptorType;			error("ascript: can't convert result value of type '%c%c%c%c'", 			      resType[0], resType[1], resType[2], resType[3]);		}		lockout_set(prevLock);     /* end of critical region */		OSADispose(gScriptComp, resultID);		AEDisposeDesc(&result);	}	else	{		error("ascript: can't convert result value (%d)", (short) err);	}} /* SendScriptResult */void CompileScript(AScript* x)	/* QElem function to compile and save the script. */	/* This MUST be done in a qelem function and not  */	/* in a callback, otherwise calls to OSACompile   */	/* may fail or crash the system (even if not      */	/* called at interrupt level).                    */{	OSAError err;	short    prevLock;	EnterCallback();		post("ascript: compiling script");	/* prevLock = lockout_set(1); /* entering critical region */	err = OSACompile(gScriptComp, &x->scriptText, kOSAModeNull, &x->scriptID);	/* lockout_set(prevLock);     /* end of critical region */	if (err == errOSAScriptError) 	{		ShowScriptErrorInfo(x);		x->scriptID = kOSANullScript;	}	else if (err == noErr)	{		int i;				post("ascript: compilation done");		SaveScript(x);	}	else	{		error("ascript: compilation failed (%d)", (short) err);		x->scriptID = kOSANullScript;	}		ExitCallback();} /* CompileScript */void DecompileScript(AScript* x)	/* QElem function to decompile a script.          */	/* This MUST be done in a qelem function and not  */	/* in a callback, otherwise calls to OSAGetSource */	/* may fail or crash the system (even if not      */	/* called at interrupt level).                    */{	OSAError err;		EnterCallback();		if (x->ed)	{		post("ascript: decompiling script");		err = OSAGetSource(gScriptComp, x->scriptID, typeChar, &x->scriptText);		if (err == noErr)		{			ed_settext(x->ed, x->scriptText.dataHandle, GetHandleSize(x->scriptText.dataHandle));		}		else		{			error("ascript: can't get script's source (%d)", (short) err);		}	}		ExitCallback();} /* DecompileScript */void ExecScript(AScript* x)	/* QElem function to execute the script.               */	/* OSAExecute must never be called at interrupt level. */{	OSAError err;	OSAID    resultID;		EnterCallback();		if (x->scriptID != kOSANullScript)	{		/* execute the script */		err = OSAExecute(gScriptComp, x->scriptID, x->contextID, kOSAModeNull, &resultID);		/* convert the script result to a Max message and send it thru outlet */		if (err == noErr)		{			SendScriptResult(x, resultID);		}		else		{			error("ascript: error while executing script (%d)", (short) err);		}	}	else	{		error("ascript: no compiled script");	}		ExitCallback();} /* ExecScript */void ExecSetGet(AScript* x)	/* QElem function to compile & execute set/get script.  */	/* The script is expected to be stored in x->setGetText */	/* and is destroyed after execution.                    */{	OSAError err;	OSAID    resultID;	short    prevLock;	EnterCallback();		/* execute the script */	/* prevLock = lockout_set(1); /* entering critical region */	err = OSACompileExecute(gScriptComp, &x->setGetText, x->contextID, kOSAModeNull, &resultID);	/* lockout_set(prevLock);     /* end of critical region */	/* convert the script result to a Max message and send it thru outlet */	if (err == noErr)	{		SendScriptResult(x, resultID);	}	else	{		error("ascript: can't execute set/get (%d)", (short) err);	}	/* the script's source is no longer needed. */	disposhandle(x->setGetText.dataHandle);		ExitCallback();} /* CompileExecScript */void LoadScript(AScript* x)	/* Loads a script from a file */{	Str32    fileName;	short    fRef;	OSAError err;	AEDesc   scriptDesc;		CtoPStr(x->fileName, fileName);	fRef = HOpenResFile(x->vRef, 0, fileName, fsRdPerm);	err  = ResError();	if (err == noErr)	{		scriptDesc.descriptorType = typeOSAGenericStorage;		scriptDesc.dataHandle     = Get1Resource(kScriptResType, kScriptResID);		err = ResError();		if (err != noErr)		{			error("ascript: can't read script (%d)", (short) err);			CloseResFile(fRef);			return;		}				err = OSALoad(gScriptComp, &scriptDesc, kOSAModeNull, &x->scriptID);		if (err != noErr)		{			error("ascript: can't load script (%d)", (short) err);			x->scriptID = kOSANullScript;		}		ReleaseResource(scriptDesc.dataHandle);		CloseResFile(fRef);	}	else	{		error("ascript: can't open script file (%d)", (short) err);		return;	}} /* LoadScript */void SaveScript(AScript* x)	/* Saves the script to a file */{	Str32    fileName;	short    fRef;	AEDesc   scriptDesc;	OSAError err;	Handle   oldRes;	FInfo    fndrInfo;		CtoPStr(x->fileName, fileName);	HCreateResFile(x->vRef, 0, fileName);	fRef = HOpenResFile(x->vRef, 0, fileName, fsRdWrPerm);	err  = ResError();	if (err == noErr)	{		/* Check if there's already a script saved */		oldRes = Get1Resource(kScriptResType, kScriptResID);				/* Get script data */		err = OSAStore(gScriptComp, x->scriptID, typeOSAGenericStorage,		               kOSAModeDontStoreParent, &scriptDesc);		if (err == noErr && scriptDesc.dataHandle)		{			if (oldRes) RemoveResource(oldRes);			AddResource(scriptDesc.dataHandle, kScriptResType, kScriptResID, (ConstStr255Param) "");			err = ResError();			if (err == noErr)			{				/* give our file the right type and creator */				err = HGetFInfo(x->vRef, 0, fileName, &fndrInfo);				if (err == noErr)				{					fndrInfo.fdType    = kScriptFileType;					fndrInfo.fdCreator = kScriptFileCreator;					err = HSetFInfo(x->vRef, 0, fileName, &fndrInfo);				}			}			else			{				error("ascript: can't write script data (%d)", (short) err);			}		}		CloseResFile(fRef);	}	else	{		error("ascript: can't open script file (%d)", (short) err);	}} /* SaveScript */	void ShowScriptErrorInfo(AScript* x)	/* Called if compilation fails. Displays an */	/* appropriate error message.               */{	OSErr    scrErr;	OSAError err;	Handle   errMsg;	AEDesc   desc, recordDesc;	DescType actualType;	Size     actualSize;	short    startPos, endPos;		err = OSAScriptError(gScriptComp, kOSAErrorNumber, typeShortInteger, &desc);	scrErr = **((OSErr**) desc.dataHandle);	AEDisposeDesc(&desc);	err = OSAScriptError(gScriptComp, kOSAErrorMessage, typeChar, &desc);	errMsg = newhandle(GetHandleSize(desc.dataHandle) + 1);	BlockMove(*desc.dataHandle, *errMsg, GetHandleSize(desc.dataHandle));	(*errMsg)[GetHandleSize(desc.dataHandle)] = '\0';	AEDisposeDesc(&desc);	err = OSAScriptError(gScriptComp, kOSAErrorRange, typeOSAErrorRange, &desc);	err = AECoerceDesc(&desc, typeAERecord, &recordDesc);	AEDisposeDesc(&desc);	err = AEGetKeyPtr(&recordDesc, keyOSASourceStart, typeShortInteger, &actualType, 	                    &startPos, sizeof(startPos), &actualSize);	err = AEGetKeyPtr(&recordDesc, keyOSASourceEnd,   typeShortInteger, &actualType,	                    &endPos,   sizeof(endPos),   &actualSize);	AEDisposeDesc(&recordDesc);	error("ascript: AppleScript error %d: ", (short) scrErr);	post(*errMsg);		OpenEditor(x);	TESetSelect(startPos, endPos, x->ed->e_teh);	disposhandle(errMsg);} /* ShowScriptErrorInfo */	void* AScriptNew(Symbol* name)	/* create a new instance of our object */{	AScript* x;	OSAError err;	AEDesc   contName;		EnterCallback();		gInstanceCount++;		if (gInstanceCount == 1)	{		/* Initialize OSA/AppleScript */		err = InitAppleScript();		if (err != noErr)		{			error("ascript: can't initialize AppleScript (%d)", (short) err);		}		else		{			err = SetAppleScriptDialect(kASEnglishDialect);			if (err != noErr)			{				error("ascript: can't set AppleScript English dialect (%d)", (short) err);			}		}	}		x = (AScript*) newobject(class);	x->resOut      = outlet_new(x, 0L);	x->fileName[0] = '\0';	x->vRef        = 0;	x->arg         = name;	x->ed          = NULL;		/* our object gets its own scripting context! */	contName.descriptorType = typeNull;	contName.dataHandle     = NULL;	err = OSAMakeContext(gScriptComp, &contName, kOSANullScript, &x->contextID);	if (err != noErr)	{		error("ascript: can't create script context (%d)", (short) err);		x->contextID = kOSANullScript;	}		x->scriptID = kOSANullScript;	x->scriptText.descriptorType = typeChar;	x->scriptText.dataHandle     = NULL;	x->setGetText.descriptorType = typeChar;	x->setGetText.dataHandle     = NULL;		x->compQE   = qelem_new(x, (method) CompileScript);	x->sourceQE = qelem_new(x, (method) DecompileScript);	x->execQE   = qelem_new(x, (method) ExecScript);	x->setGetQE = qelem_new(x, (method) ExecSetGet);		bindMe->s_thing = (Object*) x; /* so you'll get the restore message */		ExitCallback();	return x;} /* AScriptNew */void AScriptFree(AScript* x)	/* destroy our object */	/* if there are no other ascript objects, close the scripting component */{	EnterCallback();		gInstanceCount--;		qelem_free(x->compQE);	qelem_free(x->sourceQE);	qelem_free(x->execQE);	qelem_free(x->setGetQE);		if (x->scriptText.dataHandle) 	{		disposhandle(x->scriptText.dataHandle);	}	if (x->ed)	{		freeobject((t_object*) x->ed);	}	if (x->contextID != kOSANullScript)	{		OSADispose(gScriptComp, x->contextID);	}	if (x->scriptID != kOSANullScript)	{		OSADispose(gScriptComp, x->scriptID);	}	if (gInstanceCount == 0)	{		CloseComponent(gScriptComp);	}		ExitCallback();}void AScriptSave(AScript* x, Binbuf* dest)	/* save state of our object */{	int   i;	EnterCallback();		binbuf_vinsert(dest, "sss", gensym("#N"), gensym("ascript"), x->arg); /* recreate me */	binbuf_vinsert(dest, "sss",  bindMe, gensym("restore"),               /* restore state */	               gensym(x->fileName));		ExitCallback();} /* AScriptSave */void  AScriptRestore(AScript* x, Symbol* s, short argc, Atom* argv)	/* restore state of our object */{	int i;		EnterCallback();		for (i = 0; x->fileName[i] = argv[0].a_w.w_sym->s_name[i]; i++) 		bindMe->s_thing = 0; /* somewhat important */		if (x->fileName[0] && locatefiletype(x->fileName, &x->vRef, kScriptFileType, 0L) == 0)	{		LoadScript(x);	}	else	{		x->vRef = 0;	}		ExitCallback();} /* AScriptRestore */void AScriptBang(AScript* x)	/* execute the script */{	EnterCallback();		qelem_set(x->execQE);	ExitCallback();} /* AScriptBang */void  AScriptInfo(AScript* x, struct patcher* p, struct box* b)	/* User chose "Get Info" from file menu.           */	/* Open a text editor window with the script text. */{	SFTypeList typeList;	OSType     destType;	int        i;		EnterCallback();		if (x->ed)	{		/* if the window is open, make it the frontmost */		ed_vis(x->ed);	}	else if (x->scriptText.dataHandle == NULL && x->scriptID == kOSANullScript)	{		open_promptset("Open an existing script?");		typeList[0] = kScriptFileType;		if (open_dialog(x->fileName, &x->vRef, &destType, typeList, (short) 1)  == 0)		{			LoadScript(x);		}		else		{			x->ed = ed_new((t_object*) x);		}	}	else	{		OpenEditor(x);	}		ExitCallback();} /* AScriptInfo */void  AScriptEdClose(AScript* x, char** text, long size)	/* Called when the ed window is about to be closed. */{	EnterCallback();		x->ed = NULL;		ExitCallback();} /* AScriptEdClose */void* AScriptEdSave(AScript* x, char** text, long size, char* fileName, short vol)	/* Contents of ed window need to be saved */{	register int i;		EnterCallback();		/* get the source text */	if (x->scriptText.dataHandle) 	{		disposhandle(x->scriptText.dataHandle);	}	x->scriptText.dataHandle = newhandle(size);	BlockMove(*text, *x->scriptText.dataHandle, size);		for (i = 0; x->fileName[i] = fileName[i]; i++);	x->vRef = vol;		qelem_set(x->compQE);	/* update window name (for save as) */	wind_filename(x->ed->e_wind, x->fileName, x->vRef, -1);	ExitCallback();		return (void*) 1; /* Ed doesn't need to save the script */} /* AScriptEdSave */void  AScriptAssist(AScript* x, void* box, long msg, long arg, char* str)	/* Describe inlets & outlets */{	EnterCallback();	assist_string(kAssistStrs, msg, arg, kFirstInlet, kFirstOutlet, str);	ExitCallback();} /* AScriptAssist */void  AScriptSet(AScript* x, Symbol *s, short argc, Atom* argv)	/* set a global in our script context */{	char  script[256];	char* var;	long  len;		EnterCallback();		if (argc == 2)	{		if (argv[0].a_type = A_SYM)		{			var = argv[0].a_w.w_sym->s_name;			switch (argv[1].a_type)			{			case A_LONG:				sprintf(script, "global %s\rset %s to %ld", var, var, argv[1].a_w.w_long);				break;			case A_FLOAT:				sprintf(script, "global %s\rset %s to %lf", var, var, argv[1].a_w.w_float);				break;			case A_SYM:				sprintf(script, "global %s\rset %s to %s", var, var, argv[1].a_w.w_sym->s_name);				break;			}			for (len = 0; script[len]; len++); script[len] = '\r'; len++;			x->setGetText.dataHandle = newhandle(len);			BlockMove(script, *x->setGetText.dataHandle, len);						qelem_set(x->setGetQE);		}	}		ExitCallback();} /* AScriptSet */void  AScriptGet(AScript* x, Symbol *s, short argc, Atom* argv)	/* get the value of a global in our script context */{	char  script[256];	char* var;	long  len;		EnterCallback();		if (argc == 1 && argv[0].a_type == A_SYM)	{		var = argv[0].a_w.w_sym->s_name;		sprintf(script, "global %s\rreturn %s", var, var);		for (len = 0; script[len]; len++); script[len] = '\r'; len++;		x->setGetText.dataHandle = newhandle(len);		BlockMove(script, *x->setGetText.dataHandle, len);					qelem_set(x->setGetQE);	}		ExitCallback();} /* AScriptGet */void  CtoPStr(char* cStr, unsigned char* pStr)	/* Convert a C string to a Pascal string */{	register int i;		for (i = 0; cStr[i]; i++) pStr[i + 1] = cStr[i];	pStr[0] = (char) i;} /* CtoPStr */