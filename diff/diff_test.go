package diff

import (
	"reflect"
	"testing"
)

var tests = []struct {
	a, b []int
	want []ResultItem[int]
}{
	{[]int{1, 2, 3, 4, 5}, []int{1, 3, 5}, []ResultItem[int]{{Value: 1, Flag: 0}, {Value: 2, Flag: -1}, {Value: 3, Flag: 0}, {Value: 4, Flag: -1}, {Value: 5, Flag: 0}}},
	{[]int{1, 2, 3, 4, 5}, []int{1, 3, 5, 6}, []ResultItem[int]{{Value: 1, Flag: 0}, {Value: 2, Flag: -1}, {Value: 3, Flag: 0}, {Value: 4, Flag: -1}, {Value: 5, Flag: 0}, {Value: 6, Flag: 1}}},
}

func TestDiff(t *testing.T) {
	for _, test := range tests {
		got := Diff(test.a, test.b)
		if !reflect.DeepEqual(got, test.want) {
			t.Errorf("Diff(%v, %v) = %v, want %v", test.a, test.b, got, test.want)
		}
	}
}

func TestDiffAndPrint(t *testing.T) {
	for _, test := range tests {
		DiffAndPrintln(test.a, test.b)
	}
}
