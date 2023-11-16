//—————————————————————————————————————————————————————————————————————————————————————\\
// ᴠᴛᴇsᴛ                                                                               \\
// ‾‾‾‾‾                                                                               \\
// Made by : Motisma479                                                                \\
// Source code : https://github.com/Motisma479/VTest                                   \\
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
//    FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE      \\
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
    []() {testName = #name;lastId++;}();\

#define NAMESPACE(name) \
    for (NameSpaceGuard nameSpaceGuard(#name); nameSpaceGuard.isActive(); nameSpaceGuard.setInactive()) \
    
#define VTEST(name) \
    void Test_##name(); \
    struct TestRegistrer_##name \
    { \
        TestRegistrer_##name() \
        { \
            RegisterVTest(#name, Test_##name); \
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
            RegisterResult(testName,lastId, {true, "       Test passed: " #expression}); \
        } \
    } \
    catch (const std::exception& error) \
    { \
        RegisterResult(testName,lastId,{false, std::string("       ") + error.what()}); \
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
            RegisterResult(testName, lastId,{true, "       Test passed: " #result}); \
        } \
    } \
    catch (const std::exception& error) \
    { \
        RegisterResult(testName,lastId,{false, std::string("       ") + error.what()}); \
    } \
} while (false)

//-----------------------------------------------------------------------------
constexpr const char* RED = "\033[38;2;242;96;103m";
constexpr const char* GREEN = "\033[38;2;74;255;120m";
constexpr const char* BLUE = "\033[38;2;44;158;243m";
constexpr const char* PURPLE = "\033[38;2;170;142;214m";
constexpr const char* DEFAULT = "\033[0m";

struct Entry;
struct VTest;
Entry& GetEntryToPush(Entry& inEntry);

std::vector<VTest> VTests = {};

//-----------------------------------------------------------------------------
// Used to correctly register the Entry
//-----------------------------------------------------------------------------

int lastId = 0;
int lastNamespaceId = 0;
std::string testName = "";

//-----------------------------------------------------------------------------
// Used to keep track of test results
//-----------------------------------------------------------------------------

int passed = 0;
int failed = 0;

//-----------------------------------------------------------------------------

struct testResult
{
    bool result;
    std::string expression;
};
enum EntryType
{
    ERROR,
    NAMESPACE,
    TEST
};
struct Entry
{
    EntryType type = ERROR; // Use to identify the type of entry
    std::string name = "";
    int id = -1; // if -1 it's an error
    std::vector<Entry> subEntry = {};

    //related to TEST
    std::vector<testResult> results = {};
    bool asError = false;
};
struct VTest
{
    std::function<void()> function;
    Entry entry;
};
class NameSpaceGuard
{
public:
    NameSpaceGuard(const char* _name): active(true)
    {
        lastId++;
        GetEntryToPush(VTests.back().entry).subEntry.push_back({NAMESPACE, _name, lastId, {}});
        preId = lastNamespaceId;
        lastNamespaceId = lastId;
    }

    ~NameSpaceGuard()
    {
        lastNamespaceId= preId;
    }

    bool isActive() const { return active; }
	void setInactive() { active = false; }

private:
	bool active;
    int preId = 0;
};

Entry errorEntry; // an error entry to be used in case of error
Entry* lastTestEntry = &errorEntry; // Ref to the last test entry so it could be updated
Entry& GetEntryToPush(Entry& inEntry)
{
    if(inEntry.id == lastNamespaceId)
        return inEntry;
    
    for(auto& i : inEntry.subEntry)
    {
        Entry& outEntry = GetEntryToPush(i);
        if(outEntry.type != ERROR)
            return outEntry;
    }

    return errorEntry;
}

//-------------------------------------------------------------
// Working but not used. Could be useful if fully integrated
//-------------------------------------------------------------
// Entry& GetEntryById(Entry& inEntry, int id)
// {
//     if(inEntry.id == id)
//         return inEntry;
//
//     for(auto& i : inEntry.subEntry)
//     {
//         Entry& outEntry = GetEntryById(i,id);
//         if(outEntry.type != ERROR)
//             return outEntry;
//     }
//
//     return errorEntry;
// }

void RegisterResult(const std::string& _testName, const int& _id, testResult _results)
{
    if(_results.result)
        passed++;
    else
        failed++;

    if (lastTestEntry->id == _id)
    {
        lastTestEntry->results.push_back(_results);
        if (!lastTestEntry->asError)
            lastTestEntry->asError = !_results.result;
        return;
    }
     
    std::vector<testResult> temp;
    temp.push_back(_results);
    Entry& newEntry = GetEntryToPush(VTests.back().entry);
    newEntry.subEntry.push_back({ TEST, _testName, _id,{},temp,!_results.result});
    lastTestEntry = &newEntry.subEntry.back();
}

void RegisterVTest(const char* name ,const std::function<void()>& function)
{
    VTests.insert(VTests.begin(), {function, {NAMESPACE, name, lastId, {}}});
}

void Draw(Entry entry, std::vector<int> lastLineAt = {}, int recurrence = 0)
{
    if(recurrence == 0) // Draw the origin of execution
    {
        std::string hor, extraSpace;
        if (entry.name.size() < 5)
        {
            for (int i = 0; i < 2; i++)
                hor += "\xe2\x94\x81";
            for (int i = entry.name.size(); i < 5; i++)
                extraSpace += ' ';
        }
        else
        {
            for (int i = 0; i < entry.name.size()-3; i++)
                hor += "\xe2\x94\x81";
        }

        std::cout << "\xe2\x94\x8f\xe2\x94\x81\xe2\x94\x81\xe2\x94\x81" << hor << "\xe2\x94\x93" << std::endl;
        std::cout << "\xe2\x94\x83" << PURPLE << entry.name << DEFAULT << extraSpace << "\xe2\x94\x83" << std::endl;
        std::cout << "\xe2\x94\x97\xe2\x94\x81\xe2\x94\x81\xe2\x94\xaf"<< hor <<"\xe2\x94\x9b\n";
    }

    std::string tab = "";
    for (int i = 0; i < recurrence; i++)
    {
        auto checkIfBlank = [i,lastLineAt](){for(const auto& check : lastLineAt){ if(check == i){return true;}}return false;};
        if(checkIfBlank())
            tab += "       ";
        else
            tab += "   \xe2\x94\x82   "; // "    │   "
    }
        
    Entry lastEntry = entry.subEntry.back();
    for (const auto& subEntry : entry.subEntry)
    {
        const char* caseTab = "";
        if (subEntry.id == lastEntry.id)
            caseTab = "   \xe2\x94\x94\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80"; // "    └───"
        else
            caseTab = "   \xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80"; // "    ├───"

        if (subEntry.type == NAMESPACE)
        {
            std::cout << tab << caseTab << '[' << BLUE << subEntry.name << DEFAULT << "]\n";

            if (subEntry.id == lastEntry.id)
                lastLineAt.push_back(recurrence);
                
            Draw(subEntry, lastLineAt, recurrence + 1);
        }
        else
        {
            if (subEntry.asError)
            {
                std::cout << tab << caseTab << "[" << RED << "FAIL" << DEFAULT << "] " << subEntry.name << "\n";

                caseTab = subEntry.id == lastEntry.id ? "       " : "   \xe2\x94\x82   "; // "    │   "
                for (const auto& r : subEntry.results)
                {
                    auto color = r.result == true ? GREEN : RED;
                    std::cout << tab << caseTab << color << r.expression << DEFAULT << "\n";
                }
            }
            else
            {
                std::cout << tab << caseTab << "[" << GREEN << "PASS" << DEFAULT << "] " << subEntry.name << "\n";
            }
        }
    }
}

template<typename... Args>
void runTests(Args... names) {
    std::system("chcp 65001");
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    std::cout << "#############################################################################################\n";
    for (int v = VTests.size()-1; v > -1; v--)
    {
        bool shouldContinue = true;
        if(sizeof...(names) > 0)
        {
            for (const auto& name : std::initializer_list<std::string>{names...})
            {
                shouldContinue = false;
                if(VTests[v].entry.name == name)
                {
                    shouldContinue = true;
                    break;
                }
            }
        }
        if(shouldContinue)
        {
            VTests[v].function();
            Draw(VTests[v].entry);
            

            lastId = 0;
            lastNamespaceId = 0;
            testName = "";
            lastTestEntry = &errorEntry;
        }
        VTests.pop_back();
    }
    std::cout << "#############################################################################################\n";
    std::cout << "Tests run: " << passed + failed << ", Passed: " << passed << ", Failed: " << failed << std::endl;
}