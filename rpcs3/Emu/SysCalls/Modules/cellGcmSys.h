#pragma once
#include "Emu/RSX/GCM.h"

enum
{
	CELL_GCM_ERROR_FAILURE           = 0x802100ff,
	CELL_GCM_ERROR_NO_IO_PAGE_TABLE  = 0x80210001,
	CELL_GCM_ERROR_INVALID_ENUM      = 0x80210002,
	CELL_GCM_ERROR_INVALID_VALUE     = 0x80210003,
	CELL_GCM_ERROR_INVALID_ALIGNMENT = 0x80210004,
	CELL_GCM_ERROR_ADDRESS_OVERWRAP  = 0x80210005,
};

struct CellGcmOffsetTable
{
	vm::bptr<u16> ioAddress;
	vm::bptr<u16> eaAddress;
};

// Auxiliary functions
void InitOffsetTable();
u32 gcmGetLocalMemorySize();

// libgcm functions
s32 cellGcmSetPrepareFlip(vm::ptr<CellGcmContextData> ctxt, u32 id);

s32 cellGcmAddressToOffset(u64 address, vm::ptr<be_t<u32>> offset);
u32 cellGcmGetMaxIoMapSize();
void cellGcmGetOffsetTable(vm::ptr<CellGcmOffsetTable> table);
s32 cellGcmIoOffsetToAddress(u32 ioOffset, u64 address);
s32 cellGcmMapEaIoAddress(u32 ea, u32 io, u32 size);
s32 cellGcmMapEaIoAddressWithFlags(u32 ea, u32 io, u32 size, u32 flags);
s32 cellGcmMapMainMemory(u32 ea, u32 size, vm::ptr<u32> offset);
s32 cellGcmReserveIoMapSize(u32 size);
s32 cellGcmUnmapEaIoAddress(u64 ea);
s32 cellGcmUnmapIoAddress(u64 io);
s32 cellGcmUnreserveIoMapSize(u32 size);

// Syscall
s32 cellGcmCallback(vm::ptr<CellGcmContextData> context, u32 count);
