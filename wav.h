#include<iostream>
#include<vector>
#include<string>
#include<fstream>

using std::fstream;

//make a new struct to read the headr with the following format http://soundfile.sapp.org/doc/WaveFormat/

class Header
{

  public:
  //RIF
        uint8_t ChunkID[4];
        uint32_t ChunkSIZE;
        uint8_t ChunkFormat[4];
  //FMT
        uint8_t   Subchunk1ID[4]; //only want the first 4 bytes
        uint32_t  Subchunk1SIZE; 
        uint16_t  AudioFormat;
        uint16_t  NumOfChans;
        uint32_t  SampleRate;
        uint32_t  BytesRate;
        uint16_t  BlockAlign;
        uint16_t  BitsPerSample;
  //DATA
        uint8_t  dataID[4];
        uint32_t dataSize;

};

class WavReader
{
  public:
     int16_t* data;
     

     int sampleCount;
     
    //Reads the Heard and returns takes a sample from the data; 
     void Reader(Header head , char* path);

};


void WavReader::Reader(Header head, char* path)
{
    //initilize the headr and size

    std::cout<< "reading file " << path << std::endl;
    int head_len= sizeof(Header);

    

    // reading the file 
    FILE* file = fopen(path,"r");

    //read the headr with the head class 
    size_t read_head = fread(&head, 1, head_len, file);


   //  std::cout << "Header Read " << read_head << " bytes." << std::endl;

    if (read_head > 0)
    {
        /* READ HEADER */ 

        //we want to read ber byte so we divide by 8
        float bytesPerSample = head.BitsPerSample/8;
        //Total number of samples in the file
        //
        int count = 0;  
        int sampleCount = head.dataSize*8 / head.BitsPerSample; 
        int16_t* buffer = new int16_t[2*sampleCount];
        std::cout<< "bits per sample" << head.BitsPerSample << std::endl; 

        this->data = new int16_t[2*sampleCount];

        while ((read_head = fread(this->data, head.BitsPerSample/8 ,sampleCount , file)) > 0)
        {
            //keep track for deep copy 
           //  std::cout << "ay kheeeeeeeeda " << std::endl;
            count++;

                
        }

        //set our sample count
        this->sampleCount = sampleCount;

        std::cout << "Sample count" << sampleCount<< std::endl;
        std:: cout << "Data size:" << head.ChunkSIZE << std::endl;
        std:: cout << "Sampling Rate:" << head.SampleRate << std::endl;
    }

    fclose(file);
}
