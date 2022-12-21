#include <stdio.h>

struct Person {
    int age;
    char name[];
};

int main() {
    struct Person child = {5, "Bob"};
    printf("%d", child.age);
    printf("%s", child.name);

}




