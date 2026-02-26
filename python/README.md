# Design Patterns in Python

This directory contains a comprehensive Python implementation of all 23 Gang of Four design patterns, paralleling the C++ implementation found in the parent directory. Each pattern includes practical examples with real-world use cases.

## Overview

The Python implementation is organized into three categories:

- **Creational Patterns (A1-A5)**: Factory Method, Abstract Factory, Builder, Prototype, Singleton
- **Structural Patterns (B1-B7)**: Adapter, Bridge, Composite, Decorator, Facade, Flyweight, Proxy
- **Behavioral Patterns (C1-C11)**: Interpreter, Template Method, Chain of Responsibility, Command, Iterator, Mediator, Memento, Observer, State, Strategy, Visitor

## Project Structure

```
python/
├── src/
│   ├── a1_factory_method.py       # Creational patterns
│   ├── a2_abstract_factory.py
│   ├── a3_builder.py
│   ├── a4_prototype.py
│   ├── a5_singleton.py
│   ├── b1_adapter.py              # Structural patterns
│   ├── b2_bridge.py
│   ├── b3_composite.py
│   ├── b4_decorator.py
│   ├── b5_facade.py
│   ├── b6_flyweight.py
│   ├── b7_proxy.py
│   ├── c1_interpreter.py          # Behavioral patterns
│   ├── c2_template_method.py
│   ├── c3_chain_of_responsibility.py
│   ├── c4_command.py
│   ├── c5_iterator.py
│   ├── c6_mediator.py
│   ├── c7_memento.py
│   ├── c8_observer.py
│   ├── c9_state.py
│   ├── c10_strategy.py
│   └── c11_visitor.py
└── tests/
    ├── test_creational_patterns.py
    ├── test_structural_patterns.py
    └── test_behavioral_patterns.py
```

## Running Tests with uv

This project uses [uv](https://github.com/astral-sh/uv) as the Python package manager and test runner.

### Prerequisites

Install `uv` if you haven't already:

```bash
curl -LsSf https://astral.sh/uv/install.sh | sh
```

### Running All Tests

From the project root directory:

```bash
# Run all tests
uv run pytest tests/ -v

# Run with coverage
uv run pytest tests/ --cov=src --cov-report=html
```

### Running Specific Test Suites

```bash
# Run only creational pattern tests
uv run pytest tests/test_creational_patterns.py -v

# Run only structural pattern tests
uv run pytest tests/test_structural_patterns.py -v

# Run only behavioral pattern tests
uv run pytest tests/test_behavioral_patterns.py -v
```

### Running Individual Tests

```bash
# Run tests for a specific pattern class
uv run pytest tests/test_structural_patterns.py::TestAdapter -v

# Run a single test
uv run pytest tests/test_structural_patterns.py::TestAdapter::test_mp3_plays_directly -v
```

## Running Pattern Examples

Each pattern file can be executed directly to see a demonstration:

```bash
# Run a specific pattern example
uv run python src/b1_adapter.py
uv run python src/c1_interpreter.py
```

## Python Implementation Highlights

The Python implementation leverages modern Python features including:

- **Abstract Base Classes (ABC)**: For defining interfaces and abstract methods
- **dataclasses**: For clean, concise data models
- **Type Hints & Generics**: For better code documentation and type safety
- **Enum**: For type-safe enumerations
- **Context Managers**: For resource management
- **Properties**: For controlled attribute access

### Key Differences from C++ Implementation

1. **Duck Typing**: Python's dynamic typing allows for more flexible implementations
2. **First-Class Functions**: Enables simpler command and strategy patterns
3. **Decorators**: Python's `@decorator` syntax provides native support for the Decorator pattern
4. **Context Managers**: The `with` statement provides native support for patterns requiring cleanup

## Pattern Summaries

### Creational Patterns

| Pattern | File | Description |
|---------|------|-------------|
| Factory Method | `a1_factory_method.py` | Defines interface for creating objects, subclasses decide which class to instantiate |
| Abstract Factory | `a2_abstract_factory.py` | Creates families of related objects without specifying concrete classes |
| Builder | `a3_builder.py` | Constructs complex objects step by step |
| Prototype | `a4_prototype.py` | Creates new objects by cloning existing ones |
| Singleton | `a5_singleton.py` | Ensures a class has only one instance |

### Structural Patterns

| Pattern | File | Description |
|---------|------|-------------|
| Adapter | `b1_adapter.py` | Converts interface of a class into another interface clients expect |
| Bridge | `b2_bridge.py` | Decouples abstraction from implementation |
| Composite | `b3_composite.py` | Composes objects into tree structures |
| Decorator | `b4_decorator.py` | Adds responsibilities dynamically |
| Facade | `b5_facade.py` | Provides unified interface to a subsystem |
| Flyweight | `b6_flyweight.py` | Shares fine-grained objects efficiently |
| Proxy | `b7_proxy.py` | Controls access to an object |

### Behavioral Patterns

| Pattern | File | Description |
|---------|------|-------------|
| Interpreter | `c1_interpreter.py` | Implements language grammar for expressions |
| Template Method | `c2_template_method.py` | Defines algorithm skeleton, subclasses override steps |
| Chain of Responsibility | `c3_chain_of_responsibility.py` | Passes requests along a chain of handlers |
| Command | `c4_command.py` | Encapsulates requests as objects |
| Iterator | `c5_iterator.py` | Accesses aggregate elements sequentially |
| Mediator | `c6_mediator.py` | Defines centralized communication between objects |
| Memento | `c7_memento.py` | Captures/restores object state without breaking encapsulation |
| Observer | `c8_observer.py` | Defines one-to-many dependency for notifications |
| State | `c9_state.py` | Alters behavior when object's state changes |
| Strategy | `c10_strategy.py` | Defines interchangeable algorithms |
| Visitor | `c11_visitor.py` | Separates operations from object structure |

## Test Coverage

The project includes comprehensive unit tests with **118 tests** covering all 23 design patterns:

- 15 tests for Creational patterns
- 29 tests for Structural patterns
- 74 tests for Behavioral patterns

Tests verify:
- Basic functionality
- Edge cases and error handling
- State management and transitions
- Undo/redo operations
- Pattern-specific behaviors

## Learning Resources

Each pattern file includes:
- **Intent**: Brief description of the pattern's purpose
- **Key Participants**: Main classes and their roles
- **Key Benefits**: Advantages of using the pattern
- **Real-world Example**: Practical demonstration
- **Test Helper Functions**: Simplified setup for unit testing

## Contributing

When adding new patterns or modifying existing ones:

1. Ensure all tests pass: `uv run pytest tests/ -v`
2. Follow the existing code style (run `pre-commit run --all-files`)
3. Include comprehensive docstrings
4. Add test helper functions for easier testing
5. Demonstrate usage in the `if __name__ == "__main__"` block

## License

This project follows the same license as the parent DesignPatterns repository.
