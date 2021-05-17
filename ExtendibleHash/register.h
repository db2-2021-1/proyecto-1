#define blockFactor 4
#define hashSize 3

typedef int keyType;

struct Register{
    keyType id;
    char firstName[16];
    char lastName[16];
    int age;

};


class Bucket{
private:
    std::string value;
    std::vector<keyType> keyVector;
    Bucket* nextOverflowBucket;
    int depth;
public:
    Bucket(std::string const &val):nextOverflowBucket(nullptr),value(val){depth = val.size();}

    void insert(keyType key){
        keyVector.push_back(key);
    }

    const std::vector<keyType> &getKeyVector() const {
        return keyVector;
    }

    void setKeyVector(const std::vector<keyType> &keyVector) {
        Bucket::keyVector = keyVector;
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

    void clearKeyVector(){
        keyVector.clear();
    }

};