/*
The intent is to avoid coupling the sender of a request to its receiver by giving
more than one object a chance to handle the request. Chain the receiving objects
and pass the request along the chain until an object handles it.

Key participants:
- Handler: Defines an interface for handling requests and implements successor link
- ConcreteHandler: Handles requests it is responsible for; accesses its successor
- Client: Initiates the request

Real-world example: A technical support ticket system where tickets are escalated
through different levels (Level 1 support -> Level 2 -> Manager -> Director).
*/
#ifndef C1_CHAIN_OF_RESPONSIBILITY_H
#define C1_CHAIN_OF_RESPONSIBILITY_H

#include <iostream>
#include <memory>
#include <string>

namespace ChainOfResponsibility {

// ----- Request -----
class SupportTicket {
    public:
    enum class Level { Basic, Intermediate, Advanced, Critical };

    SupportTicket (const std::string& description, Level level)
    : description_ (description), level_ (level) {
    }

    std::string getDescription () const {
        return description_;
    }

    Level getLevel () const {
        return level_;
    }

    std::string getLevelString () const {
        switch (level_) {
        case Level::Basic: return "Basic";
        case Level::Intermediate: return "Intermediate";
        case Level::Advanced: return "Advanced";
        case Level::Critical: return "Critical";
        }
        return "Unknown";
    }

    private:
    std::string description_;
    Level level_;
};

// ----- Handler interface -----
class SupportHandler {
    public:
    virtual ~SupportHandler () = default;

    void setNext (std::shared_ptr<SupportHandler> next) {
        next_ = next;
    }

    virtual void handleTicket (const SupportTicket& ticket) {
        if (next_) {
            next_->handleTicket (ticket);
        } else {
            std::cout << "No handler available for this ticket.\n";
        }
    }

    protected:
    virtual bool canHandle (const SupportTicket& ticket) const = 0;
    virtual void doHandle (const SupportTicket& ticket)        = 0;

    std::shared_ptr<SupportHandler> next_;
};

// ----- Concrete Handlers -----
class Level1Support : public SupportHandler {
    protected:
    bool canHandle (const SupportTicket& ticket) const override {
        return ticket.getLevel () == SupportTicket::Level::Basic;
    }

    void doHandle (const SupportTicket& ticket) override {
        std::cout << "[Level 1 Support] Handling ticket: " << ticket.getDescription ()
                  << " (Level: " << ticket.getLevelString () << ")\n";
        std::cout << "  Issue resolved with basic troubleshooting.\n";
    }

    public:
    void handleTicket (const SupportTicket& ticket) override {
        if (canHandle (ticket)) {
            doHandle (ticket);
        } else {
            std::cout << "[Level 1 Support] Cannot handle. Escalating...\n";
            SupportHandler::handleTicket (ticket);
        }
    }
};

class Level2Support : public SupportHandler {
    protected:
    bool canHandle (const SupportTicket& ticket) const override {
        return ticket.getLevel () == SupportTicket::Level::Intermediate;
    }

    void doHandle (const SupportTicket& ticket) override {
        std::cout << "[Level 2 Support] Handling ticket: " << ticket.getDescription ()
                  << " (Level: " << ticket.getLevelString () << ")\n";
        std::cout << "  Issue resolved with technical expertise.\n";
    }

    public:
    void handleTicket (const SupportTicket& ticket) override {
        if (canHandle (ticket)) {
            doHandle (ticket);
        } else {
            std::cout << "[Level 2 Support] Cannot handle. Escalating...\n";
            SupportHandler::handleTicket (ticket);
        }
    }
};

class Manager : public SupportHandler {
    protected:
    bool canHandle (const SupportTicket& ticket) const override {
        return ticket.getLevel () == SupportTicket::Level::Advanced;
    }

    void doHandle (const SupportTicket& ticket) override {
        std::cout << "[Manager] Handling ticket: " << ticket.getDescription ()
                  << " (Level: " << ticket.getLevelString () << ")\n";
        std::cout << "  Issue resolved with management authority.\n";
    }

    public:
    void handleTicket (const SupportTicket& ticket) override {
        if (canHandle (ticket)) {
            doHandle (ticket);
        } else {
            std::cout << "[Manager] Cannot handle. Escalating...\n";
            SupportHandler::handleTicket (ticket);
        }
    }
};

class Director : public SupportHandler {
    protected:
    bool canHandle (const SupportTicket& ticket) const override {
        return ticket.getLevel () == SupportTicket::Level::Critical;
    }

    void doHandle (const SupportTicket& ticket) override {
        std::cout << "[Director] Handling ticket: " << ticket.getDescription ()
                  << " (Level: " << ticket.getLevelString () << ")\n";
        std::cout
        << "  Issue resolved at highest level. Major decisions made.\n";
    }

    public:
    void handleTicket (const SupportTicket& ticket) override {
        if (canHandle (ticket)) {
            doHandle (ticket);
        } else {
            std::cout << "[Director] Cannot handle this ticket!\n";
            SupportHandler::handleTicket (ticket);
        }
    }
};

} // namespace ChainOfResponsibility

#endif
