#include "designPatternExamples.h"

void FactoryMethodExample (bool useSpreadSheet) {
    std::unique_ptr<FactoryMethod::Application> app;

    if (useSpreadSheet) {
        app = std::make_unique<FactoryMethod::SpreadsheetApp> ();
    } else {
        app = std::make_unique<FactoryMethod::TextEditorApp> ();
    }
    // Based on the app, our factory method will decide what type of document to
    // create.
    app->newDocument ();
}

void AbstractFactoryExample (bool isWindows) {
    std::unique_ptr<AbstractFactory::GUIFactory> factory;
    if (isWindows) {
        factory = std::make_unique<AbstractFactory::WinFactory> ();
    } else {
        factory = std::make_unique<AbstractFactory::MacFactory> ();
    }

    AbstractFactory::Application app (std::move (factory));
    // A series of concrete products in the same family will be created.
    app.renderUI ();
}

void BuilderExample () {
    Builder::HttpRequest req = Builder::HttpRequestBuilder ()
                               .method ("POST")
                               .url ("https://example.com/api")
                               .body ("{\"id\":42}")
                               .timeout (5000)
                               .build ();

    std::cout << req.method << " " << req.url << " timeout=" << req.timeoutMs << "ms\n";
}

void PrototypeExample () {
    Prototype::PrototypeRegistry registry;

    // Configure some “template” objects
    auto redCircle = std::make_unique<Prototype::Circle> ();
    redCircle->setRadius (10.0);
    redCircle->setColor ("red");

    auto blueRect = std::make_unique<Prototype::Rectangle> ();
    blueRect->setSize (4.0, 2.0);
    blueRect->setColor ("blue");

    // Register them
    registry.registerPrototype ("red-circle", std::move (redCircle));
    registry.registerPrototype ("blue-rect", std::move (blueRect));

    // Later in the program: create new objects by cloning
    auto c1 = registry.create ("red-circle");
    auto c2 = registry.create ("red-circle");
    auto r1 = registry.create ("blue-rect");

    // We can still tweak the clones individually
    dynamic_cast<Prototype::Circle*> (c2.get ())->setRadius (20.0);

    std::cout << "c1: ";
    c1->draw ();
    std::cout << "c2: ";
    c2->draw ();
    std::cout << "r1: ";
    r1->draw ();
}

void SingletonExample () {
    // The log will be written into app.log file that should be in your current workdir.
    Singleton::Logger::instance ().log ("This is a singleton instance's log");
    Singleton::Logger::instance ().log ("Application exiting.");
}

void AdapterExample () {
    std::cout << "\n--- Adapter Pattern Example ---\n";
    std::cout << "Testing AudioPlayer with different formats:\n\n";

    Adapter::AudioPlayer player;

    // Built-in format (no adapter needed)
    std::cout << "1. Playing MP3 (built-in support):\n";
    player.play ("mp3", "song.mp3");

    // Adapter converts these to work with MediaPlayer interface
    std::cout << "\n2. Playing MP4 (using adapter):\n";
    player.play ("mp4", "movie.mp4");

    std::cout << "\n3. Playing VLC (using adapter):\n";
    player.play ("vlc", "video.vlc");

    // Unsupported format
    std::cout << "\n4. Trying unsupported format:\n";
    player.play ("avi", "movie.avi");
}

void BridgeExample () {
    std::cout << "\n--- Bridge Pattern Example ---\n";
    std::cout << "Testing RemoteControls with different Devices:\n\n";

    // Test 1: Basic Remote with TV
    std::cout << "1. Basic Remote with Sony TV:\n";
    auto sonyTV = std::make_unique<Bridge::TV> ("Sony");
    Bridge::RemoteControl basicRemote (std::move (sonyTV));
    basicRemote.togglePower ();
    basicRemote.volumeUp ();
    basicRemote.volumeUp ();
    basicRemote.togglePower ();

    // Test 2: Advanced Remote with Radio
    std::cout << "\n2. Advanced Remote with Radio:\n";
    auto radio = std::make_unique<Bridge::Radio> ("98.5 FM");
    Bridge::AdvancedRemoteControl advancedRemote (std::move (radio));
    advancedRemote.togglePower ();
    advancedRemote.volumeUp ();
    advancedRemote.mute ();
    advancedRemote.setChannel (5);

    // Test 3: Advanced Remote with different TV
    std::cout << "\n3. Advanced Remote with LG TV:\n";
    auto lgTV = std::make_unique<Bridge::TV> ("LG");
    Bridge::AdvancedRemoteControl lgRemote (std::move (lgTV));
    lgRemote.togglePower ();
    lgRemote.volumeUp ();
    lgRemote.setChannel (42);
    lgRemote.togglePower ();
}

