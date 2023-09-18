# ASTRAFlightSoftware
Main ASTRA flight software.

## Setup

Download [platformio](https://platformio.org/). We recommend getting both the IDE and
the core CLI. Instructions will generally use the CLI to be IDE-agnostic.

## Structure and Standards

### Structure

- Source files are located in ./src.
- Header files are located in ./include.
- External libraries are located in ./lib.
- Test files are located in ./test.

### Standards

Code should be written in camelCase where possible, with 4-space indentation and
C-style braces. Methods should also have a 1 or 2 line comment describing their function. Example:
```c
/* This function
 * does a thing. */
void doSomething()
{
    Serial.println("Did something!");
}
```

Files should have a comment block at the top detailing what it contains, as well as
the person taking responsibility for it (so that questions can be routed to the correct
person):
```cpp
/**
 * comms.cpp
 * This file contains methods for communicating with the ground station.
 *
 * @author <vwangsf@gmail.com>
 */
```
(For source-header file pairs, both require a comment block, but one can just reference the other.)

For header files, always add include guards:
```cpp
#ifndef TRAJECTORY_H
#define TRAJECTORY_H
void trajectory();
#endif // TRAJECTORY_H
```

When referencing object properties and methods, *always* explicitly use the `this->` pointer:
```cpp
void StateLogic::state()
{
    return this->_state;
}
```
This helps make it easier to track where variables are defined.

For anything else not covered, write "good code" where possible - use your best judgement. Some rules of thumb:
1. If you have a function that is very long (200+ lines), consider splitting it.
2. If you have very deeply nested code (e.g. a chain of 20 if statements nested within each other), consider flattening or splitting into sub-functions.

These rules are not hard requirements, but they are intended to make code easier to read.
