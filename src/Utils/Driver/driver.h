#pragma once
#include "../header.h"

namespace codes {
    constexpr ULONG init = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x696, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
    constexpr ULONG read = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x697, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
    constexpr ULONG write = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x698, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
};


extern HANDLE m_driver_handle; //handle to our driver

struct info_t {
	HANDLE target_pid = 0; //process id of process we want to read from / write to
	void* target_address = 0x0; //address in the target proces we want to read from / write to
	void* buffer_address = 0x0; //address in our usermode process to copy to (read mode) / read from (write mode)
	SIZE_T size = 0; //size of memory to copy between our usermode process and target process
	SIZE_T return_size = 0; //number of bytes successfully read / written
};


class driver_manager {
    
public:
	static bool find_driver(const char* driver_name);
	static void read_virtual_memory(PVOID address, PVOID buffer, DWORD size);
	static void write_virtual_memory(PVOID address, PVOID buffer, DWORD size);
	static void attach_to_process(DWORD process_id);
	static std::wstring get_base_module(DWORD process_id);
	static DWORD get_process_id(const wchar_t* process_name);
	//static uintptr_t get_base_address();
	static std::uintptr_t get_module_base_address(DWORD process_id, const wchar_t* module_name);
};

template<typename T>
T read_mem(const std::uintptr_t address) {
	T read_data;
	driver_manager::read_virtual_memory(reinterpret_cast<PVOID>(address), &read_data, sizeof(T));
	return read_data;
}
template<typename T>
void write_mem(const std::uintptr_t address, const T& buffer) {
	driver_manager::write_virtual_memory(reinterpret_cast<PVOID>(address), (PVOID)&buffer, sizeof(T));
}

//std::string read_mem_string(uintptr_t address, SIZE_T StringSize) {
//	const int BufferSize = 256;
//	char Buffer[BufferSize];
//	int BytesRead = 0;
//	while (BytesRead < BufferSize && BytesRead < StringSize) {
//		char Character;
//		driver_manager::read_virtual_memory(reinterpret_cast<PVOID>(address + BytesRead), &Character, sizeof(char));
//		Buffer[BytesRead] = Character;
//		if (Character == '\0')
//			break;
//		BytesRead++;
//	}
//	return std::string(Buffer);
//}