#include <gtest/gtest.h>

#include "C4Command.h"

namespace {
TEST (CommandTest, LightOnCommand) {
    auto light   = std::make_shared<Command::Light> ();
    auto command = std::make_shared<Command::LightOnCommand> (light);

    EXPECT_NO_THROW ({
        command->execute ();
        command->undo ();
    });
}

TEST (CommandTest, RemoteControl) {
    Command::RemoteControl remote;
    auto light    = std::make_shared<Command::Light> ();
    auto lightOn  = std::make_shared<Command::LightOnCommand> (light);
    auto lightOff = std::make_shared<Command::LightOffCommand> (light);

    EXPECT_NO_THROW ({
        remote.setCommand (0, lightOn, lightOff);
        remote.onButtonWasPressed (0);
        remote.offButtonWasPressed (0);
    });
}
} // namespace
