package diff

import (
	"reflect"
	"testing"
)

func TestLCS(t *testing.T) {
	tests := []struct {
		a, b []int
		want []int
	}{
		{[]int{1, 2, 3, 4, 5}, []int{1, 3, 5}, []int{1, 3, 5}},
		{[]int{1, 2, 3, 4, 5}, []int{1, 2, 3, 4, 5}, []int{1, 2, 3, 4, 5}},
		{[]int{1, 2, 3, 4, 5}, []int{1, 2, 3, 4, 5, 6}, []int{1, 2, 3, 4, 5}},
		{[]int{1, 2, 3, 4, 5}, []int{6, 7, 8, 9, 10}, []int{}},
		{[]int{1, 2, 3, 4, 5}, []int{}, []int{}},
	}

	for _, test := range tests {
		got := LCS(test.a, test.b)
		if !reflect.DeepEqual(got, test.want) {
			t.Errorf("LCS(%v, %v) = %v, want %v", test.a, test.b, got, test.want)
		}
	}
}
