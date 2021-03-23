# simpledb

A C++ version of [SimpleDB](http://www.cs.bc.edu/~sciore/simpledb/) originally written in Java by Edward Sciore. The structure of SimpleDB is explained in detail in the book [Database Design and Implementation](https://www.springer.com/gp/book/9783030338350).

# About The Project

![Gif](https://raw.github.com/wiki/wattlebirdaz/simpledb/movies/sample.gif) 

simpledb is a database engine that can handle a subset of SQL.
The following is the examples of commands which can be accepted by simpledb.

- "select F1, F2 from T1, T2 where F3=F4";
- "insert into T(F1, F2, F3) values ('a', 'b', 'c')";
- "delete from T where F1=F2";
- "update T set F1='a' where F1=F2";
- "create table T(F1 int, F2 varchar(9))";
- "create view V as select F1, F2 from T1, T2 where F3=F4;
- "create index I on T(F)";

The list of major features mentioned by the book and whether it has been implemented in this project are shown in the following table.

| Book Chapter | Feature                                    | Implemented        |
|--------------|--------------------------------------------|--------------------|
| 3            | File Manager                               | :heavy_check_mark: |
| 4            | Log Manager                                | :heavy_check_mark: |
| 4            | Buffer Manager                             | :heavy_check_mark: |
| 5            | Recovery Manager                           | :heavy_check_mark: |
| 5            | Concurrency Manager                        | :heavy_check_mark: |
| 5            | Transaction                                | :heavy_check_mark: |
| 6            | Record Pages                               | :heavy_check_mark: |
| 6            | Table Scans                                | :heavy_check_mark: |
| 7            | Metadata Manager                           | :heavy_check_mark: |
| 8            | Select Scans, Project Scans, Product Scans | :heavy_check_mark: |
| 9            | Parser                                     | :heavy_check_mark: |
| 10           | Planner                                    | :heavy_check_mark: |
| 11           | Embedded JDBC Interface                    | :heavy_check_mark: |
| 11           | Remote JDBC Interface                      | :x:                |
| 12           | Static Hash Indexes                        | :heavy_check_mark: |
| 12           | Btree Indexes                              | :heavy_check_mark: |
| 13           | Materialization and Sorting                | :heavy_check_mark: |
| 14           | MultiBuffer Sorting/Product                | :heavy_check_mark: |
| 15           | Query Optimization                         | :heavy_check_mark: |

Currently the only major feature mentioned but has not been implemented is the Remote JDBC Interface. (This is due to the difference between Java and C++. Java has RMI (Remote Method Invocation) which allows to implement remote server easily, on the other hand in C++, since there are no standard libraries for remote procedure calls, it becomes more complex to implement it.)

# Getting Started

## Build
Clone this repository.

```sh
mkdir build
cd build
cmake ..
make
```

Then the executables will be generated in the `build/bin` folder.

## Usage

```sh
cd build/bin
```

To create a sample database with student records, run

```sh
./createstudentdb
```

This will create some tables which will be stored in `root:password@localhost` folder. To see the records, run

```sh
./server
Connect> root:password@localhost
SQL> select sid, sname, majorid, gradyear from student
SQL> select did, dname from dept
SQL> select sid, sname, gradyear, dname from student, dept where did = majorid
```

To run tests, run

```sh
./simpledb_test
```









