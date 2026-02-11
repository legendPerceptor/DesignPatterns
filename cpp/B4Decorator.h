/*
The intent is to attach additional responsibilities to an object dynamically.
Decorators provide a flexible alternative to subclassing for extending functionality.

Key benefits:
- Add responsibilities to individual objects dynamically without affecting other objects
- Add responsibilities that can be withdrawn later
- Avoid explosion of subclasses for every combination of features

Real-world example: A coffee shop where you can add various toppings (milk, mocha, whip, etc.)
to any type of coffee beverage. Each topping adds its own cost and description.
*/
#ifndef B4_DECORATOR_H
#define B4_DECORATOR_H

#include <iostream>
#include <memory>
#include <string>

namespace Decorator {

// ----- Component interface -----
// Defines the interface for objects that can have responsibilities added dynamically
class Beverage {
    public:
    virtual ~Beverage ()                        = default;
    virtual std::string getDescription () const = 0;
    virtual double cost () const                = 0;
};

// ----- Concrete Component -----
// Basic implementation that decorators can extend
class Espresso : public Beverage {
    public:
    std::string getDescription () const override {
        return "Espresso";
    }

    double cost () const override {
        return 1.99;
    }
};

class HouseBlend : public Beverage {
    public:
    std::string getDescription () const override {
        return "House Blend Coffee";
    }

    double cost () const override {
        return 0.89;
    }
};

class DarkRoast : public Beverage {
    public:
    std::string getDescription () const override {
        return "Dark Roast Coffee";
    }

    double cost () const override {
        return 0.99;
    }
};

// ----- Decorator -----
// Maintains a reference to a Component object and defines an interface
// that conforms to Component's interface
class CondimentDecorator : public Beverage {
    public:
    explicit CondimentDecorator (std::unique_ptr<Beverage> beverage)
    : beverage_ (std::move (beverage)) {
    }

    protected:
    // Protected so decorators can access the wrapped beverage
    Beverage* getBeverage () const {
        return beverage_.get ();
    }

    private:
    std::unique_ptr<Beverage> beverage_;
};

// ----- Concrete Decorators -----
// Each decorator adds its own behavior before or after delegating to the wrapped object
class Milk : public CondimentDecorator {
    public:
    explicit Milk (std::unique_ptr<Beverage> beverage)
    : CondimentDecorator (std::move (beverage)) {
    }

    std::string getDescription () const override {
        return getBeverage ()->getDescription () + ", Milk";
    }

    double cost () const override {
        return getBeverage ()->cost () + 0.10;
    }
};

class Mocha : public CondimentDecorator {
    public:
    explicit Mocha (std::unique_ptr<Beverage> beverage)
    : CondimentDecorator (std::move (beverage)) {
    }

    std::string getDescription () const override {
        return getBeverage ()->getDescription () + ", Mocha";
    }

    double cost () const override {
        return getBeverage ()->cost () + 0.20;
    }
};

class Soy : public CondimentDecorator {
    public:
    explicit Soy (std::unique_ptr<Beverage> beverage)
    : CondimentDecorator (std::move (beverage)) {
    }

    std::string getDescription () const override {
        return getBeverage ()->getDescription () + ", Soy";
    }

    double cost () const override {
        return getBeverage ()->cost () + 0.15;
    }
};

class Whip : public CondimentDecorator {
    public:
    explicit Whip (std::unique_ptr<Beverage> beverage)
    : CondimentDecorator (std::move (beverage)) {
    }

    std::string getDescription () const override {
        return getBeverage ()->getDescription () + ", Whip";
    }

    double cost () const override {
        return getBeverage ()->cost () + 0.10;
    }
};

} // namespace Decorator

#endif
