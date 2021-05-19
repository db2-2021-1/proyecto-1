#include <fstream>

#define blockFactor 2
#define hashSize 4
const std::string filename = "C:\\Users\\alost\\OneDrive\\Escritorio\\sample.dat";
typedef int keyType;

struct Register{
    keyType id;
    char firstName[16];
    char lastName[16];
    int age;

    Register(){}

    Register(keyType _id, std::string _firstName, std::string _lastName, int _age):id(_id),age(_age){
        for(int i = 0; i < 16; i++){
            if(i < _firstName.size()){
                firstName[i] = _firstName[i];
            }else{
                firstName[i] = '\0';
            }
        }
        for(int i = 0; i < 16; i++){
            if(i < _lastName.size()){
                lastName[i] = _lastName[i];
            }else{
                lastName[i] = '\0';
            }
        }

    }
};


class Bucket{
private:
    std::string value;
    std::map<keyType,long> indexVec;
    Bucket* nextOverflowBucket;
    int depth;
public:
    Bucket(std::string const &val):nextOverflowBucket(nullptr),value(val){depth = val.size();}

    void insert(Register reg){
        std::fstream file;
        file.open(filename);
        file.seekg(0,std::ios::end);
        long pos = file.tellg();
        file.write((char *)&reg, sizeof(Register));
        indexVec.insert(indexVec.end(),std::pair<keyType,long>(reg.id,pos));
        file.close();
    }

    std::map<keyType,long> &getIndexVec() {
        return indexVec;
    }

    void setIndexVec(const std::map<keyType,long> &keyVector) {
        Bucket::indexVec = keyVector;
    }

    Bucket *getNextOverflowBucket() const {
        return nextOverflowBucket;
    }

    void setNextOverflowBucket(Bucket *nextOverflowBucket) {
        Bucket::nextOverflowBucket = nextOverflowBucket;
    }

    int getDepth() const {
        return depth;
    }

    void setDepth(int depth) {
        Bucket::depth = depth;
    }

    const std::string &getValue() const {
        return value;
    }

    void setValue(const std::string &value) {
        Bucket::value = value;
    }

    void clearIndexVec(){
        indexVec.clear();
    }

};