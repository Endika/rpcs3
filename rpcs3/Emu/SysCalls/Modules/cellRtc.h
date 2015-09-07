#pragma once

namespace vm { using namespace ps3; }

// Return Codes
enum
{
	CELL_RTC_ERROR_NOT_INITIALIZED      = 0x80010601,
	CELL_RTC_ERROR_INVALID_POINTER      = 0x80010602,
	CELL_RTC_ERROR_INVALID_VALUE        = 0x80010603,
	CELL_RTC_ERROR_INVALID_ARG          = 0x80010604,
	CELL_RTC_ERROR_NOT_SUPPORTED        = 0x80010605,
	CELL_RTC_ERROR_NO_CLOCK             = 0x80010606,
	CELL_RTC_ERROR_BAD_PARSE            = 0x80010607,
	CELL_RTC_ERROR_INVALID_YEAR         = 0x80010621,
	CELL_RTC_ERROR_INVALID_MONTH        = 0x80010622,
	CELL_RTC_ERROR_INVALID_DAY          = 0x80010623,
	CELL_RTC_ERROR_INVALID_HOUR         = 0x80010624,
	CELL_RTC_ERROR_INVALID_MINUTE       = 0x80010625,
	CELL_RTC_ERROR_INVALID_SECOND       = 0x80010626,
	CELL_RTC_ERROR_INVALID_MICROSECOND  = 0x80010627,
};

struct CellRtcTick
{
	be_t<u64> tick;
};

struct CellRtcDateTime
{
	be_t<u16> year;
	be_t<u16> month;
	be_t<u16> day;
	be_t<u16> hour;
	be_t<u16> minute;
	be_t<u16> second;
	be_t<u32> microsecond;
};
