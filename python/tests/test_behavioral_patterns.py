"""
Tests for Behavioral Design Patterns (C1-C11)
"""
import pytest

from src.c1_interpreter import (
    interpreter_example, Context, NumberExpression, VariableExpression,
    AddExpression, SubtractExpression, MultiplyExpression, DivideExpression,
    get_expression_tree, get_context_with_variables
)
from src.c2_template_method import (
    template_method_example, DataProcessor, TextTransformationProcessor,
    NumberCalculationProcessor, CaffeineBeverage, Coffee, Tea,
    get_text_processor, get_number_processor, get_coffee
)
from src.c3_chain_of_responsibility import (
    chain_of_responsibility_example, SupportHandler, SupportTicket, Level,
    Level1Support, Level2Support, Manager, Director,
    get_support_chain, get_basic_ticket
)
from src.c4_command import (
    command_example, Command, Light, LightOnCommand, LightOffCommand,
    Stereo, StereoOnWithCDCommand, CeilingFan, CeilingFanHighCommand,
    RemoteControl, MacroCommand, NoCommand,
    get_remote, get_ceiling_fan_remote
)
from src.c5_iterator import (
    iterator_example, Iterator, Aggregate, VectorIterator, ReverseVectorIterator,
    Playlist, Song, MenuItem, PancakeHouseMenu,
    get_playlist, get_menu
)
from src.c6_mediator import (
    mediator_example, ATCMediator, Aircraft, ControlTower,
    get_control_tower, get_aircraft_with_tower
)
from src.c7_memento import (
    memento_example, DocumentMemento, TextDocument, History,
    GameCharacter, GameCharacterMemento,
    get_document, get_game_character
)
from src.c8_observer import (
    observer_example, Observer, Subject, WeatherData, WeatherStation,
    PhoneDisplay, WebDisplay, ForecastDisplay, YouTubeChannel, Subscriber,
    get_weather_station, get_youtube_channel
)
from src.c9_state import (
    state_example, State, VendingMachine, get_vending_machine, get_empty_vending_machine
)
from src.c10_strategy import (
    strategy_example, PaymentStrategy, ShoppingCart, RouteStrategy,
    NavigationSystem, CreditCardPayment, PayPalPayment, FastestRouteStrategy,
    ShortestRouteStrategy, get_shopping_cart, get_navigation_system
)
from src.c11_visitor import (
    visitor_example, Visitor, Item, ShoppingCart as VisitorCart,
    Book, Fruit, Electronics, TaxVisitor, DiscountVisitor,
    get_shopping_cart as get_visitor_cart, get_tax_visitor
)


class TestInterpreter:
    """Tests for Interpreter Pattern"""

    def test_number_expression_returns_value(self):
        """Test that number expression returns its value"""
        expr = NumberExpression(42)
        context = Context()
        assert expr.interpret(context) == 42

    def test_variable_expression_looks_up_value(self):
        """Test that variable expression looks up value in context"""
        context = Context()
        context.set_variable("x", 10)
        expr = VariableExpression("x")
        assert expr.interpret(context) == 10

    def test_add_expression_adds_values(self):
        """Test that add expression adds two values"""
        expr = AddExpression(NumberExpression(5), NumberExpression(3))
        context = Context()
        assert expr.interpret(context) == 8

    def test_subtract_expression_subtracts_values(self):
        """Test that subtract expression subtracts two values"""
        expr = SubtractExpression(NumberExpression(10), NumberExpression(3))
        context = Context()
        assert expr.interpret(context) == 7

    def test_multiply_expression_multiplies_values(self):
        """Test that multiply expression multiplies two values"""
        expr = MultiplyExpression(NumberExpression(5), NumberExpression(4))
        context = Context()
        assert expr.interpret(context) == 20

    def test_divide_expression_divides_values(self):
        """Test that divide expression divides two values"""
        expr = DivideExpression(NumberExpression(20), NumberExpression(4))
        context = Context()
        assert expr.interpret(context) == 5

    def test_divide_by_zero_returns_zero(self, capsys):
        """Test that divide by zero returns zero with error message"""
        expr = DivideExpression(NumberExpression(10), NumberExpression(0))
        context = Context()
        result = expr.interpret(context)
        captured = capsys.readouterr()
        assert result == 0
        assert "Division by zero" in captured.out

    def test_complex_expression_tree(self):
        """Test complex expression: (5 + 3) * 2"""
        expr = MultiplyExpression(
            AddExpression(NumberExpression(5), NumberExpression(3)),
            NumberExpression(2)
        )
        context = Context()
        assert expr.interpret(context) == 16

    def test_helper_functions(self):
        """Test that helper functions create valid objects"""
        expr = get_expression_tree()
        context = get_context_with_variables()
        assert expr.interpret(context) == 8
        assert context.get_variable("a") == 10
        assert context.get_variable("b") == 5


