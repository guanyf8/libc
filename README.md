# LibC
LibC provides C implementations of data structures and typical algorithms for general use. Here are what we've done. 

## Data Structure
* [Sequential Stack](./include/stack.h)
* [Cycular Queue](./include/queue.h)
* [AVL Tree and Binary Searching Tree based on it](./include/avltree.h)
* [Concurrent RB Tree (based on linux kernel 2.6)](./include/rbtree.h)
* [Concurrent Map based on rb tree](./include/concurrent_map.h)
* [hashtable by double hashing with dynamic resize](./include/hashTable.h)
* [complete binary tree and min heap based on it](./include/complete_tree.h)
* [an array min heap](./include/bin_heap.h)
* [linked list, double linked list (based on linux kernel 2.6)](./include/list.h)
* [concurrent linked hash map with dynamic resize and treeify to rbtree](./include/hashMap.h)
* [graph (adj list)](./include/graph.h)
* [LCRS tree](./include/LCRS_tree.h)


## Algorithm
* [dijkstra algorithm](./include/graph.h)
* [critical path](./include/graph.h)
* [prim algorithm](./include/graph.h)
* [topological sort](./include/graph.h)

* [merge sort](./include/sort.h)
* [quick sort](./include/sort.h)
* [m-way merge sort](./include/sort.h)

## application examples
* [huffman tree](./example/huffman_tree.c)
* [hanoi(non recursive)](./example/hanoi.c)
* [polynomial calculation](./example/polynomial.c)
* [infix_to_postfix expression and infix calculaiton](./example/postfix.c)
* [4-way huffman tree](./example/four_huffman_tree.c)
* [simulated memory management](./example/memory.c)