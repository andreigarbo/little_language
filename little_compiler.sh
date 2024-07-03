
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 input_file output_executable"
    exit 1
fi

input_file="$1"
output_executable="$2"

./little_compiler "$input_file"

llc -filetype=obj interm.ll -o output.o

clang output.o -o "$output_executable"

rm output.o
rm interm.ll