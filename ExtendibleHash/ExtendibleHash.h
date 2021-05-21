#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <bitset>
#include <string>
#include "Register.h"


class ExtendedHash {
private:
    std::map<std::string,Bucket*> hashIndex;
    std::string filename;

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
        value = hashFunction(key);
        auto generatedBucket = new Bucket(value);
        hashIndex.insert(std::pair<std::string,Bucket*>(value,generatedBucket));
        return generatedBucket;
    }

    void fixIndex() {
        bool issue = false;
        Bucket *bucketPointer;
        for(auto hashIt:hashIndex){
            if(hashIt.second->getIndexVec().empty()){
                hashIt.second = hashIt.second->getNextOverflowBucket();
            }else{
                bucketPointer = hashIt.second;
                while(bucketPointer!= nullptr){
                    if(bucketPointer->getNextOverflowBucket()->getIndexVec().empty()){
                        bucketPointer->setNextOverflowBucket(bucketPointer->getNextOverflowBucket()->getNextOverflowBucket());
                    }
                    bucketPointer = bucketPointer->getNextOverflowBucket();
                }
            }
        }
    }

    Register textToRegisterCSV(std::string text){
        char c;
        int state = 0, id = 0, age = 0, hypertension = 0, heartDisease = 0, stroke = 0;
        std::string word, gender, everMarried, workType, residenceType, everSmoked, bmi, avgGlucose;
        for(int i = 0; i < text.size(); i++){
            if(text[i] == '\n'){
                word.clear();
                break;
            }else if(text[i] == ','){
                if(state == 0) {
                    id = std::stoi(word);
                    word.clear();
                    state++;
                }else if(state == 1){
                    gender = word;
                    word.clear();
                    state++;
                }else if(state == 2){
                    age = std::stoi(word);
                    word.clear();
                    state++;
                }else if(state == 3){
                    hypertension = std::stoi(word);
                    word.clear();
                    state++;
                }else if(state == 4){
                    heartDisease = std::stoi(word);
                    word.clear();
                    state++;
                }else if(state == 5){
                    everMarried = word;
                    word.clear();
                    state++;
                }else if(state == 6){
                    workType = word;
                    word.clear();
                    state++;
                }else if(state == 7){
                    residenceType = word;
                    word.clear();
                    state++;
                }else if(state == 8){
                    avgGlucose = word;
                    word.clear();
                    state++;
                }else if(state == 9){
                    bmi = word;
                    word.clear();
                    state++;
                }else if(state == 10){
                    everSmoked = word;
                    word.clear();
                    state++;
                }else{
                    stroke = std::stoi(word);
                    word.clear();
                }
            }else{
                word += text[i];
            }
        }
        return Register(id,gender,age,hypertension,heartDisease,everMarried,workType,residenceType,avgGlucose,bmi,everSmoked,stroke);
    }

    void insertIndex(Register const &reg, long pos){
        Bucket *bucket = locateBucket(reg.id);
        bucket->getIndexVec().insert(bucket->getIndexVec().end(),std::pair<keyType,long>(reg.id,pos));
        overflowVerificator(bucket);
    }

    /** Toma un valor entero y devuelve los n primero bits binarios en string*/
    std::string hashFunction(int key){
        return std::bitset<hashSize>(key%(int)pow(2,hashSize)).to_string();
    }

    long getRegisterPos(keyType key){
        Bucket* bucket = locateBucket(key);
        auto it = bucket->getIndexVec().begin();
        while(it->first != key){
            if(it == bucket->getIndexVec().end()){
                if(bucket->getNextOverflowBucket()!=nullptr){
                    bucket = bucket->getNextOverflowBucket();
                    it = bucket->getIndexVec().begin();
                }
            }else{
                it++;
            }
        }
        return it->second;
    }

public:

    /** Construye 2^hashSize de índices y 2 buckets iniciales*/
    ExtendedHash(std::string const &_filename){
        hashIndex.insert(hashIndex.end(),std::pair<std::string,Bucket*>("0",new Bucket("0")));
        hashIndex.insert(hashIndex.end(),std::pair<std::string,Bucket*>("1",new Bucket("1")));
        filename = _filename;
    }

    void scanAllCSV(){
        hashIndex.clear();
        std::fstream file;
        std::string ignoreHeader, line;
        long pos;
        Register reg;
        file.open(filename);
        getline(file,ignoreHeader);
        pos = file.tellg();
        while(getline(file,line)){
            reg = textToRegisterCSV(line);
            insertIndex(reg, pos);
            pos = file.tellg();
        }
    }

    /**Funcional*/
    Register find(keyType key){
        long pos = getRegisterPos(key);
        std::fstream file;
        std::string line;
        file.open(filename);
        file.seekg(pos);
        getline(file,line);
        Register reg = textToRegisterCSV(line);
        file.close();
        return reg;
    }

    /**Funcional*/
    std::vector<Register> find(keyType beginKey, keyType endKey){
        std::vector<Register> ansVec = std::vector<Register>();
        auto reg =  new Register;
        std::fstream file;
        std::string line, headerLine;
        file.open(filename);
        getline(file,headerLine);
        while(getline(file,line)){
            *reg = textToRegisterCSV(line);
            if(reg->id >= beginKey && reg->id <= endKey){
                ansVec.push_back(*reg);
            }
        }
        return ansVec;
    }

    /**Funcional*/
    void insert(Register reg){
        std::fstream file;
        file.open(filename);
        file.seekg(0,std::ios::end);
        long pos = file.tellg();
        file<<'\n'<<reg.dataToCSV();
        file.close();
        insertIndex(reg,pos);
    }

    /**Funcional*/
    void erase(keyType key){
        std::vector<Register> regVec;
        Register reg;
        std::fstream file;
        std::string line, header;
        file.open(filename);
        getline(file,header);
        while(getline(file,line)){
            reg = textToRegisterCSV(line);
            if(reg.id != key)
                regVec.push_back(reg);
        }
        file.close();
        file.open(filename, std::ofstream::out | std::ofstream::trunc);
        file<<header;
        for(auto it:regVec){
            file<<'\n'<<it.dataToCSV();
        }
        file.close();
        scanAllCSV();
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