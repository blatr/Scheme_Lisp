#include <iostream>
#include "scheme.h"

int main() {
    Scheme new_scheme;
    while (true) {
        std::string line;
        std::getline(std::cin, line);
        std::stringstream ss(line);
        Tokenizer tok(&ss);
        auto node = Read(&tok);
        auto eval_node = new_scheme.EvaluateExpr(node);
        std::cout << Print(eval_node);
    }
}