class TestTemplateMethod:
    """Tests for Template Method Pattern"""

    def test_text_processor_follows_template(self, capsys):
        """Test that text processor follows template method structure"""
        processor = TextTransformationProcessor()
        processor.process_data("hello")
        captured = capsys.readouterr()
        assert "Starting Data Processing" in captured.out
        assert "text transformation engine" in captured.out
        assert "Transforming to uppercase" in captured.out
        assert "Data Processing Complete" in captured.out

    def test_text_processor_validates_input(self):
        """Test that text processor validates input length"""
        processor = TextTransformationProcessor()
        with pytest.raises(ValueError, match="Text too short"):
            processor.process_data("hi")

    def test_number_processor_calculates_sum_and_average(self, capsys):
        """Test that number processor calculates sum and average"""
        processor = NumberCalculationProcessor()
        processor.process_data("10,20,30")
        captured = capsys.readouterr()
        assert "Sum=60" in captured.out
        assert "Avg=20.0" in captured.out

    def test_empty_input_raises_error(self):
        """Test that empty input raises error"""
        processor = TextTransformationProcessor()
        with pytest.raises(ValueError, match="Input cannot be empty"):
            processor.process_data("")

    def test_coffee_brewing_steps(self, capsys):
        """Test that coffee follows correct brewing steps"""
        coffee = Coffee()
        coffee.prepare_recipe()
        captured = capsys.readouterr()
        assert "Boiling water" in captured.out
        assert "Dripping coffee" in captured.out
        assert "Pouring into cup" in captured.out
        assert "sugar and milk" in captured.out

    def test_tea_brewing_steps(self, capsys):
        """Test that tea follows correct brewing steps"""
        tea = Tea()
        tea.prepare_recipe()
        captured = capsys.readouterr()
        assert "Boiling water" in captured.out
        assert "Steeping the tea" in captured.out
        assert "Pouring into cup" in captured.out
        assert "Adding lemon" in captured.out

    def test_helper_functions(self):
        """Test that helper functions create valid processors"""
        text_proc = get_text_processor()
        num_proc = get_number_processor()
        coffee = get_coffee()
        assert isinstance(text_proc, DataProcessor)
        assert isinstance(num_proc, DataProcessor)
        assert isinstance(coffee, CaffeineBeverage)


class TestChainOfResponsibility:
    """Tests for Chain of Responsibility Pattern"""

    def test_basic_ticket_handled_by_level1(self, capsys):
        """Test that basic ticket is handled by level 1 support"""
        level1 = Level1Support()
        level2 = Level2Support()
        manager = Manager()
        level1.set_next(level2).set_next(manager)
        ticket = SupportTicket("WiFi issue", Level.BASIC)
        level1.handle_ticket(ticket)
        captured = capsys.readouterr()
        assert "Level 1 Support" in captured.out
        assert "basic troubleshooting" in captured.out

    def test_intermediate_ticket_escalated_to_level2(self, capsys):
        """Test that intermediate ticket is handled by level 2 support"""
        level1 = Level1Support()
        level2 = Level2Support()
        manager = Manager()
        level1.set_next(level2).set_next(manager)
        ticket = SupportTicket("Server crash", Level.INTERMEDIATE)
        level1.handle_ticket(ticket)
        captured = capsys.readouterr()
        assert "Level 2 Support" in captured.out
        assert "technical expertise" in captured.out

    def test_advanced_ticket_escalated_to_manager(self, capsys):
        """Test that advanced ticket is handled by manager"""
        level1 = Level1Support()
        level2 = Level2Support()
        manager = Manager()
        level1.set_next(level2).set_next(manager)
        ticket = SupportTicket("Budget needed", Level.ADVANCED)
        level1.handle_ticket(ticket)
        captured = capsys.readouterr()
        assert "Manager" in captured.out
        assert "management authority" in captured.out

    def test_chain_stops_at_appropriate_handler(self, capsys):
        """Test that chain stops at first appropriate handler"""
        level1 = Level1Support()
        level2 = Level2Support()
        manager = Manager()
        level1.set_next(level2).set_next(manager)
        ticket = get_basic_ticket()
        level1.handle_ticket(ticket)
        captured = capsys.readouterr()
        # Should not escalate to level 2
        assert "Level 2 Support" not in captured.out

    def test_helper_function(self):
        """Test that helper creates valid chain"""
        level1 = Level1Support()
        level2 = Level2Support()
        manager = Manager()
        level1.set_next(level2).set_next(manager)
        assert isinstance(level1, SupportHandler)


