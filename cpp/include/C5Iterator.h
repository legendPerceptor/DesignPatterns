/*
The intent is to provide a way to access the elements of an aggregate object sequentially
without exposing its underlying representation.

Key participants:
- Iterator: Defines an interface for accessing and traversing elements
- ConcreteIterator: Implements the iterator interface and keeps track of current position
- Aggregate: Defines an interface for creating an iterator object
- ConcreteAggregate: Implements the iterator creation interface

Key benefits:
- Supports variations in the traversal of an aggregate
- Simplifies the aggregate interface
- Multiple traversals can be active concurrently

Real-world example: A music playlist that can be traversed forward, backward, or shuffled.
*/
#ifndef C3_ITERATOR_H
#define C3_ITERATOR_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace Iterator {

// ----- Iterator interface -----
template <typename T> class Iterator {
    public:
    virtual ~Iterator ()          = default;
    virtual bool hasNext () const = 0;
    virtual T next ()             = 0;
    virtual void reset ()         = 0;
};

// ----- Aggregate interface -----
template <typename T> class Aggregate {
    public:
    virtual ~Aggregate ()                                  = default;
    virtual std::shared_ptr<Iterator<T>> createIterator () = 0;
    virtual void add (const T& item)                       = 0;
    virtual size_t size () const                           = 0;
};

// ----- Concrete Iterator -----
template <typename T> class VectorIterator : public Iterator<T> {
    public:
    explicit VectorIterator (const std::vector<T>& items)
    : items_ (items), index_ (0) {
    }

    bool hasNext () const override {
        return index_ < items_.size ();
    }

    T next () override {
        if (!hasNext ()) {
            throw std::out_of_range ("No more elements");
        }
        return items_[index_++];
    }

    void reset () override {
        index_ = 0;
    }

    private:
    const std::vector<T>& items_;
    size_t index_;
};

// Reverse iterator
template <typename T> class ReverseVectorIterator : public Iterator<T> {
    public:
    explicit ReverseVectorIterator (const std::vector<T>& items)
    : items_ (items), index_ (items.size () - 1) {
    }

    bool hasNext () const override {
        return index_ < items_.size () && !items_.empty ();
    }

    T next () override {
        if (!hasNext ()) {
            throw std::out_of_range ("No more elements");
        }
        return items_[index_--];
    }

    void reset () override {
        if (!items_.empty ()) {
            index_ = items_.size () - 1;
        }
    }

    private:
    const std::vector<T>& items_;
    size_t index_;
};

// ----- Concrete Aggregate -----
template <typename T> class Playlist : public Aggregate<T> {
    public:
    std::shared_ptr<Iterator<T>> createIterator () override {
        return std::make_shared<VectorIterator<T>> (songs_);
    }

    std::shared_ptr<Iterator<T>> createReverseIterator () {
        return std::make_shared<ReverseVectorIterator<T>> (songs_);
    }

    void add (const T& item) override {
        songs_.push_back (item);
    }

    size_t size () const override {
        return songs_.size ();
    }

    private:
    std::vector<T> songs_;
};

// ----- Example usage with Song class -----
class Song {
    public:
    Song (const std::string& title, const std::string& artist)
    : title_ (title), artist_ (artist) {
    }

    std::string getTitle () const {
        return title_;
    }

    std::string getArtist () const {
        return artist_;
    }

    std::string toString () const {
        return "\"" + title_ + "\" by " + artist_;
    }

    bool operator== (const Song& other) const {
        return title_ == other.title_ && artist_ == other.artist_;
    }

    private:
    std::string title_;
    std::string artist_;
};

// ----- Menu example (another common use case) -----
class MenuItem {
    public:
    MenuItem (const std::string& name, const std::string& description, double price)
    : name_ (name), description_ (description), price_ (price) {
    }

    std::string getName () const {
        return name_;
    }

    std::string getDescription () const {
        return description_;
    }

    double getPrice () const {
        return price_;
    }

    private:
    std::string name_;
    std::string description_;
    double price_;
};

class MenuIterator : public Iterator<std::shared_ptr<MenuItem>> {
    public:
    explicit MenuIterator (const std::vector<std::shared_ptr<MenuItem>>& items)
    : items_ (items), index_ (0) {
    }

    bool hasNext () const override {
        return index_ < items_.size ();
    }

    std::shared_ptr<MenuItem> next () override {
        if (!hasNext ()) {
            throw std::out_of_range ("No more items");
        }
        return items_[index_++];
    }

    void reset () override {
        index_ = 0;
    }

    private:
    std::vector<std::shared_ptr<MenuItem>> items_;
    size_t index_;
};

class PancakeHouseMenu {
    public:
    PancakeHouseMenu () {
        addItem ("K&B's Pancake Breakfast", "Pancakes with scrambled eggs and toast", 2.99);
        addItem ("Regular Pancake Breakfast", "Pancakes with fried eggs, sausage", 2.99);
        addItem ("Blueberry Pancakes", "Pancakes made with fresh blueberries", 3.49);
        addItem ("Waffles", "Waffles with your choice of blueberries or strawberries", 3.59);
    }

    void addItem (const std::string& name, const std::string& description, double price) {
        auto item = std::make_shared<MenuItem> (name, description, price);
        menuItems_.push_back (item);
    }

    std::shared_ptr<Iterator<std::shared_ptr<MenuItem>>> createIterator () {
        return std::make_shared<MenuIterator> (menuItems_);
    }

    private:
    std::vector<std::shared_ptr<MenuItem>> menuItems_;
};

class DinerMenu {
    public:
    static constexpr int MAX_ITEMS = 6;
    DinerMenu () : numberOfItems_ (0) {
        addItem ("Vegetarian BLT",
        "(Fakin') Bacon with lettuce & tomato on whole wheat", 2.99);
        addItem ("BLT", "Bacon with lettuce & tomato on whole wheat", 2.99);
        addItem ("Soup of the day", "Soup of the day, with a side of potato salad", 3.29);
        addItem ("Hotdog",
        "A hot dog, with sauerkraut, relish, onions, topped with cheese", 3.05);
    }

    void addItem (const std::string& name, const std::string& description, double price) {
        if (numberOfItems_ >= MAX_ITEMS) {
            std::cout << "Sorry, menu is full! Can't add item to menu.\n";
        } else {
            auto item = std::make_shared<MenuItem> (name, description, price);
            menuItems_.push_back (item);
            numberOfItems_++;
        }
    }

    std::shared_ptr<Iterator<std::shared_ptr<MenuItem>>> createIterator () {
        return std::make_shared<MenuIterator> (menuItems_);
    }

    private:
    std::vector<std::shared_ptr<MenuItem>> menuItems_;
    int numberOfItems_;
};

} // namespace Iterator

#endif
