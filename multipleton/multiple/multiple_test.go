package multiple

import (
	"sync"
	"sync/atomic"
	"testing"
)

func TestMultiple(t *testing.T) {
	var counter int64

	type Sam struct{}

	NewSam := func(counter *int64) *Sam {
		atomic.AddInt64(counter, 1)
		return &Sam{}
	}

	var (
		sam       *Sam
		multiTime Multiple
	)

	SingletonNewSam := func() *Sam {
		multiTime.Do(func() {
			sam = NewSam(&counter)
		})
		return sam
	}

	var wg sync.WaitGroup
	for i := 0; i < 10; i++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			SingletonNewSam()
		}()
	}
	wg.Wait()

	if atomic.LoadInt64(&counter) != 1 {
		t.Errorf("expected counter to be 1, got %d", counter)
	}
}

func TestRenew(t *testing.T) {
	var counter int64

	type Sam struct{}

	NewSam := func(counter *int64) *Sam {
		atomic.AddInt64(counter, 1)
		return &Sam{}
	}

	var (
		sam       *Sam
		multiTime Multiple
	)

	SingletonNewSam := func() *Sam {
		multiTime.Do(func() {
			sam = NewSam(&counter)
		})
		return sam
	}

	SingletonRenew := func() {
		multiTime.Renew()
	}

	var wg sync.WaitGroup
	for i := 0; i < 10; i++ {
		wg.Add(1)
		go func(i int) {
			defer wg.Done()
			if i == 5 {
				SingletonRenew()
			} else {
				SingletonNewSam()
			}
		}(i)
	}
	wg.Wait()

	if atomic.LoadInt64(&counter) != 2 {
		t.Errorf("expected counter to be 2, got %d", counter)
	}
}
