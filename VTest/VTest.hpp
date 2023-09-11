//—————————————————————————————————————————————————————————————————————————————————————\\
// ᴠᴛᴇsᴛ                                                                               \\
// ‾‾‾‾‾                                                                               \\
// Made by : Motisma479                                                                \\
// Source code : https://github.com/Motisma479/Violet                                  \\
// License : MIT License                                                               \\
// {                                                                                   \\
//    Copyright (c) 2023 Mathieu Robion                                                \\
//                                                                                     \\
//    Permission is hereby granted, free of charge, to any person obtaining a copy     \\
//    of this software and associated documentation files (the "Software"), to deal    \\
//    in the Software without restriction, including without limitation the rights     \\
//    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell        \\
//    copies of the Software, and to permit persons to whom the Software is            \\
//    furnished to do so, subject to the following conditions:                         \\
//                                                                                     \\
//    The above copyright notice and this permission notice shall be included in all   \\
//    copies or substantial portions of the Software.                                  \\
//                                                                                     \\
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR       \\
//    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,         \\
//    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE      \\
//    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER           \\
//    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,    \\
//    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE    \\
//    SOFTWARE.                                                                        \\
// }                                                                                   \\
//—————————————————————————————————————————————————————————————————————————————————————\\

#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include <string>

#include <chrono>
#include <thread>

