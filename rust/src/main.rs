// Design Patterns in Rust
//
// This file demonstrates all 23 Gang of Four design patterns implemented in Rust.

mod a1_factory_method;
mod a2_abstract_factory;
mod a3_builder;
mod a4_prototype;
mod a5_singleton;

mod b1_adapter;
mod b2_bridge;
mod b3_composite;
mod b4_decorator;
mod b5_facade;
mod b6_flyweight;
mod b7_proxy;

mod c1_interpreter;
mod c2_template_method;
mod c3_chain_of_responsibility;
mod c4_command;
mod c5_iterator;
mod c6_mediator;
mod c7_memento;
mod c8_observer;
mod c9_state;
mod c10_strategy;
mod c11_visitor;

fn main() {
    println!("╔════════════════════════════════════════════════════════════╗");
    println!("║       Design Patterns in Rust - All 23 Patterns          ║");
    println!("╚════════════════════════════════════════════════════════════╝");

    // Creational Patterns (A1-A5)
    println!("\n═══ Creational Patterns ═══");
    a1_factory_method::example();
    a2_abstract_factory::example();
    a3_builder::example();
    a4_prototype::example();
    a5_singleton::example();

    // Structural Patterns (B1-B7)
    println!("\n═══ Structural Patterns ═══");
    b1_adapter::example();
    b2_bridge::example();
    b3_composite::example();
    b4_decorator::example();
    b5_facade::example();
    b6_flyweight::example();
    b7_proxy::example();

    // Behavioral Patterns (C1-C11)
    println!("\n═══ Behavioral Patterns ═══");
    c1_interpreter::example();
    c2_template_method::example();
    c3_chain_of_responsibility::example();
    c4_command::example();
    c5_iterator::example();
    c6_mediator::example();
    c7_memento::example();
    c8_observer::example();
    c9_state::example();
    c10_strategy::example();
    c11_visitor::example();

    println!("\n╔════════════════════════════════════════════════════════════╗");
    println!("║           All patterns demonstrated successfully!         ║");
    println!("╚════════════════════════════════════════════════════════════╝");
}
