/*
The intent is to define the skeleton of an algorithm in an operation, deferring
some steps to subclasses. Template Method lets subclasses redefine certain steps
of an algorithm without changing the algorithm's structure.

Key participants:
- AbstractClass: Defines abstract primitive operations that concrete subclasses
  define to implement steps of an algorithm; implements a template method
defining the skeleton of an algorithm
- ConcreteClass: Implements the primitive operations to carry out
subclass-specific steps of the algorithm

Key benefits:
- Maximizes code reuse
- Common behavior is defined once in the abstract class
- Subclasses only need to implement the varying parts

Real-world example: A data processing pipeline where the high-level steps are
fixed (setup, process, cleanup) but the specific processing varies.
*/
#ifndef C2_TEMPLATE_METHOD_H
#define C2_TEMPLATE_METHOD_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace TemplateMethod {

// ----- Abstract Class -----
class DataProcessor {
    public:
    virtual ~DataProcessor () = default;

    // Template method - defines the algorithm structure
    void processData (const std::string& input) {
        std::cout << "===== Starting Data Processing =====\n";
        initialize ();
        std::string validated = validateInput (input);
        std::string result    = performCoreOperation (validated);
        finalize (result);
        std::cout << "===== Data Processing Complete =====\n\n";
    }

    protected:
    // Primitive operations to be implemented by subclasses
    virtual void initialize ()                                         = 0;
    virtual std::string performCoreOperation (const std::string& data) = 0;
    virtual void finalize (const std::string& result)                  = 0;

    // Hook method with default implementation (can be overridden)
    virtual std::string validateInput (const std::string& data) {
        std::cout << "[Default Validation] Checking if input is empty...\n";
        if (data.empty ()) {
            throw std::runtime_error ("Input cannot be empty!");
        }
        std::cout << "[Default Validation] Input validation passed.\n";
        return data;
    }

    // Another hook with default behavior
    virtual void logError (const std::string& error) {
        std::cout << "[ERROR] " << error << "\n";
    }
};

// ----- Concrete Class 1: Text Transformation Processor -----
class TextTransformationProcessor : public DataProcessor {
    protected:
    void initialize () override {
        std::cout
        << "[TextProcessor] Initializing text transformation engine...\n";
        std::cout << "[TextProcessor] Loading transformation rules...\n";
        std::cout << "[TextProcessor] Ready to process text.\n";
    }

    std::string performCoreOperation (const std::string& data) override {
        std::cout << "[TextProcessor] Transforming text to uppercase...\n";
        std::string result = data;

        // Convert to uppercase
        for (char& c : result) {
            c = std::toupper (c);
        }

        std::cout << "[TextProcessor] Adding decorative borders...\n";
        return "*** " + result + " ***";
    }

    void finalize (const std::string& result) override {
        std::cout << "[TextProcessor] Finalizing text transformation...\n";
        std::cout << "[TextProcessor] Result: " << result << "\n";
        std::cout << "[TextProcessor] Transformation complete!\n";
    }

    // Override validation to add custom checks
    std::string validateInput (const std::string& data) override {
        std::cout << "[TextProcessor] Custom validation for text data...\n";
        // Call parent validation
        std::string validated = DataProcessor::validateInput (data);

        // Additional text-specific validation
        if (validated.length () < 3) {
            logError ("Text too short! Minimum 3 characters required.");
            throw std::runtime_error ("Text too short!");
        }

        std::cout << "[TextProcessor] Text validation passed.\n";
        return validated;
    }
};

// ----- Concrete Class 2: Number Calculation Processor -----
class NumberCalculationProcessor : public DataProcessor {
    protected:
    void initialize () override {
        std::cout << "[NumberProcessor] Initializing calculation engine...\n";
        std::cout << "[NumberProcessor] Loading math libraries...\n";
        std::cout << "[NumberProcessor] Ready for calculations.\n";
    }

    std::string performCoreOperation (const std::string& data) override {
        std::cout << "[NumberProcessor] Parsing numbers from input...\n";

        // Parse comma-separated numbers
        std::vector<int> numbers;
        std::string current;
        for (char c : data) {
            if (c == ',' || c == ' ') {
                if (!current.empty ()) {
                    numbers.push_back (std::stoi (current));
                    current.clear ();
                }
            } else if (std::isdigit (c) || c == '-') {
                current += c;
            }
        }
        if (!current.empty ()) {
            numbers.push_back (std::stoi (current));
        }

        std::cout << "[NumberProcessor] Calculating sum and average...\n";
        int sum = 0;
        for (int num : numbers) {
            sum += num;
        }
        double average =
        numbers.empty () ? 0.0 : static_cast<double> (sum) / numbers.size ();

        std::cout << "[NumberProcessor] Sum: " << sum << ", Average: " << average << "\n";
        return "Sum=" + std::to_string (sum) + ", Avg=" + std::to_string (average);
    }

    void finalize (const std::string& result) override {
        std::cout << "[NumberProcessor] Finalizing calculation results...\n";
        std::cout << "[NumberProcessor] Result: " << result << "\n";
        std::cout << "[NumberProcessor] Calculation complete!\n";
    }

