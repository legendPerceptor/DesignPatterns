#include <gtest/gtest.h>

#include "B1Adapter.h"

namespace {
TEST (AdapterTest, PlayMp3Directly) {
    Adapter::AudioPlayer player;
    EXPECT_NO_THROW ({ player.play ("mp3", "song.mp3"); });
}

TEST (AdapterTest, PlayMp4WithAdapter) {
    Adapter::AudioPlayer player;
    EXPECT_NO_THROW ({ player.play ("mp4", "movie.mp4"); });
}
} // namespace
