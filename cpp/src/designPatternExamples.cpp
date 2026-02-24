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

void InterpreterExample () {
    std::cout << "\n--- Interpreter Pattern Example ---\n";
    std::cout << "\n1. Mathematical Expression Interpreter:\n\n";

    Interpreter::Context context;

    // Example: Evaluate "5 + 3 - 2"
    std::cout << "Expression: 5 + 3 - 2\n";
    auto expr1 = std::make_shared<Interpreter::AddExpression> (
    std::make_shared<Interpreter::NumberExpression> (5),
    std::make_shared<Interpreter::NumberExpression> (3));
    auto expr2 = std::make_shared<Interpreter::SubtractExpression> (
    expr1, std::make_shared<Interpreter::NumberExpression> (2));
    std::cout << "Result: " << expr2->interpret (context) << "\n\n";

    // Example: Evaluate "10 * 2 / 5"
    std::cout << "Expression: 10 * 2 / 5\n";
    auto expr3 = std::make_shared<Interpreter::MultiplyExpression> (
    std::make_shared<Interpreter::NumberExpression> (10),
    std::make_shared<Interpreter::NumberExpression> (2));
    auto expr4 = std::make_shared<Interpreter::DivideExpression> (
    expr3, std::make_shared<Interpreter::NumberExpression> (5));
    std::cout << "Result: " << expr4->interpret (context) << "\n\n";

    // Example with variables
    std::cout << "\n2. Expression with Variables:\n";
    context.setVariable ("x", 10);
    context.setVariable ("y", 5);
    std::cout << "Expression: x * y - 15 (where x=10, y=5)\n";
    auto varX  = std::make_shared<Interpreter::VariableExpression> ("x");
    auto varY  = std::make_shared<Interpreter::VariableExpression> ("y");
    auto expr5 = std::make_shared<Interpreter::MultiplyExpression> (varX, varY);
    auto expr6 = std::make_shared<Interpreter::SubtractExpression> (
    expr5, std::make_shared<Interpreter::NumberExpression> (15));
    std::cout << "Result: " << expr6->interpret (context) << "\n\n";

    // Example 3: Boolean expressions
    std::cout << "\n3. Boolean Expression Interpreter:\n\n";
    context.setVariable ("is_raining", 1);
    context.setVariable ("has_umbrella", 1);

    auto raining   = std::make_shared<Interpreter::Variable> ("is_raining");
    auto umbrella  = std::make_shared<Interpreter::Variable> ("has_umbrella");
    auto goOutside = std::make_shared<Interpreter::Or> (
    raining, std::make_shared<Interpreter::Not> (umbrella));

    std::cout << "Expression: is_raining AND NOT has_umbrella\n";
    std::cout << "  (Should we stay inside?)\n";
    bool stayInside = std::make_shared<Interpreter::And> (
    raining, std::make_shared<Interpreter::Not> (umbrella))
                      ->evaluate (context);
    std::cout << "Result: " << (stayInside ? "true (stay inside)" : "false (go outside)")
              << "\n\n";
}

void TemplateMethodExample () {
    std::cout << "\n--- Template Method Pattern Example ---\n";

    // Example 1: Data Processing
    std::cout << "\n1. Data Processing Example:\n\n";

    TemplateMethod::TextTransformationProcessor textProcessor;
    std::cout << "\n--- Processing Text Data ---\n";
    textProcessor.processData ("Hello Design Patterns!");

    std::cout << "\n--- Processing Number Data ---\n";
    TemplateMethod::NumberCalculationProcessor numberProcessor;
    numberProcessor.processData ("10, 20, 30, 40, 50");

    // Example 2: Caffeine Beverages
    std::cout << "\n2. Caffeine Beverage Example:\n\n";

    std::cout << "--- Making Coffee ---\n";
    TemplateMethod::Coffee coffee;
    coffee.prepareRecipe ();

    std::cout << "\n--- Making Tea ---\n";
    TemplateMethod::Tea tea;
    tea.prepareRecipe ();

    // Example 3: Game AI
    std::cout << "\n3. Game AI Example:\n\n";

    std::cout << "--- Aggressive AI Turn ---\n";
    TemplateMethod::AggressiveAI aggressiveAI;
    aggressiveAI.takeTurn ();

    std::cout << "\n--- Defensive AI Turn ---\n";
    TemplateMethod::DefensiveAI defensiveAI;
    defensiveAI.takeTurn ();
}

