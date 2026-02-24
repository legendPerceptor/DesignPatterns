#include <iostream>
#include <string>

#include "designPatternExamples.h"

void simple_log (std::string_view log) {
    std::cout << "-----" << log << "-----" << std::endl;
}

int main () {
    simple_log ("Factory Method");
    FactoryMethodExample ();
    simple_log ("Abstract Factory");
    AbstractFactoryExample ();
    simple_log ("Builder");
    BuilderExample ();
    simple_log ("Prototype");
    PrototypeExample ();
    simple_log ("Singleton");
    SingletonExample ();
    simple_log ("Adapter");
    AdapterExample ();
    simple_log ("Bridge");
    BridgeExample ();
    simple_log ("Composite");
    CompositeExample ();
    simple_log ("Decorator");
    DecoratorExample ();
    simple_log ("Facade");
    FacadeExample ();
    simple_log ("Flyweight");
    FlyweightExample ();
    simple_log ("Proxy");
    ProxyExample ();
    simple_log ("Interpreter");
    InterpreterExample ();
    simple_log ("Template Method");
    TemplateMethodExample ();
    simple_log ("Chain of Responsibility");
    ChainOfResponsibilityExample ();
    simple_log ("Command");
    CommandExample ();
    simple_log ("Iterator");
    IteratorExample ();
    simple_log ("Mediator");
    MediatorExample ();
    simple_log ("Memento");
    MementoExample ();
    simple_log ("Observer");
    ObserverExample ();
    simple_log ("State");
    StateExample ();
    simple_log ("Strategy");
    StrategyExample ();
    simple_log ("Visitor");
    VisitorExample ();
    return 0;
}