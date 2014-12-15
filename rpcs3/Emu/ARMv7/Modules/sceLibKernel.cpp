#include "stdafx.h"
#include "Emu/System.h"
#include "Emu/Memory/Memory.h"
#include "Emu/CPU/CPUThreadManager.h"
#include "Emu/ARMv7/ARMv7Thread.h"
#include "Emu/ARMv7/PSVFuncList.h"

extern psv_log_base sceLibKernel;

#define RETURN_ERROR(code) { Emu.Pause(); sceLibKernel.Error("%s() failed: %s", __FUNCTION__, #code); return code; }

#pragma pack(push, 4)

typedef s32(*SceKernelThreadEntry)(u32 argSize, vm::psv::ptr<void> pArgBlock);

union SceKernelSysClock
{
	struct
	{
		u32 low;
		u32 hi;
	};
	u64 quad;
};

struct SceKernelThreadOptParam
{
	u32 size;
	u32 attr;
};

struct SceKernelThreadInfo
{
	u32 size;
	s32 processId;
	char name[32];
	u32 attr;
	u32 status;
	SceKernelThreadEntry entry;
	vm::psv::ptr<void> pStack;
	u32 stackSize;
	s32 initPriority;
	s32 currentPriority;
	s32 initCpuAffinityMask;
	s32 currentCpuAffinityMask;
	s32 currentCpuId;
	s32 lastExecutedCpuId;
	u32 waitType;
	s32 waitId;
	s32 exitStatus;
	SceKernelSysClock runClocks;
	u32 intrPreemptCount;
	u32 threadPreemptCount;
	u32 threadReleaseCount;
	s32 changeCpuCount;
	s32 fNotifyCallback;
	s32 reserved;
};

struct SceKernelThreadRunStatus
{
	u32 size;

	struct
	{
		s32 processId;
		s32 threadId;
		s32 priority;

	} cpuInfo[4];
};

struct SceKernelSystemInfo
{
	u32 size;
	u32 activeCpuMask;

	struct
	{
		SceKernelSysClock idleClock;
		u32 comesOutOfIdleCount;
		u32 threadSwitchCount;

	} cpuInfo[4];
};

#pragma pack(pop)

s32 sceKernelCreateThread(
	vm::psv::ptr<const char> pName,
	vm::psv::ptr<SceKernelThreadEntry> entry,
	s32 initPriority,
	u32 stackSize,
	u32 attr,
	s32 cpuAffinityMask,
	vm::psv::ptr<const SceKernelThreadOptParam> pOptParam)
{
	sceLibKernel.Error("sceKernelCreateThread(pName_addr=0x%x ('%s'), entry_addr=0x%x, initPriority=%d, stackSize=0x%x, attr=0x%x, cpuAffinityMask=0x%x, pOptParam_addr=0x%x)",
		pName.addr(), pName.get_ptr(), entry.addr(), initPriority, stackSize, attr, cpuAffinityMask, pOptParam.addr());

	std::string name = pName.get_ptr();

	ARMv7Thread& new_thread = *(ARMv7Thread*)&Emu.GetCPU().AddThread(CPU_THREAD_ARMv7);

	u32 id = new_thread.GetId();
	new_thread.SetEntry(entry.addr() ^ 1);
	new_thread.SetPrio(initPriority);
	new_thread.SetStackSize(stackSize);
	new_thread.SetName(name);

	sceLibKernel.Error("*** New ARMv7 Thread [%s] (entry=0x%x): id = %d", name.c_str(), entry, id);

	new_thread.Run();

	Emu.Pause();

	return id;
}

s32 sceKernelStartThread(s32 threadId, u32 argSize, vm::psv::ptr<const void> pArgBlock)
{
	sceLibKernel.Error("sceKernelStartThread(threadId=%d, argSize=%d, pArgBlock_addr=0x%x)", threadId, argSize, pArgBlock.addr());

	CPUThread* t = Emu.GetCPU().GetThread(threadId);

	if (!t || t->GetType() != CPU_THREAD_ARMv7)
	{
		RETURN_ERROR(SCE_KERNEL_ERROR_INVALID_UID);
	}

	// push arg block onto the stack
	u32 pos = (static_cast<ARMv7Thread*>(t)->SP -= argSize);
	memcpy(vm::get_ptr<void>(pos), pArgBlock.get_ptr(), argSize);

	// set SceKernelThreadEntry function arguments
	static_cast<ARMv7Thread*>(t)->write_gpr(0, argSize);
	static_cast<ARMv7Thread*>(t)->write_gpr(1, pos);

	t->Exec();
	return SCE_OK;
}

s32 sceKernelExitThread(ARMv7Thread& CPU, s32 exitStatus)
{
	sceLibKernel.Error("sceKernelExitThread(exitStatus=0x%x)", exitStatus);

	// exit status is stored in r0
	CPU.Stop();

	return SCE_OK;
}

