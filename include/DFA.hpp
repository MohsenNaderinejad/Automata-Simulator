#pragma once
#include"GLOBAL_VARIABLES.hpp"

class DFA {
private:
    std::set<std::string> states;
    std::set<char> alphabet;
    std::string initial_state;
    std::set<std::string> final_states;

    std::unordered_map<std::string, int> state_to_id;
    std::vector<std::string> id_to_state;
    std::unordered_map<char, int> symbol_to_index;
    int dead_state_id;

    int** transition_matrix;

    void encoding_states();
    void encoding_symbols();
    void encoding();

    void check_transition_legitimacy(const std::string& start_state, const char& symb, 
        const std::string& end_state);

    bool check_input_legitimacy(const std::string& input);

    int transition(int &current_state_id, const char &symb);
    bool check_dead_state_halt(int& current_state_id);
    void check_final_state_halt(int& end_of_input_state_id);
    std::string transition_output(const std::string& i_state, const char& symb, const std::string& j_state);
    void reject_message();
    void accept_message();

public:
    DFA(const std::set<std::string>& states, const std::set<char>& alphabet, 
        const std::string& intital_state, const std::set<std::string>& final_states);
    ~DFA();

    void unreachable_states();
    void dead_states();
    void check_emptiness();

    void add_dead_state_transitions();
    void add_transition(const std::string& start_state, const char& symb, 
        const std::string& end_state);
    void operator()(const std::string& input);
};