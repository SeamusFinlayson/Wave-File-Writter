#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
//#include <stdlib.h>

#define PI 3.14159265359

/*struct wavHeader {

	//riff chunk
	char chunkID[4];

	//fmt subchunk

	//data sub chunk
};*/

//write two byte data to a file in little endian (LE) byte order
void writeTwoBytesLE(unsigned short twoByteData, FILE *file);

//write four byte data to a file in little endian (LE) byte order
void writeFourBytesLE(unsigned int fourByteData, FILE *file);

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

	//riff chunk
		//chunk id
		fprintf(waveFile, "RIFF");

		//chunk size
		//char chuckSize[] = { 0x24, 0x4c, 0x1d, 0x00 };
		//fwrite(chuckSize, sizeof(char), sizeof(chuckSize) / sizeof(char), waveFile);
		writeFourBytesLE(0x244c1d00, waveFile);

		//format
		fprintf(waveFile, "WAVE");

	//fmt sub chunk
		//subchunk 1 ID
		fprintf(waveFile, "fmt ");

		//sub chunk 1 size
		//char subchunk1Size[] = { 0x10, 0x00, 0x00, 0x00 };
		//fwrite(subchunk1Size, sizeof(char), sizeof(subchunk1Size) / sizeof(char), waveFile);
		writeFourBytesLE(0x10000000, waveFile);

		//audio format
		//unsigned short audioFormat = 0x0100;
		//audioFormat = ((audioFormat << 8) & 0xff00) | ((audioFormat >> 8) & 0x00ff);
		//char audioFormat[] = { 0x01, 0x00 };
		//fwrite(&audioFormat, sizeof(audioFormat), 1, wavePtr);
		writeTwoBytesLE(0x0100, waveFile);

		//num channels
		/*char numChannels [] = { 0x01, 0x00 };
		fwrite(numChannels, sizeof(char), sizeof(numChannels) / sizeof(char), waveFile);*/
		writeTwoBytesLE(0x0100, waveFile);

		//sample rate
		/*char sampleRate[] = { 0x80, 0x3e, 0x00, 0x00 };
		fwrite(sampleRate, sizeof(char), sizeof(sampleRate) / sizeof(char), waveFile);*/
		writeFourBytesLE(0x803e0000, waveFile);

		//byte rate
		/*char byteRate[] = { 0x00, 0x7d, 0x00, 0x00 };
		fwrite(byteRate, sizeof(char), sizeof(byteRate) / sizeof(char), waveFile);*/
		writeFourBytesLE(0x007d0000, waveFile);

		//block align
		/*char blockAlign[] = { 0x02, 0x00 };
		fwrite(blockAlign, sizeof(char), sizeof(blockAlign) / sizeof(char), waveFile);*/
		writeTwoBytesLE(0x0200, waveFile);

		//bits per sample
		/*char bitsPerSample[] = { 0x10, 0x00 };
		fwrite(bitsPerSample, sizeof(char), sizeof(bitsPerSample) / sizeof(char), waveFile);*/
		writeTwoBytesLE(0x1000, waveFile);

	//datasub chunk
		//subchunk2id
		fprintf(waveFile, "data");

		//subchunk 2 size
		/*char subchunk2Size[] = { 0x00, 0x4c, 0x1d, 0x00 };
		fwrite(subchunk2Size, sizeof(char), sizeof(subchunk2Size) / sizeof(char), waveFile);*/
		writeFourBytesLE(0x004c1d00, waveFile);

		//write 60s of data to file
		short sample;
		short AMPLITUDE = -0X7F00;
		short OFFSET = 0;
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

void writeTwoBytesLE(unsigned short twoByteData, FILE *file) {

	//byte swap data
	twoByteData = 
		((twoByteData << 8) & 0xff00) | 
		((twoByteData >> 8) & 0x00ff);

	//write to file
	fwrite(&twoByteData , sizeof(twoByteData ), 1, file);

}

void writeFourBytesLE(unsigned int fourByteData, FILE *file) {

	//byte swap data
	fourByteData = 
		((fourByteData << 3 * 8) & 0xff000000) | 
		((fourByteData << 8) & 0x00ff0000) | 
		((fourByteData >> 8) & 0x0000ff00) | 
		((fourByteData >> 3 * 8) & 0x000000ff);

	//write to file
	fwrite(&fourByteData, sizeof(fourByteData), 1, file);
}