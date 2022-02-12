#include <stdio.h>
#include <initializer_list>
#include <iostream>

using namespace std;

static int findPosition;

enum foreColor
{
    fNone = 1,

    fBlack = 30,
    fRed = 31,
    fGreen = 32,
    fYellow = 33,
    fBlue = 34,
    fMagenta = 35,
    fCyan = 36,
    fGray = 90,
    fBrightRed = 91,
    fBrightGreen = 92,
    fBrightYellow = 93,
    fBrightBlue = 94,
    fBrightMagenta = 95,
    fBrightCyan = 96,
    fBrightWhite = 97,
};

enum backColor
{
    bNone = 1,

    bBlack = 40,
    bRed = 41,
    bGreen = 42,
    bYellow = 43,
    bBLue = 44,
    bMagenta = 45,
    bCyan = 46,
    bGray = 100,
    bBrightRed = 101,
    bBrightGreen = 102,
    bBrightYellow = 103,
    bBrightBlue = 104,
    bBrightMagenta = 105,
    bBrightCyan = 106,
    bBrightWhite = 107
};

void colorizedPrint(int foreground, int background, string textToPrint, initializer_list<string> arguments = initializer_list<string>())
{
    for(auto argument = arguments.begin();argument != arguments.end(); argument++){
        findPosition = textToPrint.find("%s",0);
        if(findPosition != string::npos){
            textToPrint.replace(findPosition,2,*argument);
        }
        else{
            break;
        }
    }
    printf("\033[%i;%im%s\033[0m", foreground, background, textToPrint.c_str());
}
