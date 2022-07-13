# The task is to develop two functions do compress/decompress files in UTF-8 using Huffman coding. 

There are three difficulty levels in this problem:
- a mandatory part that requires a working Huffman decompressor (i.e. compression is not required), moreover, the decompressed files consist purely of ASCII characters (0-127),
- an optional part that supports the decompression (i.e., compression is still not required), however UTF-8 characters must be supported in the compressed/decompressed files,
- the bonus part with the support for the compression.

Huffman code is a compression technique based on statistic and information theory. It is based on an observation that the frequency of characters is not evenly distributed. For instance, spaces are very frequent in text files. On the other hand, some characters such as Czech accent 'ř' are rare even in Czech texts. Huffman coding analyzes the data to compress and establishes a frequency for each input character. Based on the frequency, the input characters are re-coded. Typically, the length of the code varies between 1 bit and 10-20 bits. The frequent characters are assigned short codes whereas rare are assigned long codes. Thus, the overall length of the input is decreased.

The basic idea is simple, however, the variable code length introduces some technical problems. First, the Huffman code decoder needs to know how many bits to read in order to decode exactly one character. If the input codes are fixed (such as ASCII where 1 character = 8 bits = 1 byte), the problem is trivial. If the input data is in UTF-8 code (i.e. 1 character is 1 to 4 bytes), the input decoder must examine the bytes and assemble the bytes accordingly. This becomes even more difficult with Huffman code, where the code lengths are not byte-aligned and vary. Huffman code is developed as a prefix-free code. It means that a bit sequence which forms some character is not a prefix of any other bit sequence in the code. For instance, if space character is coded as a sequence of two zero bits 00, then bit sequences 001, 000, 0001, 0010, ... cannot be used as codes for other characters. This property guarantees unambiguous decoding.

The second problem is padding at the end of the file (and the related problem - detecting the last character to decode). Since the codes may be of arbitrary length in bits, the total number of bits in the file may be arbitrary. However, our files must be byte- aligned. Thus, if the total number of bits is not a multiple of 8, we shall add some padding bits at the end of the file. It is not a problem. However, the extra added bits may be interpreted as extra characters by the decoder. Suppose we had to pad the input with 5 zero bits, moreover, we code space as two zero bits. The decoded would decode the extra 5 zeros as two extra spaces. To avoid this, we code the data in chunks and we include chunk lengths in the compressed files. In our implementation we add an extra bit at the beginning of each chunk:
- a single bit set to 1 starts a chunk of length 4096 characters (coded using Huffman code),
- a single bit set to 0 is then followed by 12 bits representing the number of characters coded by the chunk (i.e. the length of the chunk is between 0 and 4095 characters). Following the chunk length, there are the compressed characters. The chunk starting with zero bit must be the last chunk in the file to clearly indicate where to stop the decompression. The last chunk may be 0 characters long if the number of characters in the file was a multiple of 4096.

For example, an input file with 15800 characters will form the following chunks:
```
1 <bits coding characters 0 to 4095>
1 <bits coding characters 4096 to 8191>
1 <bits coding characters 8192 to 12287>
0 110110111000 <bits coding characters 12288 to 15799>
<(optional) bits padding the last byte>
```

We will demonstrate the coding process in examples:
```
the input file consists of 7 characters (the word is marry-go-around in Czech):
Kolotoc

An example Huffman code for this input is: 
K       110
o       0
l       111
t       100 
c       101

Thus, the input is coded as a bit sequence: 
K   o l   o t   o c
110 0 111 0 100 0 101

Such input would form just one chunk (subsequently the last chunk),
thus the bit representation of the characters would be preceded  by the
last chunk indicator (bit 0) and the number of characters (7 characters,
as a 12 bit number, i.e., 000000000111):

0 000000000111 110 0 111 0 100 0 101
```

The compression decreased the input size from 7 bytes to 4 bytes, thus it saves approx. 50% of the size. However, to decompress the file, the decoder needs to know the (de)coding tables. We may save the coding table at the beginning of the compressed file. To save the coding table efficiently, we will keep the coding table in the form of a complete binary tree. Our sample coding table would be represented as:
```
    *
 0/   \1
 o     *
    0/   \1
    *     *
  0/ \1 0/ \1
  t   c K   l
```

The tree can be serialized into the file using pre-order notation. We traverse the tree in a pre-order manner. When we visit an inner node, we write zero bit to the output. When we visit a leaf, we write bit 1 to the output and we add the code of the character being coded by the leaf. The characters is stored in 8 bits (for ASCII characters), or in 8 to 32 bits (UTF-8 sequences). Note that the codes for ASCII characters 0-127 and the UTF-8 sequences for characters 0-127 are the same. The difference comes with characters not included in the ASCII set. In our example the characters are all ASCII characters, thus they are coded using 8 bits. Thus the sample Huffman tree will be coded as:
```
Pre-order tree traversal:
0 1 'o' 0 0 1 't' 1 'c' 0 1 'K' 1 'l' <chunks>

Characters replaced with their corresponding UTF-8 codes:
0 1 01101111 0 0 1 01110100 1 01100011 0 1 01001011 1 01101100 <chunks>

The same with the chunks from the previous paragraph:
0 1 01101111 0 0 1 01110100 1 01100011 0 1 01001011 1 01101100
0 000000000111  110 0 111 0 100 0 101

Bits rearranged into bytes:
01011011 11001011 10100101 10001101 01001011 10110110
00000000 00011111 00111010 00101xxx

Zero padding in the last byte:
01011011 11001011 10100101 10001101 01001011 10110110
00000000 00011111 00111010 00101000

The same as bytes in hexadecimal (10 bytes, example file test0.huf):
5b       cb       a5       8d       4b       b6
00       1f       3a       28
```

