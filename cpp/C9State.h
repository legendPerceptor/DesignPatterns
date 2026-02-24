/*
The intent is to allow an object to alter its behavior when its internal state
changes. The object will appear to change its class.

Key participants:
- Context: Defines the interface of interest to clients; maintains an instance
of a ConcreteState subclass that defines the current state
- State: Defines an interface for encapsulating the behavior associated with a
  particular state of the Context
- ConcreteState: Each subclass implements a behavior associated with a state of
  the Context

Key benefits:
- Localizes state-specific behavior and partitions behavior for different states
- Makes state transitions explicit
- State objects can be shared

Real-world example: A vending machine that goes through different states
(NoCoin, HasCoin, Sold, OutOfStock) and behaves differently in each state.
*/
#ifndef C9_STATE_H
#define C9_STATE_H

#include <iostream>
#include <memory>
#include <string>

namespace State {

// ----- Forward declaration -----
class VendingMachine;

// ----- State interface -----
class State {
    public:
    virtual ~State () = default;

    virtual void insertCoin ()           = 0;
    virtual void ejectCoin ()            = 0;
    virtual void pressButton ()          = 0;
    virtual void dispense ()             = 0;
    virtual std::string getName () const = 0;
};

// ----- Concrete States -----
class NoCoinState : public State {
    public:
    explicit NoCoinState (VendingMachine* machine) : machine_ (machine) {
    }

    void insertCoin () override;
    void ejectCoin () override;
    void pressButton () override;
    void dispense () override;
    std::string getName () const override {
        return "No Coin";
    }

    private:
    VendingMachine* machine_;
};

class HasCoinState : public State {
    public:
    explicit HasCoinState (VendingMachine* machine) : machine_ (machine) {
    }

    void insertCoin () override;
    void ejectCoin () override;
    void pressButton () override;
    void dispense () override;
    std::string getName () const override {
        return "Has Coin";
    }

    private:
    VendingMachine* machine_;
};

class SoldState : public State {
    public:
    explicit SoldState (VendingMachine* machine) : machine_ (machine) {
    }

    void insertCoin () override;
    void ejectCoin () override;
    void pressButton () override;
    void dispense () override;
    std::string getName () const override {
        return "Sold";
    }

    private:
    VendingMachine* machine_;
};

class OutOfStockState : public State {
    public:
    explicit OutOfStockState (VendingMachine* machine) : machine_ (machine) {
    }

    void insertCoin () override;
    void ejectCoin () override;
    void pressButton () override;
    void dispense () override;
    std::string getName () const override {
        return "Out of Stock";
    }

    private:
    VendingMachine* machine_;
};

// ----- Context -----
class VendingMachine {
    public:
    VendingMachine (int itemCount) : itemCount_ (itemCount) {
        noCoinState_     = std::make_unique<NoCoinState> (this);
        hasCoinState_    = std::make_unique<HasCoinState> (this);
        soldState_       = std::make_unique<SoldState> (this);
        outOfStockState_ = std::make_unique<OutOfStockState> (this);

        if (itemCount_ > 0) {
            currentState_ = noCoinState_.get ();
        } else {
            currentState_ = outOfStockState_.get ();
        }
    }

    void insertCoin () {
        currentState_->insertCoin ();
    }

    void ejectCoin () {
        currentState_->ejectCoin ();
    }

    void pressButton () {
        currentState_->pressButton ();
        currentState_->dispense ();
    }

    void setState (State* state) {
        currentState_ = state;
    }

    void releaseProduct () {
        std::cout << "[Vending Machine] Dispensing product...\n";
        if (itemCount_ > 0) {
            itemCount_--;
            std::cout << "[Vending Machine] Product dispensed! Remaining: " << itemCount_
                      << "\n";
        }
    }

    int getItemCount () const {
        return itemCount_;
    }

    bool hasStock () const {
        return itemCount_ > 0;
    }

    State* getNoCoinState () const {
        return noCoinState_.get ();
    }

    State* getHasCoinState () const {
        return hasCoinState_.get ();
    }

    State* getSoldState () const {
        return soldState_.get ();
    }

    State* getOutOfStockState () const {
        return outOfStockState_.get ();
    }

    std::string getStateName () const {
        return currentState_->getName ();
    }

