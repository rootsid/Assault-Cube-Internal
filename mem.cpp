#include <vector>
#include "mem.h"

void mem::PatchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess)
{
	DWORD oldprotect;
	VirtualProtectEx(hProcess, dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	WriteProcessMemory(hProcess, dst, src, size, nullptr);
	VirtualProtectEx(hProcess, dst, size, oldprotect, &oldprotect);
}
void mem::NopEx(BYTE* dst, unsigned int size, HANDLE hProcess)
{
	BYTE* nopArray = new BYTE[size];
	memset(nopArray, 0x90, size);

	PatchEx(dst, nopArray, size, hProcess);
	delete[] nopArray;
}

uintptr_t mem::FindDMAAddy(HANDLE hproc, uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); i++)
	{
		// https://docs.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-readprocessmemory
		ReadProcessMemory(hproc, (BYTE*)addr, &addr, sizeof(addr), 0);
		// process, baseAdd, Buffer, Size, NoOfBytesRead
		addr += offsets[i];
	}
	return addr;
}

void mem::Patch(BYTE* dst, BYTE* src, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	memcpy( dst, src, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}
void mem::Nop(BYTE* dst, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	memset(dst, 0x90, size); //0x90 NOP
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}

uintptr_t mem::FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); i++)
	{
		addr = *(uintptr_t*)addr;
		addr += offsets[i];
	}
	return addr;
}