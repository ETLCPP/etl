---
title: "Scanning an arbitrarily rotated rectangular region"
weight: 3
---

**Author:** John Wellbelove  
**Date:** 2019  

Quite often in image processing we need to scan a rectangular region in an image. The region is usually aligned to capture a specific feature in the image.  
You may be presented with a skewed image, necessitating that the region should also be skewed.  

![ScanRectangle1](images/ScanRectangle1.png)

If it is not absolutely necessary to scan the region at the angle of skew, then the fastest way to interrogate the pixels is to scan the image horizontally.  



As was shown in the previous post, the crossing points between two lines can be easily and efficiently determined.  
By moving a reference line down across the region, the start and end x coordinates of each scan line can be found.  
Only the y coordinate of the reference line is relevant, though it must have a length of at least 1 unit.  

Of course, we must check every line in the rectangle, and every line in the rectangle may produce a crossing point.  
The reference line shown below will identify points `P1`, `P2`, `P3` and `P4` as valid crossing points.  

![ScanRectangle4](images/ScanRectangle4.png)

The solution is to compare the crossing coordinates against the bounds of the enclosing rectangle. 

![ScanRectangle5](images/ScanRectangle5.png)

This would eliminate points `P1` and `P4`. 

![ScanRectangle2](images/ScanRectangle2.png)

This can be done for all scan lines in the bounded rectangle.

![ScanRectangle6](images/ScanRectangle6.png)

Vertical scans may be easily achieved by simply having a vertical reference line.  
This technique can be applied to any convex hull.

