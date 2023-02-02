//File Name: main.c
//Author: Seamus Finlayson
//Date: 2023-01-30

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define PI 3.14159265359

//user changable audio parameters
#define SAMPLE_RATE 16000		//samples/second
#define AUDIO_DURATION 10		//seconds -- must be less than 34 hours to prevent overflow

//fixed audio parameters
#define SUBCHUNK1_SIZE 16		//size format subchunk
#define BITS_PER_SAMPLE 16		//bits -- can be changed but data types must be changed
#define AUDIO_FORMAT 1			//1 for Pulse-code modulation (PCM)
#define NUM_CHANNELS 1			//1 channel for mono audio

// Write two byte data to a file in little endian (LE) byte order.
unsigned short writeTwoBytesLE(unsigned short twoByteData, FILE* file);

// Write four byte data to a file in little endian (LE) byte order.
unsigned int writeFourBytesLE(unsigned int fourByteData, FILE* file);

int main(void) {

	//indicate program is running
	printf("Running...\n\n");

	//open file for writing
	FILE* waveFile;
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
		unsigned int chunkSize = (BITS_PER_SAMPLE * SAMPLE_RATE * AUDIO_DURATION) / 8 + 36; //size of file after this header = data + headers
		writeFourBytesLE(chunkSize, waveFile);

		//format
		fprintf(waveFile, "WAVE");

		/////////////////////////////////////////////////////////////
		// fmt sub chunk
		/////////////////////////////////////////////////////////////

		//subchunk 1 ID
		fprintf(waveFile, "fmt ");

		//sub chunk 1 size
		writeFourBytesLE(SUBCHUNK1_SIZE, waveFile);

		//audio format
		writeTwoBytesLE(AUDIO_FORMAT, waveFile);

		//num channels
		writeTwoBytesLE(NUM_CHANNELS, waveFile);

		//sample rate
		writeFourBytesLE(SAMPLE_RATE, waveFile);

		//byte rate
		unsigned int byteRate = SAMPLE_RATE * NUM_CHANNELS * BITS_PER_SAMPLE / 8;
		writeFourBytesLE(byteRate, waveFile);

		//block align
		unsigned short blockAlign = NUM_CHANNELS * BITS_PER_SAMPLE / 8;
		writeTwoBytesLE(blockAlign, waveFile);

		//bits per sample
		writeTwoBytesLE(BITS_PER_SAMPLE, waveFile);

		/////////////////////////////////////////////////////////////
		// data sub chunk
		/////////////////////////////////////////////////////////////

		//subchunk2id
		fprintf(waveFile, "data");

		//subchunk 2 size
		unsigned int subchunk2Size = BITS_PER_SAMPLE * SAMPLE_RATE * AUDIO_DURATION / 8;
		writeFourBytesLE(subchunk2Size, waveFile);

		//write 60s of data to file -- will need to offset and take 2's compliment of raw data
		volatile short sample;
		double AMPLITUDE = 0X2000;
		double frequencyDeviation = 300;
		const double ONE_HZ = 2 * PI / SAMPLE_RATE; //Digital frequency equivalent to 1 Hz -- units of cycles/sample
		int noiseFrequency = 0;
		double decay = 0;
		srand(time(NULL));
		for (int i = 1; i <= SAMPLE_RATE * AUDIO_DURATION; i++) {

			//sine wave at 300Hz - for sample size greater than 8 bytes, data is stored as signed short
			sample = 0;
			//sample += (short)(sin((double)i * ONE_HZ * 300));
			//decay = exp(-ONE_HZ * (i % ))
			sample += AMPLITUDE * sin(i * ONE_HZ * 262);
			sample += AMPLITUDE * sin(i * ONE_HZ * 330);
			sample += AMPLITUDE * sin(i * ONE_HZ * 392);

			//noise
			if (!(i % (SAMPLE_RATE / 10000))) {
				noiseFrequency = (rand() % 12000) - 4000;
			}
			//sample += (short)(AMPLITUDE / 20 * sin(ONE_HZ * noiseFrequency * i));

			//write to file
			writeTwoBytesLE(sample, waveFile);
		}

		//close file
		fclose(waveFile);
	}

	//exit program
	return 0;
}

unsigned short writeTwoBytesLE(unsigned short twoByteData, FILE* file) {

	//byte swap data - may need on microcontroller depending on system endianness
	//twoByteData =
	//	((twoByteData << 8) & 0xff00) | 
	//	((twoByteData >> 8) & 0x00ff);

	//write to file
	fwrite(&twoByteData, sizeof(twoByteData), 1, file);

	//return little endian data
	return twoByteData;
}

unsigned int writeFourBytesLE(unsigned int fourByteData, FILE* file) {

	//byte swap data - may need on microcontroller depending on system endianness
	//fourByteData = 
	//	((fourByteData << 3 * 8) & 0xff000000) | 
	//	((fourByteData << 8) & 0x00ff0000) | 
	//	((fourByteData >> 8) & 0x0000ff00) | 
	//	((fourByteData >> 3 * 8) & 0x000000ff);

	//write to file
	fwrite(&fourByteData, sizeof(fourByteData), 1, file);

	//return little endian data
	return fourByteData;
}