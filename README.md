# EXOTIC.json

[![Build](https://img.shields.io/badge/build-passing-brightgreen)](#)
[![License](https://img.shields.io/badge/license-BSL%201.0-blue)](#)
[![C++ Version](https://img.shields.io/badge/C%2B%2B-23-blue)](#)
[![Status](https://img.shields.io/badge/status-pre--release-orange)](#)

**EXOTIC.json** is a high-performance, flexible, C++ library for manipulating, and serializing JSON data. Part of the **EXOTIC** collection of modern C++ libraries, it provides a powerful and customizable interface designed for performance, flexibility, and seamless integration with modern C++ applications.



---

## Overview

This project was originally part of my final-year Computer Science project at the Cégep de Rimouski. It involved a comprehensive introspection tool, similar in nature to a compiler, that parsed the entire source code of a C++ program to extract information, build a complete syntax tree based on C++ grammar, and generate a JSON file containing every detail (such as function parameter names, types, qualifiers, modifiers, etc.). The entire process was designed to enable another module I implemented to read this JSON and inject C++ code into the original project, specifically, code utilizing introspection-style syntax (e.g., `myClass::members::functions::foo::parameters::a::type`).

So, to pull this all together, I had to build a JSON library from scratch. For the API design, I drew significant inspiration from the Boost JSON library. As for the internal implementation, with a few exceptions, the design is almost entirely my own work.

As for the code quality, well, it’s not quite there; I didn't get the chance to clean everything up before finishing the project. However, that’s just been postponed, as I do intend to do it at some point. Regarding the code quality, it certainly isn't perfect. In fact, looking back at it now, writing this some six or seven months after coding it, I find the project quite ambitious and well-conceived. Still, the code hasn't aged very well; there are things in there that are borderline "illegal," a bit naive, and just plain bad, but they certainly reflect the skill level I was at back then.

For example, the decision to mandate CRTP is a monumental mistake in this project. I was going through a CRTP phase at the time, so it’s understandable... It adds immense complexity to the API and muddies the result. Yes, it offers better performance, but it clearly wasn't worth it.


TO BE FINISHED
