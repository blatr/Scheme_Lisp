#include "parser.h"
#include "scheme.h"

/// Evaluate для нод в дереве (вытащить тип ноды из скопа)
std::shared_ptr<Object> SymbolNode::Evaluate(std::shared_ptr<Scope> scp) {
  return scp->Lookup(name_);
}
SymbolNode::SymbolNode(std::string name) : name_(name) {}
void SymbolNode::PrintTo(std::ostream *out) { *out << name_; }
std::string SymbolNode::GetName() { return name_; }
////

std::shared_ptr<Object> CellNode::Evaluate(std::shared_ptr<Scope> scp) {
  auto base_op = number_first_->Evaluate(scp);
  auto fn = std::dynamic_pointer_cast<Function>(base_op);
  auto syntax = std::dynamic_pointer_cast<Syntax>(base_op);

  if (!fn && !syntax) {
    throw RuntimeError("first element must be a function or syntax");
  }

  std::vector<std::shared_ptr<Object>> args = ToVector(number_second_);
  if (fn) {
    for (auto &arg : args) {
      arg = arg->Evaluate(scp);
    }
    return fn->Apply(scp, args);
  }
  return syntax->Apply(scp, args);
}
CellNode::CellNode() {}
CellNode::CellNode(Object first, Object second)
    : number_first_(std::make_shared<Object>(first)),
      number_second_(std::make_shared<Object>(second)) {}
void CellNode::PrintTo(std::ostream *out) {
  *out << "(";
  ::PrintTo(number_first_, out);
  auto next = std::dynamic_pointer_cast<CellNode>(number_second_);
  if (!next && number_second_) {
    *out << " . ";
    ::PrintTo(number_second_, out);
  } else {
    while (next) {
      *out << " ";
      ::PrintTo(next->number_first_, out);
      auto next_obj = std::dynamic_pointer_cast<CellNode>(next->number_second_);
      if (!next_obj && next->number_second_) {
        *out << " . ";
        ::PrintTo(next->number_second_, out);
      }
      next = next_obj;
    }
  }
  *out << ")";
}
auto CellNode::GetFirst() { return number_first_; }
auto CellNode::GetSecond() { return number_second_; }
void CellNode::SetFirst(std::shared_ptr<Object> first) {
  number_first_ = first;
}
void CellNode::SetSecond(std::shared_ptr<Object> second) {
  number_second_ = second;
}

//// Методы Function
void Function::PrintTo(std::ostream *out) { *out << "<function>"; }
std::shared_ptr<Object> Function::Evaluate(std::shared_ptr<Scope> scp) {
  throw std::runtime_error("can't eval function");
}

std::shared_ptr<Object>
Plus::Apply(const std::shared_ptr<Scope> &scp,
            const std::vector<std::shared_ptr<Object>> &args) {
  int64_t value = 0;
  for (const auto &arg : args) {
    auto number = std::dynamic_pointer_cast<NumberNode>(arg);
    if (!number) {
      throw RuntimeError("Wrong type for plus func value");
    }
    value += number->GetValue();
  }
  return std::make_shared<NumberNode>(value);
}

std::shared_ptr<Object>
Minus::Apply(const std::shared_ptr<Scope> &scp,
             const std::vector<std::shared_ptr<Object>> &args) {
  if (!args.empty()) {
    if (auto number = std::dynamic_pointer_cast<NumberNode>(args[0])) {
      int64_t value = number->GetValue();
      for (auto arg = args.begin() + 1; arg != args.end(); ++arg) {
        number = std::dynamic_pointer_cast<NumberNode>(*arg);
        if (!number) {
          throw RuntimeError("Wrong type for minus func value");
        }
        value -= number->GetValue();
      }
      return std::make_shared<NumberNode>(value);
    } else {
      throw RuntimeError("Wrong type for minus func value");
    }
  } else {
    throw RuntimeError("Wrong size for minus func value");
  }
}