class TestCommand:
    """Tests for Command Pattern"""

    def test_light_on_command_turns_on_light(self, capsys):
        """Test that light on command turns on the light"""
        light = Light()
        command = LightOnCommand(light)
        command.execute()
        captured = capsys.readouterr()
        assert "Light is ON" in captured.out

    def test_light_off_command_turns_off_light(self, capsys):
        """Test that light off command turns off the light"""
        light = Light()
        command = LightOffCommand(light)
        command.execute()
        captured = capsys.readouterr()
        assert "Light is OFF" in captured.out

    def test_command_undo_reverses_action(self, capsys):
        """Test that command undo reverses the action"""
        light = Light()
        command = LightOnCommand(light)
        command.execute()
        command.undo()
        captured = capsys.readouterr()
        assert captured.out.count("Light is ON") == 1
        assert captured.out.count("Light is OFF") == 1

    def test_stereo_command_performs_multiple_actions(self, capsys):
        """Test that stereo command performs setup sequence"""
        stereo = Stereo()
        command = StereoOnWithCDCommand(stereo)
        command.execute()
        captured = capsys.readouterr()
        assert "Stereo is ON" in captured.out
        assert "CD player selected" in captured.out
        assert "volume set to 11" in captured.out

    def test_ceiling_fan_command_remembers_previous_speed(self, capsys):
        """Test that ceiling fan command remembers previous speed for undo"""
        fan = CeilingFan()
        remote, fan_ref = get_ceiling_fan_remote()
        fan_ref.set_speed(CeilingFan.Speed.MEDIUM)
        remote.on_button_was_pressed(0)
        remote.undo_button_was_pressed()
        captured = capsys.readouterr()
        assert "MEDIUM speed" in captured.out

    def test_remote_control_undo_stack(self, capsys):
        """Test that remote control maintains undo stack"""
        remote = get_remote()
        remote.on_button_was_pressed(0)
        remote.off_button_was_pressed(0)
        remote.undo_button_was_pressed()
        captured = capsys.readouterr()
        assert "UNDO button pressed" in captured.out

    def test_macro_command_executes_multiple_commands(self, capsys):
        """Test that macro command executes all commands in sequence"""
        light = Light()
        stereo = Stereo()
        light_on = LightOnCommand(light)
        stereo_on = StereoOnWithCDCommand(stereo)
        macro = MacroCommand([light_on, stereo_on])
        macro.execute()
        captured = capsys.readouterr()
        assert "Light is ON" in captured.out
        assert "Stereo is ON" in captured.out

    def test_helper_functions(self):
        """Test that helper functions create valid objects"""
        remote = get_remote()
        remote, fan = get_ceiling_fan_remote()
        assert isinstance(remote, RemoteControl)
        assert isinstance(fan, CeilingFan)


