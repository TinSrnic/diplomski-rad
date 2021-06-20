#pragma once
#include <Windows.h>
#include <psapi.h>
#include <TCHAR.h>
#include <pdh.h>

class ProcessInfo {

private:
	PDH_HQUERY cpuQuery;
	PDH_HCOUNTER cpuTotal;

	ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
	int numProcessors;
	HANDLE self;

public:
	float m_totalVirtualMemory;
	float m_virtualMemoryUsed;
	float m_virtualMemoryUsedProcess;
	
	float m_totalPhysicalMemory;
	float m_physicalMemoryUsed;
	float m_phyisicalMemoryUsedProcess;

	double m_CPUTotalUsed;
	double m_CPUProcessUsed;

public:
	ProcessInfo() {
		InitCPUTotal();
		InitCPUCurrentProcess();
	}

	void Update() {
		m_CPUTotalUsed = GetCPUTotalUsed();
		m_CPUProcessUsed = GetCPUUsedByCurrentProcess();
		GetTotalMemoryInfo(m_totalVirtualMemory, m_virtualMemoryUsed, m_totalPhysicalMemory, m_physicalMemoryUsed);
		GetCurrentProcessMemoryInfo(m_virtualMemoryUsedProcess, m_phyisicalMemoryUsedProcess);
	}

private:
	void InitCPUTotal() {
		PdhOpenQuery(NULL, NULL, &cpuQuery);
		// You can also use L"\\Processor(*)\\% Processor Time" and get individual CPU values with PdhGetFormattedCounterArray()
		PdhAddEnglishCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
		PdhCollectQueryData(cpuQuery);
	}
	void InitCPUCurrentProcess() {
		SYSTEM_INFO sysInfo;
		FILETIME ftime, fsys, fuser;

		GetSystemInfo(&sysInfo);
		numProcessors = sysInfo.dwNumberOfProcessors;

		GetSystemTimeAsFileTime(&ftime);
		memcpy(&lastCPU, &ftime, sizeof(FILETIME));

		self = GetCurrentProcess();
		GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
		memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
		memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));
	}

	double GetCPUTotalUsed() {
		PDH_FMT_COUNTERVALUE counterVal;

		PdhCollectQueryData(cpuQuery);
		PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
		return counterVal.doubleValue;
	}
	double GetCPUUsedByCurrentProcess() {
		FILETIME ftime, fsys, fuser;
		ULARGE_INTEGER now, sys, user;
		double percent;

		GetSystemTimeAsFileTime(&ftime);
		memcpy(&now, &ftime, sizeof(FILETIME));

		GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
		memcpy(&sys, &fsys, sizeof(FILETIME));
		memcpy(&user, &fuser, sizeof(FILETIME));
		percent = static_cast<double>((sys.QuadPart - lastSysCPU.QuadPart) + (user.QuadPart - lastUserCPU.QuadPart));
		percent /= (now.QuadPart - lastCPU.QuadPart);
		percent /= numProcessors;
		lastCPU = now;
		lastUserCPU = user;
		lastSysCPU = sys;

		return percent * 100;
	}

	void GetTotalMemoryInfo(float& totalVirtualMemory, float& virtualMemoryUsed, float& totalPhysMem, float& physMemUsed) {
		MEMORYSTATUSEX memInfo;
		memInfo.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&memInfo);
		float bytesToGB = 1000000000.0f;
		totalVirtualMemory = memInfo.ullTotalPageFile / bytesToGB;
		virtualMemoryUsed = (memInfo.ullTotalPageFile - memInfo.ullAvailPageFile) / bytesToGB;
		totalPhysMem = memInfo.ullTotalPhys / bytesToGB;
		physMemUsed = (memInfo.ullTotalPhys - memInfo.ullAvailPhys) / bytesToGB;
	}
	void GetCurrentProcessMemoryInfo(float& virtualMemoryUsedByProcess, float& physicalMemoryUsedByProcess) {
		PROCESS_MEMORY_COUNTERS_EX pmc;
		GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
		float bytesToGB = 1000000000.0f;
		virtualMemoryUsedByProcess = pmc.PrivateUsage / bytesToGB;
		physicalMemoryUsedByProcess = pmc.WorkingSetSize / bytesToGB;
	}
};