#pragma warning(disable : 4996)

#include <iostream>
#include <stdio.h>
#include <conio.h>

#include "teVirtualMidi.h"

#define MAX_SYSEX_BUFFER 65535

using namespace std;

char* binToStr(const unsigned char* data, DWORD length) {
	static char dumpBuffer[MAX_SYSEX_BUFFER * 3];
	DWORD index = 0;

	while (length--) {
		sprintf(dumpBuffer + index, "%02x", *data);
		if (length) {
			strcat(dumpBuffer, ":");
		}
		index += 3;
		data++;
	}
	return dumpBuffer;
}

void CALLBACK teVMCallback(LPVM_MIDI_PORT midiPort, LPBYTE midiDataBytes, DWORD length, DWORD_PTR dwCallbackInstance) {
	if ((NULL == midiDataBytes) || (0 == length)) {
		printf("empty command - driver was probably shut down!\n");
		return;
	}
	if (!virtualMIDISendData(midiPort, midiDataBytes, length)) {
		printf("error sending data: %d\n" + GetLastError());
		return;
	}
	printf("command: %s\n", binToStr(midiDataBytes, length));
}

int main()
{
	LPVM_MIDI_PORT port;

	port = virtualMIDICreatePortEx2(L"C loopback", teVMCallback, 0, MAX_SYSEX_BUFFER, TE_VM_FLAGS_PARSE_RX);
	if (!port) {
		printf("could not create port: %d\n", GetLastError());
		return 0;
	}

	printf("Virtual port created - press enter to close port again\n");
	_getch();

	virtualMIDIClosePort(port);
	printf("Virtual port closed - press enter to terminate application\n");
	_getch();

	return 0;
}
