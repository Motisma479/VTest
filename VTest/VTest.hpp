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
//    FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE     \\
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
#include <cstring>
#include <string>
#include <iomanip>
#include <chrono>

//weird msvc behavior exception
#ifdef _MSC_VER
#define VA_ARGS(...) , __VA_ARGS__
#else
#define VA_ARGS(...) __VA_OPT__(,) __VA_ARGS__ 
#endif

//-VTEST-----------------------------------------------------------------------
/**
 * @brief Register a new Unit Test block.
 *
 * @param name The unic-name of the unit Test block.
 */
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



//-NAMESPACE-------------------------------------------------------------------
/**
 * @brief Register a namespace/sub-group of tests.
 *
 * @param name The name of the namespace.
 * @param __VA_ARGS__ a possible list of tags like "tag1, tag2".
*/
#define NAMESPACE(name, ...) \
    for (NameSpaceGuard nameSpaceGuard(#name VA_ARGS(__VA_ARGS__)); nameSpaceGuard.isActive(); nameSpaceGuard.setInactive())
//-----------------------------------------------------------------------------



//-TEST------------------------------------------------------------------------
/**
 * @brief Register a test.
 *
 * If all the requirements are met, the test will be marked as PASS else it will be marked as FAIL.
 * 
 * @param name The name of the namespace.
 * @param __VA_ARGS__ a possible list of tags like "tag1, tag2".
*/
#define TEST(name, ...) \
    for( TestGuard testGuard(#name VA_ARGS(__VA_ARGS__)); testGuard.isActive(); testGuard.update())
//-----------------------------------------------------------------------------

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


#define COMPARE_WITH_PRECISION(result, expectation, precision) do \
{ \
    try \
    { \
        if ([&](){auto r = result - expectation; if(r<0) r = -r; return r > precision;}()) \
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



//-ArgsGestureFunction---------------------------------------------------------

template<typename T, typename... Args>
T GetArgAtIndex(int index, T defaultSolution, Args... args)
{
    T result = defaultSolution;
    
    if(index <= sizeof...(args)+1)
    {
        int i = 0;
        for (const auto& arg : std::initializer_list<T>{args...})
        {
            i++;
            result = arg;
            if (i == index)
                break;
        }
    }
    return result;
}

//-----------------------------------------------------------------------------

constexpr const char* RED = "\033[38;2;242;96;103m";
constexpr const char* GREEN = "\033[38;2;74;255;120m";
constexpr const char* BLUE = "\033[38;2;44;158;243m";
constexpr const char* PURPLE = "\033[38;2;170;142;214m";
constexpr const char* YELLOW = "\033[38;2;237;216;40m";
constexpr const char* LIGHTGRAY = "\033[38;2;100;100;100m";
constexpr const char* DEFAULT = "\033[0m";

struct Entry;
struct VTest;
std::vector<std::string> GetTagList(const char* tagListBrut);
void SaveLastDuration(uint64_t duration);
void SaveTagList(const std::vector<std::string>& tagList);
void PopTagList();
bool IsTagged(Entry& entry);

std::vector<VTest> VTests = {}; //List of all tests

//-----------------------------------------------------------------------------


//-Used to correctly register the Entry----------------------------------------

int lastId = 0;
int lastNamespaceId = 0;
std::string testName = "";
std::vector<std::string> TagsList;

//-----------------------------------------------------------------------------


//-Used to keep track of test results------------------------------------------

int passed = 0;
int failed = 0;

//-----------------------------------------------------------------------------


//-Used if we want to check if a tag is present in the list--------------------

bool shouldCheckTagList = false;
std::vector<std::string> tagListToCheck;

//-----------------------------------------------------------------------------


//-Used to keep track of the number of iterations of a test--------------------

int iterationValue = 1;
bool asDoneFirstIter = false; // used to not register the results after the first iteration

//-----------------------------------------------------------------------------


//-Entry related---------------------------------------------------------------

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
    std::string name = "";  // Name of the entry
    int id = -1;            // if -1 it's an error
    std::vector<Entry> subEntry = {};

    //-related to TEST--------------------------
    std::vector<testResult> results = {};
    bool asError = false;

    uint64_t duration = 0; // in nanosecond
    uint8_t longestNameLength = 0;

    //-related to Stream---------------
    std::string capturedOutput = "";
    bool asCapturedOutput = false;
    //---------------------------------

    //------------------------------------------
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

//-----------------------------------------------------------------------------


//Used to capture the output of std::cout
class CustomStreamBuf : public std::streambuf {
public:
    CustomStreamBuf(std::streambuf* originalBuf) : originalBuf(originalBuf) {}

    // Function to retrieve the captured output
    std::string getCapturedOutput() const {
        return capturedOutput;
    }

protected:
    // This is called when new characters are output
    virtual int overflow(int c) override {
        if(asDoneFirstIter) return c;
        lastTestEntry->asCapturedOutput = true;

        // Add the character to the captured string
        if (c != EOF) {
            capturedOutput += static_cast<char>(c);
        }
        return c; // Return the character as processed
    }

private:
    std::streambuf* originalBuf;  // The original buffer of std::cout
    std::string capturedOutput;   // Variable to store the output
};


//-VTest related---------------------------------------------------------------

//The struct that contains the function and the entries of a test section
struct VTest
{
    std::function<void()> function;
    Entry entry;
};

void RegisterVTest(const char* name ,const std::function<void()>& function)
{
    VTests.insert(VTests.begin(), {function, {NAMESPACE, name, lastId, {}}});
}

//-----------------------------------------------------------------------------


//-These classes are used to keep track of the namespace and tests-------------

class NameSpaceGuard
{
public:
    template<typename... Args>
    NameSpaceGuard(const char* _name, Args... args): active(true)
    {
        lastId++;
        GetEntryToPush(VTests.back().entry).subEntry.push_back({NAMESPACE, _name, lastId, {}});
        // SaveTagList(lastId, GetTagList(GetArgAtIndex(1, "", args...)));
        
        std::vector<std::string> tagList = GetTagList(GetArgAtIndex<const char*>(1, "", args...));
        SaveTagList(tagList);
        registeredTag = tagList.size();


        preId = lastNamespaceId;
        lastNamespaceId = lastId;
    }

    ~NameSpaceGuard()
    {
        lastNamespaceId= preId;
        for(int i = 0; i < registeredTag; i++) PopTagList();
    }

    bool isActive() const { return active; }
	void setInactive() { active = false; }

private:
    int registeredTag = 0;
	bool active;
    int preId = 0;
};

class TestGuard
{
public:
    template<typename... Args>
    TestGuard(const char* _name, Args... args)
    {
        testName = _name;
        uint64_t testNameLength = testName.length();
        lastId++;

        std::vector<std::string> tagList = GetTagList(GetArgAtIndex<const char*>(1, "", args...));
        SaveTagList(tagList);
        registeredTag = tagList.size();

        asDoneFirstIter = false;
        iteration = 0;
        start = std::chrono::high_resolution_clock::now();

        originalBuf = std::cout.rdbuf();// Save the previous buffer;
        customBuf = new CustomStreamBuf(originalBuf);
        std::cout.rdbuf(customBuf);// Redirect std::cout to our custom streambuf

        //create a new entry     
        Entry& newEntry = GetEntryToPush(VTests.back().entry);
        newEntry.subEntry.push_back({ TEST, testName, lastId,{},{},false});
        
        if (newEntry.longestNameLength < testNameLength) newEntry.longestNameLength = testNameLength;

        lastTestEntry = &newEntry.subEntry.back();

        if(!IsTagged(*lastTestEntry))
        {
            isIgnored = true;
            iteration = iterationValue;

            newEntry.subEntry.pop_back();
        }
    }

    ~TestGuard()
    {
        if(!isIgnored)
        {
            if(lastTestEntry->asError)
                failed++;
            else
                passed++;
        }
        for(int i = 0; i < registeredTag; i++) PopTagList();

        lastTestEntry->capturedOutput = customBuf->getCapturedOutput(); // Save the capturedOutput
        std::cout.rdbuf(originalBuf); // Restore the previous buffer;

        delete customBuf;
    }

    bool isActive() const { return iteration < iterationValue; }

    void update()
    {
        std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();
        SaveLastDuration(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
        start = std::chrono::high_resolution_clock::now();
        asDoneFirstIter = true;
        iteration++;
    }

private:
    bool isIgnored = false;
    int registeredTag = 0;
    int iteration;
    std::chrono::time_point<std::chrono::high_resolution_clock> start;

    std::streambuf* originalBuf;
    CustomStreamBuf* customBuf;
};

//-----------------------------------------------------------------------------


void RegisterResult(const std::string& _testName, const int& _id, testResult _results)
{
    if(asDoneFirstIter) return;

    if (lastTestEntry->id == _id)
    {
        lastTestEntry->results.push_back(_results);
        if (!lastTestEntry->asError)
            lastTestEntry->asError = !_results.result;
    }
}

//Used for the VTest Square to suport UTF-8 why? because I can.
int CountCharsWithUtf8Support(const std::string& str)
{
    int count = 0;
    for (size_t i = 0; i < str.size(); ++i)
        if ((str[i] & 0xC0) != 0x80)
            ++count;
    return count;
}


//-Related to TagsList---------------------------------------------------------

std::vector<std::string> GetTagList(const char* tagListBrut)
{
    std::vector<std::string> tagList;
    std::string tag;
    for(int i = 0; i < std::strlen(tagListBrut); i++)
    {
        if(tagListBrut[i] == ' ') //ignore spaces
            continue;
        if(tagListBrut[i] == ',' || tagListBrut[i] == '|') // used to separate tags
        {
            tagList.push_back(tag);
            tag = "";
        }
        else

        tag += tagListBrut[i];
    }
    if(!tag.empty()) // used to put the last tag
        tagList.push_back(tag);

    return tagList;
}

void SaveTagList(const std::vector<std::string>& tagList)
{
    for(auto& i : tagList)
        TagsList.push_back(i);
}
void PopTagList()
{
    TagsList.pop_back();
}

bool IsTagged(Entry& entry)
{
    if(!shouldCheckTagList) return true;
            
    for (const auto& tag : TagsList) // search if entry as tag
    {
        for(const auto& tagToCheck : tagListToCheck) // search if entry has tag in common
        {
            if (tag == tagToCheck) 
            {
                return true;
            }
        }
    }
    return false;
}

//-----------------------------------------------------------------------------

//-Save the last duration in nanosecond----------------------------------------
void SaveLastDuration(uint64_t duration)
{
    lastTestEntry->duration += duration;
}
//-----------------------------------------------------------------------------

//Sanitize the output of the CustomStreamBuf
std::string OutputLNSanitizer(const std::string& str, const std::string& tab)
{
    std::string out = LIGHTGRAY + str;
    for (size_t i = 0; i < out.size()-1; ++i)
        if (out[i] == '\n')
            out.replace(i, 1, "\n" + std::string(DEFAULT) + tab + LIGHTGRAY);
    
    if(out[out.size()-1] != '\n')
        out += '\n';
    return out;
}


//-Draw sub function to lighten the draw function------------------------------
void Draw(Entry entry, std::vector<int> lastLineAt, int recurrence);

void DrawHead(std::string name)
{
    std::string hor, extraSpace;
    int charNumber = CountCharsWithUtf8Support(name);
    if (charNumber < 5)
    {
        for (int i = 0; i < 2; i++)
            hor += "\xe2\x94\x81";
        for (int i =  charNumber; i < 5; i++)
            extraSpace += ' ';
    }
    else
    {
        for (int i = 0; i < charNumber-3; i++)
            hor += "\xe2\x94\x81";
    }

    std::cout << "\xe2\x94\x8f\xe2\x94\x81\xe2\x94\x81\xe2\x94\x81" << hor << "\xe2\x94\x93" << std::endl;
    std::cout << "\xe2\x94\x83" << PURPLE << name << DEFAULT << extraSpace << "\xe2\x94\x83" << std::endl;
    std::cout << "\xe2\x94\x97\xe2\x94\x81\xe2\x94\x81\xe2\x94\xaf"<< hor <<"\xe2\x94\x9b\n";
}

void DrawTest(const Entry& subEntry, const Entry& lastEntry, std::string tab, std::string caseTab, int iterationValue, uint64_t durationPadding)
{
    std::cout << std::fixed << std::setprecision(6); //use 6 digits after the decimal point

    std::string durationPaddingText = "";
    for (int i = 0; i < durationPadding; i++)
    {
        durationPaddingText += ' ';
    }

    if (subEntry.asError)
    {
        std::cout << tab << caseTab << "[" << RED << "FAIL" << DEFAULT << "] " << subEntry.name << durationPaddingText << " \xe2\x94\x82 " << YELLOW << (subEntry.duration / iterationValue) * 1e-6 << " ms" << DEFAULT << "\n";

        caseTab = subEntry.id == lastEntry.id ? "       " : "   \xe2\x94\x82   "; // "    │   "
        for (const auto& r : subEntry.results)
        {
            auto color = r.result == true ? GREEN : RED;
            std::cout << tab << caseTab << color << r.expression << DEFAULT << "\n";
        }
    }
    else
    {
        std::cout << tab << caseTab << "[" << GREEN << "PASS" << DEFAULT << "] " << subEntry.name << durationPaddingText << " \xe2\x94\x82 " << YELLOW << (subEntry.duration / iterationValue) * 1e-6 << " ms" << DEFAULT << "\n";
    }

    if (subEntry.asCapturedOutput)
    {
        caseTab = subEntry.id == lastEntry.id ? "              " : "   \xe2\x94\x82          "; // "    │   "

        std::cout << tab << caseTab << OutputLNSanitizer(subEntry.capturedOutput, tab+caseTab) << DEFAULT;
    }
}

void DrawNameSpace(const Entry& subEntry, const Entry& lastEntry, std::string tab, std::string caseTab,std::vector<int> lastLineAt, int recurrence)
{
    std::cout << tab << caseTab << '[' << BLUE << subEntry.name << DEFAULT << "]\n";

    if (subEntry.id == lastEntry.id)
        lastLineAt.push_back(recurrence);
        
    Draw(subEntry, lastLineAt, recurrence + 1);
}

//-----------------------------------------------------------------------------

void Draw(Entry entry, std::vector<int> lastLineAt = {}, int recurrence = 0)
{
    if(recurrence == 0) // Draw the origin of execution
    {
        DrawHead(entry.name);
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

    if (entry.subEntry.size() == 0)
        return;
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
            DrawNameSpace(subEntry, lastEntry, tab, caseTab, lastLineAt, recurrence);
        }
        else
        {
            DrawTest(subEntry, lastEntry, tab, caseTab, iterationValue, entry.longestNameLength - subEntry.name.length());
        }
    }
}


//-Usable functions---------------------------------------------

//value must be > 1 else it will be set to 1
void IterationValue(int value) { iterationValue = value>1?value:1; }

/**
 * @brief Register the tags to target.
 * 
 * @param tagList a possible list of tags like "tag1, tag2".
*/
void TagsToTarget(const char* tagList = "")
{
    if(std::strlen(tagList) == 0)
    {
        tagListToCheck.clear();
        shouldCheckTagList = false;
        return;
    }
    tagListToCheck = GetTagList(tagList);
    shouldCheckTagList = true;
}


template<typename... Args>
void runTests(Args... names) {
    std::system("chcp 65001");
    // std::this_thread::sleep_for(std::chrono::milliseconds(20)); //TODO remove complitly if no bug reported at the next release
    std::cout << "#############################################################################################\n";
    for (int v = VTests.size()-1; v > -1; v--)
    {
        bool shouldContinue = true;
        //Check if should continue the VTest
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