/*
The intent is to define a family of algorithms, encapsulate each one, and make them
interchangeable. Strategy lets the algorithm vary independently from clients that use it.

Key participants:
- Strategy: Declares an interface common to all supported algorithms
- ConcreteStrategy: Implements the algorithm using the Strategy interface
- Context: Is configured with a ConcreteStrategy object; maintains a reference to
  a Strategy object

Key benefits:
- Families of related algorithms can be defined
- Alternative strategies can be swapped at runtime
- Eliminates conditional statements
- Different implementations of the same behavior

Real-world example: A navigation app that can calculate routes using different
strategies (fastest route, shortest distance, avoid highways, etc.).
*/
#ifndef C10_STRATEGY_H
#define C10_STRATEGY_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace Strategy {

// ----- Example 1: Payment Processing Strategies -----
class PaymentStrategy {
    public:
    virtual ~PaymentStrategy ()          = default;
    virtual void pay (double amount)     = 0;
    virtual std::string getName () const = 0;
};

class CreditCardPayment : public PaymentStrategy {
    public:
    CreditCardPayment (const std::string& cardNumber, const std::string& name)
    : cardNumber_ (cardNumber), name_ (name) {
    }

    void pay (double amount) override {
        std::cout << "[Credit Card] Paying $" << amount << " using card ending in "
                  << cardNumber_.substr (cardNumber_.length () - 4) << "\n";
        std::cout << "[Credit Card] Cardholder: " << name_ << "\n";
        std::cout << "[Credit Card] Transaction approved.\n";
    }

    std::string getName () const override {
        return "Credit Card";
    }

    private:
    std::string cardNumber_;
    std::string name_;
};

class PayPalPayment : public PaymentStrategy {
    public:
    explicit PayPalPayment (const std::string& email) : email_ (email) {
    }

    void pay (double amount) override {
        std::cout << "[PayPal] Paying $" << amount << " using PayPal account\n";
        std::cout << "[PayPal] Account: " << email_ << "\n";
        std::cout << "[PayPal] Redirecting to PayPal for authentication...\n";
        std::cout << "[PayPal] Payment completed successfully.\n";
    }

    std::string getName () const override {
        return "PayPal";
    }

    private:
    std::string email_;
};

class ApplePayPayment : public PaymentStrategy {
    public:
    explicit ApplePayPayment (const std::string& deviceId)
    : deviceId_ (deviceId) {
    }

    void pay (double amount) override {
        std::cout << "[Apple Pay] Paying $" << amount << " using Apple Pay\n";
        std::cout << "[Apple Pay] Device ID: " << deviceId_ << "\n";
        std::cout << "[Apple Pay] Authenticate with Face ID/Touch ID...\n";
        std::cout << "[Apple Pay] Payment successful.\n";
    }

    std::string getName () const override {
        return "Apple Pay";
    }

    private:
    std::string deviceId_;
};

class CryptoPayment : public PaymentStrategy {
    public:
    explicit CryptoPayment (const std::string& walletAddress)
    : walletAddress_ (walletAddress) {
    }

    void pay (double amount) override {
        std::cout << "[Crypto] Paying $" << amount << " in cryptocurrency\n";
        std::cout << "[Crypto] Wallet: " << walletAddress_.substr (0, 10) << "...\n";
        std::cout << "[Crypto] Processing blockchain transaction...\n";
        std::cout << "[Crypto] Payment confirmed on blockchain.\n";
    }

    std::string getName () const override {
        return "Cryptocurrency";
    }

    private:
    std::string walletAddress_;
};

// Context
class ShoppingCart {
    public:
    void setPaymentStrategy (std::shared_ptr<PaymentStrategy> strategy) {
        paymentStrategy_ = strategy;
        std::cout
        << "[Shopping Cart] Payment method set to: " << strategy->getName () << "\n";
    }

    void checkout (double total) {
        std::cout << "\n[Shopping Cart] Starting checkout process...\n";
        std::cout << "[Shopping Cart] Total amount: $" << total << "\n";
        if (paymentStrategy_) {
            paymentStrategy_->pay (total);
        } else {
            std::cout << "[Shopping Cart] ERROR: No payment method selected!\n";
        }
        std::cout << "[Shopping Cart] Checkout complete.\n\n";
    }

    private:
    std::shared_ptr<PaymentStrategy> paymentStrategy_;
};

// ----- Example 2: Compression Strategies -----
class CompressionStrategy {
    public:
    virtual ~CompressionStrategy ()                     = default;
    virtual void compress (const std::string& fileName) = 0;
    virtual std::string getExtension () const           = 0;
};