class TestIterator:
    """Tests for Iterator Pattern"""

    def test_vector_iterator_traverses_forward(self):
        """Test that vector iterator traverses items forward"""
        playlist = get_playlist()
        iterator = playlist.create_iterator()
        songs = []
        while iterator.has_next():
            songs.append(iterator.next())
        assert len(songs) == 2
        assert "Song 1" in str(songs[0])

    def test_reverse_iterator_traverses_backward(self):
        """Test that reverse iterator traverses items backward"""
        playlist = get_playlist()
        iterator = playlist.create_reverse_iterator()
        songs = []
        while iterator.has_next():
            songs.append(iterator.next())
        assert len(songs) == 2
        assert "Song 2" in str(songs[0])

    def test_iterator_raises_exception_when_exhausted(self):
        """Test that iterator raises exception when exhausted"""
        playlist = Playlist[Song]()
        iterator = playlist.create_iterator()
        with pytest.raises(StopIteration):
            iterator.next()

    def test_iterator_reset(self):
        """Test that iterator can be reset"""
        playlist = get_playlist()
        iterator = playlist.create_iterator()
        iterator.next()
        iterator.reset()
        assert iterator.has_next()

    def test_menu_iterator(self):
        """Test that menu iterator works with menu items"""
        menu = get_menu()
        iterator = menu.create_iterator()
        items = []
        while iterator.has_next():
            items.append(iterator.next())
        assert len(items) == 3
        assert items[0].name == "K&B's Pancake Breakfast"

    def test_aggregate_size(self):
        """Test that aggregate reports correct size"""
        playlist = get_playlist()
        assert playlist.size() == 2

    def test_aggregate_add_items(self):
        """Test that items can be added to aggregate"""
        playlist = Playlist[Song]()
        playlist.add(Song("Test", "Artist"))
        assert playlist.size() == 1

    def test_helper_functions(self):
        """Test that helper functions create valid objects"""
        playlist = get_playlist()
        menu = get_menu()
        assert playlist.size() > 0
        assert isinstance(menu, PancakeHouseMenu)


class TestMediator:
    """Tests for Mediator Pattern"""

    def test_aircraft_registered_with_mediator(self, capsys):
        """Test that aircraft are registered with mediator"""
        tower = get_control_tower()
        aircraft = Aircraft("Test", tower)
        tower.register_aircraft(aircraft)
        captured = capsys.readouterr()
        assert "registered in airspace" in captured.out

    def test_message_broadcast_to_other_aircraft(self, capsys):
        """Test that message is broadcast to other aircraft"""
        tower = ControlTower()
        plane1 = Aircraft("Plane1", tower)
        plane2 = Aircraft("Plane2", tower)
        tower.register_aircraft(plane1)
        tower.register_aircraft(plane2)
        plane1.send_message("Hello")
        captured = capsys.readouterr()
        assert "Plane2 receives" in captured.out

    def test_aircraft_can_land_when_runway_available(self, capsys):
        """Test that aircraft can land when runway is available"""
        aircraft, tower = get_aircraft_with_tower()
        aircraft.takeoff()
        aircraft.land()
        captured = capsys.readouterr()
        assert "cleared to land" in captured.out

    def test_aircraft_can_takeoff(self, capsys):
        """Test that aircraft can take off"""
        aircraft, tower = get_aircraft_with_tower()
        aircraft.takeoff()
        captured = capsys.readouterr()
        assert "cleared for takeoff" in captured.out

    def test_helper_functions(self):
        """Test that helper functions create valid objects"""
        tower = get_control_tower()
        aircraft, tower = get_aircraft_with_tower()
        assert isinstance(tower, ControlTower)
        assert isinstance(aircraft, Aircraft)


class TestMemento:
    """Tests for Memento Pattern"""

    def test_document_write_adds_content(self):
        """Test that writing adds content to document"""
        doc = TextDocument()
        doc.write("Hello")
        assert doc.content == "Hello"

    def test_document_save_creates_memento(self):
        """Test that saving creates a memento with current state"""
        doc = TextDocument()
        doc.write("Hello")
        memento = doc.save()
        assert memento.content == "Hello"
        assert memento.cursor_position == 5

    def test_document_restore_reverts_state(self):
        """Test that restoring reverts to saved state"""
        doc = TextDocument()
        doc.write("Hello")
        memento = doc.save()
        doc.write(" World")
        assert doc.content == "Hello World"
        doc.restore(memento)
        assert doc.content == "Hello"

    def test_history_undo(self):
        """Test that history can undo to previous state"""
        doc, history = get_document()
        doc.write("Hello")
        history.save_state(doc.save())
        doc.write(" World")
        history.save_state(doc.save())
        memento = history.undo()
        if memento:
            doc.restore(memento)
        assert doc.content == "Hello"

    def test_history_redo(self):
        """Test that history can redo after undo"""
        doc, history = get_document()
        doc.write("Hello")
        history.save_state(doc.save())
        doc.write(" World")
        history.save_state(doc.save())
        history.undo()
        memento = history.redo()
        if memento:
            doc.restore(memento)
        assert doc.content == "Hello World"

    def test_game_character_checkpoint(self):
        """Test that game character can create and restore checkpoint"""
        hero = get_game_character()
        hero.move(10, 20)
        checkpoint = hero.create_checkpoint()
        hero.take_damage(50)
        hero.restore_from_checkpoint(checkpoint)
        assert hero._health == 100  # Should be restored to full health

    def test_helper_functions(self):
        """Test that helper functions create valid objects"""
        doc, history = get_document()
        hero = get_game_character()
        assert isinstance(doc, TextDocument)
        assert isinstance(history, History)
        assert isinstance(hero, GameCharacter)