s32 sceKernelDeleteThread(s32 threadId)
{
	sceLibKernel.Todo("sceKernelDeleteThread(threadId=%d)", threadId);

	return SCE_OK;
}

s32 sceKernelExitDeleteThread(s32 exitStatus)
{
	sceLibKernel.Todo("sceKernelExitDeleteThread(exitStatus=0x%x)", exitStatus);

	return SCE_OK;
}

s32 sceKernelChangeThreadCpuAffinityMask(s32 threadId, s32 cpuAffinityMask)
{
	sceLibKernel.Todo("sceKernelChangeThreadCpuAffinityMask(threadId=%d, cpuAffinityMask=0x%x)", threadId, cpuAffinityMask);

	return SCE_OK;
}

s32 sceKernelGetThreadCpuAffinityMask(s32 threadId)
{
	sceLibKernel.Todo("sceKernelGetThreadCpuAffinityMask(threadId=0x%x)", threadId);

	return SCE_OK;
}

s32 sceKernelChangeThreadPriority(s32 threadId, s32 priority)
{
	sceLibKernel.Todo("sceKernelChangeThreadPriority(threadId=0x%x, priority=%d)", threadId, priority);

	return SCE_OK;
}

s32 sceKernelGetThreadCurrentPriority()
{
	sceLibKernel.Todo("sceKernelGetThreadCurrentPriority()");

	return SCE_OK;
}

s32 sceKernelGetThreadId()
{
	sceLibKernel.Todo("sceKernelGetThreadId()");

	return SCE_OK;
}

s32 sceKernelChangeCurrentThreadAttr(u32 clearAttr, u32 setAttr)
{
	sceLibKernel.Todo("sceKernelChangeCurrentThreadAttr()");

	return SCE_OK;
}

s32 sceKernelGetThreadExitStatus(s32 threadId, vm::psv::ptr<s32> pExitStatus)
{
	sceLibKernel.Todo("sceKernelGetThreadExitStatus(threadId=%d, pExitStatus_addr=0x%x)", threadId, pExitStatus.addr());

	return SCE_OK;
}

s32 sceKernelGetProcessId()
{
	sceLibKernel.Todo("sceKernelGetProcessId()");

	return SCE_OK;
}

s32 sceKernelCheckWaitableStatus()
{
	sceLibKernel.Todo("sceKernelCheckWaitableStatus()");

	return SCE_OK;
}

s32 sceKernelGetThreadInfo(s32 threadId, vm::psv::ptr<SceKernelThreadInfo> pInfo)
{
	sceLibKernel.Todo("sceKernelGetThreadInfo(threadId=%d, pInfo_addr=0x%x)", threadId, pInfo.addr());

	return SCE_OK;
}

s32 sceKernelGetThreadRunStatus(vm::psv::ptr<SceKernelThreadRunStatus> pStatus)
{
	sceLibKernel.Todo("sceKernelGetThreadRunStatus(pStatus_addr=0x%x)", pStatus.addr());

	return SCE_OK;
}

s32 sceKernelGetSystemInfo(vm::psv::ptr<SceKernelSystemInfo> pInfo)
{
	sceLibKernel.Todo("sceKernelGetSystemInfo(pInfo_addr=0x%x)", pInfo.addr());

	return SCE_OK;
}

s32 sceKernelGetThreadmgrUIDClass(s32 uid)
{
	sceLibKernel.Todo("sceKernelGetThreadmgrUIDClass(uid=%d)", uid);

	return SCE_OK;
}

s32 sceKernelChangeThreadVfpException(s32 clearMask, s32 setMask)
{
	sceLibKernel.Todo("sceKernelChangeThreadVfpException(clearMask=0x%x, setMask=0x%x)", clearMask, setMask);

	return SCE_OK;
}

s32 sceKernelGetCurrentThreadVfpException()
{
	sceLibKernel.Todo("sceKernelGetCurrentThreadVfpException()");

	return SCE_OK;
}

s32 sceKernelDelayThread(u32 usec)
{
	sceLibKernel.Todo("sceKernelDelayThread()");

	return SCE_OK;
}

s32 sceKernelDelayThreadCB(u32 usec)
{
	sceLibKernel.Todo("sceKernelDelayThreadCB()");

	return SCE_OK;
}

s32 sceKernelWaitThreadEnd(s32 threadId, vm::psv::ptr<s32> pExitStatus, vm::psv::ptr<u32> pTimeout)
{
	sceLibKernel.Todo("sceKernelWaitThreadEnd(threadId=%d, pExitStatus_addr=0x%x, pTimeout_addr=0x%x)", threadId, pExitStatus.addr(), pTimeout.addr());

	return SCE_OK;
}

s32 sceKernelWaitThreadEndCB(s32 threadId, vm::psv::ptr<s32> pExitStatus, vm::psv::ptr<u32> pTimeout)
{
	sceLibKernel.Todo("sceKernelWaitThreadEndCB(threadId=%d, pExitStatus_addr=0x%x, pTimeout_addr=0x%x)", threadId, pExitStatus.addr(), pTimeout.addr());

	return SCE_OK;
}

