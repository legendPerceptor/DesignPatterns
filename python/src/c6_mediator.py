"""
The intent is to define an object that encapsulates how a set of objects interact.
Mediator promotes loose coupling by keeping objects from referring to each other explicitly.

Key participants:
- Mediator: Defines interface for communication with Colleagues
- ConcreteMediator: Implements cooperative behavior
- Colleague: Knows its Mediator and communicates with it

Key benefits:
- Decouples colleagues
- Simplifies object protocols
- Centralizes control

Real-world example: Air traffic control system mediating aircraft communication.
"""

from abc import ABC, abstractmethod
from typing import List


# ----- Mediator interface -----
class ATCMediator(ABC):
    @abstractmethod
    def register_aircraft(self, aircraft: "Aircraft") -> None:
        pass

    @abstractmethod
    def send_message(self, message: str, sender_name: str) -> None:
        pass

    @abstractmethod
    def request_landing(self, aircraft: "Aircraft") -> None:
        pass

    @abstractmethod
    def request_takeoff(self, aircraft: "Aircraft") -> None:
        pass


# ----- Colleague -----
class Aircraft:
    def __init__(self, name: str, mediator: ATCMediator) -> None:
        self._name = name
        self._mediator = mediator
        self._is_flying = False
        self._in_airspace = True

    @property
    def name(self) -> str:
        return self._name

    def land(self) -> None:
        if self._is_flying:
            self._mediator.request_landing(self)
        else:
            print(f"[{self._name}] Already on the ground.")

    def takeoff(self) -> None:
        if not self._is_flying and self._in_airspace:
            self._mediator.request_takeoff(self)
        else:
            print(f"[{self._name}] Cannot take off.")

    def send_message(self, message: str) -> None:
        print(f"[{self._name} sends] {message}")
        self._mediator.send_message(message, self._name)

    def receive_message(self, message: str) -> None:
        print(f"[{self._name} receives] {message}")

    def set_flying(self, flying: bool) -> None:
        self._is_flying = flying
        status = "Taking off..." if flying else "Landing..."
        print(f"[{self._name}] {status}")

    @property
    def is_flying(self) -> bool:
        return self._is_flying


# ----- Concrete Mediator -----
class ControlTower(ATCMediator):
    def __init__(self) -> None:
        self._aircraft: List[Aircraft] = []
        self._runway_available = True

    def register_aircraft(self, aircraft: Aircraft) -> None:
        self._aircraft.append(aircraft)
        print(f"[ATC] {aircraft.name} registered in airspace.")

    def send_message(self, message: str, sender_name: str) -> None:
        for aircraft in self._aircraft:
            if aircraft.name != sender_name:
                aircraft.receive_message(message)

    def request_landing(self, aircraft: Aircraft) -> None:
        print(f"[ATC] {aircraft.name} requests landing.")

        if not self._runway_available:
            print(f"[ATC] Runway occupied. {aircraft.name}, please hold.")
            return

        for other in self._aircraft:
            if other != aircraft and other.is_flying:
                print(f"[ATC] Traffic nearby. {aircraft.name}, maintain separation.")
                return

        print(f"[ATC] {aircraft.name}, cleared to land runway 27L.")
        self._runway_available = False
        aircraft.set_flying(False)
        self._runway_available = True

    def request_takeoff(self, aircraft: Aircraft) -> None:
        print(f"[ATC] {aircraft.name} requests takeoff.")

        if not self._runway_available:
            print(f"[ATC] Runway occupied. {aircraft.name}, hold short.")
            return

        for other in self._aircraft:
            if other != aircraft and other.is_flying:
                print(f"[ATC] Departing traffic. {aircraft.name}, wait.")
                return

        print(f"[ATC] {aircraft.name}, cleared for takeoff runway 09R.")
        self._runway_available = False
        aircraft.set_flying(True)
        self._runway_available = True


# ----- Example usage -----
def mediator_example() -> None:
    """
    Demonstrates the Mediator pattern with an air traffic control system.
    """
    print("\n--- Mediator Pattern Example ---")

    tower = ControlTower()

    plane1 = Aircraft("Flight 123", tower)
    plane2 = Aircraft("Flight 456", tower)
    plane3 = Aircraft("Flight 789", tower)

    tower.register_aircraft(plane1)
    tower.register_aircraft(plane2)
    tower.register_aircraft(plane3)

    print("\n--- Communication Test ---")
    plane1.send_message("Good morning from Flight 123")

    print("\n--- Landing Test ---")
    plane1.takeoff()
    plane1.land()


# Test helpers
def get_control_tower() -> ControlTower:
    """Helper for tests: Create a control tower."""
    return ControlTower()


def get_aircraft_with_tower() -> tuple[Aircraft, ControlTower]:
    """Helper for tests: Create an aircraft registered with tower."""
    tower = ControlTower()
    aircraft = Aircraft("Test Flight", tower)
    tower.register_aircraft(aircraft)
    return aircraft, tower


if __name__ == "__main__":
    mediator_example()
