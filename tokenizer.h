#pragma once

#include <variant>
#include <iostream>
#include <vector>

struct SymbolToken {
    std::string name;
};

struct QuoteToken {};

struct DotToken {};

enum class BracketToken { OPEN, CLOSE };

struct ConstantToken {
    int value;
};

typedef std::variant<SymbolToken, ConstantToken, BracketToken, QuoteToken, DotToken> Token;

inline bool operator==(const SymbolToken& lhs, const SymbolToken& rhs) {
    return lhs.name == rhs.name;
}

inline bool operator==(const ConstantToken& lhs, const ConstantToken& rhs) {
    return lhs.value == rhs.value;
}

inline bool operator==(const QuoteToken& lhs, const QuoteToken& rhs) {
    return true;
}

inline bool operator==(const DotToken& lhs, const DotToken& rhs) {
    return true;
}

std::vector<Token> Read(const std::string& string);

class Tokenizer {
public:
    Tokenizer(std::istream* in) {
        in_stream_ = in;
        Next();
    }

    bool IsEnd() {
        return eof_;
    }

    void Next() {
        while (in_stream_->peek() == ' ') {
            in_stream_->get();
        }
        std::string current_char;
        if (std::isdigit(in_stream_->peek()) || in_stream_->peek() == '-' ||
            in_stream_->peek() == '+') {
            current_char += in_stream_->get();
            while (in_stream_->peek() != ' ' && in_stream_->peek() != EOF &&
                   std::isdigit(in_stream_->peek())) {
                current_char += in_stream_->get();
            }
            if (std::isdigit(current_char[0]) ||
                (current_char.size() > 1 && current_char[0] == '-') ||
                (current_char.size() > 1 && current_char[0] == '+')) {
                current_token_ = Token{ConstantToken{std::stoi(current_char)}};
            } else {
                current_token_ = Token(SymbolToken{current_char});
            }
        }
        //        else if (std::isalpha(in_stream_->peek())) {
        //            while (in_stream_->peek() != ' ' && in_stream_->peek() != EOF) {
        //                current_char += in_stream_->get();
        //            }
        //            current_token_ = Token(SymbolToken{current_char});
        //        }
        else if (in_stream_->peek() == EOF) {
            eof_ = true;
        } else {
            current_char = in_stream_->get();
            if (current_char[0] == '.') {
                current_token_ = Token(DotToken());
            } else if (current_char[0] == '(') {
                ++bracket_balance_;
                current_token_ = Token(BracketToken::OPEN);
            } else if (current_char[0] == ')') {
                --bracket_balance_;
                current_token_ = Token(BracketToken::CLOSE);
            } else if (current_char[0] == '\'') {
                current_token_ = Token(QuoteToken{});
            } else {
                while (in_stream_->peek() != ' ' && in_stream_->peek() != EOF &&
                       in_stream_->peek() != ')' && in_stream_->peek() != '(' && in_stream_->peek() != '\n') {
                    current_char += in_stream_->get();
                }
                current_token_ = Token(SymbolToken{current_char});
            }
        }
    }

    Token GetToken() {
        return current_token_;
    }

    int64_t BracketBalance() {
        return bracket_balance_;
    }

private:
    std::istream* in_stream_;
    Token current_token_;
    bool eof_ = false;
    int64_t bracket_balance_ = 0;
};
