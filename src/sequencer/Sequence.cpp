#include <eeros/sequencer/Sequence.hpp>
#include <eeros/sequencer/SequenceException.hpp>

eeros::sequencer::Sequence::Sequence(std::string name, TimeDomain* ptimeDomain):
Executor(0),
sequenceName(name),
timeDomain(ptimeDomain){
}

void eeros::sequencer::Sequence::addSubSequence(Sequence* seq){
	subSequences.push_back(seq);
}

void eeros::sequencer::Sequence::deleteAllSubSequences(){
	std::list<Sequence*>::iterator iter = subSequences.begin();
	Sequence* sequence = 0;
	while(iter != subSequences.end()){
		sequence = *iter;
		delete sequence;
		iter++;
	}
	subSequences.clear();
}

void eeros::sequencer::Sequence::run(){
	try{
		run_state();
	}catch(SequenceException* e){
		next(e->nextMethod);
	}
}

void eeros::sequencer::Sequence::next(method step){
	this->fun = step;
}
void eeros::sequencer::Sequence::run_state(){
	(this->*fun)();
}

std::string eeros::sequencer::Sequence::getName(){
	return sequenceName;
}

eeros::sequencer::Sequence* eeros::sequencer::Sequence::findSequence(std::string name){
	std::list<Sequence*>::iterator iter = subSequences.begin();
		while(iter != subSequences.end()){
			if((*iter)->getName().compare(name) == 0){
				return *iter;
			}
			iter++;
		}
		return 0;
}

void eeros::sequencer::Sequence::deleteSequence(std::string name){
	std::list<Sequence*>::iterator iter = subSequences.begin();
		while(iter != subSequences.end()){
			if((*iter)->getName().compare(name) == 0){
				subSequences.erase(iter);
				return;
			}
			iter++;
		}
}