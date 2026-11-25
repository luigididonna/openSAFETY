# Two-node openSAFETY demonstrator

This is the integration project for turning the protocol stack into runnable
software nodes. It is kept separate from `src/eplssrc/` so the upstream stack
and its original paths remain intact.

## Current status

- The upstream SN and SCM C sources compile as the `opensafety_stack` static
  library with the 32-bit MinGW toolchain.
- A runnable node still needs its SHNF transmit/receive adapter, application
  callbacks, a configured Safety Object Dictionary, and a cyclic main loop.
- No two-node communication is claimed yet. The next milestone is a local
  transport proof of SPDO exchange; MQTT is deliberately postponed.

The 32-bit target is intentional. This legacy 1.4 stack stores pointers in
32-bit fields in some configurations, so a 64-bit build is not a valid runtime
target even if the compiler can produce a library.

## Integration layout

The planned separation is:

```text
examples/two-nodes/
  app/          node and manager application entry points, callbacks, SOD
  transport/    replaceable SHNF transport adapter
  config/       per-node protocol and object-dictionary configuration
```

The first transport will be local UDP/loopback for easy observation. The
protocol frames and SHNF boundary will remain independent of UDP so a later
MQTT transport can be evaluated without rewriting the openSAFETY application.

## Stack library build

From the repository root, with CMake and a 32-bit MinGW GCC toolchain installed:

```powershell
cmake -S . -B build -G "MinGW Makefiles" `
  -DCMAKE_C_COMPILER=C:/toolchains/mingw32/bin/gcc.exe
cmake --build build --target opensafety_stack
```

This builds the protocol library only; it does not create runnable nodes yet.
