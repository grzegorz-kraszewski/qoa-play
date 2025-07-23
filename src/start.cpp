/*-------------------------------------*/
/* Minimal C++ startup for AmigaOS 3.x */
/* RastPort, 2024                      */
/*-------------------------------------*/

#include "rptypes.h"

#include <proto/exec.h>
#include <proto/dos.h>
#include <workbench/startup.h>

Library *SysBase;
Library *DOSBase;
void* TaskPool = NULL;

extern int32 Main(WBStartup *wbmsg);

#define HAVE_GLOBAL_CONSTRUCTORS

#ifdef HAVE_GLOBAL_CONSTRUCTORS
extern void (*__CTOR_LIST__[])(void);
extern void (*__DTOR_LIST__[])(void);
void GlobalConstructors();
void GlobalDestructors();
#endif

__attribute__((saveds)) int32 Start(void)
{
	Process *myproc = NULL;
	WBStartup *wbmsg = NULL;
	BOOL have_shell = FALSE;
	int32 result = RETURN_OK;

	SysBase = *(Library**)4L;
	myproc = (Process*)FindTask(NULL);

	if (myproc->pr_CLI) have_shell = TRUE;

	if (!have_shell)
	{
		WaitPort(&myproc->pr_MsgPort);
		wbmsg = (WBStartup*)GetMsg(&myproc->pr_MsgPort);
	}

	result = RETURN_FAIL;

	if (DOSBase = OpenLibrary("dos.library", 39))
	{
		if (TaskPool = CreatePool(MEMF_ANY, 4096, 2048))
		{
			#ifdef HAVE_GLOBAL_CONSTRUCTORS
			GlobalConstructors();
			#endif
			result = Main(wbmsg);
			#ifdef HAVE_GLOBAL_CONSTRUCTORS
			GlobalDestructors();
			#endif
			DeletePool(TaskPool);
		}

		CloseLibrary(DOSBase);
	}

	if (wbmsg)
	{
		Forbid();
		ReplyMsg(&wbmsg->sm_Message);
	}

	return (int32)result;
}


__attribute__((section(".text"))) UBYTE VString[] = "$VER: QoaPlay 0.3 (23.07.2025)\r\n";



void* operator new(uint32 size) throw()
{
	uint32 *m;

	size += 4;

	if (m = (uint32*)AllocPooled(TaskPool, size))
	{
		*m = size;
		return m + 1;
	}
	else return NULL;
}


void* operator new[](uint32 size)
{
	return operator new(size);
}


void operator delete(void* memory)
{
	uint32 *m = (uint32*)memory - 1;

	FreePooled(TaskPool, m, *m);
}


void operator delete[](void* memory)
{
	operator delete(memory);
}


#ifdef HAVE_GLOBAL_CONSTRUCTORS
void GlobalConstructors()
{
	for (long i = (long)__CTOR_LIST__[0]; i > 0; i--) __CTOR_LIST__[i](); 
}

void GlobalDestructors()
{
	void (**dtor)(void) = __DTOR_LIST__;

	while (*(++dtor)) (*dtor)();
}
#endif
