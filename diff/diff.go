package diff

import "github.com/gookit/color"

type ResultItem[T any] struct {
	Value T
	Flag  int // -1: 删除; 0: 不变: 1: 新增
}

// Diff: a -> b
func Diff[T comparable](a, b []T) []ResultItem[T] {
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

	l := n + m - f[n][m]
	res := make([]ResultItem[T], l)
	idx := l - 1

	i, j := n-1, m-1
	for i >= 0 && j >= 0 {
		if a[i] == b[j] {
			res[idx] = ResultItem[T]{Value: a[i], Flag: 0}
			i--
			j--
		} else if f[i][j+1] > f[i+1][j] {
			res[idx] = ResultItem[T]{Value: a[i], Flag: -1}
			i--
		} else {
			res[idx] = ResultItem[T]{Value: b[j], Flag: 1}
			j--
		}
		idx--
	}

	return res
}

func DiffAndPrintln[T comparable](a, b []T) {
	res := Diff(a, b)
	for _, v := range res {
		switch v.Flag {
		case -1:
			color.Red.Printf("%v", v.Value)
		case 0:
			color.Printf("%v", v.Value)
		case 1:
			color.Green.Printf("%v", v.Value)
		}
	}
	color.Println()
}
