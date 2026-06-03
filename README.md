# Automata Simulator — DFA & DPDA

A C++ implementation of two classical automata simulators: a **Deterministic Finite Automaton (DFA)** and a **Deterministic Pushdown Automaton (DPDA)**. Both simulators validate the machine definition, run input strings step-by-step, and report detailed execution traces.

---

## Features

### DFA Simulator
- Full machine validation (states, alphabet, transitions)
- Detects **unreachable states** and **dead states**
- Checks whether the **language is empty**
- Automatically adds a `DEAD` state for missing transitions
- Step-by-step execution trace per input string

### DPDA Simulator
- Full machine validation including **determinism enforcement**
- Supports **epsilon (λ) transitions**
- Two acceptance modes: **Final State** and **Empty Stack**
- Step-by-step execution trace with **stack state** after every transition

---

## Build

Requires **CMake ≥ 3.10** and a **C++17** compiler.

```bash
cmake -S . -B build
cmake --build build
```

This produces two executables inside `build/Debug/`:
- `dfa_simulator`
- `dpda_simulator`

---

## Running

Both simulators read from **stdin** interactively.

```bash
./build/Debug/dfa_simulator
./build/Debug/dpda_simulator
```

Or pipe a prepared input file:

```bash
./build/Debug/dfa_simulator < input.txt
./build/Debug/dpda_simulator < input.txt
```

---

## Input Format

### DFA

```
States: q0 q1 q2
Alphabet: a b
Start state: q0
Final states: q2
Number of transitions: 3
q0 a q1
q1 b q2
q2 a q2
Number of test strings: 2
ab
aba
```

**Transition format:** `<from_state> <symbol> <to_state>`

---

### DPDA

```
States: q0 q1 q2
Input alphabet: a b
Stack alphabet: Z A
Start state: q0
Initial stack symbol: Z
Final states: q2
Acceptance mode: final
Number of transitions: 4
q0 a Z q1 AZ
q1 a A q1 AA
q1 b A q2 eps
q2 b A q2 eps
Number of test strings: 2
aabb
ab
```

**Transition format:** `<from_state> <input_symbol> <stack_top> <to_state> <push_string>`

- Use `eps` as input symbol for epsilon transitions
- Use `eps` as push string to pop without pushing (i.e. just pop)
- Push string is pushed **left-to-right**, so `AZ` means `A` ends up on top

**Acceptance modes:** `final` or `empty`

---

## Output

### DFA Example

```
--- DFA Analysis ---
q1 is an unreachable state.
The language of the DFA is not empty.

--- Running Strings ---
--------------------
Input string: ab
Start at state: q0
Read 'a' -> move from q0 to q1
Read 'b' -> move from q1 to q2
Halted at state: q2
Result: Accepted
--------------------
```

If the machine enters the `DEAD` state, execution halts immediately:

```
Read 'b' -> move from q0 to DEAD
Entered DEAD state.
Execution halted early.
Result: Rejected
```

---

### DPDA Example

```
--------------------
Input string: aabb
Acceptance mode: final

State: q0 , Stack: Z

Read a ->  Popped Z and Pushed AZ
State: q1 , Stack: AZ

Read a ->  Popped A and Pushed AA
State: q1 , Stack: AAZ

Read b ->  Popped A and Pushed nothing
State: q2 , Stack: AZ

Read b ->  Popped A and Pushed nothing
State: q2 , Stack: Z

Halted at state: q2
Result: Accepted
--------------------
```

---

## Project Structure

```
.
├── include/
│   ├── DFA.hpp
│   ├── DPDA.hpp
│   └── GLOBAL_VARIABLES.hpp
├── src/
│   ├── DFA/
│   │   ├── DFA.cpp
│   │   └── main.cpp
│   ├── DPDA/
│   │   ├── DPDA.cpp
│   │   └── main.cpp
│   └── GLOBAL_VARIABLES.cpp
└── CMakeLists.txt
```

---

## Error Handling

Both simulators throw descriptive exceptions for invalid input, including:

| Error | Cause |
|-------|-------|
| `InitialStateNotFound` | Start state not in states set |
| `FinalStatesNotFound` | A final state not in states set |
| `InvalidTransitionStartEnd` | Transition references unknown state |
| `InvalidTransitionSymbol` | Transition symbol not in alphabet |
| `MultipleTransitions` | DFA has two transitions for same (state, symbol) |
| `PreviouslyDefinedTransition` | DPDA transition already defined for this (state, input, stack_top) |
| `LambdaTransitionExists` | Adding a regular transition when an epsilon one already exists for same (state, stack_top) — violates determinism |
| `InvalidAcceptanceMode` | Acceptance mode is not `final` or `empty` |

All errors are caught in `main` and printed to `stderr`.