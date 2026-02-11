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
    return 0;
}