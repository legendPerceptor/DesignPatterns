/*
The intent is to capture and externalize an object's internal state without
violating encapsulation, so that the object can be restored to this state later.

Key participants:
- Memento: Stores internal state of the Originator object; prevents outside
  objects from accessing it
- Originator: Creates a memento containing a snapshot of its current internal
  state and uses the memento to restore its internal state
- Caretaker: Responsible for the memento's safekeeping; never operates on or
  examines the contents of a memento

Key benefits:
- Preserves encapsulation boundaries
- Simplifies the Originator by keeping the snapshot logic in the Memento
- Provides easy rollback/undo functionality

Real-world example: A text editor that saves snapshots of document state,
allowing users to undo/redo changes.
*/
#ifndef C7_MEMENTO_H
#define C7_MEMENTO_H

#include <iostream>
#include <memory>
#include <stack>
#include <string>
#include <vector>

namespace Memento {

// ----- Memento -----
class DocumentMemento {
    public:
    DocumentMemento (const std::string& content, int cursorPosition)
    : content_ (content), cursorPosition_ (cursorPosition), timestamp_ (getTime ()) {
    }

    std::string getContent () const {
        return content_;
    }

    int getCursorPosition () const {
        return cursorPosition_;
    }

    std::string getTimestamp () const {
        return timestamp_;
    }

    private:
    std::string content_;
    int cursorPosition_;
    std::string timestamp_;

    static std::string getTime () {
        time_t now = time (nullptr);
        char buf[80];
        strftime (buf, sizeof (buf), "%H:%M:%S", localtime (&now));
        return std::string (buf);
    }
};

// ----- Originator -----
class TextDocument {
    public:
    TextDocument () : content_ (""), cursorPosition_ (0) {
    }

    void write (const std::string& text) {
        content_ += text;
        cursorPosition_ += text.length ();
        std::cout << "[Document] Wrote: \"" << text << "\"\n";
        std::cout << "  Current content: \"" << content_ << "\"\n";
    }

    void deleteLast (int count = 1) {
        if (content_.length () >= count) {
            content_.erase (content_.length () - count);
            cursorPosition_ = std::max (0, cursorPosition_ - count);
            std::cout << "[Document] Deleted last " << count << " character(s)\n";
            std::cout << "  Current content: \"" << content_ << "\"\n";
        }
    }

    std::shared_ptr<DocumentMemento> save () {
        std::cout << "[Document] Saving state...\n";
        return std::make_shared<DocumentMemento> (content_, cursorPosition_);
    }

    void restore (std::shared_ptr<DocumentMemento> memento) {
        content_        = memento->getContent ();
        cursorPosition_ = memento->getCursorPosition ();
        std::cout << "[Document] Restored to state from "
                  << memento->getTimestamp () << "\n";
        std::cout << "  Content: \"" << content_ << "\"\n";
        std::cout << "  Cursor position: " << cursorPosition_ << "\n";
    }

    std::string getContent () const {
        return content_;
    }

    private:
    std::string content_;
    int cursorPosition_;
};

// ----- Caretaker -----
class History {
    public:
    void saveState (std::shared_ptr<DocumentMemento> memento) {
        undoStack_.push (memento);
        // Clear redo stack when new action is performed
        while (!redoStack_.empty ()) {
            redoStack_.pop ();
        }
        std::cout
        << "[History] State saved. Undo stack size: " << undoStack_.size () << "\n\n";
    }

    std::shared_ptr<DocumentMemento> undo () {
        if (undoStack_.size () <= 1) {
            std::cout << "[History] Nothing to undo.\n";
            return nullptr;
        }

        // Pop current state and push to redo stack
        redoStack_.push (undoStack_.top ());
        undoStack_.pop ();

        auto memento = undoStack_.top ();
        std::cout << "[History] Undo performed. Remaining: " << undoStack_.size () << "\n";
        return memento;
    }

    std::shared_ptr<DocumentMemento> redo () {
        if (redoStack_.empty ()) {
            std::cout << "[History] Nothing to redo.\n";
            return nullptr;
        }

        auto memento = redoStack_.top ();
        redoStack_.pop ();
        undoStack_.push (memento);

        std::cout << "[History] Redo performed.\n";
        return memento;
    }

    void setInitialState (std::shared_ptr<DocumentMemento> memento) {
        undoStack_.push (memento);
    }

    private:
    std::stack<std::shared_ptr<DocumentMemento>> undoStack_;
    std::stack<std::shared_ptr<DocumentMemento>> redoStack_;
};

// ----- Example: Game Character with health/position state -----
class GameCharacterMemento {
    public:
    GameCharacterMemento (int health, int x, int y, const std::string& state)
    : health_ (health), x_ (x), y_ (y), state_ (state) {
    }

    int getHealth () const {
        return health_;
    }
    int getX () const {
        return x_;
    }
    int getY () const {
        return y_;
    }
    std::string getState () const {
        return state_;
    }

    private:
    int health_;
    int x_;
    int y_;
    std::string state_;
};

class GameCharacter {
    public:
    GameCharacter (const std::string& name)
    : name_ (name), health_ (100), x_ (0), y_ (0), state_ ("Normal") {
    }

    void takeDamage (int damage) {
        health_ = std::max (0, health_ - damage);
        state_  = health_ > 70 ? "Normal" :
         health_ > 30          ? "Wounded" :
                                 "Critical";
        std::cout << "[" << name_ << "] Took " << damage
                  << " damage. Health: " << health_ << " (" << state_ << ")\n";
    }

    void move (int dx, int dy) {
        x_ += dx;
        y_ += dy;
        std::cout << "[" << name_ << "] Moved to (" << x_ << ", " << y_ << ")\n";
    }

    void heal (int amount) {
        health_ = std::min (100, health_ + amount);
        state_  = health_ > 70 ? "Normal" :
         health_ > 30          ? "Wounded" :
                                 "Critical";
        std::cout << "[" << name_ << "] Healed " << amount
                  << ". Health: " << health_ << " (" << state_ << ")\n";
    }

    std::shared_ptr<GameCharacterMemento> createCheckpoint () {
        std::cout << "[" << name_ << "] Checkpoint created!\n";
        return std::make_shared<GameCharacterMemento> (health_, x_, y_, state_);
    }

    void restoreFromCheckpoint (std::shared_ptr<GameCharacterMemento> memento) {
        health_ = memento->getHealth ();
        x_      = memento->getX ();
        y_      = memento->getY ();
        state_  = memento->getState ();
        std::cout << "[" << name_ << "] Restored from checkpoint!\n";
        std::cout << "  Health: " << health_ << ", Position: (" << x_ << ", "
                  << y_ << "), State: " << state_ << "\n";
    }

    private:
    std::string name_;
    int health_;
    int x_;
    int y_;
    std::string state_;
};

} // namespace Memento

#endif
