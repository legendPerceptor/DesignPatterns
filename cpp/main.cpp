#include "A1FactoryMethod.h"
#include "A2AbstractFactory.h"

void FactoryMethodExample (bool useSpreadSheet = false) {
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

void AbstractFactoryExample (bool isWindows = false) {
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

int main () {
    FactoryMethodExample ();
    AbstractFactoryExample ();
    return 0;
}