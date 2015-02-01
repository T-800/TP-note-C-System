gcc -o rle main.c
./rle -c ./fichiers/decomp_in ./fichiers/comp_out
#diff -s ./fichiers/comp_out ./fichiers/comp_in
./rle -d ./fichiers/comp_out ./fichiers/decomp_out
diff -s ./fichiers/decomp_out ./fichiers/decomp_in
./rle -c ./fichiers/test ./fichiers/test.out
diff -s ./fichiers/test.out ./fichiers/test.comp
./rle -d ./fichiers/test.out ./fichiers/decomp_test
diff -s ./fichiers/decomp_test ./fichiers/test


./rle -d ./fichiers/comp_in ./fichiers/decomp_out
diff -s ./fichiers/comp_in ./fichiers/decomp_out