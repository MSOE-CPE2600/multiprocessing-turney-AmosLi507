# System Programming Lab 11: Multiprocessing
Amos (Mohan) Li  
CPE 2600-121  
11/25/2024  
Lab 11: Multiprocessing  

### Overview of My Implementation for Lab 11
I modified the `mandel.c` file to add functionality for generating 50 slightly different images by slightly changing parameters through multiprocessing with assigned number of child process.
All the images are saved in the `image` directory and can be compiled into a movie using the following command:  
```bash
ffmpeg -i mandel%d.jpg mandel.mpg
```

### Overview of My Implementation for Lab 12
I modified the 'mandel.c' file to add one function which calculate each chunk of image. Each image is divided into multiple chucks based on the number of thread request(1~20), and each thread work at the same time with the shared memory(in the struct typedef).
```bash
void* compute_image_chunk(void* thread_ptr);
```

And a struct typedef to storge the shared memory

```bash
 typedef struct {
...
} ThreadArgs;
```

and modified compute_image() to compute each image by multi-threading
```bash
void compute_image(imgRawImage* img, double xmin, double xmax, double ymin, double ymax, int max, int num_thread );
```

### Runtime Data for Lab 11

| # Processes | Runtime (`real`) in seconds |
|-------------|-----------------------------|
| 1           | 67.614s                     |
| 2           | 30.542s                     |
| 5           | 16.106s                     |
| 10          | 11.659s                     |
| 20          | 10.239s                     |

[Download the Excel File](./Runtime_Data.xlsx)

### Discussion of result:
Using a single process to generate 50 images takes the longest time, while using multiple processes can significantly reduce it. However, using 10 or more processes takes a similar amount of time, approximately 10 seconds.






## Runtime Table for Lab 12
| Number of Threads / Number of Processes | 1 Process | 5 Processes | 10 Processes | 20 Processes |
|-----------------------------------------|-----------|-------------|--------------|--------------|
| 1 Thread                                | 78.261s   | 22.692s     | 16.438s      | 14.846s      | 
| 5 Threads                               | 37.873s   | 14.879s     | 13.027s      | 12.935s      |
| 10 Threads                              | 25.803s   | 13.962a     | 12.835s      | 13.007s      |
| 20 Threads                              | 18.516s   | 15.204s     | 12.996s      | 13.171s      |

### Discussion of result:

### i. Which technique seemed to impact runtime more – multithreading or multiprocessing. Why do you think that is?
Multiprocessing impacts runtime more than multithreading, because in multiprcessing, each process runs independently in its own memory space, allowing ultiple processes are scheduled across different CPU cores. In my case, runtime is superlong when number of process is 1 does not matter how many is number of threads.
However, for multi-threading, each thread has the shared memory space, and actually overhead of managing thread might could reduce the performance gains. In my case, runtime for 20 processes, 20 threads is slower than runtime of 20 processes, 5 threads.

### ii. Was there a “sweet spot” where optimal (minimal) runtime was achieved?
Based on my data, it is the combination of 10 processes and 10 threads(12.835s). Which is when a balance between the number of processes and threads was reached.
