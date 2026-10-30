# HTTP server command options

The command family supports HTTP/1, HTTP/2, and HTTP/3:

```sh
sleela http-server 1 --port 8080 --threads 4 --root ./www --log ./build/http1.log --once
sleela http-server 2 --port 8082 --threads 4 --root ./www --log ./build/http2.log --once
sleela http-server 3 --port 8083 --threads 4 --root ./www --log ./build/http3.log --once
```

Options documented by the manual: `--port N`, `--threads N`, `--root DIR`,
`--log FILE`, and `--once`. These commands start servers; use a test port,
a controlled document root, and an explicit shutdown plan. HTTP/2 and HTTP/3
may require platform/backend dependencies.
