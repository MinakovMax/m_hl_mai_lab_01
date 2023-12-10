./test_no_cache.sh

Running 10s test @ http://localhost:8080/
1 threads and 1 connections
Thread Stats Avg Stdev Max +/- Stdev
Latency 2.79ms 3.28ms 65.42ms 95.56%
Req/Sec 399.19 51.41 490.00 75.00%
Latency Distribution
50% 1.96ms
75% 3.23ms
90% 4.70ms
99% 11.05ms
3983 requests in 10.02s, 1.01MB read
Non-2xx or 3xx responses: 3983
Requests/sec: 397.42
Transfer/sec: 103.64KB

Running 10s test @ http://localhost:8080/
2 threads and 2 connections
Thread Stats Avg Stdev Max +/- Stdev
Latency 3.66ms 2.87ms 34.94ms 88.30%
Req/Sec 297.01 50.85 558.00 73.50%
Latency Distribution
50% 2.87ms
75% 4.75ms
90% 6.86ms
99% 13.84ms
5938 requests in 10.05s, 1.65MB read
Non-2xx or 3xx responses: 3027
Requests/sec: 590.80
Transfer/sec: 167.90KB

Running 10s test @ http://localhost:8080/
5 threads and 5 connections
Thread Stats Avg Stdev Max +/- Stdev
Latency 5.78ms 4.58ms 59.67ms 84.93%
Req/Sec 188.21 39.96 353.00 68.00%
Latency Distribution
50% 4.83ms
75% 7.71ms
90% 11.10ms
99% 20.90ms
9421 requests in 10.08s, 2.77MB read
Non-2xx or 3xx responses: 1892
Requests/sec: 935.00
Transfer/sec: 281.87KB

Running 10s test @ http://localhost:8080/
10 threads and 10 connections
Thread Stats Avg Stdev Max +/- Stdev
Latency 9.75ms 7.10ms 81.84ms 72.44%
Req/Sec 109.59 30.09 220.00 68.80%
Latency Distribution
50% 8.69ms
75% 13.37ms
90% 18.38ms
99% 33.21ms
11001 requests in 10.09s, 3.22MB read
Non-2xx or 3xx responses: 3395
Requests/sec: 1089.76
Transfer/sec: 326.42KB
