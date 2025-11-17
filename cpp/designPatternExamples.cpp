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