#define REG_FUNC(nid, name) reg_psv_func(nid, &sceLibKernel, #name, &name)

psv_log_base sceLibKernel = []() -> psv_log_base
{
	//REG_FUNC(0x23EAA62, sceKernelPuts);
	//REG_FUNC(0xB0335388, sceClibToupper);
	//REG_FUNC(0x4C5471BC, sceClibTolower);
	//REG_FUNC(0xD8EBBB7E, sceClibLookCtypeTable);
	//REG_FUNC(0x20EC3210, sceClibGetCtypeTable);
	//REG_FUNC(0x407D6153, sceClibMemchr);
	//REG_FUNC(0x9CC2BFDF, sceClibMemcmp);
	//REG_FUNC(0x14E9DBD7, sceClibMemcpy);
	//REG_FUNC(0x736753C8, sceClibMemmove);
	//REG_FUNC(0x632980D7, sceClibMemset);
	//REG_FUNC(0xFA26BC62, sceClibPrintf);
	//REG_FUNC(0x5EA3B6CE, sceClibVprintf);
	//REG_FUNC(0x8CBA03D5, sceClibSnprintf);
	//REG_FUNC(0xFA6BE467, sceClibVsnprintf);
	//REG_FUNC(0xA2FB4D9D, sceClibStrcmp);
	//REG_FUNC(0x70CBC2D5, sceClibStrlcat);
	//REG_FUNC(0x2CDFCD1C, sceClibStrlcpy);
	//REG_FUNC(0xA37E6383, sceClibStrncat);
	//REG_FUNC(0x660D1F6D, sceClibStrncmp);
	//REG_FUNC(0xC458D60A, sceClibStrncpy);
	//REG_FUNC(0xAC595E68, sceClibStrnlen);
	//REG_FUNC(0x614076B7, sceClibStrchr);
	//REG_FUNC(0x6E728AAE, sceClibStrrchr);
	//REG_FUNC(0xE265498B, sceClibStrstr);
	//REG_FUNC(0xB54C0BE4, sceClibStrncasecmp);
	//REG_FUNC(0x2F2C6046, sceClibAbort);
	//REG_FUNC(0x2E581B88, sceClibStrtoll);
	//REG_FUNC(0x3B9E301A, sceClibMspaceCreate);
	//REG_FUNC(0xAE1A21EC, sceClibMspaceDestroy);
	//REG_FUNC(0x86EF7680, sceClibMspaceMalloc);
	//REG_FUNC(0x9C56B4D1, sceClibMspaceFree);
	//REG_FUNC(0x678374AD, sceClibMspaceCalloc);
	//REG_FUNC(0x3C847D57, sceClibMspaceMemalign);
	//REG_FUNC(0x774891D6, sceClibMspaceRealloc);
	//REG_FUNC(0x586AC68D, sceClibMspaceReallocalign);
	//REG_FUNC(0x46A02279, sceClibMspaceMallocUsableSize);
	//REG_FUNC(0x8CC1D38E, sceClibMspaceMallocStats);
	//REG_FUNC(0x738E0322, sceClibMspaceMallocStatsFast);
	//REG_FUNC(0xD1D59701, sceClibMspaceIsHeapEmpty);
	//REG_FUNC(0xE960FDA2, sceKernelAtomicSet8);
	//REG_FUNC(0x450BFECF, sceKernelAtomicSet16);
	//REG_FUNC(0xB69DA09B, sceKernelAtomicSet32);
	//REG_FUNC(0xC8A4339C, sceKernelAtomicSet64);
	//REG_FUNC(0x27A2AAFA, sceKernelAtomicGetAndAdd8);
	//REG_FUNC(0x5674DB0C, sceKernelAtomicGetAndAdd16);
	//REG_FUNC(0x2611CB0B, sceKernelAtomicGetAndAdd32);
	//REG_FUNC(0x63DAF37D, sceKernelAtomicGetAndAdd64);
	//REG_FUNC(0x8F7BD940, sceKernelAtomicAddAndGet8);
	//REG_FUNC(0x495C52EC, sceKernelAtomicAddAndGet16);
	//REG_FUNC(0x2E84A93B, sceKernelAtomicAddAndGet32);
	//REG_FUNC(0xB6CE9B9A, sceKernelAtomicAddAndGet64);
	//REG_FUNC(0xCDF5DF67, sceKernelAtomicGetAndSub8);
	//REG_FUNC(0xAC51979C, sceKernelAtomicGetAndSub16);
	//REG_FUNC(0x115C516F, sceKernelAtomicGetAndSub32);
	//REG_FUNC(0x4AE9C8E6, sceKernelAtomicGetAndSub64);
	//REG_FUNC(0x99E1796E, sceKernelAtomicSubAndGet8);
	//REG_FUNC(0xC26BBBB1, sceKernelAtomicSubAndGet16);
	//REG_FUNC(0x1C9CD92, sceKernelAtomicSubAndGet32);
	//REG_FUNC(0x9BB4A94B, sceKernelAtomicSubAndGet64);
	//REG_FUNC(0x53DCA02B, sceKernelAtomicGetAndAnd8);
	//REG_FUNC(0x7A0CB056, sceKernelAtomicGetAndAnd16);
	//REG_FUNC(0x8266595, sceKernelAtomicGetAndAnd32);
	//REG_FUNC(0x4828BC43, sceKernelAtomicGetAndAnd64);
	//REG_FUNC(0x86B9170F, sceKernelAtomicAndAndGet8);
	//REG_FUNC(0xF9890F7E, sceKernelAtomicAndAndGet16);
	//REG_FUNC(0x6709D30C, sceKernelAtomicAndAndGet32);
	//REG_FUNC(0xAED2B370, sceKernelAtomicAndAndGet64);
	//REG_FUNC(0x107A68DF, sceKernelAtomicGetAndOr8);
	//REG_FUNC(0x31E49E73, sceKernelAtomicGetAndOr16);
	//REG_FUNC(0x984AD276, sceKernelAtomicGetAndOr32);
	//REG_FUNC(0xC39186CD, sceKernelAtomicGetAndOr64);
	//REG_FUNC(0x51693931, sceKernelAtomicOrAndGet8);
	//REG_FUNC(0x8E248EBD, sceKernelAtomicOrAndGet16);
	//REG_FUNC(0xC3B2F7F8, sceKernelAtomicOrAndGet32);
	//REG_FUNC(0x809BBC7D, sceKernelAtomicOrAndGet64);
	//REG_FUNC(0x7350B2DF, sceKernelAtomicGetAndXor8);
	//REG_FUNC(0x6E2D0B9E, sceKernelAtomicGetAndXor16);
	//REG_FUNC(0x38739E2F, sceKernelAtomicGetAndXor32);
	//REG_FUNC(0x6A19BBE9, sceKernelAtomicGetAndXor64);
	//REG_FUNC(0x634AF062, sceKernelAtomicXorAndGet8);
	//REG_FUNC(0x6F524195, sceKernelAtomicXorAndGet16);
	//REG_FUNC(0x46940704, sceKernelAtomicXorAndGet32);
	//REG_FUNC(0xDDC6866E, sceKernelAtomicXorAndGet64);
	//REG_FUNC(0x327DB4C0, sceKernelAtomicCompareAndSet8);
	//REG_FUNC(0xE8C01236, sceKernelAtomicCompareAndSet16);
	//REG_FUNC(0x1124A1D4, sceKernelAtomicCompareAndSet32);
	//REG_FUNC(0x1EBDFCCD, sceKernelAtomicCompareAndSet64);
	//REG_FUNC(0xD7D49E36, sceKernelAtomicClearMask8);
	//REG_FUNC(0x5FE7DFF8, sceKernelAtomicClearMask16);
	//REG_FUNC(0xE3DF0CB3, sceKernelAtomicClearMask32);
	//REG_FUNC(0x953D118A, sceKernelAtomicClearMask64);
	//REG_FUNC(0x7FD94393, sceKernelAtomicAddUnless8);
	//REG_FUNC(0x1CF4AA4B, sceKernelAtomicAddUnless16);
	//REG_FUNC(0x4B33FD3C, sceKernelAtomicAddUnless32);
	//REG_FUNC(0xFFCE7438, sceKernelAtomicAddUnless64);
	//REG_FUNC(0x9DABE6C3, sceKernelAtomicDecIfPositive8);
	//REG_FUNC(0x323718FB, sceKernelAtomicDecIfPositive16);
	//REG_FUNC(0xCA3294F1, sceKernelAtomicDecIfPositive32);
	//REG_FUNC(0x8BE2A007, sceKernelAtomicDecIfPositive64);
	//REG_FUNC(0xBBE82155, sceKernelLoadModule);
	//REG_FUNC(0x2DCC4AFA, sceKernelLoadStartModule);
	//REG_FUNC(0x702425D5, sceKernelStartModule);
	//REG_FUNC(0x3B2CBA09, sceKernelStopModule);
	//REG_FUNC(0x1987920E, sceKernelUnloadModule);
	//REG_FUNC(0x2415F8A4, sceKernelStopUnloadModule);
	//REG_FUNC(0x15E2A45D, sceKernelCallModuleExit);
	//REG_FUNC(0xD11A5103, sceKernelGetModuleInfoByAddr);
	//REG_FUNC(0x4F2D8B15, sceKernelOpenModule);
	//REG_FUNC(0x657FA50E, sceKernelCloseModule);
	//REG_FUNC(0x7595D9AA, sceKernelExitProcess);
	//REG_FUNC(0x4C7AD128, sceKernelPowerLock);
	//REG_FUNC(0xAF8E9C11, sceKernelPowerUnlock);
	//REG_FUNC(0xB295EB61, sceKernelGetTLSAddr);
	REG_FUNC(0xFB972F9, sceKernelGetThreadId);
	REG_FUNC(0xA37A6057, sceKernelGetCurrentThreadVfpException);
	//REG_FUNC(0xCA71EA2, sceKernelSendMsgPipe);
	//REG_FUNC(0xA5CA74AC, sceKernelSendMsgPipeCB);
	//REG_FUNC(0xDFC670E0, sceKernelTrySendMsgPipe);
	//REG_FUNC(0x4E81DD5C, sceKernelReceiveMsgPipe);
	//REG_FUNC(0x33AF829B, sceKernelReceiveMsgPipeCB);
	//REG_FUNC(0x5615B006, sceKernelTryReceiveMsgPipe);
	//REG_FUNC(0xA7819967, sceKernelLockLwMutex);
	//REG_FUNC(0x6F9C4CC1, sceKernelLockLwMutexCB);
	//REG_FUNC(0x9EF798C1, sceKernelTryLockLwMutex);
	//REG_FUNC(0x499EA781, sceKernelUnlockLwMutex);
	//REG_FUNC(0xF7D8F1FC, sceKernelGetLwMutexInfo);
	REG_FUNC(0xDDB395A9, sceKernelWaitThreadEnd);
	REG_FUNC(0xC54941ED, sceKernelWaitThreadEndCB);
	REG_FUNC(0xD5DC26C4, sceKernelGetThreadExitStatus);
	//REG_FUNC(0x4373B548, __sce_aeabi_idiv0);
	//REG_FUNC(0xFB235848, __sce_aeabi_ldiv0);
	REG_FUNC(0xF08DE149, sceKernelStartThread);
	REG_FUNC(0x58DDAC4F, sceKernelDeleteThread);
	REG_FUNC(0x5150577B, sceKernelChangeThreadCpuAffinityMask);
	REG_FUNC(0x8C57AC2A, sceKernelGetThreadCpuAffinityMask);
	REG_FUNC(0xDF7E6EDA, sceKernelChangeThreadPriority);
	//REG_FUNC(0xBCB63B66, sceKernelGetThreadStackFreeSize);
	REG_FUNC(0x8D9C5461, sceKernelGetThreadInfo);
	REG_FUNC(0xD6B01013, sceKernelGetThreadRunStatus);
	REG_FUNC(0xE0241FAA, sceKernelGetSystemInfo);
	REG_FUNC(0xF994FE65, sceKernelGetThreadmgrUIDClass);
	//REG_FUNC(0xB4DE10C7, sceKernelGetActiveCpuMask);
	REG_FUNC(0x2C1321A3, sceKernelChangeThreadVfpException);
	//REG_FUNC(0x3849359A, sceKernelCreateCallback);
	//REG_FUNC(0x88DD1BC8, sceKernelGetCallbackInfo);
	//REG_FUNC(0x464559D3, sceKernelDeleteCallback);
	//REG_FUNC(0xBD9C8F2B, sceKernelNotifyCallback);
	//REG_FUNC(0x3137A687, sceKernelCancelCallback);
	//REG_FUNC(0x76A2EF81, sceKernelGetCallbackCount);
	//REG_FUNC(0xD4F75281, sceKernelRegisterCallbackToEvent);
	//REG_FUNC(0x8D3940DF, sceKernelUnregisterCallbackFromEvent);
	//REG_FUNC(0x2BD1E682, sceKernelUnregisterCallbackFromEventAll);
	//REG_FUNC(0x120F03AF, sceKernelWaitEvent);
	//REG_FUNC(0xA0490795, sceKernelWaitEventCB);
	//REG_FUNC(0x241F3634, sceKernelPollEvent);
	//REG_FUNC(0x603AB770, sceKernelCancelEvent);
	//REG_FUNC(0x10586418, sceKernelWaitMultipleEvents);
	//REG_FUNC(0x4263DBC9, sceKernelWaitMultipleEventsCB);
	//REG_FUNC(0x8516D040, sceKernelCreateEventFlag);
	//REG_FUNC(0x11FE9B8B, sceKernelDeleteEventFlag);
	//REG_FUNC(0xE04EC73A, sceKernelOpenEventFlag);
	//REG_FUNC(0x9C0B8285, sceKernelCloseEventFlag);
	//REG_FUNC(0x83C0E2AF, sceKernelWaitEventFlag);
	//REG_FUNC(0xE737B1DF, sceKernelWaitEventFlagCB);
	//REG_FUNC(0x1FBB0FE1, sceKernelPollEventFlag);
	//REG_FUNC(0x2A12D9B7, sceKernelCancelEventFlag);
	//REG_FUNC(0x8BA4C0C1, sceKernelGetEventFlagInfo);
	//REG_FUNC(0x9EF9C0C5, sceKernelSetEventFlag);
	//REG_FUNC(0xD018793F, sceKernelClearEventFlag);
	//REG_FUNC(0x297AA2AE, sceKernelCreateSema);
	//REG_FUNC(0xC08F5BC5, sceKernelDeleteSema);
	//REG_FUNC(0xB028AB78, sceKernelOpenSema);
	//REG_FUNC(0x817707AB, sceKernelCloseSema);
	//REG_FUNC(0xC7B834B, sceKernelWaitSema);
	//REG_FUNC(0x174692B4, sceKernelWaitSemaCB);
	//REG_FUNC(0x66D6BF05, sceKernelCancelSema);
	//REG_FUNC(0x595D3FA6, sceKernelGetSemaInfo);
	//REG_FUNC(0x3012A9C6, sceKernelPollSema);
	//REG_FUNC(0x2053A496, sceKernelSignalSema);
	//REG_FUNC(0xED53334A, sceKernelCreateMutex);
	//REG_FUNC(0x12D11F65, sceKernelDeleteMutex);
	//REG_FUNC(0x16B85235, sceKernelOpenMutex);
	//REG_FUNC(0x43DDC9CC, sceKernelCloseMutex);
	//REG_FUNC(0x1D8D7945, sceKernelLockMutex);
	//REG_FUNC(0x2BDAA524, sceKernelLockMutexCB);
	//REG_FUNC(0x2144890D, sceKernelCancelMutex);
	//REG_FUNC(0x9A6C43CA, sceKernelGetMutexInfo);
	//REG_FUNC(0xE5901FF9, sceKernelTryLockMutex);
	//REG_FUNC(0x34746309, sceKernelUnlockMutex);
	//REG_FUNC(0x50572FDA, sceKernelCreateCond);
	//REG_FUNC(0xFD295414, sceKernelDeleteCond);
	//REG_FUNC(0xCB2A73A9, sceKernelOpenCond);
	//REG_FUNC(0x4FB91A89, sceKernelCloseCond);
	//REG_FUNC(0xC88D44AD, sceKernelWaitCond);
	//REG_FUNC(0x4CE42CE2, sceKernelWaitCondCB);
	//REG_FUNC(0x6864DCE2, sceKernelGetCondInfo);
	//REG_FUNC(0x10A4976F, sceKernelSignalCond);
	//REG_FUNC(0x2EB86929, sceKernelSignalCondAll);
	//REG_FUNC(0x87629E6, sceKernelSignalCondTo);
	//REG_FUNC(0xA10C1C8, sceKernelCreateMsgPipe);
	//REG_FUNC(0x69F6575D, sceKernelDeleteMsgPipe);
	//REG_FUNC(0x230691DA, sceKernelOpenMsgPipe);
	//REG_FUNC(0x7E5C0C16, sceKernelCloseMsgPipe);
	//REG_FUNC(0x94D506F7, sceKernelSendMsgPipeVector);
	//REG_FUNC(0x9C6F7F79, sceKernelSendMsgPipeVectorCB);
	//REG_FUNC(0x60DB346F, sceKernelTrySendMsgPipeVector);
	//REG_FUNC(0x9F899087, sceKernelReceiveMsgPipeVector);
	//REG_FUNC(0xBE5B3E27, sceKernelReceiveMsgPipeVectorCB);
	//REG_FUNC(0x86ECC0FF, sceKernelTryReceiveMsgPipeVector);
	//REG_FUNC(0xEF14BA37, sceKernelCancelMsgPipe);
	//REG_FUNC(0x4046D16B, sceKernelGetMsgPipeInfo);
	//REG_FUNC(0xDA6EC8EF, sceKernelCreateLwMutex);
	//REG_FUNC(0x244E76D2, sceKernelDeleteLwMutex);
	//REG_FUNC(0x4846613D, sceKernelGetLwMutexInfoById);
	//REG_FUNC(0x48C7EAE6, sceKernelCreateLwCond);
	//REG_FUNC(0x721F6CB3, sceKernelDeleteLwCond);
	//REG_FUNC(0xE1878282, sceKernelWaitLwCond);
	//REG_FUNC(0x8FA54B07, sceKernelWaitLwCondCB);
	//REG_FUNC(0x3AC63B9A, sceKernelSignalLwCond);
	//REG_FUNC(0xE5241A0C, sceKernelSignalLwCondAll);
	//REG_FUNC(0xFC1A48EB, sceKernelSignalLwCondTo);
	//REG_FUNC(0xE4DF36A0, sceKernelGetLwCondInfo);
	//REG_FUNC(0x971F1DE8, sceKernelGetLwCondInfoById);
	//REG_FUNC(0x2255B2A5, sceKernelCreateTimer);
	//REG_FUNC(0x746F3290, sceKernelDeleteTimer);
	//REG_FUNC(0x2F3D35A3, sceKernelOpenTimer);
	//REG_FUNC(0x17283DE6, sceKernelCloseTimer);
	//REG_FUNC(0x1478249B, sceKernelStartTimer);
	//REG_FUNC(0x75B1329, sceKernelStopTimer);
	//REG_FUNC(0x1F59E04D, sceKernelGetTimerBase);
	//REG_FUNC(0x3223CCD1, sceKernelGetTimerBaseWide);
	//REG_FUNC(0x381DC300, sceKernelGetTimerTime);
	//REG_FUNC(0x53C5D833, sceKernelGetTimerTimeWide);
	//REG_FUNC(0xFFAD717F, sceKernelSetTimerTime);
	//REG_FUNC(0xAF67678B, sceKernelSetTimerTimeWide);
	//REG_FUNC(0x621D293B, sceKernelSetTimerEvent);
	//REG_FUNC(0x9CCF768C, sceKernelCancelTimer);
	//REG_FUNC(0x7E35E10A, sceKernelGetTimerInfo);
	//REG_FUNC(0x8667951D, sceKernelCreateRWLock);
	//REG_FUNC(0x3D750204, sceKernelDeleteRWLock);
	//REG_FUNC(0xBA4DAC9A, sceKernelOpenRWLock);
	//REG_FUNC(0xA7F94E64, sceKernelCloseRWLock);
	//REG_FUNC(0xFA670F0F, sceKernelLockReadRWLock);
	//REG_FUNC(0x2D4A62B7, sceKernelLockReadRWLockCB);
	//REG_FUNC(0x1B8586C0, sceKernelTryLockReadRWLock);
	//REG_FUNC(0x675D10A8, sceKernelUnlockReadRWLock);
	//REG_FUNC(0x67A187BB, sceKernelLockWriteRWLock);
	//REG_FUNC(0xA4777082, sceKernelLockWriteRWLockCB);
	//REG_FUNC(0x597D4607, sceKernelTryLockWriteRWLock);
	//REG_FUNC(0xD9369DF2, sceKernelUnlockWriteRWLock);
	//REG_FUNC(0x190CA94B, sceKernelCancelRWLock);
	//REG_FUNC(0x79A573B, sceKernelGetRWLockInfo);
	//REG_FUNC(0x8AF15B5F, sceKernelGetSystemTime);
	//REG_FUNC(0x99B2BF15, sceKernelPMonThreadGetCounter);
	//REG_FUNC(0x7C21C961, sceKernelPMonCpuGetCounter);
	//REG_FUNC(0xADCA94E5, sceKernelWaitSignal);
	//REG_FUNC(0x24460BB3, sceKernelWaitSignalCB);
	//REG_FUNC(0x7BE9C4C8, sceKernelSendSignal);
	REG_FUNC(0xC5C11EE7, sceKernelCreateThread);
	//REG_FUNC(0x6C60AC61, sceIoOpen);
	//REG_FUNC(0xF5C6F098, sceIoClose);
	//REG_FUNC(0x713523E1, sceIoRead);
	//REG_FUNC(0x11FED231, sceIoWrite);
	//REG_FUNC(0x99BA173E, sceIoLseek);
	//REG_FUNC(0x5CC983AC, sceIoLseek32);
	//REG_FUNC(0xE20ED0F3, sceIoRemove);
	//REG_FUNC(0xF737E369, sceIoRename);
	//REG_FUNC(0x9670D39F, sceIoMkdir);
	//REG_FUNC(0xE9F91EC8, sceIoRmdir);
	//REG_FUNC(0xA9283DD0, sceIoDopen);
	//REG_FUNC(0x9DFF9C59, sceIoDclose);
	//REG_FUNC(0x9C8B6624, sceIoDread);
	//REG_FUNC(0xBCA5B623, sceIoGetstat);
	//REG_FUNC(0x29482F7F, sceIoChstat);
	//REG_FUNC(0x98ACED6D, sceIoSync);
	//REG_FUNC(0x4B30CB2, sceIoDevctl);
	//REG_FUNC(0x54ABACFA, sceIoIoctl);
	//REG_FUNC(0x6A7EA9FD, sceIoOpenAsync);
	//REG_FUNC(0x84201C9B, sceIoCloseAsync);
	//REG_FUNC(0x7B3BE857, sceIoReadAsync);
	//REG_FUNC(0x21329B20, sceIoWriteAsync);
	//REG_FUNC(0xCAC5D672, sceIoLseekAsync);
	//REG_FUNC(0x99C54B9, sceIoIoctlAsync);
	//REG_FUNC(0x446A60AC, sceIoRemoveAsync);
	//REG_FUNC(0x73FC184B, sceIoDopenAsync);
	//REG_FUNC(0x4D0597D7, sceIoDcloseAsync);
	//REG_FUNC(0xCE32490D, sceIoDreadAsync);
	//REG_FUNC(0x8E5FCBB1, sceIoMkdirAsync);
	//REG_FUNC(0x9694D00F, sceIoRmdirAsync);
	//REG_FUNC(0xEE9857CD, sceIoRenameAsync);
	//REG_FUNC(0x9739A5E2, sceIoChstatAsync);
	//REG_FUNC(0x82B20B41, sceIoGetstatAsync);
	//REG_FUNC(0x950F78EB, sceIoDevctlAsync);
	//REG_FUNC(0xF7C7FBFE, sceIoSyncAsync);
	//REG_FUNC(0xEC96EA71, sceIoCancel);
	//REG_FUNC(0x857E0C71, sceIoComplete);
	//REG_FUNC(0x52315AD7, sceIoPread);
	//REG_FUNC(0x8FFFF5A8, sceIoPwrite);
	//REG_FUNC(0xA010141E, sceIoPreadAsync);
	//REG_FUNC(0xED25BEEF, sceIoPwriteAsync);
	//REG_FUNC(0xA792C404, sceIoCompleteMultiple);
	//REG_FUNC(0x894037E8, sceKernelBacktrace);
	//REG_FUNC(0x20E2D4B7, sceKernelPrintBacktrace);
	//REG_FUNC(0x963F4A99, sceSblACMgrIsGameProgram);
	//REG_FUNC(0x261E2C34, sceKernelGetOpenPsId);

	/* SceModulemgr */
	//REG_FUNC(0x36585DAF, sceKernelGetModuleInfo);
	//REG_FUNC(0x2EF2581F, sceKernelGetModuleList);
	//REG_FUNC(0xF5798C7C, sceKernelGetModuleIdByAddr);

	/* SceProcessmgr */
	//REG_FUNC(0xCD248267, sceKernelGetCurrentProcess);
	//REG_FUNC(0x2252890C, sceKernelPowerTick);
	//REG_FUNC(0x9E45DA09, sceKernelLibcClock);
	//REG_FUNC(0x39BE45, sceKernelLibcTime);
	//REG_FUNC(0x4B879059, sceKernelLibcGettimeofday);
	//REG_FUNC(0xC1727F59, sceKernelGetStdin);
	//REG_FUNC(0xE5AA625C, sceKernelGetStdout);
	//REG_FUNC(0xFA5E3ADA, sceKernelGetStderr);
	//REG_FUNC(0xE6E9FCA3, sceKernelGetRemoteProcessTime);
	//REG_FUNC(0xD37A8437, sceKernelGetProcessTime);
	//REG_FUNC(0xF5D0D4C6, sceKernelGetProcessTimeLow);
	//REG_FUNC(0x89DA0967, sceKernelGetProcessTimeWide);
	//REG_FUNC(0x2BE3E066, sceKernelGetProcessParam);

	/* SceStdio */
	//REG_FUNC(0x54237407, sceKernelStdin);
	//REG_FUNC(0x9033E9BD, sceKernelStdout);
	//REG_FUNC(0x35EE7CF5, sceKernelStderr);

	/* SceSysmem */
	//REG_FUNC(0xB9D5EBDE, sceKernelAllocMemBlock);
	//REG_FUNC(0xA91E15EE, sceKernelFreeMemBlock);
	//REG_FUNC(0xB8EF5818, sceKernelGetMemBlockBase);
	//REG_FUNC(0x3B29E0F5, sceKernelRemapMemBlock);
	//REG_FUNC(0xA33B99D1, sceKernelFindMemBlockByAddr);
	//REG_FUNC(0x4010AD65, sceKernelGetMemBlockInfoByAddr);

	/* SceCpu */
	//REG_FUNC(0x2704CFEE, sceKernelCpuId);

	/* SceDipsw */
	//REG_FUNC(0x1C783FB2, sceKernelCheckDipsw);
	//REG_FUNC(0x817053D4, sceKernelSetDipsw);
	//REG_FUNC(0x800EDCC1, sceKernelClearDipsw);

	/* SceThreadmgr */
	REG_FUNC(0xC8A38E1, sceKernelExitThread);
	REG_FUNC(0x1D17DECF, sceKernelExitDeleteThread);
	REG_FUNC(0x4B675D05, sceKernelDelayThread);
	REG_FUNC(0x9C0180E1, sceKernelDelayThreadCB);
	//REG_FUNC(0x1173F8, sceKernelChangeActiveCpuMask);
	REG_FUNC(0x1414F0B, sceKernelGetThreadCurrentPriority);
	REG_FUNC(0x751C9B7A, sceKernelChangeCurrentThreadAttr);
	REG_FUNC(0xD9BD74EB, sceKernelCheckWaitableStatus);
	REG_FUNC(0x9DCB4B7A, sceKernelGetProcessId);
	//REG_FUNC(0xE53E41F6, sceKernelCheckCallback);
	//REG_FUNC(0xF4EE4FA9, sceKernelGetSystemTimeWide);
	//REG_FUNC(0x47F6DE49, sceKernelGetSystemTimeLow);
	//REG_FUNC(0xC0FAF6A3, sceKernelCreateThreadForUser);

	/* SceDebugLed */
	//REG_FUNC(0x78E702D3, sceKernelSetGPO);

	return psv_log_base("sceLibKernel");
}();
