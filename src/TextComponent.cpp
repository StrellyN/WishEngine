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

#include "TextComponent.hpp"

namespace WishEngine{
    TextComponent::TextComponent(){
        maxLines = 0;
        lineSpacing = 0;
        isPlain = false;
    }

    TextComponent::TextComponent(std::string t, std::string fName, int r, int g, int b, int a, int tSize, int maxL, int lSpace, bool plain){
        text = t;
        font = fName;
        maxLines = maxL;
        lineSpacing = lSpace;
        isPlain = plain;
        for(std::string::iterator it=text.begin(); it!=text.end(); ++it){
            individualCharacterColor.push_back(Color(r,g,b,a));
            individualCharacterSize.push_back(tSize);
            individualCharacterSizeString.push_back(Utils::intToString(tSize));
        }
    }

    TextComponent::TextComponent(std::string fName, int maxL, int lSpace, bool plain){
        font = fName;
        maxLines = maxL;
        lineSpacing = lSpace;
        isPlain = plain;
    }

    TextComponent::~TextComponent(){
        individualCharacterColor.clear();
        individualCharacterSize.clear();
        individualCharacterSizeString.clear();
    }

    void TextComponent::setText(std::string t, int r, int g, int b, int a, int fSize){
        individualCharacterColor.clear();
        individualCharacterSize.clear();
        individualCharacterSizeString.clear();
        text = t;
        for(std::string::iterator it=text.begin(); it!=text.end(); ++it){
            individualCharacterColor.push_back(Color(r,g,b,a));
            individualCharacterSize.push_back(fSize);
            individualCharacterSizeString.push_back(Utils::intToString(fSize));
        }
    }

    std::string &TextComponent::getText(){
        return text;
    }

    void TextComponent::clearText(){
        text = "";
        individualCharacterColor.clear();
        individualCharacterSize.clear();
        individualCharacterSizeString.clear();
    }

    char TextComponent::getCharacter(int position){
        if(position > 0 && position < text.size()){
            return text[position];
        }
        return text[0];
    }

    void TextComponent::addCharacter(char ch, int chSize, int r, int g, int b, int a){
        text.push_back(ch);
        individualCharacterColor.push_back(Color(r,g,b,a));
        individualCharacterSize.push_back(chSize);
        individualCharacterSizeString.push_back(Utils::intToString(chSize));
    }

    void TextComponent::deleteCharacter(int position){
        if(position > 0 && position < text.size()){
            text.erase(text.begin()+position);
        }
    }

    void TextComponent::deleteCharacters(char toDelete){
        for(std::string::iterator it=text.begin(); it!=text.end(); ++it){
            if(*it == toDelete){
                text.erase(it);
            }
        }
    }

    int TextComponent::getMaxLines(){
        return maxLines;
    }

    void TextComponent::setMaxLines(int mL){
        maxLines = mL;
    }

    int TextComponent::getLineSpacing(){
        return lineSpacing;
    }

    void TextComponent::setLineSpacing(int lS){
        if(lS<0) lS = 0;
        lineSpacing = lS;
    }

    std::string &TextComponent::getFont(){
        return font;
    }

    void TextComponent::setFont(std::string fName){
        font = fName;
    }

    std::vector<int> &TextComponent::getIndividualCharacterSize(){
        return individualCharacterSize;
    }

    std::vector<Color> &TextComponent::getIndividualCharacterColor(){
        return individualCharacterColor;
    }

    std::vector<std::string> &TextComponent::getIndividualCharacterSizeString(){
        return individualCharacterSizeString;
    }

    int TextComponent::getStartingLine(){
        return startingLine;
    }

    void TextComponent::setStartingLine(int sL){
        if(sL<0) sL = 0;
        startingLine = sL;
    }

    bool TextComponent::getIsPlain(){
        return isPlain;
    }

    void TextComponent::setIsPlain(bool plain){
        isPlain = plain;
    }
}