    private:
    std::unique_ptr<NoCoinState> noCoinState_;
    std::unique_ptr<HasCoinState> hasCoinState_;
    std::unique_ptr<SoldState> soldState_;
    std::unique_ptr<OutOfStockState> outOfStockState_;
    State* currentState_;
    int itemCount_;
};

// ----- Example 2: Document Editor with States -----
class Document;

class DocumentState {
    public:
    virtual ~DocumentState ()            = default;
    virtual void publish (Document* doc) = 0;
    virtual void reject (Document* doc)  = 0;
    virtual void edit (Document* doc)    = 0;
    virtual std::string getName () const = 0;
};

class DraftState : public DocumentState {
    public:
    void publish (Document* doc) override;
    void reject (Document* doc) override;
    void edit (Document* doc) override;
    std::string getName () const override {
        return "Draft";
    }
};

class ModerationState : public DocumentState {
    public:
    void publish (Document* doc) override;
    void reject (Document* doc) override;
    void edit (Document* doc) override;
    std::string getName () const override {
        return "Under Moderation";
    }
};

class PublishedState : public DocumentState {
    public:
    void publish (Document* doc) override;
    void reject (Document* doc) override;
    void edit (Document* doc) override;
    std::string getName () const override {
        return "Published";
    }
};

class Document {
    public:
    Document () : state_ (nullptr), title_ (""), content_ ("") {
        setState (std::unique_ptr<DocumentState> (new DraftState ()));
    }

    void setState (std::unique_ptr<DocumentState> state) {
        state_ = std::move (state);
        std::cout << "[Document] State changed to: " << state_->getName () << "\n";
    }

    void publish () {
        state_->publish (this);
    }

    void reject () {
        state_->reject (this);
    }

    void edit (const std::string& content) {
        content_ = content;
        state_->edit (this);
    }

    std::string getContent () const {
        return content_;
    }

    std::string getStateName () const {
        return state_->getName ();
    }

    private:
    std::unique_ptr<DocumentState> state_;
    std::string title_;
    std::string content_;
};

// ----- Example 3: Order Processing States -----
class Order;

class OrderState {
    public:
    virtual ~OrderState ()               = default;
    virtual void process (Order* order)  = 0;
    virtual void ship (Order* order)     = 0;
    virtual void deliver (Order* order)  = 0;
    virtual void cancel (Order* order)   = 0;
    virtual std::string getName () const = 0;
};

class NewOrderState : public OrderState {
    public:
    void process (Order* order) override;
    void ship (Order* order) override;
    void deliver (Order* order) override;
    void cancel (Order* order) override;
    std::string getName () const override {
        return "New";
    }
};

class ProcessingState : public OrderState {
    public:
    void process (Order* order) override;
    void ship (Order* order) override;
    void deliver (Order* order) override;
    void cancel (Order* order) override;
    std::string getName () const override {
        return "Processing";
    }
};

class ShippedState : public OrderState {
    public:
    void process (Order* order) override;
    void ship (Order* order) override;
    void deliver (Order* order) override;
    void cancel (Order* order) override;
    std::string getName () const override {
        return "Shipped";
    }
};

class DeliveredState : public OrderState {
    public:
    void process (Order* order) override;
    void ship (Order* order) override;
    void deliver (Order* order) override;
    void cancel (Order* order) override;
    std::string getName () const override {
        return "Delivered";
    }
};

class CancelledState : public OrderState {
    public:
    void process (Order* order) override;
    void ship (Order* order) override;
    void deliver (Order* order) override;
    void cancel (Order* order) override;
    std::string getName () const override {
        return "Cancelled";
    }
};

class Order {
    public:
    Order (const std::string& orderId) : orderId_ (orderId) {
        setState (std::unique_ptr<OrderState> (new NewOrderState ()));
    }

    void setState (std::unique_ptr<OrderState> state) {
        state_ = std::move (state);
        std::cout << "[Order " << orderId_ << "] Status: " << state_->getName () << "\n";
    }

    void process () {
        state_->process (this);
    }

    void ship () {
        state_->ship (this);
    }

    void deliver () {
        state_->deliver (this);
    }

    void cancel () {
        state_->cancel (this);
    }

    std::string getStateName () const {
        return state_->getName ();
    }

    private:
    std::unique_ptr<OrderState> state_;
    std::string orderId_;
};

} // namespace State

#endif
