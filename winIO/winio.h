#ifndef WINIO_H
#define WINIO_H

#include "winio_nt.h"


extern "C"
{
  bool _stdcall InitializeWinIo();
   void _stdcall ShutdownWinIo();
   PBYTE _stdcall MapPhysToLin(tagPhysStruct &PhysStruct);
   bool _stdcall UnmapPhysicalMemory(tagPhysStruct &PhysStruct);
   bool _stdcall GetPhysLong(PBYTE pbPhysAddr, PDWORD pdwPhysVal);
   bool _stdcall SetPhysLong(PBYTE pbPhysAddr, DWORD dwPhysVal);
   bool _stdcall GetPortVal(WORD wPortAddr, PDWORD pdwPortVal, BYTE bSize);
   bool _stdcall SetPortVal(WORD wPortAddr, DWORD dwPortVal, BYTE bSize);
   bool _stdcall InstallWinIoDriver(PWSTR pszWinIoDriverPath, bool IsDemandLoaded = false);
   bool _stdcall RemoveWinIoDriver();
}

extern HANDLE hDriver;
extern bool IsWinIoInitialized;
extern bool g_Is64BitOS;

bool _stdcall StartWinIoDriver();
bool _stdcall StopWinIoDriver();

#endif
