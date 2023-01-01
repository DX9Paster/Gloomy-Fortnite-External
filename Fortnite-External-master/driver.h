/*
	*Gloomy.cc
 	*https://github.com/Chase1803
	
	Copyright (c) 2022 Chase1803
	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the "Software"), to deal in the Software without
	restriction, including without limitation the rights to use,
	copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following
	conditions:
	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.
 */

#define WIN32_LEAN_AND_MEAN  
#pragma once

#include <windows.h>
#include "tlhelp32.h"
#include "string.hpp"

struct READWRITE
{
	DWORD Pid;
	ULONG64 Address;
	DWORD Size;
	ULONG64 Buffer;
};

typedef struct _MEMORY_BASIC_INFORMATIONA
{
	ULONG64  BaseAddress;
	ULONG64  AllocationBase;
	ULONG  AllocationProtect;
	SIZE_T RegionSize;
	ULONG  State;
	ULONG  Protect;
	ULONG  Type;

} MEMORY_BASIC_INFORMATIONA, *PMEMORY_BASIC_INFORMATIONA;

typedef struct _MyMEMORY_BASIC_INFORMATION {
	ULONG64 BaseAddress;
	ULONG64 AllocationBase;
	ULONG64 AllocationProtect;
	ULONG64 RegionSize;
	ULONG64 State;
	ULONG64 Protect;
	ULONG64 Type;
} MyMEMORY_BASIC_INFORMATION, *PMyMEMORY_BASIC_INFORMATION;

typedef struct _MMEMORY_BASIC_INFORMATION {
	ULONG64 BaseAddress;
	ULONG64 AllocationBase;
	ULONG64 AllocationProtect;
	ULONG64 RegionSize;
	ULONG64 State;
	ULONG64 Protect;
	ULONG64 Type;
} MMEMORY_BASIC_INFORMATION, *PMMEMORY_BASIC_INFORMATION;

class Sandy64
{
public:
	BOOL Init();
	template<typename DataType> DataType Read(ULONG64 Address)
	{
		DataType Buffer;
		ReadPtr(Address, &Buffer, sizeof(DataType));
		return Buffer;
	}

	ULONG64 GetModuleBase(ULONG ProcessPid, LPCSTR ModuleName);
	BOOL ReadPtr(ULONG ProcessPid, ULONG64 Address, PVOID pBuffer, DWORD Size);
	BOOL WritePtr(ULONG ProcessPid, ULONG64 Address, PVOID pBuffer, DWORD Size);
};

HANDLE hDrive;

BOOL Sandy64::Init()
{
	hDrive = ::CreateFileA(XorStr("\\\\.\\Sandy64").c_str(), GENERIC_ALL, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_HIDDEN, NULL);
	return hDrive != 0;
}

ULONG64 Sandy64::GetModuleBase(ULONG ProcessPid, LPCSTR ModuleName)
{
	READWRITE ReadWrite = { ProcessPid,0,0,(ULONG64)ModuleName };
	BYTE* Temp = new BYTE[8];
	::memset(Temp, 0, 8);
	BOOL bRet = ::DeviceIoControl(hDrive, 0x22200C, &ReadWrite, sizeof(READWRITE), Temp, 8, NULL, NULL);
	ULONG64 temp = 0;
	if (bRet == TRUE)
	{
		memcpy(&temp, Temp, 8);
	}
	delete[] Temp;
	return temp;
}

BOOL Sandy64::ReadPtr(ULONG ProcessPid,ULONG64 Address, PVOID pBuffer, DWORD Size)
{
	READWRITE ReadWrite = { ProcessPid,Address,Size,0 };
	BYTE* Temp = new BYTE[Size];
	memset(Temp, 0, Size);
	BOOL bRet = ::DeviceIoControl(hDrive, 0x222000, &ReadWrite, sizeof(READWRITE), Temp, Size, NULL, NULL);
	if (bRet)
	{
		memcpy(pBuffer, Temp, Size);
	}
	delete[] Temp;
	return bRet;
}

BOOL Sandy64::WritePtr(ULONG ProcessPid,ULONG64 Address, PVOID pBuffer, DWORD Size)
{
	READWRITE ReadWrite = { ProcessPid,Address,Size,(ULONG64)pBuffer };
	BOOL bRet = ::DeviceIoControl(hDrive, 0x222004, &ReadWrite, sizeof(READWRITE), NULL, NULL, NULL, NULL);
	return bRet;
}
