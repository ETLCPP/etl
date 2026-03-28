---
title: "About"
weight: 2003
---

*"Do It Once - Do it Right - So you don't have to do it again"*  
J Wellbelove : 2014

## Why write this library?
I wrote this library, and all the others I have written over the
years, because I'm lazy.

Yes, lazy!

One of the things I really hate when writing software is having to do the same, or something almost the same, over and over again. The first thing I think when presented with a problem that requires a specific set of functionality is "Is this a specific case of a more generic problem?". Surprisingly, I can say "yes" more often than you would expect. Even if not all of the problem can be seen as generic, there are almost certainly parts that are. In every job I've had I've left an extensive code library behind me.

Reinventing the wheel every time is daft for many reasons.

### Code bloat
Multiple instances of slight variations of a theme results in an increase in code size due to no commonality of functionality.

Testing, more testing or no testing
Are all the variants tested to the same degree? Are some tested at all?

### Variable functionality
Not all the variants are going to have the same level of functionality, or the same API. Ad-hoc solutions are invariably only going to solve the little bit of the problem that was needed at the time.

This goes against the YAGNI1 principle so popular in some programming dogmas.
I think that YAGNI is often just another way of saying ISEP2.
I believe in GIRFT3.
I have over three decades of empirical proof.

### No collective knowledge base
Without commonality every new variant has to be learned. The underlying principles may be understood (i.e. Linked list), but each implementation has to be understood separately, along with its particular caveats and foibles.

Documentation is likely to be patchy, if it exists at all.

### Octopus code
The application is liable to have a close coupling with the solution. For example, I've often seen code using linked lists directly accessing the node pointers. Ad-hoc solutions are liable to have lazy (the bad kind) implementations.

## About me
I have been involved in technology and computer systems for all of my working life and have amassed considerable knowledge of designing and implementing systems that are both performant and correct.  
My role normally encompasses the entire project life-cycle, from specification to maintenance phase. 

Most systems I have worked on have required high speed and deterministic performance, often within a highly constrained platform. I am experienced in designing and adapting algorithms to solutions that are both space and time efficient, avoiding the normal overheads of standard solutions.

Acting as a mentor for colleagues has often been a significant, though unofficial, part of my role.
