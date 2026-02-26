"""
The intent is to capture and externalize an object's internal state without
violating encapsulation, so the object can be restored to this state later.

Key participants:
- Memento: Stores internal state of Originator
- Originator: Creates memento and uses it to restore state
- Caretaker: Responsible for memento's safekeeping

Key benefits:
- Preserves encapsulation
- Simplifies Originator
- Provides rollback/undo functionality

Real-world example: A text editor with undo/redo functionality.
"""

from dataclasses import dataclass
from datetime import datetime
from typing import Optional
from collections import deque


# ----- Memento -----
@dataclass
class DocumentMemento:
    content: str
    cursor_position: int
    timestamp: str

    @classmethod
    def create(cls, content: str, cursor: int) -> "DocumentMemento":
        return cls(
            content=content,
            cursor_position=cursor,
            timestamp=datetime.now().strftime("%H:%M:%S")
        )


# ----- Originator -----
class TextDocument:
    def __init__(self) -> None:
        self._content = ""
        self._cursor_position = 0

    def write(self, text: str) -> None:
        self._content += text
        self._cursor_position += len(text)
        print(f"[Document] Wrote: \"{text}\"")
        print(f"  Current content: \"{self._content}\"")

    def delete_last(self, count: int = 1) -> None:
        if len(self._content) >= count:
            self._content = self._content[:-count]
            self._cursor_position = max(0, self._cursor_position - count)
            print(f"[Document] Deleted last {count} character(s)")
            print(f"  Current content: \"{self._content}\"")

    def save(self) -> DocumentMemento:
        print("[Document] Saving state...")
        return DocumentMemento.create(self._content, self._cursor_position)

    def restore(self, memento: DocumentMemento) -> None:
        self._content = memento.content
        self._cursor_position = memento.cursor_position
        print(f"[Document] Restored to state from {memento.timestamp}")
        print(f"  Content: \"{self._content}\"")
        print(f"  Cursor position: {self._cursor_position}")

    @property
    def content(self) -> str:
        return self._content


# ----- Caretaker -----
class History:
    def __init__(self) -> None:
        self._undo_stack: deque[DocumentMemento] = deque(maxlen=50)
        self._redo_stack: deque[DocumentMemento] = deque(maxlen=50)

    def save_state(self, memento: DocumentMemento) -> None:
        self._undo_stack.append(memento)
        self._redo_stack.clear()
        print(f"[History] State saved. Undo stack: {len(self._undo_stack)}\n")

    def undo(self) -> Optional[DocumentMemento]:
        if len(self._undo_stack) <= 1:
            print("[History] Nothing to undo.")
            return None

        current = self._undo_stack.pop()
        self._redo_stack.append(current)

        memento = self._undo_stack[-1]
        print(f"[History] Undo performed. Remaining: {len(self._undo_stack)}")
        return memento

    def redo(self) -> Optional[DocumentMemento]:
        if not self._redo_stack:
            print("[History] Nothing to redo.")
            return None

        memento = self._redo_stack.pop()
        self._undo_stack.append(memento)
        print("[History] Redo performed.")
        return memento

    def set_initial(self, memento: DocumentMemento) -> None:
        self._undo_stack.append(memento)


# ----- Game Character Example -----
@dataclass
class GameCharacterMemento:
    health: int
    x: int
    y: int
    state: str


class GameCharacter:
    def __init__(self, name: str) -> None:
        self._name = name
        self._health = 100
        self._x = 0
        self._y = 0
        self._state = "Normal"

    def take_damage(self, damage: int) -> None:
        self._health = max(0, self._health - damage)
        self._state = "Normal" if self._health > 70 else "Wounded" if self._health > 30 else "Critical"
        print(f"[{self._name}] Took {damage} damage. Health: {self._health} ({self._state})")

    def move(self, dx: int, dy: int) -> None:
        self._x += dx
        self._y += dy
        print(f"[{self._name}] Moved to ({self._x}, {self._y})")

    def heal(self, amount: int) -> None:
        self._health = min(100, self._health + amount)
        self._state = "Normal" if self._health > 70 else "Wounded" if self._health > 30 else "Critical"
        print(f"[{self._name}] Healed {amount}. Health: {self._health} ({self._state})")

    def create_checkpoint(self) -> GameCharacterMemento:
        print(f"[{self._name}] Checkpoint created!")
        return GameCharacterMemento(self._health, self._x, self._y, self._state)

    def restore_from_checkpoint(self, memento: GameCharacterMemento) -> None:
        self._health = memento.health
        self._x = memento.x
        self._y = memento.y
        self._state = memento.state
        print(f"[{self._name}] Restored from checkpoint!")
        print(f"  Health: {self._health}, Position: ({self._x}, {self._y}), State: {self._state}")


# ----- Example usage -----
def memento_example() -> None:
    """
    Demonstrates the Memento pattern with undo/redo and game checkpoints.
    """
    print("\n--- Memento Pattern Example ---")

    # Text Document Example
    print("\n1. Text Editor with Undo/Redo")
    doc = TextDocument()
    history = History()

    history.set_initial(doc.save())

    doc.write("Hello")
    history.save_state(doc.save())

    doc.write(" World")
    history.save_state(doc.save())

    doc.write("!")
    history.save_state(doc.save())

    print("\n--- Undo ---")
    memento = history.undo()
    if memento:
        doc.restore(memento)

    print("\n--- Redo ---")
    memento = history.redo()
    if memento:
        doc.restore(memento)

    # Game Character Example
    print("\n2. Game Character Checkpoints")
    hero = GameCharacter("Hero")

    hero.move(10, 20)
    checkpoint1 = hero.create_checkpoint()

    hero.take_damage(30)
    checkpoint2 = hero.create_checkpoint()

    hero.take_damage(50)
    print("\nRestoring to checkpoint 2...")
    hero.restore_from_checkpoint(checkpoint2)


# Test helpers
def get_document() -> tuple[TextDocument, History]:
    """Helper for tests: Create a document with history."""
    doc = TextDocument()
    history = History()
    history.set_initial(doc.save())
    return doc, history


def get_game_character() -> GameCharacter:
    """Helper for tests: Create a game character."""
    return GameCharacter("Hero")


if __name__ == "__main__":
    memento_example()
