/*
The intent is to separate the construction of a complex object from its representation
so that the same construction process can create different representations.

It is useful when your object has many optional/complex parameters.
You want to avoid telescoping constructors like
Thing(a) / Thing(a, b) / Thing(a, b, c, d, e, ...)
*/

#ifndef A3_BUILDER_H
#define A3_BUILDER_H

#include <string>
#include <utility>

// Forward declaration of builder

namespace Builder {

class HttpRequestBuilder;

class HttpRequest {
    public:
    std::string method;
    std::string url;
    std::string body;
    int timeoutMs = 1000;

    private:
    // Only the builder should construct this directly
    HttpRequest () = default;

    friend class HttpRequestBuilder;
};

// ----- Builder (separate class) -----
class HttpRequestBuilder {
    public:
    HttpRequestBuilder& method (const std::string& m) {
        req_.method = m;
        return *this;
    }

    HttpRequestBuilder& url (const std::string& u) {
        req_.url = u;
        return *this;
    }

    HttpRequestBuilder& body (const std::string& b) {
        req_.body = b;
        return *this;
    }

    HttpRequestBuilder& timeout (int ms) {
        req_.timeoutMs = ms;
        return *this;
    }

    HttpRequest build () {
        return std::move (req_);
    }

    private:
    HttpRequest req_;
};

} // namespace Builder

#endif