#include "memory.h"

#include "driver.h"
#include "Frontend/Windows/LogWindow.h"
#include <Engine/Userdefined/Offsets.h>


Memory::Memory()
{
	windows::LogWindow::Log(windows::LogWindow::logLevels::LOGLEVEL_ONLY_LOG, "MEMORY", "Initializing memory class...");
	//only call the init function if status is bad
	if (status == bad)
	{
		//call the init function
		init();

		//set the status to inizilized
		status = inizilaized;
		windows::LogWindow::Log(windows::LogWindow::logLevels::LOGLEVEL_INFO, "MEMORY", "Initialized Memory class!");
	}

}

Memory::LoadError Memory::load(std::string processName)
{
	//should not happen!
	if (status == bad) DebugBreak();

	//only call the load function if the status is initialized
	if (status == inizilaized)
	{
		loadData(processName, baseAddress, processID);

		if (!baseAddress) {
			windows::LogWindow::Log(windows::LogWindow::logLevels::LOGLEVEL_ERROR, "MEMORY", "Error getting base address!");
			return noBaseAddress;
		}

		if (!processID) {
			windows::LogWindow::Log(windows::LogWindow::logLevels::LOGLEVEL_ERROR, "MEMORY", "Error getting process ID!");
			return noProcessID;
		}

		windows::LogWindow::Log(windows::LogWindow::logLevels::LOGLEVEL_INFO, "MEMORY", "Loaded Memory class!");
	}

	status = loaded;
	return success;
}

Memory::LoadError Memory::load(int processPID)
{
	//should not happen!
	if (status == bad) DebugBreak();

	//only call the load function if the status is initialized
	if (status == inizilaized)
	{
		baseAddress = _getBaseAddress(nullptr, processPID);

		if (!baseAddress) {
			windows::LogWindow::Log(windows::LogWindow::logLevels::LOGLEVEL_ERROR, "MEMORY", "Error getting base address!");
			return noBaseAddress;
		}

		processID = processPID;
		if (!processID) {
			windows::LogWindow::Log(windows::LogWindow::logLevels::LOGLEVEL_ERROR, "MEMORY", "Error getting process ID!");
			return noProcessID;
		}

		windows::LogWindow::Log(windows::LogWindow::logLevels::LOGLEVEL_INFO, "MEMORY", "Loaded Memory class!");
	}

	status = loaded;
	return success;
}

void Memory::checkStatus()
{
	if (status != loaded) DebugBreak();
}

Memory::MemoryStatus Memory::getStatus()
{
	return status;
}

uint64_t Memory::getBaseAddress()
{
	return baseAddress;
}

int Memory::getProcessID()
{
	return processID;
}

int Memory::getTotalReads()
{
	return totalReads;
}

int Memory::getTotalWrites()
{
	return totalWrites;
}

void Memory::read(const void* address, void* buffer, const DWORD64 size)
{
	totalReads++;
	checkStatus();

	_read(address, buffer, size);
}

void Memory::write(void* address, const void* buffer, const DWORD64 size)
{
	totalWrites++;
	checkStatus();

	_write(address, buffer, size);
}

BOOLEAN CheckMask(const char* Base, const char* Pattern, const char* Mask) {
	for (; *Mask; ++Base, ++Pattern, ++Mask) {
		if (*Mask == 'x' && *Base != *Pattern) {
			return FALSE;
		}
	}
	return TRUE;
}