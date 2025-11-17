#include <iostream>
#include <string>

#include "A1FactoryMethod.h"
#include "A2AbstractFactory.h"
#include "A3Builder.h"

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

void BuilderExample () {
    Builder::HttpRequest req = Builder::HttpRequestBuilder ()
                               .method ("POST")
                               .url ("https://example.com/api")
                               .body ("{\"id\":42}")
                               .timeout (5000)
                               .build ();

    std::cout << req.method << " " << req.url << " timeout=" << req.timeoutMs << "ms\n";
}

void simple_log (std::string_view log) {
    std::cout << "-----" << log << "-----" << std::endl;
}

int main () {
    simple_log ("Factory Method");
    FactoryMethodExample ();
    simple_log ("Abstract Factory");
    AbstractFactoryExample ();
    simple_log ("Builder");
    BuilderExample ();
    return 0;
}