void MediatorExample () {
    std::cout << "\n--- Mediator Pattern Example ---\n";
    std::cout << "Air Traffic Control System\n";

    auto controlTower = std::make_shared<Mediator::ControlTower> ();

    // Register aircraft
    auto plane1 = std::make_shared<Mediator::Aircraft> ("Flight 101", controlTower);
    auto plane2 = std::make_shared<Mediator::Aircraft> ("Flight 202", controlTower);
    auto plane3 = std::make_shared<Mediator::Aircraft> ("Flight 303", controlTower);

    controlTower->registerAircraft (plane1);
    controlTower->registerAircraft (plane2);
    controlTower->registerAircraft (plane3);

    std::cout << "\n--- Scenario 1: Plane 1 takes off ---\n";
    plane1->takeoff ();

    std::cout << "\n--- Scenario 2: Plane 2 requests landing ---\n";
    plane2->land ();

    std::cout
    << "\n--- Scenario 3: Plane 3 tries to take off while runway busy ---\n";
    plane3->takeoff ();

    std::cout << "\n--- Scenario 4: Plane 1 lands ---\n";
    plane1->land ();

    std::cout << "\n--- Scenario 5: Broadcasting message ---\n";
    plane1->sendMessage ("Weather alert: Storm approaching");
}

void MementoExample () {
    std::cout << "\n--- Memento Pattern Example ---\n";

    // Example 1: Text Editor
    std::cout << "\n1. Text Editor with Undo/Redo:\n\n";

    Memento::TextDocument doc;
    Memento::History history;

    // Initial state
    history.setInitialState (doc.save ());

    std::cout << "--- Writing text ---\n";
    doc.write ("Hello");
    history.saveState (doc.save ());

    doc.write (" World");
    history.saveState (doc.save ());

    doc.write ("!");
    history.saveState (doc.save ());

    std::cout << "\n--- Performing Undo ---\n";
    doc.restore (history.undo ());

    std::cout << "\n--- Performing Another Undo ---\n";
    doc.restore (history.undo ());

    std::cout << "\n--- Performing Redo ---\n";
    doc.restore (history.redo ());

    // Example 2: Game Character
    std::cout << "\n\n2. Game Character Checkpoints:\n\n";

    Memento::GameCharacter hero ("Hero");

    std::cout << "--- Initial state ---\n";
    std::cout << "Health: 100, Position: (0, 0)\n";

    std::cout << "\n--- Creating checkpoint 1 ---\n";
    auto checkpoint1 = hero.createCheckpoint ();

    std::cout << "\n--- Taking damage and moving ---\n";
    hero.takeDamage (30);
    hero.move (10, 5);

    std::cout << "\n--- Creating checkpoint 2 ---\n";
    auto checkpoint2 = hero.createCheckpoint ();

    std::cout << "\n--- More damage and movement ---\n";
    hero.takeDamage (50);
    hero.move (20, 10);

    std::cout << "\n--- Restoring to checkpoint 2 ---\n";
    hero.restoreFromCheckpoint (checkpoint2);

    std::cout << "\n--- Restoring to checkpoint 1 ---\n";
    hero.restoreFromCheckpoint (checkpoint1);
}

void ObserverExample () {
    std::cout << "\n--- Observer Pattern Example ---\n";

    // Example 1: Weather Station
    std::cout << "\n1. Weather Station:\n\n";

    Observer::WeatherStation weatherStation;

    auto phone1          = std::make_shared<Observer::PhoneDisplay> ("Alice");
    auto phone2          = std::make_shared<Observer::PhoneDisplay> ("Bob");
    auto webDisplay      = std::make_shared<Observer::WebDisplay> ();
    auto forecastDisplay = std::make_shared<Observer::ForecastDisplay> ();

    std::cout << "--- Attaching observers ---\n";
    weatherStation.attach (phone1);
    weatherStation.attach (phone2);
    weatherStation.attach (webDisplay);
    weatherStation.attach (forecastDisplay);

    std::cout << "\n--- First weather update ---\n";
    weatherStation.setMeasurements (25.5, 65.0, 1013.0);

    std::cout << "\n--- Second weather update ---\n";
    weatherStation.setMeasurements (28.0, 70.0, 1008.0);

    std::cout << "\n--- Detaching Bob's phone ---\n";
    weatherStation.detach (phone2);

    std::cout << "\n--- Third weather update ---\n";
    weatherStation.setMeasurements (22.0, 55.0, 1020.0);

    // Example 2: YouTube Channel
    std::cout << "\n\n2. YouTube Channel:\n\n";

    Observer::YouTubeChannel myChannel ("Code Academy");

    auto sub1 = std::make_shared<Observer::Subscriber> ("Alice");
    auto sub2 = std::make_shared<Observer::Subscriber> ("Bob");
    auto sub3 = std::make_shared<Observer::Subscriber> ("Charlie");

    std::cout << "--- Subscribing to channel ---\n";
    myChannel.attach (sub1);
    myChannel.attach (sub2);
    myChannel.attach (sub3);

    std::cout << "\n--- Uploading new video ---\n";
    myChannel.uploadVideo ("Design Patterns in C++");
}

