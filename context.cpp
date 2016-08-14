/*  
    Copyright (C) 2015-2016  ANSSI
    Copyright (C) 2015  Thomas Cordier

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "context.hpp"

#include <assert.h>

Context Context::instance = Context();

Context & Context::getInstance()
{
	return instance;
}

Context::Context()
{
	jumpTable = new int[JUMP_TABLE_SIZE];
	callTable = new int[CALL_TABLE_SIZE];
	jumpCounter = 0;
	callCounter = 0;
	flowEnd = 0;
}

Context::~Context()
{
	delete[]this->jumpTable;
	delete[]this->callTable;
}

void Context::addJump(uint32 adress)
{
//	assert(jumpCounter > JUMP_TABLE_SIZE - 1);
	jumpTable[jumpCounter] = adress;
	jumpCounter++;
}

void Context::addCall(uint32 adress)
{
//	assert(callCounter > CALL_TABLE_SIZE - 1);
	callTable[callCounter] = adress;
	callCounter++;
}

void Context::endPacket()
{
	while (callCounter > 0) {
		callCounter--;
		ua_add_cref(0, callTable[callCounter], fl_CF);
	}
	while (jumpCounter > 0) {
		jumpCounter--;
		ua_add_cref(0, jumpTable[jumpCounter], fl_JF);
	}
}

int Context::getAndResetFlowEnd()
{
	int temp = flowEnd;
	flowEnd = 0;
	return temp;
}

void Context::stopFlow()
{
	flowEnd = 1;
}
