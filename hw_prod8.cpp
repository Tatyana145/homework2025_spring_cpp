#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <unordered_map>

class Expression;
class Constant;
class Variable;
class BinaryOperator;
class Addition;
class Subtraction;
class Multiplication;
class Division;


// Expression
class Expression {
public:
    virtual ~Expression() {}
    virtual double calculate(const std::map<std::string, double>& context) const = 0;
    virtual void print(std::ostream& os) const = 0;
};

std::ostream& operator<<(std::ostream& os, const Expression& expr) {
    expr.print(os);
    return os;
}

// Constant
class Constant : public Expression {
private:
    double value;

public:
    Constant(double value) : value(value) {}
    double calculate(const std::map<std::string, double>& context) const override {
        return value;
    }
    void print(std::ostream& os) const override {
        os << value;
    }
};

// Variable
class Variable : public Expression {
private:
    std::string name;

public:
    Variable(const std::string& name) : name(name) {}
    double calculate(const std::map<std::string, double>& context) const override {
        if (context.count(name)) {
            return context.at(name);
        } else {
            std::cerr << "Error: Variable '" << name << "' not found in context." << std::endl;
            return 0.0;
        }
    }
    void print(std::ostream& os) const override {
        os << name;
    }
};

// BinaryOperator
class BinaryOperator : public Expression {
protected:
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;

public:
    BinaryOperator(Expression* left, Expression* right) : left(left), right(right) {}
    virtual ~BinaryOperator() {}
};

// 6. Concrete Operators (Composite)
class Addition : public BinaryOperator {
public:
    Addition(Expression* left, Expression* right) : BinaryOperator(left, right) {}
    double calculate(const std::map<std::string, double>& context) const override {
        return left->calculate(context) + right->calculate(context);
    }
    void print(std::ostream& os) const override {
        os << "(";
        left->print(os);
        os << " + ";
        right->print(os);
        os << ")";
    }
};

class Subtraction : public BinaryOperator {
public:
    Subtraction(Expression* left, Expression* right) : BinaryOperator(left, right) {}
    double calculate(const std::map<std::string, double>& context) const override {
        return left->calculate(context) - right->calculate(context);
    }
    void print(std::ostream& os) const override {
        os << "(";
        left->print(os);
        os << " - ";
        right->print(os);
        os << ")";
    }
};

class Multiplication : public BinaryOperator {
public:
    Multiplication(Expression* left, Expression* right) : BinaryOperator(left, right) {}
    double calculate(const std::map<std::string, double>& context) const override {
        return left->calculate(context) * right->calculate(context);
    }
    void print(std::ostream& os) const override {
        os << "(";
        left->print(os);
        os << " * ";
        right->print(os);
        os << ")";
    }
};

class Division : public BinaryOperator {
public:
    Division(Expression* left, Expression* right) : BinaryOperator(left, right) {}
    double calculate(const std::map<std::string, double>& context) const override {
        double rightValue = right->calculate(context);
        if (rightValue == 0) {
            std::cerr << "Error: Division by zero" << std::endl;
            return 0.0;
        }
        return left->calculate(context) / rightValue;
    }
    void print(std::ostream& os) const override {
        os << "(";
        left->print(os);
        os << " / ";
        right->print(os);
        os << ")";
    }
};


// ExpressionFactory
class ExpressionFactory {
private:
    std::unordered_map<double, Constant*> constants;
    std::unordered_map<std::string, Variable*> variables;

    ExpressionFactory() {
        // Pre-create constants from -5 to 256
        for (double i = -5; i <= 256; ++i) {
            constants[i] = new Constant(i);
        }
    }

    ~ExpressionFactory() {
        for (auto& pair : variables) {
            delete pair.second;
        }
    }

public:
    static ExpressionFactory& getInstance() {
        static ExpressionFactory instance;
        return instance;
    }

    Constant* createConstant(double value) {
        if (constants.count(value)) {
            return constants[value];
        }
        
        // Create new (and not reused) constants
        return new Constant(value);
    }

    Variable* createVariable(const std::string& name) {
        if (variables.count(name)) {
            return variables[name];
        }
        Variable* var = new Variable(name);
        variables[name] = var;
        return var;
    }

    void removeVariable(const std::string& name) {
        auto it = variables.find(name);
        if (it != variables.end()) {
            delete it->second;
            variables.erase(it);
        }
    }
};

int main() {
    ExpressionFactory& factory = ExpressionFactory::getInstance();
    Constant* c = factory.createConstant(2);
    Variable* v = factory.createVariable("x");
    Addition* expression = new Addition(c, v);
    std::map<std::string, double> context;
    context["x"] = 3;
    std::cout << "Expression: " << *expression << std::endl;
    std::cout << "Result: " << expression->calculate(context) << std::endl;
    delete expression;

    // Test with a pre-created constant
    Constant* c2 = factory.createConstant(2);
    if (c == c2) {
        std::cout << "Constant flyweight works!" << std::endl;
    }

    Variable* v2 = factory.createVariable("y");
    std::map<std::string, double> context2;
    context2["x"] = 5;
    context2["y"] = 10;
    Addition* expression2 = new Addition(v, v2);
    std::cout << "Expression: " << *expression2 << std::endl;
    std::cout << "Result: " << expression2->calculate(context2) << std::endl;

    factory.removeVariable("y");
    delete expression2;
    return 0;
}