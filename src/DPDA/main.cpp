#include "../../include/DPDA.hpp"

// ----------------- Input Helpers -----------------

std::set<std::string> read_states() {
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

std::set<char> read_alphabet() {
    std::cout << "Input alphabet: ";
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

std::set<char> read_stack_alphabet() {
    std::cout << "Stack alphabet: ";
    std::set<char> stack_alphabet;
    std::string line;
    std::getline(std::cin, line);
    std::istringstream iss(line);
    std::string token;
    while (iss >> token) {
        stack_alphabet.emplace(token[0]);
    }
    return stack_alphabet;
}

std::string read_initial_state() {
    std::cout << "Start state: ";
    std::string initial_state;
    std::getline(std::cin, initial_state);
    return initial_state;
}

char read_initial_stack_symbol() {
    std::cout << "Initial stack symbol: ";
    std::string line;
    std::getline(std::cin, line);
    return line[0];
}

std::set<std::string> read_final_states() {
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

std::string read_acceptance_mode() {
    std::cout << "Acceptance mode (final / empty): ";
    std::string mode;
    std::getline(std::cin, mode);
    return mode;
}

int read_num_transitions() {
    std::cout << "\nNumber of transitions: ";
    int num_transitions;
    std::cin >> num_transitions;
    std::cin.ignore();
    return num_transitions;
}

void read_transitions(DPDA& dpda, int num_transitions) {
    for (int i = 0; i < num_transitions; i++) {
        std::string start_state, symb, stack_top_str, end_state, push_str;
        std::cin >> start_state >> symb >> stack_top_str >> end_state >> push_str;
        dpda.add_transition(start_state, symb, stack_top_str[0], end_state, push_str);
    }
    std::cin.ignore();
}

int read_num_test_strings() {
    std::cout << "\nNumber of test strings: ";
    int num_strings;
    std::cin >> num_strings;
    std::cin.ignore();
    return num_strings;
}

std::vector<std::string> read_test_strings(int num_strings) {
    std::vector<std::string> test_strings;
    for (int i = 0; i < num_strings; i++) {
        std::string input;
        std::getline(std::cin, input);
        test_strings.emplace_back(input);
    }
    return test_strings;
}

// ----------------- Main -----------------

int main() {
    try {
        std::set<std::string> states = read_states();
        std::set<char> alphabet = read_alphabet();
        std::set<char> stack_alphabet = read_stack_alphabet();
        std::string initial_state = read_initial_state();
        char initial_stack_symbol = read_initial_stack_symbol();
        std::set<std::string> final_states = read_final_states();
        std::string acceptance_mode = read_acceptance_mode();

        DPDA dpda(states, alphabet, stack_alphabet, initial_state,
                  initial_stack_symbol, final_states, acceptance_mode);

        int num_transitions = read_num_transitions();
        read_transitions(dpda, num_transitions);

        int num_strings = read_num_test_strings();
        std::vector<std::string> test_strings = read_test_strings(num_strings);

        std::cout << "\n--- Running Strings ---\n";
        for (const auto& input : test_strings) {
            std::cout << "--------------------\n";
            dpda(input);
        }
        std::cout << "--------------------\n";

    } catch (const AutomataError& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }

    return 0;
}