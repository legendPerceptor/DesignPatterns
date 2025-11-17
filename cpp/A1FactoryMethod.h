/*
The intent is to put object creation into a virtual method.
The subclasses override that method to decide what to create.

Example:
You have `Product` as an interface class.
`ConcreteProductA` and `ConcreteProductB` are the actual classes you want to
create.
`Creator` will be the base class with a *Factory Method* like
virtual Product* createProduct() = 0;
`ConcreteCreatorA` and `ConcreteCreatorB` will override `createProduct()` to
return a specific product.
*/

#ifndef A1_FACTORY_METHOD_H
#define A1_FACTORY_METHOD_H

#include <iostream>
#include <memory>

namespace FactoryMethod {
// The Product
class Document {
    public:
    virtual ~Document () = default;
    virtual void open () = 0;
};

// The ConrecteProductA
class TextDocument : public Document {
    public:
    void open () override {
        std::cout << "Opening a text document.\n";
    }
};

// The ConcreteProductB
class SpreadsheetDocument : public Document {
    public:
    void open () override {
        std::cout << "Opening a spreadsheet document.\n";
    }
};

// The Creator
class Application {
    public:
    virtual ~Application () = default;
    // The Factory Method
    virtual std::unique_ptr<Document> createDocument () = 0;

    // Business logic that uses the factory method.
    void newDocument () {
        auto doc = createDocument ();
        doc->open ();
        std::cout << "Other general stuff done in Application." << std::endl;
    }
};

class TextEditorApp : public Application {
    public:
    std::unique_ptr<Document> createDocument () override {
        return std::make_unique<TextDocument> ();
    }
};

class SpreadsheetApp : public Application {
    std::unique_ptr<Document> createDocument () override {
        return std::make_unique<SpreadsheetDocument> ();
    }
};

}; // namespace FactoryMethod

#endif