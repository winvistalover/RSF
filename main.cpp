#include "System.h"
#include <unordered_map>
#include <regex>
#include <thread>
using namespace System;
using namespace System::Strings;
using namespace System::IO;


std::unordered_map<std::string, int> intVariables;
std::unordered_map<std::string, std::string> stringVariables;
std::unordered_map<std::string, std::vector<std::string>> Functions;

enum class LineType { VAR_INT, VAR_STR, PRINT, GETCIN, STOPP, RETURN, IF, WAIT, GO, NONE};

std::regex varIntRegex(R"(^\s*var\s+(\w+)\s*=\s*(-?\d+)\s*$)");  
std::regex varStrRegex(R"(^\s*var\s+(\w+)\s*=\s*\"([^\"]+)\"\s*$)");  
std::regex waitRegex(R"(^\s*wait*\(\s*(\d+)\)\s*$)");
std::regex goRegex(R"(^\s*([a-zA-Z_]\w*)\s*\(\)\s*$)");
std::regex StopParseRegex(R"(^\s*stopparse\s*<<\s*([\w]+)\s*>?\s*$)");
std::regex printRegex(R"(^\s*print\(+(.*)\)\s*$)"); 
std::regex getCinRegex(R"(^\s*getcin\(*(\w+)\)\s*$)"); 
std::regex returnRegex(R"(^\s*<-\s*([a-zA-Z_]\w*)\s*$)");
std::regex ifRegex(R"(^\s*if\s+\(([a-zA-Z0-9_]+)\)\s*(==|!=|<|>|<=|>=)\s*\"([^\"]+)\"\s*->\s*(\w+)\s*)");

LineType checkLineType(const std::string& line) {
    if (std::regex_match(line, varIntRegex)) return LineType::VAR_INT;
    if (std::regex_match(line, varStrRegex)) return LineType::VAR_STR;
    if (std::regex_match(line, printRegex)) return LineType::PRINT;
    if (std::regex_match(line, waitRegex)) return LineType::WAIT;
    if (std::regex_match(line, goRegex)) return LineType::GO;
    if (std::regex_match(line, getCinRegex)) return LineType::GETCIN;
    if (std::regex_match(line, ifRegex)) return LineType::IF;
    if (std::regex_match(line, StopParseRegex)) return LineType::STOPP;
    if (std::regex_match(line, returnRegex)) return LineType::RETURN;
    return LineType::NONE;
}

