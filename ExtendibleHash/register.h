#include <fstream>

#define blockFactor 4
#define hashSize 9
typedef long long keyType;

struct Register{
    keyType id;
    std::string gender;
    int age;
    int hypertension;
    int heartDisease;
    std::string everMarried;
    std::string workType;
    std::string residenceType;
    int avgGlucose;
    std::string bmi;
    std::string everSmoke;
    int stroke;

    Register(){};

    Register(keyType id, const std::string &gender, int age, int hypertension, int heartDisease,
             const std::string &everMarried, const std::string &workType, const std::string &residenceType,
             int avgGlucose, const std::string &bmi, const std::string &everSmoke, int stroke) : id(id), gender(gender),
                                                                                                 age(age), hypertension(
                    hypertension), heartDisease(heartDisease), everMarried(everMarried), workType(workType),
                                                                                                 residenceType(
                                                                                                         residenceType),
                                                                                                 avgGlucose(avgGlucose),
                                                                                                 bmi(bmi),
                                                                                                 everSmoke(everSmoke),
                                                                                                 stroke(stroke) {};
    void print(){
        std::cout<<"id: "<<id<<"\ngender: "<<gender<<"\nage: "<<age<<"\nhypertension: "<<hypertension
                        <<"\nheart disease: "<<heartDisease<<"\never married: "<<everMarried<<"\nwork type: "
                        <<workType<<"\nresidence type: "<<residenceType<<"\navg. glucose: "<<avgGlucose
                        <<"\nbody mass index: "<<bmi<<"\never smoke: "<<everSmoke<<"\nstroke: "<<stroke<<"\n\n";
    }

    std::string* dataToCSV(){
        return new std::string(std::to_string(id)+","+gender+","+std::to_string(age)+","+std::to_string(hypertension)+","+
                               std::to_string(heartDisease)+","+everMarried+","+workType+","+residenceType+","+std::to_string(avgGlucose)+
                               bmi+","+everSmoke+","+std::to_string(stroke)+"\n");
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



    long getRegisterPos(keyType key){
        auto it = indexVec.find(key);
        if(it != indexVec.end())
            return it->second;
        else if (this->nextOverflowBucket != nullptr)
            return nextOverflowBucket->getRegisterPos(key);
        else
            return -1;
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