#define TEST(name) \
    void Test_##name(); \
    struct TestRegistrer_##name \
    { \
        TestRegistrer_##name() \
        { \
            registerTestCase(nameSpaces, #name, Test_##name); \
        } \
    } TestRegistrerInstance_##name; \
    void Test_##name()
//-----------------------------------------------------------------------------
#define REQUIRE(expression) do \
{ \
    try \
    { \
        if (!(expression)) \
        { \
            throw std::runtime_error("Test failed: " #expression); \
        } \
        else \
        { \
            results.push_back({true, "\tTest passed: " #expression}); \
        } \
    } \
    catch (const std::exception& error) \
    { \
        asError = true; \
        results.push_back({false, std::string("\t") + error.what()}); \
    } \
} while (false)

#define COMPARE(result, expectation) do \
{ \
    try \
    { \
        if (result != expectation) \
        { \
            throw std::runtime_error(std::string("Test failed: ") + #result + std::string(" expected ") + std::to_string(expectation) + std::string(", but got ") + std::to_string(result)); \
        } \
        else \
        { \
            results.push_back({true, "\tTest passed: " #result}); \
        } \
    } \
    catch (const std::exception& error) \
    { \
        asError = true; \
        results.push_back({false, std::string("\t") + error.what()}); \
    } \
} while (false)
//-----------------------------------------------------------------------------
#define NAMESPACE(name) \
    struct NameSpaceRegistrer_##name { \
        NameSpaceRegistrer_##name() \
        { \
            nameSpaces.push_back(#name); \
        } \
    } NameSpaceRegistrerInstance_##name; \

#define END_NAMESPACE(name) \
    struct EndNameSpaceRegistrer_##name \
    { \
        EndNameSpaceRegistrer_##name() \
        { \
            nameSpaces.pop_back(); \
        } \
    } EndNameSpaceRegistrerInstance_##name;
//-----------------------------------------------------------------------------
struct testCase;
struct testResult;
struct nameSpace;

constexpr const char* RED = "\033[38;2;242;96;103m";
constexpr const char* GREEN = "\033[38;2;74;255;120m";
constexpr const char* BLUE = "\033[38;2;44;158;243m";
constexpr const char* DEFAULT = "\033[0m";

std::vector<nameSpace> nameSpaceSorter;
std::vector<testResult> results;
std::vector<std::string> nameSpaces;
int numberOfCase = 0;
bool asError = false;
int passed = 0;
int failed = 0;
//-----------------------------------------------------------------------------
struct testCase
{
    std::string name;
    std::function<void()> function;
    int id;

};
//-----------------------------------------------------------------------------
struct testResult
{
    bool result;
    std::string expression;
};
//-----------------------------------------------------------------------------
struct nameSpace
{
    std::string name;
    int tabs;
    std::vector<testCase> testCases;
    std::vector<nameSpace> subNameSpace;
    void AddCase(std::vector<std::string> nameSpaces, const std::string& name, const std::function<void()>& testFunction, int id);
    void Draw(bool isLast = false, bool isFinal = false) const;
};

void nameSpace::AddCase(std::vector<std::string> nameSpaces, const std::string& name, const std::function<void()>& testFunction, int id)
{
    if (nameSpaces.size() > tabs + 1)
    {
        if (subNameSpace.size() != 0 && subNameSpace.back().name == nameSpaces[tabs + 1])
        {
            subNameSpace.back().AddCase(nameSpaces, name, testFunction, id);
            return;
        }
        subNameSpace.push_back({ nameSpaces[tabs + 1], tabs + 1 });
        subNameSpace.back().AddCase(nameSpaces, name, testFunction, id);
        return;
    }
    testCases.push_back({ name, testFunction , id });
}
void nameSpace::Draw(bool isLast, bool isFinal) const
{
    std::string tab = "";
    std::string caseTab = "";
    int preId = 0;

    for (int i = 0; i < tabs - 1; i++)
    {
        tab += isFinal == true ? "        " : "    \xe2\x94\x82   "; // "    │   "
    }
    if (tabs != 0)
    {
        if (isLast && (tabs > 1 || isFinal))
        {
            tab += "    \xe2\x94\x94\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80"; // "    └───"
        }
        else
            tab += "    \xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80"; // "    ├───"
    }

    if (name != "__NO_NAMESPACE_FOUND")
    {
        std::cout << tab << '[' << BLUE << name << DEFAULT << "]\n";
        if (isLast && tabs > 0)
        {
            tab = isFinal == true ? "        " : "    \xe2\x94\x82   "; // "    │   "
            
            for (int i = 1; i < tabs; i++)
            {
                tab += "        ";
            }
        }
        else
        {
            tab = "";
            if (isFinal)
            {
                for (int i = 0; i < tabs - 1; i++)
                {
                    tab += "        ";
                }
                tab += "    \xe2\x94\x82   "; // "    │   "
            }
            else
                for (int i = 0; i < tabs; i++)
                {
                    tab += "    \xe2\x94\x82   "; // "    │   "
                }
        }
    }
    for (const auto& testCase : testCases)
    {
        if (name != "__NO_NAMESPACE_FOUND")
        {
            if (testCase.id == testCases.back().id && (tabs > 0 || isLast))
                caseTab = "    \xe2\x94\x94\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80"; // "    └───"
            else if (name != "__NO_NAMESPACE_FOUND")
                caseTab = "    \xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80"; // "    ├───"
        }
        if (testCase.id == preId + 1 || preId == 0)
        {
            asError = false;
            results.clear();
            testCase.function();

            if (!asError) {
                std::cout << tab << caseTab << "[" << GREEN << "PASS" << DEFAULT << "] " << testCase.name << "\n";
                passed++;
            }
            else
            {
                std::cout << tab << caseTab << "[" << RED << "FAIL" << DEFAULT << "] " << testCase.name << "\n";
                for (const auto& result : results) {
                    auto color = result.result == true ? GREEN : RED;
                    std::string suptab;
                    if (tab == "")
                        suptab = "";
                    else if (isLast)
                        suptab = "        ";
                    else
                        suptab = "    \xe2\x94\x82   "; // "    │   "


                    std::cout << tab << suptab << color << result.expression << DEFAULT << "\n";
                }
                failed++;
            }

            preId = testCase.id;
        }
        else break;
    }
    for (int i = 0; i < subNameSpace.size(); i++)
    {
        bool checkFinal = false;

        if (i == subNameSpace.size() - 1 && tabs >= 0 && isLast)
            checkFinal = true;
        if (subNameSpace[i].testCases.size() > 0 && testCases.size() > 0 && subNameSpace[i].testCases.back().id != testCases.back().id)
            checkFinal = false;

        if (i == subNameSpace.size() - 1 && (tabs > 0 || isLast))
            subNameSpace[i].Draw(true, checkFinal);
        else
            subNameSpace[i].Draw(false, isFinal);

        if (subNameSpace[i].testCases.size() > 0)
            preId = subNameSpace[i].testCases.back().id;
        for (const auto& testCase : testCases)
        {
            if (testCase.id == testCases.back().id && (tabs > 0 || isLast))
                caseTab = "    \xe2\x94\x94\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80"; // "    └───"
            else
                caseTab = "    \xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80"; // "    ├───"
            if (testCase.id == preId + 1)
            {
                asError = false;
                results.clear();
                testCase.function();

                if (!asError) {
                    std::cout << tab << caseTab << "[" << GREEN << "PASS" << DEFAULT << "] " << testCase.name << "\n";
                    passed++;
                }
                else
                {
                    std::cout << tab << caseTab << "[" << RED << "FAIL" << DEFAULT << "] " << testCase.name << "\n";
                    for (const auto& result : results) {
                        auto color = result.result == true ? GREEN : RED;
                        std::cout << tab << color << result.expression << DEFAULT << "\n";
                    }
                    failed++;
                }

                preId = testCase.id;
            }
        }
    }
}
//-----------------------------------------------------------------------------

void registerTestCase(std::vector<std::string> nameSpaces, const std::string& name, const std::function<void()>& testFunction)
{
    if (nameSpaces.size() == 0)
    {
        nameSpaceSorter.push_back({ "__NO_NAMESPACE_FOUND", 0 });
        nameSpaceSorter.back().AddCase(nameSpaces, name, testFunction, numberOfCase);
    }
    else if (nameSpaceSorter.size() == 0 || nameSpaces[0] != nameSpaceSorter.back().name)
    {
        nameSpaceSorter.push_back({ nameSpaces[0], 0 });
        nameSpaceSorter.back().AddCase(nameSpaces, name, testFunction, numberOfCase);
    }
    else
        nameSpaceSorter.back().AddCase(nameSpaces, name, testFunction, numberOfCase);

    numberOfCase++;
}

void runTests() {
    std::system("chcp 65001");
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    std::cout << "#############################################################################################\n";
    for (int i = 0; i < nameSpaceSorter.size(); i++)
    {
        if (i == nameSpaceSorter.size() - 1)
            nameSpaceSorter[i].Draw(true);
        else
            nameSpaceSorter[i].Draw();
    }
    std::cout << "#############################################################################################\n";
    std::cout << "Tests run: " << passed + failed << ", Passed: " << passed << ", Failed: " << failed << std::endl;
}
