"""
The intent is to define a family of algorithms, encapsulate each one, and make them
interchangeable. Strategy lets the algorithm vary independently from clients that use it.

Key participants:
- Strategy: Declares interface common to all supported algorithms
- ConcreteStrategy: Implements the algorithm using Strategy interface
- Context: Configured with ConcreteStrategy and maintains reference

Key benefits:
- Families of related algorithms defined
- Alternative strategies swapped at runtime
- Eliminates conditional statements

Real-world example: A navigation app with different route calculation strategies.
"""

from abc import ABC, abstractmethod
from typing import Optional


# ----- Payment Strategy Example -----
class PaymentStrategy(ABC):
    @abstractmethod
    def pay(self, amount: float) -> None:
        pass

    @abstractmethod
    def get_name(self) -> str:
        pass


class CreditCardPayment(PaymentStrategy):
    def __init__(self, card_number: str, name: str) -> None:
        self._card_number = card_number
        self._name = name

    def pay(self, amount: float) -> None:
        last_four = self._card_number[-4:]
        print(f"[Credit Card] Paying ${amount} using card ending in {last_four}")
        print(f"[Credit Card] Cardholder: {self._name}")
        print("[Credit Card] Transaction approved.")

    def get_name(self) -> str:
        return "Credit Card"


class PayPalPayment(PaymentStrategy):
    def __init__(self, email: str) -> None:
        self._email = email

    def pay(self, amount: float) -> None:
        print(f"[PayPal] Paying ${amount} using PayPal account")
        print(f"[PayPal] Account: {self._email}")
        print("[PayPal] Payment completed successfully.")

    def get_name(self) -> str:
        return "PayPal"


class ApplePayPayment(PaymentStrategy):
    def __init__(self, device_id: str) -> None:
        self._device_id = device_id

    def pay(self, amount: float) -> None:
        print(f"[Apple Pay] Paying ${amount} using Apple Pay")
        print(f"[Apple Pay] Device ID: {self._device_id}")
        print("[Apple Pay] Payment successful.")

    def get_name(self) -> str:
        return "Apple Pay"


# ----- Context -----
class ShoppingCart:
    def __init__(self) -> None:
        self._payment_strategy: Optional[PaymentStrategy] = None

    def set_payment_strategy(self, strategy: PaymentStrategy) -> None:
        self._payment_strategy = strategy
        print(f"[Shopping Cart] Payment method set to: {strategy.get_name()}")

    def checkout(self, total: float) -> None:
        print(f"\n[Shopping Cart] Starting checkout...")
        print(f"[Shopping Cart] Total amount: ${total}")
        if self._payment_strategy:
            self._payment_strategy.pay(total)
        else:
            print("[Shopping Cart] ERROR: No payment method selected!")
        print("[Shopping Cart] Checkout complete.\n")


# ----- Route Strategy Example -----
class RouteStrategy(ABC):
    @abstractmethod
    def build_route(self, start: str, end: str) -> None:
        pass

    @abstractmethod
    def get_strategy_name(self) -> str:
        pass


class FastestRouteStrategy(RouteStrategy):
    def build_route(self, start: str, end: str) -> None:
        print(f"[Fastest Route] Calculating route from {start} to {end}")
        print("[Fastest Route] Prioritizing highways...")
        print("[Fastest Route] Real-time traffic analysis...")
        print(f"[Fastest Route] Take Highway 101, then I-95 to {end}")
        print("[Fastest Route] Estimated time: 45 minutes")

    def get_strategy_name(self) -> str:
        return "Fastest Route"


class ShortestRouteStrategy(RouteStrategy):
    def build_route(self, start: str, end: str) -> None:
        print(f"[Shortest Route] Calculating route from {start} to {end}")
        print("[Shortest Route] Finding minimum distance...")
        print(f"[Shortest Route] Take local roads to {end}")
        print("[Shortest Route] Distance: 18.5 miles")

    def get_strategy_name(self) -> str:
        return "Shortest Distance"


class AvoidHighwaysStrategy(RouteStrategy):
    def build_route(self, start: str, end: str) -> None:
        print(f"[Avoid Highways] Calculating route from {start} to {end}")
        print("[Avoid Highways] Excluding all highways...")
        print(f"[Avoid Highways] Take Main Street to {end}")
        print("[Avoid Highways] Estimated time: 1 hour 15 minutes")

    def get_strategy_name(self) -> str:
        return "Avoid Highways"


