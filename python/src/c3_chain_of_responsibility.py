"""
The intent is to avoid coupling the sender of a request to its receiver by giving
more than one object a chance to handle the request. Chain the receiving objects
and pass the request along the chain until an object handles it.

Key participants:
- Handler: Defines interface for handling requests and successor link
- ConcreteHandler: Handles requests it's responsible for; accesses successor
- Client: Initiates the request

Key benefits:
- Decouples sender from receiver
- Simplifies object interconnections
- Flexible responsibility assignment

Real-world example: A technical support ticket system with escalation levels.
"""

from abc import ABC, abstractmethod
from dataclasses import dataclass
from enum import Enum
from typing import Optional


# ----- Request -----
class Level(Enum):
    BASIC = "Basic"
    INTERMEDIATE = "Intermediate"
    ADVANCED = "Advanced"
    CRITICAL = "Critical"


@dataclass
class SupportTicket:
    description: str
    level: Level

    def get_level_string(self) -> str:
        return self.level.value


# ----- Handler interface -----
class SupportHandler(ABC):
    def __init__(self) -> None:
        self._next: Optional["SupportHandler"] = None

    def set_next(self, handler: "SupportHandler") -> "SupportHandler":
        self._next = handler
        return handler

    def handle_ticket(self, ticket: SupportTicket) -> None:
        if self._can_handle(ticket):
            self._do_handle(ticket)
        elif self._next:
            print(f"{self.__class__.__name__} escalating...")
            self._next.handle_ticket(ticket)
        else:
            print("No handler available for this ticket.")

    @abstractmethod
    def _can_handle(self, ticket: SupportTicket) -> bool:
        pass

    @abstractmethod
    def _do_handle(self, ticket: SupportTicket) -> None:
        pass


# ----- Concrete Handlers -----
class Level1Support(SupportHandler):
    def _can_handle(self, ticket: SupportTicket) -> bool:
        return ticket.level == Level.BASIC

    def _do_handle(self, ticket: SupportTicket) -> None:
        print(f"[Level 1 Support] Handling: {ticket.description}")
        print("  Issue resolved with basic troubleshooting.")


class Level2Support(SupportHandler):
    def _can_handle(self, ticket: SupportTicket) -> bool:
        return ticket.level == Level.INTERMEDIATE

    def _do_handle(self, ticket: SupportTicket) -> None:
        print(f"[Level 2 Support] Handling: {ticket.description}")
        print("  Issue resolved with technical expertise.")


class Manager(SupportHandler):
    def _can_handle(self, ticket: SupportTicket) -> bool:
        return ticket.level == Level.ADVANCED

    def _do_handle(self, ticket: SupportTicket) -> None:
        print(f"[Manager] Handling: {ticket.description}")
        print("  Issue resolved with management authority.")


class Director(SupportHandler):
    def _can_handle(self, ticket: SupportTicket) -> bool:
        return ticket.level == Level.CRITICAL

    def _do_handle(self, ticket: SupportTicket) -> None:
        print(f"[Director] Handling: {ticket.description}")
        print("  Issue resolved at highest level.")


# ----- Example usage -----
def chain_of_responsibility_example() -> None:
    """
    Demonstrates the Chain of Responsibility pattern with a support ticket system.
    """
    print("\n--- Chain of Responsibility Pattern Example ---")

    # Build the chain
    level1 = Level1Support()
    level2 = Level2Support()
    manager = Manager()
    director = Director()

    level1.set_next(level2).set_next(manager).set_next(director)

    # Create tickets
    tickets = [
        SupportTicket("Can't connect to WiFi", Level.BASIC),
        SupportTicket("Server crash", Level.INTERMEDIATE),
        SupportTicket("Datacenter outage", Level.CRITICAL),
        SupportTicket("Budget approval needed", Level.ADVANCED),
    ]

    for ticket in tickets:
        print(f"\nProcessing: {ticket.description} ({ticket.get_level_string()})")
        level1.handle_ticket(ticket)


# Test helpers
def get_support_chain() -> SupportHandler:
    """Helper for tests: Create a complete support chain."""
    level1 = Level1Support()
    level2 = Level2Support()
    manager = Manager()
    return level1.set_next(level2).set_next(manager)


def get_basic_ticket() -> SupportTicket:
    """Helper for tests: Create a basic level ticket."""
    return SupportTicket("Basic issue", Level.BASIC)


if __name__ == "__main__":
    chain_of_responsibility_example()
