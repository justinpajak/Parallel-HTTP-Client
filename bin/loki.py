#!/usr/bin/env python3

from socketserver import ThreadingMixIn, ForkingMixIn
from http.server  import SimpleHTTPRequestHandler, HTTPServer

import os
import sys

# Classes

class ForkingHTTPServer(ForkingMixIn, HTTPServer):
    pass

class ThreadingHTTPServer(ThreadingMixIn, HTTPServer):
    pass

# Functions

def usage(exit_code=0):
    progname = os.path.basename(sys.argv[0])
    print(f'''Usage: {progname} [options]
    -f, --forking           Use process per connection concurrency.
    -t, --threading         Use thread per connection concurrency.
    -p, --port      PORT    Listen on specified port.
    -d, --directory PATH    Server files from specified path.''')
    sys.exit(exit_code)

def main():
    server_class = HTTPServer
    arguments    = sys.argv[1:]
    port         = 8000
    directory    = os.getcwd()

    while arguments and arguments[0].startswith('-'):
        argument = arguments.pop(0)
        if argument in ('-f', '--forking'):
            server_class = ForkingHTTPServer
        elif argument in ('-t', '--threading'):
            server_class = ThreadingHTTPServer 
        elif argument in ('-p', '--port'):
            port = int(arguments.pop(0))
        elif argument in ('-d', '--directory'):
            directory = arguments.pop(0)
        elif argument in ('-h', '--help'):
            usage(0)
        else:
            usage(1)

    print(f'HTTP server listening on port {port} and serving from {directory}')
    os.chdir(directory)
    server = server_class(('0.0.0.0', port), SimpleHTTPRequestHandler)
    server.serve_forever()

# Main Execution

if __name__ == '__main__':
    main()
