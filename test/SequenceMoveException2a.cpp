/*
    The contents of this file are subject to the Mozilla Public License
    Version 1.1 (the "License"); you may not use this file except in
    compliance with the License. You may obtain a copy of the License at
    http://www.mozilla.org/MPL/

    Software distributed under the License is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
    License for the specific language governing rights and limitations
    under the License.

    The Original Code is ______________________________________.

    The Initial Developer of the Original Code is ________________________.
    Portions created by ______________________ are Copyright (C) ______
    _______________________. All Rights Reserved.

    Contributor(s): ______________________________________.

    Alternatively, the contents of this file may be used under the terms
    of the _____ license (the  "[___] License"), in which case the
    provisions of [______] License are applicable instead of those
    above.  If you wish to allow use of your version of this file only
    under the terms of the [____] License and not to allow others to use
    your version of this file under the MPL, indicate your decision by
    deleting  the provisions above and replace  them with the notice and
    other provisions required by the [___] License.  If you do not delete
    the provisions above, a recipient may use your version of this file
    under either the MPL or the [___] License."
*/


#include "SequenceMoveException2a.hpp"
#include "ErrorHandlerA.hpp"

#include <eeros/sequencer/SequenceException.hpp>

SequenceMoveException2a::SequenceMoveException2a(std::string name, eeros::sequencer::Sequencer& caller)
	: SequenceMoveException1_3(name, caller) {
}
	
SequenceMoveException2a::~SequenceMoveException2a(){
}

void SequenceMoveException2a::moveException(){
	calledMethode.append("MoveException ");
	
	//Please take attention the exception should only be thrown once, else the sequencer thread will never end.
	//The sequence SequenceMoveException1_3 is restarted at next run() call of the base sequence, called by the executor
	static int i = 0;
	if(i == 0) {
		i++;
		ErrorHandlerA* errorHandlerA = dynamic_cast<ErrorHandlerA*>(eeros::sequencer::ErrorHandler::getErrorHandler("ErrorHandlerA"));
		if(!errorHandlerA){
			errorHandlerA = new ErrorHandlerA("ErrorHandlerA", this, false);
		}
		//case 2a (except the call in ErrorHandlerA to ErrorHandlerB it is also Casse 4a)
		//After this exception the sequence is continues in this method
		throw new eeros::sequencer::SequenceException(this, static_cast<eeros::sequencer::Sequence::method>(&SequenceMoveException2a::moveException), 0,
			                          errorHandlerA, false, false, "Exception ErrorHandlerA");
	}
}

