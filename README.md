# EMI

EMI tool based on LibTooling on Ubuntu 20.04

## Project Dependencies

Dependencies install:

```shell
sudo apt install llvm clang cmake ninja-build libclang-10-dev build-essential mysql-server libmysqlcppconn-dev clang-format
```

Csmith:

``` shell
git clone https://github.com/csmith-project/csmith.git
cd csmith
sudo apt install g++ cmake m4
cmake -DCMAKE_INSTALL_PREFIX=<INSTALL-PREFIX> .
make && make install
```

MySQL script:

```sql
CREATE DATABASE `covemidiff`;

CREATE TABLE `covemidiff`.`emi` (
  `id` int NOT NULL AUTO_INCREMENT,
  `task_id` int NOT NULL DEFAULT '0',
  `method` int NOT NULL DEFAULT '0',
  `status` int NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `uniq_key` (`task_id`,`method`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

CREATE TABLE `covemidiff`.`diff` (
  `id` int NOT NULL AUTO_INCREMENT,
  `task_id` int NOT NULL DEFAULT '0',
  `method` int NOT NULL DEFAULT '0',
  `file_type_id` int NOT NULL DEFAULT '0',
  `line_num` int NOT NULL DEFAULT '0',
  `reason` varchar(45) NOT NULL DEFAULT '',
  `coverage_tool_id` int NOT NULL DEFAULT '0',
  `count` int NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `unqi_key` (`task_id`,`method`,`file_type_id`,`line_num`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
```

## Project build

```shell
$ cmake -DCMAKE_BUILD_TYPE=[Debug|Release] -S . -B build -G Ninja
$ cmake --build build
```

- `-DCMAKE_BUILD_TYPE=Debug`: Enable debuggind, enable lowest optimization
- `-DCMAKE_BUILD_TYPE=Release`: Disable debugging, enable highest optimisation

## Command Usages

The project contains two commands are `covemi` and `emidiff`
### covemi

According to the coverage reports of gcov and llvm-cov in the directory where the input C file is located, `covemi` deletes the unexecuted statements in the input C file respectively, and obtains the EMI files based on the coverage reports of gcov and llvm-cov respectively.

Use the `-m` option to decide whether to perform preorder deletion or postorder deletion according to the AST tree, `0` is preorder, `1` is postorder.

If the parent statement is marked as unexecuted, but there are executed child statements within the parent statement. In this case, if it is a preorder deletion, both the parent statement and its child statements will be deleted; if it is a postorder deletion, the parent statement will not be deleted, only the unexecuted statements in the parent statement will be deleted.

Use the `-o` option to decide the output directory for EMI files.

```
$ build/covemi/covemi --help
USAGE: covemi [options] <source0> [... <sourceN>]

OPTIONS:

EMI Options:

  --extra-arg=<string>        - Additional argument to append to the compiler command line
  --extra-arg-before=<string> - Additional argument to prepend to the compiler command line
  -m=<int>                    - EMI prune method
  -o=<string>                 - Explicitly specify a output directory
  -p=<string>                 - Build path

Generic Options:

  --help                      - Display available options (--help-hidden for more)
  --help-list                 - Display list of available options (--help-list-hidden for more)
  --version                   - Display the version of this program
```

### emidiff

`emidiff` compares and analyzes two EMI files based on gcov and llvm-cov coverage reports, finds inconsistent statements, analyzes the causes of inconsistencies based on the context of the C file, and stores the analyzed results in the database

```shell
$ build/emidiff/emidiff --help
USAGE: emidiff [options] <source0> [... <sourceN>]

OPTIONS:

Diff Options:

  --extra-arg=<string>        - Additional argument to append to the compiler command line
  --extra-arg-before=<string> - Additional argument to prepend to the compiler command line
  -h=<string>                 - MySQL host option
  --id=<int>                  - Task Id option
  -m=<int>                    - EMI prune method option
  -p=<string>                 - Build path
  --port=<int>                - MySQL port option
  --pwd=<string>              - MySQL password option
  -u=<string>                 - MySQL user option

Generic Options:

  --help                      - Display available options (--help-hidden for more)
  --help-list                 - Display list of available options (--help-list-hidden for more)
  --version                   - Display the version of this program
```

## Execute the experiment

```shell
$ CSMITH_INCLUDE_DIR=/path/to/csmith/include CSMITH_BIN_DIR=/path/to/csmith/bin BUILD_DIR=/path/to/CovEMIDiff/build ./run.sh -u root -p root
```