    std::string validateInput (const std::string& data) override {
        std::cout << "[NumberProcessor] Validating numeric input...\n";
        std::string validated = DataProcessor::validateInput (data);

        // Check if input contains at least one digit
        bool hasDigit = false;
        for (char c : validated) {
            if (std::isdigit (c)) {
                hasDigit = true;
                break;
            }
        }

        if (!hasDigit) {
            logError ("No numbers found in input!");
            throw std::runtime_error ("No numbers in input!");
        }

        std::cout << "[NumberProcessor] Numeric validation passed.\n";
        return validated;
    }
};

// ----- Example 2: Caffeine Beverage (Classic Template Method Example) -----
class CaffeineBeverage {
    public:
    virtual ~CaffeineBeverage () = default;

    // Template method
    void prepareRecipe () {
        boilWater ();
        brew ();
        pourInCup ();
        addCondiments ();
    }

    protected:
    // Primitive operations
    virtual void brew () = 0;

    // Common operations
    void boilWater () {
        std::cout << "Boiling water...\n";
    }

    void pourInCup () {
        std::cout << "Pouring into cup...\n";
    }

    // Hook (can be overridden)
    virtual bool customerWantsCondiments () {
        return true; // Default: yes, add condiments
    }

    // Template method implementation
    void addCondiments () {
        if (customerWantsCondiments ()) {
            addCondimentsImpl ();
        }
    }

    virtual void addCondimentsImpl () = 0;
};

class Coffee : public CaffeineBeverage {
    protected:
    void brew () override {
        std::cout << "Dripping coffee through filter...\n";
    }

    void addCondimentsImpl () override {
        std::cout << "Adding sugar and milk...\n";
    }

    // Override hook to customize behavior
    bool customerWantsCondiments () override {
        // Simulate asking the customer
        std::cout << "Would you like sugar and milk with your coffee? (y/n): ";
        // In a real program, you'd get user input here
        // For demo, we'll default to true
        return true;
    }
};

class Tea : public CaffeineBeverage {
    protected:
    void brew () override {
        std::cout << "Steeping the tea...\n";
    }

    void addCondimentsImpl () override {
        std::cout << "Adding lemon...\n";
    }

    // Tea always gets lemon in this example
    bool customerWantsCondiments () override {
        return true;
    }
};

// ----- Example 3: Game AI with Template Method -----
class GameAI {
    public:
    // Template method defining the turn structure
    void takeTurn () {
        std::cout << "\n--- AI Turn Starting ---\n";
        collectResources ();
        buildStructures ();
        buildUnits ();
        attack ();

        if (isTurnEnding ()) {
            endTurn ();
        }
        std::cout << "--- AI Turn Complete ---\n";
    }

    protected:
    // Primitive operations - different AI types implement differently
    virtual void collectResources () = 0;
    virtual void buildStructures ()  = 0;
    virtual void buildUnits ()       = 0;
    virtual void attack ()           = 0;

    // Hook with default implementation
    virtual bool isTurnEnding () {
        return true; // Default: end turn after all actions
    }

    // Common operation
    void endTurn () {
        std::cout << "Ending turn and saving state...\n";
    }
};

class AggressiveAI : public GameAI {
    protected:
    void collectResources () override {
        std::cout << "[Aggressive AI] Collecting minimal resources...\n";
        std::cout << "[Aggressive AI] Resources: 50 (focused on military)\n";
    }

    void buildStructures () override {
        std::cout
        << "[Aggressive AI] Building barracks and military buildings...\n";
    }

    void buildUnits () override {
        std::cout << "[Aggressive AI] Training attack units...\n";
        std::cout << "[Aggressive AI] Army size: 100 soldiers\n";
    }

    void attack () override {
        std::cout << "[Aggressive AI] ATTACKING enemy base!\n";
        std::cout << "[Aggressive AI] Sending all units to battle!\n";
    }

    bool isTurnEnding () override {
        std::cout
        << "[Aggressive AI] Not done yet - continuing aggression...\n";
        return false;
    }
};

class DefensiveAI : public GameAI {
    protected:
    void collectResources () override {
        std::cout << "[Defensive AI] Collecting maximum resources...\n";
        std::cout << "[Defensive AI] Resources: 200 (stockpiling)\n";
    }

    void buildStructures () override {
        std::cout << "[Defensive AI] Building walls and defenses...\n";
        std::cout << "[Defensive AI] Fortifying base perimeter\n";
    }

    void buildUnits () override {
        std::cout << "[Defensive AI] Training defensive units...\n";
        std::cout << "[Defensive AI] Garrison: 30 guards\n";
    }

    void attack () override {
        std::cout
        << "[Defensive AI] Not attacking - maintaining defensive position\n";
    }

    bool isTurnEnding () override {
        std::cout << "[Defensive AI] Turn complete - defenses ready\n";
        return true;
    }
};

} // namespace TemplateMethod

#endif
