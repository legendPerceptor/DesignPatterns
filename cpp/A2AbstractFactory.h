/*
The intent is to provide an interface for creating families of related or
dependent objects without specifying their concrete classes.

Example:
Product Interfaces: `Button`, `Checkbox`, etc.
Concrete products: `WinButton`, `MacButton`, `WinCheckbox`, `MacCheckbox`, etc.
`AbstractFactory` with multiple creation methods: `createButton()`, `createCheckbox()`.
Concrete factories: `WinFactory`, `MacFactory`, each creates a complete family of matching products.
*/

#ifndef A2_ABSTRACT_FACTORY_H
#define A2_ABSTRACT_FACTORY_H

#include <iostream>
#include <memory>

namespace AbstractFactory {
// ----- Product interfaces -----
class Button {
    public:
    virtual ~Button ()     = default;
    virtual void render () = 0;
};

class Checkbox {
    public:
    virtual ~Checkbox ()   = default;
    virtual void render () = 0;
};

// ----- Concrete products: Windows style -----
class WinButton : public Button {
    public:
    void render () override {
        std::cout << "Rendering Windows-style button.\n";
    }
};

class WinCheckbox : public Checkbox {
    public:
    void render () override {
        std::cout << "Rendering Windows-style checkbox.\n";
    }
};

// ----- Concrete products: Mac style -----
class MacButton : public Button {
    public:
    void render () override {
        std::cout << "Rendering Mac-style button.\n";
    }
};

class MacCheckbox : public Checkbox {
    public:
    void render () override {
        std::cout << "Rendering Mac-style checkbox.\n";
    }
};

// ----- Abstract Factory -----
class GUIFactory {
    public:
    virtual ~GUIFactory ()                              = default;
    virtual std::unique_ptr<Button> createButton ()     = 0;
    virtual std::unique_ptr<Checkbox> createCheckbox () = 0;
};

// ----- Concrete Factories -----
class WinFactory : public GUIFactory {
    public:
    std::unique_ptr<Button> createButton () override {
        return std::make_unique<WinButton> ();
    }

    std::unique_ptr<Checkbox> createCheckbox () override {
        return std::make_unique<WinCheckbox> ();
    }
};

class MacFactory : public GUIFactory {
    public:
    std::unique_ptr<Button> createButton () override {
        return std::make_unique<MacButton> ();
    }

    std::unique_ptr<Checkbox> createCheckbox () override {
        return std::make_unique<MacCheckbox> ();
    }
};

// ----- Client code -----
class Application {
    public:
    Application (std::unique_ptr<GUIFactory> factory)
    : factory_ (std::move (factory)) {
    }

    void renderUI () {
        auto button   = factory_->createButton ();
        auto checkbox = factory_->createCheckbox ();

        button->render ();
        checkbox->render ();
    }

    private:
    std::unique_ptr<GUIFactory> factory_;
};

}; // namespace AbstractFactory

#endif