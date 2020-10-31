#include <stdio.h>
#include <stdbool.h>

// implement bracket matching for "()[]{}" without external libraries

size_t strlen(const char* s)
{
    const char *p = s;
    while (*p++)
        ;
    return p - s;
}

struct TestSt {
    char const *test_name;
    char const *input;
    bool expect;
};

// Given below array, write a function that will validate the JSON objects.
// Start simply, validate the parentheses/brackets first
// Assume that this will be used in a real Library code
// Clean code matters
// You can use C or C++ but without STL or any other external libraries.
// We don't care about the contents of those objects, we want them to be valid.
// Valid objetc means, that All parentheses/ brackets '[]','()','{}' are mathcing
// Also we have to make sure that single quotes are in place ( that can be done later )


#if 0

struct TestSt test_json_array[] = {
    {
        "Test1",
        "{ \
        'server_name': 'server1',\
        'components': ['comp1', 'comp2', comp3'], \
        'ip_address': '127.0.0.1', \
    }",
    true },
    {
        "Test2",
        "{ \
        'server_name': 'server3',\
        'components': ['comp1', 'comp2', comp3'], \
        'ip_address': '127.0.0.1', \
    }",
    true },
    {
        "Test3",
        "{ \
        'server_name': 'server2',\
        'components': ['comp1', 'comp2', comp3'], \
        'ip_address': '127.0.0.1', \
    ",
    false },
    {
        "Test4",
        "{ \
        'server_name': 'server1',\
        'components': ['comp1': [ \
            'comp1_1','comp1_2', 'comp1_3' \
           ] \
        ], \
        'ip_address': '127.0.0.1', \
    }",
    true },
};

#else

//struct TestSt test_balanced_parens[] = {
struct TestSt test_json_array[] = {
    {"Test2", "[()]", true},
    {"Test3", "([]())", true},
    {"Test4", "[()]{}{[()()]()}", true},
    {"Test5", "{}", true},
    {"Test6", "((((({})))))", true},
    {"Test6", "{()", false},
    {"Test7", "[(])", false},
    {"Test8", "((((((((())))", false},
    {"Test9", "[()]]", false},
    {"Test10", "]()[", false},
};

#endif

const char *validate_json_object( const struct TestSt obj) {
    char        stack[1000];
    char       *sp = stack;
    const char *errptr = NULL;

    printf("'%s'\n", obj.input);
    for (const char *p = obj.input; *p && errptr == NULL; p++) {
        if (*p == '{' || *p == '[' || *p == '(') {
            if (sp >= (stack + sizeof(stack))) {
                printf("Stack Overflow Error ");
                return p;
            }
            *sp++ = *p;
        } else {
            char opening = 0;
            switch (*p) {
                case ')': opening = '('; break;
                case ']': opening = '['; break;
                case '}': opening = '{'; break;
            }
            if (opening != 0) {
                if (sp == stack || *--sp != opening) {
                    printf("missing opening symbol for %c ", *p);
                    errptr = p;
                }
            }
        }
    }
    if (errptr == NULL && sp != stack) {
        printf("missing closing symbol for %c ", *(sp-1));
        errptr = obj.input + strlen(obj.input) - 1;
    }
    return errptr;
}

bool validate_json(const struct TestSt json_array[], const int num_elements) {
    bool result = true;
    for (int i = 0; i < num_elements; i++) {
        printf("\n%s, element: %d\n", json_array[i].test_name, i);
        const char *errptr = validate_json_object(json_array[i]);
        if (errptr != NULL) {
            const int errpos = errptr - json_array[i].input;
            printf("at input index %d in %s (element %d)\n", errpos, json_array[i].test_name, i);
            result = false;
        }
        const char *expected = json_array[i].expect ? "true" : "false";
        const char *actual = (errptr == NULL) ? "true" : "false";
        printf("Expected: %s, Actual %s\n", expected, actual);
    }
    return result;
}

int main() {
    const int num_elements = sizeof(test_json_array) / sizeof(struct TestSt);
    printf("num_elements: %d\n", num_elements);
    if (validate_json(test_json_array, num_elements))
        return 0;
    return 1;
}
