---
title: Spring–Mass–Damper Simulator
summary: An interactive simulation of a damped oscillator, written in C++ and compiled to run in the browser.
date: 2026
status: In progress
tags: C++, WebAssembly, System dynamics
order: 10
---
## Goal

Model the classic second-order system

`m·x'' + c·x' + k·x = F(t)`

and let visitors change the mass, damping and stiffness to see underdamped, critically damped and overdamped responses in real time.

## Plan

- Write the physics and a numerical integrator (RK4) in plain C++
- Compile it to WebAssembly with Emscripten
- Draw the moving mass and the response curve on an HTML canvas

> The live simulation will appear on this page once it's built.
