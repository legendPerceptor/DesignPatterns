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