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
        EtherCATMain(int nArgc, char* ppArgv[], int bufferSize);
		~EtherCATMain();
	public:
		static EtherCATMain* createInstance(int nArgc, char* ppArgv[], int bufferSize);
		static EtherCATMain* getInstance();
		
		bool isRunning();
		void join();
		void stop();
		
		int getBufferSize()			{ return bufferSize; };
		uint8_t* getInBuffer()		{ return inBuffer; };		// read from ethercat network
		uint8_t* getOutBuffer()		{ return outBuffer; };		// gets written to ethercat network
		std::mutex* getMutex()		{ return &m; };
		std::condition_variable* getConditionalVariable()	{ return &cv; };
		
		masterState getMasterState();
		
		// set buffer funcitons
		EC_T_BYTE getFrmByte ( uint8_t* address );
		EC_T_WORD getFrmWord ( uint8_t* address );
		EC_T_DWORD getFrmDWord ( uint8_t* address );
		void setByte( uint8_t* address, EC_T_BYTE val );
		void setWord( uint8_t* address, EC_T_WORD val );
		void setDWord( uint8_t* address, EC_T_DWORD val );
		
		struct pseudoLock {
			int lockCount;
			void lock() { lockCount++; };
			void unlock() { lockCount--; };
		};
		
		pseudoLock lockInBuffer;
		pseudoLock lockOutBuffer;
		
		
		
	protected:
		void callbackFct(EC_T_BYTE* pbyPDInPtr, EC_T_BYTE* pbyPDOutPtr);
		
		
	private:
		static EtherCATMain* instance;
		int nArgc;
		char** ppArgv;
		void (*callbackFctPtr)(EC_T_BYTE*, EC_T_BYTE*);
		
		int bufferSize;
		uint8_t* inBuffer;
		uint8_t* outBuffer;
		
		std::thread thread;    
		std::mutex m;
		std::condition_variable cv;
	};
};

#endif // ETHERCAT_ETHERCATMAIN_HPP_
