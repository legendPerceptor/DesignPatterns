"""
The intent is to provide a way to access elements of an aggregate object sequentially
without exposing its underlying representation.

Key participants:
- Iterator: Defines interface for accessing and traversing elements
- ConcreteIterator: Implements iterator and tracks current position
- Aggregate: Defines interface for creating iterator object
- ConcreteAggregate: Implements iterator creation interface

Key benefits:
- Supports variations in traversal
- Simplifies aggregate interface
- Multiple traversals active concurrently

Real-world example: A music playlist with forward/backward/shuffle traversal.
"""

from abc import ABC, abstractmethod
from dataclasses import dataclass
from typing import TypeVar, Generic, List, Optional

T = TypeVar('T')


# ----- Iterator interface -----
class Iterator(ABC, Generic[T]):
    @abstractmethod
    def has_next(self) -> bool:
        pass

    @abstractmethod
    def next(self) -> T:
        pass

    @abstractmethod
    def reset(self) -> None:
        pass


# ----- Aggregate interface -----
class Aggregate(ABC, Generic[T]):
    @abstractmethod
    def create_iterator(self) -> Iterator[T]:
        pass

    @abstractmethod
    def add(self, item: T) -> None:
        pass

    @abstractmethod
    def size(self) -> int:
        pass


# ----- Concrete Iterator -----
class VectorIterator(Iterator[T]):
    def __init__(self, items: List[T]) -> None:
        self._items = items
        self._index = 0

    def has_next(self) -> bool:
        return self._index < len(self._items)

    def next(self) -> T:
        if not self.has_next():
            raise StopIteration("No more elements")
        item = self._items[self._index]
        self._index += 1
        return item

    def reset(self) -> None:
        self._index = 0


# ----- Reverse Iterator -----
class ReverseVectorIterator(Iterator[T]):
    def __init__(self, items: List[T]) -> None:
        self._items = items
        self._index = len(items) - 1 if items else -1

    def has_next(self) -> bool:
        return self._index >= 0

    def next(self) -> T:
        if not self.has_next():
            raise StopIteration("No more elements")
        item = self._items[self._index]
        self._index -= 1
        return item

    def reset(self) -> None:
        self._index = len(self._items) - 1 if self._items else -1


# ----- Concrete Aggregate -----
class Playlist(Aggregate[T]):
    def __init__(self) -> None:
        self._songs: List[T] = []

    def create_iterator(self) -> Iterator[T]:
        return VectorIterator(self._songs)

    def create_reverse_iterator(self) -> Iterator[T]:
        return ReverseVectorIterator(self._songs)

    def add(self, item: T) -> None:
        self._songs.append(item)

    def size(self) -> int:
        return len(self._songs)


# ----- Song class -----
@dataclass
class Song:
    title: str
    artist: str

    def __str__(self) -> str:
        return f'"{self.title}" by {self.artist}'


# ----- Menu Example -----
@dataclass
class MenuItem:
    name: str
    description: str
    price: float


class MenuIterator(Iterator[MenuItem]):
    def __init__(self, items: List[MenuItem]) -> None:
        self._items = items
        self._index = 0

    def has_next(self) -> bool:
        return self._index < len(self._items)

    def next(self) -> MenuItem:
        if not self.has_next():
            raise StopIteration("No more items")
        item = self._items[self._index]
        self._index += 1
        return item

    def reset(self) -> None:
        self._index = 0


class PancakeHouseMenu:
    def __init__(self) -> None:
        self._menu_items: List[MenuItem] = [
            MenuItem("K&B's Pancake Breakfast", "Pancakes with scrambled eggs", 2.99),
            MenuItem("Regular Pancake Breakfast", "Pancakes with fried eggs", 2.99),
            MenuItem("Blueberry Pancakes", "Pancakes with fresh blueberries", 3.49),
        ]

    def create_iterator(self) -> Iterator[MenuItem]:
        return MenuIterator(self._menu_items)


# ----- Example usage -----
def iterator_example() -> None:
    """
    Demonstrates the Iterator pattern with playlists and menus.
    """
    print("\n--- Iterator Pattern Example ---")

    # Playlist example
    print("\n1. Music Playlist")
    playlist = Playlist[Song]()
    playlist.add(Song("Bohemian Rhapsody", "Queen"))
    playlist.add(Song("Stairway to Heaven", "Led Zeppelin"))
    playlist.add(Song("Hotel California", "Eagles"))

    print("   Forward iteration:")
    iterator = playlist.create_iterator()
    while iterator.has_next():
        print(f"     {iterator.next()}")

    print("\n   Reverse iteration:")
    reverse_iter = playlist.create_reverse_iterator()
    while reverse_iter.has_next():
        print(f"     {reverse_iter.next()}")

    # Menu example
    print("\n2. Restaurant Menu")
    menu = PancakeHouseMenu()
    menu_iter = menu.create_iterator()

    print("   Breakfast Menu:")
    while menu_iter.has_next():
        item = menu_iter.next()
        print(f"     {item.name} - ${item.price:.2f}")


# Test helpers
def get_playlist() -> Playlist[Song]:
    """Helper for tests: Create a playlist with songs."""
    playlist = Playlist[Song]()
    playlist.add(Song("Song 1", "Artist 1"))
    playlist.add(Song("Song 2", "Artist 2"))
    return playlist


def get_menu() -> PancakeHouseMenu:
    """Helper for tests: Create a pancake house menu."""
    return PancakeHouseMenu()


if __name__ == "__main__":
    iterator_example()
