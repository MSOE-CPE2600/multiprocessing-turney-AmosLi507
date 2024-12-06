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

### Runtime Data

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