class TestObserver:
    """Tests for Observer Pattern"""

    def test_observer_attached_to_subject(self, capsys):
        """Test that observer is attached to subject"""
        station = get_weather_station()
        observer = PhoneDisplay("Test")
        station.attach(observer)
        captured = capsys.readouterr()
        assert "Attached observer" in captured.out

    def test_observer_notified_on_update(self, capsys):
        """Test that observer is notified when subject updates"""
        station = WeatherStation()
        observer = PhoneDisplay("John")
        station.attach(observer)
        station.set_measurements(25.0, 60.0, 1015.0)
        captured = capsys.readouterr()
        assert "Notification received" in captured.out
        assert "25.0°C" in captured.out

    def test_observer_detached_from_subject(self, capsys):
        """Test that observer can be detached from subject"""
        station = WeatherStation()
        observer = PhoneDisplay("John")
        station.attach(observer)
        station.detach(observer)
        station.set_measurements(25.0, 60.0, 1015.0)
        captured = capsys.readouterr()
        assert "Detached observer" in captured.out

    def test_multiple_observers_all_notified(self, capsys):
        """Test that all observers are notified"""
        station = WeatherStation()
        observer1 = PhoneDisplay("John")
        observer2 = PhoneDisplay("Jane")
        station.attach(observer1)
        station.attach(observer2)
        station.set_measurements(25.0, 60.0, 1015.0)
        captured = capsys.readouterr()
        assert captured.out.count("Notification received") == 2

    def test_youtube_channel_notifies_subscribers(self, capsys):
        """Test that YouTube channel notifies subscribers"""
        channel = YouTubeChannel("TestChannel")
        subscriber = Subscriber("Alice")
        channel.attach(subscriber)
        channel.upload_video("Test Video")
        captured = capsys.readouterr()
        assert "Alice received notification" in captured.out
        assert "Test Video" in captured.out

    def test_helper_functions(self):
        """Test that helper functions create valid objects"""
        station = get_weather_station()
        channel = get_youtube_channel()
        assert isinstance(station, WeatherStation)
        assert isinstance(channel, YouTubeChannel)


class TestState:
    """Tests for State Pattern"""

    def test_vending_machine_initial_state(self):
        """Test that vending machine has correct initial state"""
        machine = get_vending_machine()
        assert machine.state_name == "No Coin"

    def test_insert_coin_changes_state(self):
        """Test that inserting coin changes state to HasCoin"""
        machine = get_vending_machine()
        machine.insert_coin()
        assert machine.state_name == "Has Coin"

    def test_press_button_dispenses_product(self, capsys):
        """Test that pressing button dispenses product"""
        machine = get_vending_machine()
        machine.insert_coin()
        machine.press_button()
        captured = capsys.readouterr()
        assert "Dispensing product" in captured.out
        assert machine.state_name == "No Coin"

    def test_eject_coin_returns_to_no_coin(self):
        """Test that ejecting coin returns to no coin state"""
        machine = get_vending_machine()
        machine.insert_coin()
        machine.eject_coin()
        assert machine.state_name == "No Coin"

    def test_empty_vending_machine_out_of_stock(self):
        """Test that empty vending machine starts in out of stock state"""
        machine = get_empty_vending_machine()
        assert machine.state_name == "Out of Stock"

    def test_out_of_stock_rejects_coin(self, capsys):
        """Test that out of stock state rejects coins"""
        machine = get_empty_vending_machine()
        machine.insert_coin()
        captured = capsys.readouterr()
        assert "out of stock" in captured.out

    def test_helper_functions(self):
        """Test that helper functions create valid machines"""
        machine = get_vending_machine()
        empty = get_empty_vending_machine()
        assert machine.has_stock
        assert not empty.has_stock


