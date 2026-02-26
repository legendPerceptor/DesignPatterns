"""
Tests for Structural Design Patterns (B1-B7)
"""
import pytest

from src.b1_adapter import adapter_example, AudioPlayer
from src.b2_bridge import bridge_example, RemoteControl, TV, Radio, get_basic_remote_tv
from src.b3_composite import composite_example, Directory, File, create_file_system
from src.b4_decorator import decorator_example, Espresso, DarkRoast, Mocha, Whip, create_double_mocha_dark_roast
from src.b5_facade import facade_example, HomeTheaterFacade, create_home_theater
from src.b6_flyweight import flyweight_example, CharacterFactory, Document, create_character_factory, create_document
from src.b7_proxy import proxy_example, ProxyImage, create_proxy_image


class TestAdapter:
    """Tests for Adapter Pattern"""

    def test_mp3_plays_directly(self, capsys):
        """Test that MP3 plays directly without adapter"""
        player = AudioPlayer()
        player.play("mp3", "song.mp3")
        captured = capsys.readouterr()
        assert "Playing MP3 file: song.mp3" in captured.out

    def test_adapter_converts_mp4(self, capsys):
        """Test that adapter converts MP4 format"""
        player = AudioPlayer()
        player.play("mp4", "movie.mp4")
        captured = capsys.readouterr()
        assert "Playing MP4 file: movie.mp4" in captured.out

    def test_adapter_converts_vlc(self, capsys):
        """Test that adapter converts VLC format"""
        player = AudioPlayer()
        player.play("vlc", "video.vlc")
        captured = capsys.readouterr()
        assert "Playing VLC file: video.vlc" in captured.out

    def test_unsupported_format_error_message(self, capsys):
        """Test that unsupported format shows error message"""
        player = AudioPlayer()
        player.play("avi", "movie.avi")
        captured = capsys.readouterr()
        assert "Invalid media format: avi" in captured.out


class TestBridge:
    """Tests for Bridge Pattern"""

    def test_basic_remote_with_tv(self, capsys):
        """Test basic remote control with TV"""
        tv = TV("Sony")
        remote = RemoteControl(tv)
        remote.toggle_power()
        remote.volume_up()
        remote.toggle_power()
        captured = capsys.readouterr()
        assert "Sony TV is ON" in captured.out
        assert "volume set to" in captured.out
        assert "Sony TV is OFF" in captured.out

    def test_advanced_remote_with_radio(self, capsys):
        """Test advanced remote with radio"""
        radio = Radio("98.5 FM")
        from src.b2_bridge import AdvancedRemoteControl
        remote = AdvancedRemoteControl(radio)
        remote.toggle_power()
        remote.mute()
        captured = capsys.readouterr()
        assert "Radio (98.5 FM) is ON" in captured.out
        assert "volume set to 0%" in captured.out

    def test_volume_clamping(self, capsys):
        """Test that volume is clamped between 0 and 100"""
        tv = TV("TestTV")
        remote = RemoteControl(tv)
        remote.toggle_power()
        remote.volume_up()  # 0 -> 10
        for _ in range(15):  # 10 -> 100, then clamped
            remote.volume_up()
        captured = capsys.readouterr()
        assert "volume set to 100%" in captured.out

    def test_helper_function(self):
        """Test that helper function creates valid objects"""
        remote, tv = get_basic_remote_tv()
        assert isinstance(remote, RemoteControl)
        assert isinstance(tv, TV)


class TestComposite:
    """Tests for Composite Pattern"""

    def test_file_creation(self):
        """Test that files can be created with correct properties"""
        file = File("test.txt", 1024)
        assert file.get_name() == "test.txt"
        assert file.size() == 1024

    def test_directory_can_contain_files(self):
        """Test that directories can contain files"""
        directory = Directory("test_dir")
        file1 = File("file1.txt", 100)
        file2 = File("file2.txt", 200)
        directory.add(file1)
        directory.add(file2)
        assert directory.child_count() == 2

    def test_nested_directory_structure(self):
        """Test that directories can contain other directories"""
        outer = Directory("outer")
        inner = Directory("inner")
        file1 = File("file1.txt", 100)
        outer.add(inner)
        outer.add(file1)
        assert outer.size() == 100
        assert inner.size() == 0

    def test_calculates_total_size(self):
        """Test that size calculation works correctly"""
        fs = create_file_system()
        # readme.txt (1024) + main.cpp (4096) + utils.cpp (2048)
        expected_total = 1024 + 4096 + 2048
        assert fs.size() == expected_total

    def test_helper_function(self):
        """Test that helper function creates valid structure"""
        fs = create_file_system()
        assert isinstance(fs, Directory)
        assert fs.size() > 0


