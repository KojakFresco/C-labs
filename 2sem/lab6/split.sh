#!/bin/bash
set -euo pipefail

usage() {
    echo "Usage: $0 [OPTIONS] [FILE]"
    echo "-p, --prefix=PREFIX: append an additional PREFIX to file names (default 'x')"
    echo "-a, --suffix-length=N: generate suffixes of length N (default 2)"
    echo "-b, --bytes=SIZE: put SIZE bytes per output file"
    echo "-l, --lines=N: put NUMBER lines per output file"
    echo "-h, --help: display this help"
}

to_suffix() {	
    local num="$1"
    local length="$suffix_length"
    local chars=({a..z})
    local result=""
    for (( i=0; i<length; i++ )); do
        local rem=$((num % 26))
        result="${chars[rem]}$result"
        num=$((num / 26))
    done

    echo "$result" 
}

prefix="x"
suffix_length=2
bytes=""
lines=""
input_file=""

while [[ $# -gt 0 ]]; do
    case "$1" in
        -p|--prefix) prefix="$2"; shift 2;;
        --prefix=*) prefix="${1#*=}"; shift;;
        -a|--suffix-length) suffix_length="$2"; shift 2;;
        --suffix-length=*) suffix_length="${1#*=}"; shift;;
        -b|--bytes) bytes="$2"; shift 2;;
        --bytes=*) bytes="${1#*=}"; shift;;
        -l|--lines) lines="$2"; shift 2;;
        --lines=*) lines="${1#*=}"; shift;;
        -h|--help) usage; exit 0;;
        -*) echo "Error: unknown option $1"; usage; exit 1;;
        *) 
	    if [[ -z "$input_file" ]]; then
                input_file="$1"
            else
                echo "Error: excess argument $1"
                exit 1
            fi
            shift
            ;;
    esac
done

if [[ -z "$bytes" && -z "$lines" ]]; then
    echo "Error: specify -b or -l"
    usage
    exit 1
fi

if [[ -n "$bytes" && -n "$lines" ]]; then
    echo "Error: can't split more than one way"
    usage
    exit 1
fi

if [[ -n "$input_file" &&  ! -s "$input_file"  ]]; then
    echo "Error: file '$input_file' is empty or doesn't exist"
    exit 1
fi

if [[ -n "$lines" && ! $lines =~ ^[1-9][0-9]*$ ]]; then 
    echo "Error: lines count should be a positive number"
    exit 1
fi

if [[ -n "$bytes" && ! $bytes =~ ^[1-9][0-9]*[MKB]?$ ]]; then
    echo "Error: bytes count should be positive number with correct suffix (B/K/M)"
    exit 1
fi

if [[ -z "$input_file" ]]; then
    temp_file=$(mktemp)
    trap 'rm -f "$temp_file"' EXIT
    cat > "$temp_file"
    input_file="$temp_file"
fi

if [[ -n "$lines" ]]; then
    line_count=$(wc -l < "$input_file")

    needed_files=$(( (line_count + lines - 1) / lines ))
    if (( 26**suffix_length < needed_files )); then
        echo "Error: output file suffixes exhausted"
        exit 1
    fi
    mapfile -t lines_arr < "$input_file"
    for (( file=0; file<needed_files; file++ )); do
        suffix=$(to_suffix $file)
        out_file="$prefix$suffix"
        > "$out_file"
        for (( l=0; l<lines; l++ )); do
            if (( file*lines+l >= line_count )); then
                break
            fi
            printf '%s\n' "${lines_arr[file*lines+l]}" >> "$out_file"
        done
    done
else
    if [[ "${bytes: -1}" == "M" ]]; then
        bytes_count=$(( "${bytes:0:-1}"*1024*1024 ))
    elif [[ "${bytes: -1}" == "K" ]]; then
        bytes_count=$(( "${bytes:0:-1}"*1024 ))
    elif [[ "${bytes: -1}" == "B" ]]; then
        bytes_count="${bytes:0:-1}"
    else
        bytes_count="$bytes"
    fi

    file_size=$(stat -c%s "$input_file")

    needed_files=$(( (file_size + bytes_count - 1) / bytes_count ))
    if (( 26**suffix_length < needed_files )); then
        echo "Error: output file suffixes exhausted"
        exit 1
    fi

    for (( file=0; file<needed_files; file++ )); do
        suffix=$(to_suffix $file)
        out_file="$prefix$suffix"
        > "$out_file"
        dd if="$input_file" of="$out_file" bs="$bytes_count" skip="$file" count="1" status=none
    done
fi