void CompositeExample () {
    std::cout << "\n--- Composite Pattern Example ---\n";
    std::cout << "Building a file system structure:\n\n";

    // Create individual files
    auto file1 = std::make_shared<Composite::File> ("readme.txt", 1024);
    auto file2 = std::make_shared<Composite::File> ("main.cpp", 4096);
    auto file3 = std::make_shared<Composite::File> ("utils.cpp", 2048);
    auto file4 = std::make_shared<Composite::File> ("config.json", 512);

    // Create directories and add files
    auto srcDir = std::make_shared<Composite::Directory> ("src");
    srcDir->add (file2);
    srcDir->add (file3);

    auto configDir = std::make_shared<Composite::Directory> ("config");
    configDir->add (file4);

    auto rootDir = std::make_shared<Composite::Directory> ("project");
    rootDir->add (file1);
    rootDir->add (srcDir);
    rootDir->add (configDir);

    // Print the entire structure (treats all nodes uniformly)
    rootDir->print ();

    // Calculate total size (works for both files and directories)
    std::cout << "\nTotal project size: " << rootDir->size () << " bytes\n";
}

void DecoratorExample () {
    std::cout << "\n--- Decorator Pattern Example ---\n";
    std::cout << "Ordering beverages with various toppings:\n\n";

    // Simple espresso
    std::cout << "1. Simple Espresso:\n";
    std::unique_ptr<Decorator::Beverage> beverage1 =
    std::make_unique<Decorator::Espresso> ();
    std::cout << "  " << beverage1->getDescription () << " $" << beverage1->cost () << "\n";

    // Dark Roast with double Mocha and Whip
    std::cout << "\n2. Dark Roast + Mocha + Mocha + Whip:\n";
    std::unique_ptr<Decorator::Beverage> beverage2 =
    std::make_unique<Decorator::DarkRoast> ();
    beverage2 = std::make_unique<Decorator::Mocha> (std::move (beverage2));
    beverage2 = std::make_unique<Decorator::Mocha> (std::move (beverage2));
    beverage2 = std::make_unique<Decorator::Whip> (std::move (beverage2));
    std::cout << "  " << beverage2->getDescription () << " $" << beverage2->cost () << "\n";

    // House Blend with Soy, Mocha, and Whip
    std::cout << "\n3. House Blend + Soy + Mocha + Whip:\n";
    std::unique_ptr<Decorator::Beverage> beverage3 =
    std::make_unique<Decorator::HouseBlend> ();
    beverage3 = std::make_unique<Decorator::Soy> (std::move (beverage3));
    beverage3 = std::make_unique<Decorator::Mocha> (std::move (beverage3));
    beverage3 = std::make_unique<Decorator::Whip> (std::move (beverage3));
    std::cout << "  " << beverage3->getDescription () << " $" << beverage3->cost () << "\n";
}

void FacadeExample () {
    std::cout << "\n--- Facade Pattern Example ---\n";
    std::cout << "Setting up Home Theater with Facade:\n";

    // Create all the subsystem components
    auto amp       = std::make_shared<Facade::Amplifier> ();
    auto tuner     = std::make_shared<Facade::Tuner> ();
    auto dvd       = std::make_shared<Facade::DvdPlayer> ();
    auto cd        = std::make_shared<Facade::CdPlayer> ();
    auto projector = std::make_shared<Facade::Projector> ();
    auto lights    = std::make_shared<Facade::TheaterLights> ();
    auto screen    = std::make_shared<Facade::Screen> ();
    auto popper    = std::make_shared<Facade::PopcornPopper> ();

    // Create the facade with all components
    Facade::HomeTheaterFacade homeTheater (
    amp, tuner, dvd, cd, projector, lights, screen, popper);

    // Use the simplified interface
    homeTheater.watchMovie ("Inception");
    homeTheater.endMovie ();
    std::cout << "\n";
    homeTheater.listenToRadio (101.5);
    homeTheater.endRadio ();
}

void FlyweightExample () {
    std::cout << "\n--- Flyweight Pattern Example ---\n";
    std::cout << "1. Text Editor Example:\n";

    auto factory = std::make_shared<Flyweight::CharacterFactory> ();
    Flyweight::Document document (factory);

    // Add the same character multiple times with different styles
    // Only 3 flyweights are created despite many characters
    document.addCharacter ('H', 12, "black");
    document.addCharacter ('e', 12, "black");
    document.addCharacter ('l', 12, "black");
    document.addCharacter ('l', 12, "black");
    document.addCharacter ('o', 12, "black");
    document.addSpace (12);
    document.addCharacter ('W', 14, "blue");
    document.addCharacter ('o', 14, "blue");
    document.addCharacter ('r', 14, "blue");
    document.addCharacter ('l', 14, "blue");
    document.addCharacter ('d', 14, "blue");
    document.addSpace (14);

    document.render ();

    std::cout << "\n2. Forest Example:\n";
    Flyweight::Forest forest;

    // Plant many trees, but only 3 TreeType objects are created
    forest.plantTree (10, 20, "Oak", "Green", "Rough");
    forest.plantTree (30, 40, "Oak", "Green", "Rough"); // Reuses Oak TreeType
    forest.plantTree (50, 60, "Pine", "Dark Green", "Needles");
    forest.plantTree (70, 80, "Pine", "Dark Green", "Needles"); // Reuses Pine TreeType
    forest.plantTree (90, 100, "Birch", "White", "Smooth");

    forest.draw ();
}

