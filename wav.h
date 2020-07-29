#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <math.h>
#include <queue>
#include <vector>
#include <unordered_map>
#include <map>
#include "node.h"

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
     /* --------- data and reader --------*/

     // should probably make setters and getters for this.
     int16_t* data;

     int sampleCount;

     //Reads the Heard and returns takes a sample from the data; 
     void Reader(Header head , char* path);

     /*--------- Huffman coding ---------- */
    
     //Compresser using HuffmanTree
     float generate_huff(std::unordered_map<char,int> first_prob , std::unordered_map<char,float> map);
     
     //Generate the Code
     void generate_huff_code(Node* root, int bits[], int c, std::unordered_map<char,std::string>* bit_len);

     //construct single probabilies
     std::unordered_map<char,float> construct_single_prob(std::unordered_map<char,int> map ,uint32_t size);

     //Generate the maps with frequencies
     std::unordered_map<char,int>  construct_map(int16_t* data, uint32_t size);
  
     //print the map helper
     template<class T>
     void print_map(std::unordered_map<char,T> map);

     template<class T>
     void print_map(std::map<std::string,T> map);

     /*--------- LZW ---------- */
     int LZW(int16_t* data, uint32_t size);
};
  
template<class T>
void WavReader::print_map(std::unordered_map<char,T> map)
{
  for(auto p : map)
  {
    std::cout << p.first << " : " << p.second << std::endl; 
  }

}

template<class T>
void WavReader::print_map(std::map<std::string,T> map)
{
  std::cout<< "string                                 code" << std::endl; 
  std::cout<<"---------------------------------------------" <<  std::endl;

  for(auto p : map)
  {
    std::cout << p.first << "                             " << p.second << std::endl; 
  }

}

std::unordered_map<char,float> WavReader::construct_single_prob(std::unordered_map<char,int> map ,uint32_t size)
{ 

  std::unordered_map<char,float> single_prob;

  for(auto p : map)
  {
    single_prob.insert({ p.first, (float) p.second/size });
  } 

  print_map(single_prob); 
  return single_prob;
}

std::unordered_map<char,int> WavReader::construct_map(int16_t* data, uint32_t size){

  //inital map with the frequency of each char as its value
  std::unordered_map<char,int> map;

  for(int i = 0; i < size; i++)
  {
    //insert the chars into map with the number of times they occur as map.second
    map[data[i]] += 1;
  }
  print_map(map);
  return map;
}


void WavReader::generate_huff_code(Node* root, int bits[], int c, std::unordered_map<char,std::string>* bit_len)
{

  if(root->get_left())
  {
    bits[c] = 0;
    //std::cout<<"left see: " << root->get_left()->get_ch() << std::endl;
    
    //recurse more we have a left
    generate_huff_code(root->get_left(), bits ,c+1, bit_len );
    
  }

  if(root->get_right())
  {
    bits[c] = 1;
    //std::cout<<"right see: " << root->get_right()->get_ch() << " : " << root->get_bit() << std::endl;
    
    //recurese right
    generate_huff_code(root->get_right(), bits , c+1, bit_len);
    
  }

  //BASE CASE
  else if(!root->get_right() && !root->get_left()) 
  {
     std::cout<< root->get_ch() << " : ";
     
     std::string s = "";

     for(int i = 0 ; i < c; i++){
        std::cout<<bits[i];
        s += std::to_string(bits[i]);
    }

     std::cout << std::endl;
     bit_len->insert({ root->get_ch(),s});
  }
}

