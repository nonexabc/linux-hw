#asked chatgpt to write some tricky tests cuz I'm lazy to create all of that manually

#!/bin/bash
set -e
echo "Running sparse file copy tests..."
for i in {1..5}; do rm -f sparsefile$i sparsefile${i}_copy; done

printf 'A%.0s' {1..4096} > sparsefile1
truncate -s 4M sparsefile1

printf 'A%.0s' {1..4096} > sparsefile2
truncate -s 1M sparsefile2
printf 'B%.0s' {1..4096} | dd of=sparsefile2 bs=1 seek=2M conv=notrunc
truncate -s 4M sparsefile2

truncate -s 1M sparsefile3
printf 'C%.0s' {1..4096} | dd of=sparsefile3 bs=1 seek=1M conv=notrunc
truncate -s 2M sparsefile3

dd if=/dev/urandom of=sparsefile4 bs=1M count=2

: > sparsefile5

for i in {1..5}; do
    echo "Test $i:"
    ./copy sparsefile$i sparsefile${i}_copy
    cmp sparsefile$i sparsefile${i}_copy && echo "✅ identical"
    echo
done
