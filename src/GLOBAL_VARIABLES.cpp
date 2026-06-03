#include"../include/GLOBAL_VARIABLES.hpp"

const char* AutomataError::what() const noexcept {
    return message.c_str();
}

InitialStateNotFound::InitialStateNotFound(const std::string& initialState) {
    message = "\n-------------------------------------------------------------------------\n" 
    "InitialStateNotFound Error: \n"
    "\tThe given initial state: " + initialState + "\n"
    + "\tNot found in the States set"
    "\n-------------------------------------------------------------------------\n";
}

FinalStatesNotFound::FinalStatesNotFound(const std::vector<std::string>& finalStatesNF) {
    message = "\n-------------------------------------------------------------------------\n"
    "FinalStatesNotFound Error: \n"
    "\tThe following states: ";
    for (const auto& state : finalStatesNF) {
        message += state + " ";
    }
    message += "\n\tNot found in the Final States set"
    "\n-------------------------------------------------------------------------\n";
}

InvalidTransitionStartEnd::InvalidTransitionStartEnd(const std::tuple<std::string, std::string, std::string>& badTrans) {
    message = "\n-------------------------------------------------------------------------\n"
    "InvalidTransistionStartEnd Error: \n"
    "\tThe following transition: (" + std::get<0>(badTrans) + ", " + std::get<1>(badTrans) + ") -> " + std::get<2>(badTrans) + "\n"
    "\tHas an invalid start or end state"
    "\n-------------------------------------------------------------------------\n";
}

InvalidTransitionStartEndDPDA::InvalidTransitionStartEndDPDA(const std::tuple<std::string, std::string, std::string, std::string, std::string>& badTrans) {
    message = "\n-------------------------------------------------------------------------\n"
    "InvalidTransitionStartEndDPDA Error: \n"
    "\tThe following transition: (" + std::get<0>(badTrans) + ", " + std::get<1>(badTrans) + ", " + std::get<2>(badTrans) + ") -> (" + std::get<3>(badTrans) + ", " + std::get<4>(badTrans) + ")\n"
    "\tHas an invalid start or end state"
    "\n-------------------------------------------------------------------------\n";
}

InvalidTransitionSymbol::InvalidTransitionSymbol(const std::tuple<std::string, std::string, std::string>& badTrans) {
    message = "\n-------------------------------------------------------------------------\n"
    "InvalidTransistionSymbol Error: \n"
    "\tThe following transition symbol: (" + std::get<0>(badTrans) + ", " + std::get<1>(badTrans) + ") -> " + std::get<2>(badTrans) + "\n"
    "\tIs not found in the Alphabet set"
    "\n-------------------------------------------------------------------------\n";
}

InvalidTransitionSymbolDPDA::InvalidTransitionSymbolDPDA(const std::tuple<std::string, std::string, std::string, std::string, std::string>& badTrans) {
    message = "\n-------------------------------------------------------------------------\n"
    "InvalidTransitionSymbolDPDA Error: \n"
    "\tThe following transition: (" + std::get<0>(badTrans) + ", " + std::get<1>(badTrans) + ", " + std::get<2>(badTrans) + ") -> (" + std::get<3>(badTrans) + ", " + std::get<4>(badTrans) + ")\n"
    "\tContains an invalid input symbol"
    "\n-------------------------------------------------------------------------\n";
}

InvalidTransitionStackTop::InvalidTransitionStackTop(const std::tuple<std::string, std::string, std::string, std::string, std::string>& badTrans) {
    message = "\n-------------------------------------------------------------------------\n"
    "InvalidTransitionStackTop Error: \n"
    "\tThe following transition: (" + std::get<0>(badTrans) + ", " + std::get<1>(badTrans) + ", " + std::get<2>(badTrans) + ") -> (" + std::get<3>(badTrans) + ", " + std::get<4>(badTrans) + ")\n"
    "\tContains an invalid stack top"
    "\n-------------------------------------------------------------------------\n";
}

InvalidTransitionPushString::InvalidTransitionPushString(const std::tuple<std::string, std::string, std::string, std::string, std::string>& badTrans) {
    message = "\n-------------------------------------------------------------------------\n"
    "InvalidTransitionSymbolDPDA Error: \n"
    "\tThe following transition: (" + std::get<0>(badTrans) + ", " + std::get<1>(badTrans) + ", " + std::get<2>(badTrans) + ") -> (" + std::get<3>(badTrans) + ", " + std::get<4>(badTrans) + ")\n"
    "\tContains an invalid push string"
    "\n-------------------------------------------------------------------------\n";
}

MultipleTransitions::MultipleTransitions(const std::pair<std::string, std::string>& badTrans) {
    message = "\n-------------------------------------------------------------------------\n" 
    "MultipleTransitions Error: \n"
    "\tThe following transition: (" + badTrans.first + ", " + badTrans.second + ") has multiple transitions defined for it"
    "\n-------------------------------------------------------------------------\n";
}

PreviouslyDefinedTransition::PreviouslyDefinedTransition(const std::tuple<std::string, std::string, std::string, std::string, std::string>& badTrans) {
    message = "\n-------------------------------------------------------------------------\n"
    "PreviouslyDefinedTransition Error: \n"
    "\tThe following transition: (" + std::get<0>(badTrans) + ", " + std::get<1>(badTrans) + ", " + std::get<2>(badTrans) + ") -> (" + std::get<3>(badTrans) + ", " + std::get<4>(badTrans) + ")\n"
    "\tConflicts with a transition that has already been explicitly defined"
    "\n-------------------------------------------------------------------------\n";
}

LambdaTransitionExists::LambdaTransitionExists(const std::tuple<std::string, std::string, std::string, std::string, std::string>& badTrans) {
    message = "\n-------------------------------------------------------------------------\n"
    "LambdaTransitionExists Error: \n"
    "\tDeterminism Violation for transition: (" + std::get<0>(badTrans) + ", " + std::get<1>(badTrans) + ", " + std::get<2>(badTrans) + ") -> (" + std::get<3>(badTrans) + ", " + std::get<4>(badTrans) + ")\n"
    "\tA conflicting lambda transition already exists for this state and stack top"
    "\n-------------------------------------------------------------------------\n";
}

InvaildStartStackSymbol::InvaildStartStackSymbol(const std::string& startStackSymbol) {
    message = "\n-------------------------------------------------------------------------\n"
    "InvaildStartStackSymbol Error: \n"
    "\tThe given initial stack symbol: " + startStackSymbol + "\n"
    "\tNot found in the Stack Alphabet set"
    "\n-------------------------------------------------------------------------\n";
}

InvalidAcceptanceMode::InvalidAcceptanceMode(const std::string& mode) {
    message = "\n-------------------------------------------------------------------------\n"
    "InvalidAcceptanceMode Error: \n"
    "\tThe given mode: " + mode + "\n"
    "\tMust be exactly 'final' or 'empty'\n"
    "-------------------------------------------------------------------------\n";
}