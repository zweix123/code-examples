package diff

import (
	"github.com/zweix123/suger/slice"
)

// LCS: 最长公共子序列
func LCS[T comparable](a, b []T) []T {
	n, m := len(a), len(b)
	f := make([][]int, n+1)
	for i := range f {
		f[i] = make([]int, m+1)
	}

	for i := 0; i < n; i++ {
		for j := 0; j < m; j++ {
			if a[i] == b[j] {
				f[i+1][j+1] = f[i][j] + 1 // trick: 因为索引从0开始, 但是dp依赖前一位, 而0没有前一位, 故dp数组f是比索引多一位的
			} else {
				f[i+1][j+1] = max(f[i][j+1], f[i+1][j])
			}
		}
	}

	lcs := make([]T, 0, f[n][m])
	i, j := n-1, m-1
	for i >= 0 && j >= 0 {
		if a[i] == b[j] {
			lcs = append(lcs, a[i])
			i--
			j--
		} else if f[i][j+1] > f[i+1][j] {
			i--
		} else {
			j--
		}
	}

	lcs = slice.Reverse(lcs)
	return lcs
}
