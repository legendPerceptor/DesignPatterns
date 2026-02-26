"""
The intent is to provide a surrogate or placeholder for another object to control access to it.

Types of proxies:
1. Virtual Proxy - Delays the creation and initialization of expensive objects
2. Protection Proxy - Controls access to an object based on access rights
3. Remote Proxy - Represents an object in a different address space
4. Smart Reference - Adds actions when an object is accessed (e.g., reference counting)

Key benefits:
- Lazy initialization (create objects only when needed)
- Access control
- Hiding complexity
- Performance optimization

Real-world example: Loading large images on demand. Instead of loading all images
when a document opens, use a proxy that only loads the image when it's viewed.
"""

from abc import ABC, abstractmethod
from typing import Optional


# ----- Subject interface -----
# Common interface for RealSubject and Proxy
class Image(ABC):
    @abstractmethod
    def display(self) -> None:
        pass

    @abstractmethod
    def get_filename(self) -> str:
        pass


# ----- RealSubject -----
# The actual object that the proxy represents
class RealImage(Image):
    def __init__(self, filename: str) -> None:
        self._filename = filename
        self._load_image_from_disk()

    def display(self) -> None:
        print(f"Displaying image: {self._filename}")

    def get_filename(self) -> str:
        return self._filename

    def _load_image_from_disk(self) -> None:
        """Simulate expensive loading operation."""
        print(f"Loading {self._filename} from disk...")


# ----- Proxy -----
# Acts as a surrogate, controlling access to the RealImage
class ProxyImage(Image):
    def __init__(self, filename: str) -> None:
        self._filename = filename
        self._real_image: Optional[RealImage] = None

    def display(self) -> None:
        # Lazy initialization: only load the image when first needed
        if self._real_image is None:
            print("[Proxy] Initializing real image on first access...")
            self._real_image = RealImage(self._filename)
        self._real_image.display()

    def get_filename(self) -> str:
        return self._filename


# ----- Protection Proxy Example -----
# Controls access based on user permissions


class Internet(ABC):
    @abstractmethod
    def connect_to(self, server_host: str) -> None:
        pass


class RealInternet:
    def connect_to(self, server_host: str) -> None:
        print(f"Connecting to {server_host}")


class ProxyInternet:
    """Protection proxy that blocks certain websites."""

    def __init__(self) -> None:
        self._internet = RealInternet()
        self._blocked_sites = {"blocked.com", "malware.org", "spam.net"}

    def connect_to(self, server_host: str) -> None:
        # Check if the website is allowed
        if self._try_connect(server_host):
            self._internet.connect_to(server_host)
        else:
            print(f"Access Denied: {server_host} is blocked!")

    def _try_connect(self, server_host: str) -> bool:
        """Check if site is blocked."""
        return not any(blocked in server_host for blocked in self._blocked_sites)

    def add_blocked_site(self, site: str) -> None:
        """Add a site to the blocked list."""
        self._blocked_sites.add(site)


# ----- Example usage -----
def proxy_example() -> None:
    """
    Demonstrates the Proxy pattern with virtual proxy and protection proxy.
    """
    print("\n--- Proxy Pattern Example ---")

    # Example 1: Virtual Proxy - Lazy loading
    print("\n1. Virtual Proxy (Lazy Loading):")
    print("Creating proxy but not loading image yet...")
    proxy_img = ProxyImage("large_photo.jpg")

    print("\nFirst display - should load image:")
    proxy_img.display()

    print("\nSecond display - uses cached image:")
    proxy_img.display()

    # Example 2: Protection Proxy - Access control
    print("\n2. Protection Proxy (Access Control):")
    internet = ProxyInternet()

    print("\nTrying to access allowed site:")
    internet.connect_to("google.com")

    print("\nTrying to access blocked site:")
    internet.connect_to("blocked.com")


# Test helpers
def create_proxy_image(filename: str) -> ProxyImage:
    """Helper for tests: Create a proxy image."""
    return ProxyImage(filename)


def create_proxy_internet() -> ProxyInternet:
    """Helper for tests: Create a proxy internet."""
    return ProxyInternet()


def create_real_image(filename: str) -> RealImage:
    """Helper for tests: Create a real image."""
    return RealImage(filename)


if __name__ == "__main__":
    proxy_example()
