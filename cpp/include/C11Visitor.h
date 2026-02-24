/*
The intent is to represent an operation to be performed on the elements of an object
structure. Visitor lets you define a new operation without changing the classes of
the elements on which it operates.

Key participants:
- Visitor: Declares a visit operation for each class of ConcreteElement in the
  object structure
- ConcreteVisitor: Implements each operation declared by Visitor; implements the
  algorithm for the corresponding element
- Element: Defines an accept operation that takes a visitor as an argument
- ConcreteElement: Implements an accept operation that takes a visitor as an argument
- ObjectStructure: Can enumerate its elements; may provide a high-level interface
  to allow the visitor to visit its elements

Key benefits:
- Makes adding new operations easy
- Gathers related operations in one class
- Adds operations across classes of objects

Real-world example: A tax calculator that visits different types of items (books,
food, electronics) and calculates tax based on the item type.
*/
#ifndef C11_VISITOR_H
#define C11_VISITOR_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace Visitor {

// ----- Forward declarations -----
class Book;
class Fruit;
class Electronics;

// ----- Visitor interface -----
class Visitor {
    public:
    virtual ~Visitor ()                           = default;
    virtual void visit (Book& book)               = 0;
    virtual void visit (Fruit& fruit)             = 0;
    virtual void visit (Electronics& electronics) = 0;
    virtual std::string getName () const          = 0;
};

// ----- Element interface -----
class Item {
    public:
    virtual ~Item ()                       = default;
    virtual void accept (Visitor& visitor) = 0;
    virtual std::string getName () const   = 0;
    virtual double getPrice () const       = 0;
};

// ----- Concrete Elements -----
class Book : public Item {
    public:
    Book (const std::string& title, const std::string& author, double price)
    : title_ (title), author_ (author), price_ (price) {
    }

    void accept (Visitor& visitor) override {
        visitor.visit (*this);
    }

    std::string getTitle () const {
        return title_;
    }

    std::string getAuthor () const {
        return author_;
    }

    std::string getName () const override {
        return "Book: " + title_;
    }

    double getPrice () const override {
        return price_;
    }

    private:
    std::string title_;
    std::string author_;
    double price_;
};

class Fruit : public Item {
    public:
    Fruit (const std::string& name, double pricePerKg, double weight)
    : name_ (name), pricePerKg_ (pricePerKg), weight_ (weight) {
    }

    void accept (Visitor& visitor) override {
        visitor.visit (*this);
    }

    std::string getName () const override {
        return name_;
    }

    double getPrice () const override {
        return pricePerKg_ * weight_;
    }

    double getPricePerKg () const {
        return pricePerKg_;
    }

    double getWeight () const {
        return weight_;
    }

    private:
    std::string name_;
    double pricePerKg_;
    double weight_;
};

class Electronics : public Item {
    public:
    Electronics (const std::string& model, double price, bool energyEfficient)
    : model_ (model), price_ (price), energyEfficient_ (energyEfficient) {
    }

    void accept (Visitor& visitor) override {
        visitor.visit (*this);
    }

    std::string getModel () const {
        return model_;
    }

    std::string getName () const override {
        return "Electronics: " + model_;
    }

    double getPrice () const override {
        return price_;
    }

    bool isEnergyEfficient () const {
        return energyEfficient_;
    }

    private:
    std::string model_;
    double price_;
    bool energyEfficient_;
};

// ----- Concrete Visitors -----
class TaxVisitor : public Visitor {
    public:
    void visit (Book& book) override {
        double tax = book.getPrice () * bookTaxRate_;
        std::cout << "[TaxVisitor] " << book.getName () << " by "
                  << book.getAuthor () << "\n";
        std::cout << "  Price: $" << book.getPrice () << "\n";
        std::cout << "  Tax (" << (bookTaxRate_ * 100) << "%): $" << tax << "\n";
        std::cout << "  Total: $" << (book.getPrice () + tax) << "\n\n";
    }

    void visit (Fruit& fruit) override {
        double tax = fruit.getPrice () * foodTaxRate_;
        std::cout << "[TaxVisitor] " << fruit.getName () << " (" << fruit.getWeight ()
                  << " kg @ $" << fruit.getPricePerKg () << "/kg)\n";
        std::cout << "  Price: $" << fruit.getPrice () << "\n";
        std::cout << "  Tax (" << (foodTaxRate_ * 100) << "%): $" << tax << "\n";
        std::cout << "  Total: $" << (fruit.getPrice () + tax) << "\n\n";
    }

