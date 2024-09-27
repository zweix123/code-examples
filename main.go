package main

import (
	"fmt"
	"math/rand"
	"time"
)

// _ "github.com/zweix123/code-examples/diff"

var (
	mapConst = map[int32]bool{
		240: true,
		241: true,
		630: true,
		34:  true,
		45:  true,
	}
	listConst = []int32{
		240,
		241,
		630,
		34,
		45,
	}
)

// 生成随机数字
func GenerateRandomNumber() int32 {
	return rand.Int31n(700)
}

// 通过a进行检测
func ContainMap(num int32) bool {
	return mapConst[num]
}

// 通过b进行检测
func ContainList(num int32) bool {
	for _, v := range listConst {
		if v == num {
			return true
		}
	}
	return false
}

func main() {
	// benchmark
	const numTests = 1000000
	numbers := make([]int32, numTests)
	for i := range numbers {
		numbers[i] = GenerateRandomNumber()
	}

	// 测试 ContainMap
	start := time.Now()
	for _, n := range numbers {
		ContainMap(n)
	}
	duration1 := time.Since(start)

	// 测试 ContainList
	start = time.Now()
	for _, n := range numbers {
		ContainList(n)
	}
	duration2 := time.Since(start)

	fmt.Printf("ContainMap 耗时: %v\n", duration1)
	fmt.Printf("ContainList 耗时: %v\n", duration2)
}
