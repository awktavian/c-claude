#!/bin/sh
# c - Ultimate Claude batch processor (optimal agentic config)
# Usage: c GLOB INSTRUCTION [-p PARALLEL] [-b BATCH] [-o OUTDIR] [-v] [-a] [-i FILE] [--flags...]
#        cat files | c - INSTRUCTION    # stdin mode (aggregate)
# Examples:
#   c '**/*.py' 'add type hints' -p 12
#   c '**/*.py' 'analyze' -o results/ -p 8         # save to results/file.txt
#   c 'results/*.txt' 'summarize' --cat            # aggregate all files
#   cat results/*.txt | c - 'synthesize report'   # stdin aggregate
# Flags:
#   -p N    Parallelism (default: auto-detect, max 16 for API rate limits)
#   -b N    Batch size (enables batched mode with context clearing)
#   -o D    Output directory (save each result to D/file.txt, no stdout)
#   -v      Verbose/debug (shows file count, parallelism, execution trace)
#   -a      Agentic mode (--permission-mode acceptEdits, auto-accept edits)
#   -i F    Instruction file (read instruction from file, avoids cmd limits)
#   --cat   Aggregate mode (concat all files, pass to single Claude instance)
#   --*     All claude CLI flags pass through
case $1 in help|-h|--help)sed -n 2,19p "$0";exit;;esac
[ $# -lt 1 ]&&echo "Usage: c GLOB INSTRUCTION [-flags...] OR c - INSTRUCTION (stdin)">&2&&exit 1
# Check for stdin mode
if [ "$1" = "-" ];then
  STDIN_MODE=1;shift
  [ $# -lt 1 ]&&echo "Error: INSTRUCTION required for stdin mode">&2&&exit 1
else
  STDIN_MODE=0
  # Collect file args: accumulate until we hit instruction (non-file) or -i flag
  FILES="";while [ $# -gt 0 ]&&[ "$1" != "-i" ];do
    if echo "$1"|grep -qE '^\-[pvboa]';then break  # Flags come after instruction
    elif [ -f "$1" ]||echo "$1"|grep -qE '\*|\?|\[';then FILES="$FILES$1 ";shift
    else break;fi  # Non-file = instruction
  done
  [ -z "$FILES" ]&&echo "Error: No files/pattern specified">&2&&exit 1
  G=$(echo "$FILES"|xargs)  # Trim whitespace
fi
if [ "$1" = "-i" ];then
  [ $# -lt 2 ]&&echo "Error: -i requires FILE argument">&2&&exit 1
  IFILE=$2;shift 2
  [ ! -f "$IFILE" ]&&echo "Error: File not found: $IFILE">&2&&exit 1
else
  [ $# -lt 1 ]&&echo "Error: INSTRUCTION required (or use -i FILE)">&2&&exit 1
  I=$1;shift
fi
P=$(nproc 2>/dev/null||sysctl -n hw.logicalcpu 2>/dev/null||echo 8)
[ $P -gt 16 ]&&P=16
B=0 O="" V=0 A="" CAT=0
while [ $# -gt 0 ];do
  case $1 in
    -p)P=$2;shift 2;;
    -b)B=$2;shift 2;;
    -o)O=$2;shift 2;;
    -v)V=1;shift;;
    -a)A="--permission-mode acceptEdits";shift;;
    --cat)CAT=1;shift;;
    *)break;;
  esac
done
[ $V = 1 ]&&set -x
[ -n "$O" ]&&mkdir -p "$O"
T=$(mktemp /tmp/c.XXXXXX)
trap "rm -f $T" EXIT
[ -n "$IFILE" ]&&cp "$IFILE" $T||echo "$I">$T

# STDIN MODE: pipe directly to Claude
if [ $STDIN_MODE = 1 ];then
  [ $V = 1 ]&&echo "Mode: stdin (aggregate)">&2
  cat | claude $A "$@" -p "$(cat $T)"
  exit $?
fi

# Handle glob patterns, single files, or multiple files
F=""
if echo "$G"|grep -q ' ';then
  # Multiple files (space-separated)
  for x in $G;do [ -f "$x" ]&&F="$F$x "||echo "Warning: $x not found">&2;done
  F=$(echo "$F"|xargs)  # Trim
elif [ -f "$G" ];then
  F=$G
elif echo "$G"|grep -qE '\*|\?|\[';then
  # Glob pattern - use find
  F=$(find . -path "./$G" -type f 2>/dev/null)
  [ -z "$F" ]&&F=$(find . -name "$G" -type f 2>/dev/null)  # Fallback to -name
fi
[ -z "$F" ]&&echo "Error: No files match: $G">&2&&exit 1
F=$(echo "$F"|tr ' ' '\n')  # Convert to newline-separated for xargs
[ $V = 1 ]&&echo "Files: $(echo "$F"|wc -l|tr -d ' '), Parallelism: $P, Mode: $([ $CAT = 1 ]&&echo 'aggregate'||echo 'parallel')">&2

# CAT MODE: concatenate all files and pass to single Claude
if [ $CAT = 1 ];then
  echo "$F"|xargs cat | claude $A "$@" -p "$(cat $T)"
  exit $?
fi

# PARALLEL MODE: process each file separately
if [ -n "$O" ];then
  echo "$F"|xargs -P$P -I@ sh -c 'claude '"$A"' "$@" -p "@ $(cat '"$T"')">'"$O"'/$(basename @).txt' _ "$@"
elif [ $B -gt 0 ];then
  echo "$F"|xargs -n$B|head -n$P|while read f;do(for x in $f;do claude $A "$@" -p "$x $(cat $T)"&done;wait)&done;wait
elif type parallel >/dev/null 2>&1;then
  echo "$F"|parallel -j$P claude $A "$@" -p "{} $(cat $T)"
else
  echo "$F"|xargs -P$P -I@ claude $A "$@" -p "@ $(cat $T)"
fi
