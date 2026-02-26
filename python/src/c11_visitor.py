"""
The intent is to represent an operation to be performed on the elements of an object
structure. Visitor lets you define a new operation without changing the classes.

Key participants:
- Visitor: Declares visit operation for each element type
- ConcreteVisitor: Implements each operation
- Element: Defines accept operation that takes visitor
- ConcreteElement: Implements accept operation
- ObjectStructure: Can enumerate its elements

Key benefits:
- Makes adding new operations easy
- Gathers related operations
- Adds operations across classes

Real-world example: A tax calculator visiting different item types.
"""

from abc import ABC, abstractmethod
from dataclasses import dataclass
from typing import List


# ----- Visitor interface -----
class Visitor(ABC):
    @abstractmethod
    def visit_book(self, book: "Book") -> None:
        pass

    @abstractmethod
    def visit_fruit(self, fruit: "Fruit") -> None:
        pass

    @abstractmethod
    def visit_electronics(self, electronics: "Electronics") -> None:
        pass

    @abstractmethod
    def get_name(self) -> str:
        pass


# ----- Element interface -----
class Item(ABC):
    @abstractmethod
    def accept(self, visitor: Visitor) -> None:
        pass

    @abstractmethod
    def get_name(self) -> str:
        pass

    @abstractmethod
    def get_price(self) -> float:
        pass


# ----- Concrete Elements -----
@dataclass
class Book(Item):
    title: str
    author: str
    price: float

    def accept(self, visitor: Visitor) -> None:
        visitor.visit_book(self)

    def get_name(self) -> str:
        return f"Book: {self.title}"

    def get_price(self) -> float:
        return self.price


@dataclass
class Fruit(Item):
    name: str
    price_per_kg: float
    weight: float

    def accept(self, visitor: Visitor) -> None:
        visitor.visit_fruit(self)

    def get_name(self) -> str:
        return self.name

    def get_price(self) -> float:
        return self.price_per_kg * self.weight


@dataclass
class Electronics(Item):
    model: str
    price: float
    energy_efficient: bool

    def accept(self, visitor: Visitor) -> None:
        visitor.visit_electronics(self)

    def get_name(self) -> str:
        return f"Electronics: {self.model}"

    def get_price(self) -> float:
        return self.price


# ----- Concrete Visitors -----
class TaxVisitor(Visitor):
    def __init__(self) -> None:
        self._book_tax_rate = 0.05
        self._food_tax_rate = 0.02
        self._electronics_tax_rate = 0.15
        self._eco_discount_rate = 0.03

    def visit_book(self, book: Book) -> None:
        tax = book.get_price() * self._book_tax_rate
        print(f"[TaxVisitor] {book.get_name()} by {book.author}")
        print(f"  Price: ${book.get_price():.2f}")
        print(f"  Tax ({self._book_tax_rate * 100}%): ${tax:.2f}")
        print(f"  Total: ${book.get_price() + tax:.2f}\n")

    def visit_fruit(self, fruit: Fruit) -> None:
        tax = fruit.get_price() * self._food_tax_rate
        print(f"[TaxVisitor] {fruit.get_name()} ({fruit.weight} kg @ ${fruit.price_per_kg}/kg)")
        print(f"  Price: ${fruit.get_price():.2f}")
        print(f"  Tax ({self._food_tax_rate * 100}%): ${tax:.2f}")
        print(f"  Total: ${fruit.get_price() + tax:.2f}\n")

    def visit_electronics(self, electronics: Electronics) -> None:
        tax = electronics.get_price() * self._electronics_tax_rate
        eco_discount = (electronics.get_price() * self._eco_discount_rate
                         if electronics.energy_efficient else 0.0)
        total_tax = tax - eco_discount

        print(f"[TaxVisitor] {electronics.model}")
        print(f"  Price: ${electronics.get_price():.2f}")
        print(f"  Tax ({self._electronics_tax_rate * 100}%): ${tax:.2f}")
        if electronics.energy_efficient:
            print(f"  Eco discount ({self._eco_discount_rate * 100}%): -${eco_discount:.2f}")
        print(f"  Total tax: ${total_tax:.2f}")
        print(f"  Total: ${electronics.get_price() + total_tax:.2f}\n")

    def get_name(self) -> str:
        return "Tax Calculator"