std::shared_ptr<Object>
Divide::Apply(const std::shared_ptr<Scope> &scp,
              const std::vector<std::shared_ptr<Object>> &args) {
  if (!args.empty()) {
    if (auto number = std::dynamic_pointer_cast<NumberNode>(args[0])) {
      int64_t value = number->GetValue();
      for (auto arg = args.begin() + 1; arg != args.end(); ++arg) {
        number = std::dynamic_pointer_cast<NumberNode>(*arg);
        if (!number) {
          throw RuntimeError("Wrong type for divide func value");
        }
        value /= number->GetValue();
      }
      return std::make_shared<NumberNode>(value);
    } else {
      throw RuntimeError("Wrong type for divide func value");
    }
  } else {
    throw RuntimeError("Wrong size for divide func value");
  }
}

std::shared_ptr<Object>
Multiply::Apply(const std::shared_ptr<Scope> &scp,
                const std::vector<std::shared_ptr<Object>> &args) {
  int64_t value = 1;
  for (const auto &arg : args) {
    auto number = std::dynamic_pointer_cast<NumberNode>(arg);
    if (!number) {
      throw RuntimeError("Wrong type for multiply func value");
    }
    value *= number->GetValue();
  }
  return std::make_shared<NumberNode>(value);
}

//// Предикаты
std::shared_ptr<Object>
Equal::Apply(const std::shared_ptr<Scope> &scp,
             const std::vector<std::shared_ptr<Object>> &args) {
  bool value = true;

  if (!args.empty()) {
    if (auto last_value = std::dynamic_pointer_cast<NumberNode>(args[0])) {
      for (auto arg = args.begin() + 1; arg != args.end(); ++arg) {
        auto number = std::dynamic_pointer_cast<NumberNode>(*arg);
        if (!number) {
          throw RuntimeError("Wrong type for Equal func value");
        }
        value = ((last_value->GetValue() == number->GetValue()) && value);
        last_value = number;
      }
    } else {
      throw RuntimeError("Wrong type for Equal func value");
    }
  }
  return std::make_shared<Boolean>(value);
}
std::shared_ptr<Object>
Greater::Apply(const std::shared_ptr<Scope> &scp,
               const std::vector<std::shared_ptr<Object>> &args) {
  bool value = true;

  if (!args.empty()) {
    if (auto last_value = std::dynamic_pointer_cast<NumberNode>(args[0])) {
      for (auto arg = args.begin() + 1; arg != args.end(); ++arg) {
        auto number = std::dynamic_pointer_cast<NumberNode>(*arg);
        if (!number) {
          throw RuntimeError("Wrong type for Greater func value");
        }
        value = ((last_value->GetValue() > number->GetValue()) && value);
        last_value = number;
      }
    } else {
      throw RuntimeError("Wrong type for Greater func value");
    }
  }
  return std::make_shared<Boolean>(value);
}
std::shared_ptr<Object>
GreaterOrEqual::Apply(const std::shared_ptr<Scope> &scp,
                      const std::vector<std::shared_ptr<Object>> &args) {
  bool value = true;

  if (!args.empty()) {
    if (auto last_value = std::dynamic_pointer_cast<NumberNode>(args[0])) {
      for (auto arg = args.begin() + 1; arg != args.end(); ++arg) {
        auto number = std::dynamic_pointer_cast<NumberNode>(*arg);
        if (!number) {
          throw RuntimeError("Wrong type for GreaterOrEqual func value");
        }
        value = ((last_value->GetValue() >= number->GetValue()) && value);
        last_value = number;
      }
    } else {
      throw RuntimeError("Wrong type for GreaterOrEqual func value");
    }
  }
  return std::make_shared<Boolean>(value);
}
std::shared_ptr<Object>
Less::Apply(const std::shared_ptr<Scope> &scp,
            const std::vector<std::shared_ptr<Object>> &args) {
  bool value = true;

  if (!args.empty()) {
    if (auto last_value = std::dynamic_pointer_cast<NumberNode>(args[0])) {
      for (auto arg = args.begin() + 1; arg != args.end(); ++arg) {
        auto number = std::dynamic_pointer_cast<NumberNode>(*arg);
        if (!number) {
          throw RuntimeError("Wrong type for Less func value");
        }
        value = ((last_value->GetValue() < number->GetValue()) && value);
        last_value = number;
      }
    } else {
      throw RuntimeError("Wrong type for Less func value");
    }
  }
  return std::make_shared<Boolean>(value);
}
std::shared_ptr<Object>
LessOrEqual::Apply(const std::shared_ptr<Scope> &scp,
                   const std::vector<std::shared_ptr<Object>> &args) {
  bool value = true;

  if (!args.empty()) {
    if (auto last_value = std::dynamic_pointer_cast<NumberNode>(args[0])) {
      for (auto arg = args.begin() + 1; arg != args.end(); ++arg) {
        auto number = std::dynamic_pointer_cast<NumberNode>(*arg);
        if (!number) {
          throw RuntimeError("Wrong type for LessOrEqual func value");
        }
        value = ((last_value->GetValue() <= number->GetValue()) && value);
        last_value = number;
      }
    } else {
      throw RuntimeError("Wrong type for LessOrEqual func value");
    }
  }
  return std::make_shared<Boolean>(value);
}
////

