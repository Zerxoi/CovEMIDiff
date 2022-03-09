#!/bin/bash

# Loading progressbar
source progressbar.sh || exit 1

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

if [ ! -d "tmp" ]; then
    mkdir tmp
fi
cd tmp

function usage()
{
  echo "Options: 
  -n, --number=<Number>         Number of tasks, default value is 10;
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

function run_code()
{
    clang main.c -o main -I$CSMITH_INCLUDE_DIR -w
    output=`timeout 3 ./main`
    rm main
    echo $output
}

# task_id, coverage_tool, version
function insert_report()
{
    $(MYSQL_PWD=${MYSQL_PWD} mysql -h${MYSQL_HOST} -P${MYSQL_PORT} -u${MYSQL_USER} -e "INSERT INTO covemidiff.report (task_id, coverage_tool, version, content) VALUES ('$1', '$2', '$3', load_file('`pwd`/main.c.$3.$2'));")
}

# version, task_id
function gcov_report()
{
    gcc-$1 --coverage -I$CSMITH_INCLUDE_DIR main.c -c -w &&\
    gcc-$1 --coverage main.o -o main_gcc -w &&\
    ./main_gcc > /dev/null &&\
    gcov-$1 main.c -m -r -t -w > main.c.$1.gcov &&\
    insert_report $2 gcov $1
    rm *.o *.gcda *.gcno main_gcc
}

# version, task_id
function llvm_cov_report()
{
    clang-$1 -fprofile-instr-generate -fcoverage-mapping -I$CSMITH_INCLUDE_DIR -w main.c -o main_clang &&\
    LLVM_PROFILE_FILE="main.profraw" ./main_clang > /dev/null &&\
    llvm-profdata-$1 merge -sparse main.profraw -o main.profdata &&\
    llvm-cov-$1 show ./main_clang -instr-profile=main.profdata ./ | c++filt > main.c.$1.llvm-cov &&\
    insert_report $2 llvm-cov $1
    rm *.profdata *.profraw main_clang
}

# coverage_tool, version, method, task_id
function generate_emi()
{
    covemi -t $1 -v $2 -m $3  main.c -- -I$CSMITH_INCLUDE_DIR -w
    if [ $3 -eq 1 ]; then
        method="post"
    else
        method="pre"
    fi
    clang -I$CSMITH_INCLUDE_DIR main.$1.$method.$2.c -o main.out -w
    if [ $? -ne 0 ]; then
        emi_output=''
    else
        emi_output=`./main.out`
        rm main.out
    fi
    $(MYSQL_PWD=${MYSQL_PWD} mysql -h${MYSQL_HOST} -P${MYSQL_PORT} -u${MYSQL_USER} -e "INSERT INTO covemidiff.emi_file (task_id, coverage_tool, version, method, content, result) VALUES ('$4', '$1', '$2', '$3', load_file('`pwd`/main.$1.$method.$2.c'), '$emi_output');")
}

# coverage_tool_1, version_1, method_1, coverage_tool_2, version_2, method_2, task_id
function emi_diff()
{
    results=`MYSQL_PWD=${MYSQL_PWD} mysql -h${MYSQL_HOST} -P${MYSQL_PORT} -u${MYSQL_USER} -s -e "SELECT result FROM covemidiff.emi_file WHERE task_id = '$7' AND ((coverage_tool = '$1' AND version = '$2' AND method = '$3') OR (coverage_tool = '$4' AND version = '$5' AND method = '$6'));"`
    result1=`echo "$results" | sed -n 1p`
    result2=`echo "$results" | sed -n 2p`
    if [ -z "$result1" ] || [ -z "$result2" ]; then
        diff_result=1
    else
        if [ "$result1" == "$result2" ]; then
            diff_result=0
        else
            diff_result=2
        fi
    fi
    diff_id=`MYSQL_PWD=${MYSQL_PWD} mysql -h${MYSQL_HOST} -P${MYSQL_PORT} -u${MYSQL_USER} -e "INSERT INTO covemidiff.emi_diff (task_id, coverage_tool_1, version_1, method_1, coverage_tool_2, version_2, method_2, result) VALUES ('$7', '$1', '$2', '$3', '$4', '$5', '$6', '$diff_result'); SELECT LAST_INSERT_ID() id;"`
    diff_id=`echo $diff_id | awk '{print $2}'`

    if [ $diff_result -ne 0 ]; then
        if [ $3 -eq 1 ]; then
            method1="post"
        else
            method1="pre"
        fi
        if [ $6 -eq 1 ]; then
            method2="post"
        else
            method2="pre"
        fi
        emidiff main.$1.$method1.$2.c main.$4.$method2.$5.c --id $diff_id -h ${MYSQL_HOST} --port ${MYSQL_PORT} -u ${MYSQL_USER} --pwd ${MYSQL_PWD} -- -I $CSMITH_INCLUDE_DIR -w -ferror-limit=0
    fi
}

NUMBER=10
MYSQL_HOST=localhost
MYSQL_PORT=3306
MYSQL_USER=root
PARSED_ARGUMENTS=`getopt -o n:h:u:p: --long number:,host:,user:,password: -- $@` || usage
eval set -- "$PARSED_ARGUMENTS"
while :
do
  case "$1" in
    -n | --number)      NUMBER="$2"; shift 2 ;;
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

progressbar "CovEMIDiff" 0 $NUMBER "[0/$NUMBER]"
for ii in $(seq 1 $NUMBER)
do
    csmith | clang-format-15 > main.c

    result=`run_code`

    id=$(MYSQL_PWD=${MYSQL_PWD} mysql -h${MYSQL_HOST} -P${MYSQL_PORT} -u${MYSQL_USER} -e "INSERT INTO covemidiff.task(content, result) VALUES (load_file('`pwd`/main.c'), '$result'); SELECT LAST_INSERT_ID() id;")
    id=`echo $id | awk '{print $2}'`

    if [ -z "$result" ]; then
        continue
    fi

    gcov_report 9 $id
    llvm_cov_report 10 $id
    gcov_report 11 $id
    llvm_cov_report 15 $id

    generate_emi gcov 9 0 $id
    generate_emi llvm-cov 10 0 $id
    generate_emi gcov 9 1 $id
    generate_emi llvm-cov 10 1 $id
    generate_emi gcov 11 0 $id
    generate_emi llvm-cov 15 0 $id
    generate_emi gcov 11 1 $id
    generate_emi llvm-cov 15 1 $id

    # Coverage tool RDT
    emi_diff gcov 9 0 llvm-cov 10 0 $id
    emi_diff gcov 9 1 llvm-cov 10 1 $id
    # EMI method RDT
    emi_diff gcov 9 0 gcov 9 1 $id
    emi_diff llvm-cov 10 0 llvm-cov 10 1 $id
    # Coverage tool version RDT
    emi_diff gcov 9 0 gcov 11 0 $id
    emi_diff gcov 9 1 gcov 11 1 $id
    emi_diff llvm-cov 10 0 llvm-cov 15 0 $id
    emi_diff llvm-cov 10 1 llvm-cov 15 1 $id
    rm *.c *.llvm-cov *.gcov

    progressbar "CovEMIDiff" $ii $NUMBER "[$ii/$NUMBER]"
done