void parseRSF(const std::vector<std::string>& Lines) {
    bool inFunction = false;

    for (const std::string& line : Lines) {
        switch (checkLineType(line)) {
            case LineType::VAR_INT: {
                std::smatch match;
                if (std::regex_match(line, match, varIntRegex)) {
                    intVariables[match[1]] = std::stoi(match[2]);
                }
                break;
            }

            case LineType::IF: {
                std::smatch match;
                if (std::regex_match(line, match, ifRegex)) {
                    std::string variableName = match[1];
                    std::string operatorSymbol = match[2]; 
                    std::string comparisonValue = match[3]; 
                    std::string functionToGoTo = match[4];  
                    std::string variableValue = stringVariables[variableName];
            
            
                    bool conditionMet = false;
            
                    if (operatorSymbol == "==") {
                        conditionMet = (variableValue == comparisonValue);
                    } else if (operatorSymbol == "!=") {
                        conditionMet = (variableValue != comparisonValue);
                    } else if (operatorSymbol == "<") {
                        conditionMet = (variableValue < comparisonValue);
                    } else if (operatorSymbol == ">") {
                        conditionMet = (variableValue > comparisonValue);
                    } else if (operatorSymbol == "<=") {
                        conditionMet = (variableValue <= comparisonValue);
                    } else if (operatorSymbol == ">=") {
                        conditionMet = (variableValue >= comparisonValue);
                    }
            
                    if (conditionMet) {
                        
                        for (const auto& func : Functions)
                        {
                            if (func.first == functionToGoTo)
                            {
                                std::vector<std::string> functionLines = func.second; 
                                parseRSF(functionLines);
                                break;
                            }
                        }
                    }
                }
                break;
            }
            
            
            case LineType::GETCIN: {  
                std::smatch match;
                if (std::regex_match(line, match, getCinRegex)) {
                    std::string varName = match[1];
                    std::cout << "> ";
                    std::string userInput;
                    std::getline(std::cin, userInput);
                    if (intVariables.find(varName) != intVariables.end()) {
                        intVariables[varName] = std::stoi(userInput);  
                    } else if (stringVariables.find(varName) != stringVariables.end()) {
                        stringVariables[varName] = userInput; 
                    } else {
                        stringVariables[varName] = userInput; 
                    }
                }
                break;
            }
            case LineType::RETURN: {  
                std::smatch match;
                if (std::regex_match(line, match, returnRegex)) {
                    std::string varName = "return";
                    if (intVariables.find(varName) != intVariables.end()) {
                        intVariables[varName] = std::stoi(match[1]); 
                    } else if (stringVariables.find(varName) != stringVariables.end()) {
                        stringVariables[varName] = match[1]; 
                    } else {
                        stringVariables[varName] = match[1]; 
                    }
                }
                break;
            }

            case LineType::VAR_STR: {
                std::smatch match;
                if (std::regex_match(line, match, varStrRegex)) {
                    stringVariables[match[1]] = match[2];
                }
                break;
            }

            case LineType::WAIT: {
                std::smatch match;
                if (std::regex_match(line, match, waitRegex)) {
                    int waitTime = std::stoi(match[1]);  
                    std::this_thread::sleep_for(std::chrono::milliseconds(waitTime)); 
                }
                break;
            }
            case LineType::STOPP: {
                std::smatch match;
                if (std::regex_match(line, match, StopParseRegex)) {
                    std::string varName = match[1];
                    int code = intVariables[varName];  
                    cout << "Stop parse with code: " << code; 
                    exit(code);
                }
                break;
            }

            case LineType::GO: {
                std::smatch match;
                if (std::regex_match(line, match, goRegex)) { 
                    std::string functionName = match[1].str();
                    for (const auto& func : Functions)
                    {
                        if (func.first == functionName)
                        {
                            std::vector<std::string> functionLines = func.second; 
                            parseRSF(functionLines);
                            break;
                        }
                    }
                }
                break;
            }

            case LineType::PRINT: {
                std::smatch match;
                if (std::regex_match(line, match, printRegex)) {
                    std::string printStatement = match[1];  
                    std::istringstream stream(printStatement);
                    std::string part;
                    bool firstPart = true;

                    while (std::getline(stream, part, '&')) {
                        part = std::regex_replace(part, std::regex("^\\s+|\\s+$"), "");

                        if (part == "") continue;

                        if (part[0] == '(' && part.back() == ')') {
                            std::string varName = part.substr(1, part.size() - 2);
                            if (intVariables.find(varName) != intVariables.end()) {
                                std::cout << intVariables[varName];
                                firstPart = false;
                            } else if (stringVariables.find(varName) != stringVariables.end()) {
                                std::cout << stringVariables[varName];
                                firstPart = false;
                            }
                        } else {
                            String out = part;
                            out.Remove("\"");
                            std::cout << out;
                            firstPart = false;
                        }
                        std::cout.flush();  
                    }
                }
                break;
            }

            case LineType::NONE:
                break;
        }
    }
}
void ParseFunctionsFromLines(const std::vector<std::string>& lines) {
    std::regex functionRegex(R"(^\s*void\s+([a-zA-Z_]\w*)\s*\(\)\s*$)");

    for (size_t i = 0; i < lines.size(); ++i) {
        std::smatch match;
        if (std::regex_match(lines[i], match, functionRegex)) { 
            std::string functionName = match[1].str();
            size_t start = i + 1;
            size_t end = start;
            int braceCount = 0;
            bool foundOpenBrace = false;

            while (end < lines.size()) {
                if (lines[end].find("{") != std::string::npos) {
                    braceCount++;
                    foundOpenBrace = true;
                }
                if (lines[end].find("}") != std::string::npos) {
                    braceCount--;
                }
                if (foundOpenBrace && braceCount == 0) {
                    break;
                }
                end++;
            }

            if (braceCount == 0 && foundOpenBrace) {
                std::vector<std::string> functionLines(lines.begin() + start + 1, lines.begin() + end);

                Functions[functionName] = functionLines;
                i = end; 
            }
        }
    }
}


void ParseLines(std::vector<std::string>& Lines)
{ 
    for (size_t i = 0; i < Lines.size(); ++i) {
        if (Lines[i].find("append") == 0) {
            std::string appendFile = Lines[i].substr(7); 
            appendFile.erase(appendFile.find_last_not_of("\r\n") + 1);
            
            std::vector<std::string> additionalLines = ReadAllLines(appendFile);
            ParseLines(additionalLines);
            ParseFunctionsFromLines(additionalLines);
            for (const auto& function : Functions) {
                const std::string& functionName = function.first;
                const std::vector<std::string>& functionLines = function.second;
        
                if (functionName == "_init") {
                    parseRSF(functionLines); 
                }
            }
            Lines[i].erase(Lines[i].begin()); 
        }
    }
}

class Program
{
    public: 
    void err(const int& bin)
    {
        switch (bin)
        {
        case 0x0:
            exit(bin);
        case 0x1:
            Console::WriteLine("RSF file doesn't exist. Exiting...");
            exit(bin);
        default:
            Console::WriteLine("Unknown error. Exiting...");
            exit(bin);
        } 
    }
    void main(int argc, char* argv[]) 
    {
        string filename = "";
        if (argc > 0) 
        {
            filename = argv[1];
        } else {
            filename = "./main.rsf";
        }
        cout << "Really Stupid Format" << endl;
        cout << "Copyright © 2025 Potato Lover" << endl;
        cout << "Reading " << filename << "..." << endl;
        if (!filesystem::exists(filename)) 
        {
            err(0x1);
        }
        vector<string> Lines = ReadAllLines(filename);
        stringVariables["endl"] = "\n";
        stringVariables["cr"] = "\r";
        
        ParseLines(Lines);
    
        
        // Step 2: Parse functions from the main file (test.rsf)
        ParseFunctionsFromLines(Lines);
    
        // Step 3: Execute the main function
        for (const auto& function : Functions) {
            const std::string& functionName = function.first;
            const std::vector<std::string>& functionLines = function.second;
    
            if (functionName == "main") {
                parseRSF(functionLines); 
            }
        }
    
    
    }
};

int main(int argc, char* argv[]) 
{
    Program program;
    program.main(argc, argv);
}