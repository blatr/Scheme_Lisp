#pragma once

#include "parser.h"
#include <string>
#include <unordered_map>
#include <sstream>
#include <iostream>

class Scope {
public:
    std::shared_ptr<Object> Lookup(const std::string& name);

    std::unordered_map<std::string, std::shared_ptr<Object>> scope_;
    std::shared_ptr<Scope> outer_scope_;
};
class Scheme {
public:
    Scheme();
    ~Scheme();
    std::shared_ptr<Object> EvaluateExpr(std::shared_ptr<Object> in);

private:
    std::shared_ptr<Scope> global_scope_;
};

inline void PrintTo(const std::shared_ptr<Object>& obj, std::ostream* out) {
    if (!obj) {
        *out << "()";
        return;
    }

    obj->PrintTo(out);
}
inline std::string Print(const std::shared_ptr<Object>& obj) {
    std::stringstream ss;
    PrintTo(obj, &ss);
    return ss.str();
}