//// Min/Max
std::shared_ptr<Object>
Max::Apply(const std::shared_ptr<Scope> &scp,
           const std::vector<std::shared_ptr<Object>> &args) {
  if (!args.empty()) {
    if (auto cast = std::dynamic_pointer_cast<NumberNode>(args[0])) {
      auto max_val = cast->GetValue();
      for (const auto &arg : args) {
        auto number = std::dynamic_pointer_cast<NumberNode>(arg);
        if (!number) {
          throw RuntimeError("Wrong type for max func value");
        }
        max_val = std::max(max_val, number->GetValue());
      }
      return std::make_shared<NumberNode>(max_val);
    } else {
      throw RuntimeError("Wrong type for max func value");
    }
  } else {
    throw RuntimeError("Max func should have at least one argument");
  }
}
std::shared_ptr<Object>
Min::Apply(const std::shared_ptr<Scope> &scp,
           const std::vector<std::shared_ptr<Object>> &args) {
  if (!args.empty()) {
    if (auto cast = std::dynamic_pointer_cast<NumberNode>(args[0])) {
      auto max_val = cast->GetValue();
      for (const auto &arg : args) {
        auto number = std::dynamic_pointer_cast<NumberNode>(arg);
        if (!number) {
          throw RuntimeError("Wrong type for min func value");
        }
        max_val = std::min(max_val, number->GetValue());
      }
      return std::make_shared<NumberNode>(max_val);
    } else {
      throw RuntimeError("Wrong type for max func value");
    }
  } else {
    throw RuntimeError("Min func should have at least one argument");
  }
}
////
std::shared_ptr<Object>
Abs::Apply(const std::shared_ptr<Scope> &scp,
           const std::vector<std::shared_ptr<Object>> &args) {
  if (args.size() == 1) {
    if (auto number = std::dynamic_pointer_cast<NumberNode>(args[0])) {
      return std::make_shared<NumberNode>(std::abs(number->GetValue()));
    } else {
      throw RuntimeError("Wrong type for abs func value");
    }
  } else {
    throw RuntimeError("Abs func should have strictly one argument");
  }
}

std::shared_ptr<Object>
NumberCheck::Apply(const std::shared_ptr<Scope> &scp,
                   const std::vector<std::shared_ptr<Object>> &args) {
  if (args.size() == 1) {
    if (auto number = std::dynamic_pointer_cast<NumberNode>(args[0])) {
      return std::make_shared<Boolean>(true);
    } else {
      return std::make_shared<Boolean>(false);
    }
  } else {
    throw RuntimeError("Too many arguments in Number check");
  }
}

