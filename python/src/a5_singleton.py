"""
The intent is to ensure a class has only one instance, and provide a global point
of access to it.

Key participants:
- Singleton: Defines a static method that returns a unique instance
- Instance: The single instance of the class

Key benefits:
- Controlled access to sole instance
- Reduced namespace pollution
- Can be subclassed
- More flexible than class operations

Real-world example: A logging service where all parts of an application write
to the same log file through a single Logger instance.
"""
import threading
from typing import Optional


class Singleton:
    """
    Thread-safe Singleton implementation using double-checked locking.

    This is the most Pythonic way to implement a singleton class while
    maintaining lazy initialization and thread safety.
    """
    _instance: Optional["Singleton"] = None
    _lock = threading.Lock()

    def __new__(cls) -> "Singleton":
        """Double-checked locking for thread safety"""
        if cls._instance is None:
            with cls._lock:
                if cls._instance is None:
                    cls._instance = super().__new__(cls)
                    cls._instance._initialized = False
        return cls._instance

    def __init__(self) -> None:
        """Initialize only once"""
        if self._initialized:
            return
        self._initialized = True
        self._log_count = 0
        print("[Singleton] Instance created")

    def log(self, message: str) -> None:
        """Log a message to the single log file"""
        self._log_count += 1
        print(f"[Logger #{self._log_count}] {message}")

    def get_log_count(self) -> int:
        """Return the number of log entries"""
        return self._log_count


# ----- Alternative: Module-level Singleton (more Pythonic) -----
# In Python, a module is naturally a singleton because it's imported only once.
# This approach is simpler and more Pythonic for many use cases.

class _Logger:
    """Private class - users should use the logger_instance at module level"""
    def __init__(self) -> None:
        self._log_count = 0
        print("[Module Singleton] Logger instance created")

    def log(self, message: str) -> None:
        self._log_count += 1
        print(f"[Logger #{self._log_count}] {message}")

    def get_log_count(self) -> int:
        return self._log_count


# Single instance at module level - this IS the singleton
logger_instance = _Logger()


def get_logger() -> _Logger:
    """Accessor function for the singleton logger"""
    return logger_instance


# ----- Example usage -----
def singleton_example() -> None:
    """Demonstrates the Singleton pattern"""
    print("\n--- Singleton Pattern: Logger ---\n")

    print("1. Getting singleton instance multiple times:")
    logger1 = Singleton()
    logger2 = Singleton()
    logger3 = Singleton()

    logger1.log("First message")
    logger2.log("Second message")
    logger3.log("Third message")

    print(f"\n2. Same instance? {logger1 is logger2 is logger3}")
    print(f"   Total logs: {logger1.get_log_count()}")

    print("\n3. Module-level singleton (more Pythonic):")
    log1 = get_logger()
    log2 = get_logger()
    log1.log("Message from module-level singleton")
    log2.log("Another message")
    print(f"   Same instance? {log1 is log2}")


if __name__ == "__main__":
    singleton_example()
