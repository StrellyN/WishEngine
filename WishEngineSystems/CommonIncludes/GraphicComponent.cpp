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

#include "GraphicComponent.hpp"

namespace WishEngine{
    GraphicComponent::GraphicComponent(bool isU){
        priority = 0;
        isUi = isU;
        setType(COMPONENTTYPES::GRAPHIC);
        isTexture = false;
        isText = false;
        color = Color();
        textureFile = "";
    }

    GraphicComponent::GraphicComponent(bool isU, int r, int g, int b, int a, int pr){
        setType(COMPONENTTYPES::GRAPHIC);
        isUi = isU;
        color = Color(r,g,b,a);
        priority = pr;
        isTexture = false;
        isText = false;
        textureFile = "";
    }

    GraphicComponent::GraphicComponent(bool isU, std::string tFile, int a, int pr){
        setType(COMPONENTTYPES::GRAPHIC);
        isUi = isU;
        color = Color(0,0,0,a);
        priority = pr;
        isTexture = true;
        isText = false;
        textureFile = tFile;
    }

    GraphicComponent::GraphicComponent(bool isU, std::string t, std::string font, int maxLines, int lineSpacing, int fontSize, int r, int g, int b, int a, int pr, bool isPlain){
        setType(COMPONENTTYPES::GRAPHIC);
        isUi = isU;
        priority = pr;
        isTexture = false;
        isText = true;
        if(t != ""){
            text = TextComponent(font, maxLines, lineSpacing, isPlain);
            for(std::string::iterator it=t.begin(); it!=t.end(); ++it) {
                text.addCharacter(*it, fontSize, r, g, b, a);
            }
        }
        else{
            text = TextComponent(font, maxLines, lineSpacing, isPlain);
        }
    }

    GraphicComponent::~GraphicComponent(){
    }

    std::string &GraphicComponent::getTextureFile(){
        return textureFile;
    }

    void GraphicComponent::setTextureFile(std::string file){
        textureFile = file;
    }

    int GraphicComponent::getPriority(){
        return priority;
    }

    void GraphicComponent::setPriority(int pr){
        priority = pr;
    }

    void GraphicComponent::setAlpha(int a){
        color.setA(a);
    }

    int GraphicComponent::getAlpha(){
        return color.getA();
    }

    void GraphicComponent::setColor(int r, int g, int b){
        color.setR(r);
        color.setG(g);
        color.setB(b);
    }

    void GraphicComponent::setR(int r){
        color.setR(r);
    }

    void GraphicComponent::setG(int g){
        color.setG(g);
    }

    void GraphicComponent::setB(int b){
        color.setB(b);
    }

    int GraphicComponent::getR(){
        return color.getR();
    }

    int GraphicComponent::getG(){
        return color.getG();
    }

    int GraphicComponent::getB(){
        return color.getB();
    }

    bool GraphicComponent::getIsTexture(){
        return isTexture;
    }

    void GraphicComponent::setIsTexture(bool isT){
        isTexture = isT;
    }

    bool GraphicComponent::getIsText(){
        return isText;
    }

    void GraphicComponent::setIsText(bool text){
        isText = text;
    }

    TextComponent &GraphicComponent::getText(){
        return text;
    }

    void GraphicComponent::setText(TextComponent t){
        text = t;
    }

    bool GraphicComponent::getIsUi(){
        return isUi;
    }

    void GraphicComponent::setIsUi(bool isU){
        isUi = isU;
    }
}
