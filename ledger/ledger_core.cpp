// ===========================================================================
// ledger_core.cpp -- implementation of the shared .ledger core.
// ===========================================================================
#include "ledger_core.h"

#include <array>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <fstream>
#include <sstream>

namespace ledger {

// ---- SHA-256 (FIPS 180-4) --------------------------------------------------
namespace {
inline uint32_t rotr(uint32_t x, int n) { return (x >> n) | (x << (32 - n)); }

const uint32_t K[64] = {
  0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
  0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
  0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
  0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
  0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
  0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
  0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
  0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2};

std::string to_hex(const uint8_t* d, size_t n) {
    static const char* hx = "0123456789abcdef";
    std::string s; s.resize(n * 2);
    for (size_t i = 0; i < n; ++i) { s[2*i] = hx[d[i] >> 4]; s[2*i+1] = hx[d[i] & 0xF]; }
    return s;
}
} // namespace

std::string sha256_hex(const std::string& data) {
    uint32_t h[8] = {0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,
                     0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19};
    std::string msg = data;
    uint64_t bitlen = (uint64_t)msg.size() * 8;
    msg.push_back((char)0x80);
    while (msg.size() % 64 != 56) msg.push_back((char)0x00);
    for (int i = 7; i >= 0; --i) msg.push_back((char)((bitlen >> (i * 8)) & 0xFF));

    for (size_t off = 0; off < msg.size(); off += 64) {
        uint32_t w[64];
        for (int i = 0; i < 16; ++i) {
            w[i] = ((uint8_t)msg[off+i*4] << 24) | ((uint8_t)msg[off+i*4+1] << 16) |
                   ((uint8_t)msg[off+i*4+2] << 8) | ((uint8_t)msg[off+i*4+3]);
        }
        for (int i = 16; i < 64; ++i) {
            uint32_t s0 = rotr(w[i-15],7) ^ rotr(w[i-15],18) ^ (w[i-15] >> 3);
            uint32_t s1 = rotr(w[i-2],17) ^ rotr(w[i-2],19) ^ (w[i-2] >> 10);
            w[i] = w[i-16] + s0 + w[i-7] + s1;
        }
        uint32_t a=h[0],b=h[1],c=h[2],d=h[3],e=h[4],f=h[5],g=h[6],hh=h[7];
        for (int i = 0; i < 64; ++i) {
            uint32_t S1 = rotr(e,6)^rotr(e,11)^rotr(e,25);
            uint32_t ch = (e & f) ^ (~e & g);
            uint32_t t1 = hh + S1 + ch + K[i] + w[i];
            uint32_t S0 = rotr(a,2)^rotr(a,13)^rotr(a,22);
            uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
            uint32_t t2 = S0 + maj;
            hh=g; g=f; f=e; e=d+t1; d=c; c=b; b=a; a=t1+t2;
        }
        h[0]+=a; h[1]+=b; h[2]+=c; h[3]+=d; h[4]+=e; h[5]+=f; h[6]+=g; h[7]+=hh;
    }
    uint8_t out[32];
    for (int i = 0; i < 8; ++i) {
        out[i*4]   = (h[i] >> 24) & 0xFF; out[i*4+1] = (h[i] >> 16) & 0xFF;
        out[i*4+2] = (h[i] >> 8) & 0xFF;  out[i*4+3] = h[i] & 0xFF;
    }
    return to_hex(out, 32);
}

std::string sha256_file(const std::string& path) {
    std::ifstream f(path, std::ios::binary);
    if (!f) return "";
    std::ostringstream ss; ss << f.rdbuf();
    return sha256_hex(ss.str());
}

std::string utc_iso8601() {
    std::time_t t = std::time(nullptr);
    std::tm gm{};
#if defined(_WIN32)
    gmtime_s(&gm, &t);
#else
    gmtime_r(&t, &gm);
#endif
    char buf[40];
    std::snprintf(buf, sizeof(buf), "%04d-%02d-%02dT%02d:%02d:%02dZ",
                  gm.tm_year + 1900, gm.tm_mon + 1, gm.tm_mday,
                  gm.tm_hour, gm.tm_min, gm.tm_sec);
    return std::string(buf);
}

// ---- QR encoder (byte mode, EC 'M') mirroring ledger/qr.py -----------------
namespace {
int GEXP[512], GLOG[256];
struct GFInit { GFInit() {
    int x = 1;
    for (int i = 0; i < 255; ++i) { GEXP[i] = x; GLOG[x] = i; x <<= 1; if (x & 0x100) x ^= 0x11D; }
    for (int i = 255; i < 512; ++i) GEXP[i] = GEXP[i - 255];
} } _gfinit;
int gmul(int a, int b) { return (a == 0 || b == 0) ? 0 : GEXP[GLOG[a] + GLOG[b]]; }

std::vector<int> rs_gen(int n) {
    std::vector<int> g{1};
    for (int i = 0; i < n; ++i) {
        std::vector<int> g2(g.size() + 1, 0);
        for (size_t j = 0; j < g.size(); ++j) {
            g2[j] ^= g[j];
            g2[j+1] ^= gmul(g[j], GEXP[i]);
        }
        g = g2;
    }
    return g;
}
std::vector<int> rs_encode(const std::vector<int>& data, int n) {
    std::vector<int> gen = rs_gen(n), res(n, 0);
    for (int d : data) {
        int factor = d ^ res[0];
        for (size_t i = 0; i + 1 < res.size(); ++i) res[i] = res[i+1];
        res[n-1] = 0;
        for (int i = 0; i < n; ++i) res[i] ^= gmul(gen[i+1], factor);
    }
    return res;
}

// EC table subset (versions 1..10, level M): (ecc, b1, d1, b2, d2)
struct ECRow { int ecc, b1, d1, b2, d2; };
ECRow EC_M(int v) {
    static const ECRow t[11] = {
        {0,0,0,0,0},
        {10,1,16,0,0},{16,1,28,0,0},{26,1,44,0,0},{18,2,32,0,0},{24,2,43,0,0},
        {16,4,27,0,0},{18,4,31,0,0},{22,2,38,2,39},{22,3,36,2,37},{26,4,43,1,44}
    };
    return t[v];
}
int total_data_cw(int v) { ECRow r = EC_M(v); return r.b1*r.d1 + r.b2*r.d2; }

std::vector<int> align_pos(int v) {
    switch (v) {
        case 1: return {};
        case 2: return {6,18}; case 3: return {6,22}; case 4: return {6,26};
        case 5: return {6,30}; case 6: return {6,34}; case 7: return {6,22,38};
        case 8: return {6,24,42}; case 9: return {6,26,46}; case 10: return {6,28,50};
    }
    return {};
}

int choose_version(size_t nbytes) {
    for (int v = 1; v <= 10; ++v) {
        int count_bits = v <= 9 ? 8 : 16;
        int cap_bits = total_data_cw(v) * 8;
        int need = 4 + count_bits + (int)nbytes * 8;
        if (need <= cap_bits) return v;
    }
    return 10; // clamp; identity strings fit within v10 for our use
}

std::vector<int> data_codewords(const std::string& text, int v) {
    int count_bits = v <= 9 ? 8 : 16;
    std::vector<int> bits;
    auto put = [&](int val, int n){ for (int i = n-1; i >= 0; --i) bits.push_back((val>>i)&1); };
    put(0b0100, 4);
    put((int)text.size(), count_bits);
    for (unsigned char ch : text) put(ch, 8);
    int cap = total_data_cw(v) * 8;
    int term = std::min(4, cap - (int)bits.size());
    for (int i = 0; i < term; ++i) bits.push_back(0);
    while (bits.size() % 8 != 0) bits.push_back(0);
    std::vector<int> cw;
    for (size_t i = 0; i < bits.size(); i += 8) {
        int b = 0; for (int j = 0; j < 8; ++j) b = (b<<1)|bits[i+j];
        cw.push_back(b);
    }
    int pads[2] = {0xEC, 0x11}; int k = 0;
    while ((int)cw.size() < total_data_cw(v)) cw.push_back(pads[k++ % 2]);
    return cw;
}

std::vector<int> interleave(const std::vector<int>& cw, int v) {
    ECRow r = EC_M(v);
    std::vector<std::pair<std::vector<int>,std::vector<int>>> blocks;
    int idx = 0;
    for (int i = 0; i < r.b1; ++i) { std::vector<int> d(cw.begin()+idx, cw.begin()+idx+r.d1); idx+=r.d1; blocks.push_back({d, rs_encode(d, r.ecc)}); }
    for (int i = 0; i < r.b2; ++i) { std::vector<int> d(cw.begin()+idx, cw.begin()+idx+r.d2); idx+=r.d2; blocks.push_back({d, rs_encode(d, r.ecc)}); }
    std::vector<int> out;
    size_t maxd = 0; for (auto& b : blocks) maxd = std::max(maxd, b.first.size());
    for (size_t i = 0; i < maxd; ++i) for (auto& b : blocks) if (i < b.first.size()) out.push_back(b.first[i]);
    for (int i = 0; i < r.ecc; ++i) for (auto& b : blocks) out.push_back(b.second[i]);
    return out;
}

using Mat = std::vector<std::vector<int>>; // -1 = unset
void place_finder(Mat& m, int r, int c) {
    int sz = (int)m.size();
    for (int dr = -1; dr <= 7; ++dr) for (int dc = -1; dc <= 7; ++dc) {
        int rr = r+dr, cc = c+dc;
        if (rr < 0 || rr >= sz || cc < 0 || cc >= sz) continue;
        if (dr >= 0 && dr <= 6 && dc >= 0 && dc <= 6) {
            int v = (dr==0||dr==6||dc==0||dc==6||(dr>=2&&dr<=4&&dc>=2&&dc<=4)) ? 1 : 0;
            m[rr][cc] = v;
        } else m[rr][cc] = 0;
    }
}
void functions(Mat& m, int v) {
    int sz = (int)m.size();
    place_finder(m, 0, 0); place_finder(m, 0, sz-7); place_finder(m, sz-7, 0);
    for (int i = 0; i < sz; ++i) {
        if (m[6][i] < 0) m[6][i] = (i%2==0)?1:0;
        if (m[i][6] < 0) m[i][6] = (i%2==0)?1:0;
    }
    auto ap = align_pos(v);
    for (int r : ap) for (int c : ap) {
        if (m[r][c] >= 0) continue;
        for (int dr = -2; dr <= 2; ++dr) for (int dc = -2; dc <= 2; ++dc)
            m[r+dr][c+dc] = (std::abs(dr)==2||std::abs(dc)==2||(dr==0&&dc==0)) ? 1 : 0;
    }
    m[sz-8][8] = 1;
}
void reserve_format(Mat& m) {
    int sz = (int)m.size();
    for (int i = 0; i < 9; ++i) { if (m[8][i] < 0) m[8][i]=0; if (m[i][8] < 0) m[i][8]=0; }
    for (int i = 0; i < 8; ++i) { if (m[8][sz-1-i] < 0) m[8][sz-1-i]=0; if (m[sz-1-i][8] < 0) m[sz-1-i][8]=0; }
}
std::vector<std::pair<int,int>> data_positions(const Mat& m) {
    int sz = (int)m.size();
    std::vector<std::pair<int,int>> pos;
    int col = sz - 1; bool up = true;
    while (col > 0) {
        if (col == 6) col--;
        if (up) { for (int r = sz-1; r >= 0; --r) { if (m[r][col]<0) pos.push_back({r,col}); if (m[r][col-1]<0) pos.push_back({r,col-1}); } }
        else    { for (int r = 0; r < sz; ++r) { if (m[r][col]<0) pos.push_back({r,col}); if (m[r][col-1]<0) pos.push_back({r,col-1}); } }
        up = !up; col -= 2;
    }
    return pos;
}
bool mask_bit(int k, int r, int c) {
    switch (k) {
        case 0: return (r+c)%2==0; case 1: return r%2==0; case 2: return c%3==0;
        case 3: return (r+c)%3==0; case 4: return (r/2+c/3)%2==0;
        case 5: return (r*c)%2 + (r*c)%3 == 0;
        case 6: return ((r*c)%2 + (r*c)%3)%2==0;
        default: return ((r+c)%2 + (r*c)%3)%2==0;
    }
}
int penalty(const Mat& m) {
    int sz=(int)m.size(), score=0;
    for (int pass = 0; pass < 2; ++pass) {
        for (int a = 0; a < sz; ++a) {
            int run=1;
            for (int b = 1; b < sz; ++b) {
                int cur = pass==0 ? m[a][b] : m[b][a];
                int prev= pass==0 ? m[a][b-1] : m[b-1][a];
                if (cur==prev) run++; else { if (run>=5) score += 3+(run-5); run=1; }
            }
            if (run>=5) score += 3+(run-5);
        }
    }
    for (int r = 0; r < sz-1; ++r) for (int c = 0; c < sz-1; ++c)
        if (m[r][c]==m[r][c+1] && m[r][c]==m[r+1][c] && m[r][c]==m[r+1][c+1]) score += 3;
    // rule 3: finder-like 11-cell pattern, over rows and columns (match qr.py)
    {
        const int pat1[11] = {1,0,1,1,1,0,1,0,0,0,0};
        const int pat2[11] = {0,0,0,0,1,0,1,1,1,0,1};
        for (int pass = 0; pass < 2; ++pass) {
            for (int a = 0; a < sz; ++a) {
                for (int i = 0; i + 11 <= sz; ++i) {
                    bool m1 = true, m2 = true;
                    for (int j = 0; j < 11; ++j) {
                        int cell = pass == 0 ? m[a][i+j] : m[i+j][a];
                        if (cell != pat1[j]) m1 = false;
                        if (cell != pat2[j]) m2 = false;
                    }
                    if (m1 || m2) score += 40;
                }
            }
        }
    }
    int dark=0; for (auto& row : m) for (int v : row) dark += v?1:0;
    int ratio = dark*100/(sz*sz);
    score += 10 * (std::abs(ratio-50)/5);
    return score;
}
int format_bits(int mask) {
    int data = (0b00 << 3) | mask; // level M = 0b00
    int v = data << 10, gen = 0b10100110111;
    for (int i = 14; i >= 10; --i) if ((v>>i)&1) v ^= gen << (i-10);
    return ((data<<10)|v) ^ 0b101010000010010;
}
void apply_format(Mat& m, int mask) {
    int sz=(int)m.size(), fmt=format_bits(mask);
    int bits[15]; for (int i = 0; i < 15; ++i) bits[i]=(fmt>>i)&1;
    int c1[15][2] = {{8,0},{8,1},{8,2},{8,3},{8,4},{8,5},{8,7},{8,8},{7,8},{5,8},{4,8},{3,8},{2,8},{1,8},{0,8}};
    for (int i = 0; i < 15; ++i) m[c1[i][0]][c1[i][1]] = bits[i];
    int c2[15][2] = {{sz-1,8},{sz-2,8},{sz-3,8},{sz-4,8},{sz-5,8},{sz-6,8},{sz-7,8},
                     {8,sz-8},{8,sz-7},{8,sz-6},{8,sz-5},{8,sz-4},{8,sz-3},{8,sz-2},{8,sz-1}};
    for (int i = 0; i < 15; ++i) m[c2[i][0]][c2[i][1]] = bits[i];
}
Mat build_qr(const std::string& text) {
    int v = choose_version(text.size());
    auto data = data_codewords(text, v);
    auto fin = interleave(data, v);
    int sz = 21 + (v-1)*4;
    Mat m(sz, std::vector<int>(sz, -1));
    functions(m, v);
    reserve_format(m);
    auto pos = data_positions(m);
    std::vector<int> bs;
    for (int cw : fin) for (int i = 7; i >= 0; --i) bs.push_back((cw>>i)&1);
    for (size_t i = 0; i < pos.size(); ++i)
        m[pos[i].first][pos[i].second] = (i < bs.size()) ? bs[i] : 0;
    int best = -1; Mat bestM;
    for (int k = 0; k < 8; ++k) {
        Mat cand = m;
        for (auto& p : pos) if (mask_bit(k, p.first, p.second)) cand[p.first][p.second] ^= 1;
        apply_format(cand, k);
        for (auto& row : cand) for (int& v2 : row) if (v2 < 0) v2 = 0;
        int p = penalty(cand);
        if (best < 0 || p < best) { best = p; bestM = cand; }
    }
    return bestM;
}
} // namespace

std::string qr_svg(const std::string& text) {
    Mat m = build_qr(text);
    int sz = (int)m.size(), q = 4, px = 4;
    int dim = (sz + q*2) * px;
    std::ostringstream s;
    s << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"" << dim << "\" height=\"" << dim
      << "\" viewBox=\"0 0 " << dim << " " << dim << "\" shape-rendering=\"crispEdges\">"
      << "<rect width=\"" << dim << "\" height=\"" << dim << "\" fill=\"#ffffff\"/>";
    for (int r = 0; r < sz; ++r) for (int c = 0; c < sz; ++c) if (m[r][c]) {
        int x = (c+q)*px, y = (r+q)*px;
        s << "<rect x=\"" << x << "\" y=\"" << y << "\" width=\"" << px << "\" height=\"" << px << "\" fill=\"#000000\"/>";
    }
    s << "</svg>";
    return s.str();
}

// ---- Records + chain -------------------------------------------------------
static std::string json_escape(const std::string& s) {
    std::string o;
    for (char c : s) {
        switch (c) {
            case '"': o += "\\\""; break;
            case '\\': o += "\\\\"; break;
            case '\n': o += "\\n"; break;
            case '\r': o += "\\r"; break;
            case '\t': o += "\\t"; break;
            default: o += c;
        }
    }
    return o;
}

std::string canonical(const Record& r) {
    std::ostringstream s;
    s << "{\"seq\":" << r.seq
      << ",\"path\":\"" << json_escape(r.path) << "\""
      << ",\"sha256\":\"" << r.sha256 << "\""
      << ",\"prev\":\"" << r.prev << "\""
      << ",\"timestamp_utc\":\"" << r.timestamp_utc << "\""
      << ",\"timestamp_kind\":\"" << json_escape(r.timestamp_kind) << "\""
      << ",\"qr_svg\":\"" << json_escape(r.qr_svg_path) << "\"}";
    return s.str();
}

std::string finalize(Record& r) {
    r.record_hash = sha256_hex(canonical(r));
    return r.record_hash;
}

std::string header_line() {
    std::ostringstream s;
    s << "{\"ledger\":\"sleela\",\"version\":" << LEDGER_VERSION
      << ",\"algo\":\"sha256\",\"chain\":\"prev-hash\",\"genesis\":\"" << GENESIS() << "\"}";
    return s.str();
}

std::string record_line(const Record& r) {
    std::string base = canonical(r);
    // insert record_hash before closing brace
    base.pop_back(); // remove '}'
    std::ostringstream s;
    s << base << ",\"record_hash\":\"" << r.record_hash << "\"}";
    return s.str();
}

std::string qr_identity(int seq, const std::string& path,
                        const std::string& sha, const std::string& ts) {
    std::ostringstream s;
    s << "sleela-ledger:" << LEDGER_VERSION << "|" << seq << "|" << path << "|" << sha << "|" << ts;
    return s.str();
}

} // namespace ledger
