//! Design Patterns in Rust
//!
//! This module contains implementations of all 23 Gang of Four design patterns.

// Creational Patterns (A1-A5)
pub mod a1_factory_method;
pub mod a2_abstract_factory;
pub mod a3_builder;
pub mod a4_prototype;
pub mod a5_singleton;

// Structural Patterns (B1-B7)
pub mod b1_adapter;
pub mod b2_bridge;
pub mod b3_composite;
pub mod b4_decorator;
pub mod b5_facade;
pub mod b6_flyweight;
pub mod b7_proxy;

// Behavioral Patterns (C1-C11)
pub mod c1_interpreter;
pub mod c2_template_method;
pub mod c3_chain_of_responsibility;
pub mod c4_command;
pub mod c5_iterator;
pub mod c6_mediator;
pub mod c7_memento;
pub mod c8_observer;
pub mod c9_state;
pub mod c10_strategy;
pub mod c11_visitor;
