#!/bin/bash

usage()
{
  echo "Options: 
  -b, --begin=<Begin ID>        Begin ID of the task, default value is 1;
  -e, --end=<End ID>            End ID of task, default value is 10;
  -h, --host=<Hostname>         Connect to MySQL host, default value is localhost;
  -P, --port=<Port>             Port number to use for MySQL connection, default value is 3306;
  -u, --user=<Username>         User for login MySQL, default value is root;
  -p, --password=<Password>     Password for login MySQL;

Environment Variables:
  CSMITH_INCLUDE_DIR        path to CSmith include directory;
                                the default value is /usr/local/include/csmith-2.3.0
  CSMITH_BIN_DIR            path to CSmith binary directory;
  BUILD_DIR                 path to EMI binary directory;
"
  exit 2
}

# Environment variables initialization
if [ -z $CSMITH_INCLUDE_DIR ];then
    CSMITH_INCLUDE_DIR=/usr/local/include/csmith-2.3.0
fi
if [ -n $CSMITH_BIN_DIR ];then
    PATH=$CSMITH_BIN_DIR:$PATH
fi
if [ -n $BUILD_DIR ];then
    PATH=$BUILD_DIR/covemi:$BUILD_DIR/emidiff:$PATH
fi

if [ ! -d "experiment" ]; then
    mkdir experiment
fi

cd experiment

# Directory and file initialization
if [ ! -f "timeout.txt" ]; then
    echo 0 > timeout.txt
fi
if [ ! -d "pre" ]; then
    mkdir pre
fi
if [ ! -d "post" ]; then
    mkdir post
fi

# Parse options
BEGIN=1
END=10
MYSQL_HOST=localhost
MYSQL_PORT=3306
MYSQL_USER=root
PARSED_ARGUMENTS=`getopt -o b:e:h:u:p: --long begin:,end:,host:,user:,password: -- $@` || usage
eval set -- "$PARSED_ARGUMENTS"
while :
do
  case "$1" in
    -b | --begin)       BEGIN="$2"; shift 2 ;;
    -e | --end)         END="$2"; shift 2 ;;
    -h | --host)        MYSQL_HOST="$2"; shift 2 ;;
    -P | --port)        MYSQL_PORT="$2"; shift 2 ;;
    -u | --user)        MYSQL_USER="$2"; shift 2 ;;
    -p | --password)    MYSQL_PWD="$2"; shift 2 ;;
    # -- means the end of the arguments; drop this, and break out of the while loop
    --) shift; break ;;
    # If invalid options were passed, then getopt should have reported an error,
    # which we checked as VALID_ARGUMENTS when getopt was called...
    *) echo "Unexpected option: $1"
       usage ;;
  esac
done

if [ $BEGIN -gt $END ]  ; then
    exit 1;
fi