Thus the original text (7 bytes) was compressed into 10 bytes. The increase in size is due to the size of the coding tree. If the input were longer, the extra size of the serialized tree would be amortized.

When decompressing the file, your routine must first read the serialized tree. To restore the tree into memory, read the individual bits. If 0 bit is read, create an intermediate tree node and recursively call function to read the left and right subtree. If 1 bit is read, read the following ASCII/UTF-8 code and create the corresponding leaf node.

Huffman tree can be used even to decompress the file. Once the tree is read from the file and restored in memory, the decompression routine needs to read 0/1 bits from the rest of the file and traverse the tree accordingly. If 0 bit is read, the decompression moves to the left son, bit 1 moves to the right son. When a leaf is reached, the corresponding character is appended to the output and the tree traversal starts again from the root. Repeat until the end of the last chunk.

The task is to implement the two functions compressFile and decompressFile as shown in the attached archive. Both functions take 2 parameters - the names of the input and output files. Both functions read the input, compress/decompress the contents and write the result into the output. The return value is success/failure indication. Both functions return true if the required compression/decompression went ok. If something was wrong during the compression/decompression, the return value must be false. There are many reasons for the functions to fail: input file does not exist / input cannot be read / output cannot be written / invalid file contents (UTF-8 codes were invalid) / ...

The decompression function is reasonably short (the reference is approx 200 source lines, including some debug routines). The decompression function is mandatory. If not present, your program will not be awarded any points. On the other hand, the compression is more laborious. The reference with both compression/decompression implementation is approx. 500 source lines. This is more than we assume reasonable for a homework, thus the compression routine is left as a bonus for those who like the problem. There is an explanation of the Huffman code construction on . If you decide to skip the compression, please leave the compression function in your source and let it return false.

The input test (and thus the characters in the serialized Huffman tree) are coded using UTF-8 encoding. The mandatory tests use characters in the range 0-127, thus the UTF-8 code sequences are always 1 byte (8 bits) long, making the processing significantly easier. If you do not want to deal with the UTF-8 coding, implement a program that reads fixed 8 bits/character. Such solution passes the mandatory tests, thus will be awarded some points (less than 100%).

STL library is available for your implementation. The list of available headers (libraries) is shown below. Your implementation can use the STL libraries, however, it does not have to. If implementing the decompression only, STL is not likely to be very useful. The STL containers are intended for the compression implementation.

### Notes:
- Pay a special attention to the file I/O. The testing environment tests your implementation, it tries nonexistent files, unreadable files, files with invalid contents, ...
- UTF-8 coding was outlined n PA1. You may need to read more about UTF-8, a good explanation is available on Wikipedia. Optional and bonus tests: the testing environment tests input files where the UTF-8 codes are broken (e.g. there are missing some bytes in the multibyte sequences). This applies to both compression (input text file is broken) and decompression (UTF-8 codes in the serialized tree are broken). In these cases, the expected behavior is to return false. 
- You may use either C or C++ file interface, the choice is free. 
- There are pairs of input (.orig)/compressed (.huf) files in the attached archive. File test5.huf is invalid, decompression shall fail. Therefore, there is no test5.orig in the attached archive.
- There exists more than one Huffman code for an input file. For instance, if we swapped 0 and 1 bits in the sample above, the resulting code would be also a Huffman code that could be used for the compression. Similarly, if we swapped 0 and 1 bits in some subtrees, we would get further different valid Huffman codes. This applies specifically to the compression function - if implemented, your compression function may result in a compressed file different from the samples provided, yet the implementation may be correct. A good guideline is the length of the compressed file, the length shall match. The testing environment tries to decompress (using the reference decompression routine) the file previously compressed (using your compression routine) and compares the uncompressed result.
- The compression method is not exactly specified for empty input files, or for uncompressed input files that consist of just one character (e.g., 100 times repeated character 'a'). If the input file is empty, return false (this is specific for the compression, indeed, decompression shall return false too as there would be the last chunk missing). When calling the compression function, the testing environment always uses an input file that contains at least two different characters. 
- This homework solution may be used in the code review. However, the code review expects C++ specific constructs in the source (classes, ...). Do not submit this homework source for code review if your implementation uses only C-like constructs; such solution would be awarded only a few points. A final note: classes may be surprisingly useful in this homework.
