#ifndef DESIGN_PATTERN_EXAMPLES_H
#define DESIGN_PATTERN_EXAMPLES_H

#include "A1FactoryMethod.h"
#include "A2AbstractFactory.h"
#include "A3Builder.h"
#include "A4Prototype.h"
#include "A5Singleton.h"

void FactoryMethodExample (bool useSpreadSheet = false);
void AbstractFactoryExample (bool isWindows = false);
void BuilderExample ();
void PrototypeExample ();
void SingletonExample ();

#endif