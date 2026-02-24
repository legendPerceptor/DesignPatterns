/*
The intent is to define an object that encapsulates how a set of objects interact.
Mediator promotes loose coupling by keeping objects from referring to each other
explicitly, and it lets you vary their interaction independently.

Key participants:
- Mediator: Defines an interface for communicating with Colleague objects
- ConcreteMediator: Implements cooperative behavior by coordinating Colleague objects
- Colleague: Each Colleague knows its Mediator and communicates with it rather than
  with other Colleagues directly

Key benefits:
- Decouples colleagues from each other
- Simplifies object protocols
- Centralizes control
- Individual colleagues become simpler and easier to manage

Real-world example: An air traffic control system that mediates communication
between aircraft, preventing direct coordination between planes.
*/
#ifndef C6_MEDIATOR_H
#define C6_MEDIATOR_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace Mediator {

// ----- Forward declarations -----
class Aircraft;

// ----- Mediator interface -----
class ATCMediator {
    public:
    virtual ~ATCMediator () = default;
    virtual void registerAircraft (std::shared_ptr<Aircraft> aircraft) = 0;
    virtual void sendMessage (const std::string& message,
    const std::string& senderName) const                               = 0;
    virtual void requestLanding (std::shared_ptr<Aircraft> aircraft)   = 0;
    virtual void requestTakeoff (std::shared_ptr<Aircraft> aircraft)   = 0;
};

// ----- Colleague -----
class Aircraft : public std::enable_shared_from_this<Aircraft> {
    public:
    Aircraft (const std::string& name, std::shared_ptr<ATCMediator> mediator)
    : name_ (name), mediator_ (mediator), isFlying_ (false), isInAirspace_ (true) {
    }

    std::string getName () const {
        return name_;
    }

    void land () {
        if (isFlying_) {
            mediator_->requestLanding (shared_from_this ());
        } else {
            std::cout << "[" << name_ << "] Already on the ground.\n";
        }
    }

    void takeoff () {
        if (!isFlying_ && isInAirspace_) {
            mediator_->requestTakeoff (shared_from_this ());
        } else if (isFlying_) {
            std::cout << "[" << name_ << "] Already flying.\n";
        } else {
            std::cout << "[" << name_ << "] Not in controlled airspace.\n";
        }
    }

    void sendMessage (const std::string& message) {
        std::cout << "[" << name_ << " sends] " << message << "\n";
        mediator_->sendMessage (message, name_);
    }

    void receiveMessage (const std::string& message) const {
        std::cout << "[" << name_ << " receives] " << message << "\n";
    }

    void setFlying (bool flying) {
        isFlying_ = flying;
        std::cout << "[" << name_ << "] " << (flying ? "Taking off..." : "Landing...\n");
        if (flying) {
            std::cout << "  Now airborne at cruising altitude.\n";
        } else {
            std::cout << "  Touchdown safe. Welcome to " << name_ << ".\n";
        }
    }

    void setInAirspace (bool inAirspace) {
        isInAirspace_ = inAirspace;
    }

    bool isFlying () const {
        return isFlying_;
    }

    private:
    std::string name_;
    std::shared_ptr<ATCMediator> mediator_;
    bool isFlying_;
    bool isInAirspace_;
};

// ----- Concrete Mediator -----
class ControlTower : public ATCMediator {
    public:
    void registerAircraft (std::shared_ptr<Aircraft> aircraft) override {
        aircraft_.push_back (aircraft);
        std::cout << "[ATC] " << aircraft->getName () << " registered in airspace.\n";
    }

    void sendMessage (const std::string& message, const std::string& senderName) const override {
        for (const auto& aircraft : aircraft_) {
            if (aircraft->getName () != senderName) {
                aircraft->receiveMessage (message);
            }
        }
    }

    void requestLanding (std::shared_ptr<Aircraft> aircraft) override {
        std::cout << "[ATC] " << aircraft->getName () << " requests landing.\n";

        // Check runway availability
        if (!runwayAvailable_) {
            std::cout << "[ATC] Runway occupied. " << aircraft->getName ()
                      << ", please hold pattern.\n";
            aircraft->receiveMessage ("Hold pattern - runway occupied");
            return;
        }

        // Check for other aircraft in vicinity
        for (const auto& other : aircraft_) {
            if (other != aircraft && other->isFlying ()) {
                std::cout << "[ATC] Traffic nearby. " << aircraft->getName ()
                          << ", maintain separation.\n";
                aircraft->receiveMessage (
                "Maintain separation - traffic in vicinity");
                return;
            }
        }

        // Clear for landing
        std::cout << "[ATC] " << aircraft->getName () << ", cleared to land runway 27L.\n";
        runwayAvailable_ = false;
        aircraft->setFlying (false);
        runwayAvailable_ = true;
    }

    void requestTakeoff (std::shared_ptr<Aircraft> aircraft) override {
        std::cout << "[ATC] " << aircraft->getName () << " requests takeoff.\n";

        if (!runwayAvailable_) {
            std::cout << "[ATC] Runway occupied. " << aircraft->getName () << ", hold short.\n";
            aircraft->receiveMessage ("Hold short - runway occupied");
            return;
        }

        // Check for other aircraft taking off or landing
        for (const auto& other : aircraft_) {
            if (other != aircraft && other->isFlying ()) {
                std::cout << "[ATC] Departing traffic. " << aircraft->getName ()
                          << ", wait for separation.\n";
                aircraft->receiveMessage (
                "Wait for separation - departing traffic");
                return;
            }
        }

        // Clear for takeoff
        std::cout << "[ATC] " << aircraft->getName () << ", cleared for takeoff runway 09R.\n";
        runwayAvailable_ = false;
        aircraft->setFlying (true);
        runwayAvailable_ = true;
    }

    private:
    std::vector<std::shared_ptr<Aircraft>> aircraft_;
    bool runwayAvailable_ = true;
};

} // namespace Mediator

#endif
