#include "C9State.h"

namespace State {

// ----- Concrete State Implementations -----
void NoCoinState::insertCoin () {
    std::cout
    << "[No Coin State] Coin inserted. You can now make a selection.\n";
    machine_->setState (machine_->getHasCoinState ());
}

void NoCoinState::ejectCoin () {
    std::cout
    << "[No Coin State] No coin to eject. Please insert a coin first.\n";
}

void NoCoinState::pressButton () {
    std::cout << "[No Coin State] Please insert a coin first.\n";
}

void NoCoinState::dispense () {
    std::cout << "[No Coin State] No coin inserted. Cannot dispense.\n";
}

void HasCoinState::insertCoin () {
    std::cout << "[Has Coin State] Coin already inserted. Eject first if you "
                 "want to insert another.\n";
}

void HasCoinState::ejectCoin () {
    std::cout << "[Has Coin State] Coin ejected. Returning to No Coin state.\n";
    machine_->setState (machine_->getNoCoinState ());
}

void HasCoinState::pressButton () {
    std::cout << "[Has Coin State] Button pressed. Dispensing product...\n";
    machine_->setState (machine_->getSoldState ());
}

void HasCoinState::dispense () {
    std::cout
    << "[Has Coin State] No product dispensed. Press button to dispense.\n";
}

void SoldState::insertCoin () {
    std::cout << "[Sold State] Please wait, dispensing current product.\n";
}

void SoldState::ejectCoin () {
    std::cout << "[Sold State] Cannot eject coin during transaction.\n";
}

void SoldState::pressButton () {
    std::cout << "[Sold State] Already processing. Please wait.\n";
}

void SoldState::dispense () {
    machine_->releaseProduct ();
    if (machine_->hasStock ()) {
        std::cout
        << "[Sold State] Transaction complete. Returning to No Coin state.\n";
        machine_->setState (machine_->getNoCoinState ());
    } else {
        std::cout
        << "[Sold State] Out of stock! Switching to Out Of Stock state.\n";
        machine_->setState (machine_->getOutOfStockState ());
    }
}

void OutOfStockState::insertCoin () {
    std::cout
    << "[Out of Stock State] Machine is out of stock. Coin returned.\n";
}

void OutOfStockState::ejectCoin () {
    std::cout << "[Out of Stock State] No coin to eject.\n";
}

void OutOfStockState::pressButton () {
    std::cout << "[Out of Stock State] Machine is out of stock. Please come "
                 "back later.\n";
}

void OutOfStockState::dispense () {
    std::cout << "[Out of Stock State] Cannot dispense. Machine is empty.\n";
}


void DraftState::publish (Document* doc) {
    std::cout << "[Draft State] Submitting document for moderation...\n";
    doc->setState (std::unique_ptr<DocumentState> (new ModerationState ()));
}

void DraftState::reject (Document* doc) {
    std::cout << "[Draft State] Document is already in draft. Cannot reject.\n";
}

void DraftState::edit (Document* doc) {
    std::cout << "[Draft State] Document edited successfully.\n";
}

void ModerationState::publish (Document* doc) {
    std::cout << "[Moderation State] Document approved and published!\n";
    doc->setState (std::unique_ptr<DocumentState> (new PublishedState ()));
}

void ModerationState::reject (Document* doc) {
    std::cout << "[Moderation State] Document rejected. Returning to draft.\n";
    doc->setState (std::unique_ptr<DocumentState> (new DraftState ()));
}

void ModerationState::edit (Document* doc) {
    std::cout
    << "[Moderation State] Cannot edit during moderation. Reject first.\n";
}

void PublishedState::publish (Document* doc) {
    std::cout << "[Published State] Document is already published.\n";
}

void PublishedState::reject (Document* doc) {
    std::cout << "[Published State] Cannot reject published document.\n";
}

void PublishedState::edit (Document* doc) {
    std::cout << "[Published State] Creating new draft with changes...\n";
    doc->setState (std::unique_ptr<DocumentState> (new DraftState ()));
}


void NewOrderState::process (Order* order) {
    std::cout << "[New Order] Processing payment and preparing order...\n";
    order->setState (std::unique_ptr<OrderState> (new ProcessingState ()));
}

void NewOrderState::ship (Order* order) {
    std::cout << "[New Order] Cannot ship unprocessed order.\n";
}

void NewOrderState::deliver (Order* order) {
    std::cout << "[New Order] Cannot deliver unprocessed order.\n";
}

void NewOrderState::cancel (Order* order) {
    std::cout << "[New Order] Order cancelled.\n";
    order->setState (std::unique_ptr<OrderState> (new CancelledState ()));
}

void ProcessingState::process (Order* order) {
    std::cout << "[Processing] Order already being processed.\n";
}

void ProcessingState::ship (Order* order) {
    std::cout << "[Processing] Order shipped!\n";
    order->setState (std::unique_ptr<OrderState> (new ShippedState ()));
}

void ProcessingState::deliver (Order* order) {
    std::cout << "[Processing] Order not shipped yet.\n";
}

void ProcessingState::cancel (Order* order) {
    std::cout << "[Processing] Order cancelled during processing.\n";
    order->setState (std::unique_ptr<OrderState> (new CancelledState ()));
}

void ShippedState::process (Order* order) {
    std::cout << "[Shipped] Order already processed and shipped.\n";
}

void ShippedState::ship (Order* order) {
    std::cout << "[Shipped] Order already shipped.\n";
}

void ShippedState::deliver (Order* order) {
    std::cout << "[Shipped] Order delivered successfully!\n";
    order->setState (std::unique_ptr<OrderState> (new DeliveredState ()));
}

void ShippedState::cancel (Order* order) {
    std::cout << "[Shipped] Cannot cancel shipped order.\n";
}

void DeliveredState::process (Order* order) {
    std::cout << "[Delivered] Order already delivered.\n";
}

void DeliveredState::ship (Order* order) {
    std::cout << "[Delivered] Order already delivered.\n";
}

void DeliveredState::deliver (Order* order) {
    std::cout << "[Delivered] Order already delivered.\n";
}

void DeliveredState::cancel (Order* order) {
    std::cout << "[Delivered] Cannot cancel delivered order.\n";
}

void CancelledState::process (Order* order) {
    std::cout << "[Cancelled] Cannot process cancelled order.\n";
}

void CancelledState::ship (Order* order) {
    std::cout << "[Cancelled] Cannot ship cancelled order.\n";
}

void CancelledState::deliver (Order* order) {
    std::cout << "[Cancelled] Cannot deliver cancelled order.\n";
}

void CancelledState::cancel (Order* order) {
    std::cout << "[Cancelled] Order already cancelled.\n";
}

} // namespace State