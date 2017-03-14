#ifndef ETHERCAT_ETHERCATMAIN_HPP_
#define ETHERCAT_ETHERCATMAIN_HPP_

#include <iostream>
#include <thread>

#include <string>
#include <atomic>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <EcType.h>
#include <EcOs.h>

namespace ethercat {
	
	enum masterState {
		UNKNOWN,
		INIT,
		PREOP,
		SAFEOP,
		OP,
		
		BOOTSTRAP
	};
	
	
	class EtherCATMain {
		
	public:
// 		EtherCATMain(int nArgc, char* ppArgv[], void (*callbackFctPtr)(EC_T_BYTE* pbyPDIn, EC_T_BYTE* pbyPDOut));
		EtherCATMain(int nArgc, char* ppArgv[]);
		
		void runStack(void (*callbackFctPtr)(EC_T_BYTE* pbyPDIn, EC_T_BYTE* pbyPDOut));
		
		EC_T_BOOL isRunning();
		void join();
		void stop();
		
		EC_T_BYTE* getPbyPDIn();
		EC_T_BYTE* getPbyPDOut();
		
		masterState getMasterState();
		void callbackFct(EC_T_BYTE* pbyPDInPtr, EC_T_BYTE* pbyPDOutPtr);
		
	private:
		bool deepCopy;	//PDO gets copied
		int nArgc;
		char** ppArgv;
		void (*callbackFctPtr)(EC_T_BYTE*, EC_T_BYTE*);
// 		void (*callbackFctPtr)(EC_T_BYTE* pbyPDIn, EC_T_BYTE* pbyPDOut);
		std::thread thread;	
		std::mutex m;
		std::condition_variable cv;
	};
};

#endif // ETHERCAT_ETHERCATMAIN_HPP_
