#!/bin/bash

usage()
{
  echo "Options: 
  -b, --begin=ID            begin ID of the task, default value is 1;
  -n, --number=NUMBER       number of tasks, default value is 10;

Environment Variables:
  CSMITH_INCLUDE_DIR        path to CSmith include directory;
  CSMITH_BIN_DIR            path to CSmith binary directory;
  EMI_BIN_DIR               path to EMI binary directory;
                              the default value is /usr/local/include/csmith-2.3.0"
  exit 2
}

# Environment variables initialization
if [ -z $CSMITH_INCLUDE_DIR ];then
    CSMITH_INCLUDE_DIR=/usr/local/include/csmith-2.3.0
fi
if [ -n $CSMITH_BIN_DIR ];then
    PATH=$CSMITH_BIN_DIR:$PATH
fi
if [ -n $EMI_BIN_DIR ];then
    PATH=$EMI_BIN_DIR:$PATH
fi

# Directory initialization
if [ ! -d "diff" ]; then
    mkdir diff
fi
if [ ! -d "error" ]; then
    mkdir error
fi

# Parse options
BEGIN=1
NUMBER=10
PARSED_ARGUMENTS=`getopt -o b:n: --long begin:,number: -- $@` || usage
eval set -- "$PARSED_ARGUMENTS"
while :
do
  case "$1" in
    -b | --begin)  BEGIN="$2" ; shift 2 ;;
    -n | --number) NUMBER="$2"; shift 2 ;;
    # -- means the end of the arguments; drop this, and break out of the while loop
    --) shift; break ;;
    # If invalid options were passed, then getopt should have reported an error,
    # which we checked as VALID_ARGUMENTS when getopt was called...
    *) echo "Unexpected option: $1"
       usage ;;
  esac
done

timeout=()
cplerr=()
diff=()
for i in $(seq $BEGIN `expr $BEGIN + $NUMBER - 1`)
do
    echo "============================= Task $i ============================="
    mkdir $i
    cd $i
    # Generates test cases with CSmith
    csmith > main.c

    # Generate the gcov and llvm-cov coverage reports of test cases respectively
    # In order to avoid that the program cannot return normally due to the infinite loop 
    # in the test case, a timeout period of 3 seconds is set for each test program execution,
    # and the timeout task will be put into the timeout queue.
    clang -fprofile-instr-generate -fcoverage-mapping -I$CSMITH_INCLUDE_DIR -w main.c -o main_clang &&\
    LLVM_PROFILE_FILE="main.profraw" timeout 3 ./main_clang ||\
    eval "echo 'Task $i timeout' && timeout+=($i) && cd .. && rm -r $i && continue"
    llvm-profdata merge -sparse main.profraw -o main.profdata
    llvm-cov show ./main_clang -instr-profile=main.profdata ./ | c++filt > main.c.llvm-cov
    gcc --coverage -I$CSMITH_INCLUDE_DIR main.c -o main_gcc -w &&\
    timeout 3 ./main_gcc ||\
    eval "echo 'Task $i timeout' && timeout+=($i) && cd .. && rm -r $i && continue"
    gcov main.c -m > /dev/null

    # Generate EMI program and compile it
    # Any error in any of these processes will cause the task to fail, and the
    # task will be placed in the cplerr queue
    emi ./main.c -- -I $CSMITH_INCLUDE_DIR -w &&\
    gcc -I$CSMITH_INCLUDE_DIR main.gcov.emi.c -o main_gcov_emi -w &&\
    gcc -I$CSMITH_INCLUDE_DIR main.llvm-cov.emi.c -o main_llvm-cov_emi -w ||\
    eval "echo 'Task $i compile error' && cplerr+=($i) && cd .. && mv $i error && continue"

    # Execute EMI program and compare execution results
    # If the execution results are different, the task will be added to the diff queue
    ./main_gcov_emi > gcov.txt &&\
    ./main_llvm-cov_emi > llvm-cov.txt &&\
    diff gcov.txt llvm-cov.txt ||\
    eval "echo 'Task $i diff' && diff+=($i) && cd .. && mv $i diff && continue"

    # No exception to the task
    cd .. &&\
    rm -r $i
done

echo "============================= Exception Report ============================="
if ((${#timeout[@]})); then
    echo "Timeout Task IDs:"
    echo "${timeout[@]}"
fi

if ((${#cplerr[@]})); then
    echo "Compile Error Task IDs:"
    echo "${cplerr[@]}"
fi

if ((${#diff[@]})); then
    echo "Different Rsult Task IDs:"
    echo "${diff[@]}"
fi