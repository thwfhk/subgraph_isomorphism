Implementations of some subgraph isomorphism algorithms.

## Dataset

### AIDS Antiviral Screen Data

[link](https://wiki.nci.nih.gov/display/NCIDTPdata/AIDS+Antiviral+Screen+Data)

Properties:

- small: on average, each graph has 25.4 vertices and 27.3 edges; the size of the largest graph is less than 300 vertices.

- sparse: the number of edges is almost the same as the number of vertices;

- label: most of the labels are C, O, H, N, S;

## How to test

Replace the `#include "GraphQL/GraphQL.cpp"` in `test.cpp` with your code file.

You need to provide a `bool solve(Graph P, Graph G)` function which does the subgraph isomorphism test in your code file.

Compile: `g++ test.cpp GraphDS.cpp -std=c++11 -Wall -o test`

(Boost Lib is required for VF2.)

Run `./test` to test.

Notice: *Make sure your code use the `Graph` structure defined in `GraphDS.h` and `GraphDS.cpp`.*

## Test

Random draw 100 graphs and find a random subgraph of each graph to form a query set.
Do subgraph isomorphism test with every pair of the set of 100 graphs and the query set, totally 10000 times.

Results on GraphQL:

```
average time of queryset 4: 0.000182
total match 4302 / 10000
average time of queryset 8: 0.000335
total match 985 / 10000
average time of queryset 12: 0.000488
total match 226 / 10000
average time of queryset 16: 0.000630
total match 118 / 10000
average time of queryset 20: 0.000807
total match 97 / 10000
average time of queryset 24: 0.000978
total match 65 / 10000
```