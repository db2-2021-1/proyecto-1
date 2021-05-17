#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <bitset>
#include "Register.h"


class ExtendedHash {
private:
    std::vector<long> index;
    std::map<std::string,Bucket*> hashIndex;

public:

    /** Construye 2^hashSize de índices y 2 buckets iniciales*/
    ExtendedHash(){
        hashIndex.insert(hashIndex.end(),std::pair<std::string,Bucket*>("0",new Bucket("0")));
        hashIndex.insert(hashIndex.end(),std::pair<std::string,Bucket*>("1",new Bucket("1")));
    }

    /** Toma un valor entero y devuelve los 4 primero bits binarios en string*/
    std::string hashFunction(int key){
        return std::bitset<hashSize>(key%(int)pow(2,hashSize)).to_string();
    }

    /**Falta implementar*/
    Register find(keyType key){

    }

    /**Falta implementar*/
    std::vector<Register> find(keyType beginKey, keyType endKey){

    }

    /**Falta implementar*/
    void insert(int reg){
        std::string key = hashFunction(reg);
        while(!key.empty()){
            auto it = this->hashIndex.find(key);
            if(it != this->hashIndex.end()){
                overflowVerificator(it->second,reg);
                break;
            }else{
                key.erase(0,1);
            }
        }

    }

    void overflowVerificator(Bucket *bucket, keyType key){
        int a = bucket->getKeyVector().size();
        int b = blockFactor;
        bool c = a==b;
        if(c){
            if(bucket->getDepth() == hashSize){
                auto overflowBucket = new Bucket(bucket->getValue());
                overflowBucket->setNextOverflowBucket(bucket->getNextOverflowBucket());
                bucket->setNextOverflowBucket(overflowBucket);
                overflowBucket->setKeyVector(bucket->getKeyVector());
                bucket->clearKeyVector();
                bucket->insert(key);
            }else{
                auto ofBucket0 = new Bucket("0" + bucket->getValue());
                auto ofBucket1 = new Bucket("1" + bucket->getValue());
                bucket->insert(key);
                std::string keyIt;
                for(auto it:bucket->getKeyVector()){
                    keyIt = hashFunction(it);
                    if(keyIt[bucket->getValue().size()-1] == '0'){
                        ofBucket0->insert(it);
                    }else{
                        ofBucket1->insert(it);
                    }
                }
                hashIndex.erase(bucket->getValue());
                hashIndex.insert(hashIndex.end(),std::pair<std::string,Bucket*>(ofBucket0->getValue(),ofBucket0));
                hashIndex.insert(hashIndex.end(),std::pair<std::string,Bucket*>(ofBucket1->getValue(),ofBucket1));
            }
        }else{
            bucket->insert(key);
        }
    }

    /**Falta implementar*/
    void remove(){

    }

    void print(){
        Bucket* bucketPointer;
        for(auto it:this->hashIndex){
            std::cout<<it.first<<": ";
            bucketPointer = it.second;
            while(bucketPointer != nullptr){
                for(auto it2:bucketPointer->getKeyVector()){
                    std::cout<<it2<<"-";
                }
                std::cout<<"> ";
                bucketPointer = bucketPointer->getNextOverflowBucket();
            }
            std::cout<<"end\n";
        }
    }

};