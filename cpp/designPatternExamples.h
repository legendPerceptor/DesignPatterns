#ifndef DESIGN_PATTERN_EXAMPLES_H
#define DESIGN_PATTERN_EXAMPLES_H

#include "A1FactoryMethod.h"
#include "A2AbstractFactory.h"
#include "A3Builder.h"
#include "A4Prototype.h"
#include "A5Singleton.h"
#include "B1Adapter.h"
#include "B2Bridge.h"
#include "B3Composite.h"
#include "B4Decorator.h"
#include "B5Facade.h"
#include "B6Flyweight.h"
#include "B7Proxy.h"
#include "C10Strategy.h"
#include "C11Visitor.h"
#include "C1Interpreter.h"
#include "C2TemplateMethod.h"
#include "C3ChainOfResponsibility.h"
#include "C4Command.h"
#include "C5Iterator.h"
#include "C6Mediator.h"
#include "C7Memento.h"
#include "C8Observer.h"
#include "C9State.h"

void FactoryMethodExample (bool useSpreadSheet = false);
void AbstractFactoryExample (bool isWindows = false);
void BuilderExample ();
void PrototypeExample ();
void SingletonExample ();
void AdapterExample ();
void BridgeExample ();
void CompositeExample ();
void DecoratorExample ();
void FacadeExample ();
void FlyweightExample ();
void ProxyExample ();
void InterpreterExample ();
void TemplateMethodExample ();
void ChainOfResponsibilityExample ();
void CommandExample ();
void IteratorExample ();
void MediatorExample ();
void MementoExample ();
void ObserverExample ();
void StateExample ();
void StrategyExample ();
void VisitorExample ();

#endif