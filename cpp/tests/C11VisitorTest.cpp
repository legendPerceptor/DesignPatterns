#include <gtest/gtest.h>

#include "C11Visitor.h"

namespace {
TEST (VisitorTest, BookPrice) {
    Visitor::Book book ("Design Patterns", "Gamma", 49.99);
    EXPECT_EQ (book.getPrice (), 49.99);
}

TEST (VisitorTest, ShoppingCartTaxVisitor) {
    Visitor::ShoppingCart cart;
    cart.addItem (std::make_shared<Visitor::Book> ("Book", "Author", 50.0));

    Visitor::TaxVisitor taxVisitor;
    testing::internal::CaptureStdout ();
    cart.accept (taxVisitor);
    testing::internal::GetCapturedStdout ();
}

TEST (VisitorTest, FileSystemVisitor) {
    auto file = std::make_shared<Visitor::File> ("test.txt", 1024);
    EXPECT_EQ (file->getSize (), 1024);
    EXPECT_EQ (file->getName (), "test.txt");
}
} // namespace
