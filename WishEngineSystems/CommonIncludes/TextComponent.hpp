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

#ifndef TEXTCOMPONENT_H
#define TEXTCOMPONENT_H

#include "Utils.hpp"

namespace WishEngine{
    struct FlipAndRotation{
        int flip = 0, rotateXOffset = 0, rotateYOffset = 0;
        double rotationDegrees = 0;
    };

    class TextComponent{
        private:
            int maxLines, lineSpacing; //Max lines the text is going to have and the spacing between lines (taking the first character height)
            std::string text, font; //Text to render and font name
            std::vector<int> individualCharacterSize; //Font size for each character
            std::vector<std::string> individualCharacterSizeString; //String of size to improve performance
            std::vector<Color> individualCharacterColor; //Color for each character
            std::vector<FlipAndRotation> individualCharacterFlipRotation;
            int startingLine = 0;
            bool isPlain;
        public:
            TextComponent();
            TextComponent(std::string t, std::string fName, int r, int g, int b, int a, int tSize, int maxL, int lSpace, bool plain, FlipAndRotation flipRot = FlipAndRotation());
            TextComponent(std::string fName, int maxL, int lSpace, bool plain);
            ~TextComponent();
            void setText(std::string t, int r, int g, int b, int a, int fSize, FlipAndRotation flipRot = FlipAndRotation());
            std::string &getText();
            void clearText();
            char getCharacter(int position);
            void addCharacter(char ch, int chSize, int r, int g, int b, int a, FlipAndRotation flipRot = FlipAndRotation());
            void deleteCharacter(int position);
            void deleteCharacters(char toDelete);
            int getMaxLines();
            void setMaxLines(int mL);
            int getLineSpacing();
            void setLineSpacing(int lS);
            std::string &getFont();
            void setFont(std::string fName);
            std::vector<int> &getIndividualCharacterSize();
            std::vector<Color> &getIndividualCharacterColor();
            std::vector<std::string> &getIndividualCharacterSizeString();
            std::vector<FlipAndRotation> &getIndividualCharacterFlipRotation();
            int getStartingLine();
            void setStartingLine(int sL);
            bool getIsPlain();
            void setIsPlain(bool plain);
    };
}
#endif // TEXTCOMPONENT_H
