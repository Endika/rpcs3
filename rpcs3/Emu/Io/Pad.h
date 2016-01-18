#pragma once

#include "PadHandler.h"

class PadManager
{
	std::unique_ptr<PadHandlerBase> m_pad_handler;

public:
	void Init(u32 max_connect);
	void Close();

	std::vector<Pad>& GetPads() { return m_pad_handler->GetPads(); }
	PadInfo& GetInfo() { return m_pad_handler->GetInfo(); }
	std::vector<Button>& GetButtons(const u32 pad) { return m_pad_handler->GetButtons(pad); }

	bool IsInited() const { return m_pad_handler.operator bool(); }
};