std::shared_ptr<Object>
PairCheck::Apply(const std::shared_ptr<Scope> &scp,
                 const std::vector<std::shared_ptr<Object>> &args) {
  if (auto cast = std::dynamic_pointer_cast<CellNode>(args[0])) {
    return std::make_shared<Boolean>(true);
  } else {
    return std::make_shared<Boolean>(false);
  }
}

std::shared_ptr<Object>
NullCheck::Apply(const std::shared_ptr<Scope> &scp,
                 const std::vector<std::shared_ptr<Object>> &args) {
  if (auto cast = std::dynamic_pointer_cast<CellNode>(args[0])) {
    return std::make_shared<Boolean>(false);
  } else {
    return std::make_shared<Boolean>(true);
  }
}
std::shared_ptr<Object>
ListCheck::Apply(const std::shared_ptr<Scope> &scp,
                 const std::vector<std::shared_ptr<Object>> &args) {
  auto cur_head = std::dynamic_pointer_cast<CellNode>(args[0]);
  while (cur_head && cur_head->GetSecond()) {
    if (auto cast =
            std::dynamic_pointer_cast<CellNode>(cur_head->GetSecond())) {
      cur_head = cast;
    } else {
      return std::make_shared<Boolean>(false);
    }
  }
  return std::make_shared<Boolean>(true);
}

std::shared_ptr<Object>
BooleanCheck::Apply(const std::shared_ptr<Scope> &scp,
                    const std::vector<std::shared_ptr<Object>> &args) {
  if (auto cast = std::dynamic_pointer_cast<Boolean>(args[0])) {
    return std::make_shared<Boolean>(true);
  }
  return std::make_shared<Boolean>(false);
}

std::shared_ptr<Object>
SymbolCheck::Apply(const std::shared_ptr<Scope> &scp,
                   const std::vector<std::shared_ptr<Object>> &args) {
  if (auto cast = std::dynamic_pointer_cast<SymbolNode>(args[0])) {
    return std::make_shared<Boolean>(true);
  }
  return std::make_shared<Boolean>(false);
}

std::shared_ptr<Object>
Not::Apply(const std::shared_ptr<Scope> &scp,
           const std::vector<std::shared_ptr<Object>> &args) {
  if (args.size() == 1) {
    if (auto cast = std::dynamic_pointer_cast<Boolean>(args[0])) {
      return std::make_shared<Boolean>(!cast->GetVal());
    }
    return std::make_shared<Boolean>(false);
  } else {
    throw RuntimeError("too many arguments for not func");
  }
}
std::shared_ptr<Object>
And::Apply(const std::shared_ptr<Scope> &scp,
           const std::vector<std::shared_ptr<Object>> &args) {
  if (!args.empty()) {
    for (auto &arg : args) {
      auto new_arg = arg->Evaluate(scp);
      auto cast_bool = std::dynamic_pointer_cast<Boolean>(new_arg);
      auto cast_cell = std::dynamic_pointer_cast<CellNode>(new_arg);
      if (cast_bool && !cast_bool->GetVal()) {
        return cast_bool;
      }
      if (cast_cell) {
        return cast_cell;
      }
    }
  }
  return std::make_shared<Boolean>(true);
}

std::shared_ptr<Object>
Or::Apply(const std::shared_ptr<Scope> &scp,
          const std::vector<std::shared_ptr<Object>> &args) {
  if (!args.empty()) {
    bool flg = true;
    for (auto &arg : args) {
      auto new_arg = arg->Evaluate(scp);
      auto cast_bool = std::dynamic_pointer_cast<Boolean>(new_arg);
      auto cast_cell = std::dynamic_pointer_cast<CellNode>(new_arg);
      if (cast_bool && cast_bool->GetVal()) {
        return cast_bool;
      } else if (!cast_bool) {
        return new_arg;
      }
    }
  }
  return std::make_shared<Boolean>(false);
}

