# Parallel HTTP Client

Leverages sockets and processes to fork off n number of processes that each send a http request to a specified domain name and port concurrently.

1. make
2. ./bin/thor -n REQUESTS [domain_name:port]

Example: ./bin/thor -n 10 www.google.com



