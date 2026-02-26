"""
The intent is to compose objects into tree structures to represent part-whole hierarchies.
Composite lets clients treat individual objects and compositions of objects uniformly.

Key participants:
1. Component - declares the interface for objects in the composition
2. Leaf - represents individual objects with no children
3. Composite - represents groups of objects that can have children
4. Client - manipulates objects through the Component interface

Key benefits:
- Treats individual objects and compositions uniformly
- Simplifies client code - no need to distinguish between leaf and composite
- Easy to add new kinds of components

Real-world example: A file system where files and directories should be treated uniformly.
Both can be renamed, moved, or have their size calculated, but directories can contain other items.
"""

from abc import ABC, abstractmethod
from typing import Iterator


# ----- Component interface -----
# Declares the interface for objects in the composition
class FileSystemNode(ABC):
    @abstractmethod
    def print(self, indent: str = "") -> None:
        pass

    @abstractmethod
    def size(self) -> int:
        pass

    @abstractmethod
    def get_name(self) -> str:
        pass

    # Composite-specific operations (default implementation for leaves)
    def add(self, node: "FileSystemNode") -> None:
        print("Cannot add to a file")

    def remove(self, name: str) -> None:
        print("Cannot remove from a file")

    def get_children(self) -> Iterator["FileSystemNode"]:
        return iter([])


# ----- Leaf -----
# Represents individual objects (files) with no children
class File(FileSystemNode):
    def __init__(self, name: str, file_size: int) -> None:
        self._name = name
        self._size = file_size

    def print(self, indent: str = "") -> None:
        print(f"{indent}File: {self._name} ({self._size} bytes)")

    def size(self) -> int:
        return self._size

    def get_name(self) -> str:
        return self._name


# ----- Composite -----
# Represents groups of objects (directories) that can have children
class Directory(FileSystemNode):
    def __init__(self, name: str) -> None:
        self._name = name
        self._children: list[FileSystemNode] = []

    def add(self, node: FileSystemNode) -> None:
        self._children.append(node)

    def remove(self, name: str) -> None:
        self._children = [c for c in self._children if c.get_name() != name]

    def print(self, indent: str = "") -> None:
        print(f"{indent}Directory: {self._name}/ ({self.size()} bytes total)")
        for child in self._children:
            child.print(indent + "  ")

    def size(self) -> int:
        return sum(child.size() for child in self._children)

    def get_name(self) -> str:
        return self._name

    def get_children(self) -> Iterator[FileSystemNode]:
        return iter(self._children)

    def child_count(self) -> int:
        """Return the number of children in this directory."""
        return len(self._children)


# ----- Example usage -----
def composite_example() -> None:
    """
    Demonstrates the Composite pattern with a file system structure.
    """
    print("\n--- Composite Pattern Example ---")
    print("Building a file system structure:\n")

    # Create individual files
    file1 = File("readme.txt", 1024)
    file2 = File("main.cpp", 4096)
    file3 = File("utils.cpp", 2048)
    file4 = File("config.json", 512)

    # Create directories and add files
    src_dir = Directory("src")
    src_dir.add(file2)
    src_dir.add(file3)

    config_dir = Directory("config")
    config_dir.add(file4)

    root_dir = Directory("project")
    root_dir.add(file1)
    root_dir.add(src_dir)
    root_dir.add(config_dir)

    # Print the entire structure (treats all nodes uniformly)
    root_dir.print()

    # Calculate total size (works for both files and directories)
    print(f"\nTotal project size: {root_dir.size()} bytes")


# Test helpers
def create_file_system() -> Directory:
    """Helper for tests: Create a sample file system structure."""
    file1 = File("readme.txt", 1024)
    file2 = File("main.cpp", 4096)
    file3 = File("utils.cpp", 2048)

    src_dir = Directory("src")
    src_dir.add(file2)
    src_dir.add(file3)

    root_dir = Directory("project")
    root_dir.add(file1)
    root_dir.add(src_dir)

    return root_dir


def create_nested_directories() -> Directory:
    """Helper for tests: Create deeply nested directory structure."""
    file1 = File("file1.txt", 100)
    file2 = File("file2.txt", 200)

    inner_dir = Directory("inner")
    inner_dir.add(file2)

    outer_dir = Directory("outer")
    outer_dir.add(file1)
    outer_dir.add(inner_dir)

    return outer_dir


if __name__ == "__main__":
    composite_example()
