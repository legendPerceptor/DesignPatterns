"""
The intent is to define a one-to-many dependency so that when one object changes
state, all its dependents are notified and updated automatically.

Key participants:
- Subject: Knows its observers and provides attach/detach interface
- Observer: Defines updating interface for notifications
- ConcreteSubject: Stores state of interest and sends notifications
- ConcreteObserver: Maintains reference to subject and implements update

Key benefits:
- Abstract coupling between Subject and Observer
- Support for broadcast communication
- Subjects and observers vary independently

Real-world example: A weather station broadcasting updates to multiple displays.
"""

from abc import ABC, abstractmethod
from dataclasses import dataclass
from datetime import datetime
from typing import TypeVar, Generic, List, TYPE_CHECKING

if TYPE_CHECKING:
    pass

T = TypeVar('T')


# ----- Observer interface -----
class Observer(ABC, Generic[T]):
    @abstractmethod
    def update(self, data: T) -> None:
        pass

    @abstractmethod
    def get_name(self) -> str:
        pass


# ----- Subject -----
class Subject(ABC, Generic[T]):
    def __init__(self) -> None:
        self._observers: List[Observer[T]] = []

    def attach(self, observer: Observer[T]) -> None:
        self._observers.append(observer)
        print(f"[Subject] Attached observer: {observer.get_name()}")

    def detach(self, observer: Observer[T]) -> None:
        if observer in self._observers:
            self._observers.remove(observer)
            print(f"[Subject] Detached observer: {observer.get_name()}")

    def notify(self, data: T) -> None:
        for observer in self._observers:
            observer.update(data)


# ----- Weather Station Example -----
@dataclass
class WeatherData:
    temperature: float
    humidity: float
    pressure: float
    timestamp: str


class WeatherStation(Subject[WeatherData]):
    def __init__(self) -> None:
        super().__init__()
        self._measurements = WeatherData(0.0, 0.0, 0.0, "")

    def set_measurements(self, temperature: float, humidity: float, pressure: float) -> None:
        timestamp = datetime.now().strftime("%H:%M:%S")

        self._measurements = WeatherData(temperature, humidity, pressure, timestamp)
        print(f"\n[WeatherStation] New measurements: {temperature}°C, {humidity}% humidity, {pressure} hPa")
        self.notify(self._measurements)

    @property
    def measurements(self) -> WeatherData:
        return self._measurements


# ----- Concrete Observers -----
class PhoneDisplay(Observer[WeatherData]):
    def __init__(self, owner: str) -> None:
        self._owner = owner

    def update(self, data: WeatherData) -> None:
        print(f"  [Phone Display - {self._owner}] Notification received!")
        print(f"    Weather Alert: {data.temperature}°C, {data.humidity}% humidity")
        self._display(data)

    def get_name(self) -> str:
        return f"Phone Display ({self._owner})"

    def _display(self, data: WeatherData) -> None:
        print(f"    ┌─────────────────┐")
        print(f"    │ {data.timestamp}   │")
        print(f"    │ {data.temperature}°C  │")
        print(f"    │ {data.humidity}%   │")
        print(f"    └─────────────────┘")


class WebDisplay(Observer[WeatherData]):
    def update(self, data: WeatherData) -> None:
        print("  [Web Display] Updating dashboard...")
        print(f"    <div class='weather-widget'>")
        print(f"      <h2>Weather Station</h2>")
        print(f"      <p>Temperature: {data.temperature}°C</p>")
        print(f"      <p>Humidity: {data.humidity}%</p>")
        print(f"      <p>Pressure: {data.pressure} hPa</p>")
        print(f"    </div>")

    def get_name(self) -> str:
        return "Web Display"


class ForecastDisplay(Observer[WeatherData]):
    def __init__(self) -> None:
        self._last_data: WeatherData | None = None

    def update(self, data: WeatherData) -> None:
        print("  [Forecast Display] Analyzing trends...")

        if data.pressure < 1000:
            forecast = "Rain expected"
        elif data.pressure > 1020:
            forecast = "Clear skies ahead"
        else:
            forecast = "Conditions stable"

        if data.temperature > 30:
            forecast += ", high temp advisory"
        elif data.temperature < 5:
            forecast += ", frost warning"

        print(f"    Forecast: {forecast}")
        self._last_data = data

    def get_name(self) -> str:
        return "Forecast Display"


# ----- YouTube Channel Example -----
class YouTubeChannel(Subject[str]):
    def __init__(self, name: str) -> None:
        super().__init__()
        self._name = name

    def upload_video(self, title: str) -> None:
        print(f"\n[YouTube] {self._name} uploaded new video: \"{title}\"")
        self.notify(title)


class Subscriber(Observer[str]):
    def __init__(self, name: str) -> None:
        self._name = name

    def update(self, video_title: str) -> None:
        print(f"  [Subscriber] {self._name} received notification: \"{video_title}\" is now live!")

    def get_name(self) -> str:
        return self._name


# ----- Example usage -----
def observer_example() -> None:
    """
    Demonstrates the Observer pattern with weather station and YouTube.
    """
    print("\n--- Observer Pattern Example ---")

    # Weather Station Example
    print("\n1. Weather Station")
    weather_station = WeatherStation()

    phone = PhoneDisplay("John")
    web = WebDisplay()
    forecast = ForecastDisplay()

    weather_station.attach(phone)
    weather_station.attach(web)
    weather_station.attach(forecast)

    weather_station.set_measurements(25.5, 65.0, 1015.0)
    weather_station.set_measurements(28.0, 70.0, 1008.0)

    # YouTube Example
    print("\n2. YouTube Channel")
    channel = YouTubeChannel("CodeMaster")

    subscriber1 = Subscriber("Alice")
    subscriber2 = Subscriber("Bob")
    subscriber3 = Subscriber("Charlie")

    channel.attach(subscriber1)
    channel.attach(subscriber2)
    channel.attach(subscriber3)

    channel.upload_video("Python Design Patterns Tutorial")


# Test helpers
def get_weather_station() -> WeatherStation:
    """Helper for tests: Create a weather station."""
    return WeatherStation()


def get_youtube_channel() -> YouTubeChannel:
    """Helper for tests: Create a YouTube channel."""
    return YouTubeChannel("TestChannel")


if __name__ == "__main__":
    observer_example()
