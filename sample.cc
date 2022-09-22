#include "dicty.h"

int main() {
    Dicty::Dicty d;
    d.Compile({"0", "133", "axx", "asjdasdjads", "hello world!"});
    printf("%d\n", d.Get("133"));           // get 1
    printf("%d\n", d.Get("hello world!"));  // get 4
    printf("%d\n", d.Get("asjdasdjads"));   // get 3
    printf("%d\n", d.Get("axx"));           // get 2
    printf("%d\n", d.Get("0"));             // get 0
    return 0;
}
