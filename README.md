# Linear Hashing Voter Registry (`mvote`)

An in-memory voter registry written in C on top of a **linear hashing** table that I implemented from scratch. The table grows one bucket at a time as it fills, so it never has to rehash everything at once, and it keeps lookups fast at any size.

> Built for the *Operating Systems* course (Dept. of Informatics & Telecommunications, University of Athens, Fall 2023).

## How the hash table works

- **Buckets with overflow chains.** Each bucket holds up to `-b <bucketentries>` records. When a bucket fills up, an overflow bucket is chained to it.
- **Linear hashing.** The table tracks a `level` and a split pointer `p`. Keys hash with `h_level(k) = k mod (2^level · 2)`. When the load factor `keys / (buckets · bucketentries)` goes above **0.75**:
  1. a new bucket is appended (`realloc`)
  2. bucket `p` and its overflow chain are **split**: its records are rehashed with `h_level+1` into bucket `p` or the new one
  3. `p` moves forward. When every bucket of the current round has been split, `level` goes up and `p` goes back to 0.
- **Lookup** uses `h_level(k)`. If the result is below `p`, that bucket was already split this round, so it uses `h_level+1(k)` instead.

This grows the table gradually: every insert costs O(1) amortized, with no large rehash pauses.

## Commands

The program loads a CSV of voters (`pin lastname firstname zip`) and then reads commands from `stdin`:

| Command | Description |
|---|---|
| `l <pin>` | Look up a voter |
| `i <pin> <lname> <fname> <zip>` | Insert a new voter |
| `m <pin> [pin ...]` | Mark one or more voters as having voted |
| `bv <file>` | Bulk-vote: mark every PIN listed in a file |
| `v` | Number of people who have voted so far |
| `perc` | Fraction of registered voters who have voted |
| `t` | Total registered voters |
| `exit` | Quit |

## Build & run

```bash
make
./mvote -f data/voters5000.csv -b 4
```

```text
l 100093
100093 WIGGINS DEAN 4010 N
m 100093
100093 Marked Voted
t
Total Voters 5000
perc
0.000200
exit
```

The sample datasets in `data/` were provided with the course assignment.

## Project structure

```
mvote.c                   command loop and argument parsing
hashtable_implementation.c linear hashing: hash, split/rehash, insert, search, mark
hashtable_interface.h     data structures (buckets, overflow chains, voter records)
ziplist_*                 per-zip-code index (work in progress)
```

## Author

**Giorgos Fragkakis** · [GitHub](https://github.com/fragk17)
