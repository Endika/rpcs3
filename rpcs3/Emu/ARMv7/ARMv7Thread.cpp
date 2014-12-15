#include "stdafx.h"
#include "rpcs3/Ini.h"
#include "Utilities/Log.h"
#include "Emu/Memory/Memory.h"
#include "Emu/System.h"
#include "Emu/CPU/CPUThreadManager.h"

#include "ARMv7Thread.h"
#include "ARMv7Decoder.h"
#include "ARMv7DisAsm.h"
#include "ARMv7Interpreter.h"

ARMv7Thread::ARMv7Thread()
	: CPUThread(CPU_THREAD_ARMv7)
	, m_arg(0)
	, m_last_instr_size(0)
	, m_last_instr_name("UNK")
{
}

void ARMv7Thread::InitRegs()
{
	memset(GPR, 0, sizeof(GPR[0]) * 15);
	APSR.APSR = 0;
	IPSR.IPSR = 0;
	ISET = Thumb;
	ITSTATE.IT = 0;
	SP = m_stack_addr + m_stack_size;
}

void ARMv7Thread::InitStack()
{
	if(!m_stack_addr)
	{
		m_stack_size = 0x10000;
		m_stack_addr = (u32)Memory.Alloc(0x10000, 1);
	}
}

u32 ARMv7Thread::GetStackArg(u32 pos)
{
	return vm::psv::read32(SP + sizeof(u32) * (pos - 5));
}

std::string ARMv7Thread::RegsToString()
{
	std::string result = "Registers:\n=========\n";
	for(int i=0; i<15; ++i)
	{
		result += fmt::Format("%s\t= 0x%08x\n", g_arm_reg_name[i], GPR[i]);
	}

	result += fmt::Format("APSR\t= 0x%08x [N: %d, Z: %d, C: %d, V: %d, Q: %d]\n", 
		APSR.APSR,
		fmt::by_value(APSR.N),
		fmt::by_value(APSR.Z),
		fmt::by_value(APSR.C),
		fmt::by_value(APSR.V),
		fmt::by_value(APSR.Q));
	
	return result;
}

std::string ARMv7Thread::ReadRegString(const std::string& reg)
{
	return "";
}

bool ARMv7Thread::WriteRegString(const std::string& reg, std::string value)
{
	return true;
}

void ARMv7Thread::DoReset()
{
}

void ARMv7Thread::DoRun()
{
	switch(Ini.CPUDecoderMode.GetValue())
	{
	case 0:
		//m_dec = new ARMv7Decoder(*new ARMv7DisAsm());
	break;

	case 1:
	case 2:
		m_dec = new ARMv7Decoder(*this);
	break;
	}
}

void ARMv7Thread::DoPause()
{
}

void ARMv7Thread::DoResume()
{
}

void ARMv7Thread::DoStop()
{
}

void ARMv7Thread::DoCode()
{
}

void ARMv7Thread::FastCall(u32 addr)
{
	auto old_status = m_status;
	auto old_PC = PC;
	auto old_stack = SP;
	auto old_LR = LR;
	auto old_thread = GetCurrentNamedThread();

	m_status = Running;
	PC = addr;
	LR = Emu.GetCPUThreadStop();
	SetCurrentNamedThread(this);

	CPUThread::Task();

	m_status = old_status;
	PC = old_PC;
	SP = old_stack;
	LR = old_LR;
	SetCurrentNamedThread(old_thread);
}

void ARMv7Thread::FastStop()
{
	m_status = Stopped;
}

arm7_thread::arm7_thread(u32 entry, const std::string& name, u32 stack_size, u32 prio)
{
	thread = &Emu.GetCPU().AddThread(CPU_THREAD_ARMv7);

	thread->SetName(name);
	thread->SetEntry(entry);
	thread->SetStackSize(stack_size ? stack_size : Emu.GetInfo().GetProcParam().primary_stacksize);
	thread->SetPrio(prio ? prio : Emu.GetInfo().GetProcParam().primary_prio);

	argc = 0;
}
