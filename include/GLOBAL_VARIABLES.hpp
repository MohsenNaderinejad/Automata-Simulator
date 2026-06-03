#pragma once
#include<iostream>
#include<set>
#include<unordered_map>
#include<string>
#include<exception>
#include<tuple>
#include<algorithm>
#include<sstream>
#include<vector>
#include<stack>

class AutomataError : public std::exception {
protected:
    std::string message;
public:
    const char* what() const noexcept override;
};

class InitialStateNotFound : public AutomataError {
public:
    explicit InitialStateNotFound(const std::string& initialState);
};

class FinalStatesNotFound : public AutomataError {
public:
    explicit FinalStatesNotFound(const std::vector<std::string>& finalStatesNF);
};

class InvalidTransitionStartEnd : public AutomataError {
public:
    explicit InvalidTransitionStartEnd(const std::tuple<std::string, std::string, std::string>& badTrans);
};

class InvalidTransitionStartEndDPDA : public AutomataError {
public:
    explicit InvalidTransitionStartEndDPDA(const std::tuple<std::string, std::string, std::string, std::string, std::string>& badTrans);
};

class InvalidTransitionSymbol : public AutomataError {
public:
    explicit InvalidTransitionSymbol(const std::tuple<std::string, std::string, std::string>& badTrans);
};

class InvalidTransitionSymbolDPDA : public AutomataError {
public:
    explicit InvalidTransitionSymbolDPDA(const std::tuple<std::string, std::string, std::string, std::string, std::string>& badTrans);
};

class InvalidTransitionStackTop : public AutomataError {
public:
    explicit InvalidTransitionStackTop(const std::tuple<std::string, std::string, std::string, std::string, std::string>& badTrans);
};

class InvalidTransitionPushString : public AutomataError {
public:
    explicit InvalidTransitionPushString(const std::tuple<std::string, std::string, std::string, std::string, std::string>& badTrans);
};

class MultipleTransitions : public AutomataError {
public:
    explicit MultipleTransitions(const std::pair<std::string, std::string>& badTrans);
};

class PreviouslyDefinedTransition : public AutomataError {
public:
    explicit PreviouslyDefinedTransition(const std::tuple<std::string, std::string, std::string, std::string, std::string>& badTrans);
};

class LambdaTransitionExists : public AutomataError {
public:
    explicit LambdaTransitionExists(const std::tuple<std::string, std::string, std::string, std::string, std::string>& badTrans);
};

class InvaildStartStackSymbol : public AutomataError {
public:
    explicit InvaildStartStackSymbol(const std::string& initialState);
};

class InvalidAcceptanceMode : public AutomataError {
public:
    explicit InvalidAcceptanceMode(const std::string& mode);
};