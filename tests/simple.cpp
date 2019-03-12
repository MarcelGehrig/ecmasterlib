/*-INCLUDES------------------------------------------------------------------*/
#include <AtEthercat.h>
#include <EcCommon.h>
#include <EcInterfaceCommon.h>

#include <EtherCATStack.h>
#include <selectLinkLayer.h>
#include <Logging.h>


#include "simple.hpp"



using namespace ethercat;

void callbackFct(EC_T_BYTE* pbyPDInPtr, EC_T_BYTE* pbyPDOutPtr)
{
	EtherCATMain* inst = EtherCATMain::getInstance();
	//TODO error if null
	
	static int counter = 0;
	static constexpr int maxRetries = 100;
	
// 	if (counter == 0) {
// 		std::cout << "EtherCATMain, counter == 0: " << std::endl;
// 		std::cout << "inst: " << inst << std::endl;
// 		std::cout << "inst->getBufferSize(): " << inst->getBufferSize() << std::endl;
// 		std::cout << "inst->getInBuffer():   " << (void*)inst->getInBuffer() << std::endl;
// 		std::cout << "pbyPDInPtr:            " << (void*)pbyPDInPtr << std::endl;
// 		std::cout << "inst->getOutBuffer():  " << (void*)inst->getOutBuffer() << std::endl;
// 		std::cout << "pbyPDOutPtr:           " << (void*)pbyPDOutPtr << std::endl;
// 		std::cout << std::endl;
// 	}
// 	
// 	if (counter == 100) {
// 		std::cout << "EtherCATMain, counter == 100: " << std::endl;
// 		std::cout << "inst: " << inst << std::endl;
// 		std::cout << "inst->getBufferSize(): " << inst->getBufferSize() << std::endl;
// 		std::cout << "inst->getInBuffer():   " << (void*)inst->getInBuffer() << std::endl;
// 		std::cout << "pbyPDInPtr:            " << (void*)pbyPDInPtr << std::endl;
// 		std::cout << "inst->getOutBuffer():  " << (void*)inst->getOutBuffer() << std::endl;
// 		std::cout << "pbyPDOutPtr:           " << (void*)pbyPDOutPtr << std::endl;
// 		std::cout << std::endl;
// 	}
// 	
// 	if (counter == 150) {
// 		std::cout << "EtherCATMain, counter == 150: " << std::endl;
// 		std::cout << "inst: " << inst << std::endl;
// 		std::cout << "inst->getBufferSize(): " << inst->getBufferSize() << std::endl;
// 		std::cout << "inst->getInBuffer():   " << (void*)inst->getInBuffer() << std::endl;
// 		std::cout << "pbyPDInPtr:            " << (void*)pbyPDInPtr << std::endl;
// 		std::cout << "inst->getOutBuffer():  " << (void*)inst->getOutBuffer() << std::endl;
// 		std::cout << "pbyPDOutPtr:           " << (void*)pbyPDOutPtr << std::endl;
// 		std::cout << std::endl;
// 	}
	
	
	if (counter > 200) {		// 10 are not enough
		
		// copy to inBuffer from stack
		int counterRetries = 0;
		while ( (inst->lockInBuffer.lockCount != 0) && (counterRetries < maxRetries) ) {
			usleep(1);
			counterRetries++;
		}
		if (inst->lockInBuffer.lockCount != 0) {
			std::cout << "WARNING: lockInBuffer.lockCount = " << inst->lockInBuffer.lockCount << "     retries: " << maxRetries << std::endl;
		}
		else {
			memcpy(inst->getInBuffer(), pbyPDInPtr, inst->getBufferSize());
		}
		
		
		// copy to stack from outBuffer
		counterRetries = 0;
		while ( (inst->lockOutBuffer.lockCount != 0) && (counterRetries < maxRetries) ) {
			usleep(1);
			counterRetries++;
		}
		if (inst->lockOutBuffer.lockCount != 0) {
			std::cout << "WARNING: lockOutBuffer.lockCount = " << inst->lockOutBuffer.lockCount << "     retries: " << maxRetries << std::endl;
		}
		else {
			memcpy(pbyPDOutPtr, inst->getOutBuffer(), inst->getBufferSize());
		}
	}
	
	if (counter<1000) counter++;
	// wake conditional variable
	inst->getConditionalVariable()->notify_one();
}









