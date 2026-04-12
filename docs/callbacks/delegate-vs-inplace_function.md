---
title: "delegate vs inplace_function"
weight: 1
---

## Efficiency and performance
### Invocation cost

**delegate**  
One indirect call via stub function pointer; no allocation; forwards args directly; minimal overhead.  

**inplace_function**  
One indirect call via vtable invoke pointer; no allocation; overhead effectively the same as `delegate`.  

### Copy / move cost

**delegate**  
Trivial copy/move (object pointer + stub pointer).  

**inplace_function**  
Copies/moves the stored callable in an inline buffer; cost depends on callable traits; may run destructor on clear/reset.  

### Size / footprint

**delegate**  
Typically two pointers; very small, stable footprint.  

**inplace_function**  
Buffer + vtable pointer + object pointer; size depends on `Object_Size`/`Object_Alignment`.  

### Ownership / lifetime

**delegate**  
Non-owning for functors/lambdas; stores pointer to external callable; caller must ensure lifetime.  

**inplace_function**  
Owning; stores a copy/move of the callable inline (RAII).  

### When to prefer

**delegate**  
Minimal footprint, cheapest copies, free/member function binding, controlled lifetimes.  

**inplace_function**  
Value semantics, safe storage of lambdas with captures, uniform SBO wrapper.  

## External API differences

### Template shape and size

`delegate<TSignature>`  
`inplace_function<TSignature, Object_Size = 32, Object_Alignment = alignof(void*)>`

### Construction / binding

**delegate**  
`create<FreeFn>()`  
`create(T& obj, Method)` and `create(const T& obj, ConstMethod)` at run time  
`create<T, Method, Instance>()`  
Construct from functor/lambda by reference (non-owning); rvalues deleted to avoid dangling  
`set()` mirrors `create()` for re-binding  

**inplace_function**  
Constructor from function pointer  
Constructor from object + (const/non-const) member function (run time)  
Constructor from functor/lambda by (const) reference; callable stored inline (owning)  
`set()` mirrors constructors; also `set<FreeFn>()` and `set<T, Method, Instance>()` (compile time)  
`create<FreeFn>()`, `create<T, Method, Instance>()` (compile time)  
`make_inplace_function` helpers: `make_inplace_function(free_fn)`, `make_inplace_function(obj, &T::Method)` , `make_inplace_function<TSignature>(lambda)`.  

### Equality and swap

**delegate**  
`operator==`/`!=` compare stub and object; structural equality.  

**inplace_function**  
`operator==`/`!=` with `nullptr`; `swap()` provided; no general equality between two functions.

### Introspection helpers
**delegate**  
`delegate_tag`  
`is_delegate<T>`.

**inplace_function**  
`is_inplace_function<T>`.

### Call helpers
`is_valid()`  
`explicit bool`  
`call_if(...)`  
`call_or(alternative, ...)`  
`call_or<FreeFn>(...)`.

### Functor storage semantics
**delegate**  
Non-owning pointer to external functor/lambda.

**inplace_function**  
Owns a copy/move in fixed buffer.

### Size control
**delegate**  
Fixed small size (two pointers).

**inplace_function**  
User-controlled via `Object_Size`/`Object_Alignment`; aliases `inplace_function_for` and `inplace_function_for_any` to compute required size/alignment.

### C++ language features
Classic non-type template parameter APIs; C++17 "auto" `make_*` helpers exist when enabled.

## Performance differences: 

### Call overhead
**delegate**  
One indirect call through a stub function pointer; minimal overhead.

**inplace_function**  
One indirect call via `vtable->invoke(object, ...);` typically one extra pointer load; practically similar.

### Copy / move and lifetime
**delegate**  
Trivial POD-like copy/move (object pointer + stub); no construction/destruction of target.

**inplace_function**  
Copies/moves/destroys the stored callable in the inline buffer; cost depends on callable traits.

### Construction / binding cost
**delegate**  
Binding free/member functions sets two pointers; binding a functor stores its address (non-owning).

**inplace_function**  
Constructs a copy/move of the callable into the SBO buffer; more work up front.

### Memory footprint and cache locality
**delegate**  
~2 pointers regardless of target; high container density and good cache locality.

**inplace_function**  
Buffer + vtable pointer + object pointer; larger, size depends on Object_Size/Object_Alignment.

### Clear / reset cost
**delegate**  
Clears pointers; no destructor invocation.

**inplace_function**  
May run callable’s destructor; potentially non-trivial.

### Determinism / Real-time suitability
**delegate**  
Very predictable and minimal costs at call/copy/reset; good for ISR/RT paths.

