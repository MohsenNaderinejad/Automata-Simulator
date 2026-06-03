#include"../../include/DFA.hpp"

// ----------------- Input Helpers -----------------

std::set<std::string> read_states(){
    std::cout << "States: ";
    std::set<std::string> states;
    std::string line;
    std::getline(std::cin, line);
    std::istringstream iss(line);
    std::string token;
    while (iss >> token) {
        states.emplace(token);
    }
    return states;
}

std::set<char> read_alphabet(){
    std::cout << "Alphabet: ";
    std::set<char> alphabet;
    std::string line;
    std::getline(std::cin, line);
    std::istringstream iss(line);
    std::string token;
    while (iss >> token) {
        alphabet.emplace(token[0]);
    }
    return alphabet;
}

std::string read_initial_state(){
    std::cout << "Start state: ";
    std::string initial_state;
    std::getline(std::cin, initial_state);
    return initial_state;
}

std::set<std::string> read_final_states(){
    std::cout << "Final states: ";
    std::set<std::string> final_states;
    std::string line;
    std::getline(std::cin, line);
    std::istringstream iss(line);
    std::string token;
    while (iss >> token) {
        final_states.emplace(token);
    }
    return final_states;
}

int read_num_transitions(){
    std::cout << "\nNumber of transitions: ";
    int num_transitions;
    std::cin >> num_transitions;
    std::cin.ignore();
    return num_transitions;
}

void read_transitions(DFA& dfa, int num_transitions){
    for (int i = 0; i < num_transitions; i++) {
        std::string start_state, end_state, symb_str;
        std::cin >> start_state >> symb_str >> end_state;
        dfa.add_transition(start_state, symb_str[0], end_state);
    }
    std::cin.ignore();
}

int read_num_test_strings(){
    std::cout << "\nNumber of test strings: ";
    int num_strings;
    std::cin >> num_strings;
    std::cin.ignore();
    return num_strings;
}

std::vector<std::string> read_test_strings(int num_strings){
    std::vector<std::string> test_strings;
    for (int i = 0; i < num_strings; i++) {
        std::string input;
        std::getline(std::cin, input);
        test_strings.emplace_back(input);
    }
    return test_strings;
}

// ----------------- Main -----------------

int main(){
    try {
        std::set<std::string> states = read_states();
        std::set<char> alphabet = read_alphabet();
        std::string initial_state = read_initial_state();
        std::set<std::string> final_states = read_final_states();

        DFA dfa(states, alphabet, initial_state, final_states);

        int num_transitions = read_num_transitions();
        read_transitions(dfa, num_transitions);

        std::cout << "\n--- DFA Analysis ---\n";
        dfa.unreachable_states();
        dfa.dead_states();
        dfa.check_emptiness();
        std::cout << "\n";
        dfa.add_dead_state_transitions();

        int num_strings = read_num_test_strings();
        std::vector<std::string> test_strings = read_test_strings(num_strings);

        std::cout << "\n--- Running Strings ---\n";
        for (const auto& input : test_strings) {
            std::cout << "--------------------\n";
            dfa(input);
        }
        std::cout << "--------------------\n";

    } catch (const AutomataError& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }

    return 0;
}