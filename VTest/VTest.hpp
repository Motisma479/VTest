#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include <string>

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
struct testCase
{
    std::vector<std::string> nameSpaces;
    std::string name;
    std::function<void()> function;
};
struct testResult
{
    bool result;
    std::string expression;
};

std::vector<std::string> nameSpaces;
std::vector<testCase> testCases;
std::vector<testResult> results;
bool asError = false;

void registerTestCase(std::vector<std::string> nameSpaces, const std::string& name, const std::function<void()>& testFunction)
{
    testCases.push_back({ nameSpaces, name, testFunction });
}

constexpr const char* RED = "\033[38;2;242;96;103m";
constexpr const char* GREEN = "\033[38;2;74;255;120m";
constexpr const char* BLUE = "\033[38;2;44;158;243m";
constexpr const char* DEFAULT = "\033[0m";

void runTests() {
    int passed = 0;
    int failed = 0;
    std::string lastNamespace;
    int lastSize = 0;
    std::string tab;

    std::cout << "#############################################################################################\n";
    for (const auto& testCase : testCases) {
        asError = false;
        results.clear();
        testCase.function();

        if (testCase.nameSpaces.size() == 0 || testCase.nameSpaces.size() < lastSize)
        {
            tab = "";
        }

        if (testCase.nameSpaces.size() > 0 && lastNamespace != testCase.nameSpaces.back())
        {
            if (testCase.nameSpaces.size() < lastSize)
                for (int i = 0; i < testCase.nameSpaces.size(); i++)
                {
                    tab += "    |   ";
                }
            else
            {
                int size = 0;
                if (testCase.nameSpaces.size() > lastSize)
                    size = lastSize;
                for (; size < testCase.nameSpaces.size(); size++)
                {
                    std::cout << tab << '[' << BLUE << testCase.nameSpaces[size] << DEFAULT <<']' << std::endl;
                    tab += "    |   ";
                }
            }

            lastNamespace = testCase.nameSpaces.back();
            lastSize = testCase.nameSpaces.size();
        }
        
        if (!asError) {
            std::cout << tab << "[" << GREEN << "PASS" << DEFAULT << "] " << testCase.name << "\n";
            passed++;
        }
        else {
            std::cout << tab << "[" << RED << "FAIL" << DEFAULT << "] " << testCase.name << "\n";
            for (const auto& result : results) {
                auto color = result.result == true ? GREEN : RED;
                std::cout << tab << color << result.expression << DEFAULT << "\n";
            }
            failed++;
        }
    }
    std::cout << "#############################################################################################\n";
    std::cout << "Tests run: " << passed + failed << ", Passed: " << passed << ", Failed: " << failed << std::endl;
}