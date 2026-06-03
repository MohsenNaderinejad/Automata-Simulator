#pragma once
#include"GLOBAL_VARIABLES.hpp"

struct DPDATransitionKey {
    std::string start_state;
    std::string input_symb;
    char stack_top;

    bool operator==(const DPDATransitionKey& other) const;

};

struct DPDATransitionValue {
    std::string next_state;
    std::string push_string;
};

struct KeyHash {
    std::size_t operator()(const DPDATransitionKey& k) const;
};

class DPDA {
private:
    std::set<std::string> states;
    std::set<char> alphabet;
    std::set<char> stack_alphabet;
    std::string initial_state;
    char initial_stack_symbol;
    std::set<std::string> final_states;
    std::string acceptance_mode;

    std::unordered_map<DPDATransitionKey, DPDATransitionValue, KeyHash> transition_function;
    std::stack<char> execution_stack;

    void check_transition_legitimacy(const std::string& start_state, const std::string& symb, const char& stack_top,
                                     const std::string& end_state, const std::string& push_str);

    bool check_input_legitimacy(const std::string& input);

    void check_acceptance(const std::string& last_state);
    void print_transition(const char& symb, const char& stack_char, const std::string& push_str);

    void reject_message();
    void accept_message();
    std::string stack_output();
    void print_state_stack_output(const std::string& state);

public:
    DPDA(const std::set<std::string>& states, const std::set<char>& alphabet, 
        const std::set<char>& stack_alphabet, const std::string& initial_state, 
        const char& initial_stack_symbol, const std::set<std::string>& final_states,
        const std::string& acceptance_mode);

    void add_transition(const std::string& start_state, const std::string& symb, const char& stack_top,
                        const std::string& end_state, const std::string& push_str);
    void operator()(const std::string& input);
};