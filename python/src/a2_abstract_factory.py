"""
The intent is to provide an interface for creating families of related or dependent
objects without specifying their concrete classes.

Key participants:
- AbstractFactory: Declares an interface for creating each abstract product
- ConcreteFactory: Implements operations to create concrete products
- AbstractProduct: Declares an interface for a type of product
- ConcreteProduct: Defines a product object to be created by the corresponding factory
- Client: Uses only interfaces declared by AbstractFactory and AbstractProduct

Key benefits:
- Isolates concrete classes
- Makes exchanging product families easy
- Promotes consistency among products

Real-world example: A GUI framework that creates widgets (buttons, checkboxes)
for different operating systems (Windows, Mac) ensuring all widgets match the OS style.
"""
from abc import ABC, abstractmethod
from typing import Protocol


# ----- Abstract Products -----
class Button(Protocol):
    def paint(self) -> str:
        ...


class Checkbox(Protocol):
    def paint(self) -> str:
        ...


# ----- Concrete Products (Windows) -----
class WindowsButton:
    def paint(self) -> str:
        return "Windows Button: rectangular, gray background"


class WindowsCheckbox:
    def paint(self) -> str:
        return "Windows Checkbox: square with 'X' mark"


# ----- Concrete Products (Mac) -----
class MacButton:
    def paint(self) -> str:
        return "Mac Button: rounded corners, blue accent"


class MacCheckbox:
    def paint(self) -> str:
        return "Mac Checkbox: circular with checkmark"


# ----- Abstract Factory -----
class GUIFactory(ABC):
    @abstractmethod
    def create_button(self) -> Button:
        pass

    @abstractmethod
    def create_checkbox(self) -> Checkbox:
        pass


# ----- Concrete Factories -----
class WinFactory(GUIFactory):
    def create_button(self) -> Button:
        return WindowsButton()

    def create_checkbox(self) -> Checkbox:
        return WindowsCheckbox()


class MacFactory(GUIFactory):
    def create_button(self) -> Button:
        return MacButton()

    def create_checkbox(self) -> Checkbox:
        return MacCheckbox()


# ----- Client -----
class Application:
    """
    The Client works only with AbstractFactory and AbstractProduct interfaces.
    This allows it to work with any concrete factory/product family.
    """
    def __init__(self, factory: GUIFactory):
        self.factory = factory

    def create_ui(self) -> None:
        button = self.factory.create_button()
        checkbox = self.factory.create_checkbox()

        print(f"  {button.paint()}")
        print(f"  {checkbox.paint()}")

    def render(self) -> None:
        print("\n--- Rendering UI ---")
        self.create_ui()


# ----- Example usage -----
def abstract_factory_example(is_windows: bool = False) -> None:
    """
    Demonstrates the Abstract Factory pattern.

    Args:
        is_windows: If True, uses Windows factory, otherwise Mac factory
    """
    if is_windows:
        factory = WinFactory()
        os_name = "Windows"
    else:
        factory = MacFactory()
        os_name = "Mac"

    print(f"\n--- Abstract Factory: {os_name} GUI ---")
    app = Application(factory)
    app.render()


if __name__ == "__main__":
    abstract_factory_example(is_windows=False)
    print("\n" + "="*50 + "\n")
    abstract_factory_example(is_windows=True)
