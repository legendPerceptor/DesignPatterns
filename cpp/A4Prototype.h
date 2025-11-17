/*
The intent is to specify the kinds of objects to create using a prototypical
instance, and create new objects by copying this prototype.

Prototype interface with `clone()` method.
`ConcretePrototypeA`, `ConcretePrototypeB` implement `clone()`.
*/
#ifndef A4_PROTOTYPE_H
#define A4_PROTOTYPE_H

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

namespace Prototype {
// ----- Prototype -----
class Shape {
    public:
    virtual ~Shape () = default;

    virtual void draw () const = 0;

    // Prototype's key method
    virtual std::unique_ptr<Shape> clone () const = 0;
};

// ----- Concrete Prototypes -----
class Circle : public Shape {
    public:
    Circle () = default;
    Circle (double radius, std::string color)
    : radius_ (radius), color_ (std::move (color)) {
    }

    void draw () const override {
        std::cout << "Circle: radius=" << radius_ << ", color=" << color_ << "\n";
    }

    std::unique_ptr<Shape> clone () const override {
        // copy current state
        return std::make_unique<Circle> (*this);
    }

    void setRadius (double r) {
        radius_ = r;
    }
    void setColor (const std::string& c) {
        color_ = c;
    }

    private:
    double radius_     = 1.0;
    std::string color_ = "black";
};

class Rectangle : public Shape {
    public:
    Rectangle () = default;
    Rectangle (double w, double h, std::string color)
    : width_ (w), height_ (h), color_ (std::move (color)) {
    }

    void draw () const override {
        std::cout << "Rectangle: " << width_ << "x" << height_
                  << ", color=" << color_ << "\n";
    }

    std::unique_ptr<Shape> clone () const override {
        return std::make_unique<Rectangle> (*this);
    }

    void setSize (double w, double h) {
        width_  = w;
        height_ = h;
    }
    void setColor (const std::string& c) {
        color_ = c;
    }

    private:
    double width_      = 1.0;
    double height_     = 1.0;
    std::string color_ = "black";
};

// ----- Prototype Registry / Factory using Prototypes -----
class PrototypeRegistry {
    public:
    void registerPrototype (const std::string& id, std::unique_ptr<Shape> prototype) {
        prototypes_[id] = std::move (prototype);
    }

    std::unique_ptr<Shape> create (const std::string& id) const {
        auto it = prototypes_.find (id);
        if (it == prototypes_.end ()) {
            return nullptr;
        }
        return it->second->clone (); // 🔑 cloning the prototype
    }

    private:
    std::unordered_map<std::string, std::unique_ptr<Shape>> prototypes_;
};

}; // namespace Prototype
#endif