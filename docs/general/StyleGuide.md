# OpenGSP Code Style Guide

## Interfaces

External interfaces to libraries should be 'C style' and so make use of simple 

## Architectural

Don't impose arbitrary size limits. Well written code should scale well on larger/more powerful hardware.



## Naming

All types, variables and functions to be in ``snake_case``.
Types to be appended with ``_t``.

All globally visible types and functions should be prefixed with a global module path.
E.g ``module_submodule_typename_t``.

Static globals to be prefixed with ``g_``.

## Types

``int`` for status return types (as can have -1, 0, 1).

``bool`` for flags.

``size_t`` for any situtation when imposing an arbitrary size limit isn't necessary.

## Includes

Each external include should have a justification for why it is needed.
E.g
``#include <stdio.h> // for printf()``

## Warnings/Errors

Info = something good/expected happened.

Warning = programmer error (e.g bad handle passed to function etc)
Error = system error (e.g buffer failed to allocate)

Critical = system error bad enough that the program will/needs to quit