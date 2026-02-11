/*
The intent is to encapsulate a request as an object, thereby letting you parameterize
clients with different requests, queue or log requests, and support undoable operations.

Key participants:
- Command: Declares an interface for executing an operation
- ConcreteCommand: Defines a binding between a Receiver and an action
- Invoker: Asks the command to carry out the request
- Receiver: Knows how to perform the operations

Key benefits:
- Decouples the object that invokes the operation from the one that knows how to perform it
- Commands can be manipulated and extended like any other object
- Easy to add new commands without changing existing code
- Can composite commands into macro commands

Real-world example: A remote control that can execute various commands on devices,
with support for undo operations.
*/
#ifndef C2_COMMAND_H
#define C2_COMMAND_H

#include <iostream>
#include <memory>
#include <stack>
#include <string>

namespace Command {

// ----- Receiver -----
class Light {
    public:
    void on () {
        std::cout << "Light is ON\n";
    }

    void off () {
        std::cout << "Light is OFF\n";
    }

    void dim (int level) {
        std::cout << "Light dimmed to " << level << "%\n";
    }
};

class Stereo {
    public:
    void on () {
        std::cout << "Stereo is ON\n";
    }

    void off () {
        std::cout << "Stereo is OFF\n";
    }

    void setCD () {
        std::cout << "Stereo: CD player selected\n";
    }

    void setDVD () {
        std::cout << "Stereo: DVD player selected\n";
    }

    void setVolume (int level) {
        std::cout << "Stereo: volume set to " << level << "\n";
    }
};

class CeilingFan {
    public:
    enum class Speed { OFF, LOW, MEDIUM, HIGH };

    void setSpeed (Speed speed) {
        currentSpeed_ = speed;
        switch (speed) {
        case Speed::OFF: std::cout << "Ceiling Fan: OFF\n"; break;
        case Speed::LOW: std::cout << "Ceiling Fan: LOW speed\n"; break;
        case Speed::MEDIUM: std::cout << "Ceiling Fan: MEDIUM speed\n"; break;
        case Speed::HIGH: std::cout << "Ceiling Fan: HIGH speed\n"; break;
        }
    }

    Speed getSpeed () const {
        return currentSpeed_;
    }

    private:
    Speed currentSpeed_ = Speed::OFF;
};

// ----- Command interface -----
class Command {
    public:
    virtual ~Command ()     = default;
    virtual void execute () = 0;
    virtual void undo ()    = 0;
};

// ----- Concrete Commands -----
class LightOnCommand : public Command {
    public:
    explicit LightOnCommand (std::shared_ptr<Light> light)
    : light_ (std::move (light)) {
    }

    void execute () override {
        light_->on ();
    }

    void undo () override {
        light_->off ();
    }

    private:
    std::shared_ptr<Light> light_;
};

class LightOffCommand : public Command {
    public:
    explicit LightOffCommand (std::shared_ptr<Light> light)
    : light_ (std::move (light)) {
    }

    void execute () override {
        light_->off ();
    }

    void undo () override {
        light_->on ();
    }

    private:
    std::shared_ptr<Light> light_;
};

class StereoOnWithCDCommand : public Command {
    public:
    explicit StereoOnWithCDCommand (std::shared_ptr<Stereo> stereo)
    : stereo_ (std::move (stereo)) {
    }

    void execute () override {
        stereo_->on ();
        stereo_->setCD ();
        stereo_->setVolume (11);
    }

    void undo () override {
        stereo_->off ();
    }

    private:
    std::shared_ptr<Stereo> stereo_;
};

class StereoOffCommand : public Command {
    public:
    explicit StereoOffCommand (std::shared_ptr<Stereo> stereo)
    : stereo_ (std::move (stereo)) {
    }

    void execute () override {
        stereo_->off ();
    }

    void undo () override {
        stereo_->on ();
        stereo_->setCD ();
        stereo_->setVolume (11);
    }

    private:
    std::shared_ptr<Stereo> stereo_;
};

class CeilingFanHighCommand : public Command {
    public:
    explicit CeilingFanHighCommand (std::shared_ptr<CeilingFan> fan)
    : fan_ (std::move (fan)) {
    }

    void execute () override {
        prevSpeed_ = fan_->getSpeed ();
        fan_->setSpeed (CeilingFan::Speed::HIGH);
    }

    void undo () override {
        fan_->setSpeed (prevSpeed_);
    }

    private:
    std::shared_ptr<CeilingFan> fan_;
    CeilingFan::Speed prevSpeed_;
};

class CeilingFanOffCommand : public Command {
    public:
    explicit CeilingFanOffCommand (std::shared_ptr<CeilingFan> fan)
    : fan_ (std::move (fan)) {
    }

    void execute () override {
        prevSpeed_ = fan_->getSpeed ();
        fan_->setSpeed (CeilingFan::Speed::OFF);
    }

    void undo () override {
        fan_->setSpeed (prevSpeed_);
    }

    private:
    std::shared_ptr<CeilingFan> fan_;
    CeilingFan::Speed prevSpeed_;
};

// No-op command for empty slots
class NoCommand : public Command {
    public:
    void execute () override {
        std::cout << "[No operation]\n";
    }

    void undo () override {
        // Nothing to undo
    }
};

// ----- Invoker -----
class RemoteControl {
    public:
    static constexpr int NUM_SLOTS = 7;

    RemoteControl () {
        // Initialize all slots with NoCommand
        for (int i = 0; i < NUM_SLOTS; ++i) {
            onCommands_[i]  = std::make_shared<NoCommand> ();
            offCommands_[i] = std::make_shared<NoCommand> ();
        }
    }

    void setCommand (int slot, std::shared_ptr<Command> onCommand, std::shared_ptr<Command> offCommand) {
        onCommands_[slot]  = onCommand;
        offCommands_[slot] = offCommand;
    }

    void onButtonWasPressed (int slot) {
        std::cout << "\n[Remote: ON button " << slot << " pressed]\n";
        onCommands_[slot]->execute ();
        undoStack.push (onCommands_[slot]);
    }

    void offButtonWasPressed (int slot) {
        std::cout << "\n[Remote: OFF button " << slot << " pressed]\n";
        offCommands_[slot]->execute ();
        undoStack.push (offCommands_[slot]);
    }

    void undoButtonWasPressed () {
        std::cout << "\n[Remote: UNDO button pressed]\n";
        if (!undoStack.empty ()) {
            undoStack.top ()->undo ();
            undoStack.pop ();
        } else {
            std::cout << "Nothing to undo.\n";
        }
    }

    private:
    std::shared_ptr<Command> onCommands_[NUM_SLOTS];
    std::shared_ptr<Command> offCommands_[NUM_SLOTS];
    std::stack<std::shared_ptr<Command>> undoStack;
};

// ----- Macro Command -----
class MacroCommand : public Command {
    public:
    explicit MacroCommand (std::vector<std::shared_ptr<Command>> commands)
    : commands_ (std::move (commands)) {
    }

    void execute () override {
        for (auto& command : commands_) {
            command->execute ();
        }
    }

    void undo () override {
        for (auto it = commands_.rbegin (); it != commands_.rend (); ++it) {
            (*it)->undo ();
        }
    }

    private:
    std::vector<std::shared_ptr<Command>> commands_;
};

} // namespace Command

#endif
