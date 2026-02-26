# Design Patterns

This repo aims to explain all the 23 design patterns in a systematic way, and provide concrete examples to use each of them. The goal is to help you build a system of these design patterns. Whenever you were given questions related to a design pattern, you could be confident to answer and explain it.

## A clever way to remember all 23 design patterns

The 23 design patterns consist of 5 creational patterns, 7 structural patterns, and 11 behavioral patterns. The 11 behaviroal patterns can be further divided into 5+4+2 patterns and the reason will be explained later.

For builder type patterns, we have two factories: (1) Factory Method, (2) Abstract Factory. Then because of builder type, we have (3) Builder. You will build a prototype before making it perfect, so we think of (4) Prototype. Finally, there is a pattern where only one instance could exist, that is (5) Singleton.

For structural patterns, we have ABCD, double Fs plus a proxy. ABCD are (6) Adaptor, (7) Bridge, (8) Composite, (9) Decorator. Double Fs are (10) Facade and (11) Flyweight. Plus, we have (12) Proxy.

For behaviroal patterns, we have the first group of five starting from (13) Interpretor and ending at (17) Iterator. The middle ones are (14) Template Methods, (15) Chain of Responsibilities, (16) Command.

The second group is in the middle so we think of (18) Mediator. Remember we have 4 items in the second group, we need to note it down, so we think of (19) Memo. Someone is looking at us to check our note states, and thus we think of (20) Observer (21) State.

Finally, there are (22) Strategy and (23) Vistor. They are often used together so they are in their own group.

In this way, you can easily remember all the 23 design patterns and recite it in whatever order. Next, we will start trying to understand what they actully do.

## Implementations

The best way to understand each design pattern is to implement them to do some tasks. This project provides implementations in multiple languages with practical examples and comprehensive unit tests.

### [C++ Implementation](./cpp/README.md)

Modern C++ implementation with smart pointers, RAII, and templates. Most patterns are header-only.

- **Build**: `cmake --preset debug && cmake --build --preset debug`
- **Run Tests**: `./build/debug/cpp/tests/AllTests`
- **File Naming**: `A1FactoryMethod.h`, `B1Adapter.h`, `C1Interpreter.h` (PascalCase)

### [Python Implementation](./python/README.md)

Clean Python implementation using ABC, dataclasses, and type hints.

- **Run Tests**: `uv run pytest tests/ -v`
- **File Naming**: `a1_factory_method.py`, `b1_adapter.py`, `c1_interpreter.py` (snake_case)

## Pattern Overview

| Category | Patterns |
|----------|----------|
| **Creational** (A1-A5) | Factory Method, Abstract Factory, Builder, Prototype, Singleton |
| **Structural** (B1-B7) | Adapter, Bridge, Composite, Decorator, Facade, Flyweight, Proxy |
| **Behavioral** (C1-C11) | Interpreter, Template Method, Chain of Responsibility, Command, Iterator, Mediator, Memento, Observer, State, Strategy, Visitor |

## Pre-commit

We use pre-commit to ensure the code is formatted properly. Please use `uv tool install pre-commit` to install the tool and it will read the `.pre-commit-config.yaml` file to check your files before you commit.

The following command will install the pre-commit hooks in your `.git` folder.

```bash
pre-commit install
```

We can also manually run pre-commit using the following command.

> Note that you normally do not need to trigger it manually as `git commit` will trigger it automatically.

```bash
pre-commit run --all-files 2>&1
```
