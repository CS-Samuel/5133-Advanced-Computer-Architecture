//
//  main.cpp
//  Cache
//
//  Created by samuel on 11/1/17.
//  Copyright © 2017 Samuel. All rights reserved.
//
//***************************************************************************
// Cache is implemented as 4 Kbytes (2^12) with line/block size of 2^9 bytes
// 2^12 / 2^9 = 2^3 = 8 cache lines
//
// Address size = 32 bits (stored as characters)

#include <iostream>
#include <string>
#include <vector>
#include <fstream>


#define NUM_CACHE_LINES 8
#define ADDRESS_SIZE_CHAR 8


using namespace std;

bool checkCache (vector <string> cache, string address);
string modifyAddress (string address);

int main() {
    
    int code, nextCacheLine, nextInstructionCacheLine, nextDataCacheLine;
    nextCacheLine = nextInstructionCacheLine = nextDataCacheLine = (NUM_CACHE_LINES);

    float singleCacheHits =0 , singleCacheMisses= 0, instructionCacheHits = 0, instructionCacheMisses=0, dataCacheHits=0, dataCacheMisses=0,singleCacheHitRatio, instructionCacheHitRatio, dataCacheHitRatio, totalTwoCacheHitRatio;
    
    string newAddress;
    
    bool hitFlag;
    
    vector <string> singleCache(NUM_CACHE_LINES), dataCache(NUM_CACHE_LINES), instructionCache(NUM_CACHE_LINES);
    
    ifstream inputFile;

    
// Single Cache
//***************************************************************************

    inputFile.open("cachedata.txt");
    

// New Input
    
    while (inputFile >> code)
    {
        inputFile >> newAddress;
        
        hitFlag = false;

// Modify New Address
    
        newAddress = modifyAddress(newAddress);
        

// Check Cache for address
    
        hitFlag = checkCache(singleCache, newAddress);

// Increment if hit or load address into Cache if miss
        
        if (hitFlag)
            ++ singleCacheHits;
        else
        {
            ++singleCacheMisses;
            singleCache[nextCacheLine % 8] = newAddress;
            ++ nextCacheLine;
        }
   
    }
    
    singleCacheHitRatio = singleCacheHits / (singleCacheHits+singleCacheMisses);
    
// Two Caches (Instruction and Data Caches)
//***************************************************************************

    inputFile.close();
    inputFile.open("cachedata.txt");
    
// New Input
    
    while (inputFile >> code)
    {
        inputFile >> newAddress;
        
        hitFlag = false;
        
// Modify New Address
        
        newAddress = modifyAddress(newAddress);
        
// Instruction Cache
        
        if (code == 2)
        {
            hitFlag = checkCache(instructionCache, newAddress);
            
            if (hitFlag)
                ++ instructionCacheHits;
            else
            {
                ++instructionCacheMisses;
                instructionCache[nextInstructionCacheLine % 8] = newAddress;
                ++ nextInstructionCacheLine;
            }
        }

// Data Cache
        
        else
        {
            hitFlag = checkCache(dataCache, newAddress);
            if (hitFlag)
                ++ dataCacheHits;
            else
            {
                ++dataCacheMisses;
                dataCache[nextDataCacheLine % 8] = newAddress;
                ++ nextDataCacheLine;
            }
        }
    }
    
    inputFile.close();
    
    dataCacheHitRatio = dataCacheHits / (dataCacheHits + dataCacheMisses);
    instructionCacheHitRatio = instructionCacheHits / (instructionCacheHits + instructionCacheMisses);
    totalTwoCacheHitRatio = (dataCacheHits + instructionCacheHits) / (dataCacheHits + dataCacheMisses + instructionCacheHits + instructionCacheMisses);
    
    cout << "Single Cache Hit Ratio: " << singleCacheHitRatio << endl;
    cout << "Two Cache Hit Ratio: " << totalTwoCacheHitRatio << endl << endl;
    cout << "Instruction Cache Hit Ratio: " << instructionCacheHitRatio << endl;
    cout << "Data Cache Hit Ratio: " << dataCacheHitRatio << endl << endl;
    
    return 0;
}

bool checkCache (vector <string> cache, string address)
{
    for (int i=0; i < NUM_CACHE_LINES; i++)
        if (address.compare(cache[i]) == 0)
            return true;
        
    return false;
}

string modifyAddress(string address)
{
    string modifiedAddress, nullAddress = "00000000";
    
    if (address.size() == ADDRESS_SIZE_CHAR)
        modifiedAddress = address;
    else
    {
        modifiedAddress = nullAddress;
        for (int i = 0; i < address.size(); i++)
            modifiedAddress[i + ADDRESS_SIZE_CHAR - address.size()] = address[i];
    }
    
    return modifiedAddress;
    
}
