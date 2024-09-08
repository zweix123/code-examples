package diff

import "testing"

func TestSED(t *testing.T) {
	tests := []struct {
		a, b []int
		want int
	}{
		{[]int{1, 2, 3, 4, 5}, []int{1, 3, 5}, 2},
		{[]int{1, 2, 3, 4, 5}, []int{1, 2, 3, 4, 5}, 0},
		{[]int{1, 2, 3, 4, 5}, []int{1, 2, 3, 4, 5, 6}, 1},
		{[]int{1, 2, 3, 4, 5}, []int{6, 7, 8, 9, 10}, 5},
		{[]int{1, 2, 3, 4, 5}, []int{}, 5},
	}

	for _, test := range tests {
		got := SED(test.a, test.b)
		if got != test.want {
			t.Errorf("SED(%v, %v) = %v, want %v", test.a, test.b, got, test.want)
		}
	}
}
