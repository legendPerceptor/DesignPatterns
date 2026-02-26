# Design Patterns in C++

Comprehensive C++ implementation of all 23 Gang of Four design patterns using modern C++ features (smart pointers, RAII, templates, etc.).

## Building

```bash
# Using CMake Presets (Recommended)
cmake --preset debug
cmake --build --preset debug

# Run the demo
./build/debug/cpp/src/DesignPatternDemo

# Run tests
./build/debug/cpp/tests/AllTests
```

## Pattern Files

| Category | Patterns | File Naming |
|----------|----------|-------------|
| Creational (A1-A5) | Factory Method, Abstract Factory, Builder, Prototype, Singleton | `A1FactoryMethod.h` |
| Structural (B1-B7) | Adapter, Bridge, Composite, Decorator, Facade, Flyweight, Proxy | `B1Adapter.h` |
| Behavioral (C1-C11) | Interpreter, Template Method, Chain of Responsibility, Command, Iterator, Mediator, Memento, Observer, State, Strategy, Visitor | `C1Interpreter.h` |

Most implementations are **header-only**. See `include/` for headers and `tests/` for usage examples.

## Running Tests

```bash
cd build/debug/cpp/tests/
ctest --output-on-failure

# Or run individual test suites
./CreationalTests
./StructuralTests
./BehavioralTests
```