std::shared_ptr<Object>
Cons::Apply(const std::shared_ptr<Scope> &scp,
            const std::vector<std::shared_ptr<Object>> &args) {
  auto cell_node = std::make_shared<CellNode>();
  cell_node->SetFirst(args[0]);
  cell_node->SetSecond(args[1]);
  return cell_node;
}
//// Методы Syntax
void Syntax::PrintTo(std::ostream *out) { *out << "<syntax>"; }

std::shared_ptr<Object> Syntax::Evaluate(std::shared_ptr<Scope> scp) {
  throw RuntimeError("can't eval syntax");
}

std::shared_ptr<Object>
Quote::Apply(const std::shared_ptr<Scope> &scp,
             const std::vector<std::shared_ptr<Object>> &args) {
  if (args.size() != 1) {
    throw SyntaxError("Wrong size for list");
  }
  return args[0];
}

void Quote::PrintTo(std::ostream *out) { Syntax::PrintTo(out); }
std::shared_ptr<Object>
If::Apply(const std::shared_ptr<Scope> &scp,
          const std::vector<std::shared_ptr<Object>> &args) {

  if (args.size() >= 2 && args.size() <= 3) {
    auto condition = args[0];
    auto true_branch = args[1];
    std::shared_ptr<Object> false_branch = nullptr;
    if (args.size() == 3) {
      false_branch = args[2];
    }
    auto result = std::dynamic_pointer_cast<Boolean>(condition->Evaluate(scp));
    if (result->GetVal()) {
      return true_branch->Evaluate(scp);
    } else if (false_branch) {
      return false_branch->Evaluate(scp);
    } else {
      return false_branch;
    }
  } else {
    throw SyntaxError("Incorrect number of arguments");
  }
}
std::shared_ptr<Object>
Car::Apply(const std::shared_ptr<Scope> &scp,
           const std::vector<std::shared_ptr<Object>> &args) {
  if (auto cast = std::dynamic_pointer_cast<CellNode>(args[0])) {
    return std::dynamic_pointer_cast<CellNode>(args[0]->Evaluate(scp))
        ->GetFirst();
  } else if (auto cast = std::dynamic_pointer_cast<SymbolNode>(args[0])) {
    return args[0]->Evaluate(scp);
  }
}
std::shared_ptr<Object>
Cdr::Apply(const std::shared_ptr<Scope> &scp,
           const std::vector<std::shared_ptr<Object>> &args) {
  if (auto cast = std::dynamic_pointer_cast<CellNode>(args[0])) {
    return std::dynamic_pointer_cast<CellNode>(args[0]->Evaluate(scp))
        ->GetSecond();
  } else if (auto cast = std::dynamic_pointer_cast<SymbolNode>(args[0])) {
    return args[0]->Evaluate(scp);
  }
}
std::shared_ptr<Object>
Define::Apply(const std::shared_ptr<Scope> &scp,
              const std::vector<std::shared_ptr<Object>> &args) {
  if (args.size() == 2) {
    if (auto arg_cast = std::dynamic_pointer_cast<SymbolNode>(args[0])) {
      scp->scope_[std::dynamic_pointer_cast<SymbolNode>(args[0])->GetName()] =
          args[1]->Evaluate(scp);
      return shared_from_this();
    } else if (auto arg_cast = std::dynamic_pointer_cast<CellNode>(args[0])) {
      std::shared_ptr<LambdaFunc> new_func = std::make_shared<LambdaFunc>();
      for (size_t i = 1; i < args.size(); ++i) {
        new_func->lambda_func.push_back(args[i]);
      }
      new_func->local_scope_ = std::make_shared<Scope>(*scp);
      new_func->params_ = ToVector(arg_cast->GetSecond());

      scp->scope_[std::dynamic_pointer_cast<SymbolNode>(arg_cast->GetFirst())
                      ->GetName()] = new_func;
      return shared_from_this();
    }
  } else {
    throw SyntaxError("not enough arguments");
  }
}
void Define::PrintTo(std::ostream *out) {}
std::shared_ptr<Object>
SetCar::Apply(const std::shared_ptr<Scope> &scp,
              const std::vector<std::shared_ptr<Object>> &args) {
  scp->scope_[std::dynamic_pointer_cast<SymbolNode>(args[0])->GetName()] =
      args[1]->Evaluate(scp);
  return shared_from_this();
}
void SetCar::PrintTo(std::ostream *out) {}
std::shared_ptr<Object>
SetCdr::Apply(const std::shared_ptr<Scope> &scp,
              const std::vector<std::shared_ptr<Object>> &args) {
  scp->scope_[std::dynamic_pointer_cast<SymbolNode>(args[0])->GetName()] =
      args[1]->Evaluate(scp);
  return shared_from_this();
}
void SetCdr::PrintTo(std::ostream *out) {}

