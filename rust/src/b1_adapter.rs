//! # Adapter Pattern
//!
//! The Adapter pattern converts the interface of a class into another interface
//! clients expect. Adapter lets classes work together that couldn't otherwise
//! because of incompatible interfaces.
//!
//! ## Key Participants
//! - **Target**: Defines domain-specific interface Client uses
//! - **Adapter**: Adapts Adaptee interface to Target interface
//! - **Adaptee**: Defines existing interface that needs adapting
//! - **Client**: Collaborates with objects conforming to Target interface
//!
//! ## Benefits
//! - Lets two incompatible interfaces work together
//! - Reuses existing functionality
//! - Decouples client from implemented interface

// Target interface
pub trait MediaPlayer {
    fn play(&self, audio_type: &str, filename: &str);
}

// Adaptee interface
pub trait AdvancedMediaPlayer {
    fn play_mp4(&self, filename: &str);
    fn play_vlc(&self, filename: &str);
}

// Concrete Adaptee
pub struct Mp4Player;

impl AdvancedMediaPlayer for Mp4Player {
    fn play_mp4(&self, filename: &str) {
        println!("Playing MP4 file: {}", filename);
    }

    fn play_vlc(&self, _filename: &str) {
        // Nothing to do
    }
}

pub struct VlcPlayer;

impl AdvancedMediaPlayer for VlcPlayer {
    fn play_mp4(&self, _filename: &str) {
        // Nothing to do
    }

    fn play_vlc(&self, filename: &str) {
        println!("Playing VLC file: {}", filename);
    }
}

// Adapter
pub struct MediaAdapter {
    advanced_player: Option<Box<dyn AdvancedMediaPlayer>>,
}

impl MediaAdapter {
    pub fn new(audio_type: &str) -> Self {
        let player: Option<Box<dyn AdvancedMediaPlayer>> = match audio_type {
            "mp4" => Some(Box::new(Mp4Player)),
            "vlc" => Some(Box::new(VlcPlayer)),
            _ => None,
        };

        Self { advanced_player: player }
    }
}

impl MediaPlayer for MediaAdapter {
    fn play(&self, audio_type: &str, filename: &str) {
        if let Some(ref player) = self.advanced_player {
            match audio_type {
                "mp4" => player.play_mp4(filename),
                "vlc" => player.play_vlc(filename),
                _ => {}
            }
        }
    }
}

// Concrete Target
pub struct AudioPlayer;

impl AudioPlayer {
    pub fn new() -> Self {
        Self
    }
}

impl MediaPlayer for AudioPlayer {
    fn play(&self, audio_type: &str, filename: &str) {
        match audio_type {
            "mp3" => println!("Playing MP3 file: {}", filename),
            "mp4" | "vlc" => {
                let adapter = MediaAdapter::new(audio_type);
                adapter.play(audio_type, filename);
            }
            _ => println!("Invalid media format: {}. Only mp3, mp4, vlc supported", audio_type),
        }
    }
}

/// Example demonstrating Adapter pattern
pub fn example() {
    println!("\n--- Adapter Pattern Example ---\n");
    println!("Testing AudioPlayer with different formats:\n");

    let player = AudioPlayer::new();

    println!("1. Playing MP3 (built-in support):");
    player.play("mp3", "song.mp3");

    println!("\n2. Playing MP4 (using adapter):");
    player.play("mp4", "movie.mp4");

    println!("\n3. Playing VLC (using adapter):");
    player.play("vlc", "video.vlc");

    println!("\n4. Trying unsupported format:");
    player.play("avi", "movie.avi");
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_mp3_plays_directly() {
        let player = AudioPlayer::new();
        player.play("mp3", "test.mp3");
        // Should not panic
    }

    #[test]
    fn test_adapter_handles_mp4() {
        let adapter = MediaAdapter::new("mp4");
        adapter.play("mp4", "test.mp4");
        // Should not panic
    }

    #[test]
    fn test_adapter_handles_vlc() {
        let adapter = MediaAdapter::new("vlc");
        adapter.play("vlc", "test.vlc");
        // Should not panic
    }

    #[test]
    fn test_audio_player_with_adapter() {
        let player = AudioPlayer::new();
        player.play("mp4", "test.mp4");
        player.play("vlc", "test.vlc");
        // Should not panic
    }

    #[test]
    fn test_unsupported_format_error_message() {
        let player = AudioPlayer::new();
        player.play("avi", "test.avi");
        // Should print error message, not panic
    }
}
