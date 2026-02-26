"""
The intent is to provide a unified interface to a set of interfaces in a subsystem.
Facade defines a higher-level interface that makes the subsystem easier to use.

Key benefits:
- Provides a simple interface to a complex subsystem
- Reduces coupling between clients and the subsystem
- Doesn't prevent clients from using the underlying classes if needed

Real-world example: A home theater system with many components (TV, receiver, player,
lights, etc.) that can be controlled through a single simplified interface.
"""

from dataclasses import dataclass
from typing import Optional


# ----- Subsystem classes -----
# Complex subsystem that the facade will simplify


class Amplifier:
    def on(self) -> None:
        print("Amplifier is ON")

    def off(self) -> None:
        print("Amplifier is OFF")

    def set_dvd(self, dvd: "DvdPlayer") -> None:
        print("Amplifier: DVD player connected")

    def set_surround_sound(self) -> None:
        print("Amplifier: Surround sound enabled (5 speakers, 1 subwoofer)")

    def set_volume(self, level: int) -> None:
        print(f"Amplifier: volume set to {level}")


class Tuner:
    def on(self) -> None:
        print("Tuner is ON")

    def off(self) -> None:
        print("Tuner is OFF")

    def set_am(self) -> None:
        print("Tuner: AM mode set")

    def set_fm(self) -> None:
        print("Tuner: FM mode set")

    def set_frequency(self, freq: float) -> None:
        print(f"Tuner: frequency set to {freq}")


class DvdPlayer:
    def on(self) -> None:
        print("DVD Player is ON")

    def off(self) -> None:
        print("DVD Player is OFF")

    def eject(self) -> None:
        print("DVD Player: ejecting disc")

    def play(self, movie: str) -> None:
        print(f'DVD Player: playing "{movie}"')

    def set_surround_audio(self, amp: Amplifier) -> None:
        print("DVD Player: surround audio setup")

    def stop(self) -> None:
        print("DVD Player: stopped")


class CdPlayer:
    def on(self) -> None:
        print("CD Player is ON")

    def off(self) -> None:
        print("CD Player is OFF")

    def eject(self) -> None:
        print("CD Player: ejecting disc")

    def play(self, album: str) -> None:
        print(f'CD Player: playing "{album}"')

    def pause(self) -> None:
        print("CD Player: paused")

    def stop(self) -> None:
        print("CD Player: stopped")


class Projector:
    def on(self) -> None:
        print("Projector is ON")

    def off(self) -> None:
        print("Projector is OFF")

    def set_input(self, input_source: str) -> None:
        print(f"Projector: input set to {input_source}")

    def wide_screen_mode(self) -> None:
        print("Projector: widescreen mode (16:9 aspect ratio)")


class TheaterLights:
    def on(self) -> None:
        print("Theater Lights: ON")

    def off(self) -> None:
        print("Theater Lights: OFF")

    def dim(self, level: int) -> None:
        print(f"Theater Lights: dimming to {level}%")


class Screen:
    def up(self) -> None:
        print("Screen: going UP")

    def down(self) -> None:
        print("Screen: coming DOWN")


class PopcornPopper:
    def on(self) -> None:
        print("Popcorn Popper: ON")

    def off(self) -> None:
        print("Popcorn Popper: OFF")

    def pop(self) -> None:
        print("Popcorn Popper: popping popcorn...")


# ----- Facade -----
# Provides a simplified interface to the complex home theater subsystem
@dataclass
class HomeTheaterFacade:
    amp: Amplifier
    tuner: Tuner
    dvd: DvdPlayer
    cd: CdPlayer
    projector: Projector
    lights: TheaterLights
    screen: Screen
    popper: PopcornPopper

    def watch_movie(self, movie: str) -> None:
        print("\n=== Get ready to watch a movie ===")
        self.popper.on()
        self.popper.pop()
        self.lights.dim(10)
        self.screen.down()
        self.projector.on()
        self.projector.set_input("DVD")
        self.projector.wide_screen_mode()
        self.amp.on()
        self.amp.set_dvd(self.dvd)
        self.amp.set_surround_sound()
        self.amp.set_volume(5)
        self.dvd.on()
        self.dvd.play(movie)

    def end_movie(self) -> None:
        print("\n=== Shutting movie theater down ===")
        self.popper.off()
        self.lights.on()
        self.screen.up()
        self.projector.off()
        self.amp.off()
        self.dvd.stop()
        self.dvd.eject()
        self.dvd.off()

    def listen_to_cd(self, album: str) -> None:
        print("\n=== Getting ready for CD audio ===")
        self.lights.on()
        self.amp.on()
        self.amp.set_volume(5)
        self.cd.on()
        self.cd.play(album)

    def end_cd(self) -> None:
        print("\n=== Shutting down CD ===")
        self.cd.stop()
        self.cd.eject()
        self.cd.off()
        self.amp.off()

    def listen_to_radio(self, frequency: float) -> None:
        print("\n=== Tuning in to the radio ===")
        self.tuner.on()
        self.tuner.set_fm()
        self.tuner.set_frequency(frequency)
        self.amp.on()
        self.amp.set_volume(5)

    def end_radio(self) -> None:
        print("\n=== Shutting down radio ===")
        self.tuner.off()
        self.amp.off()


# Factory function to create a complete home theater facade
def create_home_theater() -> HomeTheaterFacade:
    """Create a complete home theater facade with all components."""
    amp = Amplifier()
    tuner = Tuner()
    dvd = DvdPlayer()
    cd = CdPlayer()
    projector = Projector()
    lights = TheaterLights()
    screen = Screen()
    popper = PopcornPopper()

    return HomeTheaterFacade(amp, tuner, dvd, cd, projector, lights, screen, popper)


# ----- Example usage -----
def facade_example() -> None:
    """
    Demonstrates the Facade pattern with a home theater system.
    """
    print("\n--- Facade Pattern Example ---")
    print("Setting up Home Theater with Facade:")

    # Create all the subsystem components through the facade
    theater = create_home_theater()

    # Watch a movie with a single method call
    theater.watch_movie("The Matrix")

    # End the movie
    theater.end_movie()

    # Listen to a CD
    theater.listen_to_cd("Pink Floyd - The Wall")
    theater.end_cd()

    # Listen to the radio
    theater.listen_to_radio(101.5)
    theater.end_radio()


# Test helpers
def create_movie_facade() -> HomeTheaterFacade:
    """Helper for tests: Create a facade for movie testing."""
    return create_home_theater()


def create_cd_facade() -> HomeTheaterFacade:
    """Helper for tests: Create a facade for CD testing."""
    return create_home_theater()


if __name__ == "__main__":
    facade_example()