emi_diff()
{
    if [ $1 -eq 1 ]; then
        dir="post"
    else
        dir="pre"
    fi

    # Generate EMI program and compile it
    # Any error in any of these processes will cause the task to fail, and the
    # task will be placed in the cplerr queue
    covemi $2/main.c -m $1 -o emi -- -I $CSMITH_INCLUDE_DIR -w &&\
    # Usually the EMI program generated by llvm-cov is compilable, so first compile the EMI program of llvm-cov
    gcc -I$CSMITH_INCLUDE_DIR $2/emi/main.llvm-cov.c -o $2/emi/main_llvm-cov -w &&\
    gcc -I$CSMITH_INCLUDE_DIR $2/emi/main.gcov.c -o $2/emi/main_gcov -w
    if [ $? -ne 0 ]; then
        MYSQL_PWD=${MYSQL_PWD} mysql -h${MYSQL_HOST} -P${MYSQL_PORT} -u${MYSQL_USER} -e "INSERT INTO covemidiff.emi(task_id, method, status) VALUES ($2, $1, 0)"
        emidiff $2/emi/main.gcov.c $2/emi/main.llvm-cov.c --id $2 -m $1 -h ${MYSQL_HOST} --port ${MYSQL_PORT} -u ${MYSQL_USER} --pwd ${MYSQL_PWD} -- -I $CSMITH_INCLUDE_DIR -w -ferror-limit=0
        add_cplerr $1 $2
        cp -r $2 $dir
        rm -r $2/emi
        return
    fi

    # Execute EMI program and compare execution results
    # If the execution results are different, the task will be added to the diff queue
    $2/emi/main_gcov > $2/emi/gcov.txt &&\
    $2/emi/main_llvm-cov > $2/emi/llvm-cov.txt &&\
    diff $2/emi/gcov.txt $2/emi/llvm-cov.txt
    if [ $? -ne 0 ]; then
        MYSQL_PWD=${MYSQL_PWD} mysql -h${MYSQL_HOST} -P${MYSQL_PORT} -u${MYSQL_USER} -e "INSERT INTO covemidiff.emi(task_id, method, status) VALUES ($2, $1, 1)"
        emidiff $2/emi/main.gcov.c $2/emi/main.llvm-cov.c --id $2 -m $1 -h ${MYSQL_HOST} --port ${MYSQL_PORT} -u ${MYSQL_USER} --pwd ${MYSQL_PWD} -- -I $CSMITH_INCLUDE_DIR -w -ferror-limit=0
        add_diff $1 $2
        cp -r $2 $dir
    fi

    # No exception to the task
    rm -r $2/emi
}

add_cplerr()
{
    if [ $1 -eq 1 ]; then
        post_cplerr+=($2)
    else
        pre_cplerr+=($2)
    fi
}

add_diff()
{
    if [ $1 -eq 1 ]; then
        post_diff+=($2)
    else
        pre_diff+=($2)
    fi
}

timeout=()
timeout_cnt=$(cat timeout.txt)
echo "Previous timeout task count: $timeout_cnt"
pre_cplerr=()
pre_diff=()
post_cplerr=()
post_diff=()

for i in $(seq $BEGIN $END)
do
    echo "============================= Task $i ============================="
    mkdir $i
    # Generates test cases with CSmith
    csmith | clang-format > $i/main.c

    # Generate the gcov and llvm-cov coverage reports of test cases respectively
    # In order to avoid that the program cannot return normally due to the infinite loop 
    # in the test case, a timeout period of 3 seconds is set for each test program execution,
    # and the timeout task will be put into the timeout queue.
    cd $i
    clang -fprofile-instr-generate -fcoverage-mapping -I$CSMITH_INCLUDE_DIR -w main.c -o main_clang &&\
    LLVM_PROFILE_FILE="main.profraw" timeout 3 ./main_clang
    if [ $? -ne 0 ]; then
        echo "Task $i timeout"
        timeout+=($i)
        cd ..
        rm -r $i
        let timeout_cnt++
        echo $timeout_cnt > timeout.txt
        continue
    fi
    llvm-profdata merge -sparse main.profraw -o main.profdata
    llvm-cov show ./main_clang -instr-profile=main.profdata ./ | c++filt > main.c.llvm-cov
    gcc --coverage -I$CSMITH_INCLUDE_DIR main.c -o main_gcc -w &&\
    timeout 3 ./main_gcc
    gcov main.c -m > /dev/null
    cd ..

    emi_diff 0 $i
    emi_diff 1 $i

    rm -r $i
done

echo "============================= Exception Report ============================="
if ((${#timeout[@]})); then
    echo "Timeout Task IDs:"
    echo "${timeout[@]}"
fi

if ((${#pre_cplerr[@]})); then
    echo "PreOrder Compile Error Task IDs:"
    echo "${pre_cplerr[@]}"
fi

if ((${#pre_diff[@]})); then
    echo "PreOrder Different Rsult Task IDs:"
    echo "${pre_diff[@]}"
fi

if ((${#post_cplerr[@]})); then
    echo "PostOrder Compile Error Task IDs:"
    echo "${post_cplerr[@]}"
fi

if ((${#post_diff[@]})); then
    echo "PostOrder Different Rsult Task IDs:"
    echo "${post_diff[@]}"
fi