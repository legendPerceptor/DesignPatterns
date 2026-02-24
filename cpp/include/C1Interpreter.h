/*
The intent is to given a language, define a representation for its grammar along
with an interpreter that uses the representation to interpret sentences in the language.

Key participants:
- AbstractExpression: Declares an abstract Interpret operation that is common to
  all nodes in the abstract syntax tree
- TerminalExpression: Implements an Interpret operation associated with terminal
  symbols in the grammar
- NonterminalExpression: Implements an Interpret operation for non-terminal symbols
- Context: Contains information global to the interpreter
- Client: Builds (or is given) an abstract syntax tree representing a particular
  sentence in the language; invokes the Interpret operation

Key benefits:
- Easier to change and extend the grammar
- Implementing the grammar is easy
- Complex grammars can be hard to maintain

Real-world example: A simple expression evaluator that can interpret mathematical
expressions like "5 + 3 - 2" or "10 * 2 / 5".
*/
#ifndef C1_INTERPRETER_H
#define C1_INTERPRETER_H

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace Interpreter {

// ----- Context -----
class Context {
    public:
    void setVariable (const std::string& name, int value) {
        variables_[name] = value;
    }

    int getVariable (const std::string& name) const {
        auto it = variables_.find (name);
        if (it != variables_.end ()) {
            return it->second;
        }
        return 0; // Default value
    }

    private:
    std::map<std::string, int> variables_;
};

// ----- Abstract Expression -----
class Expression {
    public:
    virtual ~Expression ()                   = default;
    virtual int interpret (Context& context) = 0;
};

// ----- Number Expression (Terminal) -----
class NumberExpression : public Expression {
    public:
    explicit NumberExpression (int value) : value_ (value) {
    }

    int interpret (Context& context) override {
        return value_;
    }

    private:
    int value_;
};

// ----- Variable Expression (Terminal) -----
class VariableExpression : public Expression {
    public:
    explicit VariableExpression (const std::string& name) : name_ (name) {
    }

    int interpret (Context& context) override {
        return context.getVariable (name_);
    }

    private:
    std::string name_;
};

// ----- Add Expression (Non-Terminal) -----
class AddExpression : public Expression {
    public:
    AddExpression (std::shared_ptr<Expression> left, std::shared_ptr<Expression> right)
    : left_ (left), right_ (right) {
    }

    int interpret (Context& context) override {
        return left_->interpret (context) + right_->interpret (context);
    }

    private:
    std::shared_ptr<Expression> left_;
    std::shared_ptr<Expression> right_;
};

// ----- Subtract Expression (Non-Terminal) -----
class SubtractExpression : public Expression {
    public:
    SubtractExpression (std::shared_ptr<Expression> left, std::shared_ptr<Expression> right)
    : left_ (left), right_ (right) {
    }

    int interpret (Context& context) override {
        return left_->interpret (context) - right_->interpret (context);
    }

    private:
    std::shared_ptr<Expression> left_;
    std::shared_ptr<Expression> right_;
};

// ----- Multiply Expression (Non-Terminal) -----
class MultiplyExpression : public Expression {
    public:
    MultiplyExpression (std::shared_ptr<Expression> left, std::shared_ptr<Expression> right)
    : left_ (left), right_ (right) {
    }

    int interpret (Context& context) override {
        return left_->interpret (context) * right_->interpret (context);
    }

    private:
    std::shared_ptr<Expression> left_;
    std::shared_ptr<Expression> right_;
};

// ----- Divide Expression (Non-Terminal) -----
class DivideExpression : public Expression {
    public:
    DivideExpression (std::shared_ptr<Expression> left, std::shared_ptr<Expression> right)
    : left_ (left), right_ (right) {
    }

    int interpret (Context& context) override {
        int divisor = right_->interpret (context);
        if (divisor == 0) {
            std::cout << "Error: Division by zero!\n";
            return 0;
        }
        return left_->interpret (context) / divisor;
    }

    private:
    std::shared_ptr<Expression> left_;
    std::shared_ptr<Expression> right_;
};

// ----- Boolean Expression Example -----
class BooleanExpression {
    public:
    virtual ~BooleanExpression ()            = default;
    virtual bool evaluate (Context& context) = 0;
    virtual std::shared_ptr<BooleanExpression>
    replace (const std::string& name, BooleanExpression& expr) = 0;
    virtual std::shared_ptr<BooleanExpression> copy ()         = 0;
};

// Variable for boolean expressions
class Variable : public BooleanExpression {
    public:
    explicit Variable (const std::string& name) : name_ (name) {
    }

    bool evaluate (Context& context) override {
        return context.getVariable (name_) != 0;
    }

    std::shared_ptr<BooleanExpression>
    replace (const std::string& name, BooleanExpression& expr) override {
        if (name_ == name) {
            return std::make_shared<Variable> (dynamic_cast<Variable&> (expr).name_);
        }
        return std::make_shared<Variable> (name_);
    }

    std::shared_ptr<BooleanExpression> copy () override {
        return std::make_shared<Variable> (name_);
    }

    private:
    std::string name_;
};

// And expression
class And : public BooleanExpression {
    public:
    And (std::shared_ptr<BooleanExpression> left, std::shared_ptr<BooleanExpression> right)
    : left_ (left), right_ (right) {
    }

    bool evaluate (Context& context) override {
        return left_->evaluate (context) && right_->evaluate (context);
    }

    std::shared_ptr<BooleanExpression>
    replace (const std::string& name, BooleanExpression& expr) override {
        return std::make_shared<And> (
        left_->replace (name, expr), right_->replace (name, expr));
    }

    std::shared_ptr<BooleanExpression> copy () override {
        return std::make_shared<And> (left_->copy (), right_->copy ());
    }

    private:
    std::shared_ptr<BooleanExpression> left_;
    std::shared_ptr<BooleanExpression> right_;
};

// Or expression
class Or : public BooleanExpression {
    public:
    Or (std::shared_ptr<BooleanExpression> left, std::shared_ptr<BooleanExpression> right)
    : left_ (left), right_ (right) {
    }

    bool evaluate (Context& context) override {
        return left_->evaluate (context) || right_->evaluate (context);
    }

    std::shared_ptr<BooleanExpression>
    replace (const std::string& name, BooleanExpression& expr) override {
        return std::make_shared<Or> (
        left_->replace (name, expr), right_->replace (name, expr));
    }

    std::shared_ptr<BooleanExpression> copy () override {
        return std::make_shared<Or> (left_->copy (), right_->copy ());
    }

    private:
    std::shared_ptr<BooleanExpression> left_;
    std::shared_ptr<BooleanExpression> right_;
};

// Not expression
class Not : public BooleanExpression {
    public:
    explicit Not (std::shared_ptr<BooleanExpression> expr) : expr_ (expr) {
    }

    bool evaluate (Context& context) override {
        return !expr_->evaluate (context);
    }

    std::shared_ptr<BooleanExpression>
    replace (const std::string& name, BooleanExpression& expr) override {
        return std::make_shared<Not> (expr_->replace (name, expr));
    }

    std::shared_ptr<BooleanExpression> copy () override {
        return std::make_shared<Not> (expr_->copy ());
    }

    private:
    std::shared_ptr<BooleanExpression> expr_;
};

// ----- Simple Expression Parser -----
// A simple parser that builds the abstract syntax tree
class ExpressionParser {
    public:
    static std::shared_ptr<Expression> parse (const std::string& expression) {
        // This is a simplified parser for expressions like "5 + 3 - 2"
        // In a real implementation, you'd use proper parsing techniques
        std::vector<std::string> tokens = tokenize (expression);
        return parseAddSubtract (tokens, 0);
    }

    private:
    static std::vector<std::string> tokenize (const std::string& expr) {
        std::vector<std::string> tokens;
        std::string current;
        for (char c : expr) {
            if (std::isspace (c)) {
                if (!current.empty ()) {
                    tokens.push_back (current);
                    current.clear ();
                }
            } else if (c == '+' || c == '-' || c == '*' || c == '/') {
                if (!current.empty ()) {
                    tokens.push_back (current);
                    current.clear ();
                }
                tokens.push_back (std::string (1, c));
            } else {
                current += c;
            }
        }
        if (!current.empty ()) {
            tokens.push_back (current);
        }
        return tokens;
    }

    static std::shared_ptr<Expression>
    parseAddSubtract (std::vector<std::string>& tokens, size_t pos) {
        auto left = parseMultiplyDivide (tokens, pos);

        while (pos < tokens.size () && (tokens[pos] == "+" || tokens[pos] == "-")) {
            std::string op = tokens[pos++];
            auto right     = parseMultiplyDivide (tokens, pos);

            if (op == "+") {
                left = std::make_shared<AddExpression> (left, right);
            } else {
                left = std::make_shared<SubtractExpression> (left, right);
            }
        }

        return left;
    }

    static std::shared_ptr<Expression>
    parseMultiplyDivide (std::vector<std::string>& tokens, size_t pos) {
        auto left = parsePrimary (tokens, pos);

        while (pos < tokens.size () && (tokens[pos] == "*" || tokens[pos] == "/")) {
            std::string op = tokens[pos++];
            auto right     = parsePrimary (tokens, pos);

            if (op == "*") {
                left = std::make_shared<MultiplyExpression> (left, right);
            } else {
                left = std::make_shared<DivideExpression> (left, right);
            }
        }

        return left;
    }

    static std::shared_ptr<Expression>
    parsePrimary (std::vector<std::string>& tokens, size_t pos) {
        if (pos >= tokens.size ()) {
            return std::make_shared<NumberExpression> (0);
        }

        std::string token = tokens[pos++];
        try {
            int value = std::stoi (token);
            return std::make_shared<NumberExpression> (value);
        } catch (...) {
            return std::make_shared<VariableExpression> (token);
        }
    }
};

} // namespace Interpreter

#endif
