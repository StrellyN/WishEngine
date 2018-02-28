/**
    Copyright 2018 Strelly

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without restriction,
    including without limitation the rights to use, copy, modify, merge,
    publish, distribute, sublicense, and/or sell copies of the Software,
    and to permit persons to whom the Software is furnished to do so,
    subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
    OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
**/

#ifndef UTILS_H
#define UTILS_H

#include "DirectionVector.hpp"
#include "Rectangle.hpp"
#include "Color.hpp"
#include "Headers.hpp"

namespace WishEngine{
    typedef struct CollisionStruct{
        unsigned index;
        std::string name;
        CollisionStruct(unsigned ind, std::string n){
            index = ind;
            name = n;
        }
        ~CollisionStruct(){}
    };

    class Utils{
        public:
            static bool checkCollision(Rectangle &rect1, Rectangle &rect2){
                double left1 = rect1.getX(), right1 = rect1.getX() + rect1.getW();
                double top1 = rect1.getY(), bottom1 = rect1.getY() + rect1.getH();
                double left2 = rect2.getX(), right2 = rect2.getX() + rect2.getW();
                double top2 = rect2.getY(), bottom2 = rect2.getY() + rect2.getH();

                if( bottom1 < top2 ){
                    return false;
                }
                if( top1 > bottom2 ){
                    return false;
                }
                if( right1 < left2 ){
                    return false;
                }
                if( left1 > right2 ){
                    return false;
                }
                return true;
            }
            static int stringToInt(std::string str){
                int result=0, sign=0;
                for(int i=0; i<str.size(); i++){
                    if(i==0 && str[i]=='-'){
                        sign=1;
                        continue;
                    }
                    int mult = 1;
                    for(int j=0; j<str.size()-1-i-sign; j++){
                        mult *= 10;
                    }
                    result += (str[i]-'0')*mult;
                }
                if(sign){
                    return -result;
                }
                return result;
            }
            static std::string intToString(int number){
                std::string returnValue;
                if(number<0){
                    returnValue += '-';
                    number = -number;
                }
                if(number>=10){
                    returnValue += intToString(number/10);
                }
                returnValue += '0'+number%10;
                return returnValue;
            }
            static double stringToDouble(std::string str){
                int sign=0, pointPos=-1, pointAux=-1;
                int number = 0;
                double result=0;
                for(int i=0; i<str.size(); i++){
                    if(str[i]=='.'){
                        pointAux = i;
                        break;
                    }
                }
                for(int i=0; i<str.size(); i++){
                    if(i==0 && str[i]=='-'){
                        sign=1;
                        continue;
                    }
                    if(str[i]=='.'){
                        pointPos = pointAux;
                        continue;
                    }
                    if(i==pointPos){
                        continue;
                    }
                    int mult = 1;
                    if(pointPos == -1){
                        for(int j=0; j<pointAux-1-i-sign; j++){
                            mult *= 10;
                        }
                        number += (str[i]-'0')*mult;
                    }
                    else{
                        for(int j=0; j<i-pointPos; j++){
                            mult *= 10;
                        }
                        result += (double)(str[i]-'0')/(double)(mult);
                    }
                }
                result += number;
                if(sign){
                    return -result;
                }
                return result;
            }
            static std::string remainerToString(double remainer){
                std::string returnValue;
                double num = remainer*10;
                int aux = num;
                double rem = num - aux;
                returnValue += '0'+aux;
                if(rem>0.0000001 && rem < 0.9999){
                    returnValue += remainerToString(rem);
                }
                return returnValue;
            }
            static std::string doubleToString(double number){ //It doesn't work 100% right, it always changes a value
                std::string returnValue;
                int num = number;
                double remainer = number - num;
                returnValue += intToString(num);
                returnValue += '.';
                returnValue += remainerToString(remainer);
                return returnValue;
            }
            static double fRand(double fMin, double fMax){ //Change to new RANDOM?
                double f = (double)rand() / RAND_MAX;
                return fMin + f * (fMax - fMin);
            }
            static double getDistanceForAudio(DirectionVector &listen, DirectionVector &emit, DirectionVector &audio){
                return sqrt(pow((listen.getX()-(emit.getX()+audio.getX())), 2)+
                            pow((listen.getY()-(emit.getY()+audio.getY())), 2)+
                            pow((listen.getZ()-(emit.getZ()+audio.getZ())), 2));
            }
            static int getVolumeFromDistance(double minRad, double maxRad, double maxVol, double currentDist){
                return (int)(maxVol*(1-((currentDist-minRad)/(maxRad-minRad))));
            }

            static bool fileAccessible(std::string fileName){
                if (FILE *file = fopen(fileName.c_str(), "r")) {
                    fclose(file);
                    return true;
                } else {
                    return false;
                }
            }

            static void writeIntToFile(std::string fileName, std::string dataName, int data, bool append){
                std::fstream stream;
                if(append){ //check for existance and do the copying here
                    stream.open(fileName, std::ios::out | std::ios::app);
                    stream << dataName << " " << data << "\n";
                }
                else{
                    stream.open(fileName, std::ios::out);
                    stream << dataName << " " << data << "\n";
                }
                stream.close();
            }