class TestStrategy:
    """Tests for Strategy Pattern"""

    def test_shopping_cart_uses_credit_card_strategy(self, capsys):
        """Test that shopping cart can use credit card payment"""
        cart = ShoppingCart()
        cart.set_payment_strategy(CreditCardPayment("1234567890123456", "John Doe"))
        cart.checkout(99.99)
        captured = capsys.readouterr()
        assert "Credit Card" in captured.out
        assert "3456" in captured.out  # Last four digits

    def test_shopping_cart_uses_paypal_strategy(self, capsys):
        """Test that shopping cart can use PayPal payment"""
        cart = ShoppingCart()
        cart.set_payment_strategy(PayPalPayment("john@example.com"))
        cart.checkout(49.99)
        captured = capsys.readouterr()
        assert "PayPal" in captured.out
        assert "john@example.com" in captured.out

    def test_navigation_uses_fastest_route(self, capsys):
        """Test that navigation can use fastest route strategy"""
        nav = get_navigation_system()
        nav.set_route_strategy(FastestRouteStrategy())
        nav.navigate("New York", "Boston")
        captured = capsys.readouterr()
        assert "Fastest Route" in captured.out

    def test_navigation_uses_shortest_route(self, capsys):
        """Test that navigation can use shortest route strategy"""
        nav = get_navigation_system()
        nav.set_route_strategy(ShortestRouteStrategy())
        nav.navigate("New York", "Boston")
        captured = capsys.readouterr()
        assert "Shortest Distance" in captured.out

    def test_strategy_can_be_changed(self):
        """Test that strategy can be changed at runtime"""
        nav = get_navigation_system()
        nav.set_route_strategy(FastestRouteStrategy())
        nav.set_route_strategy(ShortestRouteStrategy())
        # Should work without error
        nav.navigate("A", "B")

    def test_helper_functions(self):
        """Test that helper functions create valid objects"""
        cart = get_shopping_cart()
        nav = get_navigation_system()
        assert isinstance(cart, ShoppingCart)
        assert isinstance(nav, NavigationSystem)


class TestVisitor:
    """Tests for Visitor Pattern"""

    def test_visitor_visits_book(self, capsys):
        """Test that visitor can visit book"""
        cart = get_visitor_cart()
        visitor = get_tax_visitor()
        cart.accept(visitor)
        captured = capsys.readouterr()
        assert "TaxVisitor" in captured.out
        assert "Book" in captured.out

    def test_visitor_visits_fruit(self, capsys):
        """Test that visitor can visit fruit"""
        cart = VisitorCart()
        cart.add_item(Fruit("Apple", 2.99, 1.5))
        visitor = get_tax_visitor()
        cart.accept(visitor)
        captured = capsys.readouterr()
        assert "Apple" in captured.out

    def test_visitor_visits_electronics(self, capsys):
        """Test that visitor can visit electronics"""
        cart = VisitorCart()
        cart.add_item(Electronics("Laptop", 999.99, True))
        visitor = get_tax_visitor()
        cart.accept(visitor)
        captured = capsys.readouterr()
        assert "Laptop" in captured.out

    def test_discount_visitor_applies_discount(self, capsys):
        """Test that discount visitor applies discount"""
        cart = VisitorCart()
        cart.add_item(Book("Book", "Author", 50.0))
        visitor = DiscountVisitor()
        cart.accept(visitor)
        captured = capsys.readouterr()
        assert "Discount" in captured.out
        assert "45.00" in captured.out  # 10% off

    def test_visitor_calculates_tax_for_eco_friendly(self, capsys):
        """Test that tax visitor gives eco discount"""
        cart = VisitorCart()
        cart.add_item(Electronics("Green TV", 500.0, True))
        visitor = get_tax_visitor()
        cart.accept(visitor)
        captured = capsys.readouterr()
        assert "Eco discount" in captured.out

    def test_helper_functions(self):
        """Test that helper functions create valid objects"""
        cart = get_visitor_cart()
        visitor = get_tax_visitor()
        assert isinstance(cart, VisitorCart)
        assert isinstance(visitor, TaxVisitor)
