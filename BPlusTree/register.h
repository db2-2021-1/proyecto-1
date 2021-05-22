#include <string>
#include <map>

const int MAX = 3;

typedef int keyType;
struct Register
{
    keyType id;
    std::string gender;
    int age;
    int hypertension;
    int heartDisease;
    std::string everMarried;
    std::string workType;
    std::string residenceType;
    std::string avgGlucose;
    std::string bmi;
    std::string everSmoke;
    int stroke;

    Register(){};

    Register(keyType id, const std::string &gender, int age, int hypertension, int heartDisease,
             const std::string &everMarried, const std::string &workType, const std::string &residenceType,
             const std::string avgGlucose, const std::string &bmi, const std::string &everSmoke, int stroke) : id(id), gender(gender),
                                                                                                               age(age), hypertension(
                                                                                                                             hypertension),
                                                                                                               heartDisease(heartDisease), everMarried(everMarried), workType(workType),
                                                                                                               residenceType(
                                                                                                                   residenceType),
                                                                                                               avgGlucose(avgGlucose),
                                                                                                               bmi(bmi),
                                                                                                               everSmoke(everSmoke),
                                                                                                               stroke(stroke){};

    void print()
    {
        std::cout << "id: " << id << "\ngender: " << gender << "\nage: " << age << "\nhypertension: " << hypertension
                  << "\nheart disease: " << heartDisease << "\never married: " << everMarried << "\nwork type: "
                  << workType << "\nresidence type: " << residenceType << "\navg. glucose: " << avgGlucose
                  << "\nbody mass index: " << bmi << "\never smoke: " << everSmoke << "\nstroke: " << stroke << "\n\n";
    }
};

struct Node
{
    bool IS_LEAF;
    int *key, size;
    Node **ptr;
    std::map<int, long> values; //int -> id, long -> posicion del registro
    friend class BPTree;
    Node()
    {
        key = new int[MAX];
        ptr = new Node *[MAX + 1];
    }
    int pos_registro = 0;
};