std::shared_ptr<Object>
Set::Apply(const std::shared_ptr<Scope> &scp,
           const std::vector<std::shared_ptr<Object>> &args) {
  if (args.size() == 2) {
    if (scp->scope_.find(
            std::dynamic_pointer_cast<SymbolNode>(args[0])->GetName()) !=
        scp->scope_.end()) {
      scp->scope_[std::dynamic_pointer_cast<SymbolNode>(args[0])->GetName()] =
          args[1]->Evaluate(scp);
      return shared_from_this();
    } else {
      throw NameError("Can't find variable");
    }
  } else {
    throw SyntaxError("not enough arguments");
  }
}
void Set::PrintTo(std::ostream *out) {}
std::shared_ptr<Object>
ListCmd::Apply(const std::shared_ptr<Scope> &scp,
               const std::vector<std::shared_ptr<Object>> &args) {
  if (!args.empty()) {
    auto root_node = std::make_shared<CellNode>();
    auto head = root_node;
    for (auto iter = args.begin(); iter != args.end(); ++iter) {
      head->SetFirst(*iter);
      if (iter != (args.end() - 1)) {
        head->SetSecond(std::make_shared<CellNode>());
        head = std::dynamic_pointer_cast<CellNode>(head->GetSecond());
      }
    }
    return root_node;
  } else {
    return nullptr;
  }
}
std::shared_ptr<Object>
ListTail::Apply(const std::shared_ptr<Scope> &scp,
                const std::vector<std::shared_ptr<Object>> &args) {
  auto iter_num = std::dynamic_pointer_cast<NumberNode>(args[1])->GetValue();
  auto cur_head = args[0]->Evaluate(scp);
  auto next_head = std::dynamic_pointer_cast<CellNode>(cur_head)->GetSecond();
  while (iter_num > 0 && next_head) {
    cur_head = next_head;
    next_head = std::dynamic_pointer_cast<CellNode>(cur_head)->GetSecond();
    --iter_num;
  }
  if (iter_num == 0) {
    return cur_head;
  } else if (iter_num == 1) {
    return nullptr;
  } else {
    throw RuntimeError("too big index");
  }
}
std::shared_ptr<Object>
ListRef::Apply(const std::shared_ptr<Scope> &scp,
               const std::vector<std::shared_ptr<Object>> &args) {

  auto iter_num = std::dynamic_pointer_cast<NumberNode>(args[1])->GetValue();
  auto cur_head = args[0]->Evaluate(scp);
  auto next_head = std::dynamic_pointer_cast<CellNode>(cur_head)->GetSecond();
  while (iter_num > 0 && next_head) {
    cur_head = next_head;
    next_head = std::dynamic_pointer_cast<CellNode>(cur_head)->GetSecond();
    --iter_num;
  }
  if (iter_num == 0) {
    return std::dynamic_pointer_cast<CellNode>(cur_head)->GetFirst();
  } else {
    throw RuntimeError("too big index");
  }
}

