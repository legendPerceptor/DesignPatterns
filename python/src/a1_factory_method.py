"""
The intent is to define an interface for creating an object, but let subclasses
decide which class to instantiate. Factory Method lets a class defer instantiation
to subclasses.

Key participants:
- Product: Defines the interface of objects the factory method creates
- ConcreteProduct: Implements the Product interface
- Creator: Declares the factory method that returns a Product object
- ConcreteCreator: Overrides the factory method to return a ConcreteProduct

Key benefits:
- Eliminates the need to bind application classes to specific classes
- Code only deals with the Product interface, not concrete products
- Easy to extend by creating new subclasses

Real-world example: A document processing application that can create different
types of documents (TextDocument, SpreadsheetDocument) based on the application type.
"""
from abc import ABC, abstractmethod

# ----- Product -----
class Document(ABC):
    """Abstract Product"""
    @abstractmethod
    def create(self) -> None:
        pass

    @abstractmethod
    def get_type(self) -> str:
        pass


# ----- Concrete Products -----
class TextDocument(Document):
    def create(self) -> None:
        print("[TextDocument] Creating new text document...")
        print("[TextDocument] Setting up text editor features...")
        print("[TextDocument] Document ready!")

    def get_type(self) -> str:
        return "Text Document"


class SpreadsheetDocument(Document):
    def create(self) -> None:
        print("[SpreadsheetDocument] Creating new spreadsheet...")
        print("[SpreadsheetDocument] Initializing grid and formulas...")
        print("[SpreadsheetDocument] Document ready!")

    def get_type(self) -> str:
        return "Spreadsheet Document"


# ----- Creator -----
class Application(ABC):
    """Abstract Creator"""

    def new_document(self):
        """Factory method - subclasses override this"""
        doc = self.create_document()
        doc.create()
        print(f"[Application] Created: {doc.get_type()}")
        return doc

    @abstractmethod
    def create_document(self) -> Document:
        pass


# ----- Concrete Creators -----
class TextEditorApp(Application):
    def create_document(self) -> Document:
        return TextDocument()


class SpreadsheetApp(Application):
    def create_document(self) -> Document:
        return SpreadsheetDocument()


# ----- Example usage -----
def factory_method_example(use_spreadsheet: bool = False) -> None:
    """
    Demonstrates the Factory Method pattern.

    Args:
        use_spreadsheet: If True, creates a SpreadsheetApp, otherwise TextEditorApp
    """
    if use_spreadsheet:
        app = SpreadsheetApp()
    else:
        app = TextEditorApp()

    print("\n--- Factory Method: Creating Document ---")
    app.new_document()


if __name__ == "__main__":
    factory_method_example(use_spreadsheet=False)
    print("\n" + "="*50 + "\n")
    factory_method_example(use_spreadsheet=True)
