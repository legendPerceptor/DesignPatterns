#include <gtest/gtest.h>

#include "C5Iterator.h"

namespace {
TEST (IteratorTest, PlaylistIteration) {
    Iterator::Playlist<Iterator::Song> playlist;
    playlist.add (Iterator::Song ("Song1", "Artist1"));
    playlist.add (Iterator::Song ("Song2", "Artist2"));

    auto it = playlist.createIterator ();
    EXPECT_TRUE (it->hasNext ());
    EXPECT_NO_THROW ({
        it->next ();
        it->next ();
    });
}

TEST (IteratorTest, ReverseIterator) {
    Iterator::Playlist<Iterator::Song> playlist;
    playlist.add (Iterator::Song ("Song1", "Artist1"));

    auto it = playlist.createReverseIterator ();
    EXPECT_TRUE (it->hasNext ());
}
} // namespace
