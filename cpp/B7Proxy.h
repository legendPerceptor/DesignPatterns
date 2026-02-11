/*
The intent is to provide a surrogate or placeholder for another object to control access to it.

Types of proxies:
1. Virtual Proxy - Delays the creation and initialization of expensive objects
2. Protection Proxy - Controls access to an object based on access rights
3. Remote Proxy - Represents an object in a different address space
4. Smart Reference - Adds actions when an object is accessed (e.g., reference counting)

Key benefits:
- Lazy initialization (create objects only when needed)
- Access control
- Hiding complexity
- Performance optimization

Real-world example: Loading large images on demand. Instead of loading all images
when a document opens, use a proxy that only loads the image when it's viewed.
*/
#ifndef B7_PROXY_H
#define B7_PROXY_H

#include <iostream>
#include <memory>
#include <string>

namespace Proxy {

// ----- Subject interface -----
// Common interface for RealSubject and Proxy
class Image {
    public:
    virtual ~Image ()                        = default;
    virtual void display ()                  = 0;
    virtual std::string getFilename () const = 0;
};

// ----- RealSubject -----
// The actual object that the proxy represents
class RealImage : public Image {
    public:
    explicit RealImage (const std::string& filename) : filename_ (filename) {
        loadImageFromDisk ();
    }

    void display () override {
        std::cout << "Displaying image: " << filename_ << "\n";
    }

    std::string getFilename () const override {
        return filename_;
    }

    private:
    void loadImageFromDisk () {
        std::cout << "Loading " << filename_ << " from disk...\n";
        // Simulate expensive loading operation
    }

    std::string filename_;
};

// ----- Proxy -----
// Acts as a surrogate, controlling access to the RealImage
class ProxyImage : public Image {
    public:
    explicit ProxyImage (const std::string& filename)
    : filename_ (filename), realImage_ (nullptr) {
    }

    void display () override {
        // Lazy initialization: only load the image when first needed
        if (!realImage_) {
            std::cout << "[Proxy] Initializing real image on first access...\n";
            realImage_ = std::make_unique<RealImage> (filename_);
        }
        realImage_->display ();
    }

    std::string getFilename () const override {
        return filename_;
    }

    private:
    std::string filename_;
    std::unique_ptr<RealImage> realImage_;
};

// ----- Protection Proxy Example -----
// Controls access based on user permissions

class Internet {
    public:
    virtual ~Internet ()                                   = default;
    virtual void connectTo (const std::string& serverhost) = 0;
};

class RealInternet : public Internet {
    public:
    void connectTo (const std::string& serverhost) override {
        std::cout << "Connecting to " << serverhost << "\n";
    }
};

class ProxyInternet : public Internet {
    public:
    ProxyInternet () : internet_ (std::make_unique<RealInternet> ()) {
    }

    void connectTo (const std::string& serverhost) override {
        // Check if the website is allowed
        if (tryConnect (serverhost)) {
            internet_->connectTo (serverhost);
        } else {
            std::cout << "Access Denied: " << serverhost << " is blocked!\n";
        }
    }

    private:
    bool tryConnect (const std::string& serverhost) {
        // Simulate access control - block certain sites
        if (serverhost.find ("blocked.com") != std::string::npos) {
            return false;
        }
        return true;
    }

    std::unique_ptr<RealInternet> internet_;
};

// ----- Smart Reference Example -----
// A proxy that adds reference counting behavior

template <typename T> class SmartPtr {
    public:
    explicit SmartPtr (T* ptr = nullptr) : ptr_ (ptr), refCount_ (new int (1)) {
        if (ptr_) {
            std::cout << "[SmartPtr] Created, ref count = 1\n";
        }
    }

    SmartPtr (const SmartPtr& other)
    : ptr_ (other.ptr_), refCount_ (other.refCount_) {
        (*refCount_)++;
        std::cout << "[SmartPtr] Copied, ref count = " << *refCount_ << "\n";
    }

    ~SmartPtr () {
        (*refCount_)--;
        std::cout << "[SmartPtr] Destroyed, ref count = " << *refCount_ << "\n";
        if (*refCount_ == 0) {
            delete ptr_;
            delete refCount_;
            std::cout << "[SmartPtr] Last reference deleted, cleaning up\n";
        }
    }

    SmartPtr& operator= (const SmartPtr& other) {
        if (this != &other) {
            (*refCount_)--;
            if (*refCount_ == 0) {
                delete ptr_;
                delete refCount_;
            }
            ptr_      = other.ptr_;
            refCount_ = other.refCount_;
            (*refCount_)++;
            std::cout << "[SmartPtr] Assigned, ref count = " << *refCount_ << "\n";
        }
        return *this;
    }

    T& operator* () const {
        return *ptr_;
    }

    T* operator->() const {
        return ptr_;
    }

    private:
    T* ptr_;
    int* refCount_; // Shared reference count
};

class Resource {
    public:
    Resource () {
        std::cout << "[Resource] Constructor called\n";
    }

    ~Resource () {
        std::cout << "[Resource] Destructor called\n";
    }

    void doSomething () {
        std::cout << "[Resource] Doing something...\n";
    }
};

} // namespace Proxy

#endif