class ZipCompression : public CompressionStrategy {
    public:
    void compress (const std::string& fileName) override {
        std::cout << "[ZIP Compression] Compressing " << fileName << "\n";
        std::cout << "[ZIP Compression] Using DEFLATE algorithm...\n";
        std::cout << "[ZIP Compression] Archive created: " << fileName << ".zip\n";
    }

    std::string getExtension () const override {
        return ".zip";
    }
};

class RarCompression : public CompressionStrategy {
    public:
    void compress (const std::string& fileName) override {
        std::cout << "[RAR Compression] Compressing " << fileName << "\n";
        std::cout << "[RAR Compression] Using RAR algorithm...\n";
        std::cout << "[RAR Compression] Archive created: " << fileName << ".rar\n";
    }

    std::string getExtension () const override {
        return ".rar";
    }
};

class SevenZipCompression : public CompressionStrategy {
    public:
    void compress (const std::string& fileName) override {
        std::cout << "[7-Zip Compression] Compressing " << fileName << "\n";
        std::cout << "[7-Zip Compression] Using LZMA algorithm...\n";
        std::cout << "[7-Zip Compression] Archive created: " << fileName << ".7z\n";
    }

    std::string getExtension () const override {
        return ".7z";
    }
};

class TarGzCompression : public CompressionStrategy {
    public:
    void compress (const std::string& fileName) override {
        std::cout << "[Tar+Gzip Compression] Compressing " << fileName << "\n";
        std::cout << "[Tar+Gzip Compression] Creating tar archive first...\n";
        std::cout << "[Tar+Gzip Compression] Compressing with gzip...\n";
        std::cout << "[Tar+Gzip Compression] Archive created: " << fileName << ".tar.gz\n";
    }

    std::string getExtension () const override {
        return ".tar.gz";
    }
};

// Context
class FileCompressor {
    public:
    void setCompressionStrategy (std::shared_ptr<CompressionStrategy> strategy) {
        strategy_ = strategy;
    }

    void compressFile (const std::string& fileName) {
        if (strategy_) {
            std::cout << "\n[File Compressor] Compressing file: " << fileName << "\n";
            std::cout
            << "[File Compressor] Using format: " << strategy_->getExtension () << "\n";
            strategy_->compress (fileName);
            std::cout << "[File Compressor] Compression complete.\n\n";
        } else {
            std::cout
            << "[File Compressor] ERROR: No compression strategy set!\n";
        }
    }

    private:
    std::shared_ptr<CompressionStrategy> strategy_;
};

// ----- Example 3: Route Calculation Strategies -----
struct Location {
    std::string name;
    double latitude;
    double longitude;

    Location (const std::string& n, double lat, double lon)
    : name (n), latitude (lat), longitude (lon) {
    }
};

class RouteStrategy {
    public:
    virtual ~RouteStrategy () = default;
    virtual void buildRoute (const Location& start, const Location& end) = 0;
    virtual std::string getStrategyName () const                         = 0;
};

class FastestRouteStrategy : public RouteStrategy {
    public:
    void buildRoute (const Location& start, const Location& end) override {
        std::cout << "[Fastest Route] Calculating route from " << start.name
                  << " to " << end.name << "\n";
        std::cout
        << "[Fastest Route] Prioritizing highways and high-speed roads...\n";
        std::cout << "[Fastest Route] Real-time traffic analysis...\n";
        std::cout << "[Fastest Route] Route calculated:\n";
        std::cout << "  1. Start at " << start.name << "\n";
        std::cout << "  2. Take Highway 101 (55 mph)\n";
        std::cout << "  3. Merge onto I-95 (65 mph)\n";
        std::cout << "  4. Exit at " << end.name << "\n";
        std::cout << "[Fastest Route] Estimated time: 45 minutes\n";
    }

    std::string getStrategyName () const override {
        return "Fastest Route";
    }
};

class ShortestRouteStrategy : public RouteStrategy {
    public:
    void buildRoute (const Location& start, const Location& end) override {
        std::cout << "[Shortest Route] Calculating route from " << start.name
                  << " to " << end.name << "\n";
        std::cout << "[Shortest Route] Finding minimum distance path...\n";
        std::cout << "[Shortest Route] Route calculated:\n";
        std::cout << "  1. Start at " << start.name << "\n";
        std::cout << "  2. Take local roads (scenic route)\n";
        std::cout << "  3. Through downtown area\n";
        std::cout << "  4. Arrive at " << end.name << "\n";
        std::cout << "[Shortest Route] Distance: 18.5 miles\n";
    }

    std::string getStrategyName () const override {
        return "Shortest Distance";
    }
};

