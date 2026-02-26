"""
The intent is to allow an object to alter its behavior when its internal state
changes. The object will appear to change its class.

Key participants:
- Context: Defines interface and maintains instance of ConcreteState
- State: Defines interface for encapsulating behavior
- ConcreteState: Implements behavior associated with a state

Key benefits:
- Localizes state-specific behavior
- Makes state transitions explicit
- State objects can be shared

Real-world example: A vending machine with different states (NoCoin, HasCoin, Sold).
"""

from abc import ABC, abstractmethod
from enum import Enum


# ----- State interface -----
class State(ABC):
    @abstractmethod
    def insert_coin(self) -> None:
        pass

    @abstractmethod
    def eject_coin(self) -> None:
        pass

    @abstractmethod
    def press_button(self) -> None:
        pass

    @abstractmethod
    def dispense(self) -> None:
        pass

    @abstractmethod
    def get_name(self) -> str:
        pass


# ----- Context -----
class VendingMachine:
    def __init__(self, item_count: int) -> None:
        self._item_count = item_count
        self._no_coin_state = NoCoinState(self)
        self._has_coin_state = HasCoinState(self)
        self._sold_state = SoldState(self)
        self._out_of_stock_state = OutOfStockState(self)

        self._current_state: State = self._no_coin_state if item_count > 0 else self._out_of_stock_state

    def insert_coin(self) -> None:
        self._current_state.insert_coin()

    def eject_coin(self) -> None:
        self._current_state.eject_coin()

    def press_button(self) -> None:
        self._current_state.press_button()
        self._current_state.dispense()

    def set_state(self, state: State) -> None:
        self._current_state = state

    def release_product(self) -> None:
        print("[Vending Machine] Dispensing product...")
        if self._item_count > 0:
            self._item_count -= 1
            print(f"[Vending Machine] Product dispensed! Remaining: {self._item_count}")

    @property
    def item_count(self) -> int:
        return self._item_count

    @property
    def has_stock(self) -> bool:
        return self._item_count > 0

    @property
    def no_coin_state(self) -> State:
        return self._no_coin_state

    @property
    def has_coin_state(self) -> State:
        return self._has_coin_state

    @property
    def sold_state(self) -> State:
        return self._sold_state

    @property
    def out_of_stock_state(self) -> State:
        return self._out_of_stock_state

    @property
    def state_name(self) -> str:
        return self._current_state.get_name()


# ----- Concrete States -----
class NoCoinState(State):
    def __init__(self, machine: VendingMachine) -> None:
        self._machine = machine

    def insert_coin(self) -> None:
        print("[No Coin State] Coin inserted")
        self._machine.set_state(self._machine.has_coin_state)

    def eject_coin(self) -> None:
        print("[No Coin State] No coin to eject")

    def press_button(self) -> None:
        print("[No Coin State] Please insert coin first")

    def dispense(self) -> None:
        print("[No Coin State] Payment required")

    def get_name(self) -> str:
        return "No Coin"


class HasCoinState(State):
    def __init__(self, machine: VendingMachine) -> None:
        self._machine = machine

    def insert_coin(self) -> None:
        print("[Has Coin State] Coin already inserted")

    def eject_coin(self) -> None:
        print("[Has Coin State] Coin ejected")
        self._machine.set_state(self._machine.no_coin_state)

    def press_button(self) -> None:
        if self._machine.has_stock:
            print("[Has Coin State] Button pressed")
            self._machine.set_state(self._machine.sold_state)
        else:
            print("[Has Coin State] Out of stock")
            self._machine.set_state(self._machine.out_of_stock_state)

    def dispense(self) -> None:
        print("[Has Coin State] No product dispensed")

    def get_name(self) -> str:
        return "Has Coin"


class SoldState(State):
    def __init__(self, machine: VendingMachine) -> None:
        self._machine = machine

    def insert_coin(self) -> None:
        print("[Sold State] Please wait for product")

    def eject_coin(self) -> None:
        print("[Sold State] Already dispensing")

    def press_button(self) -> None:
        print("[Sold State] Already pressed")

    def dispense(self) -> None:
        self._machine.release_product()
        if self._machine.has_stock:
            self._machine.set_state(self._machine.no_coin_state)
        else:
            self._machine.set_state(self._machine.out_of_stock_state)

    def get_name(self) -> str:
        return "Sold"


class OutOfStockState(State):
    def __init__(self, machine: VendingMachine) -> None:
        self._machine = machine

    def insert_coin(self) -> None:
        print("[Out of Stock State] Cannot accept coin - out of stock")

    def eject_coin(self) -> None:
        print("[Out of Stock State] No coin to eject")

    def press_button(self) -> None:
        print("[Out of Stock State] Out of stock")

    def dispense(self) -> None:
        print("[Out of Stock State] No product to dispense")

    def get_name(self) -> str:
        return "Out of Stock"


# ----- Example usage -----
def state_example() -> None:
    """
    Demonstrates the State pattern with a vending machine.
    """
    print("\n--- State Pattern Example ---")

    print("\n--- Vending Machine ---")
    vending_machine = VendingMachine(2)

    print(f"State: {vending_machine.state_name}")
    vending_machine.insert_coin()
    print(f"State: {vending_machine.state_name}")
    vending_machine.press_button()

    print(f"\nState: {vending_machine.state_name}")
    vending_machine.insert_coin()
    print(f"State: {vending_machine.state_name}")
    vending_machine.press_button()

    print(f"\nState: {vending_machine.state_name}")
    vending_machine.insert_coin()
    print(f"State: {vending_machine.state_name}")


# Test helpers
def get_vending_machine() -> VendingMachine:
    """Helper for tests: Create a vending machine."""
    return VendingMachine(5)


def get_empty_vending_machine() -> VendingMachine:
    """Helper for tests: Create an empty vending machine."""
    return VendingMachine(0)


if __name__ == "__main__":
    state_example()
