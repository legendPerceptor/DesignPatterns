"""
Tests for Creational Design Patterns (A1-A5)
"""
import pytest

from src.a1_factory_method import factory_method_example, TextEditorApp, SpreadsheetApp
from src.a2_abstract_factory import abstract_factory_example, WinFactory, MacFactory
from src.a3_builder import ConcreteHttpRequestBuilder, HttpRequest
from src.a4_prototype import prototype_example, PrototypeRegistry, Circle, Rectangle
from src.a5_singleton import Singleton, get_logger


class TestFactoryMethod:
    """Tests for Factory Method Pattern"""

    def test_text_editor_app_creates_text_document(self, capsys):
        """Test that TextEditorApp creates a TextDocument"""
        app = TextEditorApp()
        app.new_document()
        captured = capsys.readouterr()
        assert "TextDocument" in captured.out

    def test_spreadsheet_app_creates_spreadsheet_document(self, capsys):
        """Test that SpreadsheetApp creates a SpreadsheetDocument"""
        app = SpreadsheetApp()
        app.new_document()
        captured = capsys.readouterr()
        assert "SpreadsheetDocument" in captured.out


class TestAbstractFactory:
    """Tests for Abstract Factory Pattern"""

    def test_win_factory_creates_windows_widgets(self, capsys):
        """Test that WinFactory creates Windows-style widgets"""
        factory = WinFactory()
        button = factory.create_button()
        checkbox = factory.create_checkbox()
        print(button.paint())
        print(checkbox.paint())
        captured = capsys.readouterr()
        assert "Windows" in captured.out

    def test_mac_factory_creates_mac_widgets(self, capsys):
        """Test that MacFactory creates Mac-style widgets"""
        factory = MacFactory()
        button = factory.create_button()
        checkbox = factory.create_checkbox()
        print(button.paint())
        print(checkbox.paint())
        captured = capsys.readouterr()
        assert "Mac" in captured.out


class TestBuilder:
    """Tests for Builder Pattern"""

    def test_builder_creates_http_request(self):
        """Test that HttpRequestBuilder creates valid requests"""
        request = (ConcreteHttpRequestBuilder()
                   .set_method("POST")
                   .set_url("https://api.example.com")
                   .set_timeout(5000)
                   .build())

        assert request.method == "POST"
        assert request.url == "https://api.example.com"
        assert request.timeout == 5000

    def test_builder_with_headers(self):
        """Test that headers are added correctly"""
        request = (ConcreteHttpRequestBuilder()
                   .set_url("https://api.example.com")
                   .add_header("Authorization", "Bearer token")
                   .build())

        assert request.headers["Authorization"] == "Bearer token"

    def test_builder_requires_url(self):
        """Test that builder raises error if URL is missing"""
        builder = ConcreteHttpRequestBuilder()
        with pytest.raises(ValueError, match="URL is required"):
            builder.build()


class TestPrototype:
    """Tests for Prototype Pattern"""

    def test_registry_registers_prototypes(self):
        """Test that registry can register prototypes"""
        registry = PrototypeRegistry()
        circle = Circle(radius=10.0, color="red")
        registry.register("test-circle", circle)
        # Should not raise

    def test_registry_clones_prototypes(self):
        """Test that registry creates clones"""
        registry = PrototypeRegistry()
        original = Circle(radius=10.0, color="red")
        registry.register("test-circle", original)

        clone: Circle = registry.clone("test-circle")  # type: ignore

        assert clone.radius == original.radius
        assert clone.color == original.color
        assert clone is not original

    def test_clone_is_independent(self):
        """Test that modifying a clone doesn't affect the original"""
        registry = PrototypeRegistry()
        original = Circle(radius=10.0, color="red")
        registry.register("test-circle", original)

        clone: Circle = registry.clone("test-circle")  # type: ignore
        clone.radius = 20.0
        clone.color = "blue"

        assert original.radius == 10.0
        assert original.color == "red"

    def test_clone_nonexistent_prototype_raises_error(self):
        """Test that cloning nonexistent prototype raises ValueError"""
        registry = PrototypeRegistry()
        with pytest.raises(ValueError, match="not found"):
            registry.clone("nonexistent")


class TestSingleton:
    """Tests for Singleton Pattern"""

    def test_singleton_returns_same_instance(self):
        """Test that Singleton returns the same instance every time"""
        instance1 = Singleton()
        instance2 = Singleton()
        instance3 = Singleton()

        assert instance1 is instance2
        assert instance2 is instance3

    def test_singleton_logs_messages(self):
        """Test that singleton can log messages"""
        logger = Singleton()
        logger.log("Test message 1")
        logger.log("Test message 2")

        assert logger.get_log_count() == 2

    def test_module_level_singleton(self):
        """Test that module-level singleton works"""
        log1 = get_logger()
        log2 = get_logger()

        assert log1 is log2

    def test_singleton_is_thread_safe(self):
        """Test that singleton is thread-safe"""
        import threading

        instances = []

        def get_instance():
            instances.append(Singleton())

        threads = [threading.Thread(target=get_instance) for _ in range(10)]
        for t in threads:
            t.start()
        for t in threads:
            t.join()

        # All threads should get the same instance
        first = instances[0]
        assert all(inst is first for inst in instances)
