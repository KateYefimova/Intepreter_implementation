#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <functional>
#include <cmath>
#include <stdexcept>
#include <stack>
#include <cctype>

using namespace std;

map<string, function<double(double, double)>> functions;

double evaluateExpression(const string& expression) {
    stringstream ss(expression);
    stack<double> numbers;
    stack<char> operations;
    double num;
    char op;

    auto applyOp = [](double a, double b, char op) -> double {
        switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': if (b == 0) throw runtime_error("Division by zero"); return a / b;
        default: throw runtime_error("Unknown operator");
        }
        };

    while (ss >> num) {
        numbers.push(num);
        if (ss >> op) {
            while (!operations.empty() && ((op == '+' || op == '-') || (operations.top() == '*' || operations.top() == '/'))) {
                double b = numbers.top(); numbers.pop();
                double a = numbers.top(); numbers.pop();
                char op2 = operations.top(); operations.pop();
                numbers.push(applyOp(a, b, op2));
            }
            operations.push(op);
        }
    }

    while (!operations.empty()) {
        double b = numbers.top(); numbers.pop();
        double a = numbers.top(); numbers.pop();
        char op2 = operations.top(); operations.pop();
        numbers.push(applyOp(a, b, op2));
    }

    return numbers.empty() ? 0 : numbers.top();
}


double evaluateFunction(const string& func, double arg1, double arg2) {
    if (functions.find(func) != functions.end()) {
        return functions[func](arg1, arg2);
    }
    else {
        throw runtime_error("Unknown function: " + func);
    }
}


string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, last - first + 1);
}

double parseAndEvaluate(const string& input) {
    string trimmedInput = trim(input);
    size_t pos = trimmedInput.find('(');
    if (pos != string::npos) {
        string func = trimmedInput.substr(0, pos);
        size_t pos2 = trimmedInput.find(',', pos);
        if (pos2 == string::npos) {
            throw runtime_error("Invalid function format: " + trimmedInput);
        }
        double arg1 = evaluateExpression(trim(trimmedInput.substr(pos + 1, pos2 - pos - 1)));
        double arg2 = evaluateExpression(trim(trimmedInput.substr(pos2 + 1, trimmedInput.find(')') - pos2 - 1)));
        return evaluateFunction(func, arg1, arg2);
    }
    else {
        return evaluateExpression(trimmedInput);
    }
}

int main() {

    functions["pow"] = [](double x, double y) { return pow(x, y); };
    functions["abs"] = [](double x, double) { return abs(x); };
    functions["max"] = [](double x, double y) { return max(x, y); };
    functions["min"] = [](double x, double y) { return min(x, y); };


    try {
        string expr1 = "5 + 3 * 2";
        string expr2 = "max(5, 2)";
        string expr3 = "max(min(3 * 2, 2), 2)";

        cout << expr1 << " = " << parseAndEvaluate(expr1) << endl;
        cout << expr2 << " = " << parseAndEvaluate(expr2) << endl;
        cout << expr3 << " = " << parseAndEvaluate(expr3) << endl;
    }
    catch (const runtime_error& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}
