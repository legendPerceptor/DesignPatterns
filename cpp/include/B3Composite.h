/*
The intent is to compose objects into tree structures to represent part-whole hierarchies.
Composite lets clients treat individual objects and compositions of objects uniformly.

Key participants:
1. Component - declares the interface for objects in the composition
2. Leaf - represents individual objects with no children
3. Composite - represents groups of objects that can have children
4. Client - manipulates objects through the Component interface

Real-world example: A file system where files and directories should be treated uniformly.
Both can be renamed, moved, or have their size calculated, but directories can contain other items.
*/
#ifndef B3_COMPOSITE_H
#define B3_COMPOSITE_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace Composite {

// ----- Component interface -----
// Declares the interface for objects in the composition
class FileSystemNode {
    public:
    virtual ~FileSystemNode () = default;

    virtual void print (const std::string& indent = "") const = 0;
    virtual size_t size () const                              = 0;
    virtual std::string getName () const                      = 0;

    // Composite-specific operations (default implementation for leaves)
    virtual void add (std::shared_ptr<FileSystemNode> node) {
        std::cout << "Cannot add to a file\n";
    }
    virtual void remove (const std::string& name) {
        std::cout << "Cannot remove from a file\n";
    }
};

// ----- Leaf -----
// Represents individual objects (files) with no children
class File : public FileSystemNode {
    public:
    File (const std::string& name, size_t fileSize)
    : name_ (name), size_ (fileSize) {
    }

    void print (const std::string& indent = "") const override {
        std::cout << indent << "File: " << name_ << " (" << size_ << " bytes)\n";
    }

    size_t size () const override {
        return size_;
    }

    std::string getName () const override {
        return name_;
    }

    private:
    std::string name_;
    size_t size_;
};

// ----- Composite -----
// Represents groups of objects (directories) that can have children
class Directory : public FileSystemNode {
    public:
    explicit Directory (const std::string& name) : name_ (name) {
    }

    void add (std::shared_ptr<FileSystemNode> node) override {
        children_.push_back (std::move (node));
    }

    void remove (const std::string& name) override {
        auto it = std::find_if (children_.begin (), children_.end (),
        [&name] (const auto& child) { return child->getName () == name; });
        if (it != children_.end ()) {
            children_.erase (it);
        }
    }

    void print (const std::string& indent = "") const override {
        std::cout << indent << "Directory: " << name_ << "/ (" << size () << " bytes total)\n";
        for (const auto& child : children_) {
            child->print (indent + "  ");
        }
    }

    size_t size () const override {
        size_t total = 0;
        for (const auto& child : children_) {
            total += child->size ();
        }
        return total;
    }

    std::string getName () const override {
        return name_;
    }

    private:
    std::string name_;
    std::vector<std::shared_ptr<FileSystemNode>> children_;
};

} // namespace Composite

#endif
