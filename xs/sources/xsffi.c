#include "xsAll.h"
#include "xsffi.h"

extern txInteger fxArgc(txMachine*);

txSlot* fxArgv(txMachine* the, txInteger index) {
	return the->frame - 1 - fxCheckArg(the, index);
}

void fxPop(txMachine* the)
{
	mxPop();
}

void fxPush(txMachine* the, txSlot* slot)
{
	mxPushSlot(slot);
}

txSlot* fxResult(txMachine* the)
{
	return mxResult;
}

txSlot* fxThis(txMachine* the)
{
	return mxThis;
}

void** fxToArrayBufferHandle(txMachine* the, txSlot* slot, size_t size)
{
	fxToArrayBuffer(the, slot);
	txSlot* arrayBuffer = slot->value.reference->next;
	if (size) {
		txSlot* bufferInfo = arrayBuffer->next;
		if (arrayBuffer->value.arrayBuffer.address == C_NULL)
			mxTypeError("detached buffer");
		if (bufferInfo->value.bufferInfo.length < (txSize)size)
			mxRangeError("invalid buffer size %ld", bufferInfo->value.bufferInfo.length);
	}
	return (void**)&(arrayBuffer->value.arrayBuffer.address);
}

char** fxToStringHandle(txMachine* the, txSlot* slot)
{
	fxToString(the, slot);
	return &(slot->value.string);
}

txInteger fxGetStringLength(txMachine* the, txSlot* slot)
{
	fxToString(the, slot);
	return (txInteger)c_strlen(slot->value.string);
}

void fxStringCopy(txMachine* the, txSlot* slot, char* dst, txInteger cap)
{
	fxToString(the, slot);
	if (cap <= 0) return;
	txInteger len = (txInteger)c_strlen(slot->value.string);
	if (len >= cap) len = cap - 1;
	c_memcpy(dst, slot->value.string, len);
	dst[len] = 0;
}

void fxArrayBufferCopy(txMachine* the, txSlot* slot, void* dst, txInteger cap)
{
	fxToArrayBuffer(the, slot);
	txSlot* arrayBuffer = slot->value.reference->next;
	txSlot* bufferInfo = arrayBuffer->next;
	if (arrayBuffer->value.arrayBuffer.address == C_NULL)
		mxTypeError("detached buffer");
	txInteger len = (txInteger)bufferInfo->value.bufferInfo.length;
	if (cap < len) len = cap;
	c_memcpy(dst, arrayBuffer->value.arrayBuffer.address, len);
}

extern txAPI gxAPI;
txAPI gxAPI = {
	fxThis,
	fxArgc,
	fxArgv,
	fxPop,
	fxPush,
	fxResult,

	fxAbort,
	fxDefineID,
	fxID,
	fxNewHostFunction,

	fxFromBigInt64,
	fxFromBigUint64,
	fxInteger,
	fxNumber,
	fxUnsigned,

	fxToBigInt64,
	fxToBigUint64,
	fxToInteger,
	fxToNumber,
	fxToUnsigned,

	fxString,
	fxStringX,
	fxToStringHandle,

	fxArrayBuffer,
	fxToArrayBufferHandle,

	fxGetStringLength,
	fxStringCopy,
	fxGetArrayBufferLength,
	fxArrayBufferCopy,
};