std::shared_ptr<Object>
Lambda::Apply(const std::shared_ptr<Scope> &scp,
              const std::vector<std::shared_ptr<Object>> &args) {
  if (args.size() >= 2) {
    std::shared_ptr<LambdaFunc> new_func = std::make_shared<LambdaFunc>();
    for (size_t i = 1; i < args.size(); ++i) {
      new_func->lambda_func.push_back(args[i]);
    }
    new_func->local_scope_ = std::make_shared<Scope>(*scp);
    new_func->params_ = ToVector(args[0]);
    return new_func;
  } else {
    throw SyntaxError("Invalid number of arguments in lambda function");
  }
}

std::shared_ptr<Object>
LambdaFunc::Apply(const std::shared_ptr<Scope> &scp,
                  const std::vector<std::shared_ptr<Object>> &args) {
  for (size_t i = 0; i < params_.size(); ++i) {
    local_scope_
        ->scope_[std::dynamic_pointer_cast<SymbolNode>(params_[i])->GetName()] =
        args[i]->Evaluate(scp);
  }
  auto original_scope = std::make_shared<Scope>(*local_scope_);
  local_scope_->scope_.insert(scp->scope_.begin(), scp->scope_.end());
  std::shared_ptr<Object> last_val = nullptr;
  for (auto &func : lambda_func) {
    last_val = func->Evaluate(local_scope_);
  }

  std::vector<std::string> keys_to_remove;
  for (auto &el : local_scope_->scope_) {
    if (original_scope->scope_.find(el.first) == local_scope_->scope_.end()) {
      keys_to_remove.push_back(el.first);
    }
  }
  for (auto &key : keys_to_remove) {
    local_scope_->scope_.erase(key);
  }

  //    local_scope_ = original_scope;
  return last_val;
}
// LambdaFunc::~LambdaFunc() {
//    if (local_scope_) {
//        local_scope_->scope_.clear();
//    }
//    params_.clear();
//    lambda_func.clear();
//}

////
//// Boolean
void Boolean::PrintTo(std::ostream *out) { *out << (val_ ? "#t" : "#f"); }
bool Boolean::GetVal() { return val_; }
Boolean::Boolean(bool val) : val_(val) {}

std::vector<std::shared_ptr<Object>>
ToVector(const std::shared_ptr<Object> &head) {
  std::vector<std::shared_ptr<Object>> elements;
  if (head) {
    elements.push_back(std::dynamic_pointer_cast<CellNode>(head)->GetFirst());
    auto cur_head = std::dynamic_pointer_cast<CellNode>(head)->GetSecond();
    while (cur_head && (!std::dynamic_pointer_cast<Function>(cur_head)) &&
           (!std::dynamic_pointer_cast<Syntax>(cur_head))) {
      elements.push_back(
          std::dynamic_pointer_cast<CellNode>(cur_head)->GetFirst());
      cur_head = std::dynamic_pointer_cast<CellNode>(cur_head)->GetSecond();
    }
  }
  return elements;
}

void Object::PrintTo(std::ostream *out) { throw RuntimeError("WTF?"); }
std::shared_ptr<Object> Object::Evaluate(std::shared_ptr<Scope> scp) {
  throw RuntimeError("Placeholder");
}
std::shared_ptr<Object>
Object::Apply(Scope &scp, std::vector<std::shared_ptr<Object>> &params) {
  throw RuntimeError("Placeholder");
}
std::shared_ptr<Object> NumberNode::Evaluate(std::shared_ptr<Scope> scp) {
  return shared_from_this();
}
void NumberNode::PrintTo(std::ostream *out) { *out << number_; }
int64_t NumberNode::GetValue() { return number_; }
NumberNode::NumberNode(int num) : number_(num) {}

