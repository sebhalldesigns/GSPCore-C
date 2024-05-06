# OpenGSP Objective Design

OpenGSP is written entirely in ANSI C, so lacks built-in Object-Oriented functionality. Instead, OpenGSP is designed around an pseudo-objective design architecture.

The system promotes **Encapsulation** and **Abstraction** but it doesn't really support **Polymorphism** or **Inheritance**.

## External Interface

OpenGSP objects expose an external interface that centers around the concept of instancing.

- **Member functions** - Always take an instance as their first argument.
- **Static functions** - May take an instance as an argument, but usually don't. Examples include Allocators.

### Memory Management

OpenGSP relies on manual memory management for objects exposed to external interfaces. There are two types of functions associated with this:

- **Allocate Functions** - Names should with Alloc and return a handle to a new instance.
- **Free Functions** - Names should start with Free and return void.

## Internal Definition

All objects with external interfaces are defined internally, to describe their data structures in a way that allows for more differences between platforms. It also hides functionality from the end user, and prevents unwanted or undefined behavior.

Some objects are also internal-only and have an interface located within the internal source files. These are generally helper objects that aren't really suitable for external use (e.g GVector).

## Internal Implementation

All objects are then implemented in source files, generally with 1 source file per object. This means that for a given object, it will typically have 1 external interface file, 1 internal definition file and 1 implementation file. An exception to this is variadic objects. These are objects that have alternative implementations that are selected according to platform.

