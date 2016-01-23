# cache_manager

This is a very naive implementation where multiple reader/writers can update and read an array that is initially defined. The whole fundamental
of doing this excercise was to understand how multiple reader/writers interact and how to avoid starvation in this situation.

The code basically fills up an array that is initially declared with all values as 0. Reader thread will read the value and once the writer thread
runs it will increment the value by 1. Reader threads running after writer threads will acknowledge the new value and will use that value
for their calculation if any.

I have used semaphores for concurrency control and have made sure that the code doesnt run into deadlock or starvation for writer threads to best 
of my knowledge.

To run the code all you have to do is: 

Step 1: make
Step 2: ./fun <#of reader threads> <#of writer threads>

I have left printf statements for clarity and this can be removed.

Much can be done on this and will continue to work on it to make this simulation look like cache_manager. 
