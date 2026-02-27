//! # Iterator Pattern
//!
//! The Iterator pattern provides a way to access elements of an aggregate object
//! sequentially without exposing its underlying representation.
//!
//! ## Key Participants
//! - **Iterator**: Defines interface for accessing and traversing elements
//! - **ConcreteIterator**: Implements iterator and tracks current position
//! - **Aggregate**: Defines interface for creating iterator object
//! - **ConcreteAggregate**: Implements iterator creation interface
//!
//! ## Benefits
//! - Supports variations in traversal
//! - Simplifies aggregate interface
//! - Multiple traversals active concurrently

// Iterator trait
pub trait Iterator {
    type Item;
    fn has_next(&self) -> bool;
    fn next(&mut self) -> Option<Self::Item>;
    fn reset(&mut self);
}

// Aggregate trait
pub trait Aggregate {
    type Item;
    type Iter: Iterator<Item = Self::Item>;
    fn create_iterator(&self) -> Self::Iter;
    fn add(&mut self, item: Self::Item);
    fn size(&self) -> usize;
}

// Concrete Iterator
pub struct VectorIterator<T> {
    items: Vec<T>,
    index: usize,
}

impl<T> VectorIterator<T> {
    pub fn new(items: Vec<T>) -> Self {
        Self { items, index: 0 }
    }
}

impl<T: Clone> Iterator for VectorIterator<T> {
    type Item = T;

    fn has_next(&self) -> bool {
        self.index < self.items.len()
    }

    fn next(&mut self) -> Option<Self::Item> {
        if self.has_next() {
            let item = self.items[self.index].clone();
            self.index += 1;
            Some(item)
        } else {
            None
        }
    }

    fn reset(&mut self) {
        self.index = 0;
    }
}

// Reverse Iterator
pub struct ReverseVectorIterator<T> {
    items: Vec<T>,
    index: usize,
}

impl<T> ReverseVectorIterator<T> {
    pub fn new(items: Vec<T>) -> Self {
        let index = if items.is_empty() { 0 } else { items.len() - 1 };
        Self { items, index }
    }
}

impl<T: Clone> Iterator for ReverseVectorIterator<T> {
    type Item = T;

    fn has_next(&self) -> bool {
        !self.items.is_empty() && self.index < usize::MAX
    }

    fn next(&mut self) -> Option<Self::Item> {
        if self.has_next() && self.index < self.items.len() {
            let item = self.items[self.index].clone();
            if self.index == 0 {
                self.index = usize::MAX; // Signal exhaustion
            } else {
                self.index -= 1;
            }
            Some(item)
        } else {
            None
        }
    }

    fn reset(&mut self) {
        self.index = if self.items.is_empty() {
            0
        } else {
            self.items.len() - 1
        };
    }
}

// Concrete Aggregate - Playlist
#[derive(Clone)]
pub struct Song {
    pub title: String,
    pub artist: String,
}

impl Song {
    pub fn new(title: &str, artist: &str) -> Self {
        Self {
            title: title.to_string(),
            artist: artist.to_string(),
        }
    }
}

pub struct Playlist {
    songs: Vec<Song>,
}

impl Playlist {
    pub fn new() -> Self {
        Self { songs: Vec::new() }
    }
}

impl Aggregate for Playlist {
    type Item = Song;
    type Iter = VectorIterator<Song>;

    fn create_iterator(&self) -> Self::Iter {
        VectorIterator::new(self.songs.clone())
    }

    fn add(&mut self, item: Self::Item) {
        self.songs.push(item);
    }

    fn size(&self) -> usize {
        self.songs.len()
    }
}

impl Default for Playlist {
    fn default() -> Self {
        Self::new()
    }
}

/// Example demonstrating Iterator pattern
pub fn example() {
    println!("\n--- Iterator Pattern Example ---\n");

    println!("1. Music Playlist");
    let mut playlist = Playlist::new();
    playlist.add(Song::new("Bohemian Rhapsody", "Queen"));
    playlist.add(Song::new("Stairway to Heaven", "Led Zeppelin"));
    playlist.add(Song::new("Hotel California", "Eagles"));

    println!("   Forward iteration:");
    let mut iterator = playlist.create_iterator();
    while iterator.has_next() {
        if let Some(song) = iterator.next() {
            println!("     \"{}\" by {}", song.title, song.artist);
        }
    }

    println!("\n   Reverse iteration:");
    let mut reverse_iter = ReverseVectorIterator::new(
        playlist.songs.clone()
    );
    while reverse_iter.has_next() {
        if let Some(song) = reverse_iter.next() {
            println!("     \"{}\" by {}", song.title, song.artist);
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_iterator_traverses_forward() {
        let mut playlist = Playlist::new();
        playlist.add(Song::new("Song 1", "Artist 1"));
        playlist.add(Song::new("Song 2", "Artist 2"));

        let mut iter = playlist.create_iterator();
        assert!(iter.has_next());
        iter.next();
        assert!(iter.has_next());
        iter.next();
        assert!(!iter.has_next());
    }

    #[test]
    fn test_iterator_reset() {
        let mut playlist = Playlist::new();
        playlist.add(Song::new("Song", "Artist"));

        let mut iter = playlist.create_iterator();
        iter.next();
        assert!(!iter.has_next());
        iter.reset();
        assert!(iter.has_next());
    }

    #[test]
    fn test_playlist_size() {
        let mut playlist = Playlist::new();
        assert_eq!(playlist.size(), 0);
        playlist.add(Song::new("Song", "Artist"));
        assert_eq!(playlist.size(), 1);
    }
}
