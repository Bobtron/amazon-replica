Problem 1:
This one was pretty straight forward, I just implemented all the functionality within the mydatastore database,
and in the user, where the datastore gives the user a password to cross reference and the user
returns true if it matches the hashcode stored

Problem 2:
This one was similar to the heap we did before, although the big changes I make were adding the hashtable
which allows us to access each element real fast and I also changed the m-ary heap to 0 based indexing
instead of 1 based indexing which i used

Problem 3:
To solve this graph problem, I used an adjacency matrix, this way I could keep track of all the similarities
folks had, and the products would also keep track of their users, so it was just a matter of calculation
each time REC was called.