/*
The intent is to convert the interface of a class into another interface clients expect.
Adapter lets classes work together that couldn't otherwise because of incompatible interfaces.

Types of adapters:
1. Class Adapter - uses multiple inheritance (not shown here, less common in modern C++)
2. Object Adapter - uses composition to wrap the adaptee (shown below)

Real-world example: A media player that can play multiple formats by using adapters
to convert incompatible audio formats into a common interface.
*/
#ifndef B1_ADAPTER_H
#define B1_ADAPTER_H

#include <iostream>
#include <memory>
#include <string>

namespace Adapter {

// ----- Target interface -----
// The interface that the client expects
class MediaPlayer {
    public:
    virtual ~MediaPlayer () = default;
    virtual void play (const std::string& audioType, const std::string& fileName) = 0;
};

// ----- Adaptee -----
// The incompatible interface that needs adapting
class AdvancedMediaPlayer {
    public:
    virtual ~AdvancedMediaPlayer () = default;

    virtual void playMp4 (const std::string& fileName) {
        std::cout << "Playing MP4 file: " << fileName << "\n";
    }

    virtual void playVlc (const std::string& fileName) {
        std::cout << "Playing VLC file: " << fileName << "\n";
    }
};

// ----- Concrete Target -----
// A simple media player that only plays MP3
class Mp3Player : public MediaPlayer {
    public:
    void play (const std::string& audioType, const std::string& fileName) override {
        if (audioType == "mp3") {
            std::cout << "Playing MP3 file: " << fileName << "\n";
        } else {
            std::cout << "Invalid media format. Mp3Player only supports mp3.\n";
        }
    }
};

// ----- Adapter -----
// Adapts the AdvancedMediaPlayer interface to the MediaPlayer interface
class MediaAdapter : public MediaPlayer {
    public:
    explicit MediaAdapter (const std::string& audioType) {
        if (audioType == "mp4") {
            advancedPlayer_ = std::make_unique<AdvancedMediaPlayer> ();
        } else if (audioType == "vlc") {
            advancedPlayer_ = std::make_unique<AdvancedMediaPlayer> ();
        }
    }

    void play (const std::string& audioType, const std::string& fileName) override {
        if (audioType == "mp4" && advancedPlayer_) {
            advancedPlayer_->playMp4 (fileName);
        } else if (audioType == "vlc" && advancedPlayer_) {
            advancedPlayer_->playVlc (fileName);
        }
    }

    private:
    std::unique_ptr<AdvancedMediaPlayer> advancedPlayer_;
};

// ----- Client -----
// Uses the MediaPlayer interface to play various audio formats
class AudioPlayer : public MediaPlayer {
    public:
    AudioPlayer () = default;

    void play (const std::string& audioType, const std::string& fileName) override {
        // Built-in support for MP3
        if (audioType == "mp3") {
            mp3Player_.play (audioType, fileName);
        }
        // Use adapter for other formats
        else if (audioType == "mp4" || audioType == "vlc") {
            if (!mediaAdapter_) {
                // Lazy initialization of adapter
                mediaAdapter_ = std::make_unique<MediaAdapter> (audioType);
            }
            mediaAdapter_->play (audioType, fileName);
        } else {
            std::cout << "Invalid media format: " << audioType << " (supported: mp3, mp4, vlc)\n";
        }
    }

    private:
    Mp3Player mp3Player_;
    std::unique_ptr<MediaAdapter> mediaAdapter_;
};

} // namespace Adapter

#endif
