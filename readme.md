# Design patterns

This repo aims to explain all the 23 design patterns in a systematic way, and provide concrete examples to use each of them. The goal is to help you build a system of these design patterns. Whenever you were given questions related to a design pattern, you could be confident to answer and explain it.

## A clever way to remember all 23 design patterns

The 23 design patterns consist of 5 creational patterns, 7 structural patterns, and 11 behavioral patterns. The 11 behaviroal patterns can be further divided into 5+4+2 patterns and the reason will be explained later.

For builder type patterns, we have two factories: (1) Factory Method, (2) Abstract Factory. Then because of builder type, we have (3) Builder. You will build a prototype before making it perfect, so we think of (4) Prototype. Finally, there is a pattern where only one instance could exist, that is (5) Singleton.

For structural patterns, we have ABCD, double Fs plus a proxy. ABCD are (6) Adaptor, (7) Bridge, (8) Composite, (9) Decorator. Double Fs are (10) Facade and (11) Flyweight. Plus, we have (12) Proxy.

For behaviroal patterns, we have the first group of five starting from (13) Interpretor and ending at (17) Iterator. The middle ones are (14) Template Methods, (15) Chain of Responsibilities, (16) Command.

The second group is in the middle so we think of (18) Mediator. Remember we have 4 items in the second group, we need to note it down, so we think of (19) Memo. Someone is looking at us to check our note states, and thus we think of (20) Observer (21) State.

Finally, there are (22) Strategy and (23) Vistor. They are often used together so they are in their own group.

In this way, you can easily remember all the 23 design patterns and recite it in whatever order. Next, we will start trying to understand what they actully do.

## Self-explained examples

The best way to understand each design pattern is to implement them to do some tasks. I hope to build similar design pattern examples in multiple languages (including C++, Go, Python, Rust, etc.) Currently, I focus mainly on C++ implementations. The code lives in ./cpp folder.

### Building with CMake Presets (Recommended for VS Code)

The project uses CMake Presets for configuration. You can build using the presets:

```bash
# Configure and build in Debug mode
cmake --preset debug
cmake --build --preset debug

# Configure and build in Release mode
cmake --preset release
cmake --build --preset release

# Run the demo
./build/debug/cpp/src/DesignPatternDemo

# Run the unit tests
./build/debug/cpp/tests/AllTests
```

### Building with CMake (Command Line)

Alternatively, you can use traditional CMake commands:

```bash
# From the project root
mkdir -p build
cd build
cmake ..
cmake --build .
```

### Building with VS Code

If you're using VS Code with the CMake Tools extension:

1. Open the project in VS Code
2. The CMake Tools extension will detect the CMakePresets.json
3. Select a preset (Debug or Release) from the status bar
4. Click "Build" in the status bar or use `Ctrl+Shift+B` / `Cmd+Shift+B`

### Running Tests

The project includes unit tests using Google Test:

```bash
# Run all tests
cd build/debug/cpp/tests/
ctest --output-on-failure

# Or run specific test executables
./AllTests              # Run all tests
./CreationalTests      # Run only creational pattern tests
./StructuralTests      # Run only structural pattern tests
./BehavioralTests      # Run only behavioral pattern tests
```

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