void ProxyExample () {
    std::cout << "\n--- Proxy Pattern Example ---\n";
    std::cout << "1. Virtual Proxy (Lazy Loading):\n\n";

    // Create proxy without loading the actual image
    std::shared_ptr<Proxy::Image> image1 = std::make_shared<Proxy::ProxyImage> ("photo.jpg");
    std::shared_ptr<Proxy::Image> image2 = std::make_shared<Proxy::ProxyImage> ("photo.jpg");

    std::cout << "First display - image will be loaded:\n";
    image1->display ();

    std::cout << "\nSecond display - image already loaded:\n";
    image1->display ();

    std::cout << "\n2. Protection Proxy (Access Control):\n\n";
    Proxy::ProxyInternet internet;

    std::cout << "Trying to connect to allowed site:\n";
    internet.connectTo ("google.com");

    std::cout << "\nTrying to connect to blocked site:\n";
    internet.connectTo ("blocked.com");

    std::cout << "\n3. Smart Reference (Reference Counting):\n\n";
    {
        Proxy::SmartPtr<Proxy::Resource> ptr1 (new Proxy::Resource ());
        ptr1->doSomething ();

        std::cout << "\nCreating copy:\n";
        Proxy::SmartPtr<Proxy::Resource> ptr2 = ptr1;
        ptr2->doSomething ();

        std::cout << "\nExiting scope, ptr2 destroyed:\n";
    }

    std::cout << "\nExiting scope, ptr1 destroyed:\n";
}

void ChainOfResponsibilityExample () {
    std::cout << "\n--- Chain of Responsibility Pattern Example ---\n";
    std::cout
    << "Creating support chain: Level 1 -> Level 2 -> Manager -> Director\n\n";

    // Create the chain of responsibility
    auto level1   = std::make_shared<ChainOfResponsibility::Level1Support> ();
    auto level2   = std::make_shared<ChainOfResponsibility::Level2Support> ();
    auto manager  = std::make_shared<ChainOfResponsibility::Manager> ();
    auto director = std::make_shared<ChainOfResponsibility::Director> ();

    // Build the chain: Level1 -> Level2 -> Manager -> Director
    level1->setNext (level2);
    level2->setNext (manager);
    manager->setNext (director);

    // Test various tickets with different complexity levels
    std::cout << "Test 1: Basic issue (handled by Level 1)\n";
    ChainOfResponsibility::SupportTicket basic (
    "Password reset needed", ChainOfResponsibility::SupportTicket::Level::Basic);
    level1->handleTicket (basic);

    std::cout << "\nTest 2: Intermediate issue (escalates to Level 2)\n";
    ChainOfResponsibility::SupportTicket intermediate (
    "Software installation failure", ChainOfResponsibility::SupportTicket::Level::Intermediate);
    level1->handleTicket (intermediate);

    std::cout << "\nTest 3: Advanced issue (escalates to Manager)\n";
    ChainOfResponsibility::SupportTicket advanced (
    "System architecture decision needed",
    ChainOfResponsibility::SupportTicket::Level::Advanced);
    level1->handleTicket (advanced);

    std::cout
    << "\nTest 4: Critical issue (escalates all the way to Director)\n";
    ChainOfResponsibility::SupportTicket critical (
    "Major security breach - company-wide response needed",
    ChainOfResponsibility::SupportTicket::Level::Critical);
    level1->handleTicket (critical);
}

