---
title: "Implementing a moving average"
weight: 10
author: John Wellbelove
---

**Author:** John Wellbelove  
**Date:** 2019  

## Example
Imagine we have accumulated 10 values and our window size is 10.  

`[ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 ]`  

The average of this sequence is `(1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10) / 10 = 5.5`  

We now add a new sample, `11` to the sequence. As the windows size is 10, we drop the first value.  
`[ 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 ]`

The average of this sequence is `(2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10 + 11) / 10 = 6.5`  
Now, averaging over 10 samples each time is not that big an issue, but what if it's 1000 or 10,000? Then, the overhead, both in processing time and storage has started to become a little excessive.  
In the case of 10,000 samples of 'double' we are looking at 10,000 additions + 1 division each time a sample is added and a likely storage requirement of 80,000 bytes!  

So we have an algorithm that has processing and storage complexity of O(N).  

## What other values give the same average?

**So let's look at the problem again.**  

There are many ways of getting the same average:  

`[ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 ]` = Average of `5.5`  
`[ 7, 7, 7, 7, 7, 4, 4, 4, 4, 4 ]` = Average of `5.5`

---

**But what about this?**  
`[ 5.5, 5.5 , 5.5, 5.5, 5.5, 5.5, 5.5, 5.5, 5.5, 5.5 ]` = Average of `5.5`  

An average of `5.5` can be got from 10 samples of `5.5`.  

## So why store 10 samples? 

We can simply simulate the original sum just by multiplying the current average by the number of samples.  
The calculation of the average for adding a new sample becomes this:  
`(Old_Average * Sample_Size) + New_Sample) / (Sample_Size + 1)`  

Now we have 1 multiplication, 2 additions and 1 division.  

Our algorithm now has a complexity of O(1).

We can average over as large a sample size as we like and it will always take the same amount of time and use the same amount of storage. We can also change the sample size on-the-fly if we wish.  

There is one downside in that the average will not exactly match the original version, as the actual oldest value is not being erased from the sum, but for the performance and storage advantages it may be a perfectly suitable solution.  

You may find that you will need a slightly smaller sample size with the rolling mean to get similar results.  

---

**Note**  
The multiplication may be elided by re-arranging the formula.

`Old_Average + ((New_Sample - Old_Average) / (Sample_Size + 1))`  

This may produce a *negative* interim value, which may be a problem if you are using scaled unsigned integral types for the average value.

---

The ETL has an implementation of this algorithm.  
[pseudo_moving_average]({{% relref "../maths/pseudo_moving_average.md" %}})
