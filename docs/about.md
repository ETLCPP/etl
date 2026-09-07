---
title: "About"
weight: 3003
---

## About me
I have been involved in technology and computer systems for all of my working life and have amassed considerable knowledge of designing and implementing systems that are both performant and correct.  
My role normally encompasses the entire project life-cycle, from specification to maintenance phase. 

Most systems I have worked on have required high speed and deterministic performance, often within a highly constrained platform. I am experienced in designing and adapting algorithms to solutions that are both space and time efficient, avoiding the normal overheads of standard solutions.

Acting as a mentor for colleagues has often been a significant, though unofficial, part of my role.  

## Why write this library?
The truth behind this library, and every other tool I've built over the years, is simple: 

I’m lazy.

One of my biggest frustrations in software engineering is having to write the same code multiple times. Whenever I'm faced with a new problem, the very first question I ask is: *"Is this just a specific instance of a more generic problem?"*

More often than not, it is. Even if the entire problem isn't generic, core pieces of it usually are. I build tools to solve those pieces once and for all. It's a philosophy that has defined my career; at every company I've worked for, I've left behind a legacy of extensive code libraries that teams still use today.

Reinventing the wheel every time is a bad idea. When teams rely on fragmented, ad-hoc solutions, a predictable set of problems always follows:

*   **Code Bloat:** Multiple variations of the same theme increase code size because there is no shared functionality.
*   **Patchy Testing:** Are all the variants tested to the same degree? Are some tested at all? Usually, it's a roll of the dice.
*   **Variable Functionality:** Not all variants share the same level of features or even the same API. Ad-hoc solutions invariably only solve the tiny sliver of the problem that was needed at the exact moment they were written.

This directly conflicts with the popular **YAGNI** principle, but I think **YAGNI** can often be just another way of saying **ISEP** (It's Somebody Else's Problem). 

Personally, I believe in **GIRFT** (Get It Right First Time). I have over three decades of empirical proof to back it up.

Without a centralized, generalized library to solve these core problems, you also run into:

*   **No Collective Knowledge Base:** 
Without commonality, every new variant has to be relearned from scratch. Even if the underlying concepts are understood, each separate implementation brings its own caveats, foibles, and patchy documentation.
*   **Octopus Code:** 
Applications end up tightly coupled to the underlying solution. I've often seen code directly accessing the underlying implementation. Ad-hoc solutions are highly vulnerable to this kind of lazy engineering *(the bad kind)*.

### The Antidote
To prevent this, this library provides robust, centralized building blocks out of the box:
*   **Compile-Time Features:** Templated compile-time constants.
*   **Design Patterns:** Template-based design pattern base classes (such as Visitor and Observer).
*   **Type Safety:** Type-safe smart enumerations, typedefs, and constants.

---

**YAGNI** - *You Aren't Going To Need It*

## Why not use 'C'?
**Greenspun's Tenth Rule** has an unofficial C corollary:  
*"Any sufficiently advanced C program contains an ad hoc, informally specified, bug-ridden, slow implementation of half of C++."*

I spent 12 years programming in pure C.  
I discovered that I had been reverse engineering C++ all that time.  
