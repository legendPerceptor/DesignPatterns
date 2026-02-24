#include <gtest/gtest.h>

#include "C9State.h"

namespace {
TEST (StateTest, VendingMachineInitialState) {
    State::VendingMachine machine (3);
    EXPECT_EQ (machine.getStateName (), "No Coin");
    EXPECT_EQ (machine.getItemCount (), 3);
}

TEST (StateTest, InsertCoin) {
    State::VendingMachine machine (3);
    machine.insertCoin ();
    EXPECT_EQ (machine.getStateName (), "Has Coin");
}

TEST (StateTest, DocumentWorkflow) {
    State::Document doc;
    EXPECT_EQ (doc.getStateName (), "Draft");
    doc.publish ();
    EXPECT_EQ (doc.getStateName (), "Under Moderation");
}
} // namespace
