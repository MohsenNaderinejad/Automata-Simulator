#include"../../include/DFA.hpp"

DFA::DFA(const std::set<std::string>& states, const std::set<char>& alphabet, 
const std::string& initial_state, const std::set<std::string>& final_states){
    
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
    if (final_states_not_in_states.size() != 0) {
        throw FinalStatesNotFound(final_states_not_in_states);
    }

    this->states = std::move(states);
    this->states.emplace("DEAD");

    this->final_states = std::move(final_states);
    this->alphabet = std::move(alphabet);
    this->initial_state = std::move(initial_state);

    transition_matrix = new int*[this->states.size()];
    int alpha_size = this->alphabet.size();
    for (int state_index = 0; state_index < this->states.size(); state_index++){
        transition_matrix[state_index] = new int[alpha_size];
        std::fill_n(transition_matrix[state_index], alpha_size, -1);
    }
    
    this->encoding();
}

DFA::~DFA() {
    int alpha_size = this->alphabet.size();
    for (int state_index = 0; state_index < this->states.size(); state_index++){
        delete [] transition_matrix[state_index];
    }
    delete [] transition_matrix;
}

void DFA::encoding_states(){
    int id = 0;
    for (const auto& state : this->states) {
        this->state_to_id[state] = id++;
        this->id_to_state.emplace_back(state);
    }
}

void DFA::encoding_symbols(){
    int index = 0;
    for (const auto& symb : this->alphabet) {
        this->symbol_to_index[symb] = index++;
    }
}

void DFA::encoding(){
    this->encoding_states();
    this->encoding_symbols();
}

void DFA::add_transition(const std::string& start_state, const char& symb, 
const std::string& end_state){
    check_transition_legitimacy(start_state, symb, end_state);
    int start_state_id = this->state_to_id[start_state];
    int end_state_id = this->state_to_id[end_state];
    int symb_index = this->symbol_to_index[symb];
    if (this->transition_matrix[start_state_id][symb_index] != -1) {
        throw MultipleTransitions({start_state, std::string(1, symb)});
    }
    transition_matrix[start_state_id][symb_index] = end_state_id;
}

void DFA::check_transition_legitimacy(const std::string& start_state, const char& symb, 
const std::string& end_state){
    bool start_state_found = this->states.find(start_state) != this->states.end();
    bool end_state_found = this->states.find(end_state) != this->states.end();
    if (!start_state_found || !end_state_found) {
        std::tuple<std::string, std::string, std::string> transition(start_state, std::string(1, symb), end_state);
        throw InvalidTransitionStartEnd(transition);
    }
    bool symb_found = this->alphabet.find(symb) != this->alphabet.end();
    if(!symb_found){
        std::tuple<std::string, std::string, std::string> transition(start_state, std::string(1, symb), end_state);
        throw InvalidTransitionSymbol(transition);
    }
}

void DFA::add_dead_state_transitions() {
    this->dead_state_id = this->state_to_id["DEAD"];
    for (int state_index = 0; state_index < this->states.size(); state_index++){
        for (int alpha_index = 0; alpha_index < this->alphabet.size(); alpha_index++){
            if (this->transition_matrix[state_index][alpha_index] == -1) {
                this->transition_matrix[state_index][alpha_index] = this->dead_state_id;
            }
        }
    }
}

bool DFA::check_input_legitimacy(const std::string& input){
    for (const auto& symb : input){
        if (this->alphabet.find(symb) == this->alphabet.end()){
            std::cout << "The input string contains " + std::string(1, symb) + " which is not in the alphabet."
            "\nTherefore input skipped.\n";
            return false;
        }
    }
    return true;
}

void DFA::operator()(const std::string& input){
    std::cout << "Input string: " + input + "\n";
    bool allowed = check_input_legitimacy(input);
    if (allowed) {
        std::cout << "Start at state: " + this->initial_state + "\n";
        int current_state_id = this->state_to_id[this->initial_state];
        for (const char& symb: input) {
            current_state_id = transition(current_state_id, symb);
            if (check_dead_state_halt(current_state_id)){
                return;
            }
        }
        this->check_final_state_halt(current_state_id);
    }
}

int DFA::transition(int &current_state_id, const char &symb)
{
    int current_symbol_index = this->symbol_to_index[symb];
    int next_state_id = this->transition_matrix[current_state_id][current_symbol_index];
    std::string next_state = this->id_to_state[next_state_id];
    std::string current_state = this->id_to_state[current_state_id];
    std::cout << transition_output(current_state, symb, next_state);
    return next_state_id;
}

