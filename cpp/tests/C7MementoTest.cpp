#include <gtest/gtest.h>

#include "C7Memento.h"

namespace {
TEST (MementoTest, SaveAndRestore) {
    Memento::TextDocument doc;
    doc.write ("Hello");

    auto memento = doc.save ();
    doc.write (" World");

    doc.restore (memento);
    EXPECT_EQ (doc.getContent (), "Hello");
}

TEST (MementoTest, HistoryUndo) {
    Memento::TextDocument doc;
    Memento::History history;

    history.setInitialState (doc.save ());
    doc.write ("State1");
    history.saveState (doc.save ());

    doc.write ("State2");
    history.saveState (doc.save ());

    auto previous = history.undo ();
    ASSERT_NE (previous, nullptr);
    doc.restore (previous);
}
} // namespace
