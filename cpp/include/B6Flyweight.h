/*
The intent is to use sharing to support large numbers of fine-grained objects efficiently.
A flyweight is a shared object that can be used in multiple contexts simultaneously.

Key concepts:
- Intrinsic state: Shared, independent of the flyweight's context
- Extrinsic state: Dependent on and varies with the flyweight's context

Key benefits:
- Reduces memory usage by sharing objects
- Allows large numbers of objects when most state can be made extrinsic

Real-world example: A text editor where each character is an object. Instead of creating
a new object for every 'a' in the document, we share a single 'a' flyweight and only
store the position (extrinsic state) for each occurrence.
*/
#ifndef B6_FLYWEIGHT_H
#define B6_FLYWEIGHT_H

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace Flyweight {

// ----- Flyweight interface -----
class Character {
    public:
    virtual ~Character () = default;
    virtual void display (int fontSize, const std::string& color) const = 0;
    virtual char getChar () const                                       = 0;
};

// ----- Concrete Flyweight -----
// Contains intrinsic state (the character itself)
class Letter : public Character {
    public:
    explicit Letter (char letter) : letter_ (letter) {
    }

    void display (int fontSize, const std::string& color) const override {
        std::cout << "Letter '" << letter_ << "' (size=" << fontSize
                  << ", color=" << color << ")\n";
    }

    char getChar () const override {
        return letter_;
    }

    private:
    char letter_; // Intrinsic state - shared
};

// ----- Unshared Concrete Flyweight -----
// Some flyweights may not be shared
class Space : public Character {
    public:
    void display (int fontSize, const std::string& color) const override {
        std::cout << "[Space] (size=" << fontSize << ")\n";
    }

    char getChar () const override {
        return ' ';
    }
};

// ----- Flyweight Factory -----
// Creates and manages flyweight objects, ensures they are shared properly
class CharacterFactory {
    public:
    // Get or create a flyweight (shared object)
    std::shared_ptr<Character> getCharacter (char key) {
        // If the character exists, return it
        if (characters_.find (key) != characters_.end ()) {
            return characters_[key];
        }
        // Otherwise create and store it
        auto character   = std::make_shared<Letter> (key);
        characters_[key] = character;
        std::cout << "Creating new flyweight for '" << key << "'\n";
        return character;
    }

    std::shared_ptr<Space> getSpace () {
        if (!space_) {
            space_ = std::make_shared<Space> ();
        }
        return space_;
    }

    size_t getFlyweightCount () const {
        return characters_.size ();
    }

    private:
    std::map<char, std::shared_ptr<Letter>> characters_;
    std::shared_ptr<Space> space_;
};

// ----- Context -----
// Contains extrinsic state (varies with context)
class GlyphContext {
    public:
    GlyphContext (std::shared_ptr<Character> character, int fontSize, const std::string& color)
    : character_ (character), fontSize_ (fontSize), color_ (color) {
    }

    void display () const {
        character_->display (fontSize_, color_);
    }

    private:
    std::shared_ptr<Character> character_; // Reference to shared flyweight
    int fontSize_;                         // Extrinsic state
    std::string color_;                    // Extrinsic state
};

// Example: Document using flyweights for efficient character rendering
class Document {
    public:
    explicit Document (std::shared_ptr<CharacterFactory> factory)
    : factory_ (factory) {
    }

    // Add a character to the document (extrinsic state stored here)
    void addCharacter (char c, int fontSize, const std::string& color) {
        auto character = factory_->getCharacter (c);
        glyphs_.push_back (std::make_unique<GlyphContext> (character, fontSize, color));
    }

    void addSpace (int fontSize) {
        auto space = factory_->getSpace ();
        glyphs_.push_back (std::make_unique<GlyphContext> (space, fontSize, "black"));
    }

    void render () const {
        std::cout << "\nRendering document (" << glyphs_.size () << " glyphs, "
                  << factory_->getFlyweightCount () << " flyweights used):\n";
        for (const auto& glyph : glyphs_) {
            glyph->display ();
        }
    }

    private:
    std::shared_ptr<CharacterFactory> factory_;
    std::vector<std::unique_ptr<GlyphContext>> glyphs_;
};

// Another example: Tree objects with shared intrinsic state
class TreeType {
    public:
    TreeType (const std::string& name, const std::string& color, const std::string& texture)
    : name_ (name), color_ (color), texture_ (texture) {
    }

    void draw (int x, int y) const {
        std::cout << "Drawing " << name_ << " tree at (" << x << "," << y
                  << ") - color: " << color_ << ", texture: " << texture_ << "\n";
    }

    std::string getName () const {
        return name_;
    }

    private:
    std::string name_;    // Intrinsic state
    std::string color_;   // Intrinsic state
    std::string texture_; // Intrinsic state
};

class TreeFactory {
    public:
    std::shared_ptr<TreeType> getTreeType (const std::string& name,
    const std::string& color,
    const std::string& texture) {
        std::string key = name + "-" + color + "-" + texture;
        if (treeTypes_.find (key) == treeTypes_.end ()) {
            treeTypes_[key] = std::make_shared<TreeType> (name, color, texture);
            std::cout << "Creating new TreeType: " << name << "\n";
        }
        return treeTypes_[key];
    }

    private:
    std::map<std::string, std::shared_ptr<TreeType>> treeTypes_;
};

class Tree {
    public:
    Tree (int x, int y, std::shared_ptr<TreeType> type)
    : x_ (x), y_ (y), type_ (std::move (type)) {
    }

    void draw () const {
        type_->draw (x_, y_);
    }

    private:
    int x_, y_;                      // Extrinsic state (position)
    std::shared_ptr<TreeType> type_; // Reference to shared intrinsic state
};

class Forest {
    public:
    void plantTree (int x,
    int y,
    const std::string& name,
    const std::string& color,
    const std::string& texture) {
        auto type = factory_.getTreeType (name, color, texture);
        trees_.push_back (std::make_unique<Tree> (x, y, type));
    }

    void draw () const {
        std::cout << "\nDrawing forest:\n";
        for (const auto& tree : trees_) {
            tree->draw ();
        }
    }

    private:
    TreeFactory factory_;
    std::vector<std::unique_ptr<Tree>> trees_;
};

} // namespace Flyweight

#endif
