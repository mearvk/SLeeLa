// ===========================================================================
// source_resolve.cpp  --  Filesystem resolution of the source section (§V).
// ===========================================================================
#include "source_resolve.h"

#include <algorithm>
#include <sys/stat.h>
#include <dirent.h>

namespace nordshrift {
namespace {

bool isDir(const std::string& p) {
    struct stat st;
    return stat(p.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
}
bool exists(const std::string& p) {
    struct stat st;
    return stat(p.c_str(), &st) == 0;
}

std::string join(const std::string& a, const std::string& b) {
    if (a.empty()) return b;
    if (a.back() == '/') return a + b;
    return a + "/" + b;
}

// Recursively collect every regular file under `dir`, returning paths relative
// to `base` (which is `dir` at the top of the recursion).
void walk(const std::string& dir, const std::string& rel,
          std::vector<std::string>& out) {
    DIR* d = opendir(dir.c_str());
    if (!d) return;
    struct dirent* e;
    while ((e = readdir(d)) != nullptr) {
        std::string name = e->d_name;
        if (name == "." || name == "..") continue;
        std::string full = join(dir, name);
        std::string r = rel.empty() ? name : rel + "/" + name;
        if (isDir(full)) walk(full, r, out);
        else out.push_back(r);
    }
    closedir(d);
}

// Match a path against a glob pattern supporting `*`, `?`, and `**`.
// `**` matches any number of path segments (including zero); `*` and `?` do not
// cross `/`. Implemented as a small recursive matcher over the two strings.
bool globMatch(const char* pat, const char* str) {
    while (*pat) {
        if (pat[0] == '*' && pat[1] == '*') {
            // consume the '**' and an optional following '/'
            const char* p = pat + 2;
            if (*p == '/') p++;
            if (*p == '\0') return true;                 // '**' at end matches all
            // try to match the remainder at every position (segment boundaries)
            for (const char* s = str; ; s++) {
                if (globMatch(p, s)) return true;
                if (*s == '\0') return false;
            }
        }
        if (*pat == '*') {
            const char* p = pat + 1;
            if (*p == '\0') {                            // trailing '*': match rest of segment
                for (const char* s = str; *s; s++) if (*s == '/') return false;
                return true;
            }
            for (const char* s = str; ; s++) {
                if (globMatch(p, s)) return true;
                if (*s == '\0' || *s == '/') return false;   // '*' doesn't cross '/'
            }
        }
        if (*str == '\0') return false;
        if (*pat == '?') { if (*str == '/') return false; }
        else if (*pat != *str) return false;
        pat++; str++;
    }
    return *str == '\0';
}

} // namespace

std::vector<std::string> resolveSources(const Source& src,
                                        const std::string& sheetDir,
                                        DiagnosticBag& diags) {
    std::vector<std::string> result;

    std::string root = src.root;
    // resolve root relative to the sheet directory unless absolute/home
    std::string rootPath = (!root.empty() && (root[0] == '/' || root[0] == '~'))
                           ? root : join(sheetDir, root);

    if (!src.hasRoot || root.empty() || !exists(rootPath)) {
        diags.error("NSS-E-0030", src.present ? "" : "", src.rootLine,
                    "source.root path does not exist: '" + rootPath + "'", "SRC-01");
        return result;
    }
    if (!isDir(rootPath)) {
        diags.error("NSS-E-0030", "", src.rootLine,
                    "source.root is not a directory: '" + rootPath + "'", "SRC-01");
        return result;
    }

    // enumerate all files under root (relative paths)
    std::vector<std::string> all;
    walk(rootPath, "", all);

    // apply globs (union), then exclude
    std::vector<std::string> selected;
    for (const auto& f : all) {
        bool inc = false;
        for (const auto& g : src.globs) if (globMatch(g.c_str(), f.c_str())) { inc = true; break; }
        if (!inc) continue;
        bool exc = false;
        for (const auto& x : src.exclude) if (globMatch(x.c_str(), f.c_str())) { exc = true; break; }
        if (exc) continue;
        selected.push_back(join(rootPath, f));
    }

    std::sort(selected.begin(), selected.end());
    selected.erase(std::unique(selected.begin(), selected.end()), selected.end());

    if (selected.empty()) {
        diags.error("NSS-E-0031", "", src.line,
                    "source file set is empty after glob and exclude evaluation", "SRC-02");
    }
    return selected;
}

} // namespace nordshrift
