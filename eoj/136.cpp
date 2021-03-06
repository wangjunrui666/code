#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <algorithm>

using namespace std;

const int N = 2e6 + 10;

typedef long long ll;

int n, len;

const ll p = 998244353ll, g[2] = {3, (p + 1) / 3};

ll a[N], aa[N], b[N], t[N], c[N], f[N];

ll pw(ll a, ll b) {
    ll res = 1;
    while(b) {
        if(b & 1) res = res * a % p;
        b >>= 1, a = a * a % p;
    }
    return res;
}

ll inv(ll x) {
    return pw(x, p - 2);
}

int rev(int x, int n) {
    int res = 0;
    for(int i = 0 ; (1 << i) < n ; ++ i) (res <<= 1) |= (x >> i) & 1;
    return res;
}

void ntt(ll *t, int n, int d) {
    for(int i = 0 ; i < n ; ++ i) f[rev(i, n)] = t[i];
    for(int i = 2 ; i <= n ; i <<= 1) {
        ll wn = pw(g[d], (p - 1) / i);
        for(int j = 0 ; j < n ; j += i) {
            ll w = 1;
            for(int k = j ; k < j + i / 2 ; ++ k) {
                ll u = f[k], v = w * f[k + i / 2];
                f[k] = (u + v) % p;
                f[k + i / 2] = (u - v + p) % p;
                w = w * wn % p;
            }
        }
    }
    for(int i = 0 ; i < n ; ++ i) t[i] = (d ? f[i] * inv(n) : f[i]);
}

void sol(int x) {
    if(x == 1) {
        b[0] = inv(a[0]);
    } else {
        sol((x + 1) / 2);
        for(int i = 0 ; i < x ; ++ i) t[i] = 2 * b[i] % p, aa[i] = a[i];
        int len = 1;
        while(len <= x * 2) len <<= 1;
        for(int i = x ; i < len ; ++ i) aa[i] = 0;
        ntt(aa, len, 0);
        ntt(b, len, 0);
        for(int i = 0 ; i < len ; ++ i) c[i] = aa[i] * b[i] % p * b[i] % p;
        ntt(c, len, 1);
        for(int i = 0 ; i < x ; ++ i) b[i] = ((t[i] - c[i] + p) % p + p) % p;
        for(int i = x ; i < len ; ++ i) b[i] = 0;
    }
}

ll fac[N], invfac[N];

int q[N]; int qs;

int main() {

    scanf("%d", &qs);
    for(int i = 1 ; i <= qs ; ++ i) {
        scanf("%d", &q[i]);
        n = max(n, q[i]);
    }

    fac[0] = invfac[0] = 1;
    for(int i = 1 ; i <= n ; ++ i) {
        fac[i] = fac[i - 1] * i % p;
    }
    invfac[n] = pw(fac[n], p - 2);
    for(int i = n - 1 ; i ; -- i) {
        invfac[i] = invfac[i + 1] * (i + 1) % p;
    }

    n += 10;
    aa[0] = a[0] = 1;
    for(int i = 1 ; i < n ; ++ i) {
        aa[i] = a[i] = pw(i, p - 2);
    }
    sol(n);
    for(int i = 1 ; i < n ; ++ i) {
        b[i] = (b[i - 1] + b[i]) % p;
    }
    for(int i = 1 ; i <= n ; ++ i) {
        b[i] = b[i] * fac[i] % p;
        f[i] = (fac[i - 1] - b[i - 1]) % p;
        f[i] = f[i] * invfac[i - 1] % p;
        if(f[i] < 0) f[i] += p;
    }

    for(int i = 1 ; i <= qs ; ++ i) {
        printf("%lld\n", f[q[i]]);
    }
}