class DiscountVisitor(Visitor):
    def __init__(self) -> None:
        self._book_discount = 0.10
        self._electronics_discount = 0.20

    def visit_book(self, book: Book) -> None:
        discount = book.get_price() * self._book_discount
        print(f"[DiscountVisitor] {book.get_name()}")
        print(f"  Original Price: ${book.get_price():.2f}")
        print(f"  Discount ({self._book_discount * 100}%): -${discount:.2f}")
        print(f"  Sale Price: ${book.get_price() - discount:.2f}\n")

    def visit_fruit(self, fruit: Fruit) -> None:
        print(f"[DiscountVisitor] {fruit.get_name()}")
        print(f"  Price: ${fruit.get_price():.2f}")
        print(f"  Discount: None")
        print(f"  Sale Price: ${fruit.get_price():.2f}\n")

    def visit_electronics(self, electronics: Electronics) -> None:
        discount = electronics.get_price() * self._electronics_discount
        print(f"[DiscountVisitor] {electronics.model}")
        print(f"  Original Price: ${electronics.get_price():.2f}")
        print(f"  Discount ({self._electronics_discount * 100}%): -${discount:.2f}")
        print(f"  Sale Price: ${electronics.get_price() - discount:.2f}\n")

    def get_name(self) -> str:
        return "Discount Calculator"


class InventoryVisitor(Visitor):
    def __init__(self) -> None:
        self._book_count = 0
        self._fruit_count = 0
        self._electronics_count = 0
        self._total_value = 0.0

    def visit_book(self, book: Book) -> None:
        self._book_count += 1
        self._total_value += book.get_price()
        print(f"[InventoryVisitor] Logged book: {book.get_name()}")

    def visit_fruit(self, fruit: Fruit) -> None:
        self._fruit_count += 1
        self._total_value += fruit.get_price()
        print(f"[InventoryVisitor] Logged fruit: {fruit.get_name()} ({fruit.weight}kg)")

    def visit_electronics(self, electronics: Electronics) -> None:
        self._electronics_count += 1
        self._total_value += electronics.get_price()
        print(f"[InventoryVisitor] Logged electronics: {electronics.model}")

    def get_name(self) -> str:
        return "Inventory Counter"

    def print_summary(self) -> None:
        print(f"\n[InventoryVisitor] Summary:")
        print(f"  Books: {self._book_count}")
        print(f"  Fruits: {self._fruit_count}")
        print(f"  Electronics: {self._electronics_count}")
        total = self._book_count + self._fruit_count + self._electronics_count
        print(f"  Total items: {total}")
        print(f"  Total value: ${self._total_value:.2f}\n")


# ----- Object Structure -----
class ShoppingCart:
    def __init__(self) -> None:
        self._items: List[Item] = []

    def add_item(self, item: Item) -> None:
        self._items.append(item)

    def accept(self, visitor: Visitor) -> None:
        print(f"\n[ShoppingCart] Processing cart with visitor: {visitor.get_name()}")
        print(f"[ShoppingCart] Items in cart: {len(self._items)}\n")
        for item in self._items:
            item.accept(visitor)


# ----- Example usage -----
def visitor_example() -> None:
    """
    Demonstrates the Visitor pattern with a shopping cart.
    """
    print("\n--- Visitor Pattern Example ---")

    # Create shopping cart with items
    cart = ShoppingCart()
    cart.add_item(Book("Design Patterns", "Erich Gamma", 49.99))
    cart.add_item(Book("Clean Code", "Robert Martin", 39.99))
    cart.add_item(Fruit("Apple", 2.99, 1.5))
    cart.add_item(Electronics("Laptop Pro", 1299.99, True))

    # Apply different visitors
    print("--- Tax Calculation ---")
    cart.accept(TaxVisitor())

    print("--- Discount Calculation ---")
    cart.accept(DiscountVisitor())

    print("--- Inventory Count ---")
    inventory_visitor = InventoryVisitor()
    cart.accept(inventory_visitor)
    inventory_visitor.print_summary()


# Test helpers
def get_shopping_cart() -> ShoppingCart:
    """Helper for tests: Create a shopping cart with items."""
    cart = ShoppingCart()
    cart.add_item(Book("Test Book", "Test Author", 29.99))
    cart.add_item(Fruit("Orange", 1.99, 2.0))
    return cart


def get_tax_visitor() -> TaxVisitor:
    """Helper for tests: Create a tax visitor."""
    return TaxVisitor()


if __name__ == "__main__":
    visitor_example()
