#include <eeros/control/SignalBufferReader.hpp>
#include <eeros/core/RingBuffer.hpp>
#include <eeros/control/RealSignalOutput.hpp>
#include <string.h>

SignalBufferReader::SignalBufferReader(void* memory, uint32_t size) : SignalBuffer(memory, size) {
	headerVersion = header->version;
	readIndex = 0;
}

uint32_t SignalBufferReader::nofObservedSignals() {
	return header->nofObservedSignals;
}

uint32_t SignalBufferReader::signalTypeAvailableToRead() {
	if(nofObservedSignals() > 0 && buffer->avalableToRead() >= getSignalDataSize(getSignalType(readIndex))) {
		return getSignalType(readIndex);
	}
	return kSignalTypeUnknown;
}

bool SignalBufferReader::readRealSignal(uint32_t* id, uint64_t* timestamp, double* value) {
	if(signalTypeAvailableToRead() == kSignalTypeReal) {
		*id = getSignalId(readIndex);
		buffer->read(timestamp, sizeof(*timestamp));
		buffer->read(value, sizeof(*value));
		incrementReadIndex();
		return true;
	}
	return false;
}

sigtype_t SignalBufferReader::getSignalType(int index) {
	if(index >= 0 && index < nofObservedSignals()) {
		return header->signalInfo[index * 2 + 1];
	}
	return kSignalTypeUnknown;
}

sigid_t SignalBufferReader::getSignalId(int index) {
	if(index >= 0 && index < nofObservedSignals()) {
		return header->signalInfo[index * 2];
	}
	return kSignalIdInvalid;
}

uint32_t SignalBufferReader::getSignalDataSize(sigtype_t signalType) {
	switch(signalType) {
		case kSignalTypeReal:
			return kSignalDataSizeReal;
		default:
			return -1;
	}
}

void SignalBufferReader::incrementReadIndex() {
	if(headerVersion != header->version) { // header has changed -> all data in buffer are invalid
		readIndex = 0;
		headerVersion = header->version;
	}
	else {
		uint32_t n = nofObservedSignals();
		if(n > 0 && readIndex < n - 1) readIndex++;
		else readIndex = 0;
	}
}