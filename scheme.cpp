#pragma once

#include "scheme.h"
#include <sstream>

std::shared_ptr<Object> Scope::Lookup(const std::string& name) {
    auto it = scope_.find(name);
    if (it == scope_.end()) {
        throw NameError("Naming error");
    }
    return it->second;
}
Scheme::Scheme() : global_scope_(std::make_shared<Scope>()) {
    global_scope_->scope_["+"] = std::make_shared<Plus>();
    global_scope_->scope_["-"] = std::make_shared<Minus>();
    global_scope_->scope_["/"] = std::make_shared<Divide>();
    global_scope_->scope_["*"] = std::make_shared<Multiply>();
    global_scope_->scope_["if"] = std::make_shared<If>();
    global_scope_->scope_["'"] = std::make_shared<Quote>();
    global_scope_->scope_["quote"] = std::make_shared<Quote>();
    global_scope_->scope_["#t"] = std::make_shared<Boolean>(true);
    global_scope_->scope_["#f"] = std::make_shared<Boolean>(false);
    global_scope_->scope_["="] = std::make_shared<Equal>();
    global_scope_->scope_[">"] = std::make_shared<Greater>();
    global_scope_->scope_[">="] = std::make_shared<GreaterOrEqual>();
    global_scope_->scope_["<"] = std::make_shared<Less>();
    global_scope_->scope_["<="] = std::make_shared<LessOrEqual>();
    global_scope_->scope_["abs"] = std::make_shared<Abs>();
    global_scope_->scope_["min"] = std::make_shared<Min>();
    global_scope_->scope_["max"] = std::make_shared<Max>();
    global_scope_->scope_["number?"] = std::make_shared<NumberCheck>();
    global_scope_->scope_["pair?"] = std::make_shared<PairCheck>();
    global_scope_->scope_["null?"] = std::make_shared<NullCheck>();
    global_scope_->scope_["list?"] = std::make_shared<ListCheck>();
    global_scope_->scope_["symbol?"] = std::make_shared<SymbolCheck>();
    global_scope_->scope_["cons"] = std::make_shared<Cons>();
    global_scope_->scope_["car"] = std::make_shared<Car>();
    global_scope_->scope_["cdr"] = std::make_shared<Cdr>();
    global_scope_->scope_["define"] = std::make_shared<Define>();
    global_scope_->scope_["set-car!"] = std::make_shared<SetCar>();
    global_scope_->scope_["set-cdr!"] = std::make_shared<SetCdr>();
    global_scope_->scope_["list"] = std::make_shared<ListCmd>();
    global_scope_->scope_["list-ref"] = std::make_shared<ListRef>();
    global_scope_->scope_["list-tail"] = std::make_shared<ListTail>();
    global_scope_->scope_["boolean?"] = std::make_shared<BooleanCheck>();
    global_scope_->scope_["not"] = std::make_shared<Not>();
    global_scope_->scope_["and"] = std::make_shared<And>();
    global_scope_->scope_["or"] = std::make_shared<Or>();
    global_scope_->scope_["set!"] = std::make_shared<Set>();
    global_scope_->scope_["lambda"] = std::make_shared<Lambda>();
}
Scheme::~Scheme() {
    global_scope_->scope_.clear();
}
std::shared_ptr<Object> Scheme::EvaluateExpr(std::shared_ptr<Object> in) {
    if (in) {
        return in->Evaluate(global_scope_);
    } else {
        throw RuntimeError("Null root node");
    }
}