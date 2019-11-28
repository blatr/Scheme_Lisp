#pragma once

#include <memory>
#include <exception>
#include "tokenizer.h"
#include <vector>
#include <iostream>
#include <cassert>

class Scope;

//// Классы ошибок
struct SyntaxError : public std::runtime_error {
    explicit SyntaxError(const std::string& what) : std::runtime_error(what) {
    }
};

struct RuntimeError : public std::runtime_error {
    explicit RuntimeError(const std::string& what) : std::runtime_error(what) {
    }
};

struct NameError : public std::runtime_error {
    explicit NameError(const std::string& what) : std::runtime_error(what) {
    }
};

//// Базовый object
class Object : public std::enable_shared_from_this<Object> {
public:
    virtual ~Object() = default;
    virtual void PrintTo(std::ostream* out);
    virtual std::shared_ptr<Object> Evaluate(std::shared_ptr<Scope> scp);
    virtual std::shared_ptr<Object> Apply(Scope& scp, std::vector<std::shared_ptr<Object>>& params);
};

inline void PrintTo(const std::shared_ptr<Object>& obj, std::ostream* out);
inline std::string Print(const std::shared_ptr<Object>& obj);
std::vector<std::shared_ptr<Object>> ToVector(const std::shared_ptr<Object>& head);

//// класс Boolean
class Boolean : public Object {
public:
    Boolean(bool val);
    void PrintTo(std::ostream* out) override;
    bool GetVal();

private:
    bool val_ = true;
};
////

//// Класс функций
class Function : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scp,
                                          const std::vector<std::shared_ptr<Object>>& args) = 0;
    virtual void PrintTo(std::ostream* out) override;
    virtual std::shared_ptr<Object> Evaluate(std::shared_ptr<Scope> scp);
};

class Plus : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scp,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class Minus : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scp,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class Divide : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scp,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class Multiply : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scp,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class LessOrEqual : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scp,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class Less : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scp,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class GreaterOrEqual : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scp,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class Greater : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scp,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class Equal : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scp,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class Max : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scp,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class Min : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scp,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class Abs : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scp,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class NumberCheck : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scp,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class PairCheck : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scp,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class NullCheck : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scp,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class ListCheck : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scp,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class BooleanCheck : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scp,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class SymbolCheck : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scp,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class Cons : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scp,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class Not : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scp,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

////

//// Класс синтаксиса (особых выражений)
class Syntax : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scp,
                                          const std::vector<std::shared_ptr<Object>>& args) = 0;
    virtual void PrintTo(std::ostream* out) override;
    virtual std::shared_ptr<Object> Evaluate(std::shared_ptr<Scope> scp);
};

class Quote : public Syntax {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scp,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
    void PrintTo(std::ostream* out) override;
};

class If : public Syntax {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scp,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class Car : public Syntax {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scp,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class Cdr : public Syntax {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scp,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class Define : public Syntax {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scp,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
    void PrintTo(std::ostream* out) override;
};

class SetCar : public Syntax {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scp,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
    void PrintTo(std::ostream* out) override;
};

class SetCdr : public Syntax {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scp,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
    void PrintTo(std::ostream* out) override;
};

class Set : public Syntax {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scp,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
    void PrintTo(std::ostream* out) override;
};

class And : public Syntax {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scp,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class Or : public Syntax {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scp,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class ListCmd : public Syntax {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scp,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class ListRef : public Syntax {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scp,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class ListTail : public Syntax {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scp,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class Lambda : public Syntax {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scp,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class LambdaFunc : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scp,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
    std::shared_ptr<Scope> local_scope_;
    std::vector<std::shared_ptr<Object>> params_;
    std::vector<std::shared_ptr<Object>> lambda_func;
};

//// Виды нод в дереве
class NumberNode : public Object {
public:
    NumberNode(int num);
    std::shared_ptr<Object> Evaluate(std::shared_ptr<Scope> scp) override;
    virtual void PrintTo(std::ostream* out) override;
    int64_t GetValue();

private:
    int64_t number_;
};

class SymbolNode : public Object {
public:
    SymbolNode(std::string name);
    virtual std::shared_ptr<Object> Evaluate(std::shared_ptr<Scope> scp);
    virtual void PrintTo(std::ostream* out) override;
    std::string GetName();

private:
    std::string name_;
};

class CellNode : public Object {
public:
    CellNode();
    CellNode(Object first, Object second);

    std::shared_ptr<Object> Evaluate(std::shared_ptr<Scope> scp) override;
    virtual void PrintTo(std::ostream* out) override;
    auto GetFirst();
    auto GetSecond();
    void SetFirst(std::shared_ptr<Object> first);
    void SetSecond(std::shared_ptr<Object> second);

private:
    std::shared_ptr<Object> number_first_ = nullptr;
    std::shared_ptr<Object> number_second_ = nullptr;
};
////

//// Доп assert'ы для парсера
inline bool IsNumber(const std::shared_ptr<Object>& obj) {
    if (auto new_num = std::dynamic_pointer_cast<NumberNode>(obj)) {
        return true;
    }
    return false;
}
inline bool IsCell(const std::shared_ptr<Object>& obj) {
    if (auto new_cell = std::dynamic_pointer_cast<CellNode>(obj)) {
        return true;
    }
    return false;
}
inline bool IsSymbol(const std::shared_ptr<Object>& obj) {
    if (auto new_sym = std::dynamic_pointer_cast<SymbolNode>(obj)) {
        return true;
    }
    return false;
}

inline auto AsNumber(const std::shared_ptr<Object>& obj) {
    return std::dynamic_pointer_cast<NumberNode>(obj);
}

inline auto AsCell(const std::shared_ptr<Object>& obj) {
    return std::dynamic_pointer_cast<CellNode>(obj);
}
inline auto AsSymbol(const std::shared_ptr<Object>& obj) {
    return std::dynamic_pointer_cast<SymbolNode>(obj);
}
////

//// Парсинг
std::shared_ptr<Object> Read(Tokenizer* tokenizer);
std::shared_ptr<Object> ReadList(Tokenizer* tokenizer);
////