# ----- Context -----
class NavigationSystem:
    def __init__(self, user_name: str) -> None:
        self._user_name = user_name
        self._route_strategy: Optional[RouteStrategy] = None

    def set_route_strategy(self, strategy: RouteStrategy) -> None:
        self._route_strategy = strategy

    def navigate(self, start: str, end: str) -> None:
        print(f"\n[Navigation System] User: {self._user_name}")
        print(f"[Navigation System] Strategy: {self._route_strategy.get_strategy_name() if self._route_strategy else 'None'}")
        print("[Navigation System] Calculating route...\n")
        if self._route_strategy:
            self._route_strategy.build_route(start, end)
            print("\n[Navigation System] Route ready!\n")


# ----- Compression Strategy Example -----
class CompressionStrategy(ABC):
    @abstractmethod
    def compress(self, filename: str) -> None:
        pass

    @abstractmethod
    def get_extension(self) -> str:
        pass


class ZipCompression(CompressionStrategy):
    def compress(self, filename: str) -> None:
        print(f"[ZIP Compression] Compressing {filename}")
        print("[ZIP Compression] Using DEFLATE algorithm...")
        print(f"[ZIP Compression] Archive created: {filename}.zip")

    def get_extension(self) -> str:
        return ".zip"


class RarCompression(CompressionStrategy):
    def compress(self, filename: str) -> None:
        print(f"[RAR Compression] Compressing {filename}")
        print("[RAR Compression] Using RAR algorithm...")
        print(f"[RAR Compression] Archive created: {filename}.rar")

    def get_extension(self) -> str:
        return ".rar"


class SevenZipCompression(CompressionStrategy):
    def compress(self, filename: str) -> None:
        print(f"[7-Zip Compression] Compressing {filename}")
        print("[7-Zip Compression] Using LZMA algorithm...")
        print(f"[7-Zip Compression] Archive created: {filename}.7z")

    def get_extension(self) -> str:
        return ".7z"


# ----- Context -----
class FileCompressor:
    def __init__(self) -> None:
        self._strategy: Optional[CompressionStrategy] = None

    def set_compression_strategy(self, strategy: CompressionStrategy) -> None:
        self._strategy = strategy

    def compress_file(self, filename: str) -> None:
        if self._strategy:
            print(f"\n[File Compressor] Compressing file: {filename}")
            print(f"[File Compressor] Using format: {self._strategy.get_extension()}")
            self._strategy.compress(filename)
            print("[File Compressor] Compression complete.\n")
        else:
            print("[File Compressor] ERROR: No compression strategy set!")


# ----- Example usage -----
def strategy_example() -> None:
    """
    Demonstrates the Strategy pattern with payment, navigation, and compression.
    """
    print("\n--- Strategy Pattern Example ---")

    # Payment Strategy Example
    print("\n1. Payment Processing")
    cart = ShoppingCart()

    cart.set_payment_strategy(CreditCardPayment("1234567890123456", "John Doe"))
    cart.checkout(99.99)

    cart.set_payment_strategy(PayPalPayment("john@example.com"))
    cart.checkout(49.99)

    # Route Strategy Example
    print("\n2. Navigation System")
    nav = NavigationSystem("Alice")

    nav.set_route_strategy(FastestRouteStrategy())
    nav.navigate("New York", "Boston")

    nav.set_route_strategy(ShortestRouteStrategy())
    nav.navigate("New York", "Boston")

    # Compression Strategy Example
    print("\n3. File Compression")
    compressor = FileCompressor()

    compressor.set_compression_strategy(ZipCompression())
    compressor.compress_file("document.txt")

    compressor.set_compression_strategy(SevenZipCompression())
    compressor.compress_file("archive.zip")


# Test helpers
def get_shopping_cart() -> ShoppingCart:
    """Helper for tests: Create a shopping cart."""
    return ShoppingCart()


def get_navigation_system() -> NavigationSystem:
    """Helper for tests: Create a navigation system."""
    return NavigationSystem("Test User")


if __name__ == "__main__":
    strategy_example()
