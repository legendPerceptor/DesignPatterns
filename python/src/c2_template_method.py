"""
The intent is to define the skeleton of an algorithm, deferring some steps to subclasses.
Template Method lets subclasses redefine certain steps without changing the algorithm's structure.

Key participants:
- AbstractClass: Defines abstract primitive operations and a template method
- ConcreteClass: Implements the primitive operations

Key benefits:
- Maximizes code reuse
- Common behavior defined once
- Subclasses only implement varying parts

Real-world example: A data processing pipeline with fixed steps but varying processing logic.
"""

from abc import ABC, abstractmethod


# ----- Abstract Class -----
class DataProcessor(ABC):
    def process_data(self, input_data: str) -> None:
        """Template method - defines algorithm structure"""
        print("===== Starting Data Processing =====")
        self._initialize()
        validated = self._validate_input(input_data)
        result = self._perform_core_operation(validated)
        self._finalize(result)
        print("===== Data Processing Complete =====\n")

    @abstractmethod
    def _initialize(self) -> None:
        pass

    @abstractmethod
    def _perform_core_operation(self, data: str) -> str:
        pass

    @abstractmethod
    def _finalize(self, result: str) -> None:
        pass

    def _validate_input(self, data: str) -> str:
        """Hook with default implementation"""
        print("[Default Validation] Checking input...")
        if not data:
            raise ValueError("Input cannot be empty!")
        print("[Default Validation] Passed")
        return data


# ----- Concrete Classes -----
class TextTransformationProcessor(DataProcessor):
    def _initialize(self) -> None:
        print("[TextProcessor] Initializing text transformation engine...")

    def _perform_core_operation(self, data: str) -> str:
        print("[TextProcessor] Transforming to uppercase...")
        return f"*** {data.upper()} ***"

    def _finalize(self, result: str) -> None:
        print(f"[TextProcessor] Result: {result}")

    def _validate_input(self, data: str) -> str:
        print("[TextProcessor] Custom validation...")
        validated = super()._validate_input(data)
        if len(validated) < 3:
            raise ValueError("Text too short!")
        return validated


class NumberCalculationProcessor(DataProcessor):
    def _initialize(self) -> None:
        print("[NumberProcessor] Initializing calculation engine...")

    def _perform_core_operation(self, data: str) -> str:
        print("[NumberProcessor] Parsing numbers...")
        numbers = [int(x) for x in data.split(',') if x.strip().isdigit()]
        total = sum(numbers)
        avg = total / len(numbers) if numbers else 0
        return f"Sum={total}, Avg={avg:.1f}"

    def _finalize(self, result: str) -> None:
        print(f"[NumberProcessor] Result: {result}")


# ----- Caffeine Beverage Example -----
class CaffeineBeverage(ABC):
    def prepare_recipe(self) -> None:
        """Template method"""
        self._boil_water()
        self._brew()
        self._pour_in_cup()
        self._add_condiments()

    def _boil_water(self) -> None:
        print("Boiling water...")

    def _pour_in_cup(self) -> None:
        print("Pouring into cup...")

    @abstractmethod
    def _brew(self) -> None:
        pass

    @abstractmethod
    def _add_condiments(self) -> None:
        pass


class Coffee(CaffeineBeverage):
    def _brew(self) -> None:
        print("Dripping coffee through filter...")

    def _add_condiments(self) -> None:
        print("Adding sugar and milk...")


class Tea(CaffeineBeverage):
    def _brew(self) -> None:
        print("Steeping the tea...")

    def _add_condiments(self) -> None:
        print("Adding lemon...")


# ----- Example usage -----
def template_method_example() -> None:
    """
    Demonstrates the Template Method pattern with multiple examples.
    """
    print("\n--- Template Method Pattern Example ---")

    # Data Processor Example
    print("\n1. Data Processing Pipeline")
    text_processor = TextTransformationProcessor()
    text_processor.process_data("hello world")

    number_processor = NumberCalculationProcessor()
    number_processor.process_data("10,20,30,40")

    # Caffeine Beverage Example
    print("\n2. Caffeine Beverage Preparation")
    coffee = Coffee()
    print("\n   Preparing Coffee:")
    coffee.prepare_recipe()

    tea = Tea()
    print("\n   Preparing Tea:")
    tea.prepare_recipe()


# Test helpers
def get_text_processor() -> DataProcessor:
    """Helper for tests: Create a text transformation processor."""
    return TextTransformationProcessor()


def get_number_processor() -> DataProcessor:
    """Helper for tests: Create a number calculation processor."""
    return NumberCalculationProcessor()


def get_coffee() -> CaffeineBeverage:
    """Helper for tests: Create a coffee beverage."""
    return Coffee()


if __name__ == "__main__":
    template_method_example()
