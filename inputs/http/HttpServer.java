// HttpServer.java -- a minimal but real HTTP/1.1 server, as a toolchain input.
//
// Uses only the JDK (com.sun.net.httpserver, bundled with the JRE). Serves a
// few fixed routes and returns proper status codes, mirroring the routing logic
// of the HttpServer.sleela model in this directory.
//
//   javac HttpServer.java && java HttpServer 8080
//   curl -i http://localhost:8080/health
package inputs.http;

import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;

import java.io.IOException;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.nio.charset.StandardCharsets;

public final class HttpServer {

    public static void main(String[] args) throws IOException {
        int port = args.length > 0 ? Integer.parseInt(args[0]) : 8080;
        com.sun.net.httpserver.HttpServer server =
                com.sun.net.httpserver.HttpServer.create(new InetSocketAddress(port), 0);

        server.createContext("/", new Router());
        server.setExecutor(null); // default single-threaded executor
        server.start();
        System.out.println("HTTP server listening on http://localhost:" + port);
    }

    /** Routes a request to a status code + body, then writes an HTTP response. */
    static final class Router implements HttpHandler {
        @Override
        public void handle(HttpExchange ex) throws IOException {
            String method = ex.getRequestMethod();
            String path = ex.getRequestURI().getPath();

            int code = statusFor(method, path);
            String body = bodyFor(path, code);

            byte[] bytes = body.getBytes(StandardCharsets.UTF_8);
            ex.getResponseHeaders().set("Content-Type", "text/html; charset=utf-8");
            ex.sendResponseHeaders(code, bytes.length);
            try (OutputStream os = ex.getResponseBody()) {
                os.write(bytes);
            }
            System.out.println(method + " " + path + " -> " + code);
        }

        private static boolean isKnownMethod(String m) {
            return m.equals("GET") || m.equals("HEAD") || m.equals("POST");
        }

        private static int statusFor(String method, String path) {
            if (!isKnownMethod(method)) {
                return 405;
            }
            switch (path) {
                case "/":
                case "/index.html":
                case "/health":
                    return 200;
                default:
                    return 404;
            }
        }

        private static String bodyFor(String path, int code) {
            if (code != 200) {
                return "error " + code;
            }
            if (path.equals("/health")) {
                return "OK";
            }
            return "<html><body>Sleela HTTP model</body></html>";
        }
    }
}
