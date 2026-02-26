"""
The intent is to encapsulate a request as an object, thereby letting you parameterize
clients with different requests, queue or log requests, and support undoable operations.

Key participants:
- Command: Declares interface for executing an operation
- ConcreteCommand: Defines binding between Receiver and action
- Invoker: Asks the command to carry out the request
- Receiver: Knows how to perform the operations

Key benefits:
- Decouples invoker from receiver
- Commands are first-class objects
- Easy to add new commands
- Supports undo/redo

Real-world example: A remote control with undo functionality.
"""

from abc import ABC, abstractmethod
from collections import deque
from dataclasses import dataclass
from enum import Enum
from typing import Optional


# ----- Receivers -----
class Light:
    def on(self) -> None:
        print("Light is ON")

    def off(self) -> None:
        print("Light is OFF")

    def dim(self, level: int) -> None:
        print(f"Light dimmed to {level}%")


class Stereo:
    def on(self) -> None:
        print("Stereo is ON")

    def off(self) -> None:
        print("Stereo is OFF")

    def set_cd(self) -> None:
        print("Stereo: CD player selected")

    def set_volume(self, level: int) -> None:
        print(f"Stereo: volume set to {level}")


class CeilingFan:
    class Speed(Enum):
        OFF = "OFF"
        LOW = "LOW"
        MEDIUM = "MEDIUM"
        HIGH = "HIGH"

    def __init__(self) -> None:
        self._speed = self.Speed.OFF

    def set_speed(self, speed: Speed) -> None:
        self._speed = speed
        print(f"Ceiling Fan: {speed.value} speed")

    def get_speed(self) -> Speed:
        return self._speed


# ----- Command interface -----
class Command(ABC):
    @abstractmethod
    def execute(self) -> None:
        pass

    @abstractmethod
    def undo(self) -> None:
        pass


# ----- Concrete Commands -----
class LightOnCommand(Command):
    def __init__(self, light: Light) -> None:
        self._light = light

    def execute(self) -> None:
        self._light.on()

    def undo(self) -> None:
        self._light.off()


class LightOffCommand(Command):
    def __init__(self, light: Light) -> None:
        self._light = light

    def execute(self) -> None:
        self._light.off()

    def undo(self) -> None:
        self._light.on()


class StereoOnWithCDCommand(Command):
    def __init__(self, stereo: Stereo) -> None:
        self._stereo = stereo

    def execute(self) -> None:
        self._stereo.on()
        self._stereo.set_cd()
        self._stereo.set_volume(11)

    def undo(self) -> None:
        self._stereo.off()


class CeilingFanHighCommand(Command):
    def __init__(self, fan: CeilingFan) -> None:
        self._fan = fan
        self._prev_speed: Optional[CeilingFan.Speed] = None

    def execute(self) -> None:
        self._prev_speed = self._fan.get_speed()
        self._fan.set_speed(CeilingFan.Speed.HIGH)

    def undo(self) -> None:
        if self._prev_speed:
            self._fan.set_speed(self._prev_speed)


class NoCommand(Command):
    def execute(self) -> None:
        print("[No operation]")

    def undo(self) -> None:
        pass


# ----- Invoker -----
class RemoteControl:
    def __init__(self) -> None:
        self._on_commands: list[Command] = [NoCommand()] * 7
        self._off_commands: list[Command] = [NoCommand()] * 7
        self._undo_stack: deque[Command] = deque(maxlen=10)

    def set_command(self, slot: int, on_cmd: Command, off_cmd: Command) -> None:
        self._on_commands[slot] = on_cmd
        self._off_commands[slot] = off_cmd

    def on_button_was_pressed(self, slot: int) -> None:
        print(f"\n[Remote: ON button {slot} pressed]")
        self._on_commands[slot].execute()
        self._undo_stack.append(self._on_commands[slot])

    def off_button_was_pressed(self, slot: int) -> None:
        print(f"\n[Remote: OFF button {slot} pressed]")
        self._off_commands[slot].execute()
        self._undo_stack.append(self._off_commands[slot])

    def undo_button_was_pressed(self) -> None:
        print("\n[Remote: UNDO button pressed]")
        if self._undo_stack:
            cmd = self._undo_stack.pop()
            cmd.undo()
        else:
            print("Nothing to undo.")


# ----- Macro Command -----
class MacroCommand(Command):
    def __init__(self, commands: list[Command]) -> None:
        self._commands = commands

    def execute(self) -> None:
        for cmd in self._commands:
            cmd.execute()

    def undo(self) -> None:
        for cmd in reversed(self._commands):
            cmd.undo()


# ----- Example usage -----
def command_example() -> None:
    """
    Demonstrates the Command pattern with a remote control system.
    """
    print("\n--- Command Pattern Example ---")

    # Create receivers
    living_room_light = Light()
    stereo = Stereo()
    ceiling_fan = CeilingFan()

    # Create commands
    light_on = LightOnCommand(living_room_light)
    light_off = LightOffCommand(living_room_light)
    stereo_on = StereoOnWithCDCommand(stereo)
    fan_high = CeilingFanHighCommand(ceiling_fan)

    # Set up remote
    remote = RemoteControl()
    remote.set_command(0, light_on, light_off)
    remote.set_command(1, stereo_on, LightOffCommand(Light()))
    remote.set_command(2, fan_high, LightOffCommand(Light()))

    # Test
    remote.on_button_was_pressed(0)
    remote.off_button_was_pressed(0)

    remote.on_button_was_pressed(1)
    remote.undo_button_was_pressed()

    # Macro command
    print("\n--- Macro Command ---")
    party_mode = MacroCommand([light_on, stereo_on, fan_high])
    party_mode.execute()
    print("\nUndoing macro...")
    party_mode.undo()


# Test helpers
def get_remote() -> RemoteControl:
    """Helper for tests: Create a remote with basic commands."""
    remote = RemoteControl()
    light = Light()
    remote.set_command(0, LightOnCommand(light), LightOffCommand(light))
    return remote


def get_ceiling_fan_remote() -> tuple[RemoteControl, CeilingFan]:
    """Helper for tests: Create a remote with ceiling fan control."""
    remote = RemoteControl()
    fan = CeilingFan()
    remote.set_command(0, CeilingFanHighCommand(fan), LightOffCommand(Light()))
    return remote, fan


if __name__ == "__main__":
    command_example()
