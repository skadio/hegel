# Hegel: Dialectic Search Solver for the Set Cover Problem

This is a re-implementation of the Hegel Dialectic Search Solver for Set Cover Problem.

[The Set Cover Problem](https://en.wikipedia.org/wiki/Set_cover_problem) is a fundamental combinatorial optimization problem. At the time of its publication, Hegel solver found previously unknown solutions to the well-studied problem instances from the [OR-Lib Benchmarks](http://people.brunel.ac.uk/~mastjjb/jeb/orlib/scpinfo.html). 

The algorithm was first introduced in:

```bibtex
  @inproceedings{DBLP:conf/cp/KadiogluS09,
  author    = {Serdar Kadioglu and Meinolf Sellmann},
  title     = {Dialectic Search},
  booktitle = {Principles and Practice of Constraint Programming - {CP} 2009, 15th 
               International Conference, {CP} 2009, Lisbon, Portugal, September 20-24,
               2009, Proceedings},
  series    = {Lecture Notes in Computer Science},
  volume    = {5732},
  pages     = {486--500},
  publisher = {Springer},
  year      = {2009},
  url       = {https://doi.org/10.1007/978-3-642-04244-7\_39},
  doi       = {10.1007/978-3-642-04244-7\_39}
}
```

The exact code from that paper will never be released due to Brown University's policies. This version was created by Serdar Kadioglu after his doctorate thesis was completed and he was no longer a student of Brown. The core of the algorithm is true to one in the paper but results may be slightly different than in the paper.

Furthermore, this code is provided on an as-is basis under the [GNU GPL v3](LICENSE) and may only be used for **academic purposes**.

## Quick Start

```c++
# To Compile:
# /home/Desktop/hegel$ make main

# To Run: ./hegel <Input_File> <Number_of_Runs> <Search_Time_Limit>
# /home/Desktop/hegel$ make ./hegel input/scpa2.txt 2 5

# Example Output
Number of selected columns: 39
Selected Columns: 2321 2261 2423 141 1845 2767 2233 2011 781 1352 1261 1293 1442 468 2951 2802 1324 2424 2730 440 1881 1818 1550 1875 2731 1325 708 540 2166 921 2323 2799 1467 1284 2898 2667 2901 1821 1615 
Number of selected columns: 39
Selected Columns: 2321 2261 2423 141 1845 2767 2233 2011 781 1352 1261 1293 1442 468 2951 2802 1324 2424 2730 440 1881 1818 1550 1875 2731 1325 708 540 2166 921 2323 2799 1467 1284 2898 2667 2901 1821 1615 
Input: input/scpa2.txt
Total runtime: 10
Avg. runtime: 5
Avg. solution quality: 39
StDev solution quality: 0
Best solution quality: 39
Worst solution quality: 39
```

## Installation

The library is written in C++. To compile usign the makefile type ```make main``` and to run type ```hegel```.

## License

Hegel is licensed under the [GNU GPL v3](LICENSE) and may only be used for **academic purposes**.


