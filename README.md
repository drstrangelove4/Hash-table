My first hash table. It's rough. Taking user input is a pain point and it could do with a refactor but after 3 days and about 30 hours of work put into it, it finally works.

Need to add the ability to edit nodes but I've lost all motivation to do so. I really should fix the uninitalized/unused values in the root structs.
Probably easier to make a root struct and place them instead of using one struct for everything in the hash table. I also need to add some functionality to allow the adding of nodes, etc.
The code in main is just used to demonstrate all the functions work correctly. 

This is my second attempt. My first version couldn't free memory correctly due to the root elements being hard coded into the system and some pointer wizardry I couldn't figure out. 
I tried to use malloc to take string inputs from the user but I couldn't figure out how to free them so I resulted to copying the contents of the user input into the struct 
instead of using malloc and a pointer.

I've learned alot of about pointers, malloc and taking user input doing this but fuck me I wish taking input and freeing memory was easier in C. There seems to be a 100 ways people
do these things, each with their own pitfalls. Having one standard way to do it with good documentation would make it so much easier.
