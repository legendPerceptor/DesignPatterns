"""
The intent is to decouple an abstraction from its implementation so that the two
can vary independently. The Bridge pattern uses composition instead of
inheritance to separate the abstraction (high-level logic) from the
implementation (low-level platform-specific details).

Key benefits:
- Abstraction and implementation can vary independently
- Hides implementation details from the client
- Reduces the number of subclasses needed

Real-world example: A remote control (abstraction) that can work with different
devices (implementation). The remote control and device can evolve independently.
"""

from abc import ABC, abstractmethod


# ----- Implementation interface -----
# The low-level interface that provides the actual operations
class Device(ABC):
    @abstractmethod
    def turn_on(self) -> None:
        pass

    @abstractmethod
    def turn_off(self) -> None:
        pass

    @abstractmethod
    def set_volume(self, percent: int) -> None:
        pass

    @abstractmethod
    def get_volume(self) -> int:
        pass

    @abstractmethod
    def get_name(self) -> str:
        pass


# ----- Concrete Implementations -----
# Different devices that implement the Device interface
class TV(Device):
    def __init__(self, name: str) -> None:
        self._name = name
        self._volume = 0
        self._enabled = False

    def turn_on(self) -> None:
        self._enabled = True
        print(f"{self._name} TV is ON")

    def turn_off(self) -> None:
        self._enabled = False
        print(f"{self._name} TV is OFF")

    def set_volume(self, percent: int) -> None:
        self._volume = max(0, min(100, percent))
        print(f"{self._name} TV volume set to {self._volume}%")

    def get_volume(self) -> int:
        return self._volume

    def get_name(self) -> str:
        return f"{self._name} TV"


class Radio(Device):
    def __init__(self, station: str) -> None:
        self._station = station
        self._volume = 0
        self._enabled = False

    def turn_on(self) -> None:
        self._enabled = True
        print(f"Radio ({self._station}) is ON")

    def turn_off(self) -> None:
        self._enabled = False
        print(f"Radio ({self._station}) is OFF")

    def set_volume(self, percent: int) -> None:
        self._volume = max(0, min(100, percent))
        print(f"Radio ({self._station}) volume set to {self._volume}%")

    def get_volume(self) -> int:
        return self._volume

    def get_name(self) -> str:
        return f"Radio ({self._station})"


# ----- Abstraction -----
# The high-level interface that uses the Device implementation
class RemoteControl:
    def __init__(self, device: Device) -> None:
        self._device = device
        self._powered_on = False

    def toggle_power(self) -> None:
        if self._powered_on:
            self._device.turn_off()
            self._powered_on = False
        else:
            self._device.turn_on()
            self._powered_on = True

    def volume_up(self) -> None:
        old_volume = self._device.get_volume()
        self._device.set_volume(old_volume + 10)

    def volume_down(self) -> None:
        old_volume = self._device.get_volume()
        self._device.set_volume(old_volume - 10)

    def mute(self) -> None:
        self._device.set_volume(0)


# ----- Refined Abstraction -----
# Extended remote with additional features
class AdvancedRemoteControl(RemoteControl):
    def set_channel(self, channel: int) -> None:
        if self._powered_on:
            print(f"Channel set to {channel} on {self._device.get_name()}")
        else:
            print("Cannot change channel - device is off")

    def volume_up(self) -> None:
        if self._powered_on:
            print("(Advanced) ", end="")
            super().volume_up()
        else:
            print("Cannot change volume - device is off")

    def volume_down(self) -> None:
        if self._powered_on:
            print("(Advanced) ", end="")
            super().volume_down()
        else:
            print("Cannot change volume - device is off")


# ----- Example usage -----
def bridge_example() -> None:
    """
    Demonstrates the Bridge pattern with remote controls and devices.
    """
    print("\n--- Bridge Pattern Example ---")
    print("Testing RemoteControls with different Devices:\n")

    # Test 1: Basic Remote with TV
    print("1. Basic Remote with Sony TV:")
    sony_tv = TV("Sony")
    basic_remote = RemoteControl(sony_tv)
    basic_remote.toggle_power()
    basic_remote.volume_up()
    basic_remote.volume_up()
    basic_remote.toggle_power()

    # Test 2: Advanced Remote with Radio
    print("\n2. Advanced Remote with Radio:")
    radio = Radio("98.5 FM")
    advanced_remote = AdvancedRemoteControl(radio)
    advanced_remote.toggle_power()
    advanced_remote.volume_up()
    advanced_remote.mute()
    advanced_remote.set_channel(5)

    # Test 3: Advanced Remote with different TV
    print("\n3. Advanced Remote with LG TV:")
    lg_tv = TV("LG")
    lg_remote = AdvancedRemoteControl(lg_tv)
    lg_remote.toggle_power()
    lg_remote.volume_up()
    lg_remote.set_channel(42)
    lg_remote.toggle_power()


# Test helpers
def get_basic_remote_tv() -> tuple[RemoteControl, TV]:
    """Helper for tests: Create a basic remote with TV."""
    tv = TV("TestTV")
    return RemoteControl(tv), tv


def get_advanced_remote_tv() -> tuple[AdvancedRemoteControl, TV]:
    """Helper for tests: Create an advanced remote with TV."""
    tv = TV("TestTV")
    return AdvancedRemoteControl(tv), tv


def get_advanced_remote_radio() -> tuple[AdvancedRemoteControl, Radio]:
    """Helper for tests: Create an advanced remote with Radio."""
    radio = Radio("Test FM")
    return AdvancedRemoteControl(radio), radio


if __name__ == "__main__":
    bridge_example()
