#include "../../include/DPDA.hpp"

bool DPDATransitionKey::operator==(const DPDATransitionKey& other) const {
    return start_state == other.start_state &&
            input_symb == other.input_symb &&
            stack_top == other.stack_top;
}

std::size_t KeyHash::operator()(const DPDATransitionKey& k) const {
    std::size_t h1 = std::hash<std::string>()(k.start_state);
    std::size_t h2 = std::hash<std::string>()(k.input_symb);
    std::size_t h3 = std::hash<char>()(k.stack_top);
    return h1 ^ (h2 << 1) ^ (h3 << 2);
}

DPDA::DPDA(const std::set<std::string>& states, const std::set<char>& alphabet, 
        const std::set<char>& stack_alphabet, const std::string& initial_state, 
        const char& initial_stack_symbol, const std::set<std::string>& final_states,
        const std::string& acceptance_mode) {
    
    bool initial_state_found = states.find(initial_state) != states.end();
    if (!initial_state_found) {
        throw InitialStateNotFound(initial_state);
    }

    std::vector<std::string> final_states_not_in_states;
    for (const auto& state : final_states) {
        if (states.find(state) == states.end()) {
            final_states_not_in_states.emplace_back(state);
        }
    }
    if (stack_alphabet.find(initial_stack_symbol) == stack_alphabet.end()) {
        throw InvaildStartStackSymbol(std::string(1, initial_stack_symbol));
    }
    if (final_states_not_in_states.size() != 0) {
        throw FinalStatesNotFound(final_states_not_in_states);
    }
    if (acceptance_mode != "final" && acceptance_mode != "empty") {
        throw InvalidAcceptanceMode(acceptance_mode);
    }

    this->states = std::move(states);
    this->alphabet = std::move(alphabet);
    this->stack_alphabet = std::move(stack_alphabet);
    this->initial_state = std::move(initial_state);
    this->initial_stack_symbol = std::move(initial_stack_symbol);
    this->final_states = std::move(final_states);
    this->acceptance_mode = std::move(acceptance_mode);
}

void DPDA::add_transition(const std::string& start_state, const std::string& symb, const char& stack_top,
const std::string& end_state, const std::string& push_str) {
    check_transition_legitimacy(start_state, symb, stack_top, end_state, push_str);
    DPDATransitionKey key_hash{start_state, symb, stack_top};
    this->transition_function[key_hash] = DPDATransitionValue{end_state, push_str};
}

void DPDA::check_transition_legitimacy(const std::string& start_state, const std::string& symb, const char& stack_top,
                                     const std::string& end_state, const std::string& push_str) {
    bool start_state_found = this->states.find(start_state) != this->states.end();
    bool end_state_found = this->states.find(end_state) != this->states.end();
    if (!start_state_found || !end_state_found) {
        std::tuple<std::string, std::string, std::string, std::string, std::string> transition(
            start_state, symb, std::string(1, stack_top), end_state, push_str
        );
        throw InvalidTransitionStartEndDPDA(transition);
    }

    if (push_str != "eps") {
        for (const char& ch : push_str) {
            if (this->stack_alphabet.find(ch) == this->stack_alphabet.end()) {
                std::tuple<std::string, std::string, std::string, std::string, std::string> transition(
                    start_state, symb, std::string(1, stack_top), end_state, push_str
                );
                throw InvalidTransitionPushString(transition);
            }
        }
    }

    if (this->stack_alphabet.find(stack_top) == this->stack_alphabet.end()) {
        std::tuple<std::string, std::string, std::string, std::string, std::string> transition(
            start_state, symb, std::string(1, stack_top), end_state, push_str
        );
        throw InvalidTransitionStackTop(transition);
    }

    if (symb != "eps") {
        bool symb_found = this->alphabet.find(symb[0]) != this->alphabet.end();
        if(!symb_found){
            std::tuple<std::string, std::string, std::string, std::string, std::string> transition(
                start_state, symb, std::string(1, stack_top), end_state, push_str
            );
            throw InvalidTransitionSymbolDPDA(transition);
        }
    }

    DPDATransitionKey key_hash{start_state, symb, stack_top};
    if (this->transition_function.find(key_hash) != this->transition_function.end()) {
        std::tuple<std::string, std::string, std::string, std::string, std::string> transition(
            start_state, symb, std::string(1, stack_top), end_state, push_str
        );
        throw PreviouslyDefinedTransition(transition);
    }

    if (symb == "eps") {
        for (const char& a : this->alphabet) {
            DPDATransitionKey conflicting_key{start_state, std::string(1, a), stack_top};
            if (this->transition_function.find(conflicting_key) != this->transition_function.end()) {
                std::tuple<std::string, std::string, std::string, std::string, std::string> transition(
                    start_state, symb, std::string(1, stack_top), end_state, push_str
                );
                throw PreviouslyDefinedTransition(transition);
            }
        }
    } else {
        DPDATransitionKey conflicting_lambda_key{start_state, "eps", stack_top};
        if (this->transition_function.find(conflicting_lambda_key) != this->transition_function.end()) {
            std::tuple<std::string, std::string, std::string, std::string, std::string> transition(
                start_state, symb, std::string(1, stack_top), end_state, push_str
            );
            throw LambdaTransitionExists(transition);
        }
    }
}