void StateExample () {
    std::cout << "\n--- State Pattern Example ---\n";

    // Example 1: Vending Machine
    std::cout << "\n1. Vending Machine:\n\n";

    State::VendingMachine vendingMachine (3);

    std::cout << "--- Initial state ---\n";
    std::cout << "State: " << vendingMachine.getStateName () << "\n";
    std::cout << "Items: " << vendingMachine.getItemCount () << "\n";

    std::cout << "\n--- Try to press button without coin ---\n";
    vendingMachine.pressButton ();

    std::cout << "\n--- Insert coin ---\n";
    vendingMachine.insertCoin ();

    std::cout << "\n--- Press button ---\n";
    vendingMachine.pressButton ();

    std::cout << "\n--- Insert coin and buy again ---\n";
    vendingMachine.insertCoin ();
    vendingMachine.pressButton ();

    std::cout << "\n--- Insert coin and buy again (last item) ---\n";
    vendingMachine.insertCoin ();
    vendingMachine.pressButton ();

    std::cout << "\n--- Try to insert coin when out of stock ---\n";
    vendingMachine.insertCoin ();

    // Example 2: Document Workflow
    std::cout << "\n\n2. Document Workflow:\n\n";

    State::Document document;

    std::cout << "--- Editing in draft ---\n";
    document.edit ("Initial content");

    std::cout << "\n--- Submit for moderation ---\n";
    document.publish ();

    std::cout << "\n--- Try to edit during moderation ---\n";
    document.edit ("Trying to edit");

    std::cout << "\n--- Reject document ---\n";
    document.reject ();

    std::cout << "\n--- Edit and submit again ---\n";
    document.edit ("Revised content");
    document.publish ();

    std::cout << "\n--- Approve document ---\n";
    document.publish ();

    std::cout << "\n--- Try to edit published document ---\n";
    document.edit ("New changes");

    // Example 3: Order Processing
    std::cout << "\n\n3. Order Processing:\n\n";

    State::Order order ("ORD-12345");

    std::cout << "--- Process order ---\n";
    order.process ();

    std::cout << "\n--- Ship order ---\n";
    order.ship ();

    std::cout << "\n--- Deliver order ---\n";
    order.deliver ();

    std::cout << "\n--- Try to cancel delivered order ---\n";
    order.cancel ();

    std::cout << "\n\n--- New order cancellation ---\n";
    State::Order order2 ("ORD-67890");
    order2.cancel ();
}

