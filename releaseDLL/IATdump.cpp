#include "pch.h"
#include "framework.h"
#include "IATdump.h"

std::vector<std::string> testGetImports()
{
	std::vector<std::string> imports;
	LPVOID procBaseAddr = GetModuleHandle(NULL);
	IMAGE_DOS_HEADER dosHeader = *(IMAGE_DOS_HEADER*)procBaseAddr;
	IMAGE_NT_HEADERS ntHeader = *(IMAGE_NT_HEADERS*)((DWORD_PTR)procBaseAddr + dosHeader.e_lfanew);
	IMAGE_OPTIONAL_HEADER32 optHeader = ntHeader.OptionalHeader;
	IMAGE_DATA_DIRECTORY dataDir = optHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
	PIMAGE_IMPORT_DESCRIPTOR importDescriptor = (IMAGE_IMPORT_DESCRIPTOR*)((DWORD_PTR)procBaseAddr + dataDir.VirtualAddress);
	LPCSTR libName = NULL;
	HMODULE library = NULL;
	PIMAGE_IMPORT_BY_NAME functionName = NULL;
	while (importDescriptor->Name != NULL)
	{
		libName = (LPCSTR)importDescriptor->Name + (DWORD_PTR)procBaseAddr;
		library = LoadLibraryA(libName);
		if (library)
		{
			PIMAGE_THUNK_DATA originalFirstThunk = NULL;
			originalFirstThunk = (PIMAGE_THUNK_DATA)((DWORD_PTR)procBaseAddr + importDescriptor->OriginalFirstThunk);
			while (originalFirstThunk->u1.AddressOfData != NULL)
			{
				if (originalFirstThunk->u1.AddressOfData > 0x80000000) void(0);
				else
				{
					functionName = (PIMAGE_IMPORT_BY_NAME)((DWORD_PTR)procBaseAddr + originalFirstThunk->u1.AddressOfData);
					imports.push_back(std::string(functionName->Name));
				}
				originalFirstThunk++;
			}
		}
		importDescriptor++;

	}
	return imports;
}