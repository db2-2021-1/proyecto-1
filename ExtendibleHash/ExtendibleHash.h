#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <bitset>
#include "Register.h"


class ExtendedHash {
private:
    std::map<std::string,Bucket*> hashIndex;

public:

    /** Construye 2^hashSize de índices y 2 buckets iniciales*/
    ExtendedHash(){
        hashIndex.insert(hashIndex.end(),std::pair<std::string,Bucket*>("0",new Bucket("0")));
        hashIndex.insert(hashIndex.end(),std::pair<std::string,Bucket*>("1",new Bucket("1")));
        std::fstream file;
        file.open(filename, std::ifstream::out | std::ifstream::trunc);
        file.clear();
        file.close();
    }

    /** Toma un valor entero y devuelve los 4 primero bits binarios en string*/
    std::string hashFunction(int key){
        return std::bitset<hashSize>(key%(int)pow(2,hashSize)).to_string();
    }

    /**Funcional*/
    Register find(keyType key){
        Bucket* bucket = locateBucket(key);
        auto it = bucket->getIndexVec().begin();
        while(it->first != key){
            if(it == bucket->getIndexVec().end()){
                if(bucket->getNextOverflowBucket()!=nullptr){
                    bucket = bucket->getNextOverflowBucket();
                    it = bucket->getIndexVec().begin();
                }else{
                    Register error = Register(-1,"NOT_FOUND","NOT_FOUND",-1);
                    return error;
                }
            }
            it++;
        }
        long pos = it->second;
        std::fstream file;
        file.open(filename);
        file.seekg(pos);
        auto *rawReg = new Register;
        file.read((char *)rawReg, sizeof(Register));
        file.close();
        return *rawReg;
    }

    /**Funcional, algoritmo costoso, pero es range query en hash asi que... ¿que debería esperar?*/
    std::vector<Register> find(keyType beginKey, keyType endKey){
        std::vector<Register> ansVec = std::vector<Register>();
        auto *rawReg = new Register;
        Bucket *bucketPointer;
        long pos;
        std::fstream file;
        for(auto hashIt:hashIndex){
            bucketPointer = hashIt.second;
            while(bucketPointer != nullptr){
                for(auto bucketIt:bucketPointer->getIndexVec()){
                    if((bucketIt.first >= beginKey)&&(bucketIt.first <= endKey)){
                        pos = bucketIt.second;
                        file.open(filename);
                        file.seekg(pos);
                        file.read((char *)rawReg, sizeof(Register));
                        file.close();
                        ansVec.push_back(*rawReg);
                    }
                }
                bucketPointer = bucketPointer->getNextOverflowBucket();
            }

        }
        return ansVec;
    }

    /**Funcional*/
    void insert(Register reg){
        Bucket *bucket = locateBucket(reg.id);
        keyType key = reg.id;
        bucket->insert(reg);
        overflowVerificator(bucket);
    }

    /**Falta implementar*/
    void erase(keyType key){
        Bucket* bucket = locateBucket(key);
        auto it = bucket->getIndexVec().begin();
        while(it->first != key){
            if(it == bucket->getIndexVec().end()){
                if(bucket->getNextOverflowBucket()!=nullptr){
                    bucket = bucket->getNextOverflowBucket();
                    it = bucket->getIndexVec().begin();
                }
            }
            it++;
        }
        long pos = it->second;
        std::fstream file;
        char blankSpace[sizeof(Register)];
        file.open(filename);
        file.seekg(pos);
        file.write(blankSpace, sizeof(Register));
        file.close();
        //Si el bucket queda vacio, liberarlo


    }

    void overflowVerificator(Bucket *bucket){
        if(bucket->getIndexVec().size() > blockFactor){
            if(bucket->getDepth() == hashSize){
                auto overflowBucket = new Bucket(bucket->getValue());
                for(int i = 0; i < blockFactor; i++){
                    auto it = (--bucket->getIndexVec().end());
                    overflowBucket->getIndexVec().insert(overflowBucket->getIndexVec().begin()
                            ,std::pair<keyType,long>(it->first,it->second));
                    bucket->getIndexVec().erase(it->first);
                }
                overflowBucket->setNextOverflowBucket(bucket->getNextOverflowBucket());
                bucket->setNextOverflowBucket(overflowBucket);
            }else{
                auto ofBucket0 = new Bucket("0" + bucket->getValue());
                auto ofBucket1 = new Bucket("1" + bucket->getValue());
                std::string keyIt;
                for(auto it:bucket->getIndexVec()){
                    keyIt = hashFunction(it.first);
                    if(keyIt[hashSize - bucket->getValue().size() - 1] == '0'){
                        ofBucket0->getIndexVec().insert(ofBucket0->getIndexVec().end()
                                ,std::pair<keyType,long>(it.first,it.second));
                    }else{
                        ofBucket1->getIndexVec().insert(ofBucket1->getIndexVec().end()
                                ,std::pair<keyType,long>(it.first,it.second));
                    }
                }
                hashIndex.erase(bucket->getValue());
                if(!ofBucket0->getIndexVec().empty()){
                    hashIndex.insert(hashIndex.end()
                            ,std::pair<std::string,Bucket*>(ofBucket0->getValue(),ofBucket0));
                    overflowVerificator(ofBucket0);
                }
                if(!ofBucket1->getIndexVec().empty()){
                    hashIndex.insert(hashIndex.end()
                            ,std::pair<std::string,Bucket*>(ofBucket1->getValue(),ofBucket1));
                    overflowVerificator(ofBucket1);
                }
            }
        }
    }

    Bucket* locateBucket(keyType key){
        std::string value = hashFunction(key);
        while(!value.empty()){
            auto it = this->hashIndex.find(value);
            if(it != this->hashIndex.end()){
                return it->second;
            }else{
                value.erase(0,1);
            }
        }
        auto generatedBucket = new Bucket(value);
        hashIndex.insert(std::pair<std::string,Bucket*>(value,generatedBucket));
        return generatedBucket;
    }

    void print(){
        Bucket* bucketPointer;
        for(auto it:this->hashIndex){
            std::cout<<it.first<<": ";
            bucketPointer = it.second;
            while(bucketPointer != nullptr){
                for(auto it2:bucketPointer->getIndexVec()){
                    std::cout<<it2.first<<"-";
                }
                std::cout<<"> ";
                bucketPointer = bucketPointer->getNextOverflowBucket();
            }
            std::cout<<"end\n";
        }
    }

};