    void visit (Electronics& electronics) override {
        double tax         = electronics.getPrice () * electronicsTaxRate_;
        double ecoDiscount = electronics.isEnergyEfficient () ?
        electronics.getPrice () * ecoDiscountRate_ :
        0.0;
        double totalTax    = tax - ecoDiscount;

        std::cout << "[TaxVisitor] " << electronics.getModel () << "\n";
        std::cout << "  Price: $" << electronics.getPrice () << "\n";
        std::cout << "  Tax (" << (electronicsTaxRate_ * 100) << "%): $" << tax << "\n";
        if (electronics.isEnergyEfficient ()) {
            std::cout << "  Eco discount (" << (ecoDiscountRate_ * 100)
                      << "): -$" << ecoDiscount << "\n";
        }
        std::cout << "  Total tax: $" << totalTax << "\n";
        std::cout << "  Total: $" << (electronics.getPrice () + totalTax) << "\n\n";
    }

    std::string getName () const override {
        return "Tax Calculator";
    }

    private:
    double bookTaxRate_        = 0.05; // 5% for books
    double foodTaxRate_        = 0.02; // 2% for food
    double electronicsTaxRate_ = 0.15; // 15% for electronics
    double ecoDiscountRate_    = 0.03; // 3% discount for eco-friendly
};

class DiscountVisitor : public Visitor {
    public:
    void visit (Book& book) override {
        double discount = book.getPrice () * bookDiscountRate_;
        std::cout << "[DiscountVisitor] " << book.getName () << "\n";
        std::cout << "  Original Price: $" << book.getPrice () << "\n";
        std::cout << "  Discount (" << (bookDiscountRate_ * 100) << "%): -$"
                  << discount << "\n";
        std::cout << "  Sale Price: $" << (book.getPrice () - discount) << "\n\n";
    }

    void visit (Fruit& fruit) override {
        double discount = 0.0;
        // No discount on fruits
        std::cout << "[DiscountVisitor] " << fruit.getName () << "\n";
        std::cout << "  Price: $" << fruit.getPrice () << "\n";
        std::cout << "  Discount: None\n";
        std::cout << "  Sale Price: $" << fruit.getPrice () << "\n\n";
    }

    void visit (Electronics& electronics) override {
        double discount = electronics.getPrice () * electronicsDiscountRate_;
        std::cout << "[DiscountVisitor] " << electronics.getModel () << "\n";
        std::cout << "  Original Price: $" << electronics.getPrice () << "\n";
        std::cout << "  Discount (" << (electronicsDiscountRate_ * 100)
                  << "%): -$" << discount << "\n";
        std::cout << "  Sale Price: $" << (electronics.getPrice () - discount) << "\n\n";
    }

    std::string getName () const override {
        return "Discount Calculator";
    }

    private:
    double bookDiscountRate_        = 0.10; // 10% off books
    double electronicsDiscountRate_ = 0.20; // 20% off electronics
};

class InventoryVisitor : public Visitor {
    public:
    void visit (Book& book) override {
        bookCount_++;
        totalValue_ += book.getPrice ();
        std::cout << "[InventoryVisitor] Logged book: " << book.getName () << "\n";
    }

    void visit (Fruit& fruit) override {
        fruitCount_++;
        totalValue_ += fruit.getPrice ();
        std::cout << "[InventoryVisitor] Logged fruit: " << fruit.getName ()
                  << " (" << fruit.getWeight () << "kg)\n";
    }

    void visit (Electronics& electronics) override {
        electronicsCount_++;
        totalValue_ += electronics.getPrice ();
        std::cout
        << "[InventoryVisitor] Logged electronics: " << electronics.getModel () << "\n";
    }

    std::string getName () const override {
        return "Inventory Counter";
    }

