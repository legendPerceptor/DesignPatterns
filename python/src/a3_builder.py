"""
The intent is to separate the construction of a complex object from its representation,
so that the same construction process can create different representations.

Key participants:
- Builder: Specifies an abstract interface for creating parts of a Product
- ConcreteBuilder: Constructs and assembles parts of the product by implementing Builder
- Director: Constructs an object using the Builder interface
- Product: The object under construction

Key benefits:
- Lets you vary a product's internal representation
- Isolates construction code from representation code
- Gives you finer control over the construction process

Real-world example: Building an HTTP request with various options (method, URL, body,
headers, timeout) step by step.
"""
from abc import ABC, abstractmethod
from dataclasses import dataclass, field
from typing import Optional


# ----- Product -----
@dataclass
class HttpRequest:
    method: str = "GET"
    url: str = ""
    body: str = ""
    headers: dict[str, str] = field(default_factory=dict)
    timeout: int = 5000

    def __str__(self) -> str:
        headers_str = ", ".join(f"{k}: {v}" for k, v in self.headers.items())
        return (f"HttpRequest(method={self.method}, url={self.url}, "
                f"timeout={self.timeout}ms, headers=[{headers_str}])")


# ----- Builder -----
class HttpRequestBuilder(ABC):
    """Abstract Builder"""

    @abstractmethod
    def set_method(self, method: str) -> "HttpRequestBuilder":
        pass

    @abstractmethod
    def set_url(self, url: str) -> "HttpRequestBuilder":
        pass

    @abstractmethod
    def set_body(self, body: str) -> "HttpRequestBuilder":
        pass

    @abstractmethod
    def set_timeout(self, timeout: int) -> "HttpRequestBuilder":
        pass

    @abstractmethod
    def add_header(self, key: str, value: str) -> "HttpRequestBuilder":
        pass

    @abstractmethod
    def build(self) -> HttpRequest:
        pass


# ----- Concrete Builder -----
class ConcreteHttpRequestBuilder(HttpRequestBuilder):
    """Concrete Builder that constructs HTTP requests"""

    def __init__(self) -> None:
        self._request = HttpRequest()

    def set_method(self, method: str) -> "HttpRequestBuilder":
        self._request.method = method
        return self

    def set_url(self, url: str) -> "HttpRequestBuilder":
        self._request.url = url
        return self

    def set_body(self, body: str) -> "HttpRequestBuilder":
        self._request.body = body
        return self

    def set_timeout(self, timeout: int) -> "HttpRequestBuilder":
        self._request.timeout = timeout
        return self

    def add_header(self, key: str, value: str) -> "HttpRequestBuilder":
        self._request.headers[key] = value
        return self

    def build(self) -> HttpRequest:
        # Validate before building
        if not self._request.url:
            raise ValueError("URL is required")
        return self._request


# ----- Director (optional) -----
class HttpRequestDirector:
    """
    Director class that defines the construction process using the Builder.
    This is optional - clients can also use the builder directly.
    """

    def __init__(self, builder: HttpRequestBuilder) -> None:
        self._builder = builder

    def construct_post_request(self, url: str, body: str) -> HttpRequest:
        """Constructs a typical POST request"""
        return (self._builder
                .set_method("POST")
                .set_url(url)
                .set_body(body)
                .add_header("Content-Type", "application/json")
                .set_timeout(5000)
                .build())

    def construct_get_request(self, url: str) -> HttpRequest:
        """Constructs a typical GET request"""
        return (self._builder
                .set_method("GET")
                .set_url(url)
                .set_timeout(3000)
                .build())


# ----- Example usage -----
def builder_example() -> None:
    """Demonstrates the Builder pattern with method chaining"""
    print("\n--- Builder Pattern: HTTP Request ---\n")

    # Example 1: Using builder directly with method chaining
    print("1. Building custom request with method chaining:")
    request = (ConcreteHttpRequestBuilder()
               .set_method("POST")
               .set_url("https://example.com/api")
               .set_body('{"id": 42}')
               .add_header("Authorization", "Bearer token123")
               .set_timeout(5000)
               .build())

    print(f"   {request}")

    # Example 2: Using Director for common patterns
    print("\n2. Using Director for predefined patterns:")
    builder = ConcreteHttpRequestBuilder()
    director = HttpRequestDirector(builder)

    post_request = director.construct_post_request(
        "https://api.example.com/users",
        '{"name": "John", "email": "john@example.com"}'
    )
    print(f"   POST: {post_request}")

    # Reset and build GET request
    get_request = director.construct_get_request("https://api.example.com/users/42")
    print(f"   GET:  {get_request}")


if __name__ == "__main__":
    builder_example()
