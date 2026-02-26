"""
The intent is to use sharing to support large numbers of fine-grained objects efficiently.
A flyweight is a shared object that can be used in multiple contexts simultaneously.

Key concepts:
- Intrinsic state: Shared, independent of the flyweight's context
- Extrinsic state: Dependent on and varies with the flyweight's context

Key benefits:
- Reduces memory usage by sharing objects
- Allows large numbers of objects when most state can be made extrinsic

Real-world example: A text editor where each character is an object. Instead of creating
a new object for every 'a' in the document, we share a single 'a' flyweight and only
store the position (extrinsic state) for each occurrence.
"""

from abc import ABC, abstractmethod


# ----- Flyweight interface -----
class Character(ABC):
    @abstractmethod
    def display(self, font_size: int, color: str) -> None:
        pass

    @abstractmethod
    def get_char(self) -> str:
        pass


# ----- Concrete Flyweight -----
# Contains intrinsic state (the character itself)
class Letter(Character):
    def __init__(self, letter: str) -> None:
        self._letter = letter  # Intrinsic state - shared

    def display(self, font_size: int, color: str) -> None:
        print(f"Letter '{self._letter}' (size={font_size}, color={color})")

    def get_char(self) -> str:
        return self._letter


# ----- Unshared Concrete Flyweight -----
# Some flyweights may not be shared
class Space(Character):
    def display(self, font_size: int, color: str) -> None:
        print(f"[Space] (size={font_size})")

    def get_char(self) -> str:
        return " "


# ----- Flyweight Factory -----
# Creates and manages flyweight objects, ensures they are shared properly
class CharacterFactory:
    def __init__(self) -> None:
        self._characters: dict[str, Letter] = {}
        self._space: Space | None = None

    def get_character(self, key: str) -> Character:
        """Get or create a flyweight (shared object)."""
        if key not in self._characters:
            character = Letter(key)
            self._characters[key] = character
            print(f"Creating new flyweight for '{key}'")
        return self._characters[key]

    def get_space(self) -> Space:
        if self._space is None:
            self._space = Space()
        return self._space

    def flyweight_count(self) -> int:
        return len(self._characters)


# ----- Context -----
# Contains extrinsic state (varies with context)
class GlyphContext:
    def __init__(self, character: Character, font_size: int, color: str) -> None:
        self._character = character  # Reference to shared flyweight
        self._font_size = font_size  # Extrinsic state
        self._color = color  # Extrinsic state

    def display(self) -> None:
        self._character.display(self._font_size, self._color)


# Example: Document using flyweights for efficient character rendering
class Document:
    def __init__(self, factory: CharacterFactory) -> None:
        self._factory = factory
        self._glyphs: list[GlyphContext] = []

    def add_character(self, char: str, font_size: int, color: str) -> None:
        """Add a character to the document (extrinsic state stored here)."""
        character = self._factory.get_character(char)
        self._glyphs.append(GlyphContext(character, font_size, color))

    def add_space(self, font_size: int) -> None:
        space = self._factory.get_space()
        self._glyphs.append(GlyphContext(space, font_size, "black"))

    def render(self) -> None:
        print(
            f"\nRendering document ({len(self._glyphs)} glyphs, "
            f"{self._factory.flyweight_count()} flyweights used):"
        )
        for glyph in self._glyphs:
            glyph.display()


# ----- Tree Example with Flyweight -----
class TreeType:
    """Shared intrinsic state for trees."""

    def __init__(self, name: str, color: str, texture: str) -> None:
        self._name = name  # Intrinsic state
        self._color = color  # Intrinsic state
        self._texture = texture  # Intrinsic state

    def draw(self, x: int, y: int) -> None:
        print(
            f"Drawing {self._name} tree at ({x},{y}) - "
            f"color: {self._color}, texture: {self._texture}"
        )

    def get_name(self) -> str:
        return self._name


class TreeFactory:
    """Factory for creating and sharing TreeType objects."""

    def __init__(self) -> None:
        self._tree_types: dict[str, TreeType] = {}

    def get_tree_type(self, name: str, color: str, texture: str) -> TreeType:
        key = f"{name}-{color}-{texture}"
        if key not in self._tree_types:
            self._tree_types[key] = TreeType(name, color, texture)
            print(f"Creating new TreeType: {name}")
        return self._tree_types[key]


class Tree:
    """Individual tree with extrinsic state (position)."""

    def __init__(self, x: int, y: int, tree_type: TreeType) -> None:
        self._x = x  # Extrinsic state
        self._y = y  # Extrinsic state
        self._type = tree_type  # Reference to shared intrinsic state

    def draw(self) -> None:
        self._type.draw(self._x, self._y)


class Forest:
    """Forest containing many trees using shared TreeTypes."""

    def __init__(self) -> None:
        self._factory = TreeFactory()
        self._trees: list[Tree] = []

    def plant_tree(self, x: int, y: int, name: str, color: str, texture: str) -> None:
        tree_type = self._factory.get_tree_type(name, color, texture)
        self._trees.append(Tree(x, y, tree_type))

    def draw(self) -> None:
        print("\nDrawing forest:")
        for tree in self._trees:
            tree.draw()


# ----- Example usage -----
def flyweight_example() -> None:
    """
    Demonstrates the Flyweight pattern with character rendering and tree forest.
    """
    print("\n--- Flyweight Pattern Example ---")

    # Example 1: Document with character flyweights
    print("\n1. Document Character Example:")
    factory = CharacterFactory()
    doc = Document(factory)

    # Add characters (reusing 'h', 'e', 'l', 'o' multiple times)
    doc.add_character("h", 12, "black")
    doc.add_character("e", 12, "black")
    doc.add_character("l", 12, "black")
    doc.add_character("l", 12, "black")
    doc.add_character("o", 12, "black")
    doc.add_space(12)
    doc.add_character("w", 12, "black")
    doc.add_character("o", 12, "black")
    doc.add_character("r", 12, "black")
    doc.add_character("l", 12, "black")
    doc.add_character("d", 12, "black")

    doc.render()

    # Example 2: Forest with tree flyweights
    print("\n2. Forest Example:")
    forest = Forest()

    # Plant many trees of same types - only a few TreeTypes are created
    forest.plant_tree(1, 1, "Oak", "Green", "Rough")
    forest.plant_tree(2, 3, "Oak", "Green", "Rough")
    forest.plant_tree(5, 5, "Pine", "Dark Green", "Needle")
    forest.plant_tree(7, 7, "Pine", "Dark Green", "Needle")
    forest.plant_tree(9, 9, "Oak", "Green", "Rough")
    forest.plant_tree(10, 10, "Birch", "White", "Smooth")

    forest.draw()


# Test helpers
def create_character_factory() -> CharacterFactory:
    """Helper for tests: Create a character factory."""
    return CharacterFactory()


def create_document(factory: CharacterFactory) -> Document:
    """Helper for tests: Create a document with factory."""
    return Document(factory)


def create_forest() -> Forest:
    """Helper for tests: Create a forest."""
    return Forest()


if __name__ == "__main__":
    flyweight_example()
