#include <eeros/sequencer/Sequence.hpp>
#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/sequencer/SequenceException.hpp>
#include <eeros/sequencer/ErrorHandler.hpp>

using namespace eeros::sequencer;

std::list<eeros::sequencer::Sequence*> Sequence::allSequences;

Sequence::Sequence(std::string name, Sequencer& caller)
	: sequenceName(name),
      callerThread(caller),
	  state(kSequenceNotStarted) {
	if(&callerThread != Sequencer::getMainSequencer()){
		Sequencer::getMainSequencer()->addSubSequencer(&caller);
	}
	Sequence* seq = getSequence(sequenceName);
	if(seq){
		 throw "Sequence allready exists!";
	}
	Sequence::allSequences.push_back(this);
	currentCallBackIterator = callBacks.end();
}

Sequence::~Sequence(){
	removeSequence(sequenceName);
}

std::string Sequence::getName(){
	return sequenceName;
}

void Sequence::addCallBack(eeros::sequencer::Sequence::method callback){
	callBacks.push_back(callback);
}

std::list<Sequence::method>::iterator Sequence::findCallBack(method callback, bool setCurrent) {
	std::list<Sequence::method>::iterator iter = callBacks.begin();
	while(iter != callBacks.end()){
		if(*iter == callback){
			if(setCurrent){
				currentCallBackIterator = iter;
			}
			return iter;
		}
		iter++;
	}
	throw "no Method found!";
	return iter;
}

void Sequence::setCurrentCallBack(std::list<eeros::sequencer::Sequence::method>::iterator iter){
	currentCallBackIterator = iter;
}

void Sequence::run(){
	try{
		if(callBacks.empty()){
			fillCallBacks();
		}
		//std::list<Sequence::method>::iterator iter = callBacks.begin();
		if(currentCallBackIterator == callBacks.end()){
			currentCallBackIterator = callBacks.begin();
			state = kSequenceRunning;

		}
		method fun;
		while(currentCallBackIterator != callBacks.end()){
			fun = *currentCallBackIterator;
			(this->*fun)();
			currentCallBackIterator++;
		}
		state = kSequenceFinished;
	}catch(SequenceException* e){
		//ErrorHandling 
		try{
			if(e->errorHandler){
				if(e->returnToBegin){
					currentCallBackIterator = callBacks.begin();
				}else if (e->goToNext){
					findCallBack(e->nextMethod, true);
				}
				//else continue after Error Handling with the same Method
				e->errorHandler->run();
			}
			delete e;
		}catch(...){
			callerThread.stop();
		}
	}
}

Sequence* Sequence::getSequence(std::string name){
	std::list<Sequence*>::iterator iter = eeros::sequencer::Sequence::allSequences.begin();
		while(iter != eeros::sequencer::Sequence::allSequences.end()){
			if((*iter)->getName().compare(name) == 0){
				return *iter;
			}
			iter++;
		}
		return 0;
}

void Sequence::removeSequence(std::string name){
	std::list<Sequence*>::iterator iter = eeros::sequencer::Sequence::allSequences.begin();
	while(iter != eeros::sequencer::Sequence::allSequences.end()){
		if((*iter)->getName().compare(name) == 0){
			iter = allSequences.erase(iter);
		}else{
		  iter++;
		}
	}
}

int Sequence::getState(){
	return state;
}