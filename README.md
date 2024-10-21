# VTest

VTest is a header only library to make easy and simple Unit test.

For now the library works with five macro and three functions :
```c++
VTEST(name)
{
    NAMESPACE(name)
    {
        TEST(name) {
            REQUIRE(expression);
            COMPARE(result, expectation);
            COMPARE_WITH_PRECISION(result, expectation, precision);
        }
    }
}
IterationValue(numberToIterate);
TagsToTarget();
RunTest();

```
### Explanation
The ```VTEST(name){ } ``` macro is used to declare a unit test. The name taken by a VTEST is unique and will not be usable by another VTEST. if you declare more than one VTEST, they will all run by default, but if you only want specific VTEST, you must do the following: ```runTest("name_1","name_2");```.

The macro ``` NAMESPACE(name){ } ``` taking a name as a parameter the utility of this macro is very limited, it doesn't make an actual test but rather it helps to make clear section when the test is running.

The macro ```TEST(name){ }```it take a name has a parameter and is what make a test, it has to contain ```REQUIRE(expression);```, ```COMPARE(result, expectation);``` or ```COMPARE_WITH_PRECISION(result, expectation, precision);``` it can be alone or inside a namespace macro.

The macro ```REQUIRE(expression);``` takes an expression as a parameter and will simply check if true.

The macro ```COMPARE(result, expectation);``` is similar to the previous one but should be preferred. It will take a result and an expectation and will test if they are equal if not it will tell what was the value.

The macro ```COMPARE_WITH_PRECISION(result, expectation, precision);``` is similar to the previous one but need a precision. It will test and compare with a precision so it is useful for floating numbers like ```double``` or ```float```.

If you want to use Tags you can set a list of tags in a test or in a namespace like : ```NAMESPACE(name,"Tag, Tag2"){ }``` or ```TEST(name,"Tag, Tag2"){ }``` and then you can draw specific tags with : ```TagsToTarget("Tag, Tag2");```

The duration of a ```TEST``` will be measured and print in millisecond. for an accurate measure you can use the function: ```IterationValue(numberToIterate);```.

It is now possible to cout text in a test. You only have to use ```std::cout```. note: it is not recommended to use this option for measurement since it is heavy to cout.

### Example
```c++
#include "VTest.hpp"

int factorial(int n)
{
    return n <= 1 ? 1 : n * factorial(n - 1);
}

VTEST(Test)
{
    NAMESPACE(Test,"Tag, Tag2")
    {
        TEST(Test_1)
        {
            REQUIRE(factorial(1) == 1);
            COMPARE(factorial(2), 2);
        }

        NAMESPACE(Test2)
        {
            TEST(Test_2) {
                REQUIRE(factorial(0) == 1);
                REQUIRE(factorial(1) == 1);
                COMPARE_WITH_PRECISION(0.005, 0.0050001, 0.001); // wrong value on purpose

                REQUIRE(factorial(2) == 1);      // wrong value on purpose
                COMPARE(factorial(10), 3628801); // wrong value on purpose
            }
        }

        TEST(Test_3)
        {
            REQUIRE(factorial(1) == 1);
            REQUIRE(factorial(3) == 6);

            std::cout << "This Text is here with a std::cout" << std::endl;
            std::cout << "the functionality support multiple lines like this\none\ntwo\nthree" << std::endl;
        }

    }

    //shouldn't be drawn 
    TEST(Test_4) {
        REQUIRE(factorial(1) == 1);
        REQUIRE(factorial(1) == 1);
    }

    TEST(Test_5, "Tag2") {
        REQUIRE(factorial(1) == 1);
        REQUIRE(factorial(1) == 1);
    }
}

int main() {
    system("cls");   // used to clear and enable color on Windows
    IterationValue(3);
    TagsToTarget("Tag2");
    runTests();
    system("pause"); // used to pause at the end on Windows;
    return 0;
}
```
### Result

![image](https://github.com/Motisma479/VTest/assets/74106176/7a20ffc9-c334-4c94-aa09-ec80631d986a)


## License

[MIT License](/LICENSE)

