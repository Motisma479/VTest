# VTest

VTest is a header only library to make easy and simple Unit test.

For now the library works with five macro an one function :
```c++
NAMESPACE(name)
TEST(name) {
    REQUIRE(expression);
    COMPARE(result, expectation);
}
END_NAMESPACE(name)
RunTest();

```
### Explanation
The macro ``` NAMESPACE(name) ``` taking a name as a parameter the utility of this macro is very limited, it doesn't make an actual test but rather it helps to make clear section when the test is running. To use it Properly use the macro ```END_NAMESPACE(name)```.

The macro ``` END_NAMESPACE(name) ``` has to be used with the macro ``` NAMESPACE(name) ``` it take a name has a parameter which has no link with the name of the ```NAMESPACE(name)``` yet it is better for understanding to use one name for both.

The macro ```TEST(name){ }```it take a name has a parameter and is what make a test, it has to contain ```REQUIRE(expression);``` or ```COMPARE(result, expectation);``` it can be alone or between to namespace macro.

The macro ```REQUIRE(expression);``` takes an expression as a parameter and will simply check if true.

The macro ```COMPARE(result, expectation);``` is similar to the previous one but should be preferred. It will take a result and an expectation and will test if they are equal if not it will tell what was the value.

### Example
```c++
#include "VTest.hpp"

int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

NAMESPACE(Test)

    TEST(Test_1) {
        REQUIRE(factorial(1) == 1);
        COMPARE(factorial(2), 2);
    }

    NAMESPACE(Test2)

        TEST(Test_2) {
            REQUIRE(factorial(0) == 1);
            REQUIRE(factorial(1) == 1);

            REQUIRE(factorial(2) == 1);      // wrong value on purpose
            COMPARE(factorial(10), 3628801); // wrong value on purpose
        }

    END_NAMESPACE(Test2)

    TEST(Test_3) {
        REQUIRE(factorial(1) == 1);
        REQUIRE(factorial(3) == 6);
    }

END_NAMESPACE(Test)

TEST(Test_4) {
    REQUIRE(factorial(1) == 1);
    REQUIRE(factorial(1) == 1);
}

int main() {
    system("cls");   // used to clear and enable color on windows
    runTests();
    system("pause"); // used to pause at the end;
    return 0;
}
```
## License

[MIT License](/LICENSE)

