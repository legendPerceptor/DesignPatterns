"""
The intent is to specify the kinds of objects to create using a prototypical instance,
and create new objects by copying this prototype.

Key participants:
- Prototype: Declares an interface for cloning itself
- ConcretePrototype: Implements an operation for cloning itself
- Client: Creates a new object by asking a prototype to clone itself

Key benefits:
- Adds and removes products at runtime
- Specifies new objects by varying values
- Specifying new objects by varying structure
- Reduced subclassing

Real-world example: A graphic editor where users can clone existing shapes
(circle, rectangle) to create duplicates with the same properties, then modify them.
"""
from abc import ABC, abstractmethod
from copy import deepcopy
from dataclasses import dataclass
from typing import Any, Dict


# ----- Prototype -----
class Prototype(ABC):
    """Abstract Prototype"""

    @abstractmethod
    def clone(self) -> Any:
        """Create and return a copy of this object"""
        pass


# ----- Concrete Prototypes -----
@dataclass
class Circle(Prototype):
    radius: float = 10.0
    color: str = "red"
    x: float = 0.0
    y: float = 0.0

    def draw(self) -> None:
        print(f"Drawing Circle(radius={self.radius}, color={self.color}, "
              f"position=({self.x}, {self.y}))")

    def clone(self) -> "Circle":
        """Deep copy to create an independent clone"""
        return deepcopy(self)


@dataclass
class Rectangle(Prototype):
    width: float = 10.0
    height: float = 5.0
    color: str = "blue"
    x: float = 0.0
    y: float = 0.0

    def draw(self) -> None:
        print(f"Drawing Rectangle(width={self.width}, height={self.height}, "
              f"color={self.color}, position=({self.x}, {self.y}))")

    def clone(self) -> "Rectangle":
        return deepcopy(self)


# ----- Prototype Registry -----
class PrototypeRegistry:
    """
    Manages registered prototypes and can clone them on request.
    This is similar to the C++ version but uses a dictionary.
    """

    def __init__(self) -> None:
        self._prototypes: Dict[str, Prototype] = {}

    def register(self, name: str, prototype: Prototype) -> None:
        """Register a prototype for cloning later"""
        self._prototypes[name] = prototype
        print(f"[Registry] Registered prototype: {name}")

    def unregister(self, name: str) -> None:
        """Remove a prototype from the registry"""
        if name in self._prototypes:
            del self._prototypes[name]
            print(f"[Registry] Unregistered prototype: {name}")

    def clone(self, name: str) -> Prototype:
        """Create a new object by cloning the registered prototype"""
        if name not in self._prototypes:
            raise ValueError(f"Prototype '{name}' not found in registry")

        print(f"[Registry] Cloning prototype: {name}")
        return self._prototypes[name].clone()


# ----- Example usage -----
def prototype_example() -> None:
    """Demonstrates the Prototype pattern with shape cloning"""
    print("\n--- Prototype Pattern: Shape Cloning ---\n")

    registry = PrototypeRegistry()

    # Create and register template shapes
    red_circle = Circle(radius=10.0, color="red")
    blue_rect = Rectangle(width=4.0, height=2.0, color="blue")

    registry.register("red-circle", red_circle)
    registry.register("blue-rect", blue_rect)

    # Clone shapes and modify them
    print("\n1. Cloning red circle:")
    c1 = registry.clone("red-circle")
    c1.draw()

    print("\n2. Cloning another red circle and modifying:")
    c2 = registry.clone("red-circle")
    c2.radius = 20.0
    c2.x = 50.0
    c2.draw()

    print("\n3. Original is unchanged:")
    red_circle.draw()

    print("\n4. Cloning rectangle:")
    r1 = registry.clone("blue-rect")
    r1.draw()


if __name__ == "__main__":
    prototype_example()
