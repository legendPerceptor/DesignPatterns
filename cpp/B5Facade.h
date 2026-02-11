/*
The intent is to provide a unified interface to a set of interfaces in a subsystem.
Facade defines a higher-level interface that makes the subsystem easier to use.

Key benefits:
- Provides a simple interface to a complex subsystem
- Reduces coupling between clients and the subsystem
- Doesn't prevent clients from using the underlying classes if needed

Real-world example: A home theater system with many components (TV, receiver, player,
lights, etc.) that can be controlled through a single simplified interface.
*/
#ifndef B5_FACADE_H
#define B5_FACADE_H

#include <iostream>
#include <memory>
#include <string>

namespace Facade {

// ----- Subsystem classes -----
// Complex subsystem that the facade will simplify

class Amplifier {
    public:
    void on () {
        std::cout << "Amplifier is ON\n";
    }
    void off () {
        std::cout << "Amplifier is OFF\n";
    }
    void setDvd (std::shared_ptr<class DvdPlayer> dvd) {
        std::cout << "Amplifier: DVD player connected\n";
    }
    void setSurroundSound () {
        std::cout
        << "Amplifier: Surround sound enabled (5 speakers, 1 subwoofer)\n";
    }
    void setVolume (int level) {
        std::cout << "Amplifier: volume set to " << level << "\n";
    }
};

class Tuner {
    public:
    void on () {
        std::cout << "Tuner is ON\n";
    }
    void off () {
        std::cout << "Tuner is OFF\n";
    }
    void setAm () {
        std::cout << "Tuner: AM mode set\n";
    }
    void setFm () {
        std::cout << "Tuner: FM mode set\n";
    }
    void setFrequency (float freq) {
        std::cout << "Tuner: frequency set to " << freq << "\n";
    }
};

class DvdPlayer {
    public:
    void on () {
        std::cout << "DVD Player is ON\n";
    }
    void off () {
        std::cout << "DVD Player is OFF\n";
    }
    void eject () {
        std::cout << "DVD Player: ejecting disc\n";
    }
    void play (const std::string& movie) {
        std::cout << "DVD Player: playing \"" << movie << "\"\n";
    }
    void setSurroundAudio (std::shared_ptr<Amplifier> amp) {
        std::cout << "DVD Player: surround audio setup\n";
    }
    void stop () {
        std::cout << "DVD Player: stopped\n";
    }
};

class CdPlayer {
    public:
    void on () {
        std::cout << "CD Player is ON\n";
    }
    void off () {
        std::cout << "CD Player is OFF\n";
    }
    void eject () {
        std::cout << "CD Player: ejecting disc\n";
    }
    void play (const std::string& album) {
        std::cout << "CD Player: playing \"" << album << "\"\n";
    }
    void pause () {
        std::cout << "CD Player: paused\n";
    }
    void stop () {
        std::cout << "CD Player: stopped\n";
    }
};

class Projector {
    public:
    void on () {
        std::cout << "Projector is ON\n";
    }
    void off () {
        std::cout << "Projector is OFF\n";
    }
    void setInput (const std::string& input) {
        std::cout << "Projector: input set to " << input << "\n";
    }
    void wideScreenMode () {
        std::cout << "Projector: widescreen mode (16:9 aspect ratio)\n";
    }
};

class TheaterLights {
    public:
    void on () {
        std::cout << "Theater Lights: ON\n";
    }
    void off () {
        std::cout << "Theater Lights: OFF\n";
    }
    void dim (int level) {
        std::cout << "Theater Lights: dimming to " << level << "%\n";
    }
};

class Screen {
    public:
    void up () {
        std::cout << "Screen: going UP\n";
    }
    void down () {
        std::cout << "Screen: coming DOWN\n";
    }
};

class PopcornPopper {
    public:
    void on () {
        std::cout << "Popcorn Popper: ON\n";
    }
    void off () {
        std::cout << "Popcorn Popper: OFF\n";
    }
    void pop () {
        std::cout << "Popcorn Popper: popping popcorn...\n";
    }
};

// ----- Facade -----
// Provides a simplified interface to the complex home theater subsystem
class HomeTheaterFacade {
    public:
    HomeTheaterFacade (std::shared_ptr<Amplifier> amp,
    std::shared_ptr<Tuner> tuner,
    std::shared_ptr<DvdPlayer> dvd,
    std::shared_ptr<CdPlayer> cd,
    std::shared_ptr<Projector> projector,
    std::shared_ptr<TheaterLights> lights,
    std::shared_ptr<Screen> screen,
    std::shared_ptr<PopcornPopper> popper)
    : amp_ (amp), tuner_ (tuner), dvd_ (dvd), cd_ (cd), projector_ (projector),
      lights_ (lights), screen_ (screen), popper_ (popper) {
    }

    void watchMovie (const std::string& movie) {
        std::cout << "\n=== Get ready to watch a movie ===\n";
        popper_->on ();
        popper_->pop ();
        lights_->dim (10);
        screen_->down ();
        projector_->on ();
        projector_->setInput ("DVD");
        projector_->wideScreenMode ();
        amp_->on ();
        amp_->setDvd (dvd_);
        amp_->setSurroundSound ();
        amp_->setVolume (5);
        dvd_->on ();
        dvd_->play (movie);
    }

    void endMovie () {
        std::cout << "\n=== Shutting movie theater down ===\n";
        popper_->off ();
        lights_->on ();
        screen_->up ();
        projector_->off ();
        amp_->off ();
        dvd_->stop ();
        dvd_->eject ();
        dvd_->off ();
    }

    void listenToCd (const std::string& album) {
        std::cout << "\n=== Getting ready for CD audio ===\n";
        lights_->on ();
        amp_->on ();
        amp_->setVolume (5);
        cd_->on ();
        cd_->play (album);
    }

    void endCd () {
        std::cout << "\n=== Shutting down CD ===\n";
        cd_->stop ();
        cd_->eject ();
        cd_->off ();
        amp_->off ();
    }

    void listenToRadio (float frequency) {
        std::cout << "\n=== Tuning in to the radio ===\n";
        tuner_->on ();
        tuner_->setFm ();
        tuner_->setFrequency (frequency);
        amp_->on ();
        amp_->setVolume (5);
    }

    void endRadio () {
        std::cout << "\n=== Shutting down radio ===\n";
        tuner_->off ();
        amp_->off ();
    }

    private:
    std::shared_ptr<Amplifier> amp_;
    std::shared_ptr<Tuner> tuner_;
    std::shared_ptr<DvdPlayer> dvd_;
    std::shared_ptr<CdPlayer> cd_;
    std::shared_ptr<Projector> projector_;
    std::shared_ptr<TheaterLights> lights_;
    std::shared_ptr<Screen> screen_;
    std::shared_ptr<PopcornPopper> popper_;
};

} // namespace Facade

#endif
