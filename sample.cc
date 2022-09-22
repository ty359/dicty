#include "dicty.hpp"

int main() {
    Dicty::Dicty d;
    d.Compile({"0", "133", "axx", "asjdasdjads", "hello world!"});
    printf("%d\n", d.Get("133"));
    printf("%d\n", d.Get("hello world!"));
    printf("%d\n", d.Get("asjdasdjads"));
    return 0;
}