bool DPDA::check_input_legitimacy(const std::string& input) {
    for (const auto& symb : input){
        if (this->alphabet.find(symb) == this->alphabet.end()){
            std::cout << "The input string contains " + std::string(1, symb) + " which is not in the alphabet."
            "\nTherefore input skipped.\n";
            return false;
        }
    }
    return true;
}

void DPDA::operator()(const std::string& input) {
    std::cout << "Input string: " + input + "\n";
    std::cout << "Acceptance mode: " + this->acceptance_mode + "\n\n";
    bool allowed = check_input_legitimacy(input);
    if (allowed) {
        while (!this->execution_stack.empty()) {
            this->execution_stack.pop();
        }
        this->execution_stack.push(this->initial_stack_symbol);
        this->print_state_stack_output(this->initial_state);
        std::string current_state = this->initial_state;
        size_t index = 0;
        while (true) {
            std::cout << "\n";
            if (this->execution_stack.empty()) {
                if (index < input.length()) {
                    this->reject_message();
                } else {
                    this->check_acceptance(current_state);
                }
                return;
            }
            char current_stack_top = this->execution_stack.top();
            char symb = (index < input.length()) ? input[index] : '\0';

            DPDATransitionKey lambda_key{current_state, "eps", current_stack_top};
            auto current_val = this->transition_function.find(lambda_key);

            if (current_val != this->transition_function.end()) {
                DPDATransitionValue new_value = current_val->second;
                this->print_transition('\0', current_stack_top, new_value.push_string);
                current_state = new_value.next_state;
                this->execution_stack.pop();
                
                if (new_value.push_string != "eps") {
                    for (auto push_symb = new_value.push_string.rbegin(); push_symb != new_value.push_string.rend();) {
                        this->execution_stack.push(*push_symb);
                        ++push_symb;
                    }
                }
                this->print_state_stack_output(current_state);
            }
            else if (index < input.length()) {
                DPDATransitionKey current_key{current_state, std::string(1, symb), current_stack_top};
                current_val = this->transition_function.find(current_key);

                if (current_val != this->transition_function.end()) {
                    DPDATransitionValue new_value = current_val->second;
                    this->print_transition(symb, current_stack_top, new_value.push_string);
                    current_state = new_value.next_state;
                    this->execution_stack.pop();
                    
                    if (new_value.push_string != "eps") {
                        for (auto push_symb = new_value.push_string.rbegin(); push_symb != new_value.push_string.rend();) {
                            this->execution_stack.push(*push_symb);
                            ++push_symb;
                        }
                    }
                    this->print_state_stack_output(current_state);
                    index++;
                }
                else {
                    this->reject_message();
                    return;
                }
            }
            else {
                this->check_acceptance(current_state);
                return;
            }
        }
    }
}

void DPDA::check_acceptance(const std::string& last_state) {
    std::cout << "Halted at state: " + last_state + "\n";
    if (this->acceptance_mode == "empty") {
        if (this->execution_stack.empty()) this->accept_message();
        return;
    } else if (this->acceptance_mode == "final") {
        if (this->final_states.find(last_state) != this->final_states.end()) this->accept_message();
        return;
    }
    this->reject_message();
}

std::string DPDA::stack_output() {
    std::stack<char> temp = this->execution_stack;
    std::string res = "";
    while (!temp.empty()) {
        res += temp.top();
        temp.pop();
    }
    return "Stack: " + (res.empty() ? "empty" : res);
}

void DPDA::print_transition(const char& symb, const char& stack_char, const std::string& push_str) {
    std::cout << "Read " << ((symb == '\0') ? "eps" : std::string(1, symb)) << " -> " 
    << " Popped " << stack_char << " and Pushed " << ((push_str == "eps") ? "nothing" : push_str) << "\n";
}

void DPDA::reject_message() {
    std::cout << "Result: Rejected\n";
}

void DPDA::accept_message() {
    std::cout << "Result: Accepted\n";
}

void DPDA::print_state_stack_output(const std::string& state) {
    std::cout << "State: " << state << " , " << this->stack_output() << "\n";
}