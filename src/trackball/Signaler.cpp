#include "Signaler.h"
#include <string>

Signaler::Signaler(std::list<std::string> signalName) {
    for( auto it = signalName.begin() ; it != signalName.end() ; ++it)
        signals[*it] = std::map<void*, AnySignal*>();
}

template<typename RetT, typename ... ArgsT>
bool Signaler::connect(const std::string& signalName, std::function<RetT(ArgsT...)> callback, void * called) {
    if (exists(signalName)) {
        signals[signalName][called] = new Signal<RetT, ArgsT...>(callback);
        return true;
    }
    else 
        return false;
}

template<typename RetT>
bool Signaler::connect(const std::string& signalName, std::function<RetT()> callback, void * called) {
    if (exists(signalName)) {
        signals[signalName][called] = new NoArgsSignal<RetT>(callback);
        return true;
    }
    else 
        return false;
}

template<typename ... ArgsT>
bool Signaler::connect(const std::string& signalName, std::function<void(ArgsT...)> callback, void * called) {
    if (exists(signalName)) {
        signals[signalName][called] = new NoRetSignal<ArgsT...>(callback);
        return true;
    }
    else 
        return false;
}

bool Signaler::connect(const std::string& signalName, std::function<void()> callback, void * called) {
    if (exists(signalName)) {
        signals[signalName][called] = new SimpleSignal(callback);
        return true;
    }
    else 
        return false;
}

bool Signaler::disconnect(const std::string& signalName, void * called) {
    
    if (exists(signalName, called)) {
        signals[signalName].erase(called);
        return true;
    }
    else    
        return false;
}

void Signaler::emit(const std::string& signalName) {   

    if (exists(signalName))
        for(auto cit = signals[signalName].begin() ; cit!=signals[signalName].end() ; ++cit)
            dynamic_cast<SimpleSignal*>(cit->second)->operator()();
}

template<typename RetT, typename ... ArgsT>
std::map<void*, RetT> Signaler::emit_get(const std::string& signalName, ArgsT...args) {
    
    std::map<void*, RetT> returnedValues = std::map<void*, RetT>();
    
    if (exists(signalName))
        for(auto cit = signals[signalName].begin() ; cit!=signals[signalName].end() ; ++cit)
            returnedValues[cit->first] = dynamic_cast<Signal<RetT, ArgsT...>*>(cit->second)->operator()(args...);
    
    return returnedValues;
}

template<typename RetT>
std::map<void*, RetT> Signaler::emit_get(const std::string& signalName) {
    
    std::map<void*, RetT> returnedValues = std::map<void*, RetT>();
    
    if (exists(signalName))
        for(auto cit = signals[signalName].begin() ; cit!=signals[signalName].end() ; ++cit)
            returnedValues[cit->first] = dynamic_cast<NoArgsSignal<RetT>*>(cit->second)->operator()();
    
    return returnedValues;
}



std::function<void()> Signaler::signal(const std::string& signalName)
{
    if (exists(signalName)) {
        auto lambda = [this, &signalName]() {
            for(auto cit = signals[signalName].begin() ; cit!=signals[signalName].end() ; ++cit)
                dynamic_cast<SimpleSignal*>(cit->second)->operator()();
        };
        return lambda;
    }
    else
        return []() {};
}

bool Signaler::exists(const std::string& signalName, void * called) {
    
    auto it = signals.find(signalName);
    
    if (it == signals.end())
        return false;

    if (it->second.find(called) == it->second.end())
        return false;

    return true;
}

bool Signaler::exists(const std::string& signalName) {
    
    auto it = signals.find(signalName);
    
    if (it == signals.end())
        return false;
    else
        return true;
}

void Signaler::addSignal(const std::string& signalName) {
    if (!exists(signalName))
        signals[signalName] = std::map<void*, AnySignal*>();
}