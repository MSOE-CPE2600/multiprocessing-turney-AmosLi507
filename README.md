# System Programming Lab 11: Multiprocessing
Amos (Mohan) Li  
CPE 2600-121  
11/25/2024  
Lab 11: Multiprocessing  

### Overview of My Implementation
I modified the `mandel.c` file to add functionality for generating 50 slightly different images by slightly changing parameters through multiprocessing with assigned number of child process.
All the images are saved in the `image` directory and can be compiled into a movie using the following command:  
```bash
ffmpeg -i mandel%d.jpg mandel.mpg
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
