/*
The intent is to define a one-to-many dependency so that when one object changes
state, all its dependents are notified and updated automatically.

Key participants:
- Subject: Knows its observers and provides an interface for attaching/detaching
- Observer: Defines an updating interface for objects that should be notified
- ConcreteSubject: Stores state of interest to ConcreteObserver objects; sends
  notifications to its observers
- ConcreteObserver: Maintains a reference to a ConcreteSubject; implements the
  Observer updating interface to keep its state consistent with the subject's

Key benefits:
- Abstract coupling between Subject and Observer
- Support for broadcast communication
- Subjects and observers can vary independently

Real-world example: A weather station that broadcasts updates to multiple
display devices (phone, web, etc.).
*/
#ifndef C8_OBSERVER_H
#define C8_OBSERVER_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace Observer {

// ----- Observer interface -----
template <typename T> class Observer {
    public:
    virtual ~Observer ()                 = default;
    virtual void update (const T& data)  = 0;
    virtual std::string getName () const = 0;
};

// ----- Subject -----
template <typename T> class Subject {
    public:
    virtual ~Subject () = default;

    void attach (std::shared_ptr<Observer<T>> observer) {
        observers_.push_back (observer);
        std::cout << "[Subject] Attached observer: " << observer->getName () << "\n";
    }

    void detach (std::shared_ptr<Observer<T>> observer) {
        auto it = std::remove_if (observers_.begin (), observers_.end (),
        [&observer] (const auto& obs) { return obs == observer; });
        if (it != observers_.end ()) {
            std::cout << "[Subject] Detached observer: " << observer->getName () << "\n";
            observers_.erase (it, observers_.end ());
        }
    }

    void notify (const T& data) {
        for (auto& observer : observers_) {
            observer->update (data);
        }
    }

    protected:
    std::vector<std::shared_ptr<Observer<T>>> observers_;
};

// ----- Weather Station Example -----
struct WeatherData {
    double temperature;
    double humidity;
    double pressure;
    std::string timestamp;

    WeatherData (double temp, double hum, double press, const std::string& time)
    : temperature (temp), humidity (hum), pressure (press), timestamp (time) {
    }
};

// Concrete Subject
class WeatherStation : public Subject<WeatherData> {
    public:
    void setMeasurements (double temperature, double humidity, double pressure) {
        // Get current time
        time_t now = time (nullptr);
        char buf[80];
        strftime (buf, sizeof (buf), "%H:%M:%S", localtime (&now));
        std::string timestamp (buf);

        WeatherData data (temperature, humidity, pressure, timestamp);
        measurements_ = data;
        std::cout << "\n[WeatherStation] New measurements: " << temperature
                  << "°C, " << humidity << "% humidity, " << pressure << " hPa\n";
        notify (data);
    }

    WeatherData getMeasurements () const {
        return measurements_;
    }

    private:
    WeatherData measurements_{ 0.0, 0.0, 0.0, "" };
};

// Concrete Observers
class PhoneDisplay : public Observer<WeatherData> {
    public:
    PhoneDisplay (const std::string& owner) : owner_ (owner) {
    }

    void update (const WeatherData& data) override {
        std::cout << "  [Phone Display - " << owner_ << "] Notification received!\n";
        std::cout << "    Weather Alert: " << data.temperature << "°C, "
                  << data.humidity << "% humidity\n";
        display (data);
    }

    std::string getName () const override {
        return "Phone Display (" + owner_ + ")";
    }

    private:
    void display (const WeatherData& data) const {
        std::cout << "    ┌─────────────────┐\n";
        std::cout << "    │ " << data.timestamp << "   │\n";
        std::cout << "    │ " << data.temperature << "°C  │\n";
        std::cout << "    │ " << data.humidity << "%   │\n";
        std::cout << "    └─────────────────┘\n";
    }

    std::string owner_;
};

class WebDisplay : public Observer<WeatherData> {
    public:
    void update (const WeatherData& data) override {
        std::cout << "  [Web Display] Updating dashboard...\n";
        std::cout << "    <div class='weather-widget'>\n";
        std::cout << "      <h2>Weather Station</h2>\n";
        std::cout << "      <p>Temperature: " << data.temperature << "°C</p>\n";
        std::cout << "      <p>Humidity: " << data.humidity << "%</p>\n";
        std::cout << "      <p>Pressure: " << data.pressure << " hPa</p>\n";
        std::cout << "    </div>\n";
    }

    std::string getName () const override {
        return "Web Display";
    }
};

class ForecastDisplay : public Observer<WeatherData> {
    public:
    void update (const WeatherData& data) override {
        std::cout << "  [Forecast Display] Analyzing trends...\n";

        // Simple forecast logic
        std::string forecast;
        if (data.pressure < 1000) {
            forecast = "Rain expected";
        } else if (data.pressure > 1020) {
            forecast = "Clear skies ahead";
        } else {
            forecast = "Conditions stable";
        }

        if (data.temperature > 30) {
            forecast += ", high temp advisory";
        } else if (data.temperature < 5) {
            forecast += ", frost warning";
        }

        std::cout << "    Forecast: " << forecast << "\n";
        lastData_ = data;
    }

    std::string getName () const override {
        return "Forecast Display";
    }

    private:
    WeatherData lastData_{ 0.0, 0.0, 0.0, "" };
};

// ----- YouTube Channel Example -----
class YouTubeChannel;

class Subscriber : public Observer<std::string> {
    public:
    Subscriber (const std::string& name) : name_ (name) {
    }

    void update (const std::string& videoTitle) override {
        std::cout << "  [Subscriber] " << name_ << " received notification: \""
                  << videoTitle << "\" is now live!\n";
    }

    std::string getName () const override {
        return name_;
    }

    private:
    std::string name_;
};

class YouTubeChannel : public Subject<std::string> {
    public:
    YouTubeChannel (const std::string& name)
    : name_ (name), subscriberCount_ (0) {
    }

    void uploadVideo (const std::string& title) {
        std::cout << "\n[YouTube] " << name_ << " uploaded new video: \"" << title << "\"\n";
        notify (title);
    }

    std::string getName () const {
        return name_;
    }

    private:
    std::string name_;
    int subscriberCount_;
};

} // namespace Observer

#endif