std::shared_ptr<Object> ReadList(Tokenizer *tokenizer) {
  std::shared_ptr<CellNode> head = nullptr;
  std::shared_ptr<CellNode> tail = nullptr;
  auto cur_token = tokenizer->GetToken();
  auto bracket_token = std::get_if<BracketToken>(&cur_token);
  tokenizer->Next();
  cur_token = tokenizer->GetToken();
  bracket_token = std::get_if<BracketToken>(&cur_token);
  while ((!bracket_token || (*bracket_token) == BracketToken::OPEN) &&
         !tokenizer->IsEnd()) {
    if (auto dot_token = std::get_if<DotToken>(&cur_token)) {
      tokenizer->Next();
      auto token = Read(tokenizer);
      if (!tail && head) {
        head->SetSecond(token);
      } else if (head && tail) {
        tail->SetSecond(token);
      } else {
        throw SyntaxError("Help!");
      }
      cur_token = tokenizer->GetToken();
      bracket_token = std::get_if<BracketToken>(&cur_token);
      if (!bracket_token || (*bracket_token) != BracketToken::CLOSE ||
          tokenizer->IsEnd()) {
        throw SyntaxError("Pair finish error");
      }
      return head;
    } else {
      if (!head) {
        head = std::make_shared<CellNode>();
        head->SetFirst(Read(tokenizer));
      } else if (!tail) {
        tail = std::make_shared<CellNode>();
        tail->SetFirst(Read(tokenizer));
        head->SetSecond(tail);
      } else {
        auto token = std::make_shared<CellNode>();
        token->SetFirst(Read(tokenizer));
        tail->SetSecond(token);
        tail = token;
      }
    }
    cur_token = tokenizer->GetToken();
    bracket_token = std::get_if<BracketToken>(&cur_token);
  }
  if (tokenizer->IsEnd()) {
    throw SyntaxError{"Lost closing bracket"};
  }
  return head;
}

std::shared_ptr<Object> Read(Tokenizer *tokenizer) {
  auto cur_token = tokenizer->GetToken();
  if (auto val = std::get_if<ConstantToken>(&cur_token)) {
    auto new_node = std::make_shared<NumberNode>(val->value);
    tokenizer->Next();
    return new_node;
  } else if (auto val = std::get_if<SymbolToken>(&cur_token)) {
    tokenizer->Next();
    if (val->name != "\n") {
      auto new_node = std::make_shared<SymbolNode>(val->name);
      return new_node;
    }
    return Read(tokenizer);
  } else if (auto val = std::get_if<QuoteToken>(&cur_token)) {
    tokenizer->Next();
    auto cur_token = tokenizer->GetToken();
    auto bracket_token = std::get_if<BracketToken>(&cur_token);
    std::shared_ptr<Object> res_list;
    if (bracket_token) {
      res_list = ReadList(tokenizer);
    } else {
      res_list = std::make_shared<SymbolNode>(
          std::get_if<SymbolToken>(&cur_token)->name);
    }
    auto quote_node = std::make_shared<SymbolNode>("\'");
    auto root_node = std::make_shared<CellNode>();
    auto main_node_for_list = std::make_shared<CellNode>();
    root_node->SetFirst(quote_node);
    main_node_for_list->SetFirst(res_list);
    root_node->SetSecond(main_node_for_list);
    tokenizer->Next();
    return root_node;
  } else if (std::get_if<BracketToken>(&cur_token) &&
             (*std::get_if<BracketToken>(&cur_token)) == BracketToken::OPEN) {
    auto new_node = ReadList(tokenizer);
    tokenizer->Next();
    if (tokenizer->BracketBalance() < 0 ||
        (tokenizer->BracketBalance() != 0 && tokenizer->IsEnd())) {
      throw SyntaxError("No closing bracket");
    }
    return new_node;
  } else {
    throw SyntaxError{"Krivoi vvod"};
  }
}