void StrategyExample () {
    std::cout << "\n--- Strategy Pattern Example ---\n";

    // Example 1: Payment Processing
    std::cout << "\n1. Payment Processing:\n\n";

    Strategy::ShoppingCart cart;

    std::cout << "--- Paying with Credit Card ---\n";
    cart.setPaymentStrategy (std::make_shared<Strategy::CreditCardPayment> (
    "4111-1111-1111-1111", "John Doe"));
    cart.checkout (99.99);

    std::cout << "\n--- Paying with PayPal ---\n";
    cart.setPaymentStrategy (
    std::make_shared<Strategy::PayPalPayment> ("john.doe@email.com"));
    cart.checkout (49.99);

    std::cout << "\n--- Paying with Apple Pay ---\n";
    cart.setPaymentStrategy (
    std::make_shared<Strategy::ApplePayPayment> ("device-id-12345"));
    cart.checkout (29.99);

    std::cout << "\n--- Paying with Crypto ---\n";
    cart.setPaymentStrategy (
    std::make_shared<Strategy::CryptoPayment> ("0x1234...abcd"));
    cart.checkout (199.99);

    // Example 2: File Compression
    std::cout << "\n\n2. File Compression:\n\n";

    Strategy::FileCompressor compressor;

    compressor.setCompressionStrategy (std::make_shared<Strategy::ZipCompression> ());
    compressor.compressFile ("document.txt");

    compressor.setCompressionStrategy (std::make_shared<Strategy::RarCompression> ());
    compressor.compressFile ("photos.zip");

    compressor.setCompressionStrategy (std::make_shared<Strategy::SevenZipCompression> ());
    compressor.compressFile ("backup");

    // Example 3: Navigation
    std::cout << "\n\n3. Navigation System:\n\n";

    Strategy::Location start ("Home", 40.7128, -74.0060);
    Strategy::Location end ("Work", 40.7589, -73.9851);

    Strategy::NavigationSystem nav ("User");

    nav.setRouteStrategy (std::make_shared<Strategy::FastestRouteStrategy> ());
    nav.navigate (start, end);

    nav.setRouteStrategy (std::make_shared<Strategy::ShortestRouteStrategy> ());
    nav.navigate (start, end);

    nav.setRouteStrategy (std::make_shared<Strategy::AvoidHighwaysStrategy> ());
    nav.navigate (start, end);

    nav.setRouteStrategy (std::make_shared<Strategy::EcoFriendlyRouteStrategy> ());
    nav.navigate (start, end);
}

void VisitorExample () {
    std::cout << "\n--- Visitor Pattern Example ---\n";

    // Example 1: Shopping Cart with Different Visitors
    std::cout << "\n1. Shopping Cart with Multiple Visitors:\n\n";

    Visitor::ShoppingCart cart;

    // Add items to cart
    cart.addItem (std::make_shared<Visitor::Book> ("Design Patterns", "Erich Gamma", 49.99));
    cart.addItem (std::make_shared<Visitor::Book> ("Clean Code", "Robert Martin", 39.99));
    cart.addItem (std::make_shared<Visitor::Fruit> ("Apple", 2.99, 1.5));
    cart.addItem (std::make_shared<Visitor::Fruit> ("Banana", 1.99, 2.0));
    cart.addItem (std::make_shared<Visitor::Electronics> ("iPhone 15", 999.99, true));
    cart.addItem (std::make_shared<Visitor::Electronics> ("Laptop", 1299.99, true));

    // Apply different visitors
    std::cout << "\n=== Calculating Taxes ===\n";
    Visitor::TaxVisitor taxVisitor;
    cart.accept (taxVisitor);

    std::cout << "\n=== Applying Discounts ===\n";
    Visitor::DiscountVisitor discountVisitor;
    cart.accept (discountVisitor);

    std::cout << "\n=== Counting Inventory ===\n";
    Visitor::InventoryVisitor inventoryVisitor;
    cart.accept (inventoryVisitor);
    inventoryVisitor.printSummary ();

    // Example 2: File System Visitor
    std::cout << "\n\n2. File System Visitor:\n\n";

    // Create file system structure
    auto readme = std::make_shared<Visitor::File> ("README.md", 1024);
    auto main   = std::make_shared<Visitor::File> ("main.cpp", 4096);
    auto utils  = std::make_shared<Visitor::File> ("utils.cpp", 2048);

    auto srcDir = std::make_shared<Visitor::Directory> ("src");
    srcDir->addNode (main);
    srcDir->addNode (utils);

    auto rootDir = std::make_shared<Visitor::Directory> ("project");
    rootDir->addNode (readme);
    rootDir->addNode (srcDir);

    std::cout << "--- Calculating Sizes ---\n";
    Visitor::SizeCalculatorVisitor sizeCalc;
    rootDir->accept (sizeCalc);
    std::cout << "\nTotal directory size: " << sizeCalc.getTotalSize () << " bytes\n";

    std::cout << "\n--- Exporting to XML ---\n";
    Visitor::XmlExporterVisitor xmlExporter;
    rootDir->accept (xmlExporter);
    std::cout << xmlExporter.getXmlOutput () << "</filesystem>\n";
}