**inplace_function**  
Deterministic at call; copy/move/reset depend on callable’s traits (still allocation-free).

## Rule of thumb
- Use `delegate` when:  
  You only bind free/member functions or non-owning functors and want the smallest handle with the cheapest copies.

- Use `inplace_function` when:  
  You must own captured state safely (value semantics) and still avoid the heap; accept slightly larger handle/copy costs for safety.

## Choose etl::delegate when
- You need the smallest possible callable handle (two pointers) for large arrays/vectors of callbacks.
- You must avoid any allocation, construction, destruction, or SBO buffer management in the callable wrapper.
- You bind free functions or member functions and the target object’s lifetime is externally guaranteed.
- You want trivial copy/move/equality semantics (copy two pointers; compare stub+object) for fast shuffling/lookup.
- You need deterministic, RT/ISR-safe behavior with no hidden destructors or move/destruct paths on clear/reset.
- You build static/ROM lookup tables of callbacks (compile-time create<Fn>(), create<T,Method,Instance>()).
- You store callbacks in fixed-capacity containers (etl::array/vector) and must minimize memory per element.
- You need ABI-stable, non-owning callback tokens passed across modules without copying callables.
- You already use etl::delegate_service or similar indexed dispatch; its API expects etl::delegate.
- You don't want to size/tune an inline buffer (Object_Size/Object_Alignment) per signature or platform.

### Typical scenarios
- Interrupt vectors, GPIO/event ISR callbacks, hard real-time control loops.
- Command/ID -> handler tables, message routing switchboards, state-machine transition actions.
- Global registries/singletons where handler objects are static or live for program lifetime.
- Shared tables of callbacks across DLLs/modules where owning captured state is undesirable.

### Why etl::delegate over etl::inplace_function in these cases
- Lower footprint: always two pointers versus user-sized SBO buffer + vtable pointer + object pointer.
- Lower copy/move cost: trivial POD-like copies; no callable copy/move/dtor required.
- Non-owning by design: avoids accidental copies of heavy callables; no lifetime surprises at clear/reset.
- Compile-time binding support: completely payload-free delegates for fixed targets (no storage touched).
- Equality: pointer-based equality lets you deduplicate/lookup handlers efficiently.

### Notes
- `etl::delegate` is non-owning; only choose it when the bound object outlives the delegate (rvalue lambdas are rejected by API).
- Prefer `etl::inplace_function`0 when you must own captured state (value semantics) or need to accept arbitrary lambdas safely.

## Choose etl::inplace_function when
- You need value semantics (own the callable) so lifetimes are safe without tracking external objects.
- You want to accept arbitrary lambdas with captures and store them inline using small-buffer optimization (no heap).
- You need a single uniform callable wrapper that erases the concrete type across modules/APIs.
- You plan to copy/move callbacks between threads/queues safely (callable is self-contained in the buffer).
- You must avoid dynamic allocation but still need to store captured state (tune Object_Size/Object_Alignment).
- You want compile-time or run-time binding to free functions, member functions, and functors in one type.
- You need reset/clear semantics that correctly destroy the stored callable (RAII).
- You prefer exceptions on misuse (invoking uninitialized) to surface bugs early.  

### Typical scenarios
- Job systems, task queues, and executors that capture small state per task without heap churn.
- Pipelines and event buses that accept user-provided lambdas with captures.
- Configurable command tables where handlers carry lightweight configuration/state.
- Deferred work items, timeouts, and continuations stored in fixed-capacity containers.
- API boundaries returning/storing callbacks where the provider shouldn't manage the callee's lifetime.

### Why etl::inplace_function over etl::delegate in these cases
- Ownership: inplace_function owns the callable; no external lifetime management needed.
- Safety with captures: stores copies/moves of lambdas/functors; no dangling pointers to external objects.
- Uniform wrapper: a single type covers free/member/functor targets with the same call site and storage.
- No heap: SBO avoids allocations while still allowing captured state; predictable embedded-friendly footprint.
- Destruction: clear()/reset() run the callable's destructor when needed; no leaks or latent state.

### Notes
- Tune the buffer size/alignment to the largest expected callable; too-small buffers will `static_assert`.
- Copy/move cost depends on the callable; prefer lightweight captures for best performance.
- Invocation overhead is one indirect call through a vtable stub (similar to delegate).
- Use make_inplace_function helpers or create<> to bind targets; is_valid(), call_if(), and call_or() are available.
- If you only bind free/member functions and can guarantee lifetimes, etl::delegate is smaller and cheaper to copy.
