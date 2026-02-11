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

void FactoryMethodExample (bool useSpreadSheet = false);
void AbstractFactoryExample (bool isWindows = false);
void BuilderExample ();
void PrototypeExample ();
void SingletonExample ();
void AdapterExample ();
void BridgeExample ();
void CompositeExample ();
void DecoratorExample ();

#endif