    void printSummary () const {
        std::cout << "\n[InventoryVisitor] Summary:\n";
        std::cout << "  Books: " << bookCount_ << "\n";
        std::cout << "  Fruits: " << fruitCount_ << "\n";
        std::cout << "  Electronics: " << electronicsCount_ << "\n";
        std::cout
        << "  Total items: " << (bookCount_ + fruitCount_ + electronicsCount_) << "\n";
        std::cout << "  Total value: $" << totalValue_ << "\n\n";
    }

    private:
    int bookCount_        = 0;
    int fruitCount_       = 0;
    int electronicsCount_ = 0;
    double totalValue_    = 0.0;
};

// ----- Object Structure -----
class ShoppingCart {
    public:
    void addItem (std::shared_ptr<Item> item) {
        items_.push_back (item);
    }

    void accept (Visitor& visitor) {
        std::cout << "\n[ShoppingCart] Processing cart with visitor: "
                  << visitor.getName () << "\n";
        std::cout << "[ShoppingCart] Items in cart: " << items_.size () << "\n\n";
        for (auto& item : items_) {
            item->accept (visitor);
        }
    }

    private:
    std::vector<std::shared_ptr<Item>> items_;
};

// ----- Example 2: File System Visitor -----
class File;
class Directory;

class FileSystemVisitor {
    public:
    virtual ~FileSystemVisitor ()               = default;
    virtual void visit (File& file)             = 0;
    virtual void visit (Directory& directory)   = 0;
    virtual std::string getVisitorName () const = 0;
};

class FileSystemNode {
    public:
    virtual ~FileSystemNode ()                       = default;
    virtual void accept (FileSystemVisitor& visitor) = 0;
    virtual std::string getName () const             = 0;
    virtual int getSize () const                     = 0;
};

class File : public FileSystemNode {
    public:
    File (const std::string& name, int size) : name_ (name), size_ (size) {
    }

    void accept (FileSystemVisitor& visitor) override {
        visitor.visit (*this);
    }

    std::string getName () const override {
        return name_;
    }

    int getSize () const override {
        return size_;
    }

    private:
    std::string name_;
    int size_;
};

class Directory : public FileSystemNode {
    public:
    explicit Directory (const std::string& name) : name_ (name) {
    }

    void addNode (std::shared_ptr<FileSystemNode> node) {
        children_.push_back (node);
    }

    void accept (FileSystemVisitor& visitor) override {
        visitor.visit (*this);
        for (auto& child : children_) {
            child->accept (visitor);
        }
    }

    std::string getName () const override {
        return name_;
    }

    int getSize () const override {
        int total = 0;
        for (auto& child : children_) {
            total += child->getSize ();
        }
        return total;
    }

    private:
    std::string name_;
    std::vector<std::shared_ptr<FileSystemNode>> children_;
};

class SizeCalculatorVisitor : public FileSystemVisitor {
    public:
    void visit (File& file) override {
        std::cout << "[SizeCalculator] File: " << file.getName () << " ("
                  << file.getSize () << " bytes)\n";
        totalSize_ += file.getSize ();
    }

    void visit (Directory& directory) override {
        std::cout << "[SizeCalculator] Directory: " << directory.getName () << "/\n";
        // Don't add directory size here - children will be visited
    }

    std::string getVisitorName () const override {
        return "Size Calculator";
    }

    int getTotalSize () const {
        return totalSize_;
    }

    private:
    int totalSize_ = 0;
};

class XmlExporterVisitor : public FileSystemVisitor {
    public:
    void visit (File& file) override {
        indent ();
        xmlOutput_ += "<file name=\"" + file.getName () + "\" size=\"" +
        std::to_string (file.getSize ()) + "\"/>\n";
    }

    void visit (Directory& directory) override {
        indent ();
        xmlOutput_ += "<directory name=\"" + directory.getName () + "\">\n";
        depth_++;
        // Children will be visited through Directory::accept
        depth_--;
        indent ();
        xmlOutput_ += "</directory>\n";
    }

    std::string getVisitorName () const override {
        return "XML Exporter";
    }

    std::string getXmlOutput () const {
        return xmlOutput_;
    }

    private:
    void indent () {
        for (int i = 0; i < depth_; ++i) {
            xmlOutput_ += "  ";
        }
    }

    int depth_             = 0;
    std::string xmlOutput_ = "<?xml version=\"1.0\"?>\n<filesystem>\n";
};

} // namespace Visitor

#endif
