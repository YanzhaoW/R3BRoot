# NeuLAND Convention and Code Standard {#neuland_style}

[TOC]

To build a robust and readable software, programmers have to follow good coding standards and consistent conventions. The coding standards, either relating specific styles or following the C++ language rules, are created after decades of experience from various industries. The coding styles, probably different from companies to companies though, must be consistent throughout the whole project. The code related to NeuLAND follows strictly [the C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines) and **latest industrial standards**. Reasons why a certain rule exists or a style is preferred are also given in the following sections.

## Conventions

### ID v.s. number

All variables and functions named with `_id`, such as `module_id` is using 0-based indexing. This means if `id` is 3, it means the 4th bar or plane. On the other hand, all variables and functions named with `_num` is using 1-based indexing. For example, if `module_num` is 3, it means the 3rd module.

### Modules v.s. bars

_Modules_ are used in the context of referring to scintillators with respect to the whole detector, whereas _bars_ are used with respect to a scintillation plane. For example, `module_id` ranges from 0 to 1299 for 13 double planes, but `bar_id` only ranges from 0 to 49.

### Coordinate axes

* The positive side of the Z axis is in the direction which the beam particles are heading.

* The positive side of the Y axis is in the upward direction from ground.

* The positive side of the X axis (following the Right-hand Rule) is in the left direction when facing the front side of the NeuLAND detector.

### Right v.s. left

The right or the left side is always referring to the right or left side of the axis. For horizontal bars, (**ATTENTION!!!**) the left side is actually the right side when facing the front plane of NeuLAND. For vertical bars, the left side is the bottom side when facing the detector.

### Arrangement of bar id/number

The bars with smaller IDs or numbers of each plane are placed in positions with less position values. For example, in the first plane, bar with the bar number 1 is placed at the most negative position of the Y axis.


## Coding standards and styles

### Rule 0: Don't write C code

Always prefer **C++ ways** over the C ways (There is no such thing as "C/C++"!). This includes:

- Use `static_cast` or `dynamic_cast` instead of C cast.
- Use `constexpr` instead of C macro.
- Use `#pragma once` instead of include guard.

### Variable declaration and initialization

There are only two ways (both with **curly brackets**) to declare and initialize a variable:

1. (preferred) Use `auto`:

   ```c++
   auto value = float {1.};
   auto val_double = 0.;   // this is also ok. Implicit type is double.
   auto val_int = 1;       // this is also ok. Implicit type is int.
   ```

2. Type in the front:

   ```c++
   float a_float_value {1.};
   MyClass obj {};
   ```

_**Reason:**_ Using curly brackets `{}` makes sure the expression is an initialization instead of a function declaration. For example, `int val();` is a declaration for a function called `val` while `int val {};` is a variable initialization.

### AAA (Almost Always Auto)

Always use `auto` for the variable initialization and function declaration:

```c++
auto my_obj = MyClass{};

auto my_func(int input) -> double
{
    // ...
}
```

_**Reason:**_ Using auto (even for a simple integer) to initialize a variable **guarantees** the variable always get initialized. For example, `int i;` fails to initialize the variable but still compiles while `auto i = int;` never compiles.

### Use signed values over unsigned values
