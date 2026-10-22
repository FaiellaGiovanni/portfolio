# sims/ — WebAssembly simulations (next step)

This folder will hold the C++ simulations that run in the browser.

Plan for the first one, the spring–mass–damper:

1. `spring_mass_damper.cpp` — the physics: state (x, v), parameters (m, c, k), and an RK4 step function.
2. Compile with Emscripten (`em++`) into `static/sims/smd.js` + `smd.wasm`.
3. A small JavaScript file draws the mass and the response curve on a `<canvas>` and calls the C++ step function every frame.
4. The project page embeds it with a few lines of raw HTML in its Markdown file.

Nothing here is used by the site generator yet.
