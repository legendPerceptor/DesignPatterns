#include <gtest/gtest.h>

#include "B3Composite.h"

namespace {
// Test individual file size
TEST (CompositeTest, FileSize) {
    Composite::File file ("test.txt", 1024);
    EXPECT_EQ (file.size (), 1024);
    EXPECT_EQ (file.getName (), "test.txt");
}

// Test directory calculates total size correctly
TEST (CompositeTest, DirectoryTotalSize) {
    auto dir = std::make_shared<Composite::Directory> ("root");

    dir->add (std::make_shared<Composite::File> ("file1.txt", 100));
    dir->add (std::make_shared<Composite::File> ("file2.txt", 200));
    dir->add (std::make_shared<Composite::File> ("file3.txt", 300));

    EXPECT_EQ (dir->size (), 600);
}

// Test nested directory structure
TEST (CompositeTest, NestedDirectories) {
    auto subDir = std::make_shared<Composite::Directory> ("subdir");
    subDir->add (std::make_shared<Composite::File> ("inner.txt", 500));

    auto rootDir = std::make_shared<Composite::Directory> ("root");
    rootDir->add (std::make_shared<Composite::File> ("outer.txt", 1000));
    rootDir->add (subDir);

    EXPECT_EQ (rootDir->size (), 1500); // 1000 + 500
}

// Test empty directory
TEST (CompositeTest, EmptyDirectory) {
    Composite::Directory emptyDir ("empty");
    EXPECT_EQ (emptyDir.size (), 0);
}

// Test composite treats files and directories uniformly
TEST (CompositeTest, UniformTreatment) {
    std::shared_ptr<Composite::FileSystemNode> file =
    std::make_shared<Composite::File> ("doc.txt", 512);
    std::shared_ptr<Composite::FileSystemNode> dir =
    std::make_shared<Composite::Directory> ("docs");

    // Both can be treated as FileSystemNode
    EXPECT_EQ (file->getName (), "doc.txt");
    EXPECT_EQ (dir->getName (), "docs");
}
} // namespace
