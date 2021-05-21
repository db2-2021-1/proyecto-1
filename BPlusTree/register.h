#include <fstream>
#include <map>


/*
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

};*/