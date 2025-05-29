#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>
#include <vector>
#pragma comment(lib, "ntdll.lib")

uintptr_t virtualaddy;
uintptr_t proccessid;
uintptr_t cr3;

#define CODE_RW CTL_CODE(FILE_DEVICE_UNKNOWN, 0x4592, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define CODE_BA CTL_CODE(FILE_DEVICE_UNKNOWN, 0x5317, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define CODE_C3 CTL_CODE(FILE_DEVICE_UNKNOWN, 0x6721, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define CODE_MOUSE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x73942, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define CODE_PROT CTL_CODE(FILE_DEVICE_UNKNOWN, 0x482, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define CODE_SECURITY 0x9bcf812

typedef struct _rw {
    INT32 security;
    INT32 process_id;
    ULONGLONG address;
    ULONGLONG buffer;
    ULONGLONG size;
    BOOLEAN write;
} rw, * prw;

typedef struct sBaseAddress {
    INT32 security;
    INT32 process_id;
    ULONGLONG* address;
} ba, * pba;

typedef struct _movemouse {
    long x;
    long y;
    unsigned short button_flags;
} movemouse, * MouseMovementStruct;

typedef struct _MEMORY_OPERATION_DATA {
    uint32_t pid;
    ULONGLONG cr3; 
} MEMORY_OPERATION_DATA, * PMEMORY_OPERATION_DATA;

#include <winternl.h>

extern "C" NTSTATUS NTAPI NtCreateFile(
    PHANDLE FileHandle,
    ACCESS_MASK DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes,
    PIO_STATUS_BLOCK IoStatusBlock,
    PLARGE_INTEGER AllocationSize,
    ULONG FileAttributes,
    ULONG ShareAccess,
    ULONG CreateDisposition,
    ULONG CreateOptions,
    PVOID EaBuffer,
    ULONG EaLength
);

typedef NTSTATUS(NTAPI* NtCreateFile_t)(
    PHANDLE FileHandle,
    ACCESS_MASK DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes,
    PIO_STATUS_BLOCK IoStatusBlock,
    PLARGE_INTEGER AllocationSize,
    ULONG FileAttributes,
    ULONG ShareAccess,
    ULONG CreateDisposition,
    ULONG CreateOptions,
    PVOID EaBuffer,
    ULONG EaLength
    );

typedef struct _TARGET_PROCESS {
    int ProcessId;
} TARGET_PROCESS, * PTARGET_PROCESS;

namespace huskyud {
    HANDLE driver_handle;
    INT32 process_id;

    bool find_driver() {
        UNICODE_STRING DeviceName;
        OBJECT_ATTRIBUTES ObjAttr;
        IO_STATUS_BLOCK IoStatusBlock;
        NTSTATUS status;

        RtlInitUnicodeString(&DeviceName, L"\\Device\\\{HarddiskVolume0}");

        InitializeObjectAttributes(&ObjAttr, &DeviceName, OBJ_CASE_INSENSITIVE, NULL, NULL);

        NtCreateFile_t NtCreateFile = (NtCreateFile_t)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "NtCreateFile");

        if (NtCreateFile == NULL) {
            return false;
        }

        status = NtCreateFile(&driver_handle, GENERIC_READ | GENERIC_WRITE, &ObjAttr, &IoStatusBlock, NULL,
            FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ | FILE_SHARE_WRITE, FILE_OPEN, 0, NULL, 0);

        if (!NT_SUCCESS(status) || (driver_handle == INVALID_HANDLE_VALUE)) {
            return false;
        }

        return true;
    }

    bool load() {
        if (!find_driver()) {
            return false;
        }

        TARGET_PROCESS targetProcess;
        targetProcess.ProcessId = GetCurrentProcessId();

        DWORD bytesReturned;
        BOOL result = DeviceIoControl(
            driver_handle,
            CODE_PROT,
            &targetProcess,
            sizeof(TARGET_PROCESS),
            NULL,
            0,
            &bytesReturned,
            NULL
        );

        return result != FALSE;
    }

    void read_physical(uintptr_t address, PVOID buffer, DWORD size) {
        _rw arguments = { 0 };

        arguments.security = CODE_SECURITY;
        arguments.address = (ULONGLONG)address;
        arguments.buffer = (ULONGLONG)buffer;
        arguments.size = size;
        arguments.process_id = process_id;
        arguments.write = FALSE;

        DeviceIoControl(driver_handle, CODE_RW, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
    }

    void write_physical(uintptr_t address, PVOID buffer, DWORD size) {
        _rw arguments = { 0 };

        arguments.security = CODE_SECURITY;
        arguments.address = (ULONGLONG)address;
        arguments.buffer = (ULONGLONG)buffer;
        arguments.size = size;
        arguments.process_id = process_id;
        arguments.write = TRUE;

        DeviceIoControl(driver_handle, CODE_RW, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
    }

    uintptr_t base_ADRESS() {
        uintptr_t image_address = 0;
        sBaseAddress arguments = { 0 };

        arguments.security = CODE_SECURITY;
        arguments.process_id = process_id;
        arguments.address = (ULONGLONG*)&image_address;

        DeviceIoControl(driver_handle, CODE_BA, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);

        return image_address;
    }

    uintptr_t CR3() {
        MEMORY_OPERATION_DATA arguments = { 0 };

        arguments.pid = process_id;
        DWORD bytesReturned;
        DeviceIoControl(driver_handle, CODE_C3, &arguments, sizeof(arguments), &arguments, sizeof(arguments), &bytesReturned, NULL);

        return arguments.cr3;
    }

    INT32 find_process(LPCTSTR process_name) {
        PROCESSENTRY32 pt;
        HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        pt.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(hsnap, &pt)) {
            do {
                if (!lstrcmpi(pt.szExeFile, process_name)) {
                    CloseHandle(hsnap);
                    huskyud::process_id = pt.th32ProcessID;
                    return pt.th32ProcessID;
                }
            } while (Process32Next(hsnap, &pt));
        }

        CloseHandle(hsnap);
        return 0; 
    }
}


template <typename T>
T read(uint64_t address) {
    T buffer{ };
    huskyud::read_physical(address, &buffer, sizeof(T));
    return buffer;
}


template <typename T>
T write(uint64_t address, T buffer) {

    huskyud::write_physical((uintptr_t)address, &buffer, sizeof(T));
    return buffer;
}

bool is_valid(const uint64_t adress)
{
    if (adress <= 0x400000 || adress == 0xCCCCCCCCCCCCCCCC || reinterpret_cast<void*>(adress) == nullptr || adress >
        0x7FFFFFFFFFFFFFFF) {
        return false;
    }
    return true;
}