int main(int argc, char **argv) {
  
  
      dwRes = EtherCATStack(   &oLogging,
                      eCnfType, pbyCnfData, dwCnfDataLen,
                      TimingDesc.dwBusCycleTimeUsec, nVerbose, dwDuration,
//                       TimingDesc.dwBusCycleTimeUsec, 0, dwDuration,
                      apLinkParms[0],
                      TimingDesc.pvTimingEvent, dwCpuIndex,
                      bEnaPerfJobs
#if (defined ATEMRAS_SERVER)
                      ,wServerPort
#endif
                      ,((2 == dwNumLinkLayer)?apLinkParms[1]:EC_NULL)
                      , &oOsParms
                      , eDcmMode
                      , bCtlOff
                      ,&callbackFct
                      ,licenseKey
                    );
      
      
      
      sleep(15);
  
  
}







EtherCATMain::EtherCATMain(int nArgc, char* ppArgv[], int bufferSize) :
nArgc(nArgc), ppArgv(ppArgv), bufferSize(bufferSize),
thread(mainEtherCAT, nArgc, ppArgv )
{ 
	lockInBuffer.lockCount=0;
	lockOutBuffer.lockCount=0;
	inBuffer = new uint8_t[bufferSize];
	outBuffer = new uint8_t[bufferSize];
	ecatGetMasterState();
}

EtherCATMain::~EtherCATMain() {
	delete []inBuffer;
	delete []outBuffer;
}

EtherCATMain* EtherCATMain::instance;

EtherCATMain* EtherCATMain::createInstance(int nArgc, char* ppArgv[], int bufferSize) {
	if(instance == NULL) instance = new EtherCATMain(nArgc, ppArgv, bufferSize);
	return instance;
}

EtherCATMain* EtherCATMain::getInstance() {
	return instance;
}


bool EtherCATMain::isRunning()
{
	return bRun;
}

void EtherCATMain::join()
{
	thread.join();
}

void EtherCATMain::stop()
{
	bRun = EC_FALSE;
}


masterState EtherCATMain::getMasterState()
{
	switch ( ecatGetMasterState() ) 
	{
		case eEcatState_UNKNOWN		: return ethercat::UNKNOWN;
		case eEcatState_INIT		: return ethercat::INIT;
		case eEcatState_PREOP		: return ethercat::PREOP;
		case eEcatState_SAFEOP		: return ethercat::SAFEOP;
		case eEcatState_OP			: return ethercat::OP;
		case eEcatState_BOOTSTRAP	: return ethercat::BOOTSTRAP;
	}
}


// get from buffer functions
EC_T_WORD EtherCATMain::getFrmWord(uint8_t* address)
{
	lockInBuffer.lock();
	EC_T_WORD val = EC_GET_FRM_WORD( address );
	lockInBuffer.unlock();
	return val;
}

EC_T_DWORD EtherCATMain::getFrmDWord(uint8_t* address)
{
	lockInBuffer.lock();
	EC_T_DWORD val = EC_GET_FRM_DWORD( address );
	lockInBuffer.unlock();
	return val;
}



// set to buffer funcitons
void EtherCATMain::setByte(uint8_t* address, EC_T_BYTE val)
{
	lockOutBuffer.lock();
// 	EC_T_BYTE *pointer = static_cast<EC_T_BYTE*>( address )
// 	*(pointer) = val;
	*(address) = val;
	lockOutBuffer.unlock();
}

void EtherCATMain::setWord(uint8_t* address, EC_T_WORD val)
{
	lockOutBuffer.lock();
	EC_SETWORD( address, val );
	lockOutBuffer.unlock();
}

void EtherCATMain::setDWord(uint8_t* address, EC_T_DWORD val)
{
	lockOutBuffer.lock();
	EC_SETDWORD( address, val );
	lockOutBuffer.unlock();
}