bool DFA::check_dead_state_halt(int& current_state_id){
    if(current_state_id == this->dead_state_id){
        std::cout << "Entered DEAD state.\n"
        "Execution halted early.\n";
        this->reject_message();
        return 1;
    }
    return 0;
}

void DFA::check_final_state_halt(int& last_state_id){
    std::string last_state = this->id_to_state[last_state_id]; 
    std::cout << "Halted at state: " + last_state + "\n";
    if (this->final_states.find(last_state) != this->final_states.end()){
        this->accept_message();
        return;
    }
    this->reject_message();
}

std::string DFA::transition_output(const std::string& i_state, const char& symb, const std::string& j_state){
    return "Read \'" + std::string(1, symb) + "\' -> move from " + i_state + " to " + j_state + "\n";
}

void DFA::reject_message() {
    std::cout << "Result: Rejected\n";
}

void DFA::accept_message() {
    std::cout << "Result: Accepted\n";
}

// ----------------- DFS -----------------
namespace {

std::vector<bool> DFS(int** transition_matrix, int state_num, int alpha_size, int start_state_id){
    std::vector<bool> visited(state_num, false);
    std::stack<int> s;
    s.push(start_state_id);
    while(!s.empty()){
        int current_state = s.top();
        s.pop();
        if (visited[current_state]) continue;
        visited[current_state] = true;

        for (int symbol_idx = 0; symbol_idx < alpha_size; symbol_idx++) {
            int next_state = transition_matrix[current_state][symbol_idx];
            if (next_state != -1 && !visited[next_state]) {
                s.push(next_state);
            }
        }
    }
    return visited;
}

std::vector<bool> BackDFS(const std::vector<std::vector<std::vector<int>>>& reversed_matrix, int state_num, int alpha_size, const std::vector<int>& final_states_id){
    std::vector<bool> backward_visited(state_num, false);
    std::stack<int> s;
    for (int f_id : final_states_id) {
        s.push(f_id);
    }
    while (!s.empty()) {
        int current_state = s.top();
        s.pop();
        
        if (backward_visited[current_state]) continue;
        backward_visited[current_state] = true;
        
        for (int symbol_idx = 0; symbol_idx < alpha_size; symbol_idx++) {
            const auto& parents = reversed_matrix[current_state][symbol_idx];
            
            for (int parent_id : parents) {
                if (!backward_visited[parent_id]) {
                    s.push(parent_id);
                }
            }
        }
    }
    return backward_visited;
}

}

void DFA::unreachable_states(){
    int initial_state_id = this->state_to_id[this->initial_state];
    int state_num = this->states.size();
    std::vector<bool> reaching_states = DFS(this->transition_matrix, state_num, this->alphabet.size(), initial_state_id);
    for (int i = 0; i < state_num; i++) {
        if (!reaching_states[i] && this->id_to_state[i] != "DEAD") {
            std::cout << this->id_to_state[i] << " is an unreachable state." << std::endl;
        }
    }
}

void DFA::dead_states() {
    int state_num = this->states.size();
    int alpha_size = this->alphabet.size();

    std::vector<std::vector<std::vector<int>>> reversed_matrix(
    state_num, 
    std::vector<std::vector<int>>(alpha_size)
    );

    std::vector<int> final_states_id;
    for (const auto& f_state : this->final_states) {
        final_states_id.emplace_back(this->state_to_id[f_state]);
    }

    for (int current = 0; current < state_num; ++current) {
        for (int symbol_idx = 0; symbol_idx < alpha_size; symbol_idx++) {
            int next_state = transition_matrix[current][symbol_idx];
            if (next_state != -1) {
                reversed_matrix[next_state][symbol_idx].push_back(current);
            }
        }
    }

    std::vector<bool> backward_visited = BackDFS(reversed_matrix, state_num, alpha_size, final_states_id);
    for (int i = 0; i < state_num; i++) {
        if (!backward_visited[i] && this->id_to_state[i] != "DEAD") {
            std::cout << this->id_to_state[i] << " is a dead state." << std::endl;
        }
    }
}

void DFA::check_emptiness() {
    int initial_state_id = this->state_to_id[this->initial_state];
    int state_num = this->states.size();
    std::vector<bool> reaching_states = DFS(this->transition_matrix, state_num, this->alphabet.size(), initial_state_id);
    for (const auto& fstate : this->final_states) {
        int state_id = this->state_to_id[fstate];
        if (reaching_states[state_id]) {
            std::cout << "The language of the DFA is not empty." << std::endl;
            return;
        }
    }
    std::cout << "The language of the DFA is empty." << std::endl;
}