void CommandExample () {
    std::cout << "\n--- Command Pattern Example ---\n";
    std::cout << "Setting up remote control with devices:\n";

    // Create receivers (devices)
    auto livingRoomLight = std::make_shared<Command::Light> ();
    auto kitchenLight    = std::make_shared<Command::Light> ();
    auto stereo          = std::make_shared<Command::Stereo> ();
    auto ceilingFan      = std::make_shared<Command::CeilingFan> ();

    // Create commands for each device
    auto livingRoomLightOn = std::make_shared<Command::LightOnCommand> (livingRoomLight);
    auto livingRoomLightOff = std::make_shared<Command::LightOffCommand> (livingRoomLight);

    auto kitchenLightOn = std::make_shared<Command::LightOnCommand> (kitchenLight);
    auto kitchenLightOff = std::make_shared<Command::LightOffCommand> (kitchenLight);

    auto stereoOnWithCD = std::make_shared<Command::StereoOnWithCDCommand> (stereo);
    auto stereoOff = std::make_shared<Command::StereoOffCommand> (stereo);

    auto fanHigh = std::make_shared<Command::CeilingFanHighCommand> (ceilingFan);
    auto fanOff = std::make_shared<Command::CeilingFanOffCommand> (ceilingFan);

    // Create macro command for party mode
    std::vector<std::shared_ptr<Command::Command>> partyCommands;
    partyCommands.push_back (livingRoomLightOn);
    partyCommands.push_back (stereoOnWithCD);
    partyCommands.push_back (fanHigh);
    auto partyMode = std::make_shared<Command::MacroCommand> (partyCommands);

    // Set up remote control
    Command::RemoteControl remote;

    // Slot 0: Living Room Light
    remote.setCommand (0, livingRoomLightOn, livingRoomLightOff);

    // Slot 1: Kitchen Light
    remote.setCommand (1, kitchenLightOn, kitchenLightOff);

    // Slot 2: Stereo
    remote.setCommand (2, stereoOnWithCD, stereoOff);

    // Slot 3: Ceiling Fan
    remote.setCommand (3, fanHigh, fanOff);

    // Slot 4: Party Mode (Macro Command)
    remote.setCommand (4, partyMode,
    std::make_shared<Command::NoCommand> ()); // No off command for party mode

    // Test the remote
    std::cout << "\n--- Testing Remote Control ---\n";
    remote.onButtonWasPressed (0);  // Turn on living room light
    remote.offButtonWasPressed (0); // Turn off living room light

    std::cout << "\n--- Testing Party Mode ---\n";
    remote.onButtonWasPressed (4); // Activate party mode
    std::cout << "\n--- Testing Undo ---\n";
    remote.undoButtonWasPressed (); // Undo party mode

    std::cout << "\n--- Testing Fan Speed Change with Undo ---\n";
    remote.onButtonWasPressed (3);  // Fan to HIGH
    remote.offButtonWasPressed (3); // Fan to OFF
    std::cout << "\nUndoing fan OFF:\n";
    remote.undoButtonWasPressed (); // Should go back to HIGH
}

void IteratorExample () {
    std::cout << "\n--- Iterator Pattern Example ---\n";

    // Example 1: Music Playlist
    std::cout
    << "\n1. Music Playlist Example (forward and backward traversal):\n\n";

    Iterator::Playlist<Iterator::Song> myPlaylist;
    myPlaylist.add (Iterator::Song ("Bohemian Rhapsody", "Queen"));
    myPlaylist.add (Iterator::Song ("Stairway to Heaven", "Led Zeppelin"));
    myPlaylist.add (Iterator::Song ("Hotel California", "Eagles"));
    myPlaylist.add (Iterator::Song ("Sweet Child O Mine", "Guns N' Roses"));

    std::cout << "Playing songs forward:\n";
    auto forwardIterator = myPlaylist.createIterator ();
    while (forwardIterator->hasNext ()) {
        auto song = forwardIterator->next ();
        std::cout << "  Now playing: " << song.toString () << "\n";
    }

    std::cout << "\nPlaying songs in reverse:\n";
    auto reverseIterator = myPlaylist.createReverseIterator ();
    while (reverseIterator->hasNext ()) {
        auto song = reverseIterator->next ();
        std::cout << "  Now playing: " << song.toString () << "\n";
    }

    // Example 2: Restaurant Menus
    std::cout << "\n2. Restaurant Menus Example (uniform traversal of "
                 "different menus):\n\n";

    Iterator::PancakeHouseMenu pancakeMenu;
    Iterator::DinerMenu dinerMenu;

    std::cout << "--- Pancake House Menu ---\n";
    auto pancakeIterator = pancakeMenu.createIterator ();
    while (pancakeIterator->hasNext ()) {
        auto item = pancakeIterator->next ();
        std::cout << "  " << item->getName () << " -- $" << item->getPrice () << "\n";
        std::cout << "    " << item->getDescription () << "\n";
    }

    std::cout << "\n--- Diner Menu ---\n";
    auto dinerIterator = dinerMenu.createIterator ();
    while (dinerIterator->hasNext ()) {
        auto item = dinerIterator->next ();
        std::cout << "  " << item->getName () << " -- $" << item->getPrice () << "\n";
        std::cout << "    " << item->getDescription () << "\n";
    }
}