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

#ifndef GRAPHICCOMPONENT_H
#define GRAPHICCOMPONENT_H

#include "Component.hpp"
#include "TextComponent.hpp"

namespace WishEngine{
    class GraphicComponent : public Component{
        private:
            Color color;
            std::string textureFile;
            int priority;
            bool isTexture, isText, isUi;
            TextComponent text;
            FlipAndRotation flipRotate;
        public:
            GraphicComponent(bool isU);
            GraphicComponent(bool isU, int r, int g, int b, int a, int pr);
            GraphicComponent(bool isU, std::string tFile, int a, int pr, int fl = 0, int xOffset = 0, int yOffset = 0, double degrees = 0);
            GraphicComponent(bool isU, std::string t, std::string font, int maxLines, int lineSpacing, int fontSize, int r, int g, int b, int a, int pr, bool isPlain, int fl = 0, int xOffset = 0, int yOffset = 0, double degrees = 0);
            virtual ~GraphicComponent();
            std::string &getTextureFile();
            void setTextureFile(std::string file);
            void setPriority(int pr);
            int getPriority();
            void setAlpha(int a);
            int getAlpha();
            void setColor(int r, int g, int b);
            void setR(int r);
            void setG(int g);
            void setB(int b);
            int getR();
            int getG();
            int getB();
            bool getIsTexture();
            void setIsTexture(bool isT);
            bool getIsText();
            void setIsText(bool text);
            TextComponent &getText();
            void setText(TextComponent t);
            bool getIsUi();
            void setIsUi(bool isU);
            //Rotation getters and setters
            int getFlip();
            void setFlip(int fl);
            int getRotXOffset();
            void setRotXOffset(int offset);
            int getRotYOffset();
            void setRotYOffset(int offset);
            double getRotationDegrees();
            void setRotationDegrees(double degrees);
    };
}
#endif // GRAPHICCOMPONENT
