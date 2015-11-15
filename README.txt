Title : Xtrac, Text File Compression
Name : Siddhesh Rane
MIS : 111408057

Xtrac is a text file compression program. It saves space by reducing statistical redundancy in the input file.
It is ideally suitable for human languages as they tend to have a lot of repeating words.
Though primarily designed for human language compression, it is also suitable for computer program source code,  HTML/XML, CSS etc.

A lot of work has been done in REDUCING the number of lines of code in favour of a cleaner and leaner implementation.
Code has been properly organized into compression, decompression, utility and program entry point modules.
List data type (list.c) has been written with encapsulation and abstraction in mind. 
The data type provides methods to perform the most common operations thus discouraging direct access of its members.
Thus the background implementation can be changed and improved without having to re-write the application code that uses the data type.

Clear and consistent style and convention for the API have been incorporated throughout the program.
Every subcomponent and method is robust with proper null checks and an early  fail-fast behaviour that doesnt proceed further before checking for errors.
The command line arguments follow the GNU/Linux convention as much as possible.

Besides this there is a lot of other work that could not be included due to its experimental nature.