float WavReader::generate_huff(std::unordered_map<char,int> first_prob, std::unordered_map<char,float> map) {

     //make a min Heap with the frequency of each letter as the priority
     std::priority_queue <Node, std::vector<Node>, Node_compare> pq;
  
    // 1. make first_prob.size() nodes (as many as unique chars in our sentence) insert into the min heap
    // While size is not 1 (we have a single root) 
       // 2. extract the top 2 with lowest frequencies
       // 3. create a new Node and add the frequencies together and insert back in the Heap
       // 4. repeat 2 and 3
    // 5. once the tree is constructed, then do a traverse to assign the bits to each node
   
    //populate the heap
    for (auto p : first_prob)
    {
       Node leaf = Node(p.second, p.first);
       pq.push(leaf); 
    }
  
    // print_queue(pq);
   
    //keep track of the roots
    int count = 0;
    //std::cout << "the heap size init is : " << pq.size()  << std::endl;
    
    while(pq.size() > 1)
    {
      //pop the top 2 lowest freq
      Node l1 = Node();
      l1 =  pq.top();
      // std::cout << "TOP 1 : " << l1.get_ch() << " : " << l1.get_bit() << std::endl;
      pq.pop();
      
      Node l2 = Node();
      l2 = pq.top();
      //std::cout << "TOP 2 : " << l2.get_ch() << " : " << l2.get_bit() << std::endl;
      pq.pop();
  
      int new_freq = l1.get_freq() + l2.get_freq();
     
      //make a new internal node
      Node root = Node(new_freq ,'r');
      
      //assign left
      Node* l1n = new Node(l1.get_freq() , l1.get_ch());
      l1n->set_left(l1.get_left());
      l1n->set_right(l1.get_right());
  
      //assign right
      Node* l2n = new Node(l2.get_freq() , l2.get_ch());
      l2n->set_left(l2.get_left());
      l2n->set_right(l2.get_right());
      
      //insert to HEAP
      root.set_left(l1n);
      root.set_right(l2n);
      root.set_bit(++count);
  
      //std::cout << "setting left as : " << root.get_left()->get_ch() << " bit : " << root.get_left()->get_bit() << std::endl; 
      //std::cout << "setting right as : " << root.get_right()->get_ch() << " bit : " << root.get_right()->get_bit() << std::endl; 
  
      pq.push(root);
      // std::cout << "the heap size is : " << pq.size()  << std::endl;
    }
    
    //bit count arr
    int v[32] = {'\0'} ;
    //map to store the bits in
    std::unordered_map<char,std::string> bit_len;
    std::unordered_map<char,int> avg_lens;
   
    
    //print_queue(pq);
   
    //make a new root* to pass to our huffcode 
    Node* root = new Node();
    root->set_left(pq.top().get_left());
    root->set_right(pq.top().get_right());
  
    //GET THE HUFFING CODE 
    generate_huff_code(root ,v,0, &bit_len);
    float avg_bit = 0.0;
  
    
    std::cout << " BIT LENGTHS " << std::endl;
    for(auto p : bit_len)
    {
        avg_lens[p.first] = (float)p.second.size();
    }
    avg_bit =0.0;
    std::cout << std::endl;
    print_map(avg_lens);
  
    for(auto p : avg_lens)
    {
      //we can actually do this in 1 for loop since our original map has the same keys as our bitlength map
       avg_bit += first_prob[p.first] * p.second;
    }
  
    return avg_bit;
}
int WavReader::LZW(int16_t* data,uint32_t size)
{

  //we should use a map since its sorted
  std::map<std::string,int> dict;
  int code_count = 0;

  std::string first ="";
  first += data[0];

  int i = 1;

//  for(int j = 0; j < size; j++)
//  {
//    char c = data[i];
//    std::string new_s ="";
//    new_s += c;
//    //i cant find you so imma add a cuont
//    if(dict.find(new_s) == dict.end())
//    {
//       dict[new_s] = code_count;
//    }
//  }
//  for(auto p : dict)
//  {
//       dict[p.first] += code_count;
//       code_count ++;
//  }
//
  //we need to this again cause I want their codes to be added in order 

 // std::cout << " OUTPUT : " << std::endl;
  int counter = 1;
  while(i < size)
  {
    std::string c = "";
    c += data[i];
    if(dict.find(first+c) != dict.end())
    {
      first = first+c;
    }

    else
    {
 //     std::cout << " " << dict[first];
      std::string  next = first + c;
      dict[next] = code_count; 
      counter++;
      code_count ++;
      first = c;
    }
    i++;
  }
 // std::cout << " " <<  dict[first] << std::endl;
  return counter * sizeof(int);

}
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
    std::cout <<std::endl<< "SINGLE  MAP" << std::endl;
    std::unordered_map<char,int> map= construct_map(this->data,head.dataSize);

    std::cout <<std::endl<< "SINGLE PROBABILITIES" << std::endl;
    std::unordered_map<char,float> single_prob = construct_single_prob(map, head.dataSize);

   
    float f_bitlen = generate_huff(map, single_prob);
    std::cout << "Huffman Compression Ratio: " <<head.dataSize/f_bitlen<< std::endl;

    int code_count = LZW(this->data , head.dataSize);
    std::cout << "LZW COMPRESSION: " << (float)head.dataSize/code_count<<std::endl;
    std::cout << " SIZE " << head.dataSize << "  =  " << sampleCount*2 << std::endl;

}
