# Design patterns

This repo aims to explain all the 23 design patterns in a systematic way, and provide concrete examples to use each of them. The goal is to help you build a system of these design patterns. Whenever you were given questions related to a design pattern, you could be confident to answer and explain it.

## 1. A clever way to remember all 23 design patterns

There are 5 builder type patterns, 7 structure related patterns, and 11 behavioral patterns. The 11 behaviroal patterns can be further divided into 5+4+2 patterns and the reason will be explained later.

For builder type patterns, we have two factories: (1) Factory Method, (2) Abstract Factory. Then because of builder type, we have (3) Builder. You will build a prototype before making it perfect, so we think of (4) Prototype. Finally, there is a pattern where only one instance could exist, that is (5) Singleton.

For structural patterns, we have ABCD, double Fs plus a proxy. ABCD are (6) Adaptor, (7) Bridge, (8) Composite, (9) Decorator. Double Fs are (10) Facade and (11) Flyweight. Plus, we have (12) Proxy.

For behaviroal patterns, we have the first group of five starting from (13) Interpretor and ending at (17) Iterator. The middle ones are (14) Template Methods, (15) Chain of Responsibilities, (16) Command.

The second group is in the middle so we think of (18) Mediator. Remember we have 4 items in the second group, we need to note it down, so we think of (19) Memo. Someone is looking at us to check our note states, and thus we think of (20) Observer (21) State.

Finally, there are (22) Strategy and (23) Vistor. They are often used together so they are in their own group.

In this way, you can easily remember all the 23 design patterns and recite it in whatever order. Next, we will start trying to understand what they actully do.