class TestDecorator:
    """Tests for Decorator Pattern"""

    def test_plain_espresso_cost(self):
        """Test that plain espresso has correct cost"""
        espresso = Espresso()
        assert espresso.get_description() == "Espresso"
        assert espresso.cost() == 1.99

    def test_single_mocha_decorator(self):
        """Test that single decorator adds cost correctly"""
        beverage = Mocha(Espresso())
        assert "Mocha" in beverage.get_description()
        assert beverage.cost() == pytest.approx(1.99 + 0.20)

    def test_multiple_decorators(self):
        """Test that multiple decorators accumulate cost"""
        beverage = Whip(Mocha(Mocha(DarkRoast())))
        description = beverage.get_description()
        assert "Dark Roast" in description
        assert "Mocha" in description
        assert "Whip" in description
        assert beverage.cost() == pytest.approx(0.99 + 0.20 + 0.20 + 0.10)

    def test_helper_function(self):
        """Test that helper creates valid decorated beverage"""
        beverage = create_double_mocha_dark_roast()
        assert "Dark Roast" in beverage.get_description()
        assert beverage.get_description().count("Mocha") == 2


class TestFacade:
    """Tests for Facade Pattern"""

    def test_facade_creates_complete_system(self):
        """Test that factory creates all components"""
        facade = create_home_theater()
        assert isinstance(facade, HomeTheaterFacade)
        assert facade.amp is not None
        assert facade.tuner is not None

    def test_watch_movie_complete_flow(self, capsys):
        """Test that watch_movie runs all components"""
        facade = create_home_theater()
        facade.watch_movie("The Matrix")
        captured = capsys.readouterr()
        assert "Popcorn Popper: ON" in captured.out
        assert "Amplifier is ON" in captured.out
        assert "DVD Player: playing" in captured.out

    def test_listen_to_cd_flow(self, capsys):
        """Test that listen_to_cd runs appropriate components"""
        facade = create_home_theater()
        facade.listen_to_cd("Pink Floyd")
        captured = capsys.readouterr()
        assert "CD Player is ON" in captured.out
        assert "volume set to 5" in captured.out

    def test_helper_function(self):
        """Test that helper creates working facade"""
        facade = create_home_theater()
        facade.watch_movie("Test")
        facade.end_movie()


class TestFlyweight:
    """Tests for Flyweight Pattern"""

    def test_character_factory_shares_letters(self):
        """Test that factory shares character objects"""
        factory = CharacterFactory()
        char1 = factory.get_character("a")
        char2 = factory.get_character("a")
        assert char1 is char2

    def test_different_letters_different_objects(self):
        """Test that different letters are different objects"""
        factory = CharacterFactory()
        char_a = factory.get_character("a")
        char_b = factory.get_character("b")
        assert char_a is not char_b

    def test_flyweight_count_increases(self):
        """Test that flyweight count is tracked correctly"""
        factory = CharacterFactory()
        factory.get_character("a")
        factory.get_character("b")
        factory.get_character("a")
        assert factory.flyweight_count() == 2

    def test_document_reuses_flyweights(self):
        """Test that document uses shared flyweights"""
        factory = CharacterFactory()
        doc = Document(factory)
        doc.add_character("a", 12, "black")
        doc.add_character("a", 14, "red")
        doc.add_character("a", 12, "black")
        assert factory.flyweight_count() == 1

    def test_helper_function(self):
        """Test that helper creates working factory"""
        factory = create_character_factory()
        assert isinstance(factory, CharacterFactory)
        doc = create_document(factory)
        assert isinstance(doc, Document)


class TestProxy:
    """Tests for Proxy Pattern"""

    def test_proxy_image_lazy_loading(self, capsys):
        """Test that proxy image only loads on first access"""
        proxy = ProxyImage("large_photo.jpg")
        # First access should load
        proxy.display()
        captured1 = capsys.readouterr()
        assert "Loading large_photo.jpg from disk" in captured1.out

        # Second access should use cached image
        proxy.display()
        captured2 = capsys.readouterr()
        assert "Loading" not in captured2.out

    def test_proxy_gets_filename(self):
        """Test that proxy returns correct filename"""
        proxy = ProxyImage("test.jpg")
        assert proxy.get_filename() == "test.jpg"

    def test_helper_function(self):
        """Test that helper creates valid proxy"""
        img = create_proxy_image("test.jpg")
        assert isinstance(img, ProxyImage)
