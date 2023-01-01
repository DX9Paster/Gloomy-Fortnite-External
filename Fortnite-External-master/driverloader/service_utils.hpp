#pragma once
#include <Windows.h>
#include <string>
#include "singed.hpp"

#define CHECK_HANDLE(x) (x && x != INVALID_HANDLE_VALUE)
#define MIN_ADDRESS ((ULONG_PTR)0x8000000000000000)

#define CHECK_SC_MANAGER_HANDLE(x, ret_type)												\
if (!CHECK_HANDLE(x))																		\
{																							\
	printf(XorStr("[!] Failed to obtain service handle \n").c_str());										\
	return ret_type;																		\
}																							\

namespace service_utils
{
	SC_HANDLE open_sc_manager();

	SC_HANDLE create_service(const std::string_view driver_path);
	bool delete_service(SC_HANDLE service_handle, bool close_on_fail = true, bool close_on_success = true);

	bool start_service(SC_HANDLE service_handle);
	bool stop_service(SC_HANDLE service_handle);
}