minakov86@LAPTOP-S75B91MP:~/m_hl_mai_lab_01$ ./test_cache.sh
Running 10s test @ http://localhost:8080/
1 threads and 1 connections
Thread Stats Avg Stdev Max +/- Stdev
Latency 415.38us 362.62us 15.50ms 99.20%
Req/Sec 2.45k 160.27 2.59k 91.09%
Latency Distribution
50% 381.00us
75% 408.00us
90% 464.00us
99% 720.00us
24592 requests in 10.10s, 7.67MB read
Requests/sec: 2434.94
Transfer/sec: 777.58KB

minakov86@LAPTOP-S75B91MP:~/m_hl_mai_lab_01$ ./test_cache.sh
Running 10s test @ http://localhost:8080/
2 threads and 2 connections
Thread Stats Avg Stdev Max +/- Stdev
Latency 838.75us 608.05us 14.46ms 88.91%
Req/Sec 1.22k 527.59 2.45k 63.68%
Latency Distribution
50% 613.00us
75% 1.14ms
90% 1.47ms
99% 2.22ms
24506 requests in 10.10s, 7.30MB read
Non-2xx or 3xx responses: 7093
Requests/sec: 2426.30
Transfer/sec: 740.41KB

minakov86@LAPTOP-S75B91MP:~/m_hl_mai_lab_01$ ./test_cache.sh
Running 10s test @ http://localhost:8080/
5 threads and 5 connections
Thread Stats Avg Stdev Max +/- Stdev
Latency 1.96ms 1.33ms 23.74ms 91.33%
Req/Sec 537.78 165.58 0.97k 66.20%
Latency Distribution
50% 1.74ms
75% 2.33ms
90% 2.86ms
99% 7.68ms
26791 requests in 10.01s, 7.53MB read
Non-2xx or 3xx responses: 12593
Requests/sec: 2675.95
Transfer/sec: 770.50KB

minakov86@LAPTOP-S75B91MP:~/m_hl_mai_lab_01$ ./test_cache.sh
Running 10s test @ http://localhost:8080/
10 threads and 10 connections
Thread Stats Avg Stdev Max +/- Stdev
Latency 4.19ms 3.08ms 60.25ms 86.53%
Req/Sec 256.64 116.01 1.11k 84.30%
Latency Distribution
50% 3.81ms
75% 4.83ms
90% 6.57ms
99% 16.85ms
25592 requests in 10.07s, 6.76MB read
Non-2xx or 3xx responses: 20558
Requests/sec: 2541.73
Transfer/sec: 687.70KB

minakov86@LAPTOP-S75B91MP:~/m_hl_mai_lab_01$ ./test_no_cache.sh
Running 10s test @ http://localhost:8080/
1 threads and 1 connections
Thread Stats Avg Stdev Max +/- Stdev
Latency 0.95ms 161.13us 5.72ms 91.40%
Req/Sec 1.04k 65.54 1.12k 85.00%
Latency Distribution
50% 0.91ms
75% 0.98ms
90% 1.08ms
99% 1.49ms
10383 requests in 10.00s, 2.64MB read
Non-2xx or 3xx responses: 10383
Requests/sec: 1038.20
Transfer/sec: 270.70KB

minakov86@LAPTOP-S75B91MP:~/m_hl_mai_lab_01$ ./test_no_cache.sh
Running 10s test @ http://localhost:8080/
2 threads and 2 connections
Thread Stats Avg Stdev Max +/- Stdev
Latency 1.07ms 305.07us 9.56ms 92.91%
Req/Sec 0.93k 61.26 1.00k 93.00%
Latency Distribution
50% 1.03ms
75% 1.14ms
90% 1.27ms
99% 1.99ms
18508 requests in 10.00s, 4.71MB read
Non-2xx or 3xx responses: 18508
Requests/sec: 1849.90
Transfer/sec: 482.37KB

minakov86@LAPTOP-S75B91MP:~/m_hl_mai_lab_01$ ./test_no_cache.sh
Running 10s test @ http://localhost:8080/
5 threads and 5 connections
Thread Stats Avg Stdev Max +/- Stdev
Latency 1.77ms 1.20ms 31.61ms 94.15%
Req/Sec 596.75 81.36 710.00 84.20%
Latency Distribution
50% 1.52ms
75% 1.93ms
90% 2.51ms
99% 6.64ms
29726 requests in 10.01s, 8.13MB read
Non-2xx or 3xx responses: 18068
Requests/sec: 2969.50
Transfer/sec: 831.17KB

minakov86@LAPTOP-S75B91MP:~/m_hl_mai_lab_01$ ./test_no_cache.sh
Running 10s test @ http://localhost:8080/
10 threads and 10 connections
Thread Stats Avg Stdev Max +/- Stdev
Latency 2.73ms 1.48ms 19.07ms 78.33%
Req/Sec 377.74 43.16 515.00 70.00%
Latency Distribution
50% 2.39ms
75% 3.35ms
90% 4.53ms
99% 7.86ms
37640 requests in 10.01s, 9.78MB read
Non-2xx or 3xx responses: 33871
Requests/sec: 3759.83
Transfer/sec: 0.98MB
