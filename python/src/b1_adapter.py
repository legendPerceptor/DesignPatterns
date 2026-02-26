"""
The intent is to convert the interface of a class into another interface clients expect.
Adapter lets classes work together that couldn't otherwise because of incompatible interfaces.

Types of adapters:
1. Class Adapter - uses multiple inheritance (less common in modern Python)
2. Object Adapter - uses composition to wrap the adaptee (shown below)

Key benefits:
- Allows incompatible interfaces to work together
- Promotes reuse of existing functionality
- Decouples client code from the adapted interface

Real-world example: A media player that can play multiple formats by using adapters
to convert incompatible audio formats into a common interface.
"""

from abc import ABC, abstractmethod


# ----- Target interface -----
# The interface that the client expects
class MediaPlayer(ABC):
    @abstractmethod
    def play(self, audio_type: str, file_name: str) -> None:
        pass


# ----- Adaptee -----
# The incompatible interface that needs adapting
class AdvancedMediaPlayer:
    def play_mp4(self, file_name: str) -> None:
        print(f"Playing MP4 file: {file_name}")

    def play_vlc(self, file_name: str) -> None:
        print(f"Playing VLC file: {file_name}")


# ----- Concrete Target -----
# A simple media player that only plays MP3
class Mp3Player(MediaPlayer):
    def play(self, audio_type: str, file_name: str) -> None:
        if audio_type == "mp3":
            print(f"Playing MP3 file: {file_name}")
        else:
            print("Invalid media format. Mp3Player only supports mp3.")


# ----- Adapter -----
# Adapts the AdvancedMediaPlayer interface to the MediaPlayer interface
class MediaAdapter(MediaPlayer):
    def __init__(self, audio_type: str) -> None:
        self._advanced_player: AdvancedMediaPlayer | None = None
        if audio_type in ("mp4", "vlc"):
            self._advanced_player = AdvancedMediaPlayer()

    def play(self, audio_type: str, file_name: str) -> None:
        if audio_type == "mp4" and self._advanced_player:
            self._advanced_player.play_mp4(file_name)
        elif audio_type == "vlc" and self._advanced_player:
            self._advanced_player.play_vlc(file_name)


# ----- Client -----
# Uses the MediaPlayer interface to play various audio formats
class AudioPlayer(MediaPlayer):
    def __init__(self) -> None:
        self._mp3_player = Mp3Player()
        self._media_adapter: MediaAdapter | None = None

    def play(self, audio_type: str, file_name: str) -> None:
        # Built-in support for MP3
        if audio_type == "mp3":
            self._mp3_player.play(audio_type, file_name)
        # Use adapter for other formats
        elif audio_type in ("mp4", "vlc"):
            if not self._media_adapter:
                # Lazy initialization of adapter
                self._media_adapter = MediaAdapter(audio_type)
            self._media_adapter.play(audio_type, file_name)
        else:
            print(f"Invalid media format: {audio_type} (supported: mp3, mp4, vlc)")


# ----- Example usage -----
def adapter_example() -> None:
    """
    Demonstrates the Adapter pattern with a media player that can handle
    multiple audio formats through adapters.
    """
    print("\n--- Adapter Pattern Example ---")
    print("Testing AudioPlayer with different formats:\n")

    player = AudioPlayer()

    # Built-in format (no adapter needed)
    print("1. Playing MP3 (built-in support):")
    player.play("mp3", "song.mp3")

    # Adapter converts these to work with MediaPlayer interface
    print("\n2. Playing MP4 (using adapter):")
    player.play("mp4", "movie.mp4")

    print("\n3. Playing VLC (using adapter):")
    player.play("vlc", "video.vlc")

    # Unsupported format
    print("\n4. Trying unsupported format:")
    player.play("avi", "movie.avi")


if __name__ == "__main__":
    adapter_example()
