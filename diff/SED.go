package diff

// SED: 最小编辑距离
func SED[T comparable](a, b []T) int {
	n, m := len(a), len(b)
	f := make([][]int, n+1)
	for i := range f {
		f[i] = make([]int, m+1)
	}

	for i := 0; i <= n; i++ {
		f[i][0] = i
	}
	for j := 0; j <= m; j++ {
		f[0][j] = j
	}

	for i := 0; i < n; i++ {
		for j := 0; j < m; j++ {
			if a[i] == b[j] {
				f[i+1][j+1] = f[i][j]
			} else {
				f[i+1][j+1] = min(f[i][j], f[i+1][j], f[i][j+1]) + 1
			}
		}
	}

	return f[n][m]
}