class AvoidHighwaysStrategy : public RouteStrategy {
    public:
    void buildRoute (const Location& start, const Location& end) override {
        std::cout << "[Avoid Highways] Calculating route from " << start.name
                  << " to " << end.name << "\n";
        std::cout << "[Avoid Highways] Excluding all highways from route...\n";
        std::cout << "[Avoid Highways] Route calculated:\n";
        std::cout << "  1. Start at " << start.name << "\n";
        std::cout << "  2. Take Main Street through neighborhoods\n";
        std::cout << "  3. Follow scenic country roads\n";
        std::cout << "  4. Arrive at " << end.name << "\n";
        std::cout << "[Avoid Highways] Estimated time: 1 hour 15 minutes\n";
    }

    std::string getStrategyName () const override {
        return "Avoid Highways";
    }
};

class EcoFriendlyRouteStrategy : public RouteStrategy {
    public:
    void buildRoute (const Location& start, const Location& end) override {
        std::cout << "[Eco-Friendly Route] Calculating route from "
                  << start.name << " to " << end.name << "\n";
        std::cout << "[Eco-Friendly Route] Optimizing for fuel efficiency...\n";
        std::cout << "[Eco-Friendly Route] Avoiding steep hills and "
                     "stop-and-go traffic...\n";
        std::cout << "[Eco-Friendly Route] Route calculated:\n";
        std::cout << "  1. Start at " << start.name << "\n";
        std::cout << "  2. Take gentle terrain roads\n";
        std::cout << "  3. Maintain steady speeds (45-55 mph)\n";
        std::cout << "  4. Arrive at " << end.name << "\n";
        std::cout << "[Eco-Friendly Route] Estimated fuel savings: 15%\n";
    }

    std::string getStrategyName () const override {
        return "Eco-Friendly";
    }
};

// Context
class NavigationSystem {
    public:
    NavigationSystem (std::string userName) : userName_ (std::move (userName)) {
    }

    void setRouteStrategy (std::shared_ptr<RouteStrategy> strategy) {
        strategy_ = strategy;
    }

    void navigate (const Location& start, const Location& end) {
        std::cout << "\n[Navigation System] User: " << userName_ << "\n";
        std::cout
        << "[Navigation System] Strategy: " << strategy_->getStrategyName () << "\n";
        std::cout << "[Navigation System] Calculating route...\n\n";
        strategy_->buildRoute (start, end);
        std::cout
        << "\n[Navigation System] Route ready! Starting navigation...\n\n";
    }

    private:
    std::string userName_;
    std::shared_ptr<RouteStrategy> strategy_;
};

// ----- Example 4: Sorting Strategies -----
class SortStrategy {
    public:
    virtual ~SortStrategy ()                   = default;
    virtual void sort (std::vector<int>& data) = 0;
    virtual std::string getName () const       = 0;
};

class BubbleSortStrategy : public SortStrategy {
    public:
    void sort (std::vector<int>& data) override {
        std::cout << "[Bubble Sort] Starting sort...\n";
        for (size_t i = 0; i < data.size (); ++i) {
            for (size_t j = 0; j < data.size () - i - 1; ++j) {
                if (data[j] > data[j + 1]) {
                    std::swap (data[j], data[j + 1]);
                }
            }
        }
        std::cout << "[Bubble Sort] Sort complete.\n";
    }

    std::string getName () const override {
        return "Bubble Sort";
    }
};

class QuickSortStrategy : public SortStrategy {
    public:
    void sort (std::vector<int>& data) override {
        std::cout << "[Quick Sort] Starting sort...\n";
        quickSort (data, 0, data.size () - 1);
        std::cout << "[Quick Sort] Sort complete.\n";
    }

    std::string getName () const override {
        return "Quick Sort";
    }

    private:
    void quickSort (std::vector<int>& data, int low, int high) {
        if (low < high) {
            int pi = partition (data, low, high);
            quickSort (data, low, pi - 1);
            quickSort (data, pi + 1, high);
        }
    }

    int partition (std::vector<int>& data, int low, int high) {
        int pivot = data[high];
        int i     = low - 1;
        for (int j = low; j < high; ++j) {
            if (data[j] < pivot) {
                i++;
                std::swap (data[i], data[j]);
            }
        }
        std::swap (data[i + 1], data[high]);
        return i + 1;
    }
};

// Context
class Sorter {
    public:
    void setSortStrategy (std::shared_ptr<SortStrategy> strategy) {
        strategy_ = strategy;
    }

    void executeSort (std::vector<int>& data) {
        if (strategy_) {
            std::cout << "\n[Sorter] Using strategy: " << strategy_->getName () << "\n";
            std::cout << "[Sorter] Original data: ";
            for (int num : data) {
                std::cout << num << " ";
            }
            std::cout << "\n";
            strategy_->sort (data);
            std::cout << "[Sorter] Sorted data: ";
            for (int num : data) {
                std::cout << num << " ";
            }
            std::cout << "\n\n";
        }
    }

    private:
    std::shared_ptr<SortStrategy> strategy_;
};

} // namespace Strategy

#endif
