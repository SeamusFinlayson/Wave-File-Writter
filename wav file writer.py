# File Name: wav file writer
# Author: Seamus Finlayson
# Date: 2023-04-25

from os import stat

class waveFileClass:

    # variable parameters
    SAMPLE_RATE = int()             # samples/second
    AUDIO_DURATION = int()          # seconds -- must be less than 34 hours to prevent overflow

    # fixed audio parameters
    SUBCHUNK1_SIZE = 16             # size format subchunk
    BITS_PER_SAMPLE = 16            # bits -- can be changed but data types must be changed
    AUDIO_FORMAT = 1                # 1 for Pulse-code modulation (PCM)
    NUM_CHANNELS = 1                # 1 channel for mono audio

    # calculated parameters
    def __init__(self, sampleRate, rawDataFileName, outputFileName):

        self.SAMPLE_RATE = sampleRate
        self.AUDIO_DURATION = self.getInputFileLength(rawDataFileName)
        self.RAW_DATA_FILE = rawDataFileName
        self.FILE_NAME = outputFileName
        self.CHUNK_SIZE = int((self.BITS_PER_SAMPLE * self.SAMPLE_RATE * self.AUDIO_DURATION)/8 + 36) # size of file after this header = data + headers
        self.BYTE_RATE = int(self.SAMPLE_RATE * self.NUM_CHANNELS * self.BITS_PER_SAMPLE / 8)
        self.BLOCK_ALIGN = int(self.NUM_CHANNELS * self.BITS_PER_SAMPLE / 8)
        self.SUBCHUNK2_SIZE = int(self.BITS_PER_SAMPLE * self.SAMPLE_RATE * self.AUDIO_DURATION / 8)

    def createFile(self):

        #####################################
        # riff chunk
        #####################################

        # chunk id
        with open(self.FILE_NAME, "w") as f:
            f.write("RIFF")

        # chunk size
        self.writeIntToFile(self.CHUNK_SIZE, 4)
        # TODO get rid of this
        # with open(self.FILE_NAME, "ab") as f:
        #     b = bytearray(self.CHUNK_SIZE.to_bytes(4, "little", signed=True))
        #     print(hex(b[0]), hex(b[1]), hex(b[2]), hex(b[3]))
        #     f.write(b)

        # format
        with open(self.FILE_NAME, "a") as f:
            f.write("WAVE")

        #####################################
        # fmt subchunk
        #####################################

        # subchunk 1 id
        with open(self.FILE_NAME, "a") as f:
            f.write("fmt ")

        # subchunk 1 size
        self.writeIntToFile(self.SUBCHUNK1_SIZE, 4)

        # audio format
        self.writeIntToFile(self.AUDIO_FORMAT, 2)

        # num channels
        self.writeIntToFile(self.NUM_CHANNELS, 2)

        # sample rate 
        self.writeIntToFile(self.SAMPLE_RATE, 4)

        # byte rate
        self.writeIntToFile(self.BYTE_RATE, 4)

        # block align
        self.writeIntToFile(self.BLOCK_ALIGN, 2)

        # bits per sample
        self.writeIntToFile(self.BITS_PER_SAMPLE, 2)

        #####################################
        # data sub chunk
        #####################################

        # subchunk 2 id
        with open(self.FILE_NAME, "a") as f:
            f.write("data")

        # subchunk 2 size
        self.writeIntToFile(self.SUBCHUNK2_SIZE, 4)

        # add raw data to file with header
        with open(self.FILE_NAME, "ab") as header, open(self.RAW_DATA_FILE, "rb") as rawData:
            header.write(rawData.read())

        print("File Created")

    def writeIntToFile(self, value, numBytes):
        value = int(value)
        asBytes = bytearray(value.to_bytes(numBytes, "little", signed=True))
        with open(self.FILE_NAME, "ab") as f:
            f.write(asBytes)

    def getInputFileLength(self, dataFileName):
        fileStats = stat(dataFileName)
        self.AUDIO_DURATION = fileStats.st_size / self.SAMPLE_RATE / (self.BITS_PER_SAMPLE / 8)
        print("Length in time is: ", self.AUDIO_DURATION)
        return self.AUDIO_DURATION

# make file
waveFileClass(sampleRate=16000, rawDataFileName="WhaleData.dat", outputFileName="WhaleSounds.wav").createFile()
