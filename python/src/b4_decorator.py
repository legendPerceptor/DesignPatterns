"""
The intent is to attach additional responsibilities to an object dynamically.
Decorators provide a flexible alternative to subclassing for extending functionality.

Key benefits:
- Add responsibilities to individual objects dynamically without affecting other objects
- Add responsibilities that can be withdrawn later
- Avoid explosion of subclasses for every combination of features

Real-world example: A coffee shop where you can add various toppings (milk, mocha, whip, etc.)
to any type of coffee beverage. Each topping adds its own cost and description.
"""

from abc import ABC, abstractmethod


# ----- Component interface -----
# Defines the interface for objects that can have responsibilities added dynamically
class Beverage(ABC):
    @abstractmethod
    def get_description(self) -> str:
        pass

    @abstractmethod
    def cost(self) -> float:
        pass


# ----- Concrete Components -----
# Basic implementation that decorators can extend
class Espresso(Beverage):
    def get_description(self) -> str:
        return "Espresso"

    def cost(self) -> float:
        return 1.99


class HouseBlend(Beverage):
    def get_description(self) -> str:
        return "House Blend Coffee"

    def cost(self) -> float:
        return 0.89


class DarkRoast(Beverage):
    def get_description(self) -> str:
        return "Dark Roast Coffee"

    def cost(self) -> float:
        return 0.99


# ----- Decorator -----
# Maintains a reference to a Component object and defines an interface
# that conforms to Component's interface
class CondimentDecorator(Beverage):
    def __init__(self, beverage: Beverage) -> None:
        self._beverage = beverage


# ----- Concrete Decorators -----
# Each decorator adds its own behavior before or after delegating to the wrapped object
class Milk(CondimentDecorator):
    def get_description(self) -> str:
        return self._beverage.get_description() + ", Milk"

    def cost(self) -> float:
        return self._beverage.cost() + 0.10


class Mocha(CondimentDecorator):
    def get_description(self) -> str:
        return self._beverage.get_description() + ", Mocha"

    def cost(self) -> float:
        return self._beverage.cost() + 0.20


class Soy(CondimentDecorator):
    def get_description(self) -> str:
        return self._beverage.get_description() + ", Soy"

    def cost(self) -> float:
        return self._beverage.cost() + 0.15


class Whip(CondimentDecorator):
    def get_description(self) -> str:
        return self._beverage.get_description() + ", Whip"

    def cost(self) -> float:
        return self._beverage.cost() + 0.10


# ----- Example usage -----
def decorator_example() -> None:
    """
    Demonstrates the Decorator pattern with a coffee shop ordering system.
    """
    print("\n--- Decorator Pattern Example ---")
    print("Ordering beverages with various toppings:\n")

    # Simple espresso
    print("1. Simple Espresso:")
    beverage1 = Espresso()
    print(f"  {beverage1.get_description()} ${beverage1.cost():.2f}")

    # Dark Roast with double Mocha and Whip
    print("\n2. Dark Roast + Mocha + Mocha + Whip:")
    beverage2 = DarkRoast()
    beverage2 = Mocha(beverage2)
    beverage2 = Mocha(beverage2)
    beverage2 = Whip(beverage2)
    print(f"  {beverage2.get_description()} ${beverage2.cost():.2f}")

    # House Blend with Soy, Mocha, and Whip
    print("\n3. House Blend + Soy + Mocha + Whip:")
    beverage3 = HouseBlend()
    beverage3 = Soy(beverage3)
    beverage3 = Mocha(beverage3)
    beverage3 = Whip(beverage3)
    print(f"  {beverage3.get_description()} ${beverage3.cost():.2f}")


# Test helpers
def create_espresso() -> Beverage:
    """Helper for tests: Create a plain espresso."""
    return Espresso()


def create_dark_roast_with_mocha() -> Beverage:
    """Helper for tests: Create dark roast with mocha."""
    beverage = DarkRoast()
    return Mocha(beverage)


def create_house_blend_with_all() -> Beverage:
    """Helper for tests: Create house blend with soy, mocha, and whip."""
    beverage = HouseBlend()
    beverage = Soy(beverage)
    beverage = Mocha(beverage)
    beverage = Whip(beverage)
    return beverage


def create_double_mocha_dark_roast() -> Beverage:
    """Helper for tests: Create dark roast with double mocha and whip."""
    beverage = DarkRoast()
    beverage = Mocha(beverage)
    beverage = Mocha(beverage)
    beverage = Whip(beverage)
    return beverage


if __name__ == "__main__":
    decorator_example()
