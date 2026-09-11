# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Scope of the tree

`/home/lynx/box/dcc/comp_geometry` is a course workspace for CC5502 Geometría Computacional (DCC), **not** a git repository. Only `comp_geoT1/` is one (`origin` = `git@github.com:MFSaavedra/comp_geoT1.git`) — run all `git` commands from there. Everything else is read-only reference material:

- `comp_geoT1/` — **the deliverable**; Tarea 1 source, plus `Tarea1_GeometrA_a.pdf` (the assignment spec).
- `Clase/` — lecture slides and the professor's C++ examples in `Clases_en_C_Archivos_y_ejemplos/ClaseCpp_Archivos/` (templates, operator overloading, `friend` I/O operators, `cassert`, a plain makefile). These are the idioms the course expects; mirror them rather than inventing house style.
- `libro/`, `Lecturas/` — de Berg *Computational Geometry*, O'Rourke *Computational Geometry in C*, and papers on floating-point robustness (Goldberg, Schirra).

Course material and the assignment are in Spanish; code identifiers, comments, and Doxygen blocks follow suit (`Punto`, `distancia`, not `Point`, `distance`).

## Current state

`Punto<T>` is implemented (`include/Punto.hpp`). `Vector.hpp` and `Poligono.hpp` exist but are **empty files**. `src/` holds only `.gitkeep` — there is no `main.cpp` yet — and `test/` is an empty directory, so **no test has been written and nothing is graded yet** (see the grading rule below).

`data/.gitkeep` and `docs/.gitkeep` are pending deletions in `git status`; the README still documents `data/` and `docs/` in its structure block and mentions neither `include/` nor `test/`, so that block is stale either way.

## Build and test

CMake, out-of-source. The test directory is `test/`, **singular** — not `tests/`:

```bash
cd comp_geoT1
cmake -S . -B build
cmake --build build
cd build && ctest --output-on-failure
```

GTest resolution is `find_package(GTest CONFIG QUIET)` first (Arch's `gtest` package ships `/usr/lib/cmake/GTest`), falling back to `FetchContent` of googletest `v1.17.0`. The entire test block — GTest lookup included — is skipped while `test/` has no `.cpp`, which is why the project configures with no network access today.

Three build-system behaviors that are not guessable from reading `CMakeLists.txt`:

- **New `test/*.cpp` files are picked up automatically.** The glob uses `CONFIGURE_DEPENDS`, so a plain `cmake --build build` detects the change, re-configures itself, and compiles the new file.
- **Adding `src/main.cpp` requires one manual `cmake -S . -B build`.** The `main` target is behind `if(EXISTS ...)`, which is evaluated only at configure time. It will *sometimes* appear without that step — but only when an unrelated `test/` change happens to trigger a re-configure, so do not rely on it.
- **`gtest_discover_tests` runs the test binary at build time** to enumerate `TEST()` cases. A crash during static initialization therefore fails the *build*, with an error that does not obviously point at test code.

`geo` is an `INTERFACE` library: it compiles nothing and exists only to propagate `include/` and `cxx_std_17` to whatever links it. Use the `INTERFACE` keyword on any `target_*` call against it — `PUBLIC`/`PRIVATE` are a hard configure error on that target type.

If a test uses `cassert` rather than GTest, **never pass `-DNDEBUG`**: it compiles every `assert` away, turning a failing suite into a silent pass.

## Architecture

Three templated value types in `include/`, layered: `Punto<T>` is the primitive, `Vector<T>` composes a `Punto<T>`, `Poligono<T>` stores a `std::vector<Punto<T>>`.

**Templates force a header-only design.** Every method body must live in the `.hpp` (or be explicitly instantiated), so `src/` should contain only `main.cpp` — do not create `Punto.cpp` / `Vector.cpp` / `Poligono.cpp` expecting them to link. A split build compiles cleanly and fails at link with `undefined reference`, because the definition TU emits no symbols for a template it was never told to instantiate.

**Why `Punto<T>` carries a `z`.** The spec gives `Punto` three private coordinates `x, y, z` while its constructor takes only two `T` values and `operator<<` prints `(x,y)`. That third coordinate exists to receive `Vector::cross`: the cross product of two 2D vectors is a scalar living on the z axis, but the spec requires `cross` to return *a new vector*. `Punto` already provides the three-argument constructor and `getZ()` for exactly this; the 2D printing contract stays intact.

**One primitive drives all of `Poligono`.** `esCCW()`, the CW→CCW conversion, and `area()` are three consumers of the same signed shoelace sum: sign answers orientation, reversing the point order flips it, absolute value halved gives the area. Implement the signed sum once and express all three in terms of it instead of writing three independent loops.

### Conventions `Punto<T>` established — match them in `Vector` and `Poligono`

`T` is instantiated with both integral and floating-point types, and `Punto` settles how that is handled. Diverging in the other two classes would be the inconsistency, not the fix:

- **Metric results return `double`, never `T`.** `distancia` returns `double` because truncating a root back to `int` turns `dist((0,0),(1,1))` into `1`. `Vector::magnitud` and `Poligono::area` have the same problem.
- **Promote to `double` before arithmetic that can overflow.** `distancia` casts before subtracting so large `int` coordinates cannot overflow the squared difference. The shoelace sum needs the same treatment.
- **Equality goes through `Punto::casiIgual`** — a private `static` helper using `if constexpr (std::is_floating_point_v<T>)` to pick a combined absolute+relative tolerance for floats and exact `==` for integers. Never compare floating-point coordinates with bare `==`.
- **Comparison operators are `const`-qualified**, though the spec writes them without `const`; the unqualified form fails on a `const` object.
- **`operator<<` is defined inline inside the class body as a `friend`.** That makes it implicitly `inline` and generates one non-template function per instantiation, which is what keeps it linking across multiple TUs without a forward declaration dance.
- **Doxygen on every member**: `@brief` always, plus `@param`/`@return`/`@tparam` where they apply, and `@note` reserved for code that looks wrong without an explanation. Keep blocks short — one line per tag.

`Punto` also has a default constructor (needed by `std::vector::resize`) and `getX/getY/getZ`, both beyond the spec's minimum but relied on by the other two classes.

## Assignment constraints

The spec (`comp_geoT1/Tarea1_GeometrA_a.pdf`) is the source of truth for the required members and their exact signatures — read it before adding or renaming a public method. Extra constructors, methods, and attributes are explicitly permitted.

- **Untested methods are not graded, and a submission with no tests at all receives the minimum grade.** Every public method needs a test; treat adding a method and adding its test as one unit of work. `cassert` or Google Test only — no other third-party libraries.
- A small `main` must instantiate all three classes and exercise the implemented methods.
- The README must explain how to run the assignment; it is currently a stub reading "_Por definir._" and must end up documenting the CMake commands above. The assignment permits a makefile and says CMake is not required — it does not forbid it, but the graders will follow whatever the README says, so the README is the binding artifact.

`.gitignore` excludes `*.pdf`, so the assignment spec, and any compiled LaTeX report, are untracked by design — don't "fix" this by force-adding them. It ignores `build/` (so out-of-source CMake output is covered, but an in-source configure would leave `CMakeCache.txt` and `CMakeFiles/` untracked), and `*.out` (intended for LaTeX) silently covers `a.out`-style binaries too.
