#!/bin/bash

echo -e "\033[43mWARNING!\033[0m This build system is in development"
echo -e "\033[44mWAITING...\033[0m Looking for header files...\n  -> ls *.h"
HEADERS=$(ls *.h 2>/dev/null)
if [ -z "$HEADERS" ]; then
    echo -e "\033[41mFAILED!\033[0m No header files found!"
else
    echo -e "\033[42mSUCCESS!\033[0m Using headers: $HEADERS"
fi

echo -e "\033[44mWAITING...\033[0m Building mainc.cpp...\n  -> g++ $HEADERS main.cpp -o rsf"

g++ $HEADERS main.cpp -o rsf



if [ $? -eq 0 ]; then
    echo -e "\033[42mSUCCESS!\033[0m Build completed."
    echo -e "\033[44mWAITING...\033[0m Appling execute parms...\n  -> chmod +x ./rsf"
    chmod +x ./rsf
    echo -e "\033[44mWAITING...\033[0m Copying to /usr/bin...\n  -> cp ./rsf /usr/bin/rsf"
    cp ./rsf /usr/bin/rsf
    echo -e "\033[44mWAITING...\033[0m Running RSF...\n  -> ./rsf"
    ./rsf
else
    echo -e "\033[41mFAILED!\033[0m Compilation error."
fi
