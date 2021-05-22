#include <iostream>
#include <fstream>

#include "register.h"

const std::string filename = "./data.dat";
const std::string csv_ = "./healthcare-dataset-stroke-data.csv";

class Lector
{
public:
    Lector(){}
    Register CsvtoRegister(std::string line)
    {        char c;
        int state = 0, id = 0, age = 0, hypertension = 0, heartDisease = 0, stroke = 0;
        std::string word, gender, everMarried, workType, residenceType, everSmoked, bmi, avgGlucose;
        for(int i = 0; i < line.size(); i++){
            if(line[i] == '\n'){
                word.clear();
                break;
            }else if(line[i] == ','){
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
                word += line[i];
            }
        }
        return Register(id,gender,age,hypertension,heartDisease,everMarried,workType,residenceType,avgGlucose,bmi,everSmoked,stroke);
    }

    std::vector<Register> scanAllCSV()
    {
        std::vector<Register> retorno;
        std::fstream file;
        std::string ignoreHeader, line;
        long pos;
        Register reg;
        file.open(csv_);
        int ite = 0;
        getline(file, ignoreHeader);
        while (getline(file, line))
        {
            retorno.push_back(CsvtoRegister(line));
        }
        return retorno;
    }
};
