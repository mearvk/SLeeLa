#include "sleela_post.h"
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

static int fail(char *error, size_t n, const char *msg) {
    if (error && n) snprintf(error, n, "%s", msg);
    return -1;
}

static int write_all(int fd, const char *p, size_t n) {
    while (n) {
        ssize_t w = write(fd, p, n);
        if (w < 0) { if (errno == EINTR) continue; return -1; }
        if (w == 0) return -1;
        p += (size_t)w; n -= (size_t)w;
    }
    return 0;
}

int sleela_post_validate(const sleela_post_t *post, char *error, size_t error_size) {
    if (!post) return fail(error,error_size,"post is null");
    if (!post->method || strcmp(post->method,"POST") != 0)
        return fail(error,error_size,"post method must be POST");
    if (!post->target || !post->target[0] || post->target[0] != '/')
        return fail(error,error_size,"post target must be an absolute path");
    if (!post->content_type || !post->content_type[0])
        return fail(error,error_size,"post content type is required");
    if (!post->body) return fail(error,error_size,"post body is required");
    if (strlen(post->target) > 4096 || strlen(post->content_type) > 256 ||
        strlen(post->body) > 1024 * 1024)
        return fail(error,error_size,"post field exceeds SLeeLa limit");
    return 0;
}

int sleela_post_write(const sleela_post_t *post, int fd, char *error, size_t error_size) {
    char line[4608];
    if (sleela_post_validate(post,error,error_size) != 0) return -1;
    int n = snprintf(line,sizeof(line),"POST %s HTTP/1.1\r\nContent-Type: %s\r\nContent-Length: %zu\r\n\r\n",
                     post->target,post->content_type,strlen(post->body));
    if (n < 0 || (size_t)n >= sizeof(line)) return fail(error,error_size,"post header too large");
    if (write_all(fd,line,(size_t)n) != 0 || write_all(fd,post->body,strlen(post->body)) != 0)
        return fail(error,error_size,"post write failed");
    return 0;
}
