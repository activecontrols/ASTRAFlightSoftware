# ASTRAFlightSoftware
Main ASTRA flight software.

## Setup

Clone this repository:
```sh
git clone --recursive https://github.com/activecontrols/ASTRAFlightSoftware.git # Or use the SSH repo link
```

The `recursive` flag is required to pull in external drivers.

Download [platformio](https://platformio.org/). We recommend getting both the IDE and
the core CLI. Instructions will generally use the CLI to be IDE-agnostic.

## Structure and Standards

### Structure

- Source files are located in `./src`.
- Header files are located in `./include`.
- External libraries are located in `./lib`.
- Test files are located in `./test`.

- Lower-level drivers are in `./drivers`. This is the property of the Avionics team,
so ask before making changes in here.

### Standards

Code should be written in camelCase where possible, with 4-space indentation and
Java-style braces. Methods should also have a descriptive multi-line comment describing their function.
Constants should be uppercase where possible.
Example:
```c
char * SOMETHING = "Did something!";

/* This function prints out the
String "Did something!" */
void doSomething() {
    Serial.println(SOMETHING);
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
void StateLogic::state() {
    return this->_state;
}
```
This helps make it easier to track where variables are defined.

After dynamically deallocating a pointer, the pointer should be set back to NULL.

For anything else not covered, write "good code" where possible - use your best judgement. Some rules of thumb:
1. If you have a function that is very long (200+ lines), consider splitting it.
2. If you have very deeply nested code (e.g. a chain of 20 if statements nested within each other), consider flattening or splitting into sub-functions.
3. Use descriptive and meaningful names for variables and functions. For example, a variable named t vs trajectory/traj
4. Closing brace should be on the same indentation level as the function/loop.

These rules are not hard requirements, but they are intended to make code easier to read.

Authors: Teresa Wan, Vincent Wang
