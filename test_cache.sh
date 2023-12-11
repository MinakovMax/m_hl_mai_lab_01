wrk -d 10 -t 1 -c 1 --latency -s ./get_cache.lua http://localhost:8080/

wrk -d 10 -t 2 -c 2 --latency -s ./get_cache.lua http://localhost:8080/

wrk -d 10 -t 5 -c 5 --latency -s ./get_cache.lua http://localhost:8080/

wrk -d 10 -t 10 -c 10 --latency -s ./get_cache.lua http://localhost:8080/