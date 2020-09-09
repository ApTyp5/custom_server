# custom_server

## Apache Benchmark tests

### Self server

```
ab -c 100 -n 10000 http://localhost:4243/httptest/wikipedia_russia.html
This is ApacheBench, Version 2.3 <$Revision: 1843412 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking localhost (be patient)
Completed 1000 requests
Completed 2000 requests
Completed 3000 requests
Completed 4000 requests
Completed 5000 requests
Completed 6000 requests
Completed 7000 requests
Completed 8000 requests
Completed 9000 requests
Completed 10000 requests
Finished 10000 requests


Server Software:        custom_server
Server Hostname:        localhost
Server Port:            4243

Document Path:          /httptest/wikipedia_russia.html
Document Length:        954824 bytes

Concurrency Level:      100
Time taken for tests:   21.948 seconds
Complete requests:      10000
Failed requests:        0
Total transferred:      9549170000 bytes
HTML transferred:       9548240000 bytes
Requests per second:    455.63 [#/sec] (mean)
Time per request:       219.475 [ms] (mean)
Time per request:       2.195 [ms] (mean, across all concurrent requests)
Transfer rate:          424893.72 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    3   0.9      3      11
Processing:    13  216  19.4    217     309
Waiting:        2   11  26.7      6     170
Total:         23  219  19.2    220     310

Percentage of the requests served within a certain time (ms)
  50%    220
  66%    223
  75%    225
  80%    226
  90%    229
  95%    233
  98%    250
  99%    265
 100%    310 (longest request)

```

### Nginx

```
ab -c 100 -n 10000 http://localhost:8080/httptest/wikipedia_russia.html
This is ApacheBench, Version 2.3 <$Revision: 1843412 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking localhost (be patient)
Completed 1000 requests
Completed 2000 requests
Completed 3000 requests
Completed 4000 requests
Completed 5000 requests
Completed 6000 requests
Completed 7000 requests
Completed 8000 requests
Completed 9000 requests
Completed 10000 requests
Finished 10000 requests


Server Software:        nginx/1.19.2
Server Hostname:        localhost
Server Port:            8080

Document Path:          /httptest/wikipedia_russia.html
Document Length:        954824 bytes

Concurrency Level:      100
Time taken for tests:   18.214 seconds
Complete requests:      10000
Failed requests:        0
Total transferred:      9550620000 bytes
HTML transferred:       9548240000 bytes
Requests per second:    549.01 [#/sec] (mean)
Time per request:       182.144 [ms] (mean)
Time per request:       1.821 [ms] (mean, across all concurrent requests)
Transfer rate:          512054.03 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    3   0.8      3      12
Processing:    11  179  17.2    177     259
Waiting:        1    6   8.3      5     110
Total:         19  182  17.1    180     259

Percentage of the requests served within a certain time (ms)
  50%    180
  66%    184
  75%    189
  80%    192
  90%    201
  95%    206
  98%    214
  99%    242
 100%    259 (longest request)

```