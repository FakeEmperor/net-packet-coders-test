# Network coders tester
***Hamming coders, CRC32, Shannon-Fano, LZW compression, JPEG codecs***

This repo holds a server written in C++ 
which can compress data using *LZW, JPEG (yes, even text), Shannon-Fano* 
, or encode it using *Hamming error-correcting code (ECC)*
, or compute *CRC32 checksum*.

Of course, it's the code which is of interest and not the
server itself.

## How to build
You need **C++17**-compatible compiler and **CMake 3.8**. 

Tested and working on these compilers:
<table>
<tr>
    <th>OS</th>
    <th>Compiler</th>
    <th>Version</th>
</tr>
<tr>
    <td>Windows</td>
    <td>Visual Studio C++</td>
    <td>2017</td>
</tr>
<tr>
    <td>Windows</td>
    <td>Visual Studio C++</td>
    <td>2015</td>
</tr>
<tr>
    <td>Windows</td>
    <td>MinGW-64</td>
    <td>7.3</td>
</tr>
<tr>
    <td>Linux</td>
    <td>cmake</td>
    <td>cmake3.6 and gcc6.3</td>
</tr>
</table>


You can compile on these IDEs:
<table>
<tr>
    <th>OS</th>
    <th>IDE</th>
    <th>Version</th>
</tr>
<tr>
    <td>Windows</td>
    <td>Visual Studio</td>
    <td>2017</td>
</tr>
<tr>
    <td>Windows</td>
    <td>Visual Studio</td>
    <td>2015</td>
</tr>
<tr>
    <td>Windows/Linux</td>
    <td>CLion</td>
    <td>2017.1</td>
</tr>
</table>

## How to test
You can run python tests from `tests` folder.
Python3 interpreter with `zlib` and `unittest` libraries
*(they usually included by default)*.
Here is the list of tests:

* `test_crc32.py` 
    * `testEncode` - test CRC32 checksum returned  

* `test_hamming.py` 
    * `testEncode` - 
    test Hamming has returned (without algo check)
    * `testDecode` - 
    test Hamming can decode itself (**without errors**)
    * `testDecodeSingleError` - 
    test Hamming can detect errors and 
    decode itself (**with 1 error**)
    * `testDecodeSingleError` - 
    test Hamming can detect errors (**with 2 error**)
        
     
* `test_shannon_fano.py` 
    * `testEncode` - 
    test Shannon-Fano compression 
    * `testDecode` - 
    test Shannon-Fano decompression
    
* `test_lzw.py` 
    * `testEncode` - test LZW compression 
    * `testDecode` - test LZW decompression 
