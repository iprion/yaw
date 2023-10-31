#ifndef CALLBACL_CALLER_H
#define CALLBACL_CALLER_H

#include <map>
#include <functional>
#include <list>

class AnySignal {
public:
    AnySignal() {} ;
    virtual ~AnySignal() {};
};

template<typename RetT, typename ... ArgsT>
class Signal : public AnySignal {
public:
    
    Signal(std::function<RetT(ArgsT...)> s)  {
        signal = s;
    }

    RetT operator()(ArgsT...args) {
        return signal(args...);
    }

private:
    std::function<RetT(ArgsT...)> signal;
};

template<typename RetT>
class NoArgsSignal : public AnySignal {
public:
    
    NoArgsSignal(std::function<RetT()> s)  {
        signal = s;
    }

    RetT operator()() {
        return signal();
    }

private:
    std::function<RetT()> signal;
};

template<typename ... ArgsT>
class NoRetSignal : public AnySignal {
public:
    
    NoRetSignal(std::function<void(ArgsT...)> s)  {
        signal = s;
    }

    void operator()(ArgsT...args) {
        signal(args...);
    }

private:
    std::function<void(ArgsT...)> signal;
};

class SimpleSignal : public AnySignal {
public:
    
    SimpleSignal(std::function<void()> s)  {
        signal = s;
    }

    void operator()() {
        signal();
    }


private:
    std::function<void()> signal;
};

class Signaler { // Notifier ?

public:

    Signaler(std::list<std::string> signalsName);

    template<typename RetT, typename ... ArgsT>
    bool connect(const std::string& signalName, std::function<RetT(ArgsT...)> callback, void * called);

    template<typename RetT>
    bool connect(const std::string& signalName, std::function<RetT()> callback, void * called);

    template<typename ... ArgsT>
    bool connect(const std::string& signalName, std::function<void(ArgsT...)> callback, void * called);

    bool connect(const std::string& signalName, std::function<void()> callback, void * called);

    bool disconnect(const std::string& signalName, void * called);
      
    template<typename RetT, typename ... ArgsT>
    std::map<void*, RetT> emit_get(const std::string& signalName, ArgsT...args);

    template<typename RetT>
    std::map<void*, RetT> emit_get(const std::string& signalName);

    void emit(const std::string& signalName);

    template<typename ... ArgsT>
    void emit(const std::string& signalName, ArgsT...args){
        if (exists(signalName))
            for(auto cit = signals[signalName].begin() ; cit!=signals[signalName].end() ; ++cit)
                dynamic_cast<NoRetSignal<ArgsT...>*>(cit->second)->operator()(args...);
    }

    std::function<void()> signal(const std::string& signalName);

protected:
    void addSignal(const std::string& signalName);

private :

    bool exists(const std::string& signalName, void * called);
    bool exists(const std::string& signalName);

private:

    std::map<std::string, std::map<void*, AnySignal*> > signals;
};

#endif