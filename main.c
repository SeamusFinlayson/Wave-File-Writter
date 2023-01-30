//File Name: main.c
//Author: Seamus Finlayson
//Date: 2023-01-30

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
//#include <stdlib.h>

#define PI 3.14159265359

///< write two byte data to a file in little endian (LE) byte order
unsigned short writeTwoBytesLE(unsigned short twoByteData, FILE *file);

///< write four byte data to a file in little endian (LE) byte order
unsigned int writeFourBytesLE(unsigned int fourByteData, FILE *file);

int main(void) {

	//indicate program is running
	printf("Running...\n\n");

	//open file for writing
	FILE *waveFile;
	waveFile = fopen("Music of the Gods.wav", "wb");

	//check for failure to open file
	if (waveFile == NULL) {

		//report error in terminal
		printf("Error file not oppened\n");
	}
	//write header of wave file
	else {

		/////////////////////////////////////////////////////////////
		// riff chunk
		/////////////////////////////////////////////////////////////
		
		//chunk id
		fprintf(waveFile, "RIFF");

		//chunk size
		writeFourBytesLE(0x244c1d00, waveFile);

		//sample code for alternate byte writing method
		//char chuckSize[] = { 0x24, 0x4c, 0x1d, 0x00 };
		//fwrite(chuckSize, sizeof(char), sizeof(chuckSize) / sizeof(char), waveFile);

		//format
		fprintf(waveFile, "WAVE");

		/////////////////////////////////////////////////////////////
		// fmt sub chunk
		/////////////////////////////////////////////////////////////
		
		//subchunk 1 ID
		fprintf(waveFile, "fmt ");

		//sub chunk 1 size
		writeFourBytesLE(0x10000000, waveFile);

		//audio format
		writeTwoBytesLE(0x0100, waveFile);

		//num channels
		writeTwoBytesLE(0x0100, waveFile);

		//sample rate
		writeFourBytesLE(0x803e0000, waveFile);

		//byte rate
		writeFourBytesLE(0x007d0000, waveFile);

		//block align
		writeTwoBytesLE(0x0200, waveFile);

		//bits per sample
		writeTwoBytesLE(0x1000, waveFile);

		/////////////////////////////////////////////////////////////
		// data sub chunk
		/////////////////////////////////////////////////////////////
		
		//subchunk2id
		fprintf(waveFile, "data");

		//subchunk 2 size
		writeFourBytesLE(0x004c1d00, waveFile);

		//write 60s of data to file
		volatile short sample;
		const short AMPLITUDE = -0X7F00;
		const short OFFSET = 0;
		for (int i = 0; i < 16000*60; i++) {
			
			//sine wave at 300Hz
			//sample = 4097; //for testing
			sample = (short)(AMPLITUDE * sin(2 * PI * 300 * i / 16000) + OFFSET );

			//write to file
			fwrite(&sample, sizeof(sample), 1, waveFile);
		}

		//close file
		fclose(waveFile);
	}

	//exit program
	return 0;
}

unsigned short writeTwoBytesLE(unsigned short twoByteData, FILE *file) {

	//byte swap data
	twoByteData = 
		((twoByteData << 8) & 0xff00) | 
		((twoByteData >> 8) & 0x00ff);

	//write to file
	fwrite(&twoByteData , sizeof(twoByteData), 1, file);

	//return little endian data
	return twoByteData;
}

unsigned int writeFourBytesLE(unsigned int fourByteData, FILE *file) {

	//byte swap data
	fourByteData = 
		((fourByteData << 3 * 8) & 0xff000000) | 
		((fourByteData << 8) & 0x00ff0000) | 
		((fourByteData >> 8) & 0x0000ff00) | 
		((fourByteData >> 3 * 8) & 0x000000ff);

	//write to file
	fwrite(&fourByteData, sizeof(fourByteData), 1, file);

	//return little endian data
	return fourByteData;
}