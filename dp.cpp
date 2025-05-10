#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

using Clause = set<string>;
using CNF = vector<Clause>;


Clause parseClause(const string& str) {
    Clause clause;
    stringstream ss(str);
    string lit;
    while (ss >> lit) clause.insert(lit);
    return clause;
}


string getVar(const string& lit) {
    return (lit[0] == '-') ? lit.substr(1) : lit;
}


string negate(const string& lit) {
    return (lit[0] == '-') ? lit.substr(1) : "-" + lit;
}


CNF resolveVariable(const CNF& formula, const string& var) {
    CNF result;
    vector<Clause> pos, neg;

    for (const auto& clause : formula) {
        if (clause.count(var)) pos.push_back(clause);
        else if (clause.count("-" + var)) neg.push_back(clause);
        else result.push_back(clause);  
    }

    for (const auto& c1 : pos) {
        for (const auto& c2 : neg) {
            Clause resolvent = c1;
            resolvent.insert(c2.begin(), c2.end());
            resolvent.erase(var);
            resolvent.erase("-" + var);
            result.push_back(resolvent);
        }
    }

    return result;
}


int checkCNF(const CNF& formula) {
    for (const auto& clause : formula)
        if (clause.empty()) return -1;  
    if (formula.empty()) return 1;     
    return 0;                        
}


set<string> getVariables(const CNF& formula) {
    set<string> vars;
    for (const auto& clause : formula)
        for (const auto& lit : clause)
            vars.insert(getVar(lit));
    return vars;
}


bool davisPutnam(CNF formula) {
    while (true) {
        int status = checkCNF(formula);
        if (status == -1) return false;
        if (status == 1) return true;

        set<string> vars = getVariables(formula);
        if (vars.empty()) return true;

        string var = *vars.begin();  
        formula = resolveVariable(formula, var);
    }
}

int main() {
    vector<vector<string>> testCases = {
        {"A B", "-A", "-B"},        
        {"A B", "-A C"},          
        {"A", "-A"},               
        {"A B C", "-B C"}          
    };

    for (size_t i = 0; i < testCases.size(); ++i) {
        CNF formula;
        for (const auto& clauseStr : testCases[i])
            formula.push_back(parseClause(clauseStr));

        cout << "Test Case " << i + 1 << ": "
             << (davisPutnam(formula) ? "SATISFIABLE" : "UNSATISFIABLE") << endl;
    }

    return 0;
}
