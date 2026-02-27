# Design Patterns in Rust

Comprehensive Rust implementation of all 23 Gang of Four design patterns using idiomatic Rust features.

## Building

```bash
# Build the project
cargo build --release

# Run the demo
cargo run --bin demo

# Run tests
cargo test
```

## Pattern Files

| Category | Patterns | File Naming |
|----------|----------|-------------|
| **Creational** (A1-A5) | Factory Method, Abstract Factory, Builder, Prototype, Singleton | `a1_factory_method.rs` |
| **Structural** (B1-B7) | Adapter, Bridge, Composite, Decorator, Facade, Flyweight, Proxy | `b1_adapter.rs` |
| **Behavioral** (C1-C11) | Interpreter, Template Method, Chain of Responsibility, Command, Iterator, Mediator, Memento, Observer, State, Strategy, Visitor | `c1_interpreter.rs` |

## Running Examples

Each pattern has an `example()` function that demonstrates its usage:

```rust
// Run all patterns
cargo run

// Or test specific patterns
cargo test a1_factory_method::tests
```

## Rust-Specific Features Used

- **Traits**: For interfaces and polymorphism
- **Enums**: For state machines and pattern matching
- **Generics**: For flexible pattern implementations
- **`Box<dyn Trait>`**: For dynamic dispatch
- **`Rc<RefCell<T>>`**: For shared mutable state
- **`Option<T>`** and **`Result<T, E>`**: For error handling
- **`OnceLock`**: For singleton pattern (Rust 1.70+)
- **Iterators**: For custom iteration patterns

## Test Coverage

Run all tests with:

```bash
cargo test
```

Individual pattern tests:

```bash
# Test specific pattern
cargo test test_factory_method

# Test specific file
cargo test --test a1_factory_method
```
