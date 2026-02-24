/*
The intent is to decouple an abstraction from its implementation so that the two
can vary independently. The Bridge pattern uses composition instead of
inheritance to separate the abstraction (high-level logic) from the
implementation (low-level platform-specific details).

Real-world example: A remote control (abstraction) that can work with different
devices (implementation). The remote control and device can evolve independently
- you can add new remotes or new devices without affecting the other hierarchy.
*/
#ifndef B2_BRIDGE_H
#define B2_BRIDGE_H

#include <iostream>
#include <memory>
#include <string>

namespace Bridge {

// ----- Implementation interface -----
// The low-level interface that provides the actual operations
class Device {
    public:
    virtual ~Device ()                   = default;
    virtual void turnOn ()               = 0;
    virtual void turnOff ()              = 0;
    virtual void setVolume (int percent) = 0;
    virtual int getVolume () const       = 0;
    virtual std::string getName () const = 0;
};

// ----- Concrete Implementations -----
// Different devices that implement the Device interface
class TV : public Device {
    public:
    explicit TV (const std::string& name)
    : name_ (name), volume_ (0), enabled_ (false) {
    }

    void turnOn () override {
        enabled_ = true;
        std::cout << name_ << " TV is ON\n";
    }

    void turnOff () override {
        enabled_ = false;
        std::cout << name_ << " TV is OFF\n";
    }

    void setVolume (int percent) override {
        if (percent < 0)
            volume_ = 0;
        else if (percent > 100)
            volume_ = 100;
        else
            volume_ = percent;
        std::cout << name_ << " TV volume set to " << volume_ << "%\n";
    }

    int getVolume () const override {
        return volume_;
    }

    std::string getName () const override {
        return name_ + " TV";
    }

    private:
    std::string name_;
    int volume_;
    bool enabled_;
};

class Radio : public Device {
    public:
    explicit Radio (const std::string& station)
    : station_ (station), volume_ (0), enabled_ (false) {
    }

    void turnOn () override {
        enabled_ = true;
        std::cout << "Radio (" << station_ << ") is ON\n";
    }

    void turnOff () override {
        enabled_ = false;
        std::cout << "Radio (" << station_ << ") is OFF\n";
    }

    void setVolume (int percent) override {
        if (percent < 0)
            volume_ = 0;
        else if (percent > 100)
            volume_ = 100;
        else
            volume_ = percent;
        std::cout << "Radio (" << station_ << ") volume set to " << volume_ << "%\n";
    }

    int getVolume () const override {
        return volume_;
    }

    std::string getName () const override {
        return "Radio (" + station_ + ")";
    }

    private:
    std::string station_;
    int volume_;
    bool enabled_;
};

// ----- Abstraction -----
// The high-level interface that uses the Device implementation
class RemoteControl {
    public:
    explicit RemoteControl (std::unique_ptr<Device> device)
    : device_ (std::move (device)) {
    }

    virtual ~RemoteControl () = default;

    virtual void togglePower () {
        if (poweredOn_) {
            device_->turnOff ();
            poweredOn_ = false;
        } else {
            device_->turnOn ();
            poweredOn_ = true;
        }
    }

    virtual void volumeUp () {
        int oldVolume = device_->getVolume ();
        device_->setVolume (oldVolume + 10);
    }

    virtual void volumeDown () {
        int oldVolume = device_->getVolume ();
        device_->setVolume (oldVolume - 10);
    }

    virtual void mute () {
        device_->setVolume (0);
    }

    protected:
    std::unique_ptr<Device> device_;
    bool poweredOn_ = false;
};

// ----- Refined Abstraction -----
// Extended remote with additional features
class AdvancedRemoteControl : public RemoteControl {
    public:
    explicit AdvancedRemoteControl (std::unique_ptr<Device> device)
    : RemoteControl (std::move (device)) {
    }

    // Additional feature not in basic remote
    void setChannel (int channel) {
        if (poweredOn_) {
            std::cout << "Channel set to " << channel << " on "
                      << device_->getName () << "\n";
        } else {
            std::cout << "Cannot change channel - device is off\n";
        }
    }

    // Override to add extra behavior
    void volumeUp () override {
        if (poweredOn_) {
            std::cout << "(Advanced) ";
            RemoteControl::volumeUp ();
        } else {
            std::cout << "Cannot change volume - device is off\n";
        }
    }

    void volumeDown () override {
        if (poweredOn_) {
            std::cout << "(Advanced) ";
            RemoteControl::volumeDown ();
        } else {
            std::cout << "Cannot change volume - device is off\n";
        }
    }
};

} // namespace Bridge

#endif
