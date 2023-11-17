# VTest

VTest is a header only library to make easy and simple Unit test.

For now the library works with five macro an one function :
```c++
VTEST(name)
{
    NAMESPACE(name)
    {
        TEST(name) {
            REQUIRE(expression);
            COMPARE(result, expectation);
        }
    }
}
RunTest();

```
### Explanation
The ```VTEST(name){ } ``` macro is used to declare a unit test. The name taken by a VTEST is unique and will not be usable by another VTEST. if you declare more than one VTEST, they will all run by default, but if you only want specific VTEST, you must do the following: ```runTest("name_1","name_2");```.

The macro ``` NAMESPACE(name){ } ``` taking a name as a parameter the utility of this macro is very limited, it doesn't make an actual test but rather it helps to make clear section when the test is running.

The macro ```TEST(name){ }```it take a name has a parameter and is what make a test, it has to contain ```REQUIRE(expression);``` or ```COMPARE(result, expectation);``` it can be alone or inside a namespace macro.

The macro ```REQUIRE(expression);``` takes an expression as a parameter and will simply check if true.

The macro ```COMPARE(result, expectation);``` is similar to the previous one but should be preferred. It will take a result and an expectation and will test if they are equal if not it will tell what was the value.

### Example
```c++
#include "VTest.hpp"

int factorial(int n)
{
    return n <= 1 ? 1 : n * factorial(n - 1);
}

VTEST(Test)
{
    NAMESPACE(Test)
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

                REQUIRE(factorial(2) == 1);      // wrong value on purpose
                COMPARE(factorial(10), 3628801); // wrong value on purpose
            }
        }

        TEST(Test_3)
        {
            REQUIRE(factorial(1) == 1);
            REQUIRE(factorial(3) == 6);
        }

    }

    TEST(Test_4) {
        REQUIRE(factorial(1) == 1);
        REQUIRE(factorial(1) == 1);
    }
}

int main() {
    system("cls");   // used to clear and enable color on Windows
    runTests();
    system("pause"); // used to pause at the end on Windows;
    return 0;
}
```
### Result

![image](https://github.com/Motisma479/VTest/assets/74106176/159e7ae8-c831-49f5-b90d-7cf928abe664)

## License

[MIT License](/LICENSE)

