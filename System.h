#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
using namespace std;

namespace System
{
    namespace Exceptions
    {
        class FileAlreadyOpened : public exception
        {
        public:
            const char* what() const noexcept override
            {
                return "The file is already opened.";
            }
        };
        class FileDoesNotExist : public exception
        {
        public:
            const char* what() const noexcept override
            {
                return "The file does not exist.";
            }
        };
        class UnknownFileError : public exception
        {
        public:
            const char* what() const noexcept override
            {
                return "Unknown file error.";
            }
        };
    }
    namespace IO
    {
        string WorkingDirectory()
        {
            return filesystem::current_path().string();
        }
        string JoinWorking(const string& FileName)
        {
            return (filesystem::current_path() / FileName).string();
        }
        bool FileExists(const string& FileName)
        {
            errno = 0;
            ifstream file(FileName);
            if (file.is_open())
            {
                file.close(); 
                return true;  
            }
            else
            {
                if (errno == ENOENT)
                {
                    return false;
                }

                return true;
            }
        }
        vector<string> ReadAllLines(const string& FileName)
        {
            vector<string> lines;
            ifstream file(FileName); 

            if (file.is_open())
            {
                string line;
                while (getline(file, line)) 
                {
                    lines.push_back(line); 
                }
                file.close();  
            }
            else
            {
                throw Exceptions::UnknownFileError();
            }

            return lines;
        }
    }
    namespace Strings
    {
        bool Contains(const string& What, const string& Inside)
        {
            return What.find(Inside) != string::npos;
        }
        class String : public string
        {
        public:
            String() : std::string() {}
            String(const char* str) : std::string(str) {}
            String(const std::string& s) : std::string(s) {}
            bool Contains(const std::string& subStr) const {
                return this->find(subStr) != std::string::npos;
            }
            void Replace(const std::string& oldStr, const std::string& newStr) {
                size_t startPos = 0;
                while ((startPos = this->find(oldStr, startPos)) != std::string::npos) {
                    this->replace(startPos, oldStr.length(), newStr);
                    startPos += newStr.length(); // Move past the newly replaced substring
                }
            }
        
            void Remove(const std::string& subStr) {
                size_t startPos = 0;
                while ((startPos = this->find(subStr, startPos)) != std::string::npos) {
                    this->erase(startPos, subStr.length());
                }
            }
        };
    }
    namespace Console
    {
        struct ConsoleKey
        {
            char KeyChar;
            // Will expand
        };
        
        void WriteLine(const string& Message)
        {
            cout << Message << endl;
        }
        void Write(const string& Message)
        {
            cout << Message;
        }
        ConsoleKey ReadKey()
        {
            ConsoleKey key;
            key.KeyChar = cin.get();
            return key;
        }
        string ReadLine()
        {
            string input;
            getline(cin, input);
            return input;
        }
    }
}
