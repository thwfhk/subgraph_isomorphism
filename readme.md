Implementations of some subgraph isomorphism algorithms.

## Dataset

### AIDS Antiviral Screen Data

[link](https://wiki.nci.nih.gov/display/NCIDTPdata/AIDS+Antiviral+Screen+Data)

Properties:

- small: on average, each graph has 25.4 vertices and 27.3 edges; the size of the largest graph is less than 300 vertices.

- sparse: the number of edges is almost the same as the number of vertices;

- label: most of the labels are C, O, H, N, S;

### DBLP

dblpxxx中选取了DBLP数据集中100个大小接近xxx的community。具体如下：
```
dblp1000.data meta info:
sum: 103416
aver: 1034
max: 1389
min: 775
edge aver: 2986
---
dblp500.data meta info:
sum: 51016
aver: 510
max: 592
min: 433
edge aver: 1368
---
dblp200.data meta info:
sum: 20335
aver: 203
max: 222
min: 180
edge aver: 489
---
dblp100.data meta info:
sum: 10321
aver: 103
max: 117
min: 89
edge aver: 249
```
随机生成编号：高斯分布N(15,5)，限制在[0,30]上的整数


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

aids:
```
average time of queryset 4: 0.000200
total match 4302 / 10000
average time of queryset 8: 0.000350
total match 977 / 10000
average time of queryset 12: 0.000507
total match 222 / 10000
average time of queryset 16: 0.000686
total match 115 / 10000
average time of queryset 20: 0.000856
total match 97 / 10000
average time of queryset 24: 0.001040
total match 65 / 10000
```

dblp 500:
```
average time of queryset 4: 0.000777
total match 3989 / 10000
average time of queryset 8: 0.001373
total match 476 / 10000
average time of queryset 12: 0.002087
total match 204 / 10000
average time of queryset 16: 0.003107
total match 141 / 10000
average time of queryset 20: 0.003440
total match 119 / 10000
average time of queryset 24: 0.003865
total match 105 / 10000
```