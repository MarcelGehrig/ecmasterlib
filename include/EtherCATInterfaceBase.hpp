#ifndef ETHERCAT_INTERFACE_BASE_HPP_
#define ETHERCAT_INTERFACE_BASE_HPP_

#include <EtherCATInterfaceBase_config.hpp>
#include <EcMasterlibMain.hpp>

namespace etherCATInterface {
	class EtherCATInterfaceBase {
	public:
		EtherCATInterfaceBase(ecmasterlib::EcMasterlibMain* etherCATStack, const int numberOfDrives, int bytesPerSlaveTx, int bytesPerSlaveRx);
		
		// basic set functions:
		void set8bit(uint32_t offsetInByte, uint8_t payload);
		void set8bit(uint32_t offsetInByte, uint32_t driveNumber, uint8_t payload);
		void set8bit(uint32_t offsetInByte, int8_t payload);
		void set8bit(uint32_t offsetInByte, uint32_t driveNumber, int8_t payload);
		void set16bit(uint32_t offsetInByte, uint16_t payload);
		void set16bit(uint32_t offsetInByte, uint32_t driveNumber, uint16_t payload);
		void set16bit(uint32_t offsetInByte, int16_t payload);
		void set16bit(uint32_t offsetInByte, uint32_t driveNumber, int16_t payload);
		void set32bit(uint32_t offsetInByte, uint32_t payload);
		void set32bit(uint32_t offsetInByte, uint32_t driveNumber, uint32_t payload);
		void set32bit(uint32_t offsetInByte, int32_t payload);
		void set32bit(uint32_t offsetInByte, uint32_t driveNumber, int32_t payload);
		
		// basic get functions:
		uint8_t get8bit(uint32_t offsetInByte); 
		uint8_t get8bit(uint32_t offsetInByte, uint32_t driveNumber);
		uint16_t get16bit(uint32_t offsetInByte); 
		uint16_t get16bit(uint32_t offsetInByte, uint32_t driveNumber);
		uint32_t get32bit(uint32_t offsetInByte); 
		uint32_t get32bit(uint32_t offsetInByte, uint32_t driveNumber);
		
		bool checkOffset(int offsetInByte);
		bool checkMaskedBits(uint16_t variable, uint16_t compareWord, uint16_t mask);
        
		ecmasterlib::EcMasterlibMain* etherCATStack;
        const int numberOfDrives;
        int bytesPerSlaveTx;
        int bytesPerSlaveRx;

		private:
		uint8_t* inBuffer;
		uint8_t* outBuffer;
	};
}


#endif // ETHERCAT_INTERFACE_BASE_HPP_
