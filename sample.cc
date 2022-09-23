#include "dicty.h"
#include "select.h"

using std::vector;
using std::abs;

int main() {
    Dicty::Dicty d;
    d.Compile({"0", "133", "axx", "asjdasdjads", "hello world!", "hello", "helle", "hwllo"});
    printf("%d\n", d.Get("133"));           // get 1
    printf("%d\n", d.Get("hello world!"));  // get 4
    printf("%d\n", d.Get("asjdasdjads"));   // get 3
    printf("%d\n", d.Get("axx"));           // get 2
    printf("%d\n", d.Get("0"));             // get 0

    d.Stat();

    double eps_max = .0;
    for (int i = 2; i < 1000; ++ i) {
        vector<double> pp;
        for (int j = 0; j < i; ++ j) {
            pp.push_back(1. / i);
        }
        double ep = Dicty::Entropy(pp) - std::log2(i);
        eps_max = std::max(eps_max, ep);
    }
    printf("%e\n", eps_max);
    return 0;
}