            static void writeDoubleToFile(std::string fileName, std::string dataName, double data, bool append){
                std::fstream stream;
                if(append){ //check for existance and do the copying here
                    stream.open(fileName, std::ios::out | std::ios::app);
                    stream << dataName << " " << data << "\n";
                }
                else{
                    stream.open(fileName, std::ios::out);
                    stream << dataName << " " << data << "\n";
                }
                stream.close();
            }

            static void writeStringToFile(std::string fileName, std::string dataName, std::string data, bool append){
                std::fstream stream;
                if(append){ //check for existance and do the copying here
                    stream.open(fileName, std::ios::out | std::ios::app);
                    stream << dataName << " " << data << "\n";
                }
                else{
                    stream.open(fileName, std::ios::out);
                    stream << dataName << " " << data << "\n";
                }
                stream.close();
            }

            static void replaceIntInFile(std::string fileName, std::string dataName, int data){
                std::fstream stream, copyStream;
                std::string auxFileName = fileName;
                auxFileName += "aux";
                bool found = false;
                stream.open(fileName, std::ios::in);
                copyStream.open(auxFileName, std::ios::in | std::ios::app);

                std::stringstream line;
                std::string aux, name;
                while(std::getline(stream, aux)){
                    line << aux;
                    line >> name;
                    if(name == dataName){
                        copyStream << dataName << " " << data << "\n";
                        found = true;
                    }
                    else{
                        copyStream << aux << "\n";
                    }
                }
                if(!found){
                    copyStream << dataName << " " << data << "\n";
                }
                copyStream.close();
                stream.close();
                remove(fileName.c_str());
                rename(auxFileName.c_str(), fileName.c_str());
            }

            static void replaceDoubleInFile(std::string fileName, std::string dataName, double data){
                std::fstream stream, copyStream;
                std::string auxFileName = fileName;
                auxFileName += "aux";
                bool found = false;
                stream.open(fileName, std::ios::in);
                copyStream.open(auxFileName, std::ios::in | std::ios::app);

                std::stringstream line;
                std::string aux, name;
                while(std::getline(stream, aux)){
                    line << aux;
                    line >> name;
                    if(name == dataName){
                        copyStream << dataName << " " << data << "\n";
                        found = true;
                    }
                    else{
                        copyStream << aux << "\n";
                    }
                }
                if(!found){
                    copyStream << dataName << " " << data << "\n";
                }
                copyStream.close();
                stream.close();
                remove(fileName.c_str());
                rename(auxFileName.c_str(), fileName.c_str());
            }

            static void replaceStringInFile(std::string fileName, std::string dataName, std::string data){
                std::fstream stream, copyStream;
                std::string auxFileName = fileName;
                auxFileName += "aux";
                bool found = false;
                stream.open(fileName, std::ios::in);
                copyStream.open(auxFileName, std::ios::in | std::ios::app);

                std::stringstream line;
                std::string aux, name;
                while(std::getline(stream, aux)){
                    line << aux;
                    line >> name;
                    if(name == dataName){
                        copyStream << dataName << " " << data << "\n";
                        found = true;
                    }
                    else{
                        copyStream << aux << "\n";
                    }
                }
                if(!found){
                    copyStream << dataName << " " << data << "\n";
                }
                copyStream.close();
                stream.close();
                remove(fileName.c_str());
                rename(auxFileName.c_str(), fileName.c_str());
            }

            static int readIntFromFile(std::string fileName, std::string dataName){
                int data = -1;
                std::fstream stream(fileName, std::ios::in);
                std::stringstream line;
                std::string names, aux;
                while(std::getline(stream, aux)){
                    line << aux;
                    line >> names;
                    if(names == dataName){
                        line >> data;
                        break;
                    }
                }
                stream.close();
                return data;
            }

            static double readDoubleFromFile(std::string fileName, std::string dataName){
                double data = -1;
                std::fstream stream(fileName, std::ios::in);
                std::stringstream line;
                std::string names, aux;
                while(std::getline(stream, aux)){
                    line << aux;
                    line >> names;
                    if(names == dataName){
                        line >> data;
                        break;
                    }
                }
                stream.close();
                return data;
            }

            static std::string readStringFromFile(std::string fileName, std::string dataName){
                std::string data = "";
                std::fstream stream(fileName, std::ios::in);
                std::stringstream line;
                std::string names, aux;
                while(std::getline(stream, aux)){
                    line.str(aux);
                    line >> names;
                    if(names == dataName){
                        line >> data;
                        while(line >> aux){
                            data += " ";
                            data += aux;
                        }
                        break;
                    }
                }
                stream.close();
                return data;
            }

            static bool dataExistsInFile(std::string fileName, std::string dataName){
                std::fstream stream(fileName, std::ios::in);
                std::stringstream line;
                std::string names, aux;
                while(std::getline(stream, aux)){
                    line.str(aux);
                    line >> names;
                    if(names == dataName){
                        stream.close();
                        return true;
                    }
                }
                stream.close();
                return false;
            }
    };